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
#include <array>
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
static uint8_t read_local_aim_pitch(AActor* pawn, float& outRelativeYaw);
static uint8_t read_local_movement_flags(AActor* pawn);
static void check_local_montage_change(AActor* pawn);

static void send_movement(AActor* pawn)
{
    const FVector  loc = pawn->K2_GetActorLocation();
    const FRotator rot = pawn->K2_GetActorRotation();

    sdb::Movement mv{};
    mv.x      = static_cast<float>(loc.X);
    mv.y      = static_cast<float>(loc.Y);
    mv.z      = static_cast<float>(loc.Z);
    mv.yaw    = static_cast<float>(rot.Yaw);

    // animationState was never populated or read anywhere in this project —
    // repurposed here to carry the active weapon slot (11-14, or 0xFF for
    // none) instead of adding a new wire field. See
    // read_local_active_weapon_slot's own comment for what this is verifying.
    mv.animationState = read_local_active_weapon_slot(pawn);

    // aimState was likewise never populated or read anywhere — repurposed to
    // carry a quantized look-pitch byte (see read_local_aim_pitch's own
    // comment for why pitch-only, and why this mirrors the engine's own
    // built-in ACharacter::RemoteViewPitch mechanism). aimYaw (2026-08-13,
    // was previously just a duplicate of body yaw — a bug, not a deliberate
    // placeholder) now carries the raw absolute control yaw from the same
    // GetControlRotation() call — see read_local_aim_pitch's own comment for
    // why this is sent absolute, not pre-converted to relative-to-body here.
    mv.aimState = read_local_aim_pitch(pawn, mv.aimYaw);

    // movementState was likewise never populated or read anywhere —
    // repurposed to carry a 3-bit crouch/ADS/falling flag byte. See
    // read_local_movement_flags's own comment for the bit layout.
    mv.movementState = read_local_movement_flags(pawn);

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
// outAbsoluteYaw (2026-08-13, rewritten): sends the RAW, absolute
// ControlRotation.Yaw — deliberately NOT pre-converted to "relative to body"
// here. First attempt computed relative-to-body-yaw sender-side (either the
// instant body yaw, or a separately-smoothed lagging approximation of it)
// and both produced garbage: BP_PlayerCharacter has
// bOrientRotationToMovement (confirmed via FModel export), so body yaw
// tracks *movement* direction and is often changing almost continuously
// while actually playing — there's no stable "resting" value for a lagging
// reference to converge toward, it just perpetually chases a moving target
// and the resulting relative yaw is essentially noise. Fixed properly by
// moving the subtraction to the RECEIVE side instead, against
// RemotePlayer::renderYaw — the proxy's own already-smoothed body yaw,
// i.e. exactly what the proxy's mesh is *actually* currently showing —
// computed fresh every tick in ProxyManager::update_proxy_render_smoothing
// so the aim-offset reference can never drift out of sync with the visible
// body orientation the way two independently-smoothed values could.
static uint8_t read_local_aim_pitch(AActor* pawn, float& outAbsoluteYaw)
{
    outAbsoluteYaw = 0.0f;
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

    double yaw = std::fmod(params.ReturnValue.Yaw, 360.0);
    if (yaw > 180.0)  yaw -= 360.0;
    if (yaw < -180.0) yaw += 360.0;
    outAbsoluteYaw = static_cast<float>(yaw);

    // Temporary diagnostic (2026-08-13): confirm the sender is actually
    // seeing a live, varying camera pitch/yaw before trusting the receive side.
    // Throttled to ~1/sec so it doesn't flood debug.log at movement-tick rate.
    static uint64_t s_lastLogUs = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (nowUs - s_lastLogUs > 1'000'000ULL) {
        s_lastLogUs = nowUs;
        char buf[160];
        snprintf(buf, sizeof(buf),
                 "read_local_aim_pitch: rawPitch=%.2f quantized=%u absYaw=%.2f",
                 params.ReturnValue.Pitch, quantized, outAbsoluteYaw);
        debug_log(buf);
    }

    return quantized;
}

// Crouch/ADS/falling sync (2026-08-13): Player_AnimBP_C has three plain,
// dedicated-byte (bIsNativeBool, FieldMask=255 — not bit-packed with
// siblings) BoolProperty class members: "IsCrouching", "IsADS", "Falling"
// (confirmed via FModel export, same class-property block as "Pitch"/"Yaw").
// Read directly off the LOCAL player's own AnimInstance (same
// Mesh->GetAnimInstance() reflection path used throughout this file) rather
// than the underlying native ACharacter/CharacterMovementComponent state —
// simpler, and these values are exactly what drives the local player's own
// (correct) crouch/ADS/fall animation, so mirroring them is the right
// source of truth for what a remote player should visually look like too.
static uint8_t read_local_movement_flags(AActor* pawn)
{
    if (!pawn) return 0;
    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return 0;
    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) return 0;
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) return 0;

    auto* anim = aparams.ReturnValue;
    auto* crouching = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"IsCrouching"));
    auto* ads       = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"IsADS"));
    auto* falling   = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"Falling"));

    uint8_t flags = 0;
    if (crouching && *crouching) flags |= 0x01;
    if (ads && *ads)             flags |= 0x02;
    if (falling && *falling)     flags |= 0x04;
    return flags;
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

static void send_build_request(const std::string& itemId, float x, float y, float z, float yaw)
{
    sdb::Frame f;
    f.type = sdb::MsgType::InteractionRequest;
    build_session_frame(f);
    f.payload = sdb::encode_interaction_request_build(itemId, x, y, z, yaw);
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

    // 2026-08-15: merge non-empty fields into the "last good" cache (see
    // state.hpp's lastGoodLocalAppearance comment) — field-by-field so a
    // partial clear (e.g. hair already gone but everything else fine)
    // doesn't blank out still-good cached fields for the others.
    {
        auto& good = sdb::g_state().lastGoodLocalAppearance;
        good.isMale = pa.isMale;
        if (!pa.hairMeshName.empty())     good.hairMeshName = pa.hairMeshName;
        if (!pa.hairColorName.empty())    good.hairColorName = pa.hairColorName;
        if (!pa.beardMeshName.empty())    good.beardMeshName = pa.beardMeshName;
        if (!pa.beardColorName.empty())   good.beardColorName = pa.beardColorName;
        if (!pa.mouthMeshName.empty())    good.mouthMeshName = pa.mouthMeshName;
        if (!pa.eyebrowsMeshName.empty()) good.eyebrowsMeshName = pa.eyebrowsMeshName;
        for (size_t i = 0; i < pa.bodyPartMeshNames.size(); ++i)
            if (!pa.bodyPartMeshNames[i].empty()) good.bodyPartMeshNames[i] = pa.bodyPartMeshNames[i];
    }

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

    case sdb::MsgType::PlayMontage:
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto m = sdb::decode_play_montage(f.payload.data(), f.payload.size()))
                sdb::g_proxy_manager().on_play_montage(f.playerId, m->montageName, m->playRate);
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

        // Vitals write-back — deferred (2026-08-14, see state.hpp's
        // pendingVitalsRestore comment for why) instead of writing raw
        // memory inline here the instant find_local_pawn() first succeeds.
        // Applied a couple seconds later in do_game_tick, SEH-wrapped.
        st.vitalsHealth    = prog->health;
        st.vitalsHunger    = prog->hunger;
        st.vitalsThirst    = prog->thirst;
        st.vitalsStamina   = prog->stamina;
        st.vitalsRadiation = prog->radiation;
        st.vitalsRestoreReadyAtUs = sdb::now_micros() + 2'000'000ULL;
        st.pendingVitalsRestore.store(true, std::memory_order_release);

        Output::send<LogLevel::Normal>(
            STR("SDB: progress restored  x={:.1f} y={:.1f} z={:.1f}  health={:.2f} level={:d}  "
                "hunger={:.1f} thirst={:.1f} stamina={:.1f} radiation={:.1f}\n"),
            prog->posX, prog->posY, prog->posZ, prog->health, prog->level,
            prog->hunger, prog->thirst, prog->stamina, prog->radiation);
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
            // Same 2s post-spawn grace period used everywhere else a proxy's
            // components get touched — see on_process_event_post's own
            // comment for why (live-tested crash without it).
            // TEMPORARILY DISABLED 2026-08-13 for testing at the user's
            // request, to see whether this specific call actually needs it —
            // re-enable (uncomment the >= check) if proxies start
            // crashing/deadlocking again right after spawn.
            if (player.proxyActor /* && sdb::now_micros() - player.proxySpawnedAtUs >= 2'000'000ULL */) {
                log_aimoffset_values("proxy", static_cast<AActor*>(player.proxyActor));
                break;
            }
        }
    }
}

// Read-only diagnostic (2026-08-13): GetLeftHandLoc's entire body is
// "LeftHandWeaponLocation = K2Node_PropertyAccess_13" (confirmed via fresh
// bytecode dump + resolve_fprop — a 30-byte function, just one property
// copy). K2Node_PropertyAccess_13 is a native getter-chain cache (same
// mechanism as GetAimOffset's own K2Node_PropertyAccess_8/9/10/11), likely
// reading the equipped weapon actor's own grip socket — invisible to
// Kismet bytecode. Reads the live Vector value off both the local player
// and a proxy to see whether the proxy's version is just empty/default
// (the working theory for the one-handed shotgun-grip symptom) before
// guessing at a fix.
static void log_lefthand_values(const char* label, AActor* pawn)
{
    if (!pawn) { debug_log(std::string("watch_lefthand: ") + label + " no pawn"); return; }
    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) { debug_log(std::string("watch_lefthand: ") + label + " Mesh not found"); return; }
    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) { debug_log(std::string("watch_lefthand: ") + label + " GetAnimInstance NOT FOUND"); return; }
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) { debug_log(std::string("watch_lefthand: ") + label + " AnimInstance is null"); return; }

    auto* anim = aparams.ReturnValue;
    auto* propAccess13 = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"K2Node_PropertyAccess_13"));
    auto* leftHandLoc   = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"LeftHandWeaponLocation"));
    auto* leftHandRot   = static_cast<double*>(anim->GetValuePtrByPropertyNameInChain(L"LeftHandWeaponRotator"));
    auto* isAds = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"IsADS"));
    // "IsADS?" (with a trailing '?') showed up as a distinct name elsewhere
    // in the FModel export at least once — check both spellings live in
    // case the plain "IsADS" instance isn't actually the one GetAnimationInfo
    // FromCharacter's Ads output feeds.
    auto* isAdsQ = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"IsADS?"));

    char line[460];
    snprintf(line, sizeof(line),
        "watch_lefthand: %s PA13(x/y/z)=%.2f/%.2f/%.2f LeftHandLoc(x/y/z)=%.2f/%.2f/%.2f "
        "LeftHandRot(P/Y/R)=%.2f/%.2f/%.2f IsADS=%d IsADS?=%d",
        label,
        propAccess13 ? propAccess13[0] : 0.0, propAccess13 ? propAccess13[1] : 0.0, propAccess13 ? propAccess13[2] : 0.0,
        leftHandLoc ? leftHandLoc[0] : 0.0, leftHandLoc ? leftHandLoc[1] : 0.0, leftHandLoc ? leftHandLoc[2] : 0.0,
        leftHandRot ? leftHandRot[0] : 0.0, leftHandRot ? leftHandRot[1] : 0.0, leftHandRot ? leftHandRot[2] : 0.0,
        isAds ? (int)*isAds : -1, isAdsQ ? (int)*isAdsQ : -1);
    debug_log(line);
}

static void check_watch_lefthand_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\watch_lefthand.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    static uint64_t s_lastLogUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastLogUs < 1'000'000ULL) return;
    s_lastLogUs = now;

    log_lefthand_values("local", find_local_pawn());

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        // Same 2s post-spawn grace period used everywhere else a proxy's
        // components get touched — see on_process_event_post's own comment
        // for why (live-tested crash without it).
        if (player.proxyActor /* && sdb::now_micros() - player.proxySpawnedAtUs >= 2'000'000ULL */) {
            log_lefthand_values("proxy", static_cast<AActor*>(player.proxyActor));
            break;
        }
    }
}

// Read-only diagnostic (2026-08-13, Session 54 weapon-grip investigation):
// GetAnimationInfoFromCharacter's bytecode decode (bytecode_dump.flag) shows
// it interface-casts the owning pawn and calls "GetAnimationInfo", storing
// the FName it returns into an instance variable named "CActiveSlot"
// (resolved via resolve_fprop.flag) — the first genuinely new, unruled-out
// candidate for what drives one-handed-vs-two-handed grip pose, fed straight
// from the character every relevant update. Comparing its live value on a
// correctly-rendering local player against a proxy showing the bug is the
// most direct test of that theory so far this session.
static void log_activeslot_values(const char* label, AActor* pawn)
{
    if (!pawn) { debug_log(std::string("watch_activeslot: ") + label + " no pawn"); return; }
    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) { debug_log(std::string("watch_activeslot: ") + label + " Mesh not found"); return; }
    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) { debug_log(std::string("watch_activeslot: ") + label + " GetAnimInstance NOT FOUND"); return; }
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) { debug_log(std::string("watch_activeslot: ") + label + " AnimInstance is null"); return; }

    auto* anim = aparams.ReturnValue;
    auto* activeSlotPtr = anim->GetValuePtrByPropertyNameInChain(L"CActiveSlot");
    std::string activeSlot = activeSlotPtr
        ? native::fname_to_string(reinterpret_cast<uintptr_t>(activeSlotPtr))
        : std::string("<not found>");

    auto* inMeleeStance = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"InMeleeStance"));
    auto* isCrouching    = static_cast<uint8_t*>(anim->GetValuePtrByPropertyNameInChain(L"IsCrouching"));
    // Session 55: CombatState(int32 BlendSpace)'s entire decoded body is just
    // "BlendSpaceInt = BlendSpace; return;" (bytecode_dump.flag + kismet_disasm.py,
    // no branch/weapon-type check at all) — BlendSpaceInt itself, whatever
    // selects a BlendSpace asset in the AnimGraph, is the next concrete
    // candidate for what actually picks one-handed-vs-two-handed arm pose.
    auto* blendSpaceInt = static_cast<int32_t*>(anim->GetValuePtrByPropertyNameInChain(L"BlendSpaceInt"));

    char line[320];
    snprintf(line, sizeof(line),
        "watch_activeslot: %s CActiveSlot=\"%s\" InMeleeStance=%d IsCrouching=%d BlendSpaceInt=%d",
        label, activeSlot.c_str(),
        inMeleeStance ? (int)*inMeleeStance : -1, isCrouching ? (int)*isCrouching : -1,
        blendSpaceInt ? *blendSpaceInt : -999);
    debug_log(line);
}

static void check_watch_activeslot_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\watch_activeslot.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    static uint64_t s_lastLogUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastLogUs < 1'000'000ULL) return;
    s_lastLogUs = now;

    log_activeslot_values("local", find_local_pawn());

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        if (player.proxyActor /* && sdb::now_micros() - player.proxySpawnedAtUs >= 2'000'000ULL */) {
            log_activeslot_values("proxy", static_cast<AActor*>(player.proxyActor));
            break;
        }
    }
}

// Attachment-health monitor (2026-08-14) — built for the still-open
// mesh/item-detachment bug family (three live occurrences in one evening,
// none with a reliable on-demand repro; see research log Session 59's later
// sections). Every reactive fix attempted so far had to guess in the dark
// because nobody could ever catch the *moment* something detached — by the
// time it's visually noticed, whatever diagnostic state existed at the
// actual moment is long gone. This runs unconditionally (no flag needed)
// every ~2s per tracked actor (local pawn + one proxy, matching
// watch_activeslot's single-proxy pattern), snapshotting Mesh's
// AttachChildren pointer set (same 0xC0 data/0xC8 count offsets already
// proven safe by read_local_weapon_attachments' own SEH-guarded walk) and
// diffing against the previous snapshot. Any child present last check but
// missing this check gets logged immediately, with its resolved
// GetFullName() if the pointer is still readable (usually true — a detached
// item lies in the world, it isn't destroyed) — giving the next dedicated
// session real "what disappeared and when" data instead of only a
// several-seconds-later visual report with no diagnostic trail at all.
struct AttachHealthCtx {
    UObject* mesh;
    std::vector<uintptr_t>* prev; // previous snapshot, read+written in place
    std::string label;
    std::unordered_map<uintptr_t, bool>* itemHadMesh; // keyed by child pointer, persists across ticks
    // 2026-08-15: drift detection/repair for ANY attached child this scan
    // visits — since check_attach_health_component already runs one level
    // deep (weapon attachments: scope/mag/suppressor/etc.), this covers
    // those for free, not just top-level equipped items. Same 30-unit
    // threshold as component_drift/equip_restore_retry's own drift checks.
    // Repair re-snaps via K2_AttachToComponent using the child's OWN
    // currently-recorded socket (native `GetAttachSocketName()`, Engine.hpp)
    // rather than looking up the "correct" socket from item data — the
    // socket name itself doesn't drift, only the transform, so this works
    // without needing to know which DataAsset field applies to this child.
    std::unordered_map<uintptr_t, std::array<double, 3>>* itemLastPos;
};

