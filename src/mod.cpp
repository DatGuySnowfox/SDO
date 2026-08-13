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
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

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

// Defined later in this file, next to read_local_equipment.
static uint8_t read_local_active_weapon_slot(AActor* pawn);
static uint8_t read_local_aim_pitch(AActor* pawn);

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

    // animationState was never populated or read anywhere in this project —
    // repurposed here to carry the active weapon slot (11-14, or 0xFF for
    // none) instead of adding a new wire field. See
    // read_local_active_weapon_slot's own comment for what this is verifying.
    mv.animationState = read_local_active_weapon_slot(pawn);

    // aimState was likewise never populated or read anywhere — repurposed to
    // carry a quantized look-pitch byte (see read_local_aim_pitch's own
    // comment for why pitch-only, and why this mirrors the engine's own
    // built-in ACharacter::RemoteViewPitch mechanism).
    mv.aimState = read_local_aim_pitch(pawn);

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
// Object paths (GetFullName()) always end "....PackagePath.ObjectName" — take
// the substring after the last '.' as the asset's own short, stable name
// (e.g. "Chr_MaleHair3"), matching the naming convention already used
// project-wide for itemId-based asset resolution. Empty string in/out means
// "no asset" (e.g. a null HairMesh/BeardMesh component's StaticMesh).
static std::string short_object_name(UObject* obj)
{
    if (!obj) return {};
    std::wstring wname = obj->GetFullName();
    const int need = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string name(need > 0 ? static_cast<size_t>(need - 1) : 0, '\0');
    if (need > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), need, nullptr, nullptr);
    const size_t dot = name.find_last_of('.');
    return dot == std::string::npos ? name : name.substr(dot + 1);
}

// Reads BP_PlayerCharacter_C's own character-creation fields — IsPlayerMale?
// (@0x15A0), HairMesh/BeardMesh components (@0x7C0/@0x7C8, each a plain
// UStaticMeshComponent whose current StaticMesh @+0x5B8 is the chosen hair/
// beard style), the "Hair Color"/"Beard Color" material instances
// (@0x15C8/@0x15D0), and SkinColor (@0x15A8) — all confirmed via
// research/CXXHeaderDump/BP_PlayerCharacter.hpp, not guessed. Unlike hair/
// beard (a single dedicated component each), skin color applies across many
// separate naked-body SkeletalMeshComponents (Arms/Torso/Legs/Feet/Hands/
// head/Biceps/LowerThighs/LowerLegs) — see proxy_manager.cpp's
// sync_pawn_appearance for the apply side.
static sdb::PawnAppearance read_local_pawn_appearance(AActor* pawn)
{
    sdb::PawnAppearance out;
    if (!pawn) return out;

    out.isMale = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(pawn) + 0x15A0);

    const uintptr_t hairMeshComp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x7C0);
    if (hairMeshComp) {
        auto* hairMesh = *reinterpret_cast<UObject**>(hairMeshComp + 0x5B8);
        out.hairMeshName = short_object_name(hairMesh);
    }
    auto* hairColor = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(pawn) + 0x15C8);
    out.hairColorName = short_object_name(hairColor);

    const uintptr_t beardMeshComp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x7C8);
    if (beardMeshComp) {
        auto* beardMesh = *reinterpret_cast<UObject**>(beardMeshComp + 0x5B8);
        out.beardMeshName = short_object_name(beardMesh);
    }
    auto* beardColor = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(pawn) + 0x15D0);
    out.beardColorName = short_object_name(beardColor);

    auto* skinColor = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(pawn) + 0x15A8);
    out.skinColorName = short_object_name(skinColor);

    // Naked-body SkeletalMeshComponents (BP_PlayerCharacter.hpp) — order
    // matches sdb::PawnAppearance::bodyPartMeshNames / proxy_manager.cpp's
    // own copy of this table exactly. Reading the actual assigned mesh
    // (SkinnedAsset @+0x5B8, same offset convention as HairMesh's
    // StaticMesh) rather than computing it from isMale + a naming
    // convention: the male variants aren't uniformly named (e.g. Biceps is
    // "SK_Chr_Underwear_Male_01_Biceps", not "SK_Chr_Male_Biceps").
    static constexpr uintptr_t kBodyPartOffsets[sdb::BODY_PART_COUNT] = {
        0x06B8, // Torso
        0x0710, // Biceps
        0x0718, // LowerThighs
        0x0778, // head
        0x0788, // Arms
        0x0798, // Feet
        0x07A0, // LowerLegs
        0x07A8, // Legs
        0x07B0, // Hands
    };
    for (int i = 0; i < sdb::BODY_PART_COUNT; ++i) {
        const uintptr_t comp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + kBodyPartOffsets[i]);
        if (!comp) continue;
        auto* mesh = *reinterpret_cast<UObject**>(comp + 0x5B8);
        out.bodyPartMeshNames[i] = short_object_name(mesh);
    }

    const uintptr_t mouthComp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x0740);
    if (mouthComp) out.mouthMeshName = short_object_name(*reinterpret_cast<UObject**>(mouthComp + 0x5B8));

    const uintptr_t eyebrowsComp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x0790);
    if (eyebrowsComp) out.eyebrowsMeshName = short_object_name(*reinterpret_cast<UObject**>(eyebrowsComp + 0x5B8));

    const uintptr_t acc1Comp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x0758);
    if (acc1Comp) out.accessory1MeshName = short_object_name(*reinterpret_cast<UObject**>(acc1Comp + 0x5B8));
    const uintptr_t acc2Comp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x0750);
    if (acc2Comp) out.accessory2MeshName = short_object_name(*reinterpret_cast<UObject**>(acc2Comp + 0x5B8));
    const uintptr_t acc3Comp = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x0748);
    if (acc3Comp) out.accessory3MeshName = short_object_name(*reinterpret_cast<UObject**>(acc3Comp + 0x5B8));

    return out;
}

// Mirrors proxy_manager.cpp's RawFGameplayTag — kept as a separate local
// struct since these two translation units don't share one (see that file's
// own copy for why).
struct RawFGameplayTag { int32_t ComparisonIndex = 0; int32_t Number = 0; };

