// SurrounDead Bridge – open-source UE4SS C++ mod
//
// Environment variables (all SDB_* – no name conflict with other mods):
//   SDB_GATEWAY_HOST     gateway hostname or IP  (default 127.0.0.1)
//   SDB_GATEWAY_PORT     gateway TCP port        (default 42200)
//   SDB_JOIN_TICKET      HMAC-signed join ticket (required)
//   SDB_MOVE_INTERVAL_MS Movement send interval in ms (default 50)

#include "protocol.hpp"
#include "tcp_client.hpp"
#include "state.hpp"
#include "proxy_manager.hpp"
#include "entity_manager.hpp"
#include "debug_log.hpp"

// UE4SS stub headers (derived from UE4SS.dll export table)
#include <RC/Mod/CppUserModBase.hpp>
#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/Hook/PreCallback.hpp>
#include <RC/DynamicOutput/Output.hpp>

#include <windows.h>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

using namespace RC;
using namespace RC::Unreal;

// ── Config ────────────────────────────────────────────────────────────────

static std::string cfg_gateway_host   = "game.ristl.org";
static uint16_t    cfg_gateway_port   = 31000;
static std::string cfg_join_ticket;
static int64_t     cfg_move_interval_us = 50'000; // 50 ms

// Read %APPDATA%\SurrounDeadBridge\session.cfg (KEY=VALUE per line).
// Falls back to environment variables with the same key names.
static std::unordered_map<std::string,std::string> load_session_config()
{
    std::unordered_map<std::string,std::string> m;

    wchar_t appdata[MAX_PATH];
    if (GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH)) {
        std::wstring path = std::wstring(appdata) + L"\\SurrounDeadBridge\\session.cfg";
        HANDLE h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
                               nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h != INVALID_HANDLE_VALUE) {
            std::string content;
            char buf[4096]; DWORD n;
            while (ReadFile(h, buf, sizeof(buf), &n, nullptr) && n > 0)
                content.append(buf, n);
            CloseHandle(h);

            size_t pos = 0;
            while (pos < content.size()) {
                size_t nl  = content.find('\n', pos);
                if (nl == std::string::npos) nl = content.size();
                std::string line = content.substr(pos, nl - pos);
                if (!line.empty() && line.back() == '\r') line.pop_back();
                pos = nl + 1;
                if (line.empty() || line[0] == '#') continue;
                size_t eq = line.find('=');
                if (eq == std::string::npos) continue;
                m[line.substr(0, eq)] = line.substr(eq + 1);
            }
        }
    }
    return m;
}

static std::string cfg_get(const std::unordered_map<std::string,std::string>& m,
                            const char* key)
{
    auto it = m.find(key);
    if (it != m.end() && !it->second.empty()) return it->second;
    const char* v = std::getenv(key);
    return v ? v : "";
}

static uint16_t cfg_u16(const std::unordered_map<std::string,std::string>& m,
                         const char* key, uint16_t def)
{
    const std::string s = cfg_get(m, key);
    if (s.empty()) return def;
    const int n = std::atoi(s.c_str());
    return (n > 0 && n < 65536) ? static_cast<uint16_t>(n) : def;
}

static int64_t cfg_ms_to_us(const std::unordered_map<std::string,std::string>& m,
                              const char* key, int64_t def_us)
{
    const std::string s = cfg_get(m, key);
    if (s.empty()) return def_us;
    const int n = std::atoi(s.c_str());
    return (n > 0 && n < 10000) ? static_cast<int64_t>(n) * 1000LL : def_us;
}

// ── Globals ───────────────────────────────────────────────────────────────

static sdb::TcpClient g_tcp;
static uint8_t        g_enc_buf[sdb::FRAME_HEADER_SIZE + sdb::FRAME_MAX_PAYLOAD];

// Character creation IPC paths — suffixed with this process's PID so two mod
// instances on the same machine (e.g. two Sandboxie-sandboxed game clients
// sharing a filesystem) don't race on the same request/done files. The C++
// side computes the PID and publishes it via an environment variable so the
// Lua-side character-creation script (mods/SDOnline/Scripts/main.lua, which
// runs in this same OS process) can build the matching paths independently —
// env vars set in a process are visible to everything running inside it.
static std::wstring CC_REQUEST_FLAG;
static std::wstring CC_DONE_FILE;

static void init_cc_ipc_paths()
{
    const DWORD pid = GetCurrentProcessId();
    const std::wstring pidStr = std::to_wstring(pid);
    CC_REQUEST_FLAG = L"C:\\temp\\SDB_cc_request_" + pidStr + L".flag";
    CC_DONE_FILE    = L"C:\\temp\\SDB_cc_done_" + pidStr + L".json";
    SetEnvironmentVariableW(L"SDB_CC_PID", pidStr.c_str());
}

// ── UE4SS helpers ─────────────────────────────────────────────────────────

static AActor* find_local_pawn()
{
    UObject* ctrl = UObjectGlobals::FindFirstOf(STR("BP_PlayerController_C"));
    if (ctrl) {
        for (const wchar_t* name : { STR("AcknowledgedPawn"), STR("Pawn") }) {
            auto** pp = static_cast<UObject**>(
                ctrl->GetValuePtrByPropertyNameInChain(name));
            if (pp && *pp) return static_cast<AActor*>(*pp);
        }
    }
    // Fallback: grab the character directly (safe when we're the only player).
    UObject* chr = UObjectGlobals::FindFirstOf(STR("BP_PlayerCharacter_C"));
    return static_cast<AActor*>(chr);
}

// ── Frame send helpers ────────────────────────────────────────────────────

static void send_frame(const sdb::Frame& f)
{
    auto& s = sdb::g_state();
    uint32_t seq  = s.seq.load(std::memory_order_relaxed);
    uint32_t tick = s.tick.load(std::memory_order_relaxed);
    const int n = sdb::encode_frame(g_enc_buf, sizeof(g_enc_buf), f, seq, tick);
    s.seq.store(seq,  std::memory_order_relaxed);
    s.tick.store(tick, std::memory_order_relaxed);
    if (n > 0) g_tcp.send_bytes(g_enc_buf, n);
}

static void build_session_frame(sdb::Frame& f)
{
    auto& st = sdb::g_state();
    std::lock_guard<std::mutex> lk(st.sessionMtx);
    f.sessionId    = st.session.sessionId;
    f.worldId      = st.session.worldId;
    f.connectionId = st.session.connectionId;
    f.playerId     = st.session.playerId;
    f.entityId     = st.session.entityId;
}

static void send_header_only(sdb::MsgType type)
{
    sdb::Frame f;
    f.type = type;
    build_session_frame(f);
    send_frame(f);
}

static void send_movement(AActor* pawn)
{
    const FVector  loc = pawn->K2_GetActorLocation();
    const FRotator rot = pawn->K2_GetActorRotation();

    sdb::Movement mv{};
    mv.x      = static_cast<float>(loc.X);
    mv.y      = static_cast<float>(loc.Y);
    mv.z      = static_cast<float>(loc.Z);
    mv.yaw    = static_cast<float>(rot.Yaw);
    mv.aimYaw = static_cast<float>(rot.Yaw);

    // Velocity: not exposed via the UE4SS stub, so read it directly.
    // ACharacter::CharacterMovement (pawn+0x328) -> UMovementComponent::Velocity (+0xB8).
    const auto pawnBase = reinterpret_cast<uintptr_t>(pawn);
    const auto moveComp = *reinterpret_cast<uintptr_t*>(pawnBase + 0x328);
    if (moveComp) {
        const double vx = *reinterpret_cast<double*>(moveComp + 0xB8);
        const double vy = *reinterpret_cast<double*>(moveComp + 0xC0);
        const double vz = *reinterpret_cast<double*>(moveComp + 0xC8);
        mv.velocityX = static_cast<float>(vx);
        mv.velocityY = static_cast<float>(vy);
        mv.velocityZ = static_cast<float>(vz);
    }

    uint8_t payload[sdb::MOVEMENT_PAYLOAD_SIZE];
    sdb::encode_movement(mv, payload);

    sdb::Frame f;
    f.type = sdb::MsgType::Movement;
    build_session_frame(f);
    f.payload.assign(payload, payload + sizeof(payload));
    send_frame(f);
}