static void do_attach_health_scan(void* ctxRaw)
{
    auto* ctx = static_cast<AttachHealthCtx*>(ctxRaw);
    const uintptr_t meshAddr = reinterpret_cast<uintptr_t>(ctx->mesh);

    const uintptr_t childrenData  = *reinterpret_cast<uintptr_t*>(meshAddr + 0x00C0);
    const int32_t   childrenCount = *reinterpret_cast<int32_t*>(meshAddr + 0x00C0 + 0x08);
    // Same defensive clamp as read_local_weapon_attachments — a concurrent
    // native mutation of this TArray can transiently read as a huge garbage
    // count; skip this check entirely rather than iterate garbage.
    if (childrenCount < 0 || childrenCount > 64) return;

    std::vector<uintptr_t> current;
    if (childrenData && childrenCount > 0) {
        current.reserve(static_cast<size_t>(childrenCount));
        for (int32_t c = 0; c < childrenCount; ++c) {
            uintptr_t child = *reinterpret_cast<uintptr_t*>(childrenData + static_cast<size_t>(c) * 8);
            if (child) current.push_back(child);
        }
    }

    std::vector<uintptr_t>& prev = *ctx->prev;
    if (!prev.empty()) {
        for (uintptr_t oldChild : prev) {
            if (std::find(current.begin(), current.end(), oldChild) != current.end()) continue;
            // oldChild was attached last check, is gone now — resolve what
            // it was, if the pointer is still readable.
            std::string name = "<unresolved>";
            UObject* childObj = reinterpret_cast<UObject*>(oldChild);
            std::wstring wname = childObj->GetFullName();
            if (!wname.empty()) {
                int need = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
                if (need > 0) {
                    name.resize(static_cast<size_t>(need - 1));
                    WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), need, nullptr, nullptr);
                }
            }
            char line[384];
            snprintf(line, sizeof(line),
                "attach_health: %s DETACHED ptr=0x%llx name=%s (prevCount=%zu curCount=%zu)",
                ctx->label.c_str(), static_cast<unsigned long long>(oldChild), name.c_str(),
                prev.size(), current.size());
            debug_log(line);
        }
    }
    prev = current;

    // 2026-08-14, fourth extension same session: "PC1's AK is gone but
    // attachments are there" — the weapon's own mesh going invisible/cleared
    // while its still-attached child attachments stay visible. This is
    // distinct from the top-level attach/detach diffing above (the AK never
    // left AttachChildren at all in that report) and from component_drift's
    // fixed body-part list (a weapon item isn't one of those named
    // properties). Reuses the exact same reflection-based mesh-asset check
    // as component_drift's addition, just applied to every item currently
    // present here instead of a fixed component list — tries both
    // SkeletalMesh/SkeletalMeshAsset (character clothing/weapons can be
    // either skinned or static meshes) and StaticMesh for the same reason.
    if (ctx->itemHadMesh) {
        for (uintptr_t child : current) {
            UObject* childObj = reinterpret_cast<UObject*>(child);
            void** meshSlot = static_cast<void**>(childObj->GetValuePtrByPropertyNameInChain(L"SkeletalMesh"));
            if (!meshSlot) meshSlot = static_cast<void**>(childObj->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset"));
            if (!meshSlot) meshSlot = static_cast<void**>(childObj->GetValuePtrByPropertyNameInChain(L"StaticMesh"));
            if (!meshSlot) continue;

            const bool hasMeshNow = (*meshSlot != nullptr);
            auto it = ctx->itemHadMesh->find(child);
            // 2026-08-15: same initial-state visibility gap as
            // component_drift (see its comment) — an item missing its mesh
            // on the very first sample after a fresh baseline never gets
            // flagged by the transition check below. Log-only.
            if (it == ctx->itemHadMesh->end()) {
                debug_log("attach_health: " + ctx->label + " item ptr=0x" + std::to_string(child) +
                          " initial post-join state = " + (hasMeshNow ? "SET" : "MISSING"));
            }
            if (it != ctx->itemHadMesh->end() && it->second && !hasMeshNow) {
                debug_log("attach_health: " + ctx->label + " item ptr=0x" +
                          std::to_string(child) + " MESH ASSET CLEARED (was set, now null, still attached)");
            }
            (*ctx->itemHadMesh)[child] = hasMeshNow;
        }
    }

    // Positional drift check + repair — see AttachHealthCtx's own comment.
    // 2026-08-15: kill-switched for an isolation test — PC1 froze again
    // with no log evidence this specific new code (or the appearance-
    // repair addition) had fired yet, but an earlier freeze today also
    // happened with ALL repair fully disabled, so the actual cause is
    // still unconfirmed. This isolates whether the drift-tracking READS
    // themselves (RelativeLocation for every attached child, every 300ms —
    // new load added tonight, wasn't happening before today) contribute,
    // independent of whether repair ever fires. Flip back to true once
    // this question is answered.
    static constexpr bool kEnableItemDriftCheck = true;
    if (kEnableItemDriftCheck && ctx->itemLastPos) {
        // 2026-08-15: absolute-position pass, throttled to 1/s per label —
        // see the comment further down for why this exists alongside the
        // delta check below.
        static std::unordered_map<std::string, uint64_t> s_lastAbsoluteCheckUs;
        const uint64_t nowUs = sdb::now_micros();
        uint64_t& lastAbsUs = s_lastAbsoluteCheckUs[ctx->label];
        const bool runAbsoluteThisPass = (nowUs - lastAbsUs >= 1'000'000ULL);
        if (runAbsoluteThisPass) lastAbsUs = nowUs;

        for (uintptr_t child : current) {
            UObject* childObj = reinterpret_cast<UObject*>(child);
            const auto* rel = reinterpret_cast<const double*>(child + 0x128);
            const double x = rel[0], y = rel[1], z = rel[2];

            UFunction* getSocketFn = childObj->GetFunctionByNameInChain(L"GetAttachSocketName");
            UFunction* attachFn = childObj->GetFunctionByNameInChain(L"K2_AttachToComponent");
            bool repaired = false;

            auto it = ctx->itemLastPos->find(child);
            if (it != ctx->itemLastPos->end() && getSocketFn && attachFn) {
                const double dx = x - it->second[0], dy = y - it->second[1], dz = z - it->second[2];
                constexpr double kDriftDistSq = 30.0 * 30.0;
                if (dx * dx + dy * dy + dz * dz > kDriftDistSq) {
                    RawFGameplayTag socket{}; // same raw {ComparisonIndex,Number} layout as FName
                    childObj->ProcessEvent(getSocketFn, &socket);
                    struct AttachParams {
                        UObject* Parent = nullptr;
                        RawFGameplayTag SocketName{};
                        uint8_t  LocationRule = 2;   // SnapToTarget
                        uint8_t  RotationRule = 2;   // SnapToTarget
                        uint8_t  ScaleRule = 1;       // KeepWorld
                        bool     WeldSimulatedBodies = true;
                        bool     ReturnValue = false;
                    } aparams;
                    aparams.Parent = ctx->mesh;
                    aparams.SocketName = socket;
                    childObj->ProcessEvent(attachFn, &aparams);
                    repaired = true;
                    debug_log("attach_health: " + ctx->label + " re-snapped DRIFTED child ptr=0x" +
                              std::to_string(child) + " drifted from (" +
                              std::to_string(it->second[0]) + "," + std::to_string(it->second[1]) + "," + std::to_string(it->second[2]) +
                              ") to (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) +
                              "), reattach returned " + std::to_string(aparams.ReturnValue));
                }
            }
            (*ctx->itemLastPos)[child] = { x, y, z };

            // 2026-08-15: ABSOLUTE check — the delta check above has the
            // exact same blind spot component_drift had before its
            // off-origin fix (see check_component_drift): a child that's
            // ALREADY misplaced the first time this scan observes it, or
            // one that lands wrong once and then never moves again,
            // produces no tick-to-tick delta and is invisible to the check
            // above. Confirmed live: PC2's debug.log showed proxy0's
            // AttachChildren count steady at 12 for 2.5+ minutes with zero
            // "re-snapped DRIFTED" hits, while a screenshot from that same
            // window showed PC1's helmet/weapon/axe visibly scattered on
            // PC2's screen — stationary-but-wrong, not drifting tick to
            // tick. Fix: instead of comparing to a stored baseline, compare
            // the child's actual world position against where its OWN
            // recorded socket currently sits right now
            // (USceneComponent::GetSocketLocation, confirmed present on
            // Engine.hpp's USceneComponent, same class GetAttachSocketName/
            // K2_AttachToComponent already come from) — catches a bad
            // position on the very first poll, no history required. Covers
            // weapon attachments (scope/mag/suppressor) for free since this
            // whole scan already runs one level deep into each equipped
            // item's own AttachChildren (see check_attach_health).
            // Throttled to 1/s/label since this adds two more reflection
            // calls per child on top of GetAttachSocketName above, and this
            // scan visits every attached child, both top-level and one
            // level deeper, every 300ms.
            if (repaired || !runAbsoluteThisPass || !getSocketFn || !attachFn) continue;
            UFunction* getSocketLocFn = ctx->mesh->GetFunctionByNameInChain(L"GetSocketLocation");
            UFunction* getCompLocFn   = childObj->GetFunctionByNameInChain(L"K2_GetComponentLocation");
            if (!getSocketLocFn || !getCompLocFn) continue;

            RawFGameplayTag socket{};
            childObj->ProcessEvent(getSocketFn, &socket);
            if (socket.ComparisonIndex == 0) continue; // NAME_None — not socket-attached, nothing to compare against

            struct SocketLocParams { RawFGameplayTag InSocketName{}; FVector ReturnValue{}; } slParams;
            slParams.InSocketName = socket;
            ctx->mesh->ProcessEvent(getSocketLocFn, &slParams);

            struct CompLocParams { FVector ReturnValue{}; } clParams;
            childObj->ProcessEvent(getCompLocFn, &clParams);

            const double sdx = clParams.ReturnValue.X - slParams.ReturnValue.X;
            const double sdy = clParams.ReturnValue.Y - slParams.ReturnValue.Y;
            const double sdz = clParams.ReturnValue.Z - slParams.ReturnValue.Z;
            constexpr double kSocketDistSq = 30.0 * 30.0;
            if (sdx * sdx + sdy * sdy + sdz * sdz > kSocketDistSq) {
                struct AttachParams {
                    UObject* Parent = nullptr;
                    RawFGameplayTag SocketName{};
                    uint8_t  LocationRule = 2;
                    uint8_t  RotationRule = 2;
                    uint8_t  ScaleRule = 1;
                    bool     WeldSimulatedBodies = true;
                    bool     ReturnValue = false;
                } aparams;
                aparams.Parent = ctx->mesh;
                aparams.SocketName = socket;
                childObj->ProcessEvent(attachFn, &aparams);
                debug_log("attach_health: " + ctx->label + " re-snapped OFF-SOCKET child ptr=0x" +
                          std::to_string(child) + " was " +
                          std::to_string(std::sqrt(sdx * sdx + sdy * sdy + sdz * sdz)) +
                          " units from its own socket, reattach returned " + std::to_string(aparams.ReturnValue));
            }
        }
    }
}

// Component-level primitive, usable on any USceneComponent-shaped pointer —
// not just a character's Mesh. A child returned by walking Mesh's
// AttachChildren is itself the attached actor's own root component (UE's
// K2_AttachToComponent attaches component-to-component, not actor-to-actor),
// so the exact same scan/diff logic applies one level deeper for free: a
// weapon's own root component has its own AttachChildren for whatever's
// attached to *it* (scope, mag, suppressor, laser). Returns the current
// children list so the caller can recurse into each one.
static std::vector<uintptr_t> check_attach_health_component(
    const std::string& key, UObject* component,
    std::unordered_map<std::string, std::vector<uintptr_t>>& snapshots,
    std::unordered_map<uintptr_t, bool>& itemHadMesh,
    std::unordered_map<uintptr_t, std::array<double, 3>>& itemLastPos)
{
    if (!component) return {};
    AttachHealthCtx ctx{ component, &snapshots[key], key, &itemHadMesh, &itemLastPos };
    if (!seh_invoke(do_attach_health_scan, &ctx))
        debug_log("attach_health: " + key + " scan crashed, caught via SEH");
    return snapshots[key]; // do_attach_health_scan leaves prev == current children on success
}