// Reads BP_JigHelperComp_C.GetActiveWeaponSlot(FGameplayTag& ActiveWeapon) —
// the slot currently drawn/held in-hand, a distinct concept from
// SetEquippedInfoBySlot's per-slot item identity (research/CXXHeaderDump/
// BP_JigHelperComp.hpp). Returns 0xFF if no weapon slot is active.
//
// 2026-08-13: first tried hardcoding this tag family's ComparisonIndex
// values (mapped live once by cycling through all 4 slots: 1730633/48/64/19
// for Primary/Secondary/Sidearm/Melee). That worked within the same game
// session it was captured in, but on the next relaunch GetActiveWeaponSlot
// returned a completely different, unmapped CI (1730553) for the exact same
// held weapon — this specific "Jig.PlayerSlot.*" tag family's
// ComparisonIndex evidently isn't stable across process restarts (unlike
// kSlotTagComparisonIndex's equipment-identity tags in proxy_manager.cpp,
// which HAVE held up across many restarts all last session — apparently a
// different registration path). Resolve the tag's real string name instead,
// the same FName::ToString call itemId strings already go through
// elsewhere in this file — RawFGameplayTag/FName share the same
// {ComparisonIndex,Number} layout (already relied on by
// proxy_manager.cpp's backpack-socket re-attach), so its address can be
// passed straight in.
static uint8_t read_local_active_weapon_slot(AActor* pawn)
{
    if (!pawn) return 0xFF;
    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(pawn) + 0x700);
    if (!helper) return 0xFF;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"GetActiveWeaponSlot");
    if (!fn) {
        debug_log("read_local_active_weapon_slot: GetActiveWeaponSlot NOT FOUND");
        return 0xFF;
    }

    struct Params { RawFGameplayTag ActiveWeapon; } params;
    static_assert(sizeof(RawFGameplayTag) == 8, "FGameplayTag Kismet param size");
    helperObj->ProcessEvent(fn, &params);

    if (params.ActiveWeapon.ComparisonIndex == 0) return 0xFF; // no active weapon

    const std::string tagName = native::fname_to_string(
        reinterpret_cast<uintptr_t>(&params.ActiveWeapon));

    char buf[160];
    snprintf(buf, sizeof(buf), "read_local_active_weapon_slot: ci=%d tag=\"%s\"",
             params.ActiveWeapon.ComparisonIndex, tagName.c_str());
    debug_log(buf);

    // FModel's export of BP_JigHelperComp.json listed this tag family as
    // Jig.PlayerSlot.Primary/Secondary/Pistol/Melee, but live-tested
    // 2026-08-13: GetActiveWeaponSlot() actually returns
    // "Jig.PlayerSlot.SidearmWeapon" for the real sidearm slot at runtime —
    // the static export's tag list apparently isn't the exact string this
    // function returns. Matched on "Sidearm" (a substring of
    // "SidearmWeapon") based on the live value, not the export.
    if (tagName.find("Primary") != std::string::npos)   return 11;
    if (tagName.find("Secondary") != std::string::npos) return 12;
    if (tagName.find("Sidearm") != std::string::npos)   return 13;
    if (tagName.find("Melee") != std::string::npos)     return 14;
    return 0xFF;
}

// Look-direction sync (2026-08-13): only Pitch is transmitted, not a
// separate aim Yaw. Reasoning: mv.yaw (body rotation) already drives the
// proxy's own actor rotation every tick (see ProxyManager::tick's
// teleport_proxy call), and this game's own AnimBP has no engine-exposed
// remote-yaw mechanism to feed even if we wanted one — but it DOES have
// ACharacter's built-in RemoteViewPitch byte, the exact stock-engine
// mechanism third-person games use to replicate camera pitch to *other*
// clients' AnimBPs for spine/head aim-offset posing on simulated proxies.
// Mirrors the Velocity-not-Speed lesson from tonight's animation-sync work:
// feed the upstream input the engine's own per-frame AimOffset computation
// already reads (GetBaseAimRotation() -> RemoteViewPitch for non-locally-
// controlled pawns), not a derived/scratch AnimBP variable that would just
// get recomputed and overwritten.
//
// GetControlRotation() is a real BlueprintCallable UFUNCTION on APawn
// (calls through to Controller->GetControlRotation()), reachable via the
// same GetFunctionByNameInChain/ProcessEvent reflection pattern already
// used above — always valid here since this is called on our own locally-
// controlled pawn, which always has a live Controller.
static uint8_t read_local_aim_pitch(AActor* pawn)
{
    if (!pawn) return 0;

    UFunction* fn = pawn->GetFunctionByNameInChain(L"GetControlRotation");
    if (!fn) {
        debug_log("read_local_aim_pitch: GetControlRotation NOT FOUND");
        return 0;
    }

    struct Params { FRotator ReturnValue; } params{};
    pawn->ProcessEvent(fn, &params);

    // Same compression UE's own RemoteViewPitch setter uses internally
    // (FRotator::CompressAxisToByte): wrap to [0,360) then scale to a byte.
    double pitch = params.ReturnValue.Pitch;
    pitch = std::fmod(pitch, 360.0);
    if (pitch < 0.0) pitch += 360.0;
    const uint8_t quantized = static_cast<uint8_t>(pitch * (256.0 / 360.0));

    // Temporary diagnostic (2026-08-13): confirm the sender is actually
    // seeing a live, varying camera pitch before trusting the receive side.
    // Throttled to ~1/sec so it doesn't flood debug.log at movement-tick rate.
    static uint64_t s_lastLogUs = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (nowUs - s_lastLogUs > 1'000'000ULL) {
        s_lastLogUs = nowUs;
        char buf[128];
        snprintf(buf, sizeof(buf), "read_local_aim_pitch: rawPitch=%.2f quantized=%u",
                 params.ReturnValue.Pitch, quantized);
        debug_log(buf);
    }

    return quantized;
}

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

// Reads BP_JigPickupComponent_C.RepAttachments (FS_RepWeaponAttachment,
// research/CXXHeaderDump/S_RepWeaponAttachment.hpp +
// S_RepAttachmentInfo.hpp) off each of the local player's equipped actors
// that the proxy side can actually render an attachment onto — i.e. every
// slot proxy_manager.cpp spawns a real visual actor for (spawn_and_equip_item_visual):
// Facewear/Headwear/Eyewear/Backpack and the 4 weapon slots. This isn't
// weapon-specific at all — night vision on a helmet or a battery in a
// flashlight uses the identical BP_JigPickupComponent/RepAttachments
// mechanism as a weapon scope, just a different owning slot. Clothing slots
// (Torso/Gloves/Legs/Feet/BodyArmor) use the separate mesh-swap path with no
// spawned actor at all, so there's nothing to attach onto there — skipped.
// BP_JigPickupComponent lives at pickup-actor+0x320 (research/CXXHeaderDump/
// BP_SkeletalMeshPickup.hpp), matching the raw direct-offset style already
// used throughout this function rather than a reflection getter. Only the
// attachment's own DataAsset ItemID + container index are extracted — UID/
// Stats/ActivateState aren't needed for the proxy-side visual sync this feeds.
// GetEquippedActorBySlot was live-tested 2026-08-12 and found to return null
// for *every* slot, including ones definitely, visibly equipped (the AK15
// scan below proved a real actor genuinely exists and is genuinely
// AttachParent'd to the character's own Mesh — this getter just isn't the
// right way to find it). Walk USceneComponent::AttachChildren (Engine.hpp
// @0x00C0) on the character's own Mesh directly instead — every actor
// actually attached to the character (weapon, facewear, backpack, etc., all
// attached via K2_AttachToComponent onto Mesh per this project's own
// equip-visual work) shows up as an entry here, no per-slot lookup or class
// name needed at all. Each attached actor is then matched back to a slot by
// comparing its own BP_JigPickupComponent.ItemDataAsset's itemId against the
// already-read Equipment slot list.
// Defined later in this file — forward-declared here so the SEH-guarded scan
// below (which needs it) can come first, keeping read_local_weapon_attachments
// next to read_local_equipment.
static bool seh_invoke(void (*fn)(void*), void* ctx);

struct WeaponAttachScanCtx {
    UObject* mesh;
    const std::unordered_map<std::string, uint8_t>* itemIdToSlot;
    sdb::WeaponAttachments* out;
};