// ── Native engine calls ───────────────────────────────────────────────────
//
// Raw calls into UE5 native functions by address, rebased at runtime against
// GetModuleHandle so this survives ASLR (the IDA session this was derived
// from has imagebase 0x140000000, which is very unlikely to match the
// runtime load address). This is the first gap fix in this file that needs a
// raw function-pointer call rather than a UE4SS stub method or a plain
// pointer-offset read — static-analysis only, NOT yet live-tested against a
// running game. See research/04_ida_investigation_log.md Session 34 before
// trusting resolved itemId strings.
namespace native {

static constexpr uintptr_t kIdaImageBase = 0x140000000ULL;

static uintptr_t rebase(uintptr_t ida_addr)
{
    static const uintptr_t exe_base =
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    return exe_base + (ida_addr - kIdaImageBase);
}

// UE FString is a TArray<TCHAR>: {TCHAR* Data; int32 Num; int32 Max}.
struct UnrealFString { wchar_t* data; int32_t num; int32_t max; };

using FNameToStringFn = void(*)(const void* fname, UnrealFString* out);
using FMemoryFreeFn   = void(*)(void* ptr);

// FName::ToString @ 0x140C9D940 (research Session 9) allocates its output
// buffer via GMalloc, so the result must be freed with FMemory::Free @
// 0x140B27000 (research Session 34, traced via FName::ToString ->
// FString::Reserve -> TArray::ResizeAllocation -> FMemory::Realloc ->
// GMalloc vtable+0x38, then FMemory::Free -> GMalloc vtable+0x48) or every
// call leaks.
static std::string fname_to_string(uintptr_t fnamePtr)
{
    static auto* toString = reinterpret_cast<FNameToStringFn>(rebase(0x140C9D940));
    static auto* memFree  = reinterpret_cast<FMemoryFreeFn>(rebase(0x140B27000));

    UnrealFString out{};
    toString(reinterpret_cast<const void*>(fnamePtr), &out);
    if (!out.data || out.num <= 0) return {};

    int len = out.num;
    if (out.data[len - 1] == L'\0') --len; // FString::Num includes the null terminator

    std::string s;
    if (len > 0) {
        const int needed = WideCharToMultiByte(CP_UTF8, 0, out.data, len, nullptr, 0, nullptr, nullptr);
        s.resize(static_cast<size_t>(needed));
        WideCharToMultiByte(CP_UTF8, 0, out.data, len, s.data(), needed, nullptr, nullptr);
    }
    memFree(out.data);
    return s;
}

// Reads an FString field already sitting in memory (e.g. a UPROPERTY member)
// in place — unlike fname_to_string, this doesn't call into the engine or
// allocate anything, so there's nothing to free; the engine owns that buffer
// for as long as the containing object exists.
static std::string read_fstring_field(uintptr_t addr)
{
    const auto& fstr = *reinterpret_cast<const UnrealFString*>(addr);
    if (!fstr.data || fstr.num <= 0) return {};

    int len = fstr.num;
    if (fstr.data[len - 1] == L'\0') --len;
    if (len <= 0) return {};

    const int needed = WideCharToMultiByte(CP_UTF8, 0, fstr.data, len, nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<size_t>(needed), '\0');
    WideCharToMultiByte(CP_UTF8, 0, fstr.data, len, s.data(), needed, nullptr, nullptr);
    return s;
}

} // namespace native

// ── Vitals reader ─────────────────────────────────────────────────────────

// Reads live game state from UE5 components using IDA-confirmed offsets.
// All pointer reads are null-guarded.
static sdb::LocalVitals read_local_progress(AActor* pawn)
{
    sdb::LocalVitals v{};
    const auto base = reinterpret_cast<uintptr_t>(pawn);

    auto read_obj = [](uintptr_t addr) -> uintptr_t {
        const auto ptr = *reinterpret_cast<uintptr_t*>(addr);
        return ptr;
    };
    auto read_double = [](uintptr_t addr) -> double {
        return *reinterpret_cast<double*>(addr);
    };
    auto read_int32 = [](uintptr_t addr) -> int32_t {
        return *reinterpret_cast<int32_t*>(addr);
    };

    // MedicalComponent (pawn+0x7D0) → health at +0xD0
    if (const uintptr_t med = read_obj(base + 0x7D0))
        v.health = read_double(med + 0xD0);

    // HungerThirstComponent (pawn+0x7F8) → hunger +0xC8, thirst +0xD8
    if (const uintptr_t ht = read_obj(base + 0x7F8)) {
        v.hunger = read_double(ht + 0xC8);
        v.thirst = read_double(ht + 0xD8);
    }

    // StaminaComponent (pawn+0x800) → stamina at +0xC8
    if (const uintptr_t stam = read_obj(base + 0x800))
        v.stamina = read_double(stam + 0xC8);

    // RadiationComponent (pawn+0x7F0) → radiation at +0xC8
    if (const uintptr_t rad = read_obj(base + 0x7F0))
        v.radiation = read_double(rad + 0xC8);

    // LevellingComponent (ctrl+0x868) → level +0xC0, xp +0xC8
    //
    // NOTE (pre-existing, not introduced by gap 4/7): FindFirstOf grabs
    // whichever BP_PlayerController_C instance happens to exist first, same
    // class of bug fixed for BP_JigHelperComp/BP_JigMultiplayer in Session
    // 35 (fixed there via a direct pawn-offset read instead). With more than
    // one player in the world this could read a different player's
    // controller. Left as-is here since a fix needs the pawn's "Controller"
    // property resolved and live-verified, which hasn't been done yet.
    UObject* ctrl = UObjectGlobals::FindFirstOf(STR("BP_PlayerController_C"));
    if (ctrl) {
        const uintptr_t ctrlBase = reinterpret_cast<uintptr_t>(ctrl);
        if (const uintptr_t lvl = read_obj(ctrlBase + 0x868)) {
            v.level = read_int32(lvl + 0xC0);
            v.xp    = read_double(lvl + 0xC8);
        }

        // Extended PlayerController stats (gap 4/7, offsets from research
        // Session 32).
        v.forename              = native::read_fstring_field(ctrlBase + 0x8C8);
        v.surname               = native::read_fstring_field(ctrlBase + 0x8D8);
        v.zombieKills           = read_int32(ctrlBase + 0x90C);
        v.daysSurvived          = read_int32(ctrlBase + 0x91C);
        v.bossZombieKills       = read_int32(ctrlBase + 0x910);
        v.animalKills           = read_int32(ctrlBase + 0x914);
        v.humanKills            = read_int32(ctrlBase + 0x918);
        v.distanceTravelled     = static_cast<float>(read_double(ctrlBase + 0x920));
        v.infestationsDestroyed = read_int32(ctrlBase + 0x928);
    }

    return v;
}

// ── Equipment reader ──────────────────────────────────────────────────────

// Walks BP_JigHelperComp_C.ServerEquippedItems (helper+0xF8, 21 slots of
// FRepItemInfo, each 0x78 bytes with ItemID at +0x00) using the full offset
// table from research/04_ida_investigation_log.md Session 30. Only occupied
// slots are emitted — dispatch_frame()/ProxyManager::on_equipment() does a
// full replace of RemotePlayer.equipment per frame, so an unequipped slot
// correctly disappears simply by being absent from the next frame.
//
// Live-confirmed Session 34 (resolved real item names, no crash across a
// full play session) — see research/04_ida_investigation_log.md.
static sdb::Equipment read_local_equipment(AActor* pawn)
{
    static constexpr uintptr_t kSlotOffsets[sdb::EQUIPMENT_SLOT_COUNT] = {
        0x000, 0x078, 0x0F0, 0x168, 0x1E0, 0x258, 0x2D0, 0x348, 0x3C0, 0x438,
        0x4B0, 0x528, 0x5A0, 0x618, 0x690, 0x708, 0x780, 0x7F8, 0x870, 0x8E8, 0x960,
    };

    sdb::Equipment eq;

    // BP_PlayerCharacter_C.BP_JigHelperComp is a named property at pawn+0x700
    // (research/CXXHeaderDump/BP_PlayerCharacter.hpp) — read it directly
    // rather than via FindFirstOf("BP_JigHelperComp_C"), which only happened
    // to return the right instance in solo testing; with more than one
    // player in the world it could just as easily return someone else's.
    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(pawn) + 0x700);
    if (!helper) return eq;

    const auto equipped = helper + 0xF8;

    for (uint8_t i = 0; i < sdb::EQUIPMENT_SLOT_COUNT; ++i) {
        const uintptr_t slot   = equipped + kSlotOffsets[i];
        const uintptr_t itemDA = *reinterpret_cast<uintptr_t*>(slot + 0x00);
        if (!itemDA) continue; // empty slot

        std::string itemId = native::fname_to_string(itemDA + 0x30);
        if (itemId.empty()) continue;

        sdb::EquipmentSlot es;
        es.slotIndex = i;
        es.itemId    = std::move(itemId);
        eq.slots.push_back(std::move(es));
    }

    return eq;
}