static void check_attach_health(const std::string& label, AActor* actor,
                                 std::unordered_map<std::string, std::vector<uintptr_t>>& snapshots,
                                 std::unordered_map<uintptr_t, bool>& itemHadMesh,
                                 std::unordered_map<uintptr_t, std::array<double, 3>>& itemLastPos)
{
    if (!actor) return;
    auto** meshSlot = static_cast<UObject**>(actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return;

    std::vector<uintptr_t> weaponsAndItems = check_attach_health_component(label, mesh, snapshots, itemHadMesh, itemLastPos);

    // One level deeper: each equipped item's own attachments (weapon
    // scopes/mags/suppressors, per the same JigPickupComponent/socket
    // mechanism already used by read_local_weapon_attachments). Sanity-
    // capped the same way the top level already is, inside the shared scan.
    for (uintptr_t child : weaponsAndItems) {
        char hex[24];
        snprintf(hex, sizeof(hex), "%llx", static_cast<unsigned long long>(child));
        std::string childKey = label + ">" + hex;
        check_attach_health_component(childKey, reinterpret_cast<UObject*>(child), snapshots, itemHadMesh, itemLastPos);
    }
}

// Component-drift monitor (2026-08-14) — sibling to attach_health above,
// covering the case attach_health structurally can't: hair (and every other
// base body part — head/Torso/Arms/Hands/Legs/Feet/Clothing_*) is a *named
// direct property* on the character (`BP_PlayerCharacter.hpp`, real
// reflected offsets, not a guess: `HairMesh` @0x07C0, `BeardMesh` @0x07C8,
// `head` @0x0778, etc.), never spawned/attached/destroyed the way equipped
// items are — so it can never show up as an AttachChildren add/remove.
// PC2's hair loss produced zero attach_health hits, confirming this is a
// different mechanism. These components should always sit at a small,
// roughly-constant offset from the pawn's own Mesh (RelativeLocation,
// USceneComponent+0x128 — same already-proven offset used throughout
// proxy_manager.cpp for exactly this field). If the mesh-fragmentation
// symptom (pieces visibly floating apart, per this session's screenshot
// evidence) is really happening, whatever's wrong should show up as a large
// jump in one of these components' RelativeLocation, not a null pointer —
// the component itself is a permanent part of the character, only its
// transform would visibly "fall away."
struct ComponentDriftCtx {
    UObject* comp; double lastX, lastY, lastZ; bool hasLast; std::string key;
    bool hadMesh = false; bool meshChecked = false; // tri-state: not-yet-checked / had-asset / asset-was-null
    AActor* owner = nullptr;       // needed to call UpdateBodyParts on repair — see below
    int32_t bodyPartCi = 0;        // FName ComparisonIndex for UpdateBodyParts' Name param, 0 = not repairable
    const wchar_t* clothingOnRepName = nullptr; // matching OnRep_ClothingXEquipped?, re-covers bare skin after repair
    const char* appearanceField = nullptr; // 2026-08-15: "hair"/"beard"/"eyebrows"/"mouth"/"hands", nullptr = not appearance-repairable
    // 2026-08-15: HairMesh/BeardMesh/EyebrowsMesh/Mouth are attached to a
    // real skeleton SOCKET ("head" or "eyebrows" — confirmed via the
    // FModel export's SCS_Node "AttachToName" field, not a guess), unlike
    // Torso/Legs/Feet/Hands/every Clothing_* slot which are plain direct
    // children of Mesh with no AttachToName at all. The origin-based check
    // below assumed EVERY tracked component should sit at (0,0,0) relative
    // to Mesh — true for the direct children, but wrong for these four: a
    // socket-attached component's natural resting RelativeLocation is
    // whatever small authored offset places it correctly on that socket
    // (e.g. Mouth reads a rock-steady (0,157,0.6) on every healthy join,
    // never (0,0,0)). Confirmed live 2026-08-15: this check was firing on
    // literally every single join, forcing Mouth from its correct (0,157,
    // 0.6) down to (0,0,0) — and a screenshot taken 4 minutes after that
    // "repair" (with zero further drift logged since) still showed the
    // mouth floating away near the ceiling, proving the write had no
    // bearing on the real visual position at all. Non-null here switches
    // do_component_drift_scan to the same GetSocketLocation-based absolute
    // check already proven for item attachments (see do_attach_health_scan)
    // instead of the origin check.
    const wchar_t* expectedSocket = nullptr;
    int repairAttempts = 0;        // capped — see do_component_drift_scan's repair-call comment
    // 2026-08-14, second root-cause pass: firstSeenUs anchors a grace period
    // before repair starts counting against the cap at all, lastRepairAttemptUs
    // throttles actual repair calls to once/second instead of once per 300ms
    // poll. Needed because gating the "local" scan on equipDataReady (see
    // state.hpp) only fixed the false-alarm half of this — the base body
    // mesh (Torso/Legs/Feet) turns out to finish loading on its own
    // independent timer from RepActorsData, and the old 5-attempts-at-300ms
    // cap (1.5s total) was exhausted before that real load ever completed,
    // confirmed live: "giving up after 5" fired ~1.3s after the scan resumed
    // post-gate, well before the base mesh had actually settled.
    uint64_t firstSeenUs = 0;
    uint64_t lastRepairAttemptUs = 0;
    bool loggedWallClockGiveUp = false; // 2026-08-15, log the 5-min hard-ceiling give-up exactly once
};

// UpdateBodyParts(FName Name) repair call (2026-08-14) — traced live via
// bytecode_dump.flag: BP_PlayerCharacter_C::UpdateBodyParts dispatches by
// name ("Torso"/"Legs"/"Feet", resolved via resolve_fname) to call
// SetSkinnedAssetAndUpdate on the matching component, the exact real native
// function that (re-)applies a body part's mesh — same call shape
// OnRep_ClothingLegsEquipped uses for the clothing-overlay equivalent.
// FName param passed as a raw {ComparisonIndex, Number} struct, matching
// this project's own established pattern (resolve_fname's own
// implementation, slot_tag()'s GameplayTag constants) — no FName-from-
// string constructor exists in the vendored SDK. Hardcoded CI values below
// were resolved live this session and should be stable: unlike GameplayTag
// registration (proven unstable across restarts, see
// [[feedback_sdo_gameplaytag_ci_unstable]]), these are compile-time string
// literals baked into the shipped build's Kismet bytecode, deterministically
// ordered by the cooked global name table, not runtime DataTable load order.
struct BodyPartRepairCtx { AActor* owner; int32_t ci; const std::string* key; const wchar_t* clothingOnRepName; UObject* comp; };

// UpdateBodyParts alone reapplies the BARE body mesh only — live-reported
// 2026-08-14: after repair, bare skin showed through gaps in the pants
// texture, because UpdateBodyParts has no idea a clothing overlay should be
// covering that body part. `BodyPartVisibility` (the function that
// presumably handles this properly) is 2800 bytes and takes a complex
// struct param not reverse-engineered this session — too risky to call
// blind. Safer fix: also call the matching parameterless `OnRep_ClothingX
// Equipped?` callback (already fully decoded for Legs — reads its own
// instance state, calls SetSkinnedAssetAndUpdate with the correct
// Male/Female clothing mesh, the real logic that should re-cover the bare
// part). No struct construction needed, just ProcessEvent with null params.
// Forward-declared — real definition (and the recent-calls ring buffer it
// dumps) lives down near on_process_event_pre, needed here and in
// do_component_drift_scan below.
static void dump_recent_calls();

static void do_body_part_repair(void* ctxRaw)
{
    auto* ctx = static_cast<BodyPartRepairCtx*>(ctxRaw);
    // 2026-08-15: ci==0 now means "clothing-overlay-only repair" (see the
    // Clothing_Torso/Legs/Feet kNames entries and check_component_drift's
    // relaxed repair gate) — UpdateBodyParts doesn't know these component
    // names at all (it only dispatches on "Torso"/"Legs"/"Feet"), so calling
    // it would be a wasted reflection call at best. Skip straight to the
    // clothing OnRep for these.
    UFunction* fn = ctx->ci != 0 ? ctx->owner->GetFunctionByNameInChain(L"UpdateBodyParts") : nullptr;
    if (ctx->ci != 0 && !fn) {
        debug_log("component_drift: " + *ctx->key + " UpdateBodyParts NOT FOUND");
        return;
    }
    // 2026-08-14, diagnostic addition: immediate before/after read of the
    // SAME mesh slot component_drift itself checks, to answer directly
    // whether this ProcessEvent call is landing at all or being fought by
    // something else — live-reported the same session: 14-20 consecutive
    // calls with zero effect on the LOCAL player (not just proxies, contrary
    // to this project's prior assumption).
    void** meshSlot = ctx->comp ? static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMesh")) : nullptr;
    if (meshSlot && !*meshSlot) meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset"));
    const void* before = meshSlot ? *meshSlot : nullptr;

    // 2026-08-14: an earlier attempt at dumping here (one debug_log call per
    // ring-buffer line) stalled the game thread for ~7 real seconds and was
    // reverted; dump_recent_calls/do_dump_recent_calls were then rewritten to
    // build one in-memory string and flush with a single debug_log call.
    // 2026-08-15: REMOVED the call from this hot path entirely — live-
    // reported "PC1 keeps freezing" after retry was made uncapped (see
    // ComponentDriftCtx's repairAttempts comment). With repair now firing
    // indefinitely once a character gets stuck fighting (confirmed live:
    // repair reverting every ~1.2s for several minutes straight), even the
    // "safe" single-flush version was walking all 65536 ring-buffer entries
    // and formatting up to 1000 function names roughly every 1-2s,
    // continuously, for as long as the fight lasted — a real, ongoing
    // per-tick cost, not a one-time diagnostic anymore. The ring-buffer
    // trace already gave what it could (nothing Blueprint-dispatched touches
    // the mesh in the gap — see the investigation log's native-cause
    // findings); no further marginal value from dumping it on every retry.
    // dump_recent_calls() is still available and still safe to call
    // ad-hoc/rarely (e.g. from a flag-triggered one-shot check) — just not
    // unconditionally from an uncapped retry loop.

    if (fn) {
        struct Params { int32_t ComparisonIndex; int32_t Number; } params{ ctx->ci, 0 };
        ctx->owner->ProcessEvent(fn, &params);
    }

    const void* immediatelyAfter = meshSlot ? *meshSlot : nullptr;
    debug_log("component_drift: " + *ctx->key +
              (fn ? (" called UpdateBodyParts ci=" + std::to_string(ctx->ci)) : std::string(" skipped UpdateBodyParts (clothing-only repair)")) +
              " meshBefore=0x" + std::to_string(reinterpret_cast<uintptr_t>(before)) +
              " meshImmediatelyAfter=0x" + std::to_string(reinterpret_cast<uintptr_t>(immediatelyAfter)));

    if (ctx->clothingOnRepName) {
        UFunction* clothFn = ctx->owner->GetFunctionByNameInChain(ctx->clothingOnRepName);
        if (clothFn) {
            ctx->owner->ProcessEvent(clothFn, nullptr);
            debug_log("component_drift: " + *ctx->key + " also called clothing OnRep to re-cover bare skin");
        } else {
            debug_log("component_drift: " + *ctx->key + " clothing OnRep function NOT FOUND");
        }
    }
}

// 2026-08-14, second extension same session: "gone" doesn't always mean
// detached or moved — PC2's own hands vanishing produced zero hits on
// either attach_health or the RelativeLocation-drift check above, meaning
// the component stayed attached and stayed in place, but presumably went
// invisible or had its mesh asset cleared instead. Reflection name lookup
// (not a raw offset) so a wrong guess just returns null, no crash risk —
// tries both plausible property names since the exact one on
// USkeletalMeshComponent for this engine version wasn't independently
// confirmed (UE renamed SkeletalMesh -> SkeletalMeshAsset around 5.1).
static void do_component_drift_scan(void* ctxRaw)
{
    auto* ctx = static_cast<ComponentDriftCtx*>(ctxRaw);
    auto* relLoc = reinterpret_cast<double*>(reinterpret_cast<uintptr_t>(ctx->comp) + 0x0128);
    const double x = relLoc[0], y = relLoc[1], z = relLoc[2];
    if (ctx->hasLast) {
        const double dx = x - ctx->lastX, dy = y - ctx->lastY, dz = z - ctx->lastZ;
        constexpr double kDriftDistSq = 30.0 * 30.0; // UE units — body parts should sit near-fixed relative to Mesh
        if (dx * dx + dy * dy + dz * dz > kDriftDistSq) {
            char line[320];
            snprintf(line, sizeof(line),
                "component_drift: %s DRIFTED from (%.1f,%.1f,%.1f) to (%.1f,%.1f,%.1f)",
                ctx->key.c_str(), ctx->lastX, ctx->lastY, ctx->lastZ, x, y, z);
            debug_log(line);
        }
    }
    ctx->lastX = x; ctx->lastY = y; ctx->lastZ = z;

    // 2026-08-15: the tick-to-tick check above has the same "no baseline on
    // first observation" blind spot as every other transition-only detector
    // tonight — live-confirmed via a screenshot of a proxy's ENTIRE body
    // scattered (head/hands/boots/torso all flung apart) while every one of
    // component_drift's own mesh-asset checks read `SET` the whole time —
    // the meshes were never cleared, they were positionally wrong from the
    // very first sample, so the delta check never had a "before" to compare
    // against. Fixed with an ABSOLUTE check instead, but which one depends
    // on whether this component is socket-attached (see
    // ComponentDriftCtx::expectedSocket) — plain direct children (Torso/
    // Legs/Feet/Hands/every Clothing_* slot) really do sit at (0,0,0), but
    // HairMesh/BeardMesh/EyebrowsMesh/Mouth do not: forcing THOSE to (0,0,0)
    // was confirmed live 2026-08-15 to fire on every single join yet have no
    // bearing on the real floating-away symptom (see expectedSocket's
    // comment) — this was editing a property that doesn't control their
    // actual rendered position at all.
    if (ctx->expectedSocket) {
        // Socket-attached path — same GetSocketLocation-based absolute
        // check already proven for item attachments in do_attach_health_scan:
        // compare the component's actual world position against where its
        // OWN recorded socket currently sits, not a hardcoded relative
        // target. Catches a bad position on the first poll, no baseline or
        // prior-good-value needed.
        auto** meshSlot = static_cast<UObject**>(ctx->owner->GetValuePtrByPropertyNameInChain(L"Mesh"));
        UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
        UFunction* getSocketFn   = ctx->comp->GetFunctionByNameInChain(L"GetAttachSocketName");
        UFunction* getSocketLocFn = mesh ? mesh->GetFunctionByNameInChain(L"GetSocketLocation") : nullptr;
        UFunction* getCompLocFn  = ctx->comp->GetFunctionByNameInChain(L"K2_GetComponentLocation");
        UFunction* attachFn      = ctx->comp->GetFunctionByNameInChain(L"K2_AttachToComponent");
        if (mesh && getSocketFn && getSocketLocFn && getCompLocFn && attachFn) {
            RawFGameplayTag socket{};
            ctx->comp->ProcessEvent(getSocketFn, &socket);
            if (socket.ComparisonIndex != 0) { // NAME_None means never attached — nothing to compare against
                struct SocketLocParams { RawFGameplayTag InSocketName{}; FVector ReturnValue{}; } slParams;
                slParams.InSocketName = socket;
                mesh->ProcessEvent(getSocketLocFn, &slParams);

                struct CompLocParams { FVector ReturnValue{}; } clParams;
                ctx->comp->ProcessEvent(getCompLocFn, &clParams);

                const double sdx = clParams.ReturnValue.X - slParams.ReturnValue.X;
                const double sdy = clParams.ReturnValue.Y - slParams.ReturnValue.Y;
                const double sdz = clParams.ReturnValue.Z - slParams.ReturnValue.Z;
                constexpr double kSocketDistSq = 50.0 * 50.0;
                if (sdx * sdx + sdy * sdy + sdz * sdz > kSocketDistSq) {
                    const uint64_t nowUs = sdb::now_micros();
                    if (ctx->firstSeenUs == 0) ctx->firstSeenUs = nowUs;
                    if (nowUs - ctx->firstSeenUs >= 2'000'000ULL &&
                        nowUs - ctx->lastRepairAttemptUs >= 1'000'000ULL) {
                        ctx->lastRepairAttemptUs = nowUs;
                        struct AttachParams {
                            UObject* Parent = nullptr;
                            RawFGameplayTag SocketName{};
                            uint8_t  LocationRule = 2;   // SnapToTarget
                            uint8_t  RotationRule = 2;   // SnapToTarget
                            uint8_t  ScaleRule = 1;       // KeepWorld
                            bool     WeldSimulatedBodies = true;
                            bool     ReturnValue = false;
                        } aparams;
                        aparams.Parent = mesh;
                        aparams.SocketName = socket;
                        ctx->comp->ProcessEvent(attachFn, &aparams);
                        debug_log("component_drift: " + ctx->key + " re-snapped OFF-SOCKET component (was " +
                                  std::to_string(std::sqrt(sdx * sdx + sdy * sdy + sdz * sdz)) +
                                  " units from its own socket), reattach returned " + std::to_string(aparams.ReturnValue));
                    }
                }
            }
        }
    } else if (x * x + y * y + z * z > 50.0 * 50.0) {
        const uint64_t nowUs = sdb::now_micros();
        if (ctx->firstSeenUs == 0) ctx->firstSeenUs = nowUs;
        if (nowUs - ctx->firstSeenUs >= 2'000'000ULL &&
            nowUs - ctx->lastRepairAttemptUs >= 1'000'000ULL) {
            ctx->lastRepairAttemptUs = nowUs;
            UFunction* setRelFn = ctx->comp->GetFunctionByNameInChain(L"K2_SetRelativeLocation");
            if (setRelFn) {
                struct Params { FVector NewLocation; bool bSweep; FHitResult SweepHitResult; bool bTeleport; } params{};
                params.NewLocation = FVector{ 0.0, 0.0, 0.0 };
                params.bSweep = false;
                params.bTeleport = true;
                ctx->comp->ProcessEvent(setRelFn, &params);
                debug_log("component_drift: " + ctx->key + " re-centered OFF-ORIGIN component from (" +
                          std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ") to (0,0,0)");
            }
        }
    }

    // 2026-08-14: added "StaticMesh" — HairMesh/BeardMesh/EyebrowsMesh/Mouth
    // are all UStaticMeshComponent (BP_PlayerCharacter.hpp), not skeletal.
    // Neither "SkeletalMesh" nor "SkeletalMeshAsset" ever matches on those,
    // meaning this check has been silently unable to see hair/beard/
    // eyebrows/mouth going missing all session despite repeated live
    // reports — meshSlot was always null for them, so the whole block below
    // never ran. Real diagnostic gap, not evidence those components were
    // actually fine.
    void** meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMesh"));
    if (!meshSlot) meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset"));
    if (!meshSlot) meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"StaticMesh"));
    if (meshSlot) {
        const bool hasMeshNow = (*meshSlot != nullptr);
        // 2026-08-15: transition-only detection ("was set, now null") has a
        // real blind spot — live-reported same day: a part missing on the
        // very FIRST sample after a fresh join/respawn baseline reset never
        // gets flagged at all, since there's no "was set" to compare against.
        // Two same-day reports (PC1's gear seen on PC2's proxy right after
        // PC1 relaunched, PC2's own Hands right after her own join) both
        // produced zero hits in this detector, consistent with exactly this
        // gap. Log-only, no repair attempted here — this is visibility to
        // confirm/deny the theory, not a fix.
        if (!ctx->meshChecked) {
            debug_log(std::string("component_drift: ") + ctx->key +
                      " initial post-join state = " + (hasMeshNow ? "SET" : "MISSING"));
        }
        if (ctx->meshChecked && ctx->hadMesh && !hasMeshNow) {
            debug_log("component_drift: " + ctx->key + " MESH ASSET CLEARED (was set, now null)");
            dump_recent_calls();
        }
        // Repair, not just log — self-healing, same philosophy as
        // equip_restore_retry. Fires whenever the mesh reads null and this
        // component is one of the three UpdateBodyParts actually covers
        // (Torso/Legs/Feet, confirmed via bytecode trace) — intended to be
        // self-limiting since a successful repair should make hasMeshNow
        // true on the very next check, stopping further calls. Live-tested
        // 2026-08-14: on a PROXY, this call fired every single check (every
        // 2s) indefinitely without ever succeeding — UpdateBodyParts likely
        // has client-authority gating EquipActorToSocket doesn't (that one
        // is already proven to work cross-network for proxies elsewhere in
        // this project; this one apparently isn't the same).
        //
        // 2026-08-14, second pass same day: the original 5-attempts-at-
        // 300ms cap (1.5s total) was tuned around the OLD always-scanning
        // behavior, where the mesh being null was usually already a real,
        // long-settled failure by the time it was noticed. Gating the scan
        // on equipDataReady (state.hpp) means repair can now start almost
        // immediately after a fresh join/respawn, while the base body mesh
        // is still legitimately mid-load on its own timer (independent of
        // RepActorsData) — confirmed live: the cap was exhausted ~1.3s after
        // scanning resumed, well before the mesh actually finished loading.
        // Fixed with a 2s no-repair grace from firstSeenUs (this component's
        // first-ever sighting under the current key/actor) so a still-
        // loading mesh isn't immediately treated as broken, and throttling
        // actual repair attempts to once per second (instead of once per
        // 300ms poll).
        //
        // 2026-08-14, third pass same day: removed the attempt cap entirely
        // for LOCAL (owner-driven) repair. Live-confirmed via a before/after
        // mesh-pointer read that each individual call genuinely lands (sets
        // a valid non-null mesh asset) every single time — the failure mode
        // isn't the call not working, it's something else re-clearing the
        // mesh again within about a second afterward (still unidentified —
        // an attempt to trace it via the recent-calls ring buffer was
        // reverted, see do_body_part_repair's comment, for stalling the game
        // thread ~7s per dump). Since each attempt is cheap and harmless
        // (proven safe, no crashes, no leak — just a reflection call), and
        // giving up leaves the character permanently broken until a manual
        // fix, retrying forever seemed strictly better than capping while the
        // real cause was still being investigated.
        //
        // 2026-08-15, fourth pass: REVERTED to a bounded cap. Live-reported
        // same day: PC1 hung completely (unresponsive, debug.log itself
        // stopped growing — a real engine-thread freeze, not just a UI
        // hang) during a live test, with the freeze landing almost exactly
        // on the next expected repair tick (last successful log line at
        // T, freeze at T+~1.2s, matching this loop's 1/s cadence) while a
        // character was actively stuck fighting. Uncapped retry means up to
        // 3 components × 2 ProcessEvent calls each (UpdateBodyParts +
        // clothing OnRep) firing once/second FOREVER once a character gets
        // stuck — never previously exercised for more than a few minutes
        // continuous before tonight. Root cause of the freeze itself not
        // confirmed (could be resource accumulation from the repeated
        // reflection calls, or an interaction with some other system), but
        // the correlation is strong enough not to keep running this
        // unbounded while unconfirmed. Capped at 60 CONSECUTIVE attempts.
        //
        // 2026-08-15, fifth pass, same day: the consecutive-attempt cap
        // turned out not to actually bound anything — live-reconfirmed
        // ANOTHER freeze (this time correlated with PC2 joining, not pawn
        // movement) after 311 total repair cycles over several minutes,
        // well past the supposed 60-attempt ceiling, with "giving up after
        // 60" logged only ONCE early on. Root cause of the cap not holding:
        // `repairAttempts` resets to 0 whenever `hasMeshNow` reads true at
        // scan-start — and since our own repair briefly sets a valid mesh
        // before it reverts (~1s later), the 300ms poll sometimes catches
        // that brief fixed window and resets the counter, letting the total
        // attempt count climb indefinitely across a whole flapping session
        // even though no single unbroken streak ever reaches 60. Fixed with
        // a SEPARATE hard wall-clock ceiling anchored to `firstSeenUs`
        // (never reset by a transient hasMeshNow flicker, only by an actual
        // actor change/respawn) — once 5 real minutes have passed since
        // this component was first seen broken, stop attempting entirely,
        // full stop, regardless of the consecutive counter's state. The
        // consecutive cap is kept too (still a reasonable secondary bound).
        // The cap still applies implicitly to proxies via a different path
        // (this whole "local" scan doesn't run against proxies in the first
        // place — see check_attach_health_trigger).
        // 2026-08-15, isolation test: repeated freezes kept recurring across
        // three different cap tunings, which stopped being informative — a
        // broken character (and therefore an active repair loop) is present
        // in essentially every play session anyway, so "froze while repair
        // was running" doesn't actually discriminate between "repair causes
        // it" and "something else causes it while repair happens to also be
        // running." Kill-switched the actual ProcessEvent repair calls
        // (detection/logging above this point stays fully active) to test
        // in isolation whether freezing stops. Flip back to true once this
        // question is answered either way — do not leave this false
        // permanently without a decision either way logged here.
        // 2026-08-15: re-enabled. Isolation test (repair OFF) still froze,
        // proving repair wasn't the cause. Real cause found and fixed
        // separately: bReinitPose=true on the SetSkinnedAssetAndUpdate calls
        // in proxy_manager.cpp (see its own comment) — wrong vs. the real
        // game's own proven call shape (bReinitPose=false). UpdateBodyParts
        // itself (called from here) already dispatches internally with
        // False per its own decoded bytecode, so this loop was never the
        // direct culprit either way.
        static constexpr bool kEnableBodyPartRepairCalls = true;
        // 2026-08-15: isolation test, paired with kEnableItemDriftCheck
        // above — same reasoning, same "flip back once answered" note.
        static constexpr bool kEnableAppearanceRepair = true;
        const uint64_t nowUs = sdb::now_micros();
        if (ctx->firstSeenUs == 0) ctx->firstSeenUs = nowUs;
        const bool wallClockExpired = nowUs - ctx->firstSeenUs >= 300'000'000ULL; // 5 min hard ceiling
        if (hasMeshNow) {
            ctx->repairAttempts = 0;
        } else if (kEnableBodyPartRepairCalls && ctx->owner &&
                   (ctx->bodyPartCi != 0 || ctx->clothingOnRepName ||
                    (kEnableAppearanceRepair && ctx->appearanceField)) &&
                   nowUs - ctx->firstSeenUs >= 2'000'000ULL &&
                   ctx->repairAttempts < 60 && !wallClockExpired &&
                   nowUs - ctx->lastRepairAttemptUs >= 1'000'000ULL) {
            ctx->lastRepairAttemptUs = nowUs;
            ctx->repairAttempts++;
            // 2026-08-15: appearanceField entries (HairMesh/BeardMesh/
            // EyebrowsMesh/Mouth/Hands) go through a DIFFERENT repair path —
            // UpdateBodyParts doesn't know these component names at all, so
            // do_body_part_repair (built for Torso/Legs/Feet + Clothing_*)
            // doesn't apply. Proxy: force the existing, already-correct
            // sync_pawn_appearance to fully re-run (reapplies everything at
            // once, including this field). Local: no equivalent "resync"
            // mechanism exists, so reapply just this one component directly
            // from the cached last-good appearance (state.hpp's
            // lastGoodLocalAppearance) via the same SetStaticMesh/
            // SetSkinnedAssetAndUpdate call proxy sync uses.
            if (ctx->appearanceField) {
                const bool isProxy = ctx->key.rfind("proxy", 0) == 0;
                if (isProxy) {
                    const bool matched = sdb::g_proxy_manager().force_resync_appearance(ctx->owner);
                    debug_log("component_drift: " + ctx->key + " forced proxy appearance resync, matched=" +
                              std::to_string(matched));
                } else {
                    const auto& good = sdb::g_state().lastGoodLocalAppearance;
                    const std::string* name = nullptr;
                    bool isSkeletal = false;
                    const std::string f = ctx->appearanceField;
                    if (f == "hair")          { name = &good.hairMeshName; isSkeletal = false; }
                    else if (f == "beard")    { name = &good.beardMeshName; isSkeletal = false; }
                    else if (f == "eyebrows") { name = &good.eyebrowsMeshName; isSkeletal = false; }
                    else if (f == "mouth")    { name = &good.mouthMeshName; isSkeletal = false; }
                    else if (f == "hands")    { name = &good.bodyPartMeshNames[8]; isSkeletal = true; }
                    const bool applied = name && sdb::reapply_named_mesh(ctx->comp, *name, isSkeletal);
                    debug_log("component_drift: " + ctx->key + " local appearance repair field=" + f +
                              " cachedName=" + (name ? *name : "<none>") + " applied=" + std::to_string(applied));
                }
            } else {
                BodyPartRepairCtx repairCtx{ ctx->owner, ctx->bodyPartCi, &ctx->key, ctx->clothingOnRepName, ctx->comp };
                if (!seh_invoke(do_body_part_repair, &repairCtx))
                    debug_log("component_drift: " + ctx->key + " repair crashed, caught via SEH");
            }
            if (ctx->repairAttempts == 60)
                debug_log("component_drift: " + ctx->key + " giving up after 60 CONSECUTIVE failed repair attempts (may resume if a transient poll sees it fixed)");
        }
        if (wallClockExpired && !ctx->loggedWallClockGiveUp) {
            ctx->loggedWallClockGiveUp = true;
            debug_log("component_drift: " + ctx->key + " giving up PERMANENTLY after 5 real minutes stuck (hard ceiling, not resettable)");
        }
        ctx->hadMesh = hasMeshNow;
        ctx->meshChecked = true;
    }
    ctx->hasLast = true;
}