// The actual AttachChildren walk, split out from read_local_weapon_attachments
// so it can run under an SEH guard (see that function's own comment on why:
// live-tested 2026-08-12, crashed twice with an identical
// EXCEPTION_ACCESS_VIOLATION reading 0x9006, both times cut off mid-iteration
// near the end of the AttachChildren array — a native equip/unequip action
// (removing an attachment from a helmet) most likely destroyed/detached one
// of these components while this scan was concurrently reading through the
// same array, leaving a stale pointer this code then dereferenced. __try/
// __except can't share a stack frame with C++ objects needing unwinding
// (MSVC C2712), hence the trampoline split — same pattern as this file's own
// seh_invoke/destroy_actor_safe uses elsewhere.
static void do_weapon_attach_scan(void* ctxRaw)
{
    auto* ctx = static_cast<WeaponAttachScanCtx*>(ctxRaw);
    UObject* mesh = ctx->mesh;
    const auto& itemIdToSlot = *ctx->itemIdToSlot;
    sdb::WeaponAttachments& out = *ctx->out;

    const uintptr_t childrenData  = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(mesh) + 0x00C0);
    int32_t         childrenCount = *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(mesh) + 0x00C0 + 0x08);
    debug_log("read_local_weapon_attachments: Mesh AttachChildren count=" + std::to_string(childrenCount));
    if (!childrenData || childrenCount <= 0) return;
    // Sanity cap: a genuine character never has more than a couple dozen
    // things attached. If a native equip/unequip mutates this TArray mid-read
    // (the same race that caused the earlier crash), the count field could
    // transiently read as a huge garbage value, turning this loop into an
    // effective hang grinding through billions of bogus entries — SEH
    // doesn't catch that (it's not an exception). Clamp defensively.
    if (childrenCount > 64) {
        debug_log("read_local_weapon_attachments: AttachChildren count=" + std::to_string(childrenCount) +
                  " implausible, clamping to 0 (likely mid-mutation read)");
        return;
    }

    for (int32_t c = 0; c < childrenCount; ++c) {
        UObject* childComp = *reinterpret_cast<UObject**>(childrenData + static_cast<size_t>(c) * 8);
        if (!childComp) continue;

        UFunction* getOwnerFn = childComp->GetFunctionByNameInChain(L"GetOwner");
        AActor* owner = nullptr;
        if (getOwnerFn) childComp->ProcessEvent(getOwnerFn, &owner);
        if (!owner) {
            debug_log("read_local_weapon_attachments: child[" + std::to_string(c) + "] no owner (getOwnerFn found=" +
                      std::to_string(getOwnerFn != nullptr) + ")");
            continue;
        }

        std::string ownerName;
        {
            std::wstring wname = owner->GetFullName();
            int need = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
            ownerName.resize(need > 0 ? (size_t)(need - 1) : 0);
            if (need > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, ownerName.data(), need, nullptr, nullptr);
        }

        // BP_JigPickupComponent_C's offset differs by pickup base class
        // (weapons/melee extend ABP_SkeletalMeshPickup_C @0x320, while
        // Facewear/Headwear/Eyewear/Backpack extend ABP_StaticMeshPickup_C
        // @0x2B0 instead — confirmed via real UE4SS header dumps, not
        // guessed; see research/CXXHeaderDump/BP_StaticMeshPickup.hpp vs.
        // BP_SkeletalMeshPickup.hpp). Rather than hardcode both, use the
        // same reflection-based, layout-agnostic lookup
        // proxy_manager.cpp's set_pickup_item_data() already uses
        // successfully for the exact same field on both pickup families.
        auto** pickupCompSlot = static_cast<UObject**>(
            owner->GetValuePtrByPropertyNameInChain(L"BP_JigPickupComponent"));
        const uintptr_t pickupComp = (pickupCompSlot && *pickupCompSlot)
            ? reinterpret_cast<uintptr_t>(*pickupCompSlot) : 0;
        if (!pickupComp) {
            debug_log("read_local_weapon_attachments: child[" + std::to_string(c) + "] owner=" + ownerName + " no BP_JigPickupComponent");
            continue;
        }

        // Identify which equipped slot this attached actor belongs to by
        // matching its own ItemDataAsset (BP_JigPickupComponent_C.ItemDataAsset
        // @0x00A8) against the already-read Equipment list.
        const uintptr_t ownItemDA = *reinterpret_cast<uintptr_t*>(pickupComp + 0x00A8);
        if (!ownItemDA) {
            debug_log("read_local_weapon_attachments: child[" + std::to_string(c) + "] owner=" + ownerName + " ItemDataAsset null");
            continue;
        }
        std::string ownItemId = native::fname_to_string(ownItemDA + 0x30);
        auto slotIt = itemIdToSlot.find(ownItemId);
        if (slotIt == itemIdToSlot.end()) {
            debug_log("read_local_weapon_attachments: child[" + std::to_string(c) + "] owner=" + ownerName +
                      " itemId=" + ownItemId + " no matching equipped slot");
            continue;
        }
        const uint8_t slotIndex = slotIt->second;

        // FS_RepWeaponAttachment RepAttachments @0x0110 on BP_JigPickupComponent_C:
        // FGuid MainUID @+0x00 (unused here), TArray<FS_RepAttachmentInfo> Attachments @+0x10.
        const uintptr_t repAttachments = pickupComp + 0x0110;
        const uintptr_t arrayData  = *reinterpret_cast<uintptr_t*>(repAttachments + 0x10 + 0x00);
        const int32_t   arrayCount = *reinterpret_cast<int32_t*>(repAttachments + 0x10 + 0x08);
        debug_log("read_local_weapon_attachments: slot=" + std::to_string(slotIndex) +
                  " itemId=" + ownItemId + " arrayCount=" + std::to_string(arrayCount));
        if (!arrayData || arrayCount <= 0) continue;

        // FS_RepAttachmentInfo, 0x48 bytes each: AttachmentID (DataAsset*)
        // @0x00, AttachmentContainerIndex (int32) @0x18.
        constexpr size_t kStride = 0x48;
        for (int32_t i = 0; i < arrayCount; ++i) {
            const uintptr_t entry = arrayData + static_cast<size_t>(i) * kStride;
            const uintptr_t attachmentDA = *reinterpret_cast<uintptr_t*>(entry + 0x00);
            if (!attachmentDA) continue;

            std::string attItemId = native::fname_to_string(attachmentDA + 0x30);
            if (attItemId.empty()) continue;

            sdb::WeaponAttachmentEntry wae;
            wae.weaponSlotIndex = slotIndex;
            wae.containerIndex  = static_cast<uint8_t>(*reinterpret_cast<int32_t*>(entry + 0x18));
            wae.itemId          = std::move(attItemId);
            out.entries.push_back(std::move(wae));
        }
    }
}