// Walks BP_JigMultiplayer_C.MainJigContainers (comp+0xA8, TArray of
// FS_ReplicatedContainerInfo, each 0x50 bytes) and returns each real
// container with its own Columns/Rows and item list, matching the
// per-container ProfileRevision wire format (protocol.hpp
// InventoryContainer/InventorySlot). Gap 11 (2026-08-10): the game's own
// inventory has no fixed slot count — each container is independently
// resizable at runtime (research/04_ida_investigation_log.md Session 29) —
// so this reads every container and every item in it, with no artificial cap.
//
// BP_PlayerCharacter_C.BP_JigMultiplayer is a named property at pawn+0x818
// (research/CXXHeaderDump/BP_PlayerCharacter.hpp) — same reasoning as
// read_local_equipment() for reading it directly instead of FindFirstOf.
//
// Live-confirmed Session 35 (as the earlier flat-list v1): correctly resolved
// a real, varied inventory (ammo, meds, currency, keycards, etc.) across
// multiple real containers with no crash.
static std::vector<sdb::InventoryContainer> read_local_inventory(AActor* pawn)
{
    std::vector<sdb::InventoryContainer> out;

    const uintptr_t jigMp = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(pawn) + 0x818);
    if (!jigMp) return out;

    // TArray<FS_ReplicatedContainerInfo> MainJigContainers @ +0xA8
    const uintptr_t containersData = *reinterpret_cast<uintptr_t*>(jigMp + 0xA8);
    const int32_t   containerCount = *reinterpret_cast<int32_t*>(jigMp + 0xA8 + 0x08);
    if (!containersData || containerCount <= 0) return out;

    constexpr size_t kContainerStride = 0x50;
    constexpr size_t kContainerItemsOffset = 0x40; // TArray<FContainerPickupsInfo>
    constexpr size_t kPickupStride = 0xD8;
    constexpr size_t kItemInfoOffset = 0x28;       // FRepItemInfo within FContainerPickupsInfo

    for (int32_t c = 0; c < containerCount; ++c) {
        const uintptr_t container = containersData + static_cast<size_t>(c) * kContainerStride;

        // Columns/Rows @ +0x20/+0x24. Live-tested Session 35: MainJigContainers
        // reserves one placeholder entry per equipment slot (21 of them,
        // Columns=Rows=-1, uninitialized/unused) ahead of the real storage
        // containers (backpack, secure container, etc., which have genuine
        // Columns/Rows >= 2). Skip anything <= 1x1 so those placeholders
        // don't get read as (empty) containers.
        const int32_t columns = *reinterpret_cast<int32_t*>(container + 0x20);
        const int32_t rows    = *reinterpret_cast<int32_t*>(container + 0x24);
        if (columns <= 1 && rows <= 1) continue;

        const uintptr_t itemsData = *reinterpret_cast<uintptr_t*>(container + kContainerItemsOffset);
        const int32_t   itemCount = *reinterpret_cast<int32_t*>(container + kContainerItemsOffset + 0x08);
        if (!itemsData || itemCount <= 0) continue;

        sdb::InventoryContainer bucket;
        bucket.columns = static_cast<uint16_t>(std::clamp(columns, 0, 65535));
        bucket.rows    = static_cast<uint16_t>(std::clamp(rows, 0, 65535));

        for (int32_t i = 0; i < itemCount; ++i) {
            const uintptr_t pickup   = itemsData + static_cast<size_t>(i) * kPickupStride;
            const uintptr_t itemInfo = pickup + kItemInfoOffset;

            const uintptr_t itemDA = *reinterpret_cast<uintptr_t*>(itemInfo + 0x00);
            if (!itemDA) continue;
            const int32_t count = *reinterpret_cast<int32_t*>(itemInfo + 0x08);

            std::string itemId = native::fname_to_string(itemDA + 0x30);
            if (itemId.empty()) continue;

            sdb::InventorySlot slot;
            slot.slotIndex = static_cast<uint8_t>(std::clamp<int32_t>(i, 0, 255));
            slot.itemId    = std::move(itemId);
            slot.quantity  = static_cast<uint16_t>(std::clamp(count, 0, 65535));
            bucket.items.push_back(std::move(slot));
        }

        if (!bucket.items.empty())
            out.push_back(std::move(bucket));
    }

    return out;
}

// ── Outbound senders ──────────────────────────────────────────────────────

// entityId must be the decimal string representation so JS can BigInt() it.
[[maybe_unused]] static void send_item_pickup_request(uint64_t entityId)
{
    const std::string rid = sdb::next_request_id();
    const std::string json =
        "{\"requestId\":\"" + rid + "\","
        "\"entityId\":\"" + std::to_string(entityId) + "\"}";

    sdb::Frame f;
    f.type = sdb::MsgType::ItemPickupRequest;
    build_session_frame(f);
    f.payload = sdb::encode_world_action(json);
    send_frame(f);
}

// classPath must start with /Game/Inventory/, /Game/JigSInventory/,
// /Game/Items/, or /Game/Blueprints/Items/ to pass host-agent validation.
[[maybe_unused]] static void send_item_drop_request(const std::string& itemId,
                                    const std::string& classPath,
                                    uint16_t quantity,
                                    float x, float y, float z, float yaw)
{
    const std::string rid = sdb::next_request_id();
    const std::string json =
        "{\"requestId\":\"" + rid + "\","
        "\"itemId\":\"" + itemId + "\","
        "\"classPath\":\"" + classPath + "\","
        "\"quantity\":" + std::to_string(quantity) + ","
        "\"x\":" + std::to_string(x) + ","
        "\"y\":" + std::to_string(y) + ","
        "\"z\":" + std::to_string(z) + ","
        "\"yaw\":" + std::to_string(yaw) + "}";

    sdb::Frame f;
    f.type = sdb::MsgType::ItemDropRequest;
    build_session_frame(f);
    f.payload = sdb::encode_world_action(json);
    send_frame(f);
}

static void send_character_create()
{
    auto& st = sdb::g_state();
    std::string forename, surname, sex, age;
    int occupation;
    {
        std::lock_guard<std::mutex> lk(st.sessionMtx);
        forename   = st.ccForename;
        surname    = st.ccSurname;
        sex        = st.ccSex;
        age        = st.ccAge;
        occupation = st.ccOccupation;
    }

    // Escape helper — only handles the chars that appear in names/ages.
    auto esc = [](const std::string& s) {
        std::string out;
        for (char c : s) {
            if (c == '"' || c == '\\') out += '\\';
            out += c;
        }
        return out;
    };

    std::string json =
        "{\"forename\":\"" + esc(forename) + "\""
        ",\"surname\":\""  + esc(surname)  + "\""
        ",\"sex\":\""      + esc(sex)      + "\""
        ",\"age\":\""      + esc(age)      + "\""
        ",\"occupation\":" + std::to_string(occupation) + "}";

    sdb::Frame f;
    f.type    = sdb::MsgType::CharacterCreate;
    f.payload = sdb::encode_world_action(json);
    build_session_frame(f);
    send_frame(f);

    Output::send<LogLevel::Normal>(STR("SDB: CharacterCreate sent\n"));
}