// Forward-declared — real definition lives down near the recent-calls ring
// buffer, needed here to refresh its object watch-list every "local" scan.
static void set_recent_calls_watch(UObject* pawn, UObject* torso, UObject* legs, UObject* feet, UObject* helper);

static void check_component_drift(const std::string& label, AActor* actor,
                                   std::unordered_map<std::string, ComponentDriftCtx>& snapshots)
{
    if (!actor) return;
    // Representative subset covering tonight's actual reports (hair,
    // respirator-adjacent head slot, general body) without scanning every
    // clothing overlay — extend this list if a future report names a
    // component not covered here.
    // bodyPartCi: FName ComparisonIndex for UpdateBodyParts' Name param
    // (resolved live via resolve_fname.flag this session), 0 = not one of
    // the three names UpdateBodyParts' decoded bytecode actually covers —
    // repair is skipped for those, log-only same as before.
    // clothingOnRepName: the matching OnRep_ClothingXEquipped? callback
    // (real properties, confirmed via BP_PlayerCharacter.hpp) — called
    // after UpdateBodyParts to re-cover bare skin with the actual worn
    // clothing mesh, not just leave the repaired-but-nude body part
    // showing (live-reported 2026-08-14: skin visible through pants gaps
    // after a repair with only UpdateBodyParts called).
    // 2026-08-15: added `appearanceField` — identifies which
    // sdb::PawnAppearance field this component corresponds to, for the new
    // appearance-repair path (HairMesh/BeardMesh/EyebrowsMesh/Mouth/Hands,
    // none of which UpdateBodyParts covers by name). nullptr = not
    // appearance-repairable (unchanged behavior for those entries).
    // socket column added 2026-08-15 — see ComponentDriftCtx::expectedSocket
    // for why this matters. Confirmed via the FModel export's SCS_Node
    // "AttachToName" field (Exports/.../BP_PlayerCharacter.json): HairMesh/
    // BeardMesh/Mouth all attach to "head", EyebrowsMesh to "eyebrows".
    // Every other row below (including Hands, a SkeletalMeshComponent) has
    // no AttachToName in the export at all — plain direct children, origin
    // check is correct for those.
    static const struct { const wchar_t* w; const char* n; int32_t ci; const wchar_t* clothingOnRep; const char* appearanceField; const wchar_t* socket; } kNames[] = {
        { L"HairMesh", "HairMesh", 0, nullptr, "hair", L"head" }, { L"BeardMesh", "BeardMesh", 0, nullptr, "beard", L"head" }, { L"head", "head", 0, nullptr, nullptr, nullptr },
        { L"Torso", "Torso", 1732710, L"OnRep_ClothingTorsoEquipped?", nullptr, nullptr },
        { L"Arms", "Arms", 0, nullptr, nullptr, nullptr }, { L"Hands", "Hands", 0, nullptr, "hands", nullptr },
        { L"Legs", "Legs", 1732718, L"OnRep_ClothingLegsEquipped?", nullptr, nullptr },
        { L"Feet", "Feet", 1732721, L"OnRep_ClothingFeetEquipped?", nullptr, nullptr },
        // Added 2026-08-14 after a live report named these specifically
        // (weapon attachments/eyebrows/mouth missing after a join-time
        // cascade that only partially self-corrected) — real properties,
        // BP_PlayerCharacter.hpp @0x0790/@0x0740, both UStaticMeshComponent
        // (still USceneComponent-derived, same +0x128 RelativeLocation
        // offset applies).
        { L"EyebrowsMesh", "EyebrowsMesh", 0, nullptr, "eyebrows", L"eyebrows" }, { L"Mouth", "Mouth", 0, nullptr, "mouth", L"head" },
        // Added 2026-08-15 after a live report: PC2 spawned in with base
        // Torso/Legs/Feet all present (confirmed via the initial-post-join-
        // state logging — no base-mesh clear at all this occurrence) but
        // skin visible through her clothing — the CLOTHING OVERLAY
        // (Clothing_Torso/Legs/Feet, BP_PlayerCharacter.hpp @0x0770/@0x0768/
        // @0x0760) failed independently of the base body mesh, a distinct
        // failure this table never tracked or repaired on its own before —
        // previously the clothing OnRep only ever got called as a side
        // effect of a base-mesh repair, so a clothing-only failure with a
        // healthy base mesh had no detection AND no repair path at all.
        // ci=0 (UpdateBodyParts doesn't cover these — it only knows Torso/
        // Legs/Feet by name, not the Clothing_ variants), clothingOnRep set
        // so do_body_part_repair's now-relaxed gate (ci==0 but
        // clothingOnRep non-null) calls just the OnRep, no UpdateBodyParts.
        { L"Clothing_Torso", "Clothing_Torso", 0, L"OnRep_ClothingTorsoEquipped?", nullptr, nullptr },
        { L"Clothing_Legs", "Clothing_Legs", 0, L"OnRep_ClothingLegsEquipped?", nullptr, nullptr },
        { L"Clothing_Feet", "Clothing_Feet", 0, L"OnRep_ClothingFeetEquipped?", nullptr, nullptr },
        // Added 2026-08-15: Clothing_Gloves/Clothing_Armor (BP_PlayerCharacter.hpp
        // @0x0780/@0x07B8) were never tracked at all — every OTHER clothing
        // slot got added over the course of tonight except these two.
        // OnRep_ClothingGlovesEquipped?/OnRep_ClothingArmorEquipped? are a
        // guess following the exact naming convention already confirmed live
        // for Torso/Legs/Feet — GetFunctionByNameInChain safely returns
        // null and do_body_part_repair logs "NOT FOUND" if the guess is
        // wrong, same harmless fallback already proven throughout this file.
        { L"Clothing_Gloves", "Clothing_Gloves", 0, L"OnRep_ClothingGlovesEquipped?", nullptr, nullptr },
        { L"Clothing_Armor", "Clothing_Armor", 0, L"OnRep_ClothingArmorEquipped?", nullptr, nullptr },
    };
    UObject* torsoComp = nullptr; UObject* legsComp = nullptr; UObject* feetComp = nullptr;
    for (const auto& entry : kNames) {
        auto** slot = static_cast<UObject**>(actor->GetValuePtrByPropertyNameInChain(entry.w));
        UObject* comp = (slot && *slot) ? *slot : nullptr;
        if (!comp) continue;
        if (entry.ci == 1732710) torsoComp = comp;
        else if (entry.ci == 1732718) legsComp = comp;
        else if (entry.ci == 1732721) feetComp = comp;

        std::string key = label + ":" + entry.n;
        ComponentDriftCtx& ctx = snapshots[key];
        ctx.clothingOnRepName = entry.clothingOnRep;
        ctx.appearanceField = entry.appearanceField;
        ctx.expectedSocket = entry.socket;
        ctx.comp = comp;
        ctx.key  = key;
        ctx.owner = actor;
        ctx.bodyPartCi = entry.ci;
        if (!seh_invoke(do_component_drift_scan, &ctx))
            debug_log("component_drift: " + key + " scan crashed, caught via SEH");
    }
    // See set_recent_calls_watch's comment (near the ring buffer) for why
    // this is filtered to just these objects — only bother for "local",
    // proxies don't drive this investigation.
    if (label == "local") {
        UObject* helper = nullptr;
        if (const auto helperAddr = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(actor) + 0x700))
            helper = reinterpret_cast<UObject*>(helperAddr);
        set_recent_calls_watch(reinterpret_cast<UObject*>(actor), torsoComp, legsComp, feetComp, helper);
    }
}

// 2026-08-14, false-positive fix: snapshots are keyed by a fixed label
// ("local"/"proxyN"), which stays the same across a respawn or reconnect
// even though the underlying AActor* is now a completely different, freshly
// -spawned pawn whose components haven't finished initializing yet (mesh
// assets legitimately null for a moment). Without this, the very first
// check after any respawn/rejoin compared the new pawn's not-yet-loaded
// state against the old pawn's fully-loaded snapshot and reported a false
// "MESH ASSET CLEARED"/"DETACHED" — confirmed live: PC2's local pawn went
// NULL in two heartbeats (reconnect in progress), then the instant it came
// back a whole cascade fired (Torso/Legs/Feet + two item attachments, all
// in the same millisecond) — a respawn-init artifact, not a real bug.
// Fixed by tracking which actor each label last referred to and wiping
// every snapshot entry under that label whenever the actor identity itself
// changes, so the next check is treated as a fresh baseline instead of a
// comparison.
// Returns true if a stale-snapshot reset was needed (caller must also wipe
// s_itemHadMesh — see its own call sites below for why that map couldn't be
// scoped/cleared here directly).
static bool reset_label_snapshots_if_actor_changed(
    const std::string& label, AActor* actor,
    std::unordered_map<std::string, AActor*>& lastActorForLabel,
    std::unordered_map<std::string, std::vector<uintptr_t>>& snapshots,
    std::unordered_map<std::string, ComponentDriftCtx>& driftSnapshots)
{
    auto it = lastActorForLabel.find(label);
    const bool changed = (it == lastActorForLabel.end()) ? (actor != nullptr) : (it->second != actor);
    if (!changed) return false;
    lastActorForLabel[label] = actor;
    if (it == lastActorForLabel.end() || it->second == nullptr) return false; // first-ever sighting, nothing stale to clear

    for (auto sit = snapshots.begin(); sit != snapshots.end(); ) {
        if (sit->first == label || sit->first.rfind(label + ">", 0) == 0) sit = snapshots.erase(sit);
        else ++sit;
    }
    for (auto dit = driftSnapshots.begin(); dit != driftSnapshots.end(); ) {
        if (dit->first.rfind(label + ":", 0) == 0) dit = driftSnapshots.erase(dit);
        else ++dit;
    }
    debug_log("attach_health: " + label + " actor changed (respawn/reconnect), snapshot baseline reset");
    return true;
}

static void check_attach_health_trigger()
{
    static uint64_t s_lastCheckUs = 0;
    static std::unordered_map<std::string, std::vector<uintptr_t>> s_snapshots;
    static std::unordered_map<std::string, ComponentDriftCtx> s_driftSnapshots;
    static std::unordered_map<uintptr_t, bool> s_itemHadMesh;
    static std::unordered_map<uintptr_t, std::array<double, 3>> s_itemLastPos;
    static std::unordered_map<std::string, AActor*> s_lastActorForLabel;
    const uint64_t now = sdb::now_micros();
    // 2026-08-14: tightened from 2s to 300ms specifically to pair with the
    // new recent-calls ring buffer — a smaller gap between the actual clear
    // event and this check noticing it means the dump (when triggered)
    // still has the real causal calls in it, not just several seconds of
    // unrelated activity that happened after. More frequent reflection
    // property reads, acceptable tradeoff for tonight's active debugging.
    if (now - s_lastCheckUs < 300'000ULL) return;
    s_lastCheckUs = now;

    AActor* localPawn = find_local_pawn();
    // 2026-08-15: unambiguous hex-formatted pawn-pointer heartbeat, added
    // specifically for live IDA work — every other pointer this file logs
    // goes through debug_log("...0x" + std::to_string(ptr)), which prints
    // DECIMAL digits behind a misleading "0x" prefix (std::to_string never
    // produces hex). Live-confirmed this got misread as hex during an IDA
    // session, garbage-read memory as a result. This one uses real %llx so
    // it can be trusted directly. Low frequency (10s) — this is a debugging
    // aid, not meant to be permanent log spam.
    if (localPawn) {
        static uint64_t s_lastPawnPtrLogUs = 0;
        if (now - s_lastPawnPtrLogUs >= 10'000'000ULL) {
            s_lastPawnPtrLogUs = now;
            char line[96];
            snprintf(line, sizeof(line), "pawn_ptr_hex: local pawn=0x%llx",
                     static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(localPawn)));
            debug_log(line);
        }
    }
    // s_itemHadMesh is keyed by raw item pointer only (not by label — an
    // item's own identity, not which character owns it), so it can't be
    // scoped/cleared per-label the way the other two maps are above. A
    // respawn/reconnect can hand a freshly-spawned item the *same* memory
    // address a just-destroyed one had (confirmed live 2026-08-14: this
    // exact gap produced a second false-positive cascade even after the
    // per-label reset above shipped), so any actor-change anywhere means the
    // whole map's "was this pointer's mesh set" history is no longer
    // trustworthy — simplest correct fix is a full clear, not a targeted
    // one; cheap to rebuild (next tick or two) and runs at most a couple
    // times per respawn.
    if (reset_label_snapshots_if_actor_changed("local", localPawn, s_lastActorForLabel, s_snapshots, s_driftSnapshots)) {
        s_itemHadMesh.clear();
        s_itemLastPos.clear(); // same pointer-reuse-across-respawn risk as s_itemHadMesh
    }
    // 2026-08-14, root cause session: skip the "local" scan entirely until
    // equip_restore_retry has confirmed RepActorsData actually populated
    // (see state.hpp's equipDataReady comment). Every fall-off cascade ever
    // logged happened inside this exact window — sampling during it doesn't
    // find a bug, it finds the load still in progress and then burns the
    // repair-attempt cap fighting it. Not scanning at all here means the
    // baseline is simply seeded fresh, alarm-free, the first time we do
    // resume scanning post-load, instead of being sampled mid-load and
    // misread as a genuine clear.
    if (sdb::g_state().equipDataReady.load(std::memory_order_acquire)) {
        check_attach_health("local", localPawn, s_snapshots, s_itemHadMesh, s_itemLastPos);
        check_component_drift("local", localPawn, s_driftSnapshots);
    }

    int proxyCount = 0;
    {
        std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
        int i = 0;
        for (auto& [id, player] : sdb::g_state().players) {
            if (!player.proxyActor) continue;
            std::string label = "proxy" + std::to_string(i++);
            AActor* proxyActor = static_cast<AActor*>(player.proxyActor);
            if (reset_label_snapshots_if_actor_changed(label, proxyActor, s_lastActorForLabel, s_snapshots, s_driftSnapshots)) {
                s_itemHadMesh.clear();
                s_itemLastPos.clear();
            }
            check_attach_health(label, proxyActor, s_snapshots, s_itemHadMesh, s_itemLastPos);
            check_component_drift(label, proxyActor, s_driftSnapshots);
        }
        proxyCount = i;
    }

    // Heartbeat (2026-08-14) — every 30s, so a future "zero hits" result can
    // be told apart from "this code never actually ran." Reports local
    // Mesh's current AttachChildren count (from the just-updated snapshot)
    // and how many proxies were scanned this pass.
    static uint64_t s_lastHeartbeatUs = 0;
    if (now - s_lastHeartbeatUs >= 30'000'000ULL) {
        s_lastHeartbeatUs = now;
        auto it = s_snapshots.find("local");
        const size_t localChildren = (it != s_snapshots.end()) ? it->second.size() : 0;
        // 2026-08-14: added proxy0's own children count after noticing every
        // single proxy-level report all session (glasses/NVG/hair/AK/helmet/
        // suppressor — all seen on someone else's screen) produced zero
        // hits, while local-pawn cascades kept getting caught reliably. That
        // pattern is too consistent to be "proxies just don't have this
        // problem" — logging proxy0's actual scanned count to check whether
        // the proxy-side scan is finding a sane AttachChildren count at all,
        // or silently finding ~0 every time (which would mean Mesh
        // resolution or the whole scan is failing specifically for proxies).
        auto pit = s_snapshots.find("proxy0");
        const std::string proxy0Info = (pit != s_snapshots.end())
            ? (" proxy0Children=" + std::to_string(pit->second.size()))
            : " proxy0Children=<not found>";
        debug_log("attach_health: heartbeat localPawn=" + std::string(localPawn ? "found" : "NULL") +
                   " localMeshChildren=" + std::to_string(localChildren) +
                   " proxiesScanned=" + std::to_string(proxyCount) + proxy0Info);
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
        // Same 2s post-spawn grace period used everywhere else a proxy gets
        // touched — see on_process_event_post's own comment for why.
        // TEMPORARILY DISABLED 2026-08-13 for testing, see mod.cpp's earlier
        // grace-period comment for why it exists — re-enable if this causes
        // spawn-time crashes/deadlocks again.
        // if (sdb::now_micros() - player.proxySpawnedAtUs < 2'000'000ULL) continue;
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

// Read-only, one-shot diagnostic (2026-08-13): BP_PlayerCharacter_C::
// GetCurrentActiveWeapon() is a real, directly-callable UFUNCTION (confirmed
// via FModel export — FUNC_BlueprintCallable, returns an Actor* out-param
// named "EquippedWeapon") — the working theory for the one-handed weapon
// grip symptom is that whatever AnimGraph IK drives two-handed grip reads
// this getter (or something feeding the same underlying inventory state),
// and our proxy's weapon-visual actor (spawned separately via socket
// attach, never run through the game's own real equip flow) never gets
// returned by it, leaving the IK with nothing to target. Calls it on both
// the local pawn and any connected proxy, logging the returned actor
// address next to our own tracked weapon-visual actor pointers so they can
// be compared by eye.
static void check_active_weapon_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\active_weapon.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    auto logOne = [](const char* label, AActor* pawn) {
        if (!pawn) { debug_log(std::string("active_weapon: ") + label + " no pawn"); return; }
        UFunction* fn = pawn->GetFunctionByNameInChain(L"GetCurrentActiveWeapon");
        if (!fn) { debug_log(std::string("active_weapon: ") + label + " GetCurrentActiveWeapon NOT FOUND"); return; }
        struct Params { AActor* EquippedWeapon = nullptr; } params;
        pawn->ProcessEvent(fn, &params);
        char buf[128];
        snprintf(buf, sizeof(buf), "active_weapon: %s pawn=0x%llx -> EquippedWeapon=0x%llx",
                 label, (unsigned long long)(uintptr_t)pawn, (unsigned long long)(uintptr_t)params.EquippedWeapon);
        debug_log(buf);

        // WeaponType (2026-08-13): a real, plain, directly-writable
        // ByteProperty (Enum_Firearms) on BP_PlayerCharacter_C itself,
        // confirmed via FModel export — Pistol=0, SMG=1, AR/LMG/Marksman=2,
        // Sniper=3, Shotgun=4, ... Working theory for the one-handed-
        // shotgun symptom: this drives the animation system's one-handed-
        // vs-two-handed pose category (a pistol correctly renders one-
        // handed even for the LOCAL player — live-confirmed by the user —
        // so this isn't a generic "aiming" gate, it's weapon-type-specific).
        // If this reads correctly on local but wrong/default on the proxy,
        // that would explain the symptom directly.
        if (auto* wt = static_cast<uint8_t*>(pawn->GetValuePtrByPropertyNameInChain(L"WeaponType"))) {
            snprintf(buf, sizeof(buf), "active_weapon: %s WeaponType=%d", label, (int)*wt);
            debug_log(buf);
        } else {
            debug_log(std::string("active_weapon: ") + label + " WeaponType not found");
        }
    };

    logOne("local", find_local_pawn());

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        if (!player.proxyActor) continue;
        // TEMPORARILY DISABLED 2026-08-13 for testing, see mod.cpp's earlier
        // grace-period comment for why it exists — re-enable if this causes
        // spawn-time crashes/deadlocks again.
        // if (sdb::now_micros() - player.proxySpawnedAtUs < 2'000'000ULL) continue;
        logOne("proxy", static_cast<AActor*>(player.proxyActor));
        char buf[256];
        snprintf(buf, sizeof(buf),
            "active_weapon: proxy tracked visuals: primary=0x%llx secondary=0x%llx sidearm=0x%llx melee=0x%llx",
            (unsigned long long)(uintptr_t)player.primaryWeaponVisualActor,
            (unsigned long long)(uintptr_t)player.secondaryWeaponVisualActor,
            (unsigned long long)(uintptr_t)player.sidearmVisualActor,
            (unsigned long long)(uintptr_t)player.meleeVisualActor);
        debug_log(buf);
        break;
    }
}