static sdb::WeaponAttachments read_local_weapon_attachments(AActor* pawn)
{
    sdb::WeaponAttachments out;
    if (!pawn) return out;

    static constexpr uint8_t kAttachableSlots[] = {0, 1, 2, 10, 11, 12, 13, 14};
    const sdb::Equipment eq = read_local_equipment(pawn);
    std::unordered_map<std::string, uint8_t> itemIdToSlot;
    for (const auto& slot : eq.slots) {
        for (uint8_t s : kAttachableSlots) {
            if (slot.slotIndex == s) { itemIdToSlot[slot.itemId] = s; break; }
        }
    }
    {
        std::string line = "read_local_weapon_attachments: itemIdToSlot:";
        for (const auto& [id, s] : itemIdToSlot)
            line += " [" + std::to_string(s) + "]=" + id;
        debug_log(line);
    }

    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return out;

    WeaponAttachScanCtx ctx{ mesh, &itemIdToSlot, &out };
    if (!seh_invoke(&do_weapon_attach_scan, &ctx)) {
        debug_log("read_local_weapon_attachments: SEH caught a crash mid-scan "
                  "(stale pointer, likely a concurrent native equip/unequip change) "
                  "— discarding this cycle's partial result");
        out.entries.clear();
    }
    return out;
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

// Wire format matches server's decodeItemPickupRequest exactly:
// [version=1][targetSlot=0xFF (auto-assign)] — entityId is a frame-header
// field (f.entityId), not part of the payload; the server looks the entity
// up via f.entityId directly. (The previous JSON-via-encode_world_action
// version didn't match this at all — same category of bug ItemDropRequest
// had.)
static void send_item_pickup_request(uint64_t entityId)
{
    sdb::Frame f;
    f.type = sdb::MsgType::ItemPickupRequest;
    build_session_frame(f);
    f.entityId = entityId;
    const uint8_t payload[2] = { 1, 0xFF };
    f.payload.assign(payload, payload + 2);
    send_frame(f);
}

static void send_item_drop_request(const std::string& itemId, uint16_t quantity,
                                    float x, float y, float z)
{
    sdb::Frame f;
    f.type = sdb::MsgType::ItemDropRequest;
    build_session_frame(f);
    f.payload = sdb::encode_item_drop_request(itemId, quantity, x, y, z);
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

static void send_weapon_attachments(AActor* pawn)
{
    const sdb::WeaponAttachments wa = read_local_weapon_attachments(pawn);

    {
        std::string line = "send_weapon_attachments: entries=" + std::to_string(wa.entries.size());
        for (const auto& e : wa.entries) {
            line += " [slot=" + std::to_string(e.weaponSlotIndex) +
                    " container=" + std::to_string(e.containerIndex) +
                    " itemId=" + e.itemId + "]";
        }
        debug_log(line);
    }

    sdb::Frame f;
    f.type    = sdb::MsgType::WeaponAttachments;
    f.payload = sdb::encode_weapon_attachments(wa);
    build_session_frame(f);
    send_frame(f);
}

static void send_pawn_appearance(AActor* pawn)
{
    const sdb::PawnAppearance pa = read_local_pawn_appearance(pawn);

    {
        std::string line = "send_pawn_appearance: isMale=" + std::to_string(pa.isMale) +
              " hairMesh=" + pa.hairMeshName + " hairColor=" + pa.hairColorName +
              " beardMesh=" + pa.beardMeshName + " beardColor=" + pa.beardColorName +
              " skinColor=" + pa.skinColorName + " bodyParts=[";
        for (const auto& s : pa.bodyPartMeshNames) line += s + ",";
        line += "]";
        debug_log(line);
    }

    sdb::Frame f;
    f.type    = sdb::MsgType::PawnAppearance;
    f.payload = sdb::encode_pawn_appearance(pa);
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

    case sdb::MsgType::WeaponAttachments:
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto wa = sdb::decode_weapon_attachments(f.payload.data(), f.payload.size()))
                sdb::g_proxy_manager().on_weapon_attachments(f.playerId, *wa);
        }
        break;

    case sdb::MsgType::PawnAppearance:
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto pa = sdb::decode_pawn_appearance(f.payload.data(), f.payload.size()))
                sdb::g_proxy_manager().on_pawn_appearance(f.playerId, *pa);
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
    g_trace_until_us.store(sdb::now_micros() + 20'000'000ULL, std::memory_order_relaxed);
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

    // "abs <hex address>" on the first line: use a raw live UObject* pointer
    // directly (e.g. one already logged elsewhere, like spawn_and_attach_
    // weapon_visual's spawnedPtr) instead of FindFirstOf — needed when
    // FindFirstOf can't reliably locate a specific attached/spawned actor
    // instance among possibly many of the same class.
    UObject* widget = nullptr;
    if (classNameU8.rfind("abs ", 0) == 0) {
        unsigned long long addr = 0;
        if (sscanf_s(classNameU8.c_str() + 4, "%llx", &addr) != 1 || addr == 0) {
            debug_log("bytecode_dump: could not parse 'abs <hex address>' from '" + classNameU8 + "'");
            return;
        }
        widget = reinterpret_cast<UObject*>(static_cast<uintptr_t>(addr));
    } else {
        widget = UObjectGlobals::FindFirstOf(className.c_str());
    }
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

// Flag file's single line is the target UDataTable's own object name, e.g.
// "DT_Clothing" — see sdb::dump_clothing_table in proxy_manager.cpp.
static void check_dump_clothing_table_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\dump_clothing_table.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string tableNameU8;
    std::getline(in, tableNameU8);
    in.close();
    DeleteFileW(flag.c_str());

    if (!tableNameU8.empty() && tableNameU8.back() == '\r') tableNameU8.pop_back();
    if (tableNameU8.empty()) {
        debug_log("dump_clothing_table: flag file missing table name line");
        return;
    }

    std::wstring tableName(tableNameU8.size(), L'\0');
    int wn = MultiByteToWideChar(CP_UTF8, 0, tableNameU8.data(), (int)tableNameU8.size(),
                                  tableName.data(), (int)tableName.size());
    tableName.resize(wn);

    sdb::dump_clothing_table(tableName.c_str());
}

// One-off diagnostic: flag file content is a class name (e.g.
// "BP_AK15Pickup_C"). FindAllOf every live instance and log its own
// AttachParent (root component +0xB0) alongside the local player's own Mesh
// pointer, to determine whether a real, persistent equipped-weapon actor
// exists at all for the local player (GetEquippedActorBySlot returned null
// for every slot live-tested 2026-08-12, contradicting the assumption it
// tracks one) — and if one does exist, find it by attachment relationship
// instead of by the apparently-unreliable per-slot actor array.
static void check_scan_pickup_class_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\scan_pickup_class.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string classNameU8;
    std::getline(in, classNameU8);
    in.close();
    DeleteFileW(flag.c_str());
    if (!classNameU8.empty() && classNameU8.back() == '\r') classNameU8.pop_back();
    if (classNameU8.empty()) { debug_log("scan_pickup_class: flag file missing class name"); return; }

    std::wstring className(classNameU8.size(), L'\0');
    int wn = MultiByteToWideChar(CP_UTF8, 0, classNameU8.data(), (int)classNameU8.size(),
                                  className.data(), (int)className.size());
    className.resize(wn);

    AActor* pawn = find_local_pawn();
    if (pawn) {
        auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
        UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
        char buf[64];
        snprintf(buf, sizeof(buf), "scan_pickup_class: local pawn=0x%llx Mesh=0x%llx",
                 (unsigned long long)(uintptr_t)pawn, (unsigned long long)(uintptr_t)mesh);
        debug_log(buf);
    } else {
        debug_log("scan_pickup_class: find_local_pawn() returned null");
    }

    std::vector<UObject*> instances;
    UObjectGlobals::FindAllOf(className.c_str(), instances);
    debug_log("scan_pickup_class: " + classNameU8 + " instances=" + std::to_string(instances.size()));

    for (UObject* inst : instances) {
        auto* actor = static_cast<AActor*>(inst);
        UFunction* rootFn = actor->GetFunctionByNameInChain(L"GetSkeletalMeshComponent");
        if (!rootFn) rootFn = actor->GetFunctionByNameInChain(L"K2_GetRootComponent");
        UObject* root = nullptr;
        if (rootFn) actor->ProcessEvent(rootFn, &root);
        const void* attachParent = root
            ? *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(root) + 0xB0)
            : nullptr;
        char buf[128];
        snprintf(buf, sizeof(buf), "scan_pickup_class: instance=0x%llx root=0x%llx AttachParent=0x%llx",
                 (unsigned long long)(uintptr_t)actor, (unsigned long long)(uintptr_t)root,
                 (unsigned long long)(uintptr_t)attachParent);
        debug_log(buf);
    }
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

struct ResolveFPropCtx { uintptr_t addr; std::string result; };

static void do_resolve_fprop(void* ctxRaw)
{
    auto* ctx = static_cast<ResolveFPropCtx*>(ctxRaw);
    // FField::NamePrivate sits at +0x20 off any FField-derived pointer
    // (FProperty included) — same offset already empirically confirmed live
    // this session resolving the AnimBP's "Speed" scratch property (see
    // dump_animbp_mutables's own comment). native::fname_to_string reads an
    // in-place FName directly (no ProcessEvent/reflection needed), so this
    // works on FProperty* even though FProperty isn't a UObject and
    // GetFullName() (resolve_ptr's approach) can't be used on it at all.
    ctx->result = native::fname_to_string(ctx->addr + 0x20);
}