static void send_profile_revision(AActor* pawn)
{
    auto& st = sdb::g_state();

    const sdb::LocalVitals v = read_local_progress(pawn);

    const FVector  loc = pawn->K2_GetActorLocation();
    const FRotator rot = pawn->K2_GetActorRotation();

    sdb::PlayerProgress prog;
    prog.containers = read_local_inventory(pawn);

    {
        std::lock_guard<std::mutex> lk(st.inventoryMtx);
        prog.revision = ++st.progressRevision;
    }

    prog.health    = static_cast<float>(v.health);
    prog.hunger    = static_cast<float>(v.hunger);
    prog.thirst    = static_cast<float>(v.thirst);
    prog.stamina   = static_cast<float>(v.stamina);
    prog.radiation = static_cast<float>(v.radiation);
    prog.level     = v.level;
    prog.xp        = static_cast<float>(v.xp);
    prog.posX      = static_cast<float>(loc.X);
    prog.posY      = static_cast<float>(loc.Y);
    prog.posZ      = static_cast<float>(loc.Z);
    prog.yaw       = static_cast<float>(rot.Yaw);

    prog.forename              = v.forename;
    prog.surname               = v.surname;
    prog.zombieKills           = v.zombieKills;
    prog.daysSurvived          = v.daysSurvived;
    prog.bossZombieKills       = v.bossZombieKills;
    prog.animalKills           = v.animalKills;
    prog.humanKills            = v.humanKills;
    prog.distanceTravelled     = v.distanceTravelled;
    prog.infestationsDestroyed = v.infestationsDestroyed;

    auto buf = sdb::encode_player_progress(prog);
    if (buf.empty()) return;

    sdb::Frame f;
    f.type    = sdb::MsgType::ProfileRevision;
    f.payload = std::move(buf);
    build_session_frame(f);
    send_frame(f);

    // Cache vitals for any HUD reads
    st.localVitals = v;
}

static void send_equipment(AActor* pawn)
{
    const sdb::Equipment eq = read_local_equipment(pawn);

    sdb::Frame f;
    f.type    = sdb::MsgType::Equipment;
    f.payload = sdb::encode_equipment(eq);
    build_session_frame(f);
    send_frame(f);
}

// ── Incoming frame dispatcher ─────────────────────────────────────────────

static void dispatch_frame(const sdb::Frame& f)
{
    auto& st = sdb::g_state();

    // Latch session context from the first inbound frame.
    {
        std::lock_guard<std::mutex> lk(st.sessionMtx);
        if (!st.session.ready) {
            st.session.sessionId    = f.sessionId;
            st.session.worldId      = f.worldId;
            st.session.connectionId = f.connectionId;
            st.session.playerId     = f.playerId;
            st.session.entityId     = f.entityId;
            st.session.ready        = true;
            st.sessionLatchUs.store(sdb::now_micros(), std::memory_order_relaxed);
            Output::send<LogLevel::Normal>(
                STR("SDB: session latched, playerId={:d}\n"), f.playerId);
        }
    }

    switch (f.type) {

    case sdb::MsgType::PlayerConnected:
        if (f.entityId) {
            std::lock_guard<std::mutex> lk(st.sessionMtx);
            st.session.entityId = f.entityId;
        }
        if (f.playerId && f.playerId != st.session.playerId)
            sdb::g_proxy_manager().on_player_connected(f.playerId);
        break;

    case sdb::MsgType::PlayerDisconnected:
        sdb::g_proxy_manager().on_player_disconnected(f.playerId);
        break;

    case sdb::MsgType::Movement:
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto mv = sdb::decode_movement(f.payload.data(),
                                               static_cast<int>(f.payload.size())))
                sdb::g_proxy_manager().on_movement(f.playerId, *mv);
        }
        break;

    case sdb::MsgType::Equipment:
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto eq = sdb::decode_equipment(f.payload.data(), f.payload.size()))
                sdb::g_proxy_manager().on_equipment(f.playerId, *eq);
        }
        break;

    case sdb::MsgType::Death:
        sdb::g_proxy_manager().on_player_disconnected(f.playerId);
        sdb::g_proxy_manager().on_player_connected(f.playerId);
        {
            std::lock_guard<std::mutex> lk(st.playersMtx);
            auto it = st.players.find(f.playerId);
            if (it != st.players.end()) it->second.dead = true;
        }
        break;

    case sdb::MsgType::Respawn: {
        std::lock_guard<std::mutex> lk(st.playersMtx);
        auto it = st.players.find(f.playerId);
        if (it != st.players.end()) it->second.dead = false;
        break;
    }

    case sdb::MsgType::WorldState:
        if (auto ws = sdb::decode_world_state(f.payload.data(),
                                              static_cast<int>(f.payload.size()))) {
            std::lock_guard<std::mutex> lk(st.worldMtx);
            st.worldState      = *ws;
            st.worldStateValid = true;
        }
        break;

    case sdb::MsgType::PlayerDamage: {
        auto dmg = sdb::decode_player_damage(f.payload.data(),
                                             static_cast<int>(f.payload.size()));
        if (!dmg) break;
        // Write straight into MedicalComponent.Health — this frame carries the
        // server-authoritative current/max health, not a delta to apply.
        if (AActor* pawn = find_local_pawn()) {
            const auto base = reinterpret_cast<uintptr_t>(pawn);
            const auto med  = *reinterpret_cast<uintptr_t*>(base + 0x7D0);
            if (med) {
                *reinterpret_cast<double*>(med + 0xD0) = static_cast<double>(dmg->current);
                *reinterpret_cast<double*>(med + 0xD8) = static_cast<double>(dmg->maximum);
            }
        }
        Output::send<LogLevel::Normal>(
            STR("SDB: player damage applied  health={:.1f}/{:.1f}\n"),
            dmg->current, dmg->maximum);
        break;
    }

    // ── Entity lifecycle ──────────────────────────────────────────────────────

    case sdb::MsgType::EntitySpawn: {
        // JS sends encodeWorldEntityDescriptor (variable length, no position).
        auto desc = sdb::decode_entity_descriptor(f.payload.data(), f.payload.size());
        if (!desc) break;
        desc->entityId = f.entityId;
        sdb::g_entity_manager().on_entity_descriptor(*desc);
        break;
    }

    case sdb::MsgType::EntityState: {
        // JS sends encodeWorldEntityState (27 bytes, position + health).
        auto st_data = sdb::decode_entity_state(f.payload.data(), f.payload.size());
        if (!st_data) break;
        st_data->entityId = f.entityId;
        sdb::g_entity_manager().on_entity_state(f.entityId, *st_data);
        break;
    }

    case sdb::MsgType::EntityDespawn:
        sdb::g_entity_manager().on_entity_despawn(f.entityId);
        break;

    // ── Player progress restore ───────────────────────────────────────────────

    case sdb::MsgType::PlayerProgressRestore: {
        // Gateway replays the last-saved ProfileRevision payload verbatim, so
        // this must use decode_player_progress, not decode_movement — the
        // payload is the full PlayerProgress format (health/vitals/inventory
        // included), not the bare 39-byte Movement format.
        st.receivedProgressRestore.store(true, std::memory_order_relaxed);
        auto prog = sdb::decode_player_progress(f.payload.data(), f.payload.size());
        if (!prog) break;
        st.teleportX   = prog->posX;
        st.teleportY   = prog->posY;
        st.teleportZ   = prog->posZ;
        st.teleportYaw = prog->yaw;
        st.pendingTeleport.store(true, std::memory_order_release);
        Output::send<LogLevel::Normal>(
            STR("SDB: progress restored  x={:.1f} y={:.1f} z={:.1f}  health={:.2f} level={:d}\n"),
            prog->posX, prog->posY, prog->posZ, prog->health, prog->level);
        break;
    }

    // ── Item pickup result (JSON via encodeWorldAction) ───────────────────────

    case sdb::MsgType::ItemPickupResult: {
        auto json = sdb::decode_world_action(f.payload.data(), f.payload.size());
        if (!json) break;
        const bool ok = sdb::json_bool(*json, "success");
        if (ok) {
            Output::send<LogLevel::Normal>(STR("SDB: item pickup confirmed\n"));
        } else {
            Output::send<LogLevel::Warning>(STR("SDB: item pickup rejected\n"));
        }
        break;
    }

    // ── Item drop result (JSON via encodeWorldAction) ─────────────────────────

    case sdb::MsgType::ItemDropResult: {
        auto json = sdb::decode_world_action(f.payload.data(), f.payload.size());
        if (!json) break;
        const bool ok = sdb::json_bool(*json, "success");
        if (ok) {
            Output::send<LogLevel::Normal>(STR("SDB: item drop confirmed\n"));
        } else {
            Output::send<LogLevel::Warning>(STR("SDB: item drop rejected\n"));
        }
        break;
    }

    // ── Interaction result (JSON via encodeWorldAction) ───────────────────────

    case sdb::MsgType::InteractionResult: {
        auto json = sdb::decode_world_action(f.payload.data(), f.payload.size());
        if (!json) break;
        const bool ok = sdb::json_bool(*json, "success");
        if (ok) {
            Output::send<LogLevel::Normal>(STR("SDB: interaction confirmed\n"));
        } else {
            Output::send<LogLevel::Warning>(STR("SDB: interaction rejected\n"));
        }
        break;
    }

    default:
        break;
    }
}