// Read-only, one-shot diagnostic (2026-08-13): dumps the raw 864-byte
// AnimNode_Fabrik struct (AnimGraphNode_Fabrik_6/_7 — the two-handed weapon
// grip IK solver nodes, confirmed real via FModel export) off both the
// local player's and a proxy's live AnimInstance to .bin files, for a raw
// byte-level diff. Deliberately NOT reading any specific named field (e.g.
// "Alpha") — no verified offset exists for it in this build (no dedicated
// struct-layout export was found, and guessing risks exactly the crash
// class this project's own "never guess offsets" rule exists to prevent).
// This instead reuses the same safe methodology already proven for finding
// UFunction::Script's own offset earlier this session: dump raw bytes, diff
// by eye for a plausible pattern, rather than guess a specific field.
static void check_fabrik_dump_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\fabrik_dump.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    wchar_t outDir[MAX_PATH];
    DWORD dn = GetEnvironmentVariableW(L"APPDATA", outDir, MAX_PATH);
    if (dn == 0 || dn >= MAX_PATH) return;

    struct DumpCtx { const char* label; const wchar_t* wlabel; AActor* pawn; const wchar_t* outDir; DWORD dn; };

    auto dumpOneRaw = [](void* rawCtx) {
        auto* ctx = static_cast<DumpCtx*>(rawCtx);
        const char* label = ctx->label;
        AActor* pawn = ctx->pawn;
        if (!pawn) { debug_log(std::string("fabrik_dump: ") + label + " no pawn"); return; }
        auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
        UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
        if (!mesh) { debug_log(std::string("fabrik_dump: ") + label + " Mesh not found"); return; }
        UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
        if (!getAnimFn) { debug_log(std::string("fabrik_dump: ") + label + " GetAnimInstance NOT FOUND"); return; }
        struct Params { UObject* ReturnValue = nullptr; } aparams;
        mesh->ProcessEvent(getAnimFn, &aparams);
        if (!aparams.ReturnValue) { debug_log(std::string("fabrik_dump: ") + label + " AnimInstance is null"); return; }

        for (const wchar_t* nodeName : { L"AnimGraphNode_Fabrik_6", L"AnimGraphNode_Fabrik_7" }) {
            void* structPtr = aparams.ReturnValue->GetValuePtrByPropertyNameInChain(nodeName);
            if (!structPtr) {
                debug_log(std::string("fabrik_dump: ") + label + " node not found");
                continue;
            }
            std::wstring outPath = std::wstring(ctx->outDir, ctx->dn) + L"\\SurrounDeadBridge\\fabrik_" +
                ctx->wlabel + L"_" + nodeName + L".bin";
            std::ofstream out(outPath, std::ios::binary | std::ios::trunc);
            if (out.is_open()) out.write(reinterpret_cast<const char*>(structPtr), 864);
            debug_log(std::string("fabrik_dump: ") + label + " wrote node dump");
        }
    };

    // SEH-guarded (2026-08-13): this is exploratory struct-property access
    // on a name never previously read this session — if
    // GetValuePtrByPropertyNameInChain doesn't actually resolve
    // "AnimGraphNode_Fabrik_6/_7" the same clean way it does for simpler
    // properties, better to catch and log than silently crash the whole
    // do_game_tick call the way this did unguarded on the first attempt
    // (flag consumed, zero log output, no other symptom).
    DumpCtx localCtx{"local", L"local", find_local_pawn(), outDir, dn};
    if (!seh_invoke(dumpOneRaw, &localCtx))
        debug_log("fabrik_dump: local crashed, caught via SEH");

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (auto& [id, player] : sdb::g_state().players) {
        if (!player.proxyActor) continue;
        // TEMPORARILY DISABLED 2026-08-13 for testing, see mod.cpp's earlier
        // grace-period comment for why it exists — re-enable if this causes
        // spawn-time crashes/deadlocks again.
        // if (sdb::now_micros() - player.proxySpawnedAtUs < 2'000'000ULL) continue;
        DumpCtx proxyCtx{"proxy", L"proxy", static_cast<AActor*>(player.proxyActor), outDir, dn};
        if (!seh_invoke(dumpOneRaw, &proxyCtx))
            debug_log("fabrik_dump: proxy crashed, caught via SEH");
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
// Suppresses the game's own local zombie spawning so the client relies
// entirely on server-simulated Zombie entities instead (approved rewrite
// plan, Phase 3 — server/src/world/zombie-simulation.js is now the sole
// source of zombie spawns). SetIsSpawningStopped/KillSpawnedActors are real
// Blueprint-native UFunctions on ABP_AISpawner_Master_C
// (research/CXXHeaderDump/BP_AISpawner_Master.hpp) — no memory writes, no
// guessed offsets, called through the same GetFunctionByNameInChain+
// ProcessEvent pattern used everywhere else in this file.
//
// NOT YET LIVE-VERIFIED (2026-08-14) — run once, throttled-retried like
// every other one-shot resolution in this file, since spawner actors only
// exist once the level has finished loading.
static bool suppress_zombie_spawners()
{
    static const wchar_t* kSpawnerClasses[] = {
        L"BP_AISpawner_Zombies_C", L"BP_AISpawner_ZombieHounds_C", L"BP_AISpawner_ZombieBosses_C",
    };

    int totalFound = 0, totalStopped = 0;
    for (const wchar_t* className : kSpawnerClasses) {
        std::vector<UObject*> spawners;
        UObjectGlobals::FindAllOf(className, spawners);
        totalFound += static_cast<int>(spawners.size());

        for (UObject* obj : spawners) {
            UFunction* stopFn = obj->GetFunctionByNameInChain(L"SetIsSpawningStopped");
            if (stopFn) {
                struct { bool Stop = true; } params;
                obj->ProcessEvent(stopFn, &params);
                totalStopped++;
            }
            UFunction* killFn = obj->GetFunctionByNameInChain(L"KillSpawnedActors");
            if (killFn) {
                struct { bool AllowRespawn = false; } params;
                obj->ProcessEvent(killFn, &params);
            }
        }
    }

    debug_log("suppress_zombie_spawners: found " + std::to_string(totalFound) +
              " spawner instances, stopped " + std::to_string(totalStopped));
    return totalFound > 0; // only declare success once spawners were actually found — level may not be loaded yet
}

// Deferred vitals-restore write, split into a trampoline for SEH (2026-08-14,
// see state.hpp's pendingVitalsRestore comment). Same offsets
// read_local_progress() already reads successfully every profile-revision
// send (pawn+0x7D0/0x7F8/0x800/0x7F0 for Medical/HungerThirst/Stamina/
// Radiation components) — the offsets themselves are proven correct on a
// fully-initialized pawn; SEH here guards against the case that mattered
// tonight (writing before the pawn has finished initializing).
struct VitalsRestoreCtx {
    AActor* pawn;
    float health, hunger, thirst, stamina, radiation;
};

// 2026-08-14: re-enabled after a direct test disproved this write as the
// equipment-cascade's cause — with kEnableVitalsWrite=false (write fully
// skipped, only logging kept), the exact same cascade still fired about a
// second after the SKIPPED log line. Root cause is still open; this write
// is innocent, and the deferred+SEH-wrapped version below is still a real
// improvement over the original unguarded inline write regardless.
static constexpr bool kEnableVitalsWrite = true;

static void do_vitals_restore(void* ctxRaw)
{
    auto* ctx = static_cast<VitalsRestoreCtx*>(ctxRaw);
    const auto base = reinterpret_cast<uintptr_t>(ctx->pawn);
    if constexpr (kEnableVitalsWrite) {
        if (const auto med = *reinterpret_cast<uintptr_t*>(base + 0x7D0))
            *reinterpret_cast<double*>(med + 0xD0) = static_cast<double>(ctx->health);
        if (const auto ht = *reinterpret_cast<uintptr_t*>(base + 0x7F8)) {
            *reinterpret_cast<double*>(ht + 0xC8) = static_cast<double>(ctx->hunger);
            *reinterpret_cast<double*>(ht + 0xD8) = static_cast<double>(ctx->thirst);
        }
        if (const auto stam = *reinterpret_cast<uintptr_t*>(base + 0x800))
            *reinterpret_cast<double*>(stam + 0xC8) = static_cast<double>(ctx->stamina);
        if (const auto rad = *reinterpret_cast<uintptr_t*>(base + 0x7F0))
            *reinterpret_cast<double*>(rad + 0xC8) = static_cast<double>(ctx->radiation);
        debug_log("vitals_restore: applied deferred vitals write");
    } else {
        debug_log("vitals_restore: SKIPPED (kEnableVitalsWrite=false, testing cascade correlation)");
    }
}

// Equip-restore retry trampoline, surgical version (2026-08-14) — see
// state.hpp's pendingEquipRestoreRetry comment for the full root-cause
// chain. Live IDA reads (this session) proved the exact failure shape on a
// real, currently-broken slot: RepActorsData (BP_JigHelperComp+0xAE0, a
// TArray<FS_RepActorData>, each entry [FGameplayTag Slot, AActor* Actor] —
// research/CXXHeaderDump/S_RepActorData.hpp) held a real, valid Actor* for
// every slot including the broken one — the actor itself replicated fine.
// The failure is specifically that actor's RootComponent (AActor+0x1A0)
// ->AttachParent (USceneComponent+0xB0, both offsets already proven
// elsewhere in this file/proxy_manager.cpp) was null instead of matching
// every other (correctly-attached) slot's shared parent — confirmed live,
// side-by-side, on 11 real entries (10 attached, 1 the exact Flashlight
// slot the user had just reported broken).
//
// Rather than blindly re-running the whole native destroy+restore sequence
// (OnLoadDataRequested) — which would flicker every single slot on every
// join, not just broken ones — this walks RepActorsData directly, checks
// only the AttachParent-null condition per entry, and for any slot that's
// actually broken, calls the same native "Equip Actor to Socket" function
// this project already uses successfully for proxies
// (proxy_manager.cpp::equip_actor_to_socket — reused call shape here,
// that helper itself is file-local so duplicated rather than exposed) with
// the EXISTING actor reference already sitting right there in
// RepActorsData — no spawning, no DataAsset resolution needed, just
// re-attaching what the server already confirmed should exist.
struct EquipRestoreRetryCtx { AActor* pawn; std::string label; };

static void do_equip_restore_retry(void* ctxRaw)
{
    auto* ctx = static_cast<EquipRestoreRetryCtx*>(ctxRaw);
    const auto helper = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(ctx->pawn) + 0x700);
    if (!helper) {
        debug_log("equip_restore_retry: " + ctx->label + " pawn+0x700 BP_JigHelperComp is null");
        return;
    }
    UFunction* equipFn = reinterpret_cast<UObject*>(helper)->GetFunctionByNameInChain(L"Equip Actor to Socket");
    if (!equipFn) {
        debug_log("equip_restore_retry: " + ctx->label + " EquipActorToSocket NOT FOUND");
        return;
    }

    const uintptr_t repDataPtr  = *reinterpret_cast<uintptr_t*>(helper + 0xAE0);
    const int32_t   repCount    = *reinterpret_cast<int32_t*>(helper + 0xAE0 + 0x08);
    if (!repDataPtr || repCount <= 0 || repCount > 64) {
        debug_log("equip_restore_retry: " + ctx->label + " RepActorsData empty or implausible count=" + std::to_string(repCount));
        return;
    }
    // First plausible read since the last fresh/respawned pawn — unblock
    // component_drift/attach_health's "local" scan. See state.hpp's
    // equipDataReady comment for why this has to gate them, not just this
    // function. Local-only flag — this function now also runs per-proxy
    // (2026-08-15, extending equip repair/drift coverage to proxies, see
    // check_attach_health_trigger), and equipDataReady only ever gates the
    // "local" scan, so only set it when this call IS the local one.
    if (ctx->label == "local")
        sdb::g_state().equipDataReady.store(true, std::memory_order_release);

    // 2026-08-15: covers "AttachParent is null" (the original failure shape)
    // but NOT the distinct case live-reported same session: a helmet still
    // correctly attached (AttachParent set, mesh intact, present in
    // read_local_weapon_attachments the whole time) but visibly floating
    // away from its socket — a RelativeLocation drift, structurally
    // identical to component_drift's own DRIFTED check but that one only
    // ever covered the character's own named body components, never
    // equipped ITEM actors. Extended here since this function already walks
    // every equipped actor via RepActorsData. Same repair call
    // (EquipActorToSocket) as the orphaned-slot case — untested whether it
    // actually corrects position as well as attachment, but it's the only
    // native re-snap entry point this project has found, and worst case is
    // a no-op if it turns out to only fix AttachParent.
    static std::unordered_map<uintptr_t, std::array<double, 3>> s_lastActorPos;
    int checked = 0, fixed = 0, driftFixed = 0;
    for (int32_t i = 0; i < repCount; ++i) {
        const uintptr_t entry = repDataPtr + static_cast<size_t>(i) * 16;
        const uintptr_t actorPtr = *reinterpret_cast<uintptr_t*>(entry + 8);
        if (!actorPtr) continue;
        checked++;

        auto* actor = reinterpret_cast<AActor*>(actorPtr);
        const uintptr_t root = *reinterpret_cast<uintptr_t*>(actorPtr + 0x1A0);
        const uintptr_t attachParent = root ? *reinterpret_cast<uintptr_t*>(root + 0xB0) : 0;

        struct EquipParams { AActor* ActorRef = nullptr; bool IsSecondary = false; } params;
        params.ActorRef = actor;

        if (!attachParent) {
            // Broken slot — re-attach via the same native call this project
            // already uses successfully for proxies. IsSecondary only
            // matters for the two weapon-hand slots; false is correct for
            // everything else, and this is a best-effort repair, not a
            // guess that risks anything beyond "doesn't fix this one slot"
            // if wrong.
            reinterpret_cast<UObject*>(helper)->ProcessEvent(equipFn, &params);
            fixed++;
            s_lastActorPos.erase(actorPtr); // stale baseline, actor was just re-homed
            debug_log("equip_restore_retry: " + ctx->label + " re-attached orphaned slot entry[" + std::to_string(i) +
                      "] actor=0x" + std::to_string(actorPtr));
            continue;
        }

        // Attached and healthy per AttachParent — now check for positional
        // drift, same 30-unit threshold and RelativeLocation offset (0x128)
        // component_drift already uses on body components.
        if (root) {
            const auto* rel = reinterpret_cast<const double*>(root + 0x128);
            const double x = rel[0], y = rel[1], z = rel[2];
            auto it = s_lastActorPos.find(actorPtr);
            if (it != s_lastActorPos.end()) {
                const double dx = x - it->second[0], dy = y - it->second[1], dz = z - it->second[2];
                constexpr double kDriftDistSq = 30.0 * 30.0;
                if (dx * dx + dy * dy + dz * dz > kDriftDistSq) {
                    reinterpret_cast<UObject*>(helper)->ProcessEvent(equipFn, &params);
                    driftFixed++;
                    debug_log("equip_restore_retry: " + ctx->label + " re-snapped DRIFTED slot entry[" + std::to_string(i) +
                              "] actor=0x" + std::to_string(actorPtr) + " drifted from (" +
                              std::to_string(it->second[0]) + "," + std::to_string(it->second[1]) + "," + std::to_string(it->second[2]) +
                              ") to (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")");
                }
            }
            s_lastActorPos[actorPtr] = { x, y, z };
        }
    }
    // Only log the summary line when something was actually wrong or the
    // scan couldn't run as expected — this runs every 3s, so logging
    // "checked=N fixed=0" every single pass would just be noise once the
    // character is healthy (the common case).
    if (fixed > 0 || checked == 0)
        debug_log("equip_restore_retry: " + ctx->label + " checked=" + std::to_string(checked) + " fixed=" + std::to_string(fixed));

    // 2026-08-15, root-cause session continued: RepPrimitiveActorsData is a
    // SECOND replicated array on this same component (BP_JigHelperComp+0xAD0,
    // right before RepActorsData at +0xAE0 — real offsets from
    // research/CXXHeaderDump/BP_JigHelperComp.hpp, not a guess), a
    // TArray<FS_RepNonActorData>. Its element struct (S_RepNonActorData.hpp,
    // 0x18 bytes: FGameplayTag Slot@0x00, UJigsawItem_DataAsset_C* DA@0x08,
    // AActor* Primitive@0x10) carries a field RepActorsData's struct
    // (S_RepActorData.hpp, 0x10 bytes: Slot@0x00, Actor@0x08) simply doesn't
    // have — a SEPARATE live actor pointer per slot. Leading theory for the
    // body-mesh-clearing bug: this "Primitive" actor is what actually drives
    // the visible mesh, independent of RepActorsData's "Actor" (which is
    // what equip_restore_retry above reads and has shown to be consistently
    // fine — checked=N stays stable). This block is read-only diagnostics
    // only, no repair attempted yet — first confirm the theory live before
    // writing any fix. Logs only on a mismatch (Primitive null while the
    // matching-Slot RepActorsData entry has a live, attached Actor) since
    // that specific combination is exactly what would explain the observed
    // symptom (equip state fine, visible mesh wrong).
    const uintptr_t primDataPtr = *reinterpret_cast<uintptr_t*>(helper + 0xAD0);
    const int32_t   primCount   = *reinterpret_cast<int32_t*>(helper + 0xAD0 + 0x08);
    if (primDataPtr && primCount > 0 && primCount <= 64) {
        int mismatches = 0;
        for (int32_t i = 0; i < primCount; ++i) {
            const uintptr_t primEntry = primDataPtr + static_cast<size_t>(i) * 0x18;
            const uint64_t  primSlotTag = *reinterpret_cast<const uint64_t*>(primEntry);
            const uintptr_t primitiveActor = *reinterpret_cast<uintptr_t*>(primEntry + 0x10);
            if (primitiveActor) continue; // has a live primitive, nothing to report

            // Find the matching slot (by GameplayTag raw value) in
            // RepActorsData and check whether ITS actor is alive+attached.
            for (int32_t j = 0; j < repCount; ++j) {
                const uintptr_t repEntry = repDataPtr + static_cast<size_t>(j) * 16;
                const uint64_t  repSlotTag = *reinterpret_cast<const uint64_t*>(repEntry);
                if (repSlotTag != primSlotTag) continue;

                const uintptr_t repActor = *reinterpret_cast<uintptr_t*>(repEntry + 8);
                if (!repActor) break; // both empty, consistent, not a mismatch
                const uintptr_t repRoot = *reinterpret_cast<uintptr_t*>(repActor + 0x1A0);
                const uintptr_t repAttachParent = repRoot ? *reinterpret_cast<uintptr_t*>(repRoot + 0xB0) : 0;
                if (!repAttachParent) break; // RepActorsData itself is broken here too, not the interesting case

                mismatches++;
                debug_log("equip_restore_retry: " + ctx->label + " MISMATCH slot tag=0x" + std::to_string(primSlotTag) +
                          " RepPrimitiveActorsData.Primitive=NULL but RepActorsData.Actor=0x" +
                          std::to_string(repActor) + " (alive, attached) — Primitive theory candidate");
                break;
            }
        }
        if (mismatches == 0) {
            static uint64_t s_lastCleanLogUs = 0;
            const uint64_t nowUs = sdb::now_micros();
            if (nowUs - s_lastCleanLogUs >= 30'000'000ULL) { // heartbeat every 30s so we know it's still running
                s_lastCleanLogUs = nowUs;
                debug_log("equip_restore_retry: " + ctx->label + " RepPrimitiveActorsData checked, count=" +
                          std::to_string(primCount) + ", no mismatches this pass");
            }
        }
    }
}

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
    check_watch_lefthand_trigger();
    check_watch_activeslot_trigger();
    check_attach_health_trigger();
    check_active_weapon_trigger();
    check_fabrik_dump_trigger();
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
        // New/respawned pawn — RepActorsData hasn't necessarily replicated
        // back in yet. See state.hpp's equipDataReady comment.
        st.equipDataReady.store(false, std::memory_order_release);
    }

    // 3. Apply pending teleport from PlayerProgressRestore.
    // 2026-08-14: gated on actual distance now. This ran unconditionally on
    // every single join/reconnect regardless of whether the saved position
    // was anywhere near where the local game already put the player (its own
    // save data usually already has them close to the same spot) — live-
    // reported the same session: the LOCAL player's own equipped items
    // (respirator, "most of their meshes" on one machine) going missing,
    // not a proxy. K2_SetActorLocationAndRotation(..., bTeleport=true) is the
    // exact same call shape Session 57 already flagged as suspect for not
    // reliably preserving attached *actors'* (as opposed to components')
    // relative transforms — every equipped item is a separately attached
    // actor (see ProxyManager's own disconnect-cleanup comment: "attached
    // *actors* do not cascade with their owner, only attached *components*
    // do", the same underlying UE distinction). Tonight had an unusually
    // large number of reconnects (redeploy cycles), each firing this call
    // once — consistent with the correlation. Skipping the call entirely
    // when it wouldn't move the player meaningfully removes the highest-
    // frequency source of risk without touching the case that actually needs
    // it (a real cross-session rejoin somewhere else in the world).
    if (st.pendingTeleport.exchange(false, std::memory_order_acquire)) {
        const FVector  newLoc{ static_cast<double>(st.teleportX),
                                static_cast<double>(st.teleportY),
                                static_cast<double>(st.teleportZ) };
        const FVector  curLoc = pawn->K2_GetActorLocation();
        const double dx = newLoc.X - curLoc.X;
        const double dy = newLoc.Y - curLoc.Y;
        const double dz = newLoc.Z - curLoc.Z;
        constexpr double kTeleportDistSq = 500.0 * 500.0; // UE units — same threshold used for proxy teleport-vs-smooth elsewhere
        if (dx * dx + dy * dy + dz * dz > kTeleportDistSq) {
            const FRotator newRot{ 0.0, static_cast<double>(st.teleportYaw), 0.0 };
            // 2026-08-14, second attempt at this call, same session: gating
            // on distance (above) didn't stop it — PC2 hit this on a genuine
            // fresh join, which the distance gate correctly doesn't skip, and
            // the arms fell off again. Replaced the combined
            // K2_SetActorLocationAndRotation(bTeleport=true) with two
            // separate reflection calls (K2_SetActorLocation +
            // K2_SetActorRotation), matching what the real game's own
            // MC_ADS was found to use for rotation (Session 57) — the
            // combined call is the one specific shape never actually
            // verified safe for attached actors; splitting it is the
            // narrowest change that still accomplishes the same net
            // position+rotation update. No native binding exists for either
            // half individually (only the combined call is bound in the
            // vendored SDK), so both go through the same
            // GetFunctionByNameInChain/ProcessEvent reflection pattern
            // already proven throughout this file. HIGHER RISK than any
            // other change tonight — this runs on every local player's own
            // pawn, not a proxy. Not yet live-verified.
            UFunction* setLocFn = pawn->GetFunctionByNameInChain(L"K2_SetActorLocation");
            UFunction* setRotFn = pawn->GetFunctionByNameInChain(L"K2_SetActorRotation");
            if (setLocFn && setRotFn) {
                // 2026-08-14: tight bracketing logs + an immediate before/
                // after AttachChildren count, added specifically to confirm
                // (not just infer from a few log lines' distance) whether
                // the equipment-clear cascade is really adjacent to this
                // exact teleport call or merely near it in time. Read-only,
                // no behavior change — same AttachChildren offsets already
                // proven safe by read_local_weapon_attachments/attach_health.
                auto** preMeshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
                UObject* preMesh = (preMeshSlot && *preMeshSlot) ? *preMeshSlot : nullptr;
                int32_t preCount = -1;
                if (preMesh) preCount = *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(preMesh) + 0x00C0 + 0x08);
                debug_log("join_teleport: about to call K2_SetActorLocation/Rotation, preChildrenCount=" + std::to_string(preCount));

                struct SetLocParams { FVector NewLocation; bool bSweep; FHitResult SweepHitResult; bool bTeleport; bool ReturnValue; } locParams{};
                locParams.NewLocation = newLoc;
                locParams.bSweep      = false;
                locParams.bTeleport   = true;
                pawn->ProcessEvent(setLocFn, &locParams);

                // 2026-08-15: added the missing trailing ReturnValue field —
                // found via a systematic audit of every ProcessEvent call
                // site in this file against real native signatures
                // (Engine.hpp: bool K2_SetActorRotation(FRotator, bool)).
                // Its sibling call three lines above (SetLocParams) already
                // had this; this one didn't, an accidental omission that
                // left the Kismet-packed return slot missing — a 1-byte
                // stack overwrite adjacent to this params struct on every
                // single join-time teleport of the local player's own pawn.
                struct SetRotParams { FRotator NewRotation; bool bTeleportPhysics; bool ReturnValue; } rotParams{};
                rotParams.NewRotation     = newRot;
                rotParams.bTeleportPhysics = true;
                pawn->ProcessEvent(setRotFn, &rotParams);

                int32_t postCount = -1;
                if (preMesh) postCount = *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(preMesh) + 0x00C0 + 0x08);
                debug_log("join_teleport: K2_SetActorLocation/Rotation done, postChildrenCount=" + std::to_string(postCount));

                Output::send<LogLevel::Normal>(
                    STR("SDB: teleported (split call)  x={:.1f} y={:.1f} z={:.1f}\n"),
                    st.teleportX, st.teleportY, st.teleportZ);
            } else {
                // Fallback to the old combined call rather than silently not
                // teleporting at all if either function name ever changes.
                FHitResult hit{};
                pawn->K2_SetActorLocationAndRotation(newLoc, newRot, false, hit, true);
                Output::send<LogLevel::Normal>(
                    STR("SDB: teleported (fallback combined call, K2_SetActorLocation/Rotation not found)  x={:.1f} y={:.1f} z={:.1f}\n"),
                    st.teleportX, st.teleportY, st.teleportZ);
            }
        } else {
            Output::send<LogLevel::Normal>(
                STR("SDB: skipped teleport, already close to saved position\n"));
        }
    }

    // 3b. Apply pending vitals restore from PlayerProgressRestore (2026-08-14,
    // see state.hpp's pendingVitalsRestore comment). Deferred ~2s past the
    // join event and SEH-wrapped, replacing an inline raw-memory write that
    // ran the instant find_local_pawn() first succeeded — this session's own
    // diagnostics showed the pawn's components are still mid-initialization
    // at that exact moment (`preChildrenCount=4` vs. a normal ~14).
    if (st.pendingVitalsRestore.load(std::memory_order_acquire) &&
        now >= st.vitalsRestoreReadyAtUs) {
        st.pendingVitalsRestore.store(false, std::memory_order_release);
        VitalsRestoreCtx ctx{ pawn, st.vitalsHealth, st.vitalsHunger, st.vitalsThirst,
                              st.vitalsStamina, st.vitalsRadiation };
        if (!seh_invoke(do_vitals_restore, &ctx))
            debug_log("vitals_restore: crashed applying deferred vitals, caught via SEH");
    }

    // 3c. Equip-restore retry, periodic — see state.hpp's
    // lastEquipRestoreRetryUs comment and do_equip_restore_retry's own
    // comment for the full chain. Every 3s; cheap and a no-op when every
    // slot's already correctly attached, so running continuously (not just
    // once at join) makes this self-heal any future occurrence of the same
    // failure shape too, not only the join-time replication race.
    //
    // 2026-08-15: extended to run against every proxy too, not just the
    // local pawn. Live-reported same session: guns/glasses/a knife visibly
    // floating off characters in screenshots showing MULTIPLE players'
    // gear at once — but this project's own diagnostics had ZERO hits all
    // session (`DETACHED` never fired once, `re-snapped DRIFTED` never
    // fired once), because every repair mechanism built tonight
    // (this function included, until now) only ever ran against the LOCAL
    // pawn. `Equip Actor to Socket` is the same call this project already
    // proven-safe to use for spawning/attaching items onto PROXY actors
    // elsewhere (proxy_manager.cpp) — reading a proxy's own
    // BP_JigHelperComp (same +0x700 offset, same class) and calling the
    // same repair on it is not a new technique, just applying an existing
    // proven one to actors this loop never visited before.
    if (now - st.lastEquipRestoreRetryUs >= 3'000'000ULL) {
        st.lastEquipRestoreRetryUs = now;
        EquipRestoreRetryCtx ctx{ pawn, "local" };
        if (!seh_invoke(do_equip_restore_retry, &ctx))
            debug_log("equip_restore_retry: local crashed, caught via SEH");

        std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
        int proxyIdx = 0;
        for (auto& [id, player] : sdb::g_state().players) {
            if (!player.proxyActor) continue;
            std::string label = "proxy" + std::to_string(proxyIdx++);
            EquipRestoreRetryCtx proxyCtx{ static_cast<AActor*>(player.proxyActor), label };
            if (!seh_invoke(do_equip_restore_retry, &proxyCtx))
                debug_log("equip_restore_retry: " + label + " crashed, caught via SEH");
        }
    }

    // 4. Rate-limited movement send.
    if (now - g_last_move_us.load() >= static_cast<uint64_t>(cfg_move_interval_us)) {
        g_last_move_us.store(now);
        send_movement(pawn);
        check_local_montage_change(pawn);
    }

    // 5. Drive proxy actors.
    UWorld* world = pawn->GetWorld();
    sdb::g_proxy_manager().tick(world, pawn);

    // 6. Drive world entities.
    sdb::g_entity_manager().tick(world, pawn);

    // 6a. One-time: suppress local zombie spawning so this client relies on
    // server-simulated Zombie entities instead (see suppress_zombie_spawners's
    // own doc comment). Retried every 5s until spawners are actually found,
    // matching this file's usual "world might not be loaded yet" throttle
    // shape elsewhere (e.g. s_drop_fn resolution in on_process_event_pre).
    {
        static std::atomic<bool> s_spawnersSuppressed{false};
        static std::atomic<uint64_t> s_lastSuppressTryUs{0};
        if (!s_spawnersSuppressed.load(std::memory_order_relaxed)) {
            const uint64_t last = s_lastSuppressTryUs.load(std::memory_order_relaxed);
            if (last == 0 || now - last >= 5'000'000ULL) {
                s_lastSuppressTryUs.store(now, std::memory_order_relaxed);
                if (suppress_zombie_spawners())
                    s_spawnersSuppressed.store(true, std::memory_order_relaxed);
            }
        }
    }

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