// Flag file content: one raw hex FProperty* pointer per line, pulled from an
// EX_InstanceVariable/EX_LocalVariable "prop=0x..." operand in a Kismet
// bytecode dump (same "same live session only" ASLR caveat as resolve_ptr —
// only valid against a .bin captured from the SAME process instance).
// Superseded an earlier, wrong version of this trigger (2026-08-13) that
// compared these pointers against GetValuePtrByPropertyNameInChain's return
// value — a category error: EX_InstanceVariable's operand is a pointer to
// the FProperty *descriptor* (reflection metadata), not the value's storage
// address, so the two were never going to match regardless of name.
static void check_resolve_fprop_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\resolve_fprop.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string lineU8;
    while (std::getline(in, lineU8)) {
        if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();
        if (lineU8.empty()) continue;

        unsigned long long addr = 0;
        if (sscanf_s(lineU8.c_str(), "%llx", &addr) != 1 || addr == 0) {
            debug_log("resolve_fprop: could not parse hex address '" + lineU8 + "'");
            continue;
        }

        ResolveFPropCtx ctx{ static_cast<uintptr_t>(addr), {} };
        if (seh_invoke(&do_resolve_fprop, &ctx)) {
            debug_log("resolve_fprop: 0x" + lineU8 + " -> \"" + ctx.result + "\"");
        } else {
            debug_log("resolve_fprop: 0x" + lineU8 + " -> <access violation>");
        }
    }
    in.close();
    DeleteFileW(flag.c_str());
}

// Read-only, live-value diagnostic (2026-08-13): after resolve_fprop
// identified WHICH four instance vars feed GetAimOffset (K2Node_
// PropertyAccess_8 = bool selector, _9/_10 = SelectRotator's two branches,
// _11 = NormalizedDeltaRotator's second/baseline operand), reads their
// actual LIVE VALUES off the LOCAL player's own AnimInstance once per
// second while this flag file exists (not one-shot — stays active until the
// file is deleted), to see empirically what varies with real camera
// movement before deciding what a proxy would need fed. Purely read-only,
// zero proxy risk — nothing here touches any proxy or Controller state.
static void log_aimoffset_values(const char* label, AActor* pawn)
{
    if (!pawn) { debug_log(std::string("watch_aimoffset: ") + label + " no pawn"); return; }
    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) { debug_log(std::string("watch_aimoffset: ") + label + " Mesh not found"); return; }
    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) { debug_log(std::string("watch_aimoffset: ") + label + " GetAnimInstance NOT FOUND"); return; }
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) { debug_log(std::string("watch_aimoffset: ") + label + " AnimInstance is null"); return; }

    auto* anim = aparams.ReturnValue;
    auto* b8  = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"K2Node_PropertyAccess_8"));
    auto* r9  = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"K2Node_PropertyAccess_9"));
    auto* r10 = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"K2Node_PropertyAccess_10"));
    auto* r11 = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"K2Node_PropertyAccess_11"));
    auto* pitchP = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"Pitch"));
    auto* yawP   = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"Yaw"));

    char line[420];
    snprintf(line, sizeof(line),
        "watch_aimoffset: %s bSel8=%d R9(P/Y/R)=%.1f/%.1f/%.1f R10(P/Y/R)=%.1f/%.1f/%.1f R11(P/Y/R)=%.1f/%.1f/%.1f Pitch=%.2f Yaw=%.2f",
        label,
        b8 ? (int)(*b8 & 1) : -1,
        r9 ? r9[0] : 0.0, r9 ? r9[1] : 0.0, r9 ? r9[2] : 0.0,
        r10 ? r10[0] : 0.0, r10 ? r10[1] : 0.0, r10 ? r10[2] : 0.0,
        r11 ? r11[0] : 0.0, r11 ? r11[1] : 0.0, r11 ? r11[2] : 0.0,
        pitchP ? *pitchP : 0.0, yawP ? *yawP : 0.0);
    debug_log(line);
}

static void check_watch_aimoffset_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\watch_aimoffset.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    static uint64_t s_lastLogUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastLogUs < 1'000'000ULL) return;
    s_lastLogUs = now;

    log_aimoffset_values("local", find_local_pawn());

    // Read-only peek at the first connected proxy's own AnimInstance, using
    // the exact same property names (same AnimBP class) — checking whether
    // bSel8 (the SelectRotator condition) is true or false for a
    // non-locally-controlled proxy, before deciding whether giving it a
    // Controller would even change anything.
    {
        std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
        for (auto& [id, player] : sdb::g_state().players) {
            if (player.proxyActor) {
                log_aimoffset_values("proxy", static_cast<AActor*>(player.proxyActor));
                break;
            }
        }
    }
}

// Read-only diagnostic (2026-08-13): compares the proxy's actual live
// K2_GetActorRotation() against player.yaw (what we're actually sending it
// every tick via teleport_proxy's SetActorLocationAndRotation call), to
// check whether something in the proxy's own CharacterMovementComponent
// (e.g. bOrientRotationToMovement, a well-known UE gotcha for exactly this
// symptom) is overriding our explicit rotation write every frame, before
// guessing at a fix.
static void check_watch_rotation_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\watch_rotation.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    static uint64_t s_lastLogUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastLogUs < 1'000'000ULL) return;
    s_lastLogUs = now;

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        if (!player.proxyActor) continue;
        auto* actor = static_cast<AActor*>(player.proxyActor);
        const FRotator rot = actor->K2_GetActorRotation();
        char line[200];
        snprintf(line, sizeof(line),
            "watch_rotation: sentYaw=%.2f actualYaw=%.2f actualPitch=%.2f actualRoll=%.2f",
            player.yaw, rot.Yaw, rot.Pitch, rot.Roll);
        debug_log(line);
        break;
    }
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

    // Only auto-click through the menu when this launch is actually
    // configured to join the bridge (a real ticket in session.cfg) — a
    // plain solo/offline launch (no session.cfg, or an empty ticket) should
    // leave the menu alone for the player to navigate normally instead of
    // getting yanked into a game they didn't ask to join through us.
    if (cfg_join_ticket.empty()) return false;

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

static void check_pending_pickup(AActor* pawn);   // defined below, near the pickup-hook implementation
static void check_inventory_pickup(AActor* pawn); // defined below, alongside check_pending_pickup