// ── Engine tick ───────────────────────────────────────────────────────────

static std::atomic<uint64_t> g_last_move_us{0};
static std::atomic<uint64_t> g_last_tick_us{0};
static std::atomic<uint64_t> g_last_profile_us{0};
static std::atomic<uint64_t> g_last_equip_us{0};
static std::atomic<uint64_t> g_init_time_us{0};
static std::atomic<bool>     g_auto_open_fired{false};
static std::atomic<uint64_t> g_last_open_try_us{0};
static std::atomic<bool>     g_tcp_started{false};

// Local player's BP_JigHelperComp_C instance, refreshed every do_game_tick()
// call — used by on_process_event_pre's equip-trace diagnostic (temporary,
// see research/04_ida_investigation_log.md: figuring out what a real
// SetEquippedInfoBySlot call sequence actually looks like, since our own
// synthetic call succeeds (ok=1) but silently doesn't persist).
static std::atomic<uintptr_t> g_local_helper_ptr{0};

// Broadened equip-trace: while now_micros() < this, on_process_event_pre logs
// EVERY ProcessEvent call from ANY object (not just the helper component) —
// the narrower helper-only trace caught nothing during a real equip, meaning
// whatever handles it isn't reflected-called on that object at all. A short,
// externally-triggered unthrottled window (see check_trace_trigger below)
// keeps the log from exploding the way it would if this ran continuously.
static std::atomic<uint64_t> g_trace_until_us{0};

// Polled from do_game_tick() (already 5ms-throttled) for a flag file the
// user/operator creates right before performing the action to trace — same
// file-flag IPC pattern already used for character creation (init_cc_ipc_paths).
static void check_trace_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\trace_trigger.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    DeleteFileW(flag.c_str());
    g_trace_until_us.store(sdb::now_micros() + 5'000'000ULL, std::memory_order_relaxed);
    debug_log("check_trace_trigger: full trace window opened for 5s");
}

// One-shot diagnostic (temporary, see research/04_ida_investigation_log.md):
// find JSI_Slot_C's OnDrop UFunction via the same safe GetFunctionByNameInChain
// path already proven working (mod.cpp:847-style call), then dump raw struct
// bytes around it so the UFunction/UStruct::Script (TArray<uint8> bytecode)
// field's exact offset for this build can be identified empirically, without
// any live IDA debugging (pure pointer-arithmetic reads only).
// Flag file content: two lines, UTF-8, "<ClassName>\n<FunctionName>" — e.g.
// "JigSDragOperation_C\nDrop". Reusable across candidates without a rebuild.
// Script field confirmed live at UFunction+0x60 (TArray<uint8>: ptr@+0x60,
// Count@+0x68 low32, Max@+0x68 high32) — JSI_Slot_C::OnDrop's 33 bytes ended
// in the expected EX_EndOfScript (0x53) marker at exactly that length,
// confirming the offset. Only 0x14 (EX_LetBool) x2 + 0x04 (EX_Return) — a
// near-empty stub, meaning the real equip logic is not here.
static void check_bytecode_dump_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\bytecode_dump.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string classNameU8, funcNameU8;
    std::getline(in, classNameU8);
    std::getline(in, funcNameU8);
    in.close();
    DeleteFileW(flag.c_str());

    if (classNameU8.empty() || funcNameU8.empty()) {
        debug_log("bytecode_dump: flag file missing class/function name lines");
        return;
    }
    // strip trailing \r if present (file may have been written with CRLF)
    if (!classNameU8.empty() && classNameU8.back() == '\r') classNameU8.pop_back();
    if (!funcNameU8.empty() && funcNameU8.back() == '\r') funcNameU8.pop_back();

    auto widen = [](const std::string& s) {
        std::wstring w(s.size(), L'\0');
        int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), (int)w.size());
        w.resize(n);
        return w;
    };
    std::wstring className = widen(classNameU8);
    std::wstring funcName  = widen(funcNameU8);

    UObject* widget = UObjectGlobals::FindFirstOf(className.c_str());
    if (!widget) { debug_log("bytecode_dump: " + classNameU8 + " instance/CDO not found"); return; }

    char line[512];
    snprintf(line, sizeof(line), "bytecode_dump: found %s at 0x%llx", classNameU8.c_str(), (unsigned long long)(uintptr_t)widget);
    debug_log(line);

    UFunction* fn = widget->GetFunctionByNameInChain(funcName.c_str());
    if (!fn) { debug_log("bytecode_dump: " + funcNameU8 + " function not found"); return; }

    const uintptr_t base = reinterpret_cast<uintptr_t>(fn);
    const uintptr_t scriptPtr = *reinterpret_cast<const uintptr_t*>(base + 0x60);
    const int32_t   count     = *reinterpret_cast<const int32_t*>(base + 0x68);
    const int32_t   maxv      = *reinterpret_cast<const int32_t*>(base + 0x6C);
    snprintf(line, sizeof(line), "bytecode_dump: %s::%s UFunction*=0x%llx Script.Data=0x%llx Count=%d Max=%d",
             classNameU8.c_str(), funcNameU8.c_str(), (unsigned long long)base,
             (unsigned long long)scriptPtr, count, maxv);
    debug_log(line);

    if (scriptPtr && count > 0) {
        // Raw binary, not hex text — 9KB+ functions make a hex dump into a
        // single debug_log line impractical. Saved for offline disassembly
        // with a proper Kismet bytecode tool (EExprToken opcodes), not
        // intended to be hand-read from this file directly.
        wchar_t outDir[MAX_PATH];
        DWORD dn = GetEnvironmentVariableW(L"APPDATA", outDir, MAX_PATH);
        if (dn > 0 && dn < MAX_PATH) {
            // Some Blueprint-authored names contain characters Windows
            // forbids in filenames (e.g. "OnRep_PrimaryWeaponEquipped?") —
            // sanitize only the output filename, not the className/funcName
            // used for the actual UObject/UFunction lookups above.
            auto sanitize = [](std::wstring s) {
                for (auto& c : s) {
                    if (c == L'?' || c == L'*' || c == L':' || c == L'"' ||
                        c == L'<' || c == L'>' || c == L'|' || c == L'\\' || c == L'/')
                        c = L'_';
                }
                return s;
            };
            std::wstring outPath = std::wstring(outDir, dn) + L"\\SurrounDeadBridge\\" +
                sanitize(className) + L"_" + sanitize(funcName) + L".bin";
            std::ofstream out(outPath, std::ios::binary | std::ios::trunc);
            const bool opened = out.is_open();
            if (opened) out.write(reinterpret_cast<const char*>(scriptPtr), count);
            const bool wrote = opened && static_cast<bool>(out);
            out.close();
            snprintf(line, sizeof(line), "bytecode_dump: %s %d bytes to %s_%s.bin",
                     wrote ? "wrote" : "FAILED to write", count, classNameU8.c_str(), funcNameU8.c_str());
            debug_log(line);
        }
    }
}