// Building placement (2026-08-14, PlacedStructure server-authoritative
// rewrite — research/04_ida_investigation_log.md Session 58). Resolved off
// the local pawn's BuildingComponent (pawn+0x7E0,
// research/CXXHeaderDump/BP_PlayerCharacter.hpp), same throttled-retry
// pattern as s_drop_fn. Both SpawnBuild and Svr_SpawnBuild are resolved and
// hooked — research/CXXHeaderDump/BuildingComponent.hpp declares both with
// identical FTransform-only signatures and it isn't yet confirmed live which
// one (or both) actually fires for a real placement; handle_build_hook
// debounces by transform+time so a double-fire doesn't send two requests.
// NOT YET LIVE-VERIFIED — needs a real placement to confirm firing before
// this can be trusted, same caveat as every other new hook in this project.
static UFunction* s_buildFn    = nullptr;
static UFunction* s_svrBuildFn = nullptr;

// Session 55: BP_PlayerCharacter_C::PlayMontage(UAnimMontage* Montage,
// double PlayRate) is the single generic entry point the real game uses to
// play any one-shot action animation (melee swings confirmed via
// NormalMeleeAttackMontages/PowerMeleeAttackMontages on
// BP_WeaponsPickupComponent_C — research/CXXHeaderDump/
// BP_WeaponsPickupComponent.hpp — likely covers other actions too, not
// specifically chased down one by one). Hooking this one function covers
// every montage-driven action generically instead of needing a bespoke hook
// per action type, same "resolved once, compared by pointer" pattern as
// s_drop_fn/s_pickup_fn.
static UFunction* s_playMontage_fn = nullptr;

// Diagnostic (2026-08-13): PlayMontage resolves and the hook fires fine for
// SOME action (confirmed live), but a melee swing specifically never hits
// it — meaning melee goes through a different montage-playing path.
// BP_PlayerCharacter.hpp declares MC_Montage(UAnimMontage*, float, bool
// IncludeLocal?)/Svr_Montage(same signature) as separate functions from
// PlayMontage — log-only for now (no params touched, signatures/offsets not
// verified yet) purely to find out which one(s) actually fire during a real
// melee swing before wiring anything up to them.
static UFunction* s_mcMontage_fn  = nullptr;
static UFunction* s_svrMontage_fn = nullptr;