// find_local_pawn() is a UE4SS reflection scan over live UObjects — cheap
// once a pawn exists (found quickly), but worst-case (no pawn at all, e.g.
// sitting at the main menu or briefly on death) it has to exhaust the whole
// search before concluding "not found". do_game_tick() itself is only
// throttled to 5ms (200Hz), and previously called find_local_pawn()
// directly on every one of those ticks with no additional throttle — live
// tested 2026-08-12: this alone pegged the menu at ~6 FPS on a second
// client (PC1 masked it by auto-clicking through the menu in a few
// seconds; anything that sits at "no pawn yet" longer fully exposes it).
// This is the exact same class of bug already fixed once for the drop/
// pickup hook resolution's own find_local_pawn() calls (see s_drop_fn
// above) — throttling the *tick's* lookup was missed. 100ms is plenty
// responsive for detecting death/respawn pawn transitions.
// Read-only diagnostic (2026-08-13): Player_AnimBP_C's own Speed/Direction
// variables are compiler-anonymized in this Shipping build's FModel export
// (Exports/.../Player_AnimBP.json's CDO shows a "__AnimBlueprintMutables"
// struct of 53 fields named "__FloatProperty_N"/"__IntProperty_N"/
// "__BoolProperty_N", no real names preserved) — but the struct itself IS a
// real, named UPROPERTY, reachable via the same GetValuePtrByPropertyNameInChain
// reflection lookup already used throughout this project (e.g. "Mesh",
// "BP_JigPickupComponent"), sidestepping the need to know AnimInstance's own
// base-class size to locate it. Dumps all 53 as raw floats (some are really
// int32/bool, but those will just show as 0 or a garbage-looking float,
// easy to tell apart from a real Speed value visually) so the LOCAL
// player's own values can be diffed stationary vs walking to identify which
// index is Speed, entirely read-only — no risk to the proxy or to this
// pawn's own state, unlike the CharacterMovementComponent write attempts
// that crashed the game twice already this session.
static void dump_animbp_mutables(AActor* pawn)
{
    static uint64_t s_lastDumpUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastDumpUs < 1'000'000ULL) return;
    s_lastDumpUs = now;

    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) { debug_log("dump_animbp_mutables: Mesh not found"); return; }

    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) { debug_log("dump_animbp_mutables: GetAnimInstance NOT FOUND"); return; }
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) { debug_log("dump_animbp_mutables: AnimInstance is null"); return; }

    const float* vals = static_cast<const float*>(
        aparams.ReturnValue->GetValuePtrByPropertyNameInChain(L"__AnimBlueprintMutables"));
    if (!vals) { debug_log("dump_animbp_mutables: __AnimBlueprintMutables not found"); return; }

    std::string line = "dump_animbp_mutables:";
    for (int i = 0; i < 53; ++i) {
        char buf[24];
        snprintf(buf, sizeof(buf), " [%d]=%.2f", i, vals[i]);
        line += buf;
    }
    debug_log(line);

    // Bytecode analysis (2026-08-13, GetSpeed&Direction) found "Speed" is an
    // 8-byte DoubleProperty, not one of the 4-byte floats in the list above
    // — reading it as two adjacent floats there would misinterpret it.
    // FField::NamePrivate (confirmed live at the property's own +0x20,
    // resolved via resolve_fname to literally "Speed") sits right next to
    // FProperty::Offset_Internal further into the same struct, empirically
    // read as 23232 from that property's own memory — trying it here
    // directly as a byte offset from the AnimInstance object's own base,
    // not from __AnimBlueprintMutables, since Offset_Internal for a
    // Blueprint-compiled AnimGraph mutable appears to be object-relative.
    {
        struct Ctx { uintptr_t animBase; double result; } ctx{
            reinterpret_cast<uintptr_t>(aparams.ReturnValue), 0.0 };
        auto readFn = [](void* raw) {
            auto* c = static_cast<Ctx*>(raw);
            c->result = *reinterpret_cast<const double*>(c->animBase + 23232);
        };
        char buf[96];
        if (seh_invoke(readFn, &ctx)) {
            snprintf(buf, sizeof(buf), "dump_animbp_mutables: animBase=0x%llx offset23232_asDouble=%.3f",
                     static_cast<unsigned long long>(ctx.animBase), ctx.result);
        } else {
            snprintf(buf, sizeof(buf), "dump_animbp_mutables: animBase=0x%llx offset23232 read CRASHED (SEH caught)",
                     static_cast<unsigned long long>(ctx.animBase));
        }
        debug_log(buf);
    }
}

static AActor* cached_find_local_pawn()
{
    static std::atomic<uint64_t> s_last_try_us{0};
    static AActor* s_cached = nullptr;
    const uint64_t now = sdb::now_micros();
    const uint64_t last = s_last_try_us.load(std::memory_order_relaxed);
    if (last == 0 || now - last >= 100'000ULL) {
        s_last_try_us.store(now, std::memory_order_relaxed);
        s_cached = find_local_pawn();
    }
    return s_cached;
}