// Flag file content: one line "<ComparisonIndex> <Number>" (decimal, space
// separated) — resolves an arbitrary raw FName pulled from hand-decoded
// Kismet bytecode (e.g. an EX_LocalVirtualFunction's FName operand) to its
// real string, via the same native FName::ToString path already proven safe
// (native::fname_to_string, used throughout read_local_equipment() etc.) —
// just constructs a temporary 8-byte FName-shaped struct on the stack instead
// of pointing at a live property, since the native call only needs a valid
// memory address with that layout.
static void check_resolve_fname_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\resolve_fname.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string lineU8;
    // One "<ci> <num>" pair per line — resolves the whole batch in one trigger.
    while (std::getline(in, lineU8)) {
        if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();
        if (lineU8.empty()) continue;

        int32_t ci = 0, num = 0;
        if (sscanf_s(lineU8.c_str(), "%d %d", &ci, &num) != 2) {
            debug_log("resolve_fname: could not parse '<ci> <num>' from line '" + lineU8 + "'");
            continue;
        }

        struct { int32_t ComparisonIndex; int32_t Number; } tempName{ ci, num };
        std::string name = native::fname_to_string(reinterpret_cast<uintptr_t>(&tempName));
        char line[256];
        snprintf(line, sizeof(line), "resolve_fname: ci=%d num=%d -> \"%s\"", ci, num, name.c_str());
        debug_log(line);
    }
    in.close();
    DeleteFileW(flag.c_str());
}

// __try/__except can't share a stack frame with C++ objects that need
// unwinding (MSVC C2712), so the guarded call is split into a plain function
// (free to use std::wstring/std::string) invoked through this trampoline,
// which itself declares nothing that needs a destructor.
static bool seh_invoke(void (*fn)(void*), void* ctx)
{
    __try {
        fn(ctx);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

struct ResolvePtrCtx {
    UObject* obj;
    std::string result;
};

static void do_resolve_ptr(void* ctxRaw)
{
    auto* ctx = static_cast<ResolvePtrCtx*>(ctxRaw);
    std::wstring wname = ctx->obj->GetFullName();
    const int needed = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string name(needed > 0 ? static_cast<size_t>(needed - 1) : 0, '\0');
    if (needed > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), needed, nullptr, nullptr);
    ctx->result = std::move(name);
}

// Flag file content: one line, hex pointer value (e.g. "0x247776a0") extracted
// from a hand-decoded Kismet bytecode dump (an EX_CallMath/EX_FinalFunction
// "func=" operand). NOTE: raw pointer values from an OLD bytecode_dump.flag
// .bin file are only valid within the SAME game process instance they were
// captured from — ASLR/heap layout differs across relaunches, so this only
// works run in the same session as the .bin file being analyzed, or against
// a freshly re-captured dump. GetFullName() works on any UObject-derived
// pointer, including UFunction*, since UFunction inherits UObject. A crash
// here (bad/stale pointer) is now caught via SEH instead of taking the whole
// game down — see seh_invoke above, added after two live crashes from
// resolving a stale dump's pointers post-relaunch (2026-08-10).
static void check_resolve_ptr_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\resolve_ptr.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string lineU8;
    // One hex address per line — resolves the whole batch in one trigger.
    while (std::getline(in, lineU8)) {
        if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();
        if (lineU8.empty()) continue;

        unsigned long long addr = 0;
        if (sscanf_s(lineU8.c_str(), "%llx", &addr) != 1 || addr == 0) {
            debug_log("resolve_ptr: could not parse hex address '" + lineU8 + "'");
            continue;
        }

        auto* obj = reinterpret_cast<UObject*>(static_cast<uintptr_t>(addr));
        ResolvePtrCtx ctx{ obj, {} };
        if (seh_invoke(&do_resolve_ptr, &ctx)) {
            debug_log("resolve_ptr: 0x" + lineU8 + " -> " + ctx.result);
        } else {
            debug_log("resolve_ptr: 0x" + lineU8 + " -> <access violation, not a live UObject here>");
        }
    }
    in.close();
    DeleteFileW(flag.c_str());
}

// Flag file content: one class name per line — one-off live diagnostic to
// find the class name of whatever widget/actor is currently on screen (e.g.
// the "press any key" splash), by trying FindFirstOf against each candidate
// name in turn. Same lookup FindFirstOf/GetFunctionByNameInChain approach
// already proven throughout this file (e.g. try_open_world's MenuWidget_C
// lookup below) rather than ForEachUObject, whose linker binding in this
// vendored stub turned out to be broken (name-mangling mismatch — the
// pragma in UObjectGlobals.hpp doesn't match what this build actually needs).
static void check_widget_scan_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\widget_scan.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string lineU8;
    while (std::getline(in, lineU8)) {
        if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();
        if (lineU8.empty()) continue;

        std::wstring wname(lineU8.size(), L'\0');
        int wn = MultiByteToWideChar(CP_UTF8, 0, lineU8.data(), static_cast<int>(lineU8.size()), wname.data(), static_cast<int>(wname.size()));
        wname.resize(wn);

        UObject* obj = UObjectGlobals::FindFirstOf(wname.c_str());
        debug_log("widget_scan: " + lineU8 + " -> " + (obj ? "FOUND" : "not found"));
    }
    in.close();
    DeleteFileW(flag.c_str());
}

// Flag file content: one line "<hex offset from g_local_helper_ptr> <count>"
// — raw diagnostic dump of <count> consecutive qwords starting at
// helper+offset, for manually eyeballing a live TMap's real layout (same
// empirical technique used earlier to find UFunction::Script's +0x60
// offset: dump raw memory, look for a plausible pattern, rather than
// trusting an assumed struct layout). Logs each qword both as a raw hex
// value and split into two int32 halves, since a TMap pair here is expected
// to look like {int32 SlotIndex, FGameplayTag{int32 ComparisonIndex, int32
// Number}} — a 12-byte struct straddling qword boundaries.
static void check_mem_dump_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\mem_dump.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string lineU8;
    std::getline(in, lineU8);
    in.close();
    DeleteFileW(flag.c_str());
    if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();

    // Optional "abs " prefix: treat the hex value as an absolute address
    // instead of a helper-relative offset (needed for following a pointer
    // read out of an earlier helper-relative dump, e.g. a TMap's own
    // separately-heap-allocated element storage).
    bool isAbs = false;
    const char* parseFrom = lineU8.c_str();
    if (lineU8.rfind("abs ", 0) == 0) { isAbs = true; parseFrom += 4; }

    unsigned long long offset = 0;
    int count = 0;
    if (sscanf_s(parseFrom, "%llx %d", &offset, &count) != 2 || count <= 0 || count > 256) {
        debug_log("mem_dump: could not parse '[abs ]<hex offset/addr> <count>' from '" + lineU8 + "'");
        return;
    }

    uintptr_t base;
    if (isAbs) {
        base = static_cast<uintptr_t>(offset);
    } else {
        const uintptr_t helper = g_local_helper_ptr.load(std::memory_order_relaxed);
        if (!helper) { debug_log("mem_dump: no local helper pointer yet"); return; }
        base = helper + static_cast<uintptr_t>(offset);
    }
    for (int i = 0; i < count; ++i) {
        struct DumpCtx { uintptr_t addr; uint64_t val; bool ok; };
        DumpCtx dctx{ base + static_cast<uintptr_t>(i) * 8, 0, false };
        auto dumpFn = [](void* raw) {
            auto* c = static_cast<DumpCtx*>(raw);
            c->val = *reinterpret_cast<const uint64_t*>(c->addr);
            c->ok = true;
        };
        if (!seh_invoke(dumpFn, &dctx)) {
            char buf[96];
            snprintf(buf, sizeof(buf), "mem_dump: [+0x%llx] <access violation>",
                     static_cast<unsigned long long>(offset) + static_cast<unsigned long long>(i) * 8);
            debug_log(buf);
            continue;
        }
        char buf[128];
        snprintf(buf, sizeof(buf), "mem_dump: [+0x%llx] qword=0x%016llx lo32=%d hi32=%d",
                 static_cast<unsigned long long>(offset) + static_cast<unsigned long long>(i) * 8,
                 dctx.val,
                 static_cast<int32_t>(dctx.val & 0xFFFFFFFFu),
                 static_cast<int32_t>(dctx.val >> 32));
        debug_log(buf);
    }
}