// Neither PlayMontage nor MC_Montage/Svr_Montage fire during a real melee
// swing (all three confirmed resolved+hooked live, none triggered) — the
// real trigger is almost certainly the standard engine
// UAnimInstance::Montage_Play(UAnimMontage* MontageToPlay, float
// InPlayRate = 1, EMontagePlayReturnType, float InTimeToStartMontageAt = 0,
// bool bStopAllMontages = true), called directly from the weapon
// component's own graph. Public, well-documented UE5 API — params layout
// below (Montage*@0x00, float PlayRate@0x08) only relies on the first two
// fields, deliberately not touching anything past that until live-verified.
// Resolved off the local player's own AnimInstance (not the character —
// this is a UAnimInstance member), same lazy-resolve pattern as
// s_lastUpdateFn in on_process_event_post.
static UFunction* s_montagePlayEngine_fn = nullptr;

static void handle_montage_play_engine_hook(void* params)
{
    if (!params) return;
    auto* montage = *reinterpret_cast<UObject**>(static_cast<uint8_t*>(params) + 0x00);
    const float playRate = *reinterpret_cast<const float*>(static_cast<uint8_t*>(params) + 0x08);
    if (!montage) return;

    const std::string montageName = short_object_name(montage);
    if (montageName.empty()) return;

    sdb::PlayMontageData m;
    m.montageName = montageName;
    m.playRate    = playRate;

    sdb::Frame f;
    f.type    = sdb::MsgType::PlayMontage;
    f.payload = sdb::encode_play_montage(m);
    build_session_frame(f);
    send_frame(f);

    char buf[128];
    snprintf(buf, sizeof(buf), "send_play_montage(engine): montage=%s playRate=%.2f",
             montageName.c_str(), playRate);
    debug_log(buf);
}

static void handle_play_montage_hook(void* params)
{
    if (!params) return;
    auto* montage  = *reinterpret_cast<UObject**>(static_cast<uint8_t*>(params) + 0x00);
    const double playRate = *reinterpret_cast<const double*>(static_cast<uint8_t*>(params) + 0x08);
    if (!montage) return;

    const std::string montageName = short_object_name(montage);
    if (montageName.empty()) return;

    sdb::PlayMontageData m;
    m.montageName = montageName;
    m.playRate    = static_cast<float>(playRate);

    sdb::Frame f;
    f.type    = sdb::MsgType::PlayMontage;
    f.payload = sdb::encode_play_montage(m);
    build_session_frame(f);
    send_frame(f);

    char buf[128];
    snprintf(buf, sizeof(buf), "send_play_montage: montage=%s playRate=%.2f",
             montageName.c_str(), playRate);
    debug_log(buf);
}

// Melee swing/push/reload/equip montages (2026-08-13): the six candidate
// trigger points hooked above (PlayMontage, MC_Montage, Svr_Montage,
// AnimInstance::Montage_Play) never fire for a melee swing — live bytecode
// tracing (research/04_ida_investigation_log.md Session 57) found the real
// caller is UBP_WeaponsPickupComponent_C's own ubergraph, selecting from its
// NormalMeleeAttackMontages/PowerMeleeAttackMontages arrays and invoking the
// async PlayMontageCallbackProxy::CreateProxyObjectForPlayMontage node,
// which plays the montage via a raw C++ call inside its own Activate() —
// invisible to every ProcessEvent hook above, no matter which UFUNCTION is
// targeted. Rather than chase that (and every other current/future
// montage-driven action — push, reload, hit-react) through per-action
// bytecode archaeology, poll ACharacter::GetCurrentMontage() (confirmed via
// resolve_ptr as a plain zero-arg UFUNCTION, /Script/Engine.Character)
// once per movement-tick and broadcast whenever it changes to a new,
// non-null montage. This is a superset of every hook above — it fires for
// any montage played through any mechanism, sync or async, present or
// future — at the cost of not knowing the true PlayRate (defaulted to 1.0;
// GetCurrentMontage() doesn't expose it, and none of the montages synced
// this way are precision-timed enough for that to matter visually).
static UObject* g_last_local_montage = nullptr;

struct LocalMontageCtx {
    AActor*    pawn;
    UFunction* fn;
    UObject*   montage = nullptr;
    std::string name;
};

// Split out so the risky part (ProcessEvent into engine code, then
// GetFullName() on whatever it returns) can run under seh_invoke below —
// 2026-08-13: this call crashed the whole process live
// (EXCEPTION_ACCESS_VIOLATION writing 0x4ec, i.e. some object at a null-ish
// base pointer) the first time it observed a real melee-weapon equip
// in a 2-client session, despite GetCurrentMontage() being a plain,
// confirmed-resolvable zero-arg UFUNCTION and this exact
// GetFunctionByNameInChain/ProcessEvent/{ReturnValue} pattern being used
// safely elsewhere in this file (e.g. read_local_movement_flags's
// GetAnimInstance call) — never fully root-caused, SEH-guarding it is the
// same mitigation already applied to every other "call into engine code
// off a pointer we don't fully control the lifetime of" site in this file
// (do_resolve_ptr, do_weapon_attach_scan, etc.).
static void do_check_local_montage(void* ctxRaw)
{
    auto* ctx = static_cast<LocalMontageCtx*>(ctxRaw);
    struct Params { UObject* ReturnValue = nullptr; } p;
    ctx->pawn->ProcessEvent(ctx->fn, &p);
    ctx->montage = p.ReturnValue;
    if (ctx->montage) ctx->name = short_object_name(ctx->montage);
}

static void check_local_montage_change(AActor* pawn)
{
    if (!pawn) return;
    UFunction* getCurMontageFn = pawn->GetFunctionByNameInChain(L"GetCurrentMontage");
    if (!getCurMontageFn) return;

    LocalMontageCtx ctx{ pawn, getCurMontageFn };
    if (!seh_invoke(&do_check_local_montage, &ctx)) {
        debug_log("check_local_montage_change: access violation caught, skipping this tick");
        return;
    }

    if (ctx.montage == g_last_local_montage) return;
    g_last_local_montage = ctx.montage;
    if (!ctx.montage || ctx.name.empty()) return;

    sdb::PlayMontageData m;
    m.montageName = ctx.name;
    m.playRate    = 1.0f;

    sdb::Frame f;
    f.type    = sdb::MsgType::PlayMontage;
    f.payload = sdb::encode_play_montage(m);
    build_session_frame(f);
    send_frame(f);

    char buf[160];
    snprintf(buf, sizeof(buf), "send_play_montage(poll): montage=%s", ctx.name.c_str());
    debug_log(buf);
}

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

// SpawnBuild(FTransform SpawnTransform) / Svr_SpawnBuild(same signature) —
// params is the raw stack layout for a single by-value FTransform argument,
// same 96-byte quat+vec+vec shape proxy_manager.cpp's NativeFTransform
// already reads/writes elsewhere in this project (see e.g. its use reading
// an equipped-item transform at proxy_manager.cpp:900). Local copy here
// rather than sharing a header, matching this codebase's existing pattern of
// small per-file POD re-declarations.
struct BuildTransformParams {
    double rotX = 0.0, rotY = 0.0, rotZ = 0.0, rotW = 1.0;
    double locX = 0.0, locY = 0.0, locZ = 0.0, locPad = 0.0;
    double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0, scalePad = 0.0;
};

static void handle_build_hook(void* params)
{
    if (!params) return;
    const auto* xf = static_cast<const BuildTransformParams*>(params);

    // Debounce: SpawnBuild and Svr_SpawnBuild may both be hooked and either
    // (or both) could fire for one real placement — collapse duplicates by
    // (position, time) the same way handle_drop_hook debounces by ItemRef.
    static double   s_last_x = 0, s_last_y = 0, s_last_z = 0;
    static uint64_t s_last_build_us = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (xf->locX == s_last_x && xf->locY == s_last_y && xf->locZ == s_last_z &&
        nowUs - s_last_build_us < 500'000ULL) {
        debug_log("handle_build_hook: debounced duplicate call for same position");
        return;
    }
    s_last_x = xf->locX; s_last_y = xf->locY; s_last_z = xf->locZ;
    s_last_build_us = nowUs;

    AActor* pawn = find_local_pawn();
    if (!pawn) return;

    // BuildingComponent (pawn+0x7E0) -> DARef (+0x298, UJigsawItem_DataAsset_C*)
    // -> ItemId (+0x30, FName) — the piece currently selected in build mode,
    // same DataAsset/itemId system GroundItem already uses (research/
    // 04_ida_investigation_log.md Session 58).
    const uintptr_t buildingComp = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(pawn) + 0x7E0);
    if (!buildingComp) {
        debug_log("handle_build_hook: pawn+0x7E0 BuildingComponent is null");
        return;
    }
    const uintptr_t daRef = *reinterpret_cast<uintptr_t*>(buildingComp + 0x298);
    if (!daRef) {
        debug_log("handle_build_hook: BuildingComponent->DARef is null");
        return;
    }
    std::string itemId = native::fname_to_string(daRef + 0x30);
    if (itemId.empty()) {
        debug_log("handle_build_hook: DARef->ItemId is empty");
        return;
    }

    // Yaw from the quaternion (Z-up, standard UE convention) — building
    // pieces are placed with yaw-only rotation in this game, same assumption
    // proxy_manager.cpp's yaw<->quaternion conversions already make elsewhere.
    const double yawRad = std::atan2(
        2.0 * (xf->rotW * xf->rotZ + xf->rotX * xf->rotY),
        1.0 - 2.0 * (xf->rotY * xf->rotY + xf->rotZ * xf->rotZ));
    const float yawDeg = static_cast<float>(yawRad * 180.0 / 3.14159265358979323846);

    send_build_request(itemId, static_cast<float>(xf->locX), static_cast<float>(xf->locY),
                        static_cast<float>(xf->locZ), yawDeg);
    debug_log("handle_build_hook: sent InteractionRequest/BUILD itemId=" + itemId +
              " x=" + std::to_string(xf->locX) + " y=" + std::to_string(xf->locY));
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

// Recent-calls ring buffer (2026-08-14) — after two named-function-guess
// hooks (OnLoadDataRequested, SetSexMesh) both came back with zero hits
// during real, live occurrences of the Torso/Legs/Feet cascade, guessing a
// third name blind isn't a good use of another live cycle. Records every
// single ProcessEvent call's (func, obj, timestamp) unconditionally — cheap,
// just an array write and index increment, no string work on the hot path
// — so that whenever component_drift catches a mesh-asset-clear transition,
// it can dump exactly what actually ran in the moments before, instead of
// guessing candidate names up front. Resolving names (GetFullName, real
// work) only happens at dump time, not per-call.
// Sized generously (65536, not the originally-planned 64) — ProcessEvent
// fires very frequently (per an existing comment elsewhere in this file,
// "thousands of times per frame"), and the gap between an actual clear
// event and component_drift's own polling interval noticing it could
// otherwise blow straight through a small buffer, leaving nothing useful
// to dump by the time it's needed. Paired with tightening that polling
// interval (see check_attach_health_trigger) so the two stay in the same
// ballpark instead of relying on buffer size alone.
struct RecentCallEntry { UFunction* func; UObject* obj; uint64_t timeUs; };
static RecentCallEntry s_recentCalls[65536] = {};
static int s_recentCallsIdx = 0;

// 2026-08-14: the buffer above turned out to be dominated by unrelated
// per-tick noise — live-confirmed a SINGLE game tick alone produced 1000+
// distinct AIOptimizer::AIOSubjectComponent calls (one pair per zombie/AI
// spawn-zone subject), enough to exhaust do_dump_recent_calls' whole
// kMaxGroups cap before the dump ever reached back far enough to cover the
// actual ~1s gap since the last repair. Global capture at this call volume
// is not viable at any reasonable buffer size. Fixed by filtering what
// gets recorded at all, down to a small watch-list of object pointers that
// actually matter for this investigation (the local pawn plus its body-part
// components) — refreshed each "local" component_drift scan via
// set_recent_calls_watch below. Everything else is skipped before it ever
// touches the ring buffer, so the buffer now covers a real multi-second
// window of exactly the activity worth seeing.
static UObject* s_watchObjs[8] = {};
static int s_watchCount = 0;

static void set_recent_calls_watch(UObject* pawn, UObject* torso, UObject* legs, UObject* feet, UObject* helper)
{
    int n = 0;
    if (pawn)   s_watchObjs[n++] = pawn;
    if (torso)  s_watchObjs[n++] = torso;
    if (legs)   s_watchObjs[n++] = legs;
    if (feet)   s_watchObjs[n++] = feet;
    if (helper) s_watchObjs[n++] = helper;
    s_watchCount = n;
}

static void record_recent_call(UObject* obj, UFunction* func)
{
    bool watched = false;
    for (int i = 0; i < s_watchCount; ++i) {
        if (s_watchObjs[i] == obj) { watched = true; break; }
    }
    if (!watched) return;
    s_recentCalls[s_recentCallsIdx] = { func, obj, sdb::now_micros() };
    s_recentCallsIdx = (s_recentCallsIdx + 1) % 65536;
}

// Called from component_drift on a detected clear — SEH-wrapped since
// GetFullName() on a func pointer that happens to have gone stale between
// being recorded and being dumped (e.g. a since-destroyed transient object)
// is a real, if unlikely, risk worth guarding against, same discipline as
// every other speculative pointer read in this file.
struct DumpRecentCallsCtx { int startIdx; };
static void do_dump_recent_calls(void* ctxRaw)
{
    auto* ctx = static_cast<DumpRecentCallsCtx*>(ctxRaw);
    // 2026-08-14, rewritten after the original version (one debug_log call
    // per line, all 65536 entries) was caught live stalling the game thread
    // for ~7 real seconds per dump — see do_body_part_repair's comment.
    // Fixed two ways: build the ENTIRE dump into one in-memory string and
    // flush it with a single debug_log call instead of one file write per
    // line (the per-call I/O was almost certainly the real cost, not the
    // string formatting), and cap it to a bounded number of collapsed
    // groups (not raw entries — collapsing already does the heavy lifting
    // against per-frame anim/tick noise) so pathological cases can't still
    // produce a huge single write.
    std::string out;
    out.reserve(16384);
    out += "recent_calls: dumping last 65536 ProcessEvent calls, consecutive repeats collapsed (oldest first)\n";
    UFunction* lastFunc = nullptr;
    int repeatCount = 0;
    int groups = 0;
    constexpr int kMaxGroups = 1000;
    for (int i = 0; i < 65536 && groups < kMaxGroups; ++i) {
        const auto& entry = s_recentCalls[(ctx->startIdx + i) % 65536];
        if (!entry.func) continue;
        // Collapse runs of the identical function repeating (very common —
        // per-frame anim/tick calls otherwise drown out the genuinely
        // interesting one-off calls this dump exists to surface).
        if (entry.func == lastFunc) { repeatCount++; continue; }
        if (repeatCount > 0)
            out += "recent_calls:   (previous line repeated " + std::to_string(repeatCount) + " more times)\n";
        lastFunc = entry.func;
        repeatCount = 0;
        ++groups;

        std::wstring wname = entry.func->GetFullName();
        int need = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string name;
        if (need > 0) {
            name.resize(static_cast<size_t>(need - 1));
            WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), need, nullptr, nullptr);
        }
        out += "recent_calls:   t=" + std::to_string(entry.timeUs) + " obj=0x" +
               std::to_string(reinterpret_cast<uintptr_t>(entry.obj)) + " func=" + name + "\n";
    }
    if (repeatCount > 0) {
        out += "recent_calls:   (previous line repeated " + std::to_string(repeatCount) + " more times)\n";
    }
    if (groups >= kMaxGroups) out += "recent_calls:   (truncated at " + std::to_string(kMaxGroups) + " groups)\n";
    debug_log(out);
}

static void dump_recent_calls()
{
    static std::atomic<uint64_t> s_lastDumpUs{0};
    const uint64_t now = sdb::now_micros();
    const uint64_t last = s_lastDumpUs.load(std::memory_order_relaxed);
    if (last != 0 && now - last < 500'000ULL) return; // de-dup: Torso/Legs/Feet often clear in the same tick
    s_lastDumpUs.store(now, std::memory_order_relaxed);

    DumpRecentCallsCtx ctx{ s_recentCallsIdx };
    if (!seh_invoke(do_dump_recent_calls, &ctx))
        debug_log("recent_calls: dump crashed, caught via SEH");
}

// Fires on the game thread for every UObject::ProcessEvent call.
// Live diagnostic (2026-08-14) — direct empirical answer to "does
// RepActorsData populate before or after OnLoadDataRequested runs," instead
// of more static bytecode archaeology with a stale-pointer risk (this
// session's own .bin dumps only stay valid within the same process instance
// they were captured from — a relaunch invalidates every func/property
// pointer in them, confirmed live when a resolve came back garbage after a
// relaunch happened in between). UFunction* is shared per-class (see
// [[feedback_sdo_ufunction_shared_per_class]]), so resolving this once off
// any live BP_JigHelperComp_C instance covers every instance's calls,
// local and proxy alike. Cheap pointer-equality fast path, same pattern as
// s_lastUpdateFn in on_process_event_post.
static UFunction* s_loadDataRequestedFn = nullptr;

static void check_load_data_requested_hook(UObject* obj, UFunction* func)
{
    if (!s_loadDataRequestedFn) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                const auto helper = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(pawn) + 0x700);
                if (helper)
                    s_loadDataRequestedFn = reinterpret_cast<UObject*>(helper)->GetFunctionByNameInChain(L"OnLoadDataRequested");
            }
        }
    }
    if (func != s_loadDataRequestedFn) return;

    const auto objAddr = reinterpret_cast<uintptr_t>(obj);
    const uintptr_t repDataPtr = *reinterpret_cast<uintptr_t*>(objAddr + 0xAE0);
    const int32_t   repCount   = *reinterpret_cast<int32_t*>(objAddr + 0xAE0 + 0x08);
    debug_log("load_data_requested: PRE-call obj=0x" + std::to_string(objAddr) +
              " RepActorsData count=" + std::to_string(repCount) +
              " (data_ptr=" + (repDataPtr ? "set" : "NULL") + ")");
}

// 2026-08-14, second hook: OnLoadDataRequested was proven NOT the cause of
// the Torso/Legs/Feet body-part cascade — the live hook above caught zero
// calls to it during an actual, real occurrence of that exact cascade (log-
// confirmed: component_drift's repair loop fired and gave up with no
// load_data_requested line anywhere nearby). That whole chain only explains
// the separate equipped-item (RepActorsData/AttachParent) symptom. Watching
// SetSexMesh next — a real BP_PlayerCharacter_C function, name alone
// strongly suggests it (re)sets the base body mesh set by gender, which is
// exactly what Torso/Legs/Feet are. Same cached-UFunction-pointer pattern,
// resolved directly off the local pawn (not via a component this time).
static UFunction* s_setSexMeshFn = nullptr;