// Core game-tick logic — called from both on_actor_tick and on_process_event_pre.
// Rate-limited to once per 5 ms via g_last_tick_us; safe because both callers
// are always on the game thread.
//
// Root-caused live 2026-08-13 (IDA remote-debugger attach, real call stack
// captured at the fault): on_process_event_pre calls this on ~1-in-256
// ProcessEvent calls. ProxyManager::tick() (called from here) holds
// g_state().playersMtx for its whole duration while calling
// comp->ProcessEvent(...) many times per proxy (sync_equipment/
// sync_pawn_appearance/etc) — and every one of *those* ProcessEvent calls
// re-enters UE4SS's hook, i.e. on_process_event_pre, recursively. If enough
// wall-clock time (which the many-ProcessEvent-call loop can easily spend)
// passes during that recursion, the 5ms throttle above alone let a nested
// do_game_tick() call proceed, re-entering ProxyManager::tick() and trying
// to lock playersMtx a second time on the very thread that already holds
// it — undefined behavior on a plain (non-recursive) std::mutex. This
// manifested as a debug-STL/CRT internal `int 3` trap in one live repro
// (call stack: on_process_event_pre -> do_game_tick -> TcpClient::recv_all
// -> dispatch_frame -> ProxyManager::on_movement, mid-lock) and as a genuine
// hang in others (SetSkinnedAssetAndUpdate never returning) — same root
// cause, different manifestation depending on timing. This re-entrancy
// guard is the actual fix; single bool is safe since both callers are
// always on the game thread (never cross-thread re-entrancy here).
static void do_game_tick()
{
    static bool s_in_game_tick = false;
    if (s_in_game_tick) return;
    struct ReentryGuard {
        bool& flag;
        ReentryGuard(bool& f) : flag(f) { flag = true; }
        ~ReentryGuard() { flag = false; }
    } reentry_guard(s_in_game_tick);

    const uint64_t now = sdb::now_micros();
    if (now - g_last_tick_us.load(std::memory_order_relaxed) < 5'000ULL) return;
    g_last_tick_us.store(now, std::memory_order_relaxed);

    check_trace_trigger();
    check_bytecode_dump_trigger();
    check_resolve_fname_trigger();
    check_resolve_ptr_trigger();
    check_resolve_fprop_trigger();
    check_watch_aimoffset_trigger();
    check_watch_rotation_trigger();
    check_widget_scan_trigger();
    check_scan_pickup_class_trigger();
    check_dump_clothing_table_trigger();
    check_call_trigger();
    check_mem_dump_trigger();

    // Lazy-connect: open TCP once a pawn exists (level fully loaded).
    if (!g_tcp.is_open()) {
        if (!g_tcp_started.load(std::memory_order_relaxed)
            && !cfg_join_ticket.empty()
            && cached_find_local_pawn())
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
    AActor* pawn = cached_find_local_pawn();

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

    // 6b. Resolve any pickup interact caught by handle_pickup_hook a moment
    // ago, plus the inventory-diff fallback for the drag-and-drop UI path
    // the hook doesn't cover (see check_inventory_pickup's comment).
    check_pending_pickup(pawn);
    check_inventory_pickup(pawn);

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
        send_weapon_attachments(pawn);
        send_pawn_appearance(pawn);
    }
}

// Item-drop hook: BP_JigMultiplayer_C::ItemDropRequest_Event_0(UJSI_Slot_C*
// ItemRef, int32 Count, UJSIContainer_C* Container) — confirmed live via a
// full ProcessEvent trace during a real in-game drop (research/
// 04_ida_investigation_log.md), NOT a name guess like the first attempt
// (BP_JigHelperComp_C::RequestDropAsPickup, which resolves fine but never
// actually fires on a real drop — wrong component entirely). Ubergraph
// bytecode at this function's entry point (dumped + disassembled) confirmed
// the real server call is JigMultiplayer's own SERVER_RequestDropItem(FGuid
// ItemUID, int32 Count, ...) — but ItemUID there is just the slot widget's
// own UObject::GetUniqueID(), an ephemeral per-session id, not a stable item
// identity — so ItemRef (the widget, still valid here) is the right thing to
// read, via its own GetItemID(FName& ItemId) UFUNCTION (research/
// CXXHeaderDump/JSI_Slot.hpp) rather than any raw memory offset.
// Resolved once via GetFunctionByNameInChain off pawn+0x818's
// BP_JigMultiplayer component (research/CXXHeaderDump/BP_PlayerCharacter.hpp)
// and compared by pointer on every call — cheap enough to check
// unconditionally, same reasoning as the equip-trace diagnostic.
static UFunction* s_drop_fn = nullptr;

static void handle_drop_hook(void* params)
{
    if (!params) return;
    auto* itemRef = *reinterpret_cast<UObject**>(static_cast<uint8_t*>(params) + 0x00);
    const int32_t count = *reinterpret_cast<const int32_t*>(static_cast<uint8_t*>(params) + 0x08);
    if (!itemRef || count <= 0) return;

    // A single drop action was observed calling ItemDropRequest_Event_0
    // twice for the same ItemRef (2026-08-12 live test: two identical
    // ItemDropRequests, two duplicate world entities on the other client) —
    // debounce by (ItemRef pointer, time) rather than trusting one
    // ProcessEvent call per real drop.
    static void*   s_last_item_ref = nullptr;
    static uint64_t s_last_drop_us = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (itemRef == s_last_item_ref && nowUs - s_last_drop_us < 500'000ULL) {
        debug_log("handle_drop_hook: debounced duplicate call for same ItemRef");
        return;
    }
    s_last_item_ref = itemRef;
    s_last_drop_us  = nowUs;

    UFunction* getIdFn = itemRef->GetFunctionByNameInChain(L"GetItemID");
    if (!getIdFn) {
        debug_log("handle_drop_hook: GetItemID not found on ItemRef");
        return;
    }
    struct { int32_t ComparisonIndex = 0, Number = 0; } idParams;
    itemRef->ProcessEvent(getIdFn, &idParams);
    std::string itemId = native::fname_to_string(reinterpret_cast<uintptr_t>(&idParams));
    if (itemId.empty()) {
        debug_log("handle_drop_hook: GetItemID returned empty FName");
        return;
    }

    AActor* pawn = find_local_pawn();
    if (!pawn) return;

    const FVector loc = pawn->K2_GetActorLocation();
    send_item_drop_request(itemId, static_cast<uint16_t>(count),
        static_cast<float>(loc.X), static_cast<float>(loc.Y), static_cast<float>(loc.Z));
    debug_log("handle_drop_hook: sent ItemDropRequest itemId=" + itemId +
              " qty=" + std::to_string(count));
}

// Item-pickup detection — six hook attempts ruled out live (2026-08-12 and
// earlier), landing on inventory-diff polling as the fallback (see git
// history / research/04_ida_investigation_log.md for the five ruled-out
// attempts: TryPickup, the interact-opens-UI dead end, SetEquippedInfoBySlot,
// CombineItemRequest, HandleContainerOnContainer). The real event, confirmed
// 2026-08-12 by cross-referencing the original SD-Online client's own UE4SS
// Lua hooks (extracted from a legitimate install, see research notes), is
// BP_PlayerCharacter_C::OnPickupInteractExecuted(AActor* PickupRef,
// UJSIContainer_C* TargetContainer, bool& Result) — confirmed present on
// this exact class (research/CXXHeaderDump/BP_PlayerCharacter.hpp:275), just
// never tried before because nobody had this exact name/signature to try.
//
// Result is an out param only meaningful after the function body actually
// runs, and this project's ProcessEvent hook is pre-only (no post-callback
// resolved for this UE4SS build) — so instead of trusting Result directly,
// take an inventory snapshot at hook time and diff it one tick later. This
// is the same confirmation check the old poll used, but now triggered by
// the real interact attempt instead of blindly every 1.5s, and resolved by
// exact actor identity (PickupRef matched against WorldEntity::actor)
// instead of nearest-itemId-in-range guessing.
static UFunction* s_pickup_fn = nullptr;
static AActor*    s_pending_pickup_ref = nullptr;
static std::unordered_map<std::string, int32_t> s_pending_pickup_snapshot;
static uint64_t   s_pending_pickup_us = 0;

static void handle_pickup_hook(AActor* pawn, void* params)
{
    if (!params || !pawn) return;
    auto* pickupRef = *reinterpret_cast<AActor**>(static_cast<uint8_t*>(params) + 0x00);
    if (!pickupRef) return;

    s_pending_pickup_ref = pickupRef;
    s_pending_pickup_us  = sdb::now_micros();
    s_pending_pickup_snapshot.clear();
    for (const auto& container : read_local_inventory(pawn))
        for (const auto& slot : container.items)
            s_pending_pickup_snapshot[slot.itemId] += slot.quantity;
}

// Called every tick; resolves a pending pickup ~100ms after the interact
// fired, once OnPickupInteractExecuted's body has actually had a chance to
// run and (if successful) update the local inventory.
static void check_pending_pickup(AActor* pawn)
{
    if (!s_pending_pickup_ref || !pawn) return;
    if (sdb::now_micros() - s_pending_pickup_us < 100'000ULL) return;

    AActor* pickupRef = s_pending_pickup_ref;
    s_pending_pickup_ref = nullptr;

    std::unordered_map<std::string, int32_t> curCounts;
    for (const auto& container : read_local_inventory(pawn))
        for (const auto& slot : container.items)
            curCounts[slot.itemId] += slot.quantity;

    bool anyIncrease = false;
    for (const auto& [itemId, curQty] : curCounts) {
        const auto it = s_pending_pickup_snapshot.find(itemId);
        const int32_t prevQty = (it != s_pending_pickup_snapshot.end()) ? it->second : 0;
        if (curQty > prevQty) { anyIncrease = true; break; }
    }
    if (!anyIncrease) {
        debug_log("check_pending_pickup: no inventory increase — interact did not succeed");
        return;
    }

    std::lock_guard<std::mutex> lk(sdb::g_state().entityMtx);
    for (const auto& [id, entity] : sdb::g_state().entities) {
        if (entity.actor != pickupRef) continue;
        send_item_pickup_request(id);
        debug_log("check_pending_pickup: sent ItemPickupRequest eid=" + std::to_string(id) +
                  " (matched by actor identity)");
        return;
    }
    debug_log("check_pending_pickup: no tracked entity matches PickupRef — likely non-synced world loot");
}

// check_pending_pickup only catches a direct interact-key pickup, which
// OnPickupInteractExecuted fires for — but live testing 2026-08-12 confirmed
// the actually-used pickup method is a two-step "open loot window, drag item
// into own inventory" UI flow (the same drag/drop path five earlier hook
// attempts already failed to intercept — see the comment above
// handle_pickup_hook), which never calls OnPickupInteractExecuted at all.
// Keep this inventory-diff poll running alongside the hook rather than
// choosing one: the hook is lower-latency and identity-exact when it does
// apply, this is the proven fallback for the drag-and-drop path players
// actually use.
static void check_inventory_pickup(AActor* pawn)
{
    static std::unordered_map<std::string, int32_t> s_lastCounts;
    static uint64_t s_lastCheckUs = 0;

    const uint64_t now = sdb::now_micros();
    if (s_lastCheckUs != 0 && now - s_lastCheckUs < 1'500'000ULL) return;
    s_lastCheckUs = now;

    std::unordered_map<std::string, int32_t> curCounts;
    for (const auto& container : read_local_inventory(pawn))
        for (const auto& slot : container.items)
            curCounts[slot.itemId] += slot.quantity;

    if (!s_lastCounts.empty()) {
        const FVector loc = pawn->K2_GetActorLocation();
        for (const auto& [itemId, curQty] : curCounts) {
            const auto it = s_lastCounts.find(itemId);
            const int32_t prevQty = (it != s_lastCounts.end()) ? it->second : 0;
            if (curQty <= prevQty) continue; // not an increase — nothing picked up

            std::lock_guard<std::mutex> lk(sdb::g_state().entityMtx);
            for (const auto& [id, entity] : sdb::g_state().entities) {
                if (entity.itemId != itemId || !entity.hasPosition) continue;
                const double dx = entity.x - loc.X, dy = entity.y - loc.Y, dz = entity.z - loc.Z;
                if (dx*dx + dy*dy + dz*dz > 300.0*300.0) continue;

                send_item_pickup_request(id);
                debug_log("check_inventory_pickup: sent ItemPickupRequest itemId=" + itemId +
                          " eid=" + std::to_string(id));
                break; // one match is enough for this itemId this poll
            }
        }
    }

    s_lastCounts = std::move(curCounts);
}

// Fires on the game thread for every UObject::ProcessEvent call.
static void on_process_event_pre(UObject* obj, UFunction* func, void* params)
{
    // Resolving s_drop_fn needs find_local_pawn() — a UE4SS reflection scan
    // over all live UObjects, expensive enough that it must NOT run on every
    // ProcessEvent call (this fires thousands of times per frame). Retried
    // at most once per second until it succeeds, same throttling pattern as
    // g_last_open_try_us/g_last_equip_us elsewhere in this file. Missing
    // this throttle pegged the game at ~1 FPS even at the main menu (no pawn
    // yet => the lookup kept retrying on literally every ProcessEvent call).
    static std::atomic<uint64_t> s_last_drop_fn_try_us{0};
    if (func && !s_drop_fn) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_drop_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_drop_fn_try_us.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                const uintptr_t jigMp = *reinterpret_cast<uintptr_t*>(
                    reinterpret_cast<uintptr_t>(pawn) + 0x818);
                if (jigMp) {
                    s_drop_fn = reinterpret_cast<UObject*>(jigMp)
                        ->GetFunctionByNameInChain(L"ItemDropRequest_Event_0");
                    debug_log(s_drop_fn ? "on_process_event_pre: ItemDropRequest_Event_0 resolved"
                                        : "on_process_event_pre: ItemDropRequest_Event_0 NOT FOUND on BP_JigMultiplayer");
                } else {
                    debug_log("on_process_event_pre: pawn+0x818 BP_JigMultiplayer is null");
                }
            } else {
                debug_log("on_process_event_pre: find_local_pawn() returned null (drop-fn resolve)");
            }
        }
    }
    if (func && func == s_drop_fn) {
        handle_drop_hook(params);
    }

    // Same throttled-retry shape as s_drop_fn above, but OnPickupInteractExecuted
    // is declared directly on BP_PlayerCharacter_C (research/CXXHeaderDump/
    // BP_PlayerCharacter.hpp:275), not a pawn+0x818 component — resolved
    // straight off the pawn itself.
    static std::atomic<uint64_t> s_last_pickup_fn_try_us{0};
    if (func && !s_pickup_fn) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_pickup_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_pickup_fn_try_us.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                s_pickup_fn = pawn->GetFunctionByNameInChain(L"OnPickupInteractExecuted");
                debug_log(s_pickup_fn ? "on_process_event_pre: OnPickupInteractExecuted resolved"
                                      : "on_process_event_pre: OnPickupInteractExecuted NOT FOUND on pawn");
            }
        }
    }
    if (func && func == s_pickup_fn) {
        handle_pickup_hook(find_local_pawn(), params);
    }

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