// Flag file content: two lines, class name then no-arg function name — a
// generic "find this class, call this zero-parameter function on it" live
// action trigger, for one-off manual pokes (e.g. RemoveFromParent on a
// stuck LoadingScreenWidget_C) without needing a new dedicated function and
// rebuild cycle for every new case.
static void check_call_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\call.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string classNameU8, funcNameU8;
    std::getline(in, classNameU8);
    std::getline(in, funcNameU8);
    in.close();
    DeleteFileW(flag.c_str());
    if (!classNameU8.empty() && classNameU8.back() == '\r') classNameU8.pop_back();
    if (!funcNameU8.empty() && funcNameU8.back() == '\r') funcNameU8.pop_back();
    if (classNameU8.empty() || funcNameU8.empty()) { debug_log("call: missing class/func line"); return; }

    auto widen = [](const std::string& s) {
        std::wstring w(s.size(), L'\0');
        int wn = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), w.data(), static_cast<int>(w.size()));
        w.resize(wn);
        return w;
    };

    UObject* obj = UObjectGlobals::FindFirstOf(widen(classNameU8).c_str());
    if (!obj) { debug_log("call: " + classNameU8 + " not found"); return; }

    UFunction* fn = obj->GetFunctionByNameInChain(widen(funcNameU8).c_str());
    if (!fn) { debug_log("call: " + funcNameU8 + " not found on " + classNameU8); return; }

    obj->ProcessEvent(fn, nullptr);
    debug_log("call: " + classNameU8 + "." + funcNameU8 + "() done");
}

// Returns true if Continue was clicked (or already in-world); false = retry later.
//
// NOTE (2026-08-11): tried auto-dismissing PressAnyKeyWidget_C (the splash
// overlay before this) twice this session, including a properly one-shot
// gated RemoveFromParent() call — still left the game stuck on the same
// screen. Dropped entirely per direct instruction; only handles the menu
// stage now. A real key press is required to get past the splash.
static bool try_open_world()
{
    if (find_local_pawn()) return true; // already in-world

    UObject* menu = UObjectGlobals::FindFirstOf(L"MenuWidget_C");
    if (!menu) return false;

    static bool s_logged = false;
    if (!s_logged) { s_logged = true;
        Output::send<LogLevel::Normal>(STR("SDB: MenuWidget found, clicking ContinueGame\n")); }

    UFunction* fn = menu->GetFunctionByNameInChain(
        L"BndEvt__MenuWidget_ContinueGame_K2Node_ComponentBoundEvent_25_OnButtonClickedEvent__DelegateSignature");
    if (fn) {
        menu->ProcessEvent(fn, nullptr);
        Output::send<LogLevel::Normal>(STR("SDB: ContinueGame clicked\n"));
        return true;
    }

    Output::send<LogLevel::Error>(STR("SDB: ContinueGame fn not found on MenuWidget\n"));
    return false;
}