static void check_set_sex_mesh_hook(UObject* obj, UFunction* func)
{
    if (!s_setSexMeshFn) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn())
                s_setSexMeshFn = pawn->GetFunctionByNameInChain(L"SetSexMesh");
        }
    }
    if (func != s_setSexMeshFn) return;
    debug_log("set_sex_mesh: PRE-call obj=0x" + std::to_string(reinterpret_cast<uintptr_t>(obj)));
}

// 2026-08-14, root-cause session, next step: CXXHeaderDump/BP_PlayerCharacter.hpp
// shows a real Server/Multicast RPC pair — Svr_AttachClothing / MC_AttachClothing
// (class USkinnedMeshComponent* Clothing, class USkinnedAsset* Mesh,
// FBodyPartSettings Parts, bool IsPlayerMale, FName BodyPart, bool
// UpdateAllBodyParts) — the real, server-authoritative mechanism for
// applying body/clothing meshes, distinct from UpdateBodyParts (which this
// project has been calling as a repair — it only sets a hardcoded default,
// per decoded_UpdateBodyParts.txt's EX_ObjectConst literals, not the
// player's actual equipped appearance). Neither RPC showed up anywhere in
// the filtered recent-calls window around a live re-clear, which either
// means they fire far less often than once/second (so the window missed
// them) or they never fire again after an initially-broken join, leaving
// the character server-side "unequipped" for real — this hook answers
// which, by logging every time either actually runs.
static UFunction* s_svrAttachClothingFn = nullptr;
static UFunction* s_mcAttachClothingFn = nullptr;
static UFunction* s_equipClothingToMeshFn = nullptr;

static void check_attach_clothing_hooks(UObject* obj, UFunction* func)
{
    if (!s_svrAttachClothingFn || !s_mcAttachClothingFn || !s_equipClothingToMeshFn) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                if (!s_svrAttachClothingFn) s_svrAttachClothingFn = pawn->GetFunctionByNameInChain(L"Svr_AttachClothing");
                if (!s_mcAttachClothingFn) s_mcAttachClothingFn = pawn->GetFunctionByNameInChain(L"MC_AttachClothing");
                if (!s_equipClothingToMeshFn) s_equipClothingToMeshFn = pawn->GetFunctionByNameInChain(L"EquipClothingToMesh");
            }
        }
    }
    if (func == s_svrAttachClothingFn)
        debug_log("attach_clothing: Svr_AttachClothing PRE-call obj=0x" + std::to_string(reinterpret_cast<uintptr_t>(obj)));
    else if (func == s_mcAttachClothingFn)
        debug_log("attach_clothing: MC_AttachClothing PRE-call obj=0x" + std::to_string(reinterpret_cast<uintptr_t>(obj)));
    else if (func == s_equipClothingToMeshFn)
        debug_log("attach_clothing: EquipClothingToMesh PRE-call obj=0x" + std::to_string(reinterpret_cast<uintptr_t>(obj)));
}

static void on_process_event_pre(UObject* obj, UFunction* func, void* params)
{
    record_recent_call(obj, func);
    check_load_data_requested_hook(obj, func);
    check_set_sex_mesh_hook(obj, func);
    check_attach_clothing_hooks(obj, func);

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

    // Same throttled-retry shape as s_drop_fn above, but resolved off
    // pawn+0x7E0's BuildingComponent instead of pawn+0x818's BP_JigMultiplayer.
    // Both SpawnBuild and Svr_SpawnBuild are resolved — see s_buildFn's own
    // declaration comment for why (not yet confirmed live which one fires).
    static std::atomic<uint64_t> s_last_build_fn_try_us{0};
    if (func && (!s_buildFn || !s_svrBuildFn)) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_build_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_build_fn_try_us.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                const uintptr_t buildingComp = *reinterpret_cast<uintptr_t*>(
                    reinterpret_cast<uintptr_t>(pawn) + 0x7E0);
                if (buildingComp) {
                    auto* comp = reinterpret_cast<UObject*>(buildingComp);
                    if (!s_buildFn)    s_buildFn    = comp->GetFunctionByNameInChain(L"SpawnBuild");
                    if (!s_svrBuildFn) s_svrBuildFn = comp->GetFunctionByNameInChain(L"Svr_SpawnBuild");
                    debug_log(std::string("on_process_event_pre: SpawnBuild=") + (s_buildFn ? "resolved" : "NOT FOUND") +
                              " Svr_SpawnBuild=" + (s_svrBuildFn ? "resolved" : "NOT FOUND"));
                } else {
                    debug_log("on_process_event_pre: pawn+0x7E0 BuildingComponent is null");
                }
            } else {
                debug_log("on_process_event_pre: find_local_pawn() returned null (build-fn resolve)");
            }
        }
    }
    if (func && (func == s_buildFn || func == s_svrBuildFn)) {
        handle_build_hook(params);
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

    // handle_play_montage_hook (below) is now DEAD/disabled — 2026-08-13.
    // UFunction* is per-CLASS, not per-instance: s_playMontage_fn resolved
    // off find_local_pawn() is the SAME pointer PROXY actors call PlayMontage
    // through too (BP_PlayerCharacter_C, same class). Once on_play_montage
    // (proxy_manager.cpp) started actually calling PlayMontage on proxies to
    // apply a received montage, this hook fired for THAT call too, treated
    // it as a fresh local action, and re-broadcast it — client A's own
    // montage bounces to client B's proxy-of-A, which re-broadcasts back to
    // A applied to B's proxy, forever, amplifying with whatever both sides
    // happened to have played recently. Looked like severe jitter/desync
    // live (send_play_montage: spamming the same ~9-montage sequence every
    // ~30ms). check_local_montage_change() (the GetCurrentMontage() poll,
    // further down this file) already supersedes this hook entirely and is
    // correctly scoped — it's called directly on a known local-pawn
    // pointer, never resolved as a bare class-wide UFunction* compared
    // against every ProcessEvent call in the process.

    // Diagnostic-only resolution for MC_Montage/Svr_Montage — see their
    // declaration comment above. Same throttled-retry shape.
    static std::atomic<uint64_t> s_last_mcsvr_fn_try_us{0};
    if (func && (!s_mcMontage_fn || !s_svrMontage_fn)) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_mcsvr_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_mcsvr_fn_try_us.store(now, std::memory_order_relaxed);
            if (AActor* pawn = find_local_pawn()) {
                if (!s_mcMontage_fn)  s_mcMontage_fn  = pawn->GetFunctionByNameInChain(L"MC_Montage");
                if (!s_svrMontage_fn) s_svrMontage_fn = pawn->GetFunctionByNameInChain(L"Svr_Montage");
                debug_log(std::string("on_process_event_pre: MC_Montage=") + (s_mcMontage_fn ? "resolved" : "NOT FOUND") +
                          " Svr_Montage=" + (s_svrMontage_fn ? "resolved" : "NOT FOUND"));
            }
        }
    }
    if (func && func == s_mcMontage_fn)  debug_log("montage_diag: MC_Montage fired");
    if (func && func == s_svrMontage_fn) debug_log("montage_diag: Svr_Montage fired");

    // handle_montage_play_engine_hook is also DEAD/disabled — 2026-08-13,
    // same reason as the PlayMontage hook above: UAnimInstance::Montage_Play
    // is one shared UFunction* per class, and proxies' AnimInstances are
    // that same class, so this fired for proxy playback too and fed the
    // same echo loop. Superseded by check_local_montage_change() below.

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

// Look-direction/crouch/ADS/falling sync, take 2 (2026-08-13): GetAimOffset
// unconditionally hard-resets the AnimBP's own Pitch to 0 every single frame
// for a non-locally-controlled proxy (confirmed live via bytecode tracing +
// direct value sampling — see proxy_manager.cpp's old apply_proxy_aim_pitch_
// safe comment, since removed, for the full chain of evidence). A same-tick
// property write can never win that race, and live testing found
// IsCrouching/IsADS jitter the same way (winning some ticks, losing others)
// once actually tried — the same class of problem, not unique to Pitch.
//
// First attempt hooked GetLeftHandLoc specifically (the last function in
// BlueprintThreadSafeUpdateAnimation's known per-frame call sequence:
// GetThreadSafeBooleans -> GetSpeed&Direction -> GetHeadRot -> GetAimOffset
// -> GetLean -> GetLeftHandLoc, resolved via resolve_fname against hand-
// decoded bytecode) — its post-callback NEVER fired, confirmed live via a
// diagnostic trace (zero hits despite the function's own effects clearly
// happening every frame). Root cause: BlueprintThreadSafeUpdateAnimation
// calls each of those via EX_LocalVirtualFunction, a Kismet compiler
// optimization for "call a function on self" that invokes directly within
// the *already-executing* ProcessEvent call rather than triggering its own
// separate ProcessEvent dispatch — so no per-sub-function hook, pre or
// post, can ever see them individually. Only the OUTER function
// (BlueprintThreadSafeUpdateAnimation itself, which the engine's own native
// anim-update system calls via a real ProcessEvent) is hookable this way.
// Fixed by hooking that instead — reapplying ALL of our proxy overrides
// (Pitch, IsCrouching, IsADS, Falling) right after it runs guarantees we're
// the last writer for the entire per-frame update block, regardless of
// which specific internal sub-function actually owns which property.
//
// UE4SS.dll in this build DOES export RegisterProcessEventPostCallback
// (verified live via GetProcAddress against the actual on-disk DLL, same
// mangled-name pattern as the existing Pre registration below with
// Pre->Post substituted) — an earlier comment elsewhere in this file
// claiming "no post-callback resolved for this UE4SS build" was simply
// never actually tested for this specific symbol.
static UFunction* s_lastUpdateFn = nullptr;

// Split out from on_process_event_post for SEH-wrapping (2026-08-14, see
// that function's own comment on why) — __try/__except can't share a stack
// frame with C++ objects needing unwinding (MSVC C2712), hence the
// trampoline split, same pattern as this file's other seh_invoke uses.
struct AimWriteCtx { UObject* obj; sdb::RemotePlayer* player; };

static void do_aim_write(void* ctxRaw)
{
    auto* ctx = static_cast<AimWriteCtx*>(ctxRaw);
    UObject* obj = ctx->obj;
    sdb::RemotePlayer& player = *ctx->player;

    // Smoothed values (ProxyManager::update_proxy_render_smoothing, run
    // from the game tick), not the raw packet-driven aimPitchByte/aimYaw
    // directly — 2026-08-13, the raw values only change once per network
    // packet (~50ms) and writing them straight in here every frame
    // produced a visible step/stutter, most noticeable while ADS. Yaw
    // (left/right look) is new this session — previously not sent at
    // all (mv.aimYaw was a copy-paste bug duplicating body yaw, not real
    // camera yaw), so it silently never worked; see Player_AnimBP.hpp's
    // Yaw property (0x5AF8, right after Pitch at 0x5AF0 — same struct,
    // same GetAimOffset per-frame reset-to-zero this whole mechanism
    // already exists to win against for Pitch).
    auto* pitchSlot = static_cast<double*>(obj->GetValuePtrByPropertyNameInChain(L"Pitch"));
    auto* yawSlot   = static_cast<double*>(obj->GetValuePtrByPropertyNameInChain(L"Yaw"));

    // Temporary diagnostic (2026-08-13): densified to ~20/sec (was
    // 1/sec) at the user's request to actually see the "quickly rotate
    // -> jumps then snaps back to 0" behavior, which a 1-second sample
    // rate was clearly too coarse to catch. Includes renderYaw (the
    // proxy's synced body yaw, and the turn-in-place reference) so it's
    // possible to tell whether a "reset to 0" is the aim-yaw itself
    // collapsing or renderYaw chasing it down (turn-in-place working
    // exactly as designed, just faster/more aggressively than expected).
    static uint64_t s_lastAimDiagUs = 0;
    const uint64_t nowDiagUs = sdb::now_micros();
    if (nowDiagUs - s_lastAimDiagUs > 50'000ULL) {
        s_lastAimDiagUs = nowDiagUs;
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "aim_write: beforePitch=%.2f targetPitch=%.2f  beforeYaw=%.2f targetYaw=%.2f  renderYaw=%.2f rawAimYaw=%.2f",
                 pitchSlot ? *pitchSlot : -999.0, player.renderAimPitch,
                 yawSlot   ? *yawSlot   : -999.0, player.renderAimYaw,
                 player.renderYaw, player.aimYaw);
        debug_log(buf);
    }

    // Negated (2026-08-13): live-confirmed the whole upper-body/arm pose
    // mirrored — turning the camera left visibly swung the pose right.
    // This game's AnimBP Yaw blendspace convention is inverted from the
    // standard UE ControlYaw-minus-BodyYaw sign (i.e. its Left/Right
    // samples are the opposite way round from every other stock
    // third-person setup). Flipped here at the write, not earlier in
    // the pipeline — negating the sender's raw absolute control yaw
    // instead would NOT be equivalent, since renderYaw (the body-yaw
    // term in the subtraction) isn't negated too: -(a) - b != -(a - b).
    if (pitchSlot) *pitchSlot = static_cast<double>(player.renderAimPitch);
    if (yawSlot)   *yawSlot   = static_cast<double>(-player.renderAimYaw);

    if (auto* crouching = static_cast<uint8_t*>(obj->GetValuePtrByPropertyNameInChain(L"IsCrouching")))
        *crouching = (player.movState & 0x01) ? 1 : 0;
    if (auto* ads = static_cast<uint8_t*>(obj->GetValuePtrByPropertyNameInChain(L"IsADS")))
        *ads = (player.movState & 0x02) ? 1 : 0;
    if (auto* falling = static_cast<uint8_t*>(obj->GetValuePtrByPropertyNameInChain(L"Falling")))
        *falling = (player.movState & 0x04) ? 1 : 0;
}

static void on_process_event_post(UObject* obj, UFunction* func, void* /*params*/)
{
    if (!obj || !func) return;

    // Lazily resolve once, off the LOCAL player's own AnimInstance — the
    // UFunction object is shared across every Player_AnimBP_C instance
    // (proxies included), so a single resolution covers all of them for the
    // rest of the session. Throttled the same way s_drop_fn's lookup is
    // above: find_local_pawn() is an expensive reflection scan and this
    // fires on every ProcessEvent call otherwise.
    if (!s_lastUpdateFn) {
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
                            s_lastUpdateFn = aparams.ReturnValue->GetFunctionByNameInChain(L"BlueprintThreadSafeUpdateAnimation");
                    }
                }
            }
        }
    }

    // Cheap pointer-compare fast path — skips essentially every ProcessEvent
    // call in the game (this fires thousands of times per frame); only
    // BlueprintThreadSafeUpdateAnimation calls (one per Player_AnimBP_C
    // instance per frame, a small handful total) do any real work below.
    if (func != s_lastUpdateFn) return;

    UFunction* getOwnerFn = obj->GetFunctionByNameInChain(L"GetOwningActor");
    if (!getOwnerFn) return;
    struct OwnerParams { AActor* ReturnValue = nullptr; } oparams;
    obj->ProcessEvent(getOwnerFn, &oparams);
    AActor* owner = oparams.ReturnValue;
    if (!owner) return;

    // MUST be non-blocking. This hook fires on every ProcessEvent call
    // globally, including ones nested *inside* ProxyManager::tick()'s own
    // sync burst (tick() holds this exact mutex for its whole duration
    // while making many ProcessEvent calls, any of which can indirectly
    // trigger a BlueprintThreadSafeUpdateAnimation call that matches this
    // hook's fast path). A blocking std::lock_guard here is the *exact*
    // same self-deadlock/UB class as the original do_game_tick reentrancy
    // bug (see [[feedback_sdo_isolate_risky_proxy_changes]] — locking a
    // non-recursive std::mutex twice on the same thread) — just through a
    // brand new acquisition site that do_game_tick's own reentry guard
    // doesn't cover, since this function doesn't go through do_game_tick
    // at all. Live-tested 2026-08-13: PC1 crashed the instant PC2's proxy
    // spawned in even with a 2s post-spawn grace period added (a real but
    // secondary concern, not the actual cause). If the lock isn't
    // immediately available, just skip this one frame's override — the
    // same override gets reattempted on the very next per-frame call,
    // completely harmless to skip once.
    std::unique_lock<std::mutex> lk(sdb::g_state().playersMtx, std::try_to_lock);
    if (!lk.owns_lock()) return;

    for (auto& [id, player] : sdb::g_state().players) {
        if (static_cast<AActor*>(player.proxyActor) != owner) continue;

        // Same 2s post-spawn grace period ProxyManager::tick() already uses
        // before its own heavy sync calls (RemotePlayer::proxySpawnedAtUs) —
        // a freshly-spawned proxy's components aren't fully ready yet.
        // TEMPORARILY DISABLED 2026-08-13 for testing at the user's request
        // — this is the aim pitch/yaw write path specifically, a much
        // lighter operation (two pointer writes) than the equipment-sync
        // burst that originally caused this grace period to be added; may
        // not need it at all. Re-enable if proxies crash/deadlock right
        // after spawn again.
        // if (sdb::now_micros() - player.proxySpawnedAtUs < 2'000'000ULL) break;

        // Smoothed values (ProxyManager::update_proxy_render_smoothing, run
        // from the game tick), not the raw packet-driven aimPitchByte/aimYaw
        // directly — 2026-08-13, the raw values only change once per network
        // packet (~50ms) and writing them straight in here every frame
        // produced a visible step/stutter, most noticeable while ADS. Yaw
        // (left/right look) is new this session — previously not sent at
        // all (mv.aimYaw was a copy-paste bug duplicating body yaw, not real
        // camera yaw), so it silently never worked; see Player_AnimBP.hpp's
        // Yaw property (0x5AF8, right after Pitch at 0x5AF0 — same struct,
        // same GetAimOffset per-frame reset-to-zero this whole mechanism
        // already exists to win against for Pitch).
        // 2026-08-14, SEH-hardened after a live crash: PC2 crashed
        // (EXCEPTION_ACCESS_VIOLATION writing 0x4ec — a near-null address,
        // consistent with a stale/dangling AnimInstance pointer) with the
        // very last log line before the crash being this function's own
        // aim_write diagnostic. This block has written through `obj`
        // directly, unguarded, since Session 53 — the exact same
        // stale-pointer-during-proxy-destroy/respawn race this project has
        // hardened everywhere else (see ProxyManager's own SEH-wrapped
        // equivalents), just never SEH-wrapped here. Not a confirmed root
        // cause (no debugger was attached to catch the fault directly), but
        // matches the established risk pattern closely enough to be worth
        // hardening regardless of the exact trigger — same reasoning as
        // every other proactive SEH-wrap in this codebase.
        AimWriteCtx ctx{ obj, &player };
        if (!seh_invoke(do_aim_write, &ctx))
            debug_log("on_process_event_post: aim/state write crashed, caught via SEH");
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