// Look-direction sync, take 2 (2026-08-13): GetAimOffset unconditionally
// hard-resets the AnimBP's own Pitch to 0 every single frame for a
// non-locally-controlled proxy (confirmed live via bytecode tracing +
// direct value sampling — see proxy_manager.cpp's apply_proxy_aim_pitch_safe
// comment for the full chain of evidence). A same-tick property write can
// never win that race. Fixed here instead: UE4SS.dll in this build DOES
// export RegisterProcessEventPostCallback (verified live via GetProcAddress
// against the actual on-disk DLL, same mangled-name pattern as the existing
// Pre registration below with Pre->Post substituted) — an earlier comment
// elsewhere in this file claiming "no post-callback resolved for this UE4SS
// build" was simply never actually tested for this specific symbol. Once
// registered, this runs immediately AFTER the real GetAimOffset call
// completes (including its own Pitch=0 write), so writing here always wins
// cleanly — no fight, no flicker, unlike a same-tick pre-write.
static UFunction* s_getAimOffsetFn = nullptr;

static void on_process_event_post(UObject* obj, UFunction* func, void* /*params*/)
{
    if (!obj || !func) return;

    // Lazily resolve once, off the LOCAL player's own AnimInstance — the
    // UFunction object is shared across every Player_AnimBP_C instance
    // (proxies included), so a single resolution covers all of them for the
    // rest of the session. Throttled the same way s_drop_fn's lookup is
    // above: find_local_pawn() is an expensive reflection scan and this
    // fires on every ProcessEvent call otherwise.
    if (!s_getAimOffsetFn) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
                UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
                if (mesh) {
                    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
                    if (getAnimFn) {
                        struct Params { UObject* ReturnValue = nullptr; } aparams;
                        mesh->ProcessEvent(getAnimFn, &aparams);
                        if (aparams.ReturnValue)
                            s_getAimOffsetFn = aparams.ReturnValue->GetFunctionByNameInChain(L"GetAimOffset");
                    }
                }
            }
        }
    }

    // Cheap pointer-compare fast path — skips essentially every ProcessEvent
    // call in the game (this fires thousands of times per frame); only
    // GetAimOffset calls (one per Player_AnimBP_C instance per frame, a
    // small handful total) do any real work below.
    if (func != s_getAimOffsetFn) return;

    UFunction* getOwnerFn = obj->GetFunctionByNameInChain(L"GetOwningActor");
    if (!getOwnerFn) return;
    struct OwnerParams { AActor* ReturnValue = nullptr; } oparams;
    obj->ProcessEvent(getOwnerFn, &oparams);
    AActor* owner = oparams.ReturnValue;
    if (!owner) return;

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        if (static_cast<AActor*>(player.proxyActor) != owner) continue;

        double degrees = player.aimPitchByte * (360.0 / 256.0);
        if (degrees > 180.0) degrees -= 360.0;
        if (auto* pitchSlot = static_cast<double*>(obj->GetValuePtrByPropertyNameInChain(L"Pitch")))
            *pitchSlot = degrees;
        break;
    }
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

        // Verified live via GetProcAddress against the actual on-disk
        // UE4SS.dll (2026-08-13) — exists in this build despite an earlier,
        // never-actually-tested assumption elsewhere in this file that it
        // didn't. Used by on_process_event_post to win the per-frame
        // GetAimOffset race for proxy aim-pitch (see that function's comment).
        auto* fn_pe_post = ue4ss ? reinterpret_cast<RegPE>(GetProcAddress(ue4ss,
            "?RegisterProcessEventPostCallback@Hook@Unreal@RC@@YAXV?$function"
            "@$$A6AXPEAVUObject@Unreal@RC@@PEAVUFunction@23@PEAX@Z@std@@@Z")) : nullptr;
        if (fn_pe_post) fn_pe_post(on_process_event_post);
        else Output::send<LogLevel::Error>(STR("SDB: RegisterProcessEventPostCallback not found\n"));

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