// Core game-tick logic — called from both on_actor_tick and on_process_event_pre.
// Rate-limited to once per 5 ms via g_last_tick_us; safe because both callers
// are always on the game thread.
static void do_game_tick()
{
    const uint64_t now = sdb::now_micros();
    if (now - g_last_tick_us.load(std::memory_order_relaxed) < 5'000ULL) return;
    g_last_tick_us.store(now, std::memory_order_relaxed);

    check_trace_trigger();
    check_bytecode_dump_trigger();
    check_resolve_fname_trigger();
    check_resolve_ptr_trigger();
    check_widget_scan_trigger();
    check_call_trigger();
    check_mem_dump_trigger();

    // Lazy-connect: open TCP once a pawn exists (level fully loaded).
    if (!g_tcp.is_open()) {
        if (!g_tcp_started.load(std::memory_order_relaxed)
            && !cfg_join_ticket.empty()
            && find_local_pawn())
        {
            if (!g_tcp_started.exchange(true, std::memory_order_relaxed)) {
                Output::send<LogLevel::Normal>(
                    STR("[tcp] pawn found — connecting to gateway...\n"));
                g_tcp.open(cfg_gateway_host, cfg_gateway_port, cfg_join_ticket);
            }
        }
        return;
    }

    // 1. Drain receive buffer.
    for (const auto& f : g_tcp.recv_all())
        dispatch_frame(f);

    // 1b. New-player detection: if no PlayerProgressRestore arrives within 6s
    //     of session latch, this is a first join → trigger character creation.
    {
        auto& st2 = sdb::g_state();
        const uint64_t latch = st2.sessionLatchUs.load(std::memory_order_relaxed);
        if (latch != 0
            && !st2.receivedProgressRestore.load(std::memory_order_relaxed)
            && !st2.ccRequestWritten.load(std::memory_order_relaxed)
            && !st2.ccDone.load(std::memory_order_relaxed)
            && now - latch >= 6'000'000ULL)
        {
            st2.ccRequestWritten.store(true, std::memory_order_relaxed);
            // Write the flag file the Lua mod polls for.
            HANDLE h = CreateFileW(CC_REQUEST_FLAG.c_str(),
                GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
            Output::send<LogLevel::Normal>(
                STR("SDB: new player detected — character creation requested\n"));
        }
    }

    // 1c. Poll for character creation completion (Lua writes CC_DONE_FILE).
    {
        auto& st2 = sdb::g_state();
        if (st2.ccRequestWritten.load(std::memory_order_relaxed)
            && !st2.ccDone.load(std::memory_order_relaxed))
        {
            HANDLE h = CreateFileW(CC_DONE_FILE.c_str(),
                GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h != INVALID_HANDLE_VALUE) {
                char buf[2048] = {};
                DWORD n = 0;
                ReadFile(h, buf, sizeof(buf) - 1, &n, nullptr);
                CloseHandle(h);
                DeleteFileW(CC_DONE_FILE.c_str());

                // Parse character fields from done JSON.
                std::string json(buf, n);
                {
                    std::lock_guard<std::mutex> lk(st2.sessionMtx);
                    st2.ccForename   = sdb::json_str(json, "forename");
                    st2.ccSurname    = sdb::json_str(json, "surname");
                    st2.ccSex        = sdb::json_str(json, "sex");
                    st2.ccAge        = sdb::json_str(json, "age");
                    st2.ccOccupation = static_cast<int>(
                        sdb::json_double(json, "occupation"));
                }

                st2.ccDone.store(true, std::memory_order_relaxed);
                Output::send<LogLevel::Normal>(
                    STR("SDB: character creation complete\n"));

                send_character_create();
            }
        }
    }

    // 2. Find local pawn.
    auto& st    = sdb::g_state();
    AActor* pawn = find_local_pawn();

    if (pawn) {
        g_local_helper_ptr.store(*reinterpret_cast<uintptr_t*>(
            reinterpret_cast<uintptr_t>(pawn) + 0x700), std::memory_order_relaxed);
    }

    if (!pawn) {
        const bool had = st.hasPawn.exchange(false);
        if (had) st.noPlayerSinceUs.store(now);

        const uint64_t since = st.noPlayerSinceUs.load();
        if (since && !st.sentDeath.load()) {
            if (now - since > 1'000'000ULL) {
                send_header_only(sdb::MsgType::DeathRequest);
                st.sentDeath.store(true);
            }
        }

        sdb::g_proxy_manager().tick(nullptr, nullptr);
        return;
    }

    const bool was_dead = st.sentDeath.exchange(false);
    if (!st.hasPawn.exchange(true) || was_dead) {
        st.noPlayerSinceUs.store(0);
        if (was_dead) send_header_only(sdb::MsgType::RespawnRequest);
    }

    // 3. Apply pending teleport from PlayerProgressRestore.
    if (st.pendingTeleport.exchange(false, std::memory_order_acquire)) {
        const FVector  newLoc{ static_cast<double>(st.teleportX),
                                static_cast<double>(st.teleportY),
                                static_cast<double>(st.teleportZ) };
        const FRotator newRot{ 0.0, static_cast<double>(st.teleportYaw), 0.0 };
        FHitResult hit{};
        pawn->K2_SetActorLocationAndRotation(newLoc, newRot, false, hit, true);
        Output::send<LogLevel::Normal>(
            STR("SDB: teleported  x={:.1f} y={:.1f} z={:.1f}\n"),
            st.teleportX, st.teleportY, st.teleportZ);
    }

    // 4. Rate-limited movement send.
    if (now - g_last_move_us.load() >= static_cast<uint64_t>(cfg_move_interval_us)) {
        g_last_move_us.store(now);
        send_movement(pawn);
    }

    // 5. Drive proxy actors.
    UWorld* world = pawn->GetWorld();
    sdb::g_proxy_manager().tick(world, pawn);

    // 6. Drive world entities.
    sdb::g_entity_manager().tick(world, pawn);

    // 7. Periodic profile revision: push live vitals/position to server every 30 s.
    const uint64_t last_prof = g_last_profile_us.load(std::memory_order_relaxed);
    if (last_prof == 0 || now - last_prof >= 30'000'000ULL) {
        g_last_profile_us.store(now, std::memory_order_relaxed);
        send_profile_revision(pawn);
    }

    // 8. Periodic equipment sync: push loadout to other players every 2 s.
    // Polling, not hook-driven (see gap 8) — SetEquippedInfoBySlot is the
    // documented real hook point but this file has no per-UFunction hook
    // filtering infrastructure yet; polling 21 pointers is cheap enough.
    const uint64_t last_equip = g_last_equip_us.load(std::memory_order_relaxed);
    if (last_equip == 0 || now - last_equip >= 2'000'000ULL) {
        g_last_equip_us.store(now, std::memory_order_relaxed);
        send_equipment(pawn);
    }
}

// Fires on the game thread for every UObject::ProcessEvent call.
static void on_process_event_pre(UObject* obj, UFunction* func, void* /*params*/)
{
    // Equip-trace diagnostic (temporary, see research/04_ida_investigation_log.md):
    // our own synthetic SetEquippedInfoBySlot call reports success but never
    // persists, and a helper-only trace caught nothing during a real equip —
    // meaning whatever handles it isn't reflected-called on that object at
    // all. Broadened to log EVERY ProcessEvent call from ANY object, but only
    // during a short externally-triggered window (check_trace_trigger) so
    // this doesn't run continuously and flood the log the way an unthrottled
    // full trace would (research/04_ida_investigation_log.md: SDB.log hit
    // 106MB from an unrelated tight retry loop earlier this session).
    if (func && sdb::now_micros() < g_trace_until_us.load(std::memory_order_relaxed)) {
        std::wstring wname = reinterpret_cast<UObject*>(func)->GetFullName();
        const int needed = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string name(needed > 0 ? static_cast<size_t>(needed - 1) : 0, '\0');
        if (needed > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), needed, nullptr, nullptr);
        debug_log("full-trace: " + name);
    }

    // ProcessEvent fires thousands of times per frame; skip most calls so the
    // time-check (now_micros / QueryPerformanceCounter) isn't called every call.
    static uint32_t s_skip = 0;
    if (s_skip++ & 0xFFu) return; // let through 1 in 256 calls

    const uint64_t init_t = g_init_time_us.load(std::memory_order_relaxed);
    if (init_t == 0) return;
    const uint64_t now = sdb::now_micros();

    // Auto-open the world (click ContinueGame if still needed).
    if (!g_auto_open_fired.load(std::memory_order_relaxed)) {
        if (now - init_t >= 3'000'000ULL) {
            const uint64_t last = g_last_open_try_us.load(std::memory_order_relaxed);
            if (last == 0 || now - last >= 2'000'000ULL) {
                g_last_open_try_us.store(now, std::memory_order_relaxed);
                if (try_open_world())
                    g_auto_open_fired.store(true);
            }
        }
    }

    // Full game tick — runs here because on_actor_tick is not reliable after
    // level transitions in this version of UE4SS.
    do_game_tick();
}

// Fires per actor per frame; drives do_game_tick when on_actor_tick is available.
static void on_actor_tick(AActor* /*actor*/, float /*delta*/)
{
    do_game_tick();
}

// ── UE4SS 3.x mod class ───────────────────────────────────────────────────

class SDBMod : public RC::CppUserModBase {

public:
    SDBMod() { ModName = STR("SurrounDeadBridge"); }

    void on_unreal_init() override
    {
        debug_log("on_unreal_init: entered");

        init_cc_ipc_paths();
        auto sc = load_session_config();

        cfg_gateway_host     = cfg_get(sc, "SDB_GATEWAY_HOST");
        if (cfg_gateway_host.empty()) cfg_gateway_host = "game.ristl.org";
        cfg_gateway_port     = cfg_u16(sc, "SDB_GATEWAY_PORT", 31000);
        cfg_join_ticket      = cfg_get(sc, "SDB_JOIN_TICKET");
        cfg_move_interval_us = cfg_ms_to_us(sc, "SDB_MOVE_INTERVAL_MS", 50'000);

        debug_log("on_unreal_init: config loaded, gateway=" + cfg_gateway_host + ":" +
                   std::to_string(cfg_gateway_port) + " ticket_len=" + std::to_string(cfg_join_ticket.size()));

        Output::send<LogLevel::Normal>(
            STR("SDB: starting  gateway port={:d}  interval={:d}ms\n"),
            cfg_gateway_port,
            static_cast<int>(cfg_move_interval_us / 1000));

        debug_log("on_unreal_init: after first Output::send call");

        if (cfg_join_ticket.empty())
            Output::send<LogLevel::Warning>(
                STR("SDB: no join ticket — will not connect (run play.ps1 first)\n"));

        sdb::g_proxy_manager().init();
        sdb::g_entity_manager().init();

        auto* ue4ss = GetModuleHandleW(L"UE4SS.dll");

        using RegTick = void(*)(Hook::AActorTickFn);
        auto* fn_tick = ue4ss ? reinterpret_cast<RegTick>(GetProcAddress(ue4ss,
            "?RegisterAActorTickPreCallback@Hook@Unreal@RC@@YAXV?$function"
            "@$$A6AXPEAVAActor@Unreal@RC@@M@Z@std@@@Z")) : nullptr;
        if (fn_tick) fn_tick(on_actor_tick);
        else Output::send<LogLevel::Error>(STR("SDB: RegisterAActorTickPreCallback not found\n"));

        using RegPE = void(*)(std::function<void(UObject*, UFunction*, void*)>);
        auto* fn_pe = ue4ss ? reinterpret_cast<RegPE>(GetProcAddress(ue4ss,
            "?RegisterProcessEventPreCallback@Hook@Unreal@RC@@YAXV?$function"
            "@$$A6AXPEAVUObject@Unreal@RC@@PEAVUFunction@23@PEAX@Z@std@@@Z")) : nullptr;
        if (fn_pe) fn_pe(on_process_event_pre);
        else Output::send<LogLevel::Error>(STR("SDB: RegisterProcessEventPreCallback not found\n"));

        Output::send<LogLevel::Normal>(STR("SDB: ready\n"));
        g_init_time_us.store(sdb::now_micros());
        debug_log("on_unreal_init: complete, hooks registered");
    }

    void on_uninstall() override
    {
        g_tcp.shutdown();
        g_tcp_started.store(false, std::memory_order_relaxed);
        g_auto_open_fired.store(false, std::memory_order_relaxed);
        auto& st = sdb::g_state();
        st.sessionLatchUs.store(0, std::memory_order_relaxed);
        st.receivedProgressRestore.store(false, std::memory_order_relaxed);
        st.ccRequestWritten.store(false, std::memory_order_relaxed);
        st.ccDone.store(false, std::memory_order_relaxed);
        {
            std::lock_guard<std::mutex> lk(st.sessionMtx);
            st.ccForename.clear(); st.ccSurname.clear();
            st.ccSex.clear();      st.ccAge.clear();
            st.ccOccupation = 0;
        }
        Output::send<LogLevel::Normal>(STR("SDB: unloaded\n"));
    }
};

// ── Entry points (UE4SS 3.x API) ─────────────────────────────────────────

extern "C" __declspec(dllexport) RC::CppUserModBase* start_mod()
{
    return new SDBMod();
}

extern "C" __declspec(dllexport) void uninstall_mod(RC::CppUserModBase* mod)
{
    delete mod;
}
