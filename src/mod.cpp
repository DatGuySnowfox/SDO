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
#include <shellapi.h>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cmath>
#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
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

// Steam launch-option support (2026-08-26): steam://run/<appid>//-sdb_host=
// ... -sdb_port=... -sdb_ticket=... lets a browser-based "Join" button
// (directory-worker's status page) launch straight into a server with zero
// local script/download — Steam appends everything after the trailing //
// as literal argv tokens on the launched process. Overwrites (not merged
// under) session.cfg's values when present, since a launch-arg join means
// "connect to THIS server right now", superseding whatever stale ticket a
// previous session might have left in session.cfg. Ticket format is
// base64url (server/src/lib/ticket.js: "<base64url>.<base64url>") —
// deliberately URL- and command-line-safe characters only
// ([A-Za-z0-9_.-]), so no quoting/escaping is needed on either the URI or
// argv side. Only recognizes our own sdb_* switches — never touches
// anything else on the command line (UE's own engine switches etc.).
static void merge_command_line_args(std::unordered_map<std::string,std::string>& m)
{
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) return;

    static const std::unordered_map<std::wstring, const char*> kKeyMap = {
        { L"sdb_host",   "SDB_GATEWAY_HOST" },
        { L"sdb_port",   "SDB_GATEWAY_PORT" },
        { L"sdb_ticket", "SDB_JOIN_TICKET" },
    };

    for (int i = 1; i < argc; ++i) {
        const std::wstring arg(argv[i]);
        if (arg.size() < 2 || arg[0] != L'-') continue;
        const size_t eq = arg.find(L'=');
        if (eq == std::wstring::npos) continue;

        const std::wstring wkey = arg.substr(1, eq - 1);
        const auto it = kKeyMap.find(wkey);
        if (it == kKeyMap.end()) continue;

        const std::wstring wval = arg.substr(eq + 1);
        const int needed = WideCharToMultiByte(CP_UTF8, 0, wval.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (needed <= 0) continue;
        std::string val(static_cast<size_t>(needed - 1), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wval.c_str(), -1, val.data(), needed, nullptr, nullptr);
        m[it->second] = std::move(val);
    }
    LocalFree(argv);
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
    // Fallback (primary path above found no PlayerController or no
    // possessed Pawn — e.g. a brief window during a respawn/teleport-like
    // transition). Previously: FindFirstOf(BP_PlayerCharacter_C), which is
    // ambiguous — it matches ANY live instance of the class, including
    // locally-spawned proxy actors (proxies are also BP_PlayerCharacter_C,
    // see spawn_proxy()). Live-confirmed 2026-08-17: this fallback grabbed
    // a proxy actor instead of the real local pawn during an artificial
    // same-position "teleport" (a direct server-side saved-position edit
    // moved this player to another player's exact live coordinates); the
    // wrong pointer then got locked into cached_find_local_pawn()'s 100ms
    // cache for the rest of the session, since the primary path above never
    // got a chance to fail loud enough to notice — every 100ms re-check
    // silently returned the same wrong actor. Confirmed via
    // send_pawn_appearance broadcasting the OTHER player's real appearance
    // (isMale/hair/body all matched them, not this client's own character).
    // Fix: scan every candidate and skip any pointer already tracked as
    // someone else's proxyActor, rather than trusting whichever the engine
    // happens to enumerate first.
    std::vector<UObject*> candidates;
    UObjectGlobals::FindAllOf(STR("BP_PlayerCharacter_C"), candidates);
    if (candidates.empty()) return nullptr;
    if (candidates.size() == 1) return static_cast<AActor*>(candidates[0]);

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    for (UObject* obj : candidates) {
        bool isKnownProxy = false;
        for (auto& [id, player] : sdb::g_state().players) {
            if (player.proxyActor == obj) { isKnownProxy = true; break; }
        }
        if (!isKnownProxy) return static_cast<AActor*>(obj);
    }
    // Every candidate is a known proxy — shouldn't happen, but return the
    // first rather than null so callers still get something to work with.
    return static_cast<AActor*>(candidates[0]);
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
static void check_head_rot_diagnostic(AActor* pawn);
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

// 2026-08-16: dedicated health-only read for the per-tick death-detection
// check below — read_local_progress() above ALSO does a UObjectGlobals::
// FindFirstOf(L"BP_PlayerController_C") full reflection scan (for level/xp/
// forename/etc.), fine on its original throttled send_profile_revision()
// cadence but catastrophic called every do_game_tick (~200Hz): live-
// reproduced as PC2 dropping to 1 FPS "as soon as it loads into the world"
// the instant this project's own do_game_tick started calling
// read_local_progress() unconditionally every tick — the exact same "FindFirstOf
// every tick" pattern already documented multiple times elsewhere in this
// codebase (find_local_pawn()/FindAllOf regressions). Health alone needs
// nothing past the MedicalComponent pointer chase, no controller lookup at all.
static double read_local_health_only(AActor* pawn)
{
    const auto base = reinterpret_cast<uintptr_t>(pawn);
    const uintptr_t med = *reinterpret_cast<uintptr_t*>(base + 0x7D0);
    return med ? *reinterpret_cast<double*>(med + 0xD0) : 100.0;
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

// TEMPORARY diagnostic (2026-08-21): head-look sync live A/B — compare the
// LOCAL player's real, correctly-computed HeadRotation/HeadQuat (from the
// real GetHeadRot, which has an actual Controller to read) against the raw
// ControlRotation/ActorRotation delta, to empirically find the real mapping
// instead of guessing further. First deploy's proxy write produced "pitch
// input looks like yaw output, yaw input has no visible effect" — pure
// bytecode+quaternion-math reasoning couldn't explain that, so falling back
// to this project's proven live-diagnostic technique (same one that found
// the NVG roll=0-is-on/roll=-60-is-off inversion). Remove once the real
// mapping is confirmed and the head-write fix is corrected to match.
static void check_head_rot_diagnostic(AActor* pawn)
{
    if (!pawn) return;
    static uint64_t s_lastUs = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (nowUs - s_lastUs < 100'000ULL) return;
    s_lastUs = nowUs;

    UFunction* fn = pawn->GetFunctionByNameInChain(L"GetControlRotation");
    FRotator control{};
    if (fn) {
        struct Params { FRotator ReturnValue; } params{};
        pawn->ProcessEvent(fn, &params);
        control = params.ReturnValue;
    }
    const FRotator actorRot = pawn->K2_GetActorRotation();

    auto** meshSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) { debug_log("head_rot_diag: Mesh not found"); return; }
    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) { debug_log("head_rot_diag: GetAnimInstance NOT FOUND"); return; }
    struct AnimParams { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) { debug_log("head_rot_diag: AnimInstance is null"); return; }
    auto* anim = aparams.ReturnValue;

    struct FRotatorD { double Pitch, Yaw, Roll; };
    struct FQuatD    { double X, Y, Z, W; };
    auto* headRot  = static_cast<FRotatorD*>(anim->GetValuePtrByPropertyNameInChain(L"HeadRotation"));
    auto* headQuat = static_cast<FQuatD*>(anim->GetValuePtrByPropertyNameInChain(L"HeadQuat"));

    char buf[400];
    snprintf(buf, sizeof(buf),
             "head_rot_diag: controlPitch=%.2f controlYaw=%.2f actorYaw=%.2f | "
             "headRot(P=%.2f Y=%.2f R=%.2f) headQuat(X=%.3f Y=%.3f Z=%.3f W=%.3f)",
             control.Pitch, control.Yaw, actorRot.Yaw,
             headRot ? headRot->Pitch : -999.0, headRot ? headRot->Yaw : -999.0, headRot ? headRot->Roll : -999.0,
             headQuat ? headQuat->X : -9.0, headQuat ? headQuat->Y : -9.0,
             headQuat ? headQuat->Z : -9.0, headQuat ? headQuat->W : -9.0);
    debug_log(buf);
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

        // 2026-08-17: live ActivateState per attachment, for toggleable
        // tactical lights/lasers (flashlight/NVG sync request). RepAttachments
        // above only carries DataAsset metadata, not a live actor reference —
        // walk childComp's OWN AttachChildren (same "one level deeper" nested
        // scan do_attach_health_scan already does for weapon sub-attachments)
        // to find the actual spawned BP_<Item>LocalAttachment_C actors, match
        // by itemId. These actors are NOT BP_JigPickupComponent-based (a
        // completely different class hierarchy from the weapon/pickup actors
        // above) — their itemId comes from ABP_AMainLocalAttachment_C's own
        // "Attachment Info" UJigsawItem_DataAsset_C* field (@0x02C0), and
        // their on/off state from the sibling ActivateState FGameplayTag
        // (@0x02C8, shared by every attachment subclass including
        // TacticalLaserLightCombo/SmallTacticalLight/SmallTacticalLaserSight/
        // TacticalLight — research/CXXHeaderDump/BP_AMainLocalAttachment.hpp).
        // Heuristic, not yet live-confirmed against the exact tag values
        // Jig_SetAttachmentActiveState uses: ComparisonIndex==0 (NAME_None,
        // the tag's default/unset state) treated as inactive, anything else
        // as active — matches this project's own "resolve GameplayTags by
        // name not raw CI" caution (CI values aren't stable across restarts)
        // only in that this doesn't hardcode any specific CI at all, just its
        // zero-ness.
        std::unordered_map<std::string, bool> nestedActiveByItemId;
        {
            const uintptr_t nestedData  = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(childComp) + 0x00C0);
            const int32_t   nestedCount = *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(childComp) + 0x00C0 + 0x08);
            if (nestedData && nestedCount > 0 && nestedCount <= 32) {
                for (int32_t j = 0; j < nestedCount; ++j) {
                    UObject* nestedComp = *reinterpret_cast<UObject**>(nestedData + static_cast<size_t>(j) * 8);
                    if (!nestedComp) continue;
                    UFunction* nestedGetOwnerFn = nestedComp->GetFunctionByNameInChain(L"GetOwner");
                    AActor* nestedOwner = nullptr;
                    if (nestedGetOwnerFn) nestedComp->ProcessEvent(nestedGetOwnerFn, &nestedOwner);
                    if (!nestedOwner) continue;

                    const uintptr_t attInfoDA = *reinterpret_cast<uintptr_t*>(
                        reinterpret_cast<uintptr_t>(nestedOwner) + 0x02C0);
                    if (!attInfoDA) continue;
                    std::string nestedItemId = native::fname_to_string(attInfoDA + 0x30);
                    if (nestedItemId.empty()) continue;

                    const uint32_t activateCi = *reinterpret_cast<const uint32_t*>(
                        reinterpret_cast<uintptr_t>(nestedOwner) + 0x02C8);
                    nestedActiveByItemId[nestedItemId] = (activateCi != 0);
                }
            }
        }

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
            auto activeIt = nestedActiveByItemId.find(attItemId);
            wae.active = (activeIt != nestedActiveByItemId.end()) && activeIt->second;
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
        if (st.progressRevision == 0) {
            // Seed from wall-clock instead of starting at 0 — the server
            // only accepts a save whose revision is >= the last one it has
            // stored (db.js's upsert CASE), but this counter has no
            // persistence of its own and resets to 0 every process
            // relaunch. Without this seed, any session shorter than
            // roughly (storedRevision * 30s) — the time needed for this
            // counter, incrementing once per periodic send, to climb back
            // past the stored value — has EVERY save silently rejected,
            // including hunger/thirst/position/inventory, not just one
            // field. Live-confirmed 2026-08-17: repeated short relaunches
            // during barber-menu testing left this player's last accepted
            // save an hour stale. Unix seconds comfortably fits uint32_t
            // (rolls over ~2106) and is always far larger than any
            // realistic counter-based revision already in the DB, so the
            // very first save of any new session is guaranteed to be
            // accepted.
            st.progressRevision = static_cast<uint32_t>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count());
        }
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

// 2026-08-17: character-level light toggles (flashlight/NVG sync request) —
// distinct from Equipment's slot 16 (whether a flashlight ITEM is equipped
// at all). BP_PlayerCharacter.hpp: FlashlightOn? is a plain bool @0x13E5,
// PlayerUsingNightVision? @0x1401 — both read directly, no ProcessEvent
// needed, same convention as isMale@0x15A0 in read_local_pawn_appearance.
static sdb::PlayerLights read_local_player_lights(AActor* pawn)
{
    sdb::PlayerLights out;
    if (!pawn) return out;
    out.flashlightOn  = *reinterpret_cast<const bool*>(reinterpret_cast<uintptr_t>(pawn) + 0x13E5);
    out.nightVisionOn = *reinterpret_cast<const bool*>(reinterpret_cast<uintptr_t>(pawn) + 0x1401);

    // Ground-truth from FlashlightToggle's own decoded bytecode: the real
    // on-value is computed per-equipped-item, not a fixed constant. Read it
    // live off this player's own Flashlight component (BP_PlayerCharacter.hpp
    // @0x0738, a USpotLightComponent — Intensity sits at ULightComponentBase
    // +0x02B4, research/CXXHeaderDump/Engine.hpp) rather than guess a value —
    // only meaningful while flashlightOn is true; left at 0 otherwise so an
    // off-but-stale intensity never gets misread as "was bright".
    if (out.flashlightOn) {
        auto* lightSlot = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(pawn) + 0x0738);
        if (lightSlot)
            out.flashlightIntensity = *reinterpret_cast<const float*>(
                reinterpret_cast<uintptr_t>(lightSlot) + 0x02B4);
    }
    return out;
}

static void send_player_lights(AActor* pawn)
{
    const sdb::PlayerLights pl = read_local_player_lights(pawn);

    static bool  s_lastFlashlight  = false;
    static bool  s_lastNightVision = false;
    static float s_lastIntensity   = 0.0f;
    static bool  s_haveSent        = false;
    if (s_haveSent && pl.flashlightOn == s_lastFlashlight && pl.nightVisionOn == s_lastNightVision &&
        pl.flashlightIntensity == s_lastIntensity) return;
    s_haveSent = true;
    s_lastFlashlight  = pl.flashlightOn;
    s_lastNightVision = pl.nightVisionOn;
    s_lastIntensity   = pl.flashlightIntensity;

    debug_log("send_player_lights: flashlightOn=" + std::to_string(pl.flashlightOn) +
              " nightVisionOn=" + std::to_string(pl.nightVisionOn) +
              " flashlightIntensity=" + std::to_string(pl.flashlightIntensity));

    sdb::Frame f;
    f.type    = sdb::MsgType::PlayerLights;
    f.payload = sdb::encode_player_lights(pl);
    build_session_frame(f);
    send_frame(f);
}

// 2026-08-21: weapon-fire detection via three separate ProcessEvent hooks
// (FireBullet alone, Svr_WeaponShot, Local_StartShooting — see
// s_fireBullet_fn's comment for the full trace) all failed to catch
// single-fire's only bullet, because every one of those calls happens
// *inside* an already-executing Kismet function and never re-enters
// ProcessEvent, however it's wrapped (Local, plain Virtual, even
// cross-object via Context). Switched to this project's own established,
// proven pattern instead — same as read_local_player_lights's FlashlightOn?/
// NightVisionOn? polling: read a persistent instance property directly and
// edge-detect against a static last-value, no ProcessEvent involved at all.
// BP_PlayerCharacter.hpp: CurrentFiringWeapon (ABP_FirearmPickup_C*, offset
// 0x0930) is set to the equipped weapon actor right before
// Local_StartShooting is called, and cleared to null by Local_StopShooting
// — confirmed by tracing InpActEvt_IA_PrimaryAction_..._64/_65's real
// bytecode in BP_PlayerCharacter_C's Ubergraph (entries 60116/59833). A
// null→non-null edge here fires exactly once per trigger pull, single or
// auto alike — full-auto's subsequent rounds are still covered by
// s_fireBullet_fn's Timer-driven hook, so no double-broadcast. Same
// accepted tradeoff as any of the three hook attempts would have had: ammo
// isn't checked until inside Local_StartShooting, so a dry-fire on an empty
// weapon also edges here and broadcasts a phantom WeaponFired.
static void check_weapon_fire_edge(AActor* pawn)
{
    if (!pawn) return;
    auto* current = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pawn) + 0x0930);

    static void* s_lastCurrentFiringWeapon = nullptr;
    const bool edge = current && !s_lastCurrentFiringWeapon;
    s_lastCurrentFiringWeapon = current;
    if (!edge) return;

    debug_log("check_weapon_fire_edge: CurrentFiringWeapon null->non-null, sent WeaponFired");
    send_header_only(sdb::MsgType::WeaponFired);
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

    case sdb::MsgType::PlayerLights:
        debug_log("dispatch_frame: PlayerLights received playerId=" + std::to_string(f.playerId) +
                  " ownId=" + std::to_string(st.session.playerId) +
                  " payloadSize=" + std::to_string(f.payload.size()));
        if (f.playerId && f.playerId != st.session.playerId
                       && !f.payload.empty()) {
            if (auto pl = sdb::decode_player_lights(f.payload.data(), f.payload.size()))
                sdb::g_proxy_manager().on_player_lights(f.playerId, *pl);
            else
                debug_log("dispatch_frame: PlayerLights decode_player_lights FAILED");
        }
        break;

    case sdb::MsgType::WeaponFired:
        // Header-only (no payload) — the receiving player and slot are
        // enough info; replay just needs to touch whichever weapon actor is
        // currently tracked as that player's active/in-hand weapon. See
        // on_process_event_pre's own comment for the full sender-side story.
        if (f.playerId && f.playerId != st.session.playerId) {
            sdb::g_proxy_manager().on_weapon_fired(f.playerId);
        }
        break;

    case sdb::MsgType::FirstJoin:
        // Authoritative "this playerId has never saved before" from the
        // server (gateway.js sends this instead of PlayerProgressRestore
        // when db.getProgress() is empty) — replaces the old 6s-timeout
        // heuristic. Actual handling (teleport to barber, open menu) is
        // deferred to do_game_tick since a pawn may not exist yet at the
        // moment this arrives.
        st.pendingFirstJoin.store(true, std::memory_order_release);
        Output::send<LogLevel::Normal>(STR("SDB: FirstJoin received — new player\n"));
        break;

    case sdb::MsgType::Death:
        // This is a per-connection confirmation (gateway.js routes MsgType::Death
        // only back to the connection that sent DeathRequest — see that file's
        // "Sent to a specific client" comment), so a player's own death arrives
        // back at their own client with f.playerId == their own id. Every sibling
        // case in this cluster (Equipment/WeaponAttachments/PawnAppearance/
        // PlayMontage, immediately above) guards against exactly this with
        // f.playerId != st.session.playerId before touching the proxy manager —
        // this case needs the same guard, otherwise the local player's own
        // death would default-construct a RemotePlayer entry for their own id
        // and ProxyManager::tick() would spawn a duplicate self-proxy.
        //
        // 2026-08-16: REMOVED the on_player_disconnected()+on_player_connected()
        // pair this case used to call before setting dead=true. That combo
        // fully destroys the proxy actor AND every equipped visual actor
        // (weapons/headwear/facewear/etc. — see on_player_disconnected's own
        // cleanup list) and erases the player's entire cached state, then
        // recreates a blank RemotePlayer with x/y/z/yaw defaulted to 0 and no
        // equipment. Nothing re-populates that data on death (the server
        // doesn't resend Equipment/PawnAppearance just because someone died),
        // so the visible result was exactly the live-reported symptom: the
        // model disappears (proxy destroyed) rather than dying (no replacement
        // ever gets dressed or positioned correctly — it's sitting undressed
        // at the world origin, not visibly "respawning" at all). The
        // destroy+recreate was solving a real but different problem (the
        // missing self-guard, immediately above) — the guard alone is
        // sufficient; the proxy itself should simply persist through death.
        // ProxyManager::tick() already stops teleporting/rotating a proxy
        // once player.dead is true (see its own `if (!player.dead)` gate),
        // so the character now freezes in place at its death location,
        // fully dressed, instead of vanishing — not a real death animation,
        // but a correct starting point rather than actively wrong.
        if (f.playerId && f.playerId != st.session.playerId) {
            std::lock_guard<std::mutex> lk(st.playersMtx);
            auto& p = st.players[f.playerId]; // ensure an entry exists without wiping one that already does
            p.playerId = f.playerId;
            p.dead = true;
            debug_log("dispatch_frame: Death for remote playerId=" + std::to_string(f.playerId) +
                      " proxyActor=0x" + [&]{ char b[20]; snprintf(b, sizeof(b), "%llx",
                      (unsigned long long)(uintptr_t)p.proxyActor); return std::string(b); }() +
                      " renderPos=(" + std::to_string(p.renderX) + "," + std::to_string(p.renderY) +
                      "," + std::to_string(p.renderZ) + ")");
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
// 2026-08-15: live-captured hang dump proved UE4SS's ProcessEvent hooks
// fire on ANY thread calling ProcessEvent, not just GameThread — a
// "Background Worker" thread's stack showed UE4SS!ProcessEvent calling
// straight into our own main.dll code (on_process_event_pre/post), several
// frames deep, ending in a blocked engine-internal wait. Neither hook ever
// checked which thread it was running on — do_game_tick()'s own reentry
// guard (a plain non-atomic bool) and on_process_event_pre's sample counter
// (a plain non-atomic uint32_t) both explicitly assumed GameThread-only
// execution ("safe since both callers are always on the game thread"),
// which this dump disproves. Almost every reflection/ProcessEvent call this
// mod makes assumes GameThread (UE's own engine APIs mostly aren't
// thread-safe off it), so if either hook's real work ever ran from a
// worker thread, that's a plausible root cause for hangs unrelated to any
// specific function called — not just SetSkinnedAssetAndUpdate. Captured
// once in on_unreal_init (which UE4SS itself only ever calls from
// GameThread), then both hooks below bail out immediately if the calling
// thread doesn't match.
static DWORD g_game_thread_id = 0;
// 2026-08-15: on_actor_tick reliability watchdog — see on_actor_tick's own
// comment for the full rationale. g_last_actor_tick_us is updated every
// time the clean (non-nested) hook actually fires; g_actor_tick_ever_fired
// distinguishes "never worked this session" (don't re-register — nothing to
// recover, and re-registering before the world/hook table even exists
// could be its own risk) from "was working, now silent" (genuinely worth
// recovering). g_last_actor_tick_reregister_us throttles recovery attempts
// so a persistently-broken hook doesn't get re-registered every single
// on_process_event_pre sample.
static std::atomic<uint64_t> g_last_actor_tick_us{0};
static std::atomic<bool>     g_actor_tick_ever_fired{false};
static std::atomic<uint64_t> g_last_actor_tick_reregister_us{0};

// ProcessEvent nesting depth, THIS thread only (ProcessEvent can fire from a
// Background Worker thread, not just GameThread — see on_process_event_pre's
// own comment — so this must be thread_local, not a shared global/atomic; a
// worker thread's own ProcessEvent activity has no bearing on whether
// GameThread is currently nested). Incremented as the literal first
// statement of on_process_event_pre, decremented as the literal first
// statement of on_process_event_post, so every increment is guaranteed a
// matching decrement regardless of what either hook's own early-return logic
// does afterward. 2026-08-16: added as the verification signal for the
// WndProc-triggered clean tick — see do_risky_game_tick_work's own comment.
// A value of 0 at the moment that trigger fires proves the "engine's Windows
// message pump runs outside any ProcessEvent dispatch" assumption in
// practice, not just in theory.
static thread_local uint32_t t_processEventDepth = 0;

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
// file-flag IPC pattern used by the other check_*_trigger functions
// throughout this file.
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
// UObject::GetClassPrivate() in the vendored UE4SS stub header is declared
// on the wrong class for its own mangled name (the header's comment shows
// the true export as `UObjectBase::GetClassPrivate`, but the C++ declaration
// puts it on `UObject`, so the import lib entry the linker generates never
// matches the DLL's actual export and this fails LNK2019). Resolved by
// address instead — same technique already proven in proxy_manager.cpp's
// own copy of this exact function (kept as a separate file-local copy there
// rather than shared, so duplicated here too). MSVC returns a reference
// (UClass*&) as a pointer to the referenced storage in RAX, so the resolved
// signature returns UClass** here, not UClass*.
static UClass* get_class_private(UObject* obj)
{
    using Fn = UClass**(__fastcall*)(void*);
    static Fn fn = [] {
        auto* ue4ss = GetModuleHandleW(L"UE4SS.dll");
        return ue4ss ? reinterpret_cast<Fn>(GetProcAddress(ue4ss,
            "?GetClassPrivate@UObjectBase@Unreal@RC@@QEAAAEAPEAVUClass@23@XZ")) : nullptr;
    }();
    if (!fn || !obj) return nullptr;
    UClass** ref = fn(obj);
    return ref ? *ref : nullptr;
}

// Flag-triggered, on-demand test of force-opening CharacterBarberMenu via
// the STANDARD engine widget-creation API (WidgetBlueprintLibrary::Create +
// UUserWidget::AddToViewport) rather than trying to reverse-engineer
// BP_Barber_C's own internal trigger — confirmed live 2026-08-17 via a full
// ProcessEvent trace that none of BP_Barber_C's 8 interact-related
// functions fire when the menu opens; CharacterBarberMenu_C::Construct
// fires instead, meaning the actual open call is a native engine operation,
// not something specific to this game's Blueprint logic. This is an
// ISOLATED test — deliberately not wired into any "first join" logic yet,
// given the recent live data-corruption incident from an unverified
// ProcessEvent call (see feedback_sdo_live_data_corruption_incident
// memory). Lower risk than that incident: this only creates a UI widget,
// it doesn't touch persisted player save data even if the param layout
// turns out wrong — worst case is a failed/garbage widget or a crash, not
// silent save corruption.
// Tracks the single most recent force-created widget so repeated triggers
// during testing don't pile up overlapping instances — every earlier call
// left its widget on screen forever (never removed), and by this point in
// the session there were likely several stacked CharacterBarberMenu_C
// instances. That fully explains the flaky exit behavior seen live: which
// widget actually receives the click (and whether IT specifically still
// has a working camera/delegate state) becomes unpredictable once more
// than one is alive. Removed via RemoveFromParent() before creating a new
// one.
static UObject* s_lastForcedBarberWidget = nullptr;

static void do_restore_camera_and_input(); // defined below, used by the poller here
static bool teleport_to_random_spawn_point(AActor* pawn); // defined below, used by the poller here

// Shows/hides the corner health/hunger/thirst/stamina HUD while the barber
// menu is open — user-requested 2026-08-17. UBP_Ingame_C (the persistent
// main HUD widget) already exposes exactly this via its own
// StatsVisibilty(bool Hide) UFunction (CXXHeaderDump/BP_Ingame.hpp) — a
// clean existing toggle, not a raw property write, so no risk of guessing
// a wrong struct layout for something core-HUD-related.
static void set_ingame_stats_hidden(bool hide)
{
    UObject* ingame = UObjectGlobals::FindFirstOf(STR("BP_Ingame_C"));
    if (!ingame) { debug_log("set_ingame_stats_hidden: no live BP_Ingame_C instance"); return; }
    UFunction* statsVisFn = ingame->GetFunctionByNameInChain(L"StatsVisibilty");
    if (!statsVisFn) { debug_log("set_ingame_stats_hidden: StatsVisibilty UFunction not found"); return; }
    struct StatsVisParams { bool Hide = false; } params;
    params.Hide = hide;
    struct Ctx { UObject* obj; UFunction* fn; StatsVisParams* p; };
    Ctx ctx{ ingame, statsVisFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);
    debug_log(ok ? (hide ? "set_ingame_stats_hidden: hidden" : "set_ingame_stats_hidden: shown")
                  : "set_ingame_stats_hidden: StatsVisibilty crashed, caught via SEH");
}

// More robust alternative to hooking BP_Barber_C::Event_Exit (see
// check_barber_exit_hook's own long comment for the two wrong guesses that
// preceded this) — that event fires reliably on a *normally*-opened
// widget but was observed live to NOT fire consistently on this trigger's
// force-created one (unclear exact cause, possibly some other state the
// real open flow sets up that this replicates only partially). Rather than
// keep guessing at which specific Blueprint-internal event to catch, this
// polls the widget's own IsInViewport() state directly — the moment it's
// no longer in the viewport, the menu is closed, *regardless of how* that
// happened (Exit button, Escape key, or anything else), so this needs no
// theory about the exact internal close mechanism to be correct. Runs
// every do_game_tick call (already throttled to ~5ms), own 200ms throttle
// on top since a bool check doesn't need to be that frequent.
static void check_barber_widget_removed_poller()
{
    if (!s_lastForcedBarberWidget) return;
    static uint64_t s_lastPollUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastPollUs < 200'000ULL) return;
    s_lastPollUs = now;

    UFunction* isInViewportFn = s_lastForcedBarberWidget->GetFunctionByNameInChain(L"IsInViewport");
    if (!isInViewportFn) return;
    struct IsInViewportParams { bool ReturnValue = false; } params;
    struct PollCtx { UObject* obj; UFunction* fn; IsInViewportParams* p; };
    PollCtx pctx{ s_lastForcedBarberWidget, isInViewportFn, &params };
    const bool pollOk = seh_invoke([](void* raw) {
        auto* c = static_cast<PollCtx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &pctx);
    if (!pollOk) {
        debug_log("check_barber_widget_removed_poller: IsInViewport crashed, caught via SEH — stopping poll");
        s_lastForcedBarberWidget = nullptr;
        return;
    }
    if (params.ReturnValue) return; // still open, nothing to do

    debug_log("check_barber_widget_removed_poller: widget no longer in viewport — restoring camera/input");
    s_lastForcedBarberWidget = nullptr;
    if (!seh_invoke([](void*) { do_restore_camera_and_input(); }, nullptr))
        debug_log("check_barber_widget_removed_poller: do_restore_camera_and_input crashed, caught via SEH");

    // First-join flow only: an organic/manually-triggered barber visit
    // should never randomly relocate the player, so this only fires when
    // do_open_barber_menu() was itself kicked off by the server's
    // FirstJoin signal (see do_game_tick's first-join handling).
    auto& st = sdb::g_state();
    if (st.inFirstJoinFlow) {
        st.inFirstJoinFlow = false;
        if (AActor* pawn = find_local_pawn()) {
            debug_log("check_barber_widget_removed_poller: first-join flow — teleporting to a random spawn point");
            teleport_to_random_spawn_point(pawn);
        } else {
            debug_log("check_barber_widget_removed_poller: first-join flow but no local pawn, skipping spawn teleport");
        }
    }
}

// Does the actual force-open work — factored out so both the manual
// on-demand flag trigger below AND the real FirstJoin-driven flow
// (do_game_tick's first-join handling) can call the exact same,
// already-proven sequence rather than duplicating it.
static void do_open_barber_menu()
{
    if (s_lastForcedBarberWidget) {
        if (UFunction* removeFn = s_lastForcedBarberWidget->GetFunctionByNameInChain(L"RemoveFromParent")) {
            struct RemoveCtx { UObject* obj; UFunction* fn; };
            RemoveCtx rctx{ s_lastForcedBarberWidget, removeFn };
            const bool removeOk = seh_invoke([](void* raw) {
                auto* c = static_cast<RemoveCtx*>(raw);
                c->obj->ProcessEvent(c->fn, nullptr);
            }, &rctx);
            debug_log(removeOk ? "open_barber_menu: removed previous force-created widget"
                                : "open_barber_menu: RemoveFromParent on previous widget crashed, caught via SEH");
        }
        s_lastForcedBarberWidget = nullptr;
    }

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("open_barber_menu: no local pawn"); return; }

    UObject* ctrl = UObjectGlobals::FindFirstOf(STR("BP_PlayerController_C"));
    if (!ctrl) { debug_log("open_barber_menu: no local PlayerController"); return; }

    // Resolved early (not just for the camera switch further down) — also
    // needed to populate the widget's own "Barber" property right after
    // creation, see below.
    UObject* barberActor = UObjectGlobals::FindFirstOf(STR("BP_Barber_C"));
    if (!barberActor) { debug_log("open_barber_menu: no live BP_Barber_C actor found"); return; }

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* widgetLib = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__WidgetBlueprintLibrary");
    if (!widgetLib) widgetLib = UObjectGlobals::FindFirstOf(L"WidgetBlueprintLibrary");
    if (!widgetLib) { debug_log("open_barber_menu: WidgetBlueprintLibrary CDO not found"); return; }

    UFunction* createFn = widgetLib->GetFunctionByNameInChain(L"Create");
    if (!createFn) { debug_log("open_barber_menu: Create UFunction not found"); return; }

    // Need CharacterBarberMenu_C's UClass. First choice: its Class Default
    // Object, via the same "Default__<ClassName>" lookup already proven
    // for WidgetBlueprintLibrary/KismetSystemLibrary above/elsewhere in
    // this file — a Blueprint class's CDO is created the moment the class
    // ASSET loads (BP_Barber_C hard-references CharacterBarberMenu_C, so
    // it should already be loaded even with zero gameplay instances ever
    // spawned), so this doesn't need anyone to have opened the menu first.
    // Falls back to reading the class off a live instance (old technique)
    // if the CDO lookup ever comes back empty for some reason.
    UClass* menuClass = nullptr;
    if (UObject* menuCdo = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__CharacterBarberMenu_C")) {
        menuClass = get_class_private(menuCdo);
        if (menuClass) debug_log("open_barber_menu: resolved CharacterBarberMenu_C class via its CDO");
    }
    if (!menuClass) {
        UObject* barberMenuInstance = UObjectGlobals::FindFirstOf(STR("CharacterBarberMenu_C"));
        if (!barberMenuInstance) {
            debug_log("open_barber_menu: no CDO and no live CharacterBarberMenu_C instance to read UClass from — "
                       "open a barber menu manually once first, then retry");
            return;
        }
        menuClass = get_class_private(barberMenuInstance);
    }
    if (!menuClass) { debug_log("open_barber_menu: get_class_private failed"); return; }

    // WidgetBlueprintLibrary::Create(UObject* WorldContextObject,
    // TSubclassOf<UUserWidget> WidgetType, APlayerController* OwningPlayer)
    // -> UUserWidget* — a standard, stable UE5 Blueprint node ("Create
    // Widget"), TSubclassOf flattens to a plain UClass* in the params ABI,
    // return value appended after the declared params (same convention
    // already relied on throughout this file).
    struct CreateParams {
        UObject* WorldContextObject = nullptr;
        UClass*  WidgetType         = nullptr;
        UObject* OwningPlayer       = nullptr;
        UObject* ReturnValue        = nullptr;
    } cparams;
    cparams.WorldContextObject = pawn;
    cparams.WidgetType         = menuClass;
    cparams.OwningPlayer       = ctrl;
    widgetLib->ProcessEvent(createFn, &cparams);

    if (!cparams.ReturnValue) { debug_log("open_barber_menu: Create returned null"); return; }

    // Sanity-check the returned object's class actually matches what we
    // asked for, before trusting it enough to call AddToViewport — catches
    // a wrong params-struct layout producing a garbage/misinterpreted
    // pointer instead of silently proceeding on bad data.
    UClass* returnedClass = get_class_private(cparams.ReturnValue);
    if (returnedClass != menuClass) {
        char line[256];
        snprintf(line, sizeof(line),
                 "open_barber_menu: Create returned obj=0x%llx but its class (0x%llx) doesn't match "
                 "CharacterBarberMenu_C (0x%llx) — NOT calling AddToViewport, params struct is likely wrong",
                 reinterpret_cast<unsigned long long>(cparams.ReturnValue),
                 reinterpret_cast<unsigned long long>(returnedClass),
                 reinterpret_cast<unsigned long long>(menuClass));
        debug_log(line);
        return;
    }

    // Widget context — ground truth from the same decode, offsets
    // 0x0048/0x0071/0x0092: the real flow sets three properties on the
    // widget right after Create(), before showing it: "Chr" (the character
    // being edited, an object ref), "SkipAnim?" (bool True), and "Barber"
    // (the barber actor itself, an object ref). Live-confirmed missing
    // previously — user reported "live preview doesn't work" with the
    // widget otherwise fully functional (camera + input + cursor all
    // working), consistent with the widget having no idea which character
    // to preview. Property names resolved earlier this session via the
    // resolve_fname.flag mechanism against a live process (ci=2248233 ->
    // "Chr", ci=2251247 -> "SkipAnim?", ci=2365656 -> "Barber") — the
    // ComparisonIndex values themselves don't survive a relaunch (same
    // per-process instability as raw pointers), but the property name
    // strings are compile-time-fixed and safe to hardcode. Using
    // GetValuePtrByPropertyNameInChain + direct write (this codebase's
    // established safe pattern, see bShowMouseCursor above) rather than
    // resolving the real SetObjectPropertyByName/SetBoolPropertyByName
    // UFunctions (func=0x41ed5c60/0x41ed67c0) and guessing their params
    // struct layout.
    if (auto** chrSlot = static_cast<UObject**>(cparams.ReturnValue->GetValuePtrByPropertyNameInChain(L"Chr"))) {
        *chrSlot = pawn;
        debug_log("open_barber_menu: Chr set");
    } else {
        debug_log("open_barber_menu: Chr property not found");
    }
    if (auto* skipAnimSlot = static_cast<uint8_t*>(cparams.ReturnValue->GetValuePtrByPropertyNameInChain(L"SkipAnim?"))) {
        *skipAnimSlot = 1;
        debug_log("open_barber_menu: SkipAnim? set");
    } else {
        debug_log("open_barber_menu: SkipAnim? property not found");
    }
    if (auto** barberSlot = static_cast<UObject**>(cparams.ReturnValue->GetValuePtrByPropertyNameInChain(L"Barber"))) {
        *barberSlot = barberActor;
        debug_log("open_barber_menu: Barber set");
    } else {
        debug_log("open_barber_menu: Barber property not found");
    }

    UFunction* addToViewportFn = cparams.ReturnValue->GetFunctionByNameInChain(L"AddToViewport");
    if (!addToViewportFn) { debug_log("open_barber_menu: AddToViewport UFunction not found"); return; }

    struct AddToViewportParams { int32_t ZOrder = 0; } aparams;
    cparams.ReturnValue->ProcessEvent(addToViewportFn, &aparams);
    s_lastForcedBarberWidget = cparams.ReturnValue;
    set_ingame_stats_hidden(true);

    char line[128];
    snprintf(line, sizeof(line), "open_barber_menu: SUCCESS widget=0x%llx",
             reinterpret_cast<unsigned long long>(cparams.ReturnValue));
    debug_log(line);

    // Camera switch — ground truth from the fully-resolved BP_Barber_C
    // Ubergraph decode (OnExecuteInteract, entry point 1514, offset 0x05ea):
    // the real game calls PlayerController::SetViewTargetWithBlend(NewView
    // Target=Self [the BP_Barber_C actor itself, since EX_Self inside that
    // actor's own bytecode refers to it], BlendTime=0.5,
    // BlendFunc=0/VTBlend_Linear, BlendExp=0.0, bLockOutgoing=False). Params
    // struct order/types read directly off that disassembly, not guessed —
    // see research/bytecode/ubergraph_decoded/
    // BP_Barber_C_ExecuteUbergraph_BP_Barber.decoded.txt lines ~412-439.
    // Deliberately NOT adding the two calls that follow it in the real flow
    // (ChangePlayerPerspective, SetPlayerGearVisibility) yet — those are
    // additional unverified-signature calls layered on top, and per the
    // isolate-risky-changes lesson from the recent corruption incident this
    // is being tested one new call at a time rather than bundled.
    // (barberActor already resolved above, before Create().)
    UFunction* viewTargetFn = ctrl->GetFunctionByNameInChain(L"SetViewTargetWithBlend");
    if (!viewTargetFn) {
        debug_log("open_barber_menu: SetViewTargetWithBlend UFunction not found, skipping camera switch");
        return;
    }
    struct SetViewTargetWithBlendParams {
        AActor* NewViewTarget = nullptr;
        float   BlendTime     = 0.0f;
        uint8_t BlendFunc     = 0;   // EViewTargetBlendFunction::VTBlend_Linear
        float   BlendExp      = 0.0f;
        bool    bLockOutgoing = false;
    } vparams;
    vparams.NewViewTarget = reinterpret_cast<AActor*>(barberActor);
    vparams.BlendTime     = 0.5f;
    struct ViewCtx { UObject* obj; UFunction* fn; SetViewTargetWithBlendParams* p; };
    ViewCtx vctx{ ctrl, viewTargetFn, &vparams };
    const bool viewOk = seh_invoke([](void* raw) {
        auto* c = static_cast<ViewCtx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &vctx);
    debug_log(viewOk ? "open_barber_menu: SetViewTargetWithBlend called"
                      : "open_barber_menu: SetViewTargetWithBlend crashed, caught via SEH");

    // Input mode — also ground truth from the same decode (offset 0x00f7):
    // WidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController,
    // InWidgetToFocus=the created widget, InMouseLockMode=0, <bool>=False).
    // 4-param signature (not the commonly-documented 3-param one) confirmed
    // directly from the disassembly's param count, not assumed. Needed for
    // the widget to actually receive mouse/keyboard input rather than just
    // being visually overlaid — without it the menu is a static picture.
    UFunction* inputModeFn = widgetLib->GetFunctionByNameInChain(L"SetInputMode_UIOnlyEx");
    if (!inputModeFn) {
        debug_log("open_barber_menu: SetInputMode_UIOnlyEx UFunction not found, skipping");
        return;
    }
    struct SetInputModeUIOnlyExParams {
        UObject* PlayerController = nullptr;
        UObject* InWidgetToFocus  = nullptr;
        uint8_t  InMouseLockMode  = 0;
        bool     Param4           = false;
    } iparams;
    iparams.PlayerController = ctrl;
    iparams.InWidgetToFocus  = cparams.ReturnValue;
    struct InputCtx { UObject* obj; UFunction* fn; SetInputModeUIOnlyExParams* p; };
    InputCtx ictx{ widgetLib, inputModeFn, &iparams };
    const bool inputOk = seh_invoke([](void* raw) {
        auto* c = static_cast<InputCtx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ictx);
    debug_log(inputOk ? "open_barber_menu: SetInputMode_UIOnlyEx called"
                       : "open_barber_menu: SetInputMode_UIOnlyEx crashed, caught via SEH");

    // bShowMouseCursor — same decode (offset 0x0146), a direct property
    // write (EX_LetBool), not a UFunction call — SetInputMode_UIOnlyEx only
    // changes input ROUTING, it does not make the cursor visible on its own.
    // Live-confirmed missing: first test with just the two calls above left
    // the user "didn't have access to the mouse". Using this codebase's
    // already-proven GetValuePtrByPropertyNameInChain pattern (used
    // hundreds of times elsewhere in this file for bool props like
    // IsCrouching/IsADS) rather than resolving FProperty/UStruct::
    // FindProperty by address — no need for a new low-level mechanism when
    // an existing safe one already does this.
    if (auto* cursorSlot = static_cast<uint8_t*>(ctrl->GetValuePtrByPropertyNameInChain(L"bShowMouseCursor"))) {
        *cursorSlot = 1;
        debug_log("open_barber_menu: bShowMouseCursor set");
    } else {
        debug_log("open_barber_menu: bShowMouseCursor property not found");
    }

    // Reposition — same decode, offsets 0x0167-0x021e: the real flow snaps
    // the interacting player's CapsuleComponent to a fixed "Debug" scene
    // component's world transform on the barber actor, via
    // SceneComponent::K2_GetComponentLocation/K2_GetComponentRotation on
    // "Debug" then SceneComponent::K2_SetWorldLocationAndRotation on the
    // player's "CapsuleComponent" (bSweep=False, bTeleport=False — 5-param
    // signature matches the vendored AActor::K2_SetActorLocationAndRotation
    // already used elsewhere in this file, just called via raw
    // ProcessEvent here since it's a SceneComponent-level call, not an
    // AActor-level one). Live-confirmed needed: without this, triggering
    // the flag from far away leaves the character too far from the barber
    // to see anything in the preview.
    auto** debugSlot = static_cast<UObject**>(barberActor->GetValuePtrByPropertyNameInChain(L"Debug"));
    UObject* debugComp = debugSlot ? *debugSlot : nullptr;
    auto** capsuleSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(L"CapsuleComponent"));
    UObject* capsuleComp = capsuleSlot ? *capsuleSlot : nullptr;
    if (!debugComp || !capsuleComp) {
        debug_log("open_barber_menu: Debug or CapsuleComponent not found, skipping reposition");
        return;
    }
    UFunction* getLocFn = debugComp->GetFunctionByNameInChain(L"K2_GetComponentLocation");
    UFunction* getRotFn = debugComp->GetFunctionByNameInChain(L"K2_GetComponentRotation");
    UFunction* setLocRotFn = capsuleComp->GetFunctionByNameInChain(L"K2_SetWorldLocationAndRotation");
    if (!getLocFn || !getRotFn || !setLocRotFn) {
        debug_log("open_barber_menu: reposition UFunctions not found, skipping");
        return;
    }
    struct GetLocParams { FVector ReturnValue{}; } locParams;
    struct GetRotParams { FRotator ReturnValue{}; } rotParams;
    struct RepositionCtx { UObject* debugComp; UObject* capsuleComp; UFunction* getLocFn; UFunction* getRotFn;
                            UFunction* setLocRotFn; GetLocParams* locParams; GetRotParams* rotParams; };
    RepositionCtx rctx{ debugComp, capsuleComp, getLocFn, getRotFn, setLocRotFn, &locParams, &rotParams };
    const bool repositionOk = seh_invoke([](void* raw) {
        auto* c = static_cast<RepositionCtx*>(raw);
        c->debugComp->ProcessEvent(c->getLocFn, c->locParams);
        c->debugComp->ProcessEvent(c->getRotFn, c->rotParams);
        struct SetLocRotParams {
            FVector    NewLocation;
            FRotator   NewRotation;
            bool       bSweep = false;
            FHitResult SweepHitResult{};
            bool       bTeleport = false;
        } slrParams;
        slrParams.NewLocation = c->locParams->ReturnValue;
        slrParams.NewRotation = c->rotParams->ReturnValue;
        c->capsuleComp->ProcessEvent(c->setLocRotFn, &slrParams);
    }, &rctx);
    debug_log(repositionOk ? "open_barber_menu: reposition to Debug transform called"
                            : "open_barber_menu: reposition crashed, caught via SEH");
}

// Manual on-demand wrapper around do_open_barber_menu() (flag
// `open_barber_menu.flag`) — kept for isolated testing. The real trigger
// is the FirstJoin-driven flow in do_game_tick, which calls
// do_open_barber_menu() directly.
static void check_open_barber_menu_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\open_barber_menu.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());
    do_open_barber_menu();
}

// Mirror-image "give control back" for check_open_barber_menu_trigger —
// needed because that trigger only replicates the game's real *open*
// sequence, not its close/cleanup path (which lives in the widget's own
// bound "Exit" delegate handler, a different part of the Blueprint not yet
// decoded). Live-confirmed necessary 2026-08-17: closing the force-opened
// widget left the camera stuck on the barber actor with no way back to
// gameplay. Every call here is the exact inverse of one made in
// check_open_barber_menu_trigger, same functions/technique, just the
// opposite direction — SetViewTargetWithBlend back to the player's own
// pawn, SetInputMode_GameOnly (ptr=0x41f97b00, seen right next to
// SetInputMode_UIOnlyEx in the same resolved pointer table), and
// bShowMouseCursor=false.
static void do_restore_camera_and_input()
{
    set_ingame_stats_hidden(false);

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("restore_camera: no local pawn"); return; }

    UObject* ctrl = UObjectGlobals::FindFirstOf(STR("BP_PlayerController_C"));
    if (!ctrl) { debug_log("restore_camera: no local PlayerController"); return; }

    UFunction* viewTargetFn = ctrl->GetFunctionByNameInChain(L"SetViewTargetWithBlend");
    if (viewTargetFn) {
        struct SetViewTargetWithBlendParams {
            AActor* NewViewTarget = nullptr;
            float   BlendTime     = 0.0f;
            uint8_t BlendFunc     = 0;
            float   BlendExp      = 0.0f;
            bool    bLockOutgoing = false;
        } vparams;
        vparams.NewViewTarget = pawn;
        vparams.BlendTime     = 0.5f;
        struct ViewCtx { UObject* obj; UFunction* fn; SetViewTargetWithBlendParams* p; };
        ViewCtx vctx{ ctrl, viewTargetFn, &vparams };
        const bool viewOk = seh_invoke([](void* raw) {
            auto* c = static_cast<ViewCtx*>(raw);
            c->obj->ProcessEvent(c->fn, c->p);
        }, &vctx);
        debug_log(viewOk ? "restore_camera: SetViewTargetWithBlend(pawn) called"
                          : "restore_camera: SetViewTargetWithBlend crashed, caught via SEH");
    } else {
        debug_log("restore_camera: SetViewTargetWithBlend UFunction not found");
    }

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* widgetLib = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__WidgetBlueprintLibrary");
    if (!widgetLib) widgetLib = UObjectGlobals::FindFirstOf(L"WidgetBlueprintLibrary");
    if (widgetLib) {
        UFunction* gameOnlyFn = widgetLib->GetFunctionByNameInChain(L"SetInputMode_GameOnly");
        if (gameOnlyFn) {
            // Unlike SetViewTargetWithBlend/SetInputMode_UIOnlyEx above, this
            // params struct is NOT read off a real call site in the decoded
            // Ubergraph — SetInputMode_GameOnly's pointer only appeared in
            // the resolved-pointer table (referenced somewhere else in the
            // game, never actually called in the traced OnExecuteInteract
            // path). Using the standard documented 2-param UE5 signature as
            // a best guess; SEH-wrapped below in case this build adds an
            // extra param the way UIOnlyEx unexpectedly did.
            struct SetInputModeGameOnlyParams {
                UObject* PlayerController  = nullptr;
                bool     bConsumeCaptureMouseDown = true;
            } gparams;
            gparams.PlayerController = ctrl;
            struct GameCtx { UObject* obj; UFunction* fn; SetInputModeGameOnlyParams* p; };
            GameCtx gctx{ widgetLib, gameOnlyFn, &gparams };
            const bool gameOk = seh_invoke([](void* raw) {
                auto* c = static_cast<GameCtx*>(raw);
                c->obj->ProcessEvent(c->fn, c->p);
            }, &gctx);
            debug_log(gameOk ? "restore_camera: SetInputMode_GameOnly called"
                              : "restore_camera: SetInputMode_GameOnly crashed, caught via SEH");
        } else {
            debug_log("restore_camera: SetInputMode_GameOnly UFunction not found");
        }
    } else {
        debug_log("restore_camera: WidgetBlueprintLibrary CDO not found");
    }

    if (auto* cursorSlot = static_cast<uint8_t*>(ctrl->GetValuePtrByPropertyNameInChain(L"bShowMouseCursor"))) {
        *cursorSlot = 0;
        debug_log("restore_camera: bShowMouseCursor cleared");
    } else {
        debug_log("restore_camera: bShowMouseCursor property not found");
    }
}

// Isolated, on-demand test of UBFL_SaveGames_C::"Reset Player Stats"
// (CXXHeaderDump/BFL_SaveGames.hpp — the literal display name has spaces,
// a real UE-allowed Blueprint function name, not a header-dump artifact;
// passed verbatim to GetFunctionByNameInChain) — the game's own built-in
// function for resetting a player's save-backed stats, found 2026-08-17
// while looking for a way to give first-join players a genuinely fresh
// local save state (our server-side FirstJoin signal only affects this
// mod's own synced player_progress row, not the game's own local
// EasyMultiSave-backed save file). High blast-radius if the params
// struct guess is wrong (this resets real save-backed player state,
// same risk class as the corruption incident earlier this session) — the
// user backed up the live local save slot (test1/) before running this,
// same discipline as that incident's lesson. Flag `reset_player_stats.flag`.
static void check_reset_player_stats_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\reset_player_stats.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("reset_player_stats: no local pawn"); return; }

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* saveLib = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__BFL_SaveGames_C");
    if (!saveLib) saveLib = UObjectGlobals::FindFirstOf(L"BFL_SaveGames_C");
    if (!saveLib) { debug_log("reset_player_stats: BFL_SaveGames_C CDO not found"); return; }

    UFunction* resetFn = saveLib->GetFunctionByNameInChain(L"Reset Player Stats");
    if (!resetFn) { debug_log("reset_player_stats: 'Reset Player Stats' UFunction not found"); return; }

    struct ResetParams { AActor* Player = nullptr; UObject* WorldContext = nullptr; } params;
    params.Player       = pawn;
    params.WorldContext  = pawn;
    struct Ctx { UObject* obj; UFunction* fn; ResetParams* p; };
    Ctx ctx{ saveLib, resetFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);
    debug_log(ok ? "reset_player_stats: 'Reset Player Stats' called"
                  : "reset_player_stats: crashed, caught via SEH");
}

// Isolated on-demand vitals maxing (2026-08-18) — writes health/hunger/
// thirst to full via the exact same proven raw-offset mechanism as
// do_vitals_restore below (MedicalComponent@0x7D0+0xD0 health,
// HungerThirstComponent@0x7F8+0xC8/0xD8 hunger/thirst — offsets confirmed
// live via read_local_progress()'s own reads of these same fields every
// profile-revision send). 100.0 matches state.hpp's own health=100.0f
// default/full-health convention; hunger/thirst use the same 0-100 scale.
// Flag `max_vitals.flag`.
static void check_max_vitals_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\max_vitals.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("max_vitals: no local pawn"); return; }

    const auto base = reinterpret_cast<uintptr_t>(pawn);
    if (const auto med = *reinterpret_cast<uintptr_t*>(base + 0x7D0))
        *reinterpret_cast<double*>(med + 0xD0) = 100.0;
    if (const auto ht = *reinterpret_cast<uintptr_t*>(base + 0x7F8)) {
        *reinterpret_cast<double*>(ht + 0xC8) = 100.0;
        *reinterpret_cast<double*>(ht + 0xD8) = 100.0;
    }
    debug_log("max_vitals: health/hunger/thirst set to 100");
}

// Raw FName(const wchar_t*, EFindName, void*) constructor, resolved by
// address — same by-address GetProcAddress workaround as get_class_private
// elsewhere in this file. This codebase's own RC::Unreal::FName wrapper
// (Core.hpp) only exposes a from-INDEX constructor, useless here since raw
// ComparisonIndex values are per-process/session-unstable (see
// project_sdo_save_slot_research memory and feedback_sdo_gameplaytag_ci_
// unstable). Writes into an 8-byte {ComparisonIndex, Number} buffer — the
// same hand-rolled layout already used elsewhere in this file (resolve_fname,
// GameplayTag reads) instead of trusting RC::Unreal::FName's own declared
// (member-less, so untrustworthy for sizeof purposes) C++ type directly.
// EFindName's real values: Find=0, Add=1 — passing Add so a not-yet-interned
// name still resolves instead of silently becoming NAME_None.
static bool construct_fname_from_string(const wchar_t* str, void* out8ByteBuf)
{
    using CtorFn = void(__fastcall*)(void* thisFName, const wchar_t* name, uint32_t findType, void* unused);
    static CtorFn ctor = nullptr;
    if (!ctor) {
        HMODULE ue4ss = GetModuleHandleW(L"UE4SS.dll");
        if (!ue4ss) return false;
        ctor = reinterpret_cast<CtorFn>(GetProcAddress(ue4ss,
            "??0FName@Unreal@RC@@QEAA@PEB_WW4EFindName@12@PEAX@Z"));
        if (!ctor) return false;
    }
    ctor(out8ByteBuf, str, /*EFindName::Add=*/1, nullptr);
    return true;
}

// Isolated, on-demand test of SaveMenu_C::LoadGameFromSlot(bool PauseMenu,
// FName Level) — the real call the game's own "Continue" -> pick slot ->
// "Load Game" -> "Yes" flow makes, fully decoded 2026-08-17 (see
// project_sdo_save_slot_research memory for the whole traced sequence).
// HIGH RISK: this destroys every live BP_PlayerCharacter_C/main-menu actor
// and triggers a full PersistentLevel_C::Event_LoadLevel reload — ONLY
// safe to test at the actual main menu (no live SDO session to break),
// never mid-session. "LongdownValley" is this game's real, confirmed
// persistent level name (read directly out of the decoded bytecode, not
// guessed). Flag `load_game_from_slot.flag`.
static void check_load_game_from_slot_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\load_game_from_slot.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* saveMenu = UObjectGlobals::FindFirstOf(STR("SaveMenu_C"));
    if (!saveMenu) saveMenu = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__SaveMenu_C");
    if (!saveMenu) { debug_log("load_game_from_slot: SaveMenu_C instance/CDO not found"); return; }

    UFunction* loadFn = saveMenu->GetFunctionByNameInChain(L"LoadGameFromSlot");
    if (!loadFn) { debug_log("load_game_from_slot: LoadGameFromSlot UFunction not found"); return; }

    uint8_t levelNameBuf[8] = {};
    if (!construct_fname_from_string(L"LongdownValley", levelNameBuf)) {
        debug_log("load_game_from_slot: could not resolve raw FName constructor");
        return;
    }

    struct LoadParams {
        bool     PauseMenu = false;
        uint32_t LevelComparisonIndex = 0;
        uint32_t LevelNumber = 0;
    } params;
    params.PauseMenu = false; // main-menu context, matching where this trigger is meant to run
    std::memcpy(&params.LevelComparisonIndex, levelNameBuf, 8);

    struct Ctx { UObject* obj; UFunction* fn; LoadParams* p; };
    Ctx ctx{ saveMenu, loadFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);
    debug_log(ok ? "load_game_from_slot: LoadGameFromSlot called"
                  : "load_game_from_slot: crashed, caught via SEH");
}

// Isolated, on-demand test of SaveMenu_C::SetCurrentSaveSlotByName(FString
// SaveGameName) — decoded 2026-08-17 as one of the "light, safe" SaveMenu_C
// functions (no actor destruction, no level reload, unlike LoadGameFromSlot
// which needs a UI-selected slot object that a direct call like this can't
// provide — see project_sdo_save_slot_research memory). Uses a deliberately
// test-only slot name (never "test1" or any real slot) so this can't
// clobber real save data even in the worst case. Flag
// `set_current_save_slot.flag`.
static void check_set_current_save_slot_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\set_current_save_slot.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* saveMenu = UObjectGlobals::FindFirstOf(STR("SaveMenu_C"));
    if (!saveMenu) saveMenu = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__SaveMenu_C");
    if (!saveMenu) { debug_log("set_current_save_slot: SaveMenu_C instance/CDO not found"); return; }
    {
        char line[128];
        snprintf(line, sizeof(line), "set_current_save_slot: using SaveMenu_C obj=0x%llx",
                 reinterpret_cast<unsigned long long>(saveMenu));
        debug_log(line);
    }

    UFunction* setFn = saveMenu->GetFunctionByNameInChain(L"SetCurrentSaveSlotByName");
    if (!setFn) { debug_log("set_current_save_slot: SetCurrentSaveSlotByName UFunction not found"); return; }

    static const wchar_t kSlotName[] = L"SDOTestSlot_DoNotUse";
    struct SetParams { native::UnrealFString SaveGameName{}; } params;
    params.SaveGameName = native::UnrealFString{ const_cast<wchar_t*>(kSlotName),
                                                  static_cast<int32_t>(std::size(kSlotName)),
                                                  static_cast<int32_t>(std::size(kSlotName)) };

    struct Ctx { UObject* obj; UFunction* fn; SetParams* p; };
    Ctx ctx{ saveMenu, setFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);
    debug_log(ok ? "set_current_save_slot: SetCurrentSaveSlotByName called"
                  : "set_current_save_slot: crashed, caught via SEH");
}

// Isolated, on-demand read of UEMSObject::CurrentSaveGameName — verifies
// whether check_set_current_save_slot_trigger's call actually took effect
// (that call has no visible on-screen effect on its own, so this is the
// only way to confirm it without also risking an actual save/load).
// UEMSObject is a UGameInstanceSubsystem (EasyMultiSave.hpp), so it should
// always exist once the game instance exists, main menu or not. Read-only,
// no side effects. Flag `read_current_save_slot.flag`.
static void check_read_current_save_slot_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\read_current_save_slot.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* ems = UObjectGlobals::FindFirstOf(STR("EMSObject"));
    bool viaCdo = false;
    if (!ems) { ems = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__EMSObject"); viaCdo = true; }
    if (!ems) { debug_log("read_current_save_slot: EMSObject instance/CDO not found"); return; }
    {
        char line[160];
        snprintf(line, sizeof(line), "read_current_save_slot: using EMSObject obj=0x%llx viaCdo=%d",
                 reinterpret_cast<unsigned long long>(ems), viaCdo ? 1 : 0);
        debug_log(line);
    }

    void* slot = ems->GetValuePtrByPropertyNameInChain(L"CurrentSaveGameName");
    if (!slot) { debug_log("read_current_save_slot: CurrentSaveGameName property not found"); return; }

    const std::string name = native::read_fstring_field(reinterpret_cast<uintptr_t>(slot));
    debug_log("read_current_save_slot: CurrentSaveGameName=\"" + name + "\"");
}

// Isolated, on-demand test of EMSFunctionLibrary::SetCurrentSaveGameName
// (UObject* WorldContextObject, FString SaveGameName) DIRECTLY — bypassing
// SaveMenu_C::SetCurrentSaveSlotByName entirely, and critically passing the
// local PAWN (guaranteed to have a fully-initialized World/GameInstance,
// since it's actively connected and syncing through this whole session) as
// WorldContextObject instead of a UUserWidget. Tests a specific hypothesis
// from IDA decompilation of the real native implementation (2026-08-17,
// see project_sdo_save_slot_research memory): the earlier no-op may be
// because UEngine::GetWorldFromContextObject()'s World lacked a linked
// OwningGameInstance at the bare main menu, not because the widget itself
// was a bad context object. Same deliberately-fake test slot name as
// check_set_current_save_slot_trigger, for the same reason (can't clobber
// real save data even if this behaves unexpectedly). Flag
// `set_save_name_direct.flag`.
static void check_set_save_name_direct_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\set_save_name_direct.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    // Flag file's single line is the target slot name — defaults to the
    // test-only name if the file is empty, but lets a real name (e.g.
    // "test1", to restore after a test) be passed without a rebuild.
    std::wstring slotName = L"SDOTestSlot_DoNotUse2";
    {
        std::ifstream in(flag, std::ios::binary);
        std::string lineU8;
        std::getline(in, lineU8);
        if (!lineU8.empty() && lineU8.back() == '\r') lineU8.pop_back();
        if (!lineU8.empty()) {
            int wn = MultiByteToWideChar(CP_UTF8, 0, lineU8.data(), (int)lineU8.size(), nullptr, 0);
            std::wstring w(wn, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, lineU8.data(), (int)lineU8.size(), w.data(), wn);
            slotName = w;
        }
    }
    DeleteFileW(flag.c_str());

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("set_save_name_direct: no local pawn"); return; }

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* saveLib = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__EMSFunctionLibrary");
    if (!saveLib) saveLib = UObjectGlobals::FindFirstOf(L"EMSFunctionLibrary");
    if (!saveLib) { debug_log("set_save_name_direct: EMSFunctionLibrary CDO not found"); return; }

    UFunction* setFn = saveLib->GetFunctionByNameInChain(L"SetCurrentSaveGameName");
    if (!setFn) { debug_log("set_save_name_direct: SetCurrentSaveGameName UFunction not found"); return; }

    struct SetParams {
        UObject* WorldContextObject = nullptr;
        native::UnrealFString SaveGameName{};
    } params;
    params.WorldContextObject = pawn;
    params.SaveGameName = native::UnrealFString{ const_cast<wchar_t*>(slotName.c_str()),
                                                  static_cast<int32_t>(slotName.size() + 1),
                                                  static_cast<int32_t>(slotName.size() + 1) };

    struct Ctx { UObject* obj; UFunction* fn; SetParams* p; };
    Ctx ctx{ saveLib, setFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);
    debug_log(ok ? "set_save_name_direct: SetCurrentSaveGameName(pawn, ...) called"
                  : "set_save_name_direct: crashed, caught via SEH");
}

// Manual on-demand test wrapper around do_restore_camera_and_input() — kept
// for testing the restore path in isolation (flag `restore_camera.flag`).
// The automatic path is check_barber_exit_hook() below, which calls the
// same shared function when the real Exit button fires.
static void check_restore_camera_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\restore_camera.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());
    do_restore_camera_and_input();
}

// Flag-triggered, one-shot dump of every live BP_GameModeStart_C (a
// Blueprint subclass of the native APlayerStart, per CXXHeaderDump/
// BP_GameModeStart.hpp) instance's exact world location/rotation — for
// picking real spawn-point coordinates (2026-08-17 "spawn new characters
// at the barber, then teleport to one of the few usual spawn points"
// feature) instead of relying on the user's own in-game position readouts
// (which lack Z height and yaw). Read-only, no side effects.
static void check_dump_playerstarts_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\dump_playerstarts.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    // Flag file's single line is the target class name — defaults to
    // BP_GameModeStart_C (the confirmed APlayerStart subclass) if the file
    // is empty, but lets other candidate names be tried (e.g. native
    // "PlayerStart") without a rebuild if that one turns out to have no
    // live instances.
    std::ifstream in(flag, std::ios::binary);
    std::string classNameU8;
    std::getline(in, classNameU8);
    in.close();
    DeleteFileW(flag.c_str());
    if (!classNameU8.empty() && classNameU8.back() == '\r') classNameU8.pop_back();
    if (classNameU8.empty()) classNameU8 = "BP_GameModeStart_C";
    std::wstring className(classNameU8.size(), L'\0');
    int wn = MultiByteToWideChar(CP_UTF8, 0, classNameU8.data(), (int)classNameU8.size(),
                                  className.data(), (int)className.size());
    className.resize(wn);

    std::vector<UObject*> starts;
    UObjectGlobals::FindAllOf(className.c_str(), starts);
    char line[192];
    snprintf(line, sizeof(line), "dump_playerstarts: class=%s found %zu instances", classNameU8.c_str(), starts.size());
    debug_log(line);
    for (UObject* obj : starts) {
        auto* actor = static_cast<AActor*>(obj);
        const FVector loc = actor->K2_GetActorLocation();
        const FRotator rot = actor->K2_GetActorRotation();
        snprintf(line, sizeof(line),
                 "dump_playerstarts: obj=0x%llx x=%.2f y=%.2f z=%.2f yaw=%.2f",
                 reinterpret_cast<unsigned long long>(obj), loc.X, loc.Y, loc.Z, rot.Yaw);
        debug_log(line);
    }
}

// Every live native PlayerStart actor's world transform, captured 2026-08-17
// via check_dump_playerstarts_trigger — these are level-placed static
// actors, so their coordinates are stable across relaunches (unlike the raw
// obj= pointers they were read through, which are per-process ASLR values
// and NOT reused here). Feeds the "spawn new characters at the barber for
// customization, then teleport to one of the usual spawn points" flow —
// used instead of the user's own manually-read map-UI coordinates, which
// turned out to be in a different (scaled/normalized) coordinate space and
// couldn't be reconciled with real world units.
struct SpawnPoint { double x, y, z, yaw; };
static constexpr SpawnPoint kUsualSpawnPoints[] = {
    { 257427.95, 245820.70, 1650.94,   10.00 },
    { 273903.74, 279206.30, 2078.98, -153.99 },
    { 184439.63, 267062.48, 1732.44,  125.11 },
    { 151933.58, 230553.50, 1313.34,  167.74 },
    {  54902.54, 278716.41, 1444.52, -103.36 },
    { 234891.91, 138653.78, 1352.76,  116.24 },
    { 188645.34, 130775.63, 2501.72,   66.56 },
    { 113230.00, 281024.12, 1252.45, -144.26 },
    { 219251.42, 266931.36, 2010.98, -160.00 },
    {  15733.38, 170826.57, 1489.64, -143.55 },
    { 260101.94,  76268.03, 2131.79,  140.00 },
    {   3451.35, 200182.76, 1615.82,  -75.00 },
    { 256846.14,  39483.45, 1962.60,  -20.08 },
    { -78721.44, 264461.83, 1226.52,  110.00 },
    {  68420.02, 143617.43, 1755.76, -129.98 },
    { 196819.94,  43495.80, 1691.26,  110.00 },
    { 119786.34,  84128.31, 1299.46, -129.98 },
    { 258124.84, 284405.41, 1598.61, -113.89 },
    { 153825.05, 124277.10, 1711.31,  -25.00 },
    { 165450.46,  49718.81, 1514.18,  171.79 },
    { 109235.30, 255088.40, 2630.89,  -85.00 },
    { 192482.81,  45902.18, 1602.25, -103.32 },
};

static bool teleport_to_random_spawn_point(AActor* pawn)
{
    static std::mt19937 rng{ static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()) };
    std::uniform_int_distribution<size_t> dist(0, std::size(kUsualSpawnPoints) - 1);
    const SpawnPoint& sp = kUsualSpawnPoints[dist(rng)];

    struct Ctx { AActor* pawn; const SpawnPoint* sp; bool ok = false; };
    Ctx ctx{ pawn, &sp };
    const bool called = seh_invoke([](void* raw) {
        auto* c = static_cast<Ctx*>(raw);
        FHitResult hit{};
        c->ok = c->pawn->K2_SetActorLocationAndRotation(
            FVector{ c->sp->x, c->sp->y, c->sp->z },
            FRotator{ 0.0, c->sp->yaw, 0.0 },
            false, hit, true);
    }, &ctx);

    char line[192];
    snprintf(line, sizeof(line),
             "teleport_to_random_spawn_point: target x=%.2f y=%.2f z=%.2f yaw=%.2f called=%d ok=%d",
             sp.x, sp.y, sp.z, sp.yaw, called, ctx.ok);
    debug_log(line);
    return called && ctx.ok;
}

// Isolated on-demand test — flag `teleport_random_spawn.flag` — before
// wiring this into the real first-join flow, per this session's established
// discipline of proving each new risky operation alone first.
static void check_teleport_random_spawn_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\teleport_random_spawn.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;
    DeleteFileW(flag.c_str());

    AActor* pawn = find_local_pawn();
    if (!pawn) { debug_log("teleport_random_spawn: no local pawn"); return; }
    teleport_to_random_spawn_point(pawn);
}

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
        // Fall back to the class's own CDO if no live instance exists —
        // works for any class whose asset is loaded at all (its CDO is
        // created the moment the class loads, independent of whether
        // anything has ever spawned an instance), same technique already
        // proven for CharacterBarberMenu_C in check_open_barber_menu_
        // trigger. Bytecode itself lives on the UFunction, shared by CDO
        // and every instance alike, so which one this resolves to doesn't
        // matter for dumping purposes.
        if (!widget) {
            UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
            widget = UObjectGlobals::FindObject(nullptr, kAnyPackage, (L"Default__" + className).c_str());
        }
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
            // The 2s post-spawn grace period used elsewhere for proxy writes
            // (on_process_event_post's own comment) was tested disabled here
            // starting 2026-08-13, since this is a read-only diagnostic, not
            // a write — 4 days of live sessions with no incident confirmed it
            // isn't needed for this specific call. Kept off permanently.
            if (player.proxyActor) {
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

static void dump_recent_calls(); // real definition further down — see its own comment
static void set_item_recent_calls_watch(const std::vector<uintptr_t>& children); // real definition further down, next to dump_recent_calls' ring buffer

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
    // 2026-08-16: RE-ENABLED. Turned off earlier tonight because it wasn't
    // correlating with the "still-attached-but-visually-frozen" reports
    // being chased at the time — but a fresh, unambiguous live event (real
    // pickup items — helmet/rifle/backpack/hatchet — resting on the ground
    // with physics, on both PC1's own gear and proxy0's) is exactly the
    // genuine-detachment class this scan was built to catch (see this
    // function's own top comment), and there was zero diagnostic trail for
    // it with this off. Two changes vs. the original version: (1) name
    // resolution now runs through seh_invoke — the original bare
    // GetFullName() call on a pointer that had just left AttachChildren was
    // flagged as a real stale-pointer risk (SEH catches instead of crashing
    // if the actor was actually destroyed, not just detached, in the
    // interim); (2) dump_recent_calls() fires on every hit, same as this
    // file's "MESH ASSET CLEARED" handling — the point is capturing the
    // ProcessEvent call that immediately preceded the detach, not just
    // logging that it happened.
    struct ResolveNameCtx { UObject* obj; std::string result; };
    for (uintptr_t p : prev) {
        if (std::find(current.begin(), current.end(), p) != current.end()) continue;
        ResolveNameCtx rctx{ reinterpret_cast<UObject*>(p), "<unresolved>" };
        if (!seh_invoke([](void* raw) {
                auto* c = static_cast<ResolveNameCtx*>(raw);
                std::wstring wname = c->obj->GetFullName();
                const int needed = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
                std::string name(needed > 0 ? static_cast<size_t>(needed - 1) : 0, '\0');
                if (needed > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), needed, nullptr, nullptr);
                c->result = std::move(name);
            }, &rctx)) {
            rctx.result = "<crashed resolving name, pointer likely freed>";
        }
        debug_log("attach_health: " + ctx->label + " DETACHED child ptr=0x" +
                  std::to_string(p) + " name=" + rctx.result);
        dump_recent_calls();
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
        // 2026-08-16: widened 1s -> 20s, and the re-snap below made
        // unconditional (not just on detected offset) in the same change —
        // see the unconditional-re-snap block's own comment. All items
        // under one label share this single gate, so at 1s this could burst
        // up to ~16 ProcessEvent K2_AttachToComponent calls back-to-back in
        // a single scan pass once a second; both PC1 and PC2 hung shortly
        // after this (and the same-shaped leader-pose change) went live.
        // 20s trades slower self-heal for far less frequent risk exposure.
        const bool runAbsoluteThisPass = (nowUs - lastAbsUs >= 20'000'000ULL);
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
            const double sDist = std::sqrt(sdx * sdx + sdy * sdy + sdz * sdz);
            constexpr double kSocketDistSq = 30.0 * 30.0;
            const bool offSocket = sdx * sdx + sdy * sdy + sdz * sdz > kSocketDistSq;
            // 2026-08-16: live-reported items (mag/laser/shotgun) visibly
            // detached/floating in the world on PC1, but AttachChildren's
            // own count never moved (heartbeat steady at 16 the whole time)
            // and this exact check produced zero "OFF-SOCKET" hits either —
            // meaning K2_GetComponentLocation here is reading the SAME
            // small/correct distance our own reflection call sees, while
            // whatever's actually rendered on screen disagrees. Both
            // K2_GetComponentLocation (game-thread ComponentToWorld cache)
            // and the visible render (render-thread's own copy of that
            // transform) are SUPPOSED to always agree — UE re-syncs the
            // render proxy any time the game-thread transform changes via
            // MarkRenderTransformDirty, normally triggered automatically by
            // a real attach/move call. If that notification silently
            // doesn't fire for some re-attach paths, the two would disagree
            // exactly like this: our own reads (and the engine's own
            // internal bookkeeping) are fine, only the pixels are stuck.
            // Logged unconditionally (not just on threshold-exceeded) so
            // this theory is directly checkable against the next live
            // report instead of guessed at.
            char sdbuf[160];
            snprintf(sdbuf, sizeof(sdbuf), "attach_health: %s child ptr=0x%llx socket-dist=%.1f offSocket=%d",
                      ctx->label.c_str(), static_cast<unsigned long long>(child), sDist, offSocket ? 1 : 0);
            debug_log(sdbuf);
            // 2026-08-16: tried re-snapping UNCONDITIONALLY on this throttled
            // pass (not just when offSocket) to test the render-thread-
            // notification-gap theory above — the very next live test
            // crashed (0xe06d7363, unhandled C++ exception, stack entirely
            // inside this mod's module via the WndProc path, while idle; no
            // dump generated). Combined with the sibling leader-pose
            // proactive-refresh change hanging the session right before
            // this, that's two different failure modes from "make a
            // previously-reactive-only repair call fire unconditionally on
            // a timer" across two consecutive tests. Reverted the
            // "unconditional" part permanently — this block stays strictly
            // reactive (offSocket-gated only, same throttle as before).
            //
            // 2026-08-16, later: REPLACED the repair action itself, not just
            // its frequency. Full bytecode decode of
            // BP_JigPickupComponent_C::ValidateAttachedActor (research/
            // 04_ida_investigation_log.md) found this is exactly what the
            // real game does to fix a misplaced-but-still-attached item: NOT
            // a re-attach (K2_AttachToComponent, what this block used to
            // call — only re-confirms the parent-child relationship, which
            // was never actually broken; AttachChildren's own count stays
            // steady through every reported occurrence of this bug) but a
            // direct reposition — read the parent socket's CURRENT transform
            // and K2_SetActorTransform the item onto it with bTeleport=true.
            // SetActorTransform is UE's general "move any actor" API, which
            // drives the same render-transform-dirty path any actor move
            // goes through — a plausible way to force the render refresh
            // this whole investigation has been chasing, that a bare
            // re-attach apparently doesn't. Skips ValidateAttachedActor's own
            // parent-rediscovery/Character-cast steps — this scan already
            // knows ctx->mesh is the confirmed parent (that's what's being
            // walked), so re-deriving and re-checking it here would be
            // redundant, not safer.
            if (offSocket) {
                UFunction* getOwnerFn = childObj->GetFunctionByNameInChain(L"GetOwner");
                UFunction* getSocketTransformFn = ctx->mesh->GetFunctionByNameInChain(L"GetSocketTransform");
                struct OwnerParams { AActor* ReturnValue = nullptr; } ownerParams;
                if (getOwnerFn) childObj->ProcessEvent(getOwnerFn, &ownerParams);

                UFunction* setActorTransformFn = ownerParams.ReturnValue
                    ? ownerParams.ReturnValue->GetFunctionByNameInChain(L"K2_SetActorTransform")
                    : nullptr;

                if (getOwnerFn && ownerParams.ReturnValue && getSocketTransformFn && setActorTransformFn) {
                    // Same proven 96-byte Kismet FTransform marshaling shape
                    // used throughout this project (rotation quat, location+
                    // pad, scale+pad, all doubles) — see FreezeCheckNativeFTransform's
                    // own comment further down this file for the precedent.
                    struct SocketTransformNative {
                        double rotX = 0.0, rotY = 0.0, rotZ = 0.0, rotW = 1.0;
                        double locX = 0.0, locY = 0.0, locZ = 0.0, locPad = 0.0;
                        double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0, scalePad = 0.0;
                    };
                    struct SocketTransformParams {
                        RawFGameplayTag InSocketName{};
                        uint8_t RelativeSpace = 0; // ERelativeTransformSpace::RTS_World, matches the decoded ByteConst 0
                        SocketTransformNative ReturnValue{};
                    } stParams;
                    stParams.InSocketName = socket;
                    ctx->mesh->ProcessEvent(getSocketTransformFn, &stParams);

                    struct SetTransformParams {
                        SocketTransformNative NewTransform{};
                        bool       bSweep = false;
                        FHitResult SweepHitResult{};
                        bool       bTeleport = true;
                        bool       ReturnValue = false;
                    } setParams;
                    setParams.NewTransform = stParams.ReturnValue;
                    ownerParams.ReturnValue->ProcessEvent(setActorTransformFn, &setParams);

                    debug_log("attach_health: " + ctx->label + " transform-snapped OFF-SOCKET child ptr=0x" +
                              std::to_string(child) + " was " + std::to_string(sDist) +
                              " units from its own socket, K2_SetActorTransform returned " +
                              std::to_string(setParams.ReturnValue));
                } else {
                    debug_log("attach_health: " + ctx->label + " OFF-SOCKET child ptr=0x" +
                              std::to_string(child) +
                              " but GetOwner/GetSocketTransform/K2_SetActorTransform not found, skipped repair");
                }
            }
        }
    }
}

// 2026-08-16: targeted diagnostic for the item-detach investigation.
// CORRECTED same night, later: a full bytecode decode of
// CheckDistanceFromActor (research/04_ida_investigation_log.md,
// "CheckDistanceFromActor — fully decoded, and it CORRECTS an earlier
// assumption") proved this original comment wrong. CheckDistanceFromActor
// does NOT "auto-release" an item — past 300 units it only clears its own
// repeating timer, calls OnInteractActorOverDistance on CurrentActor (the
// interacting CHARACTER, cast to BP_JigCharacterInterface — not the item)
// to close its own interaction prompt, then nulls its own CurrentActor
// tracking variable. Nothing in that path touches attachment, socket, or
// physics state. CurrentActor/InteractingActorLoc (comp+0xE0/+0xE8) track
// the loot-interaction-prompt lifecycle (walk away from an item you were
// about to interact with → its prompt cancels), not an equipped item's
// ongoing physical state — ruled out as a lead for the render/attachment
// desync bug this diagnostic was chasing. Left running (harmless,
// informational) but treat any findings here as unrelated to that bug.
// Forward-declared — real definition lives later in the file, needed here
// for check_current_actor_diagnostic's "local" scan target.
static AActor* cached_find_local_pawn();

struct CurrentActorFinding { uintptr_t pickupComp; void* currentActor; uintptr_t childPtr; };
struct CurrentActorScanCtx { AActor* actor; std::vector<CurrentActorFinding>* findings; };

static void do_current_actor_scan(void* rawCtx)
{
    auto* ctx = static_cast<CurrentActorScanCtx*>(rawCtx);
    auto** meshSlot = static_cast<UObject**>(ctx->actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return;

    const uintptr_t meshAddr = reinterpret_cast<uintptr_t>(mesh);
    const uintptr_t childrenData  = *reinterpret_cast<uintptr_t*>(meshAddr + 0x00C0);
    const int32_t   childrenCount = *reinterpret_cast<int32_t*>(meshAddr + 0x00C0 + 0x08);
    if (childrenCount < 0 || childrenCount > 64 || !childrenData) return;

    for (int32_t c = 0; c < childrenCount; ++c) {
        uintptr_t child = *reinterpret_cast<uintptr_t*>(childrenData + static_cast<size_t>(c) * 8);
        if (!child) continue;
        UObject* childObj = reinterpret_cast<UObject*>(child);

        auto** pickupSlot = static_cast<UObject**>(childObj->GetValuePtrByPropertyNameInChain(L"BP_JigPickupComponent"));
        UObject* pickupComp = (pickupSlot && *pickupSlot) ? *pickupSlot : nullptr;
        if (!pickupComp) continue;

        const uintptr_t pickupAddr = reinterpret_cast<uintptr_t>(pickupComp);
        void* currentActor = *reinterpret_cast<void**>(pickupAddr + 0xE0);
        ctx->findings->push_back({ pickupAddr, currentActor, child });
    }
}

static void check_current_actor_diagnostic()
{
    static uint64_t s_lastCheckUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastCheckUs < 1'000'000ULL) return;
    s_lastCheckUs = now;

    static std::unordered_map<uintptr_t, bool> s_wasNonNull; // keyed by pickup component ptr

    auto scanOne = [](const char* label, AActor* actor) {
        if (!actor) return;
        std::vector<CurrentActorFinding> findings;
        CurrentActorScanCtx ctx{ actor, &findings };
        if (!seh_invoke(do_current_actor_scan, &ctx)) {
            debug_log(std::string("current_actor_diag: ") + label + " scan crashed, caught via SEH");
            return;
        }
        for (const auto& f : findings) {
            const bool nonNull = f.currentActor != nullptr;
            bool& was = s_wasNonNull[f.pickupComp];
            if (nonNull && !was) {
                char buf[256];
                snprintf(buf, sizeof(buf),
                    "current_actor_diag: %s child=0x%llx pickupComp=0x%llx CurrentActor BECAME NON-NULL = 0x%llx",
                    label, static_cast<unsigned long long>(f.childPtr),
                    static_cast<unsigned long long>(f.pickupComp),
                    reinterpret_cast<unsigned long long>(f.currentActor));
                debug_log(buf);
            } else if (!nonNull && was) {
                char buf[192];
                snprintf(buf, sizeof(buf),
                    "current_actor_diag: %s child=0x%llx pickupComp=0x%llx CurrentActor cleared back to null",
                    label, static_cast<unsigned long long>(f.childPtr),
                    static_cast<unsigned long long>(f.pickupComp));
                debug_log(buf);
            }
            was = nonNull;
        }
    };

    scanOne("local", cached_find_local_pawn());

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    int i = 0;
    for (auto& [id, player] : sdb::g_state().players) {
        if (!player.proxyActor) continue;
        std::string label = "proxy" + std::to_string(i++);
        scanOne(label.c_str(), static_cast<AActor*>(player.proxyActor));
    }
}

// 2026-08-16: pure render/world-position freeze detector — live-reported
// "PC1's helmet froze locally" while read_local_weapon_attachments still
// showed it correctly present in AttachChildren at the exact same moment,
// proving this is a DIFFERENT failure mode than either DETACHED (leaves
// AttachChildren) or DRIFTED/OFF-SOCKET (RelativeLocation moves away from
// where it should be) — both of those only read the DATA MODEL, which this
// report shows can look completely correct while the actual per-frame
// world transform the engine draws simply stops updating. Also reported:
// seems more common with a proxy connected — worth watching whether this
// correlates with proxy sync bursts once live data comes in. Tracks each
// equipped item's own live K2_GetComponentToWorld() position against the
// owning character's K2_GetActorLocation() across consecutive polls — if
// the character moved meaningfully but a specific item barely moved, that's
// the freeze signature, independent of what the attachment data claims.
// Same NativeFTransform layout already proven for this exact call in
// proxy_manager.cpp's spawn_and_equip_item_visual (Kismet FTransform return
// marshaling — rotation quat, then location+pad, then scale+pad, 96 bytes).
struct FreezeCheckNativeFTransform {
    double rotX = 0.0, rotY = 0.0, rotZ = 0.0, rotW = 1.0;
    double locX = 0.0, locY = 0.0, locZ = 0.0, locPad = 0.0;
    double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0, scalePad = 0.0;
};

struct FreezeItemFinding { uintptr_t itemRoot; double x, y, z; };
struct FreezeScanCtx {
    AActor* actor;
    std::vector<FreezeItemFinding>* findings;
    double actorX = 0, actorY = 0, actorZ = 0;
    bool actorLocValid = false;
};

static void do_freeze_check_scan(void* rawCtx)
{
    auto* ctx = static_cast<FreezeScanCtx*>(rawCtx);

    UFunction* getLocFn = ctx->actor->GetFunctionByNameInChain(L"K2_GetActorLocation");
    if (getLocFn) {
        struct { double X = 0, Y = 0, Z = 0; } loc;
        ctx->actor->ProcessEvent(getLocFn, &loc);
        ctx->actorX = loc.X; ctx->actorY = loc.Y; ctx->actorZ = loc.Z;
        ctx->actorLocValid = true;
    }

    auto** meshSlot = static_cast<UObject**>(ctx->actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return;

    const uintptr_t meshAddr = reinterpret_cast<uintptr_t>(mesh);
    const uintptr_t childrenData  = *reinterpret_cast<uintptr_t*>(meshAddr + 0x00C0);
    const int32_t   childrenCount = *reinterpret_cast<int32_t*>(meshAddr + 0x00C0 + 0x08);
    if (childrenCount < 0 || childrenCount > 64 || !childrenData) return;

    for (int32_t c = 0; c < childrenCount; ++c) {
        uintptr_t child = *reinterpret_cast<uintptr_t*>(childrenData + static_cast<size_t>(c) * 8);
        if (!child) continue;
        UObject* childObj = reinterpret_cast<UObject*>(child);

        UFunction* toWorldFn = childObj->GetFunctionByNameInChain(L"K2_GetComponentToWorld");
        if (!toWorldFn) continue;
        struct Params { FreezeCheckNativeFTransform ReturnValue; } wparams;
        childObj->ProcessEvent(toWorldFn, &wparams);

        ctx->findings->push_back({ child, wparams.ReturnValue.locX, wparams.ReturnValue.locY, wparams.ReturnValue.locZ });
    }
}

// 2s poll (not every tick) — this does a K2_GetActorLocation plus one
// K2_GetComponentToWorld ProcessEvent call per attached item (up to ~16),
// so keeping it infrequent matters after tonight's own FindFirstOf-every-
// tick FPS regression lesson, even though this is far cheaper (no
// UObjectGlobals scan, just direct ProcessEvent calls on already-known
// pointers).
static void check_item_freeze_diagnostic()
{
    static uint64_t s_lastCheckUs = 0;
    const uint64_t now = sdb::now_micros();
    if (now - s_lastCheckUs < 2'000'000ULL) return;
    s_lastCheckUs = now;

    struct LastState { double x = 0, y = 0, z = 0; bool valid = false; };
    static std::unordered_map<uintptr_t, LastState> s_lastItemPos;
    static std::unordered_map<std::string, LastState> s_lastActorPos;

    auto scanOne = [](const char* label, AActor* actor) {
        if (!actor) {
            debug_log(std::string("item_freeze_diag: ") + label + " actor is null, skipping");
            return;
        }
        std::vector<FreezeItemFinding> findings;
        FreezeScanCtx ctx{ actor, &findings };
        if (!seh_invoke(do_freeze_check_scan, &ctx)) {
            debug_log(std::string("item_freeze_diag: ") + label + " scan crashed, caught via SEH");
            return;
        }
        if (!ctx.actorLocValid) {
            debug_log(std::string("item_freeze_diag: ") + label + " K2_GetActorLocation failed/not found");
            return;
        }

        LastState& lastActor = s_lastActorPos[label];
        double actorDeltaSq = 0;
        if (lastActor.valid) {
            const double dx = ctx.actorX - lastActor.x, dy = ctx.actorY - lastActor.y, dz = ctx.actorZ - lastActor.z;
            actorDeltaSq = dx * dx + dy * dy + dz * dz;
        }
        const bool actorMoved = lastActor.valid && actorDeltaSq > (50.0 * 50.0);
        lastActor = { ctx.actorX, ctx.actorY, ctx.actorZ, true };

        // 2026-08-16: heartbeat every poll (not just on a FROZEN hit) —
        // live-reported "axe froze" produced zero output at all, need to
        // see whether this is even running/finding items/meeting the
        // movement threshold, rather than guessing again.
        char hb[192];
        snprintf(hb, sizeof(hb),
            "item_freeze_diag: %s heartbeat items=%zu actorMovedThisPoll=%d actorDelta=%.1f",
            label, findings.size(), actorMoved, std::sqrt(actorDeltaSq));
        debug_log(hb);

        for (const auto& f : findings) {
            LastState& lastItem = s_lastItemPos[f.itemRoot];
            if (lastItem.valid && actorMoved) {
                const double dx = f.x - lastItem.x, dy = f.y - lastItem.y, dz = f.z - lastItem.z;
                const double itemDeltaSq = dx * dx + dy * dy + dz * dz;
                char buf[256];
                snprintf(buf, sizeof(buf),
                    "item_freeze_diag: %s item=0x%llx actorMoved=%.1f itemMoved=%.1f%s",
                    label, static_cast<unsigned long long>(f.itemRoot),
                    std::sqrt(actorDeltaSq), std::sqrt(itemDeltaSq),
                    itemDeltaSq < (10.0 * 10.0) ? " <-- FROZEN" : "");
                debug_log(buf);
            }
            lastItem = { f.x, f.y, f.z, true };
        }
    };

    scanOne("local", cached_find_local_pawn());

    std::lock_guard<std::mutex> lk(sdb::g_state().playersMtx);
    int i = 0;
    for (auto& [id, player] : sdb::g_state().players) {
        if (!player.proxyActor) continue;
        std::string label = "proxy" + std::to_string(i++);
        scanOne(label.c_str(), static_cast<AActor*>(player.proxyActor));
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

    // Mirror into the recent-calls forensic watch (see its own comment) —
    // only "local" drives that investigation, same scoping as the clothing
    // watch this sits alongside.
    if (label == "local") set_item_recent_calls_watch(weaponsAndItems);

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
    uint64_t healthySinceUs = 0;   // 2026-08-16 — see do_component_drift_scan's firstSeenUs comment
    uint64_t lastLeaderPoseRefreshUs = 0; // 2026-08-16 — see check_component_drift's proactive-refresh comment
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
// clothingCompOffset: the corresponding Clothing_X overlay component's raw
// offset on the owner (0 = no counterpart, e.g. for the Clothing_X repair
// entries themselves, which ARE the counterpart). See do_body_part_repair's
// own comment for why this is needed: a null base-mesh slot is CORRECT,
// not broken, whenever clothing actually covers that slot.
struct BodyPartRepairCtx { AActor* owner; int32_t ci; const std::string* key; const wchar_t* clothingOnRepName; UObject* comp; uintptr_t clothingCompOffset; };

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
// Forward-declared — real definitions live near on_actor_tick, needed here
// (on_process_event_pre, above both) for the reliability watchdog. See
// register_actor_tick_hook's own comment for why this exists.
static bool register_actor_tick_hook();
// Forward-declared — real definition lives near register_actor_tick_hook,
// needed here (do_game_tick, above it) to make the one-time first
// registration call once a real pawn exists. See its own comment.
static void fixup_and_register_actor_tick_hook(AActor* pawn);
// Forward-declared — real definition lives near register_actor_tick_hook,
// needed here (do_game_tick, above it) to start the WndProc-subclass clean
// tick trigger once a real pawn exists. See its own comment and the
// "Reliable GameThread Trigger via WndProc Subclass" plan.
static void ensure_hwnd_ticker_started();
// Forward-declared — real definition lives right after ensure_hwnd_ticker_
// started, needed here (do_game_tick, above it) to fire the one-time
// a.ParallelAnimEvaluation=0 console command once a real pawn exists. See
// its own comment for the full UE-191796 research writeup.
static void ensure_parallel_anim_eval_disabled(AActor* worldContextActor);
// Forward-declared — real definition lives near do_game_tick, needed here
// (check_attach_health_trigger, above it) now that it's called every
// do_game_tick() invocation instead of only once per 300ms. 100ms-cached
// wrapper around the expensive find_local_pawn() reflection scan — see
// check_attach_health_trigger's own comment for why the raw scan can't be
// used here anymore.
static AActor* cached_find_local_pawn();

static void do_body_part_repair(void* ctxRaw)
{
    auto* ctx = static_cast<BodyPartRepairCtx*>(ctxRaw);

    // 2026-08-15: a null BASE mesh (Torso/Legs/Feet) is the CORRECT state
    // whenever clothing actually covers that slot — the real game hides the
    // bare mesh and shows the Clothing_X overlay instead (confirmed live:
    // bracketed logging showed OnRep_ClothingXEquipped? clearing this exact
    // slot back to null immediately after every repair, which turned out to
    // be that function correctly re-asserting "clothing is on, hide the
    // bare mesh" — not a bug). Treating null as always-broken here is what
    // caused the endless UpdateBodyParts-vs-OnRep fight every ~1.2s all
    // session. Check the actual Clothing_X overlay's own mesh first: if
    // it's set (clothing genuinely equipped), leave this slot alone
    // entirely — nothing to repair. Only proceed to UpdateBodyParts below
    // when clothing is NOT covering this slot, i.e. bare skin should
    // genuinely be showing and isn't.
    if (ctx->clothingCompOffset) {
        auto* clothingComp = *reinterpret_cast<UObject**>(
            reinterpret_cast<uintptr_t>(ctx->owner) + ctx->clothingCompOffset);
        if (clothingComp) {
            void** clothMeshSlot = static_cast<void**>(clothingComp->GetValuePtrByPropertyNameInChain(L"SkeletalMesh"));
            if (!clothMeshSlot) clothMeshSlot = static_cast<void**>(clothingComp->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset"));
            if (clothMeshSlot && *clothMeshSlot) {
                // 2026-08-15: mesh-presence alone isn't sufficient proof the
                // clothing is actually showing — live-reported immediately
                // after the mesh-only version of this check shipped: boots/
                // shirt genuinely invisible (not just bare skin underneath)
                // while this exact skip fired continuously, meaning
                // Clothing_Feet/Torso had a valid mesh reference but was
                // hidden (bVisible false) — likely a stale SetVisibility
                // (false) from some earlier point that never got reversed.
                // Check visibility too: if hidden, that's the real bug —
                // show it — rather than falling through to bare skin, which
                // isn't correct either when clothing is genuinely equipped.
                UFunction* isVisFn = clothingComp->GetFunctionByNameInChain(L"IsVisible");
                bool isVisible = true; // fail open — don't force a SetVisibility call if we can't even check
                if (isVisFn) {
                    struct Params { bool ReturnValue = false; } vparams;
                    clothingComp->ProcessEvent(isVisFn, &vparams);
                    isVisible = vparams.ReturnValue;
                }
                if (!isVisible) {
                    UFunction* setVisFn = clothingComp->GetFunctionByNameInChain(L"SetVisibility");
                    if (setVisFn) {
                        struct Params { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
                        clothingComp->ProcessEvent(setVisFn, &vparams);
                        debug_log("component_drift: " + *ctx->key + " Clothing_X overlay had a mesh but was HIDDEN — re-shown");
                    }
                } else {
                    // 2026-08-15: refresh the leader-pose link routinely here
                    // too, not just right after WE change the mesh — a
                    // live-reported recurrence (shirt/pants still not
                    // showing, this exact "equipped and visible" skip still
                    // firing) proved the desync can happen from something
                    // outside our own repair flow (the game's own initial
                    // equip, most likely), in which case this component
                    // never gets touched by any mesh-setting call site at
                    // all — nothing would ever refresh it. Cheap and
                    // idempotent if the link is already fine; see
                    // refresh_leader_pose's own comment (proxy_manager.cpp).
                    auto** leaderMeshSlot2 = static_cast<UObject**>(ctx->owner->GetValuePtrByPropertyNameInChain(L"Mesh"));
                    sdb::refresh_leader_pose(clothingComp, (leaderMeshSlot2 && *leaderMeshSlot2) ? *leaderMeshSlot2 : nullptr);
                    debug_log("component_drift: " + *ctx->key + " skipped mesh-repair (Clothing_X equipped+visible) but refreshed leader-pose link");
                }
                return;
            }
        }
    }

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
    // 2026-08-15: "meshBefore=0x0 meshImmediatelyAfter=0x0" on every single
    // retry for 5 straight minutes (a live-reported occurrence, female
    // character) is ambiguous with the existing logging alone — meshSlot
    // itself (the property pointer, not its value) being null produces the
    // exact same "0x0" output as UpdateBodyParts genuinely having zero
    // effect on a resolved slot. Log which case this actually is before
    // guessing at a fix.
    void** skelMeshSlot = ctx->comp ? static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMesh")) : nullptr;
    void** skelMeshAssetSlot = ctx->comp ? static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset")) : nullptr;
    void** meshSlot = (skelMeshSlot && *skelMeshSlot) ? skelMeshSlot : (skelMeshAssetSlot ? skelMeshAssetSlot : skelMeshSlot);
    const void* before = meshSlot ? *meshSlot : nullptr;
    debug_log("component_drift: " + *ctx->key + " repair-diag compValid=" + std::to_string(ctx->comp != nullptr) +
              " UpdateBodyPartsFn=" + std::to_string(fn != nullptr) +
              " SkeletalMeshSlot=" + std::to_string(skelMeshSlot != nullptr) +
              " SkeletalMeshAssetSlot=" + std::to_string(skelMeshAssetSlot != nullptr) +
              " chosenSlot=" + std::to_string(meshSlot != nullptr));

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
        // Defensive — UpdateBodyParts is the game's own Blueprint function
        // (not ours to inspect/modify), so whether it internally refreshes
        // the leader-pose bone mapping after changing this component's
        // SkinnedAsset is unconfirmed. Refresh unconditionally after calling
        // it, matching every other direct SetSkinnedAssetAndUpdate call site
        // in this project — see refresh_leader_pose's own comment
        // (proxy_manager.cpp) for the full rationale. Harmless no-op if
        // UpdateBodyParts already handled it correctly.
        auto** leaderMeshSlot = static_cast<UObject**>(ctx->owner->GetValuePtrByPropertyNameInChain(L"Mesh"));
        sdb::refresh_leader_pose(ctx->comp, (leaderMeshSlot && *leaderMeshSlot) ? *leaderMeshSlot : nullptr);
    }

    const void* immediatelyAfter = meshSlot ? *meshSlot : nullptr;
    debug_log("component_drift: " + *ctx->key +
              (fn ? (" called UpdateBodyParts ci=" + std::to_string(ctx->ci)) : std::string(" skipped UpdateBodyParts (clothing-only repair)")) +
              " meshBefore=0x" + std::to_string(reinterpret_cast<uintptr_t>(before)) +
              " meshImmediatelyAfter=0x" + std::to_string(reinterpret_cast<uintptr_t>(immediatelyAfter)));

    // 2026-08-15: PROVEN live (not theory) — bracketed read confirmed
    // calling this OnRep from the BASE Torso/Legs/Feet repair (ci != 0)
    // clears the mesh it was just set to, every single time
    // (meshBeforeOnRep=<valid> meshAfterOnRep=0x0), explaining the
    // "reverts within ~1.2s, forever" pattern observed all session. The
    // earlier theory ("OnRep_ClothingLegsEquipped? only touches the
    // Clothing_Legs overlay, not the bare Legs mesh") was wrong — its real
    // logic toggles between showing the clothing overlay and showing the
    // bare mesh based on the actual equip state, so calling it from the
    // BASE mesh's own repair fights whatever state it just decided (if
    // clothing is genuinely equipped, a null base mesh is the CORRECT
    // state — our repair had been endlessly re-setting it, and this
    // function endlessly re-clearing it back, an unbounded and entirely
    // pointless fight against the game's own intended behavior). Restricted
    // to ci == 0 (the Clothing_Torso/Legs/Feet overlay-component repair
    // path), the one place calling it is actually correct — there, a
    // missing Clothing_X mesh while the OWN component's read shows null is
    // exactly the case this function is meant to fix.
    if (ctx->clothingOnRepName && ctx->ci == 0) {
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
    bool isSkeletalComp = meshSlot != nullptr;
    if (!meshSlot) meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"SkeletalMeshAsset"));
    if (meshSlot && !isSkeletalComp) isSkeletalComp = true; // resolved via the SkeletalMeshAsset fallback, still skeletal
    if (!meshSlot) { meshSlot = static_cast<void**>(ctx->comp->GetValuePtrByPropertyNameInChain(L"StaticMesh")); isSkeletalComp = false; }
    if (meshSlot) {
        const bool hasMeshNow = (*meshSlot != nullptr);
        // 2026-08-15: routine leader-pose refresh — every skeletal component
        // tracked here (Torso/Legs/Feet/Arms/head/Hands/Clothing_X, per the
        // confirmed FModel export) is a leader-pose follower. Unlike the
        // mesh/position checks above, a stale leader-pose link doesn't
        // change RelativeLocation OR clear the mesh reference — it only
        // affects which bone transforms actually get used to render, so
        // this is the ONLY place that would ever catch/fix a desync that
        // happened outside our own repair flow (e.g. the game's own initial
        // equip). Cheap and idempotent when already correctly linked; see
        // refresh_leader_pose's own comment (proxy_manager.cpp).
        if (isSkeletalComp && hasMeshNow) {
            auto** leaderMeshSlotRoutine = static_cast<UObject**>(ctx->owner->GetValuePtrByPropertyNameInChain(L"Mesh"));
            sdb::refresh_leader_pose(ctx->comp, (leaderMeshSlotRoutine && *leaderMeshSlotRoutine) ? *leaderMeshSlotRoutine : nullptr);
        }
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
        // 2026-08-16: firstSeenUs was being stamped unconditionally on this
        // ctx's very first scan ever (component healthy or not), not on
        // first-seen-BROKEN as the comment above claims and as the 5-minute
        // ceiling's own name implies. Live-confirmed the bug: at 13:09:45-48
        // essentially every tracked category on BOTH local and proxy0 (most
        // of which were healthy the whole time — e.g. local:Legs was still
        // logging benign "skipped mesh-repair...refreshed leader-pose link"
        // at 13:06:0x) hit "giving up PERMANENTLY" within the same 3-second
        // window — that's "5 minutes since character load", not "5 minutes
        // since broken". Once tripped, repair is disabled for that component
        // for the rest of the session even if it breaks for the first time
        // minutes later (live-reported: local:Legs/"pants" froze ~9 minutes
        // after its ceiling had already silently tripped).
        //
        // First fix attempt (only arm firstSeenUs the first time hasMeshNow
        // reads false, never reset it back) was INCOMPLETE — live-reproduced
        // again immediately after redeploy: 13:32:36-13:33:18 hit the exact
        // same mass-simultaneous "giving up PERMANENTLY" across nearly every
        // local AND proxy0 category. Cause: most components are broken for a
        // moment right at spawn (mesh still streaming in — the very reason
        // the 2s no-repair grace below exists), which arms firstSeenUs once,
        // and since it was never reset, the 5-minute clock still expires on
        // schedule even though the component has been solidly healthy for
        // the entire 5 minutes since. Real fix: track how long the
        // component has been CONTINUOUSLY healthy (healthySinceUs); once
        // that streak is comfortably past the loading-grace window, clear
        // firstSeenUs entirely, so a component that recovered and stayed
        // recovered stops being tracked as "stuck" at all. A component that
        // flaps (our own repair transiently fixes it for ~1s, then it
        // breaks again) never accumulates a long enough healthy streak to
        // clear firstSeenUs, so the 2026-08-15 anti-flapping ceiling still
        // holds for the case it was actually built for.
        if (hasMeshNow) {
            if (ctx->healthySinceUs == 0) ctx->healthySinceUs = nowUs;
            if (ctx->firstSeenUs != 0 && nowUs - ctx->healthySinceUs >= 10'000'000ULL) {
                ctx->firstSeenUs = 0;
                ctx->loggedWallClockGiveUp = false;
            }
        } else {
            ctx->healthySinceUs = 0;
            if (ctx->firstSeenUs == 0) ctx->firstSeenUs = nowUs;
        }
        const bool wallClockExpired = ctx->firstSeenUs != 0 &&
                                       nowUs - ctx->firstSeenUs >= 300'000'000ULL; // 5 min hard ceiling, anchored to first-seen-BROKEN
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
                    // leaderMesh only matters for isSkeletal (Hands) — see
                    // reapply_named_mesh/refresh_leader_pose's own comments.
                    auto** leaderMeshSlot = isSkeletal ? static_cast<UObject**>(ctx->owner->GetValuePtrByPropertyNameInChain(L"Mesh")) : nullptr;
                    UObject* leaderMesh = (leaderMeshSlot && *leaderMeshSlot) ? *leaderMeshSlot : nullptr;
                    const bool applied = name && sdb::reapply_named_mesh(ctx->comp, *name, isSkeletal, leaderMesh);
                    debug_log("component_drift: " + ctx->key + " local appearance repair field=" + f +
                              " cachedName=" + (name ? *name : "<none>") + " applied=" + std::to_string(applied));
                }
            } else {
                // See BodyPartRepairCtx::clothingCompOffset's comment.
                // Offsets match the Clothing_Torso/Legs/Feet entries already
                // used elsewhere in this file (proxy_manager.cpp's
                // sync_equipment clothing-slot switch, this file's own
                // kNames table) — 0 for anything without a base/overlay
                // pair (including the Clothing_X rows themselves, ci==0,
                // which don't need this check at all).
                uintptr_t clothingOffset = 0;
                switch (ctx->bodyPartCi) {
                    case 1732710: clothingOffset = 0x0770; break; // Torso
                    case 1732718: clothingOffset = 0x0768; break; // Legs
                    case 1732721: clothingOffset = 0x0760; break; // Feet
                    default: break;
                }
                BodyPartRepairCtx repairCtx{ ctx->owner, ctx->bodyPartCi, &ctx->key, ctx->clothingOnRepName, ctx->comp, clothingOffset };
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
        // OnRep_ClothingGlovesEquipped? guessed correctly (verified against
        // BP_PlayerCharacter.hpp: matches the real ClothingGlovesEquipped?
        // property). Armor's guess was WRONG and live-confirmed as
        // "NOT FOUND" — the game's own naming breaks convention for this one
        // slot: the real property/function is BodyArmorEquipped? /
        // OnRep_BodyArmorEquipped? (BP_PlayerCharacter.hpp line 226/288),
        // not "Clothing"-prefixed like every other slot.
        { L"Clothing_Gloves", "Clothing_Gloves", 0, L"OnRep_ClothingGlovesEquipped?", nullptr, nullptr },
        { L"Clothing_Armor", "Clothing_Armor", 0, L"OnRep_BodyArmorEquipped?", nullptr, nullptr },
    };
    UObject* torsoComp = nullptr; UObject* legsComp = nullptr; UObject* feetComp = nullptr;
    UObject* leaderMesh = nullptr;
    {
        auto** meshSlot = static_cast<UObject**>(actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
        leaderMesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    }
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

        // 2026-08-16: PROACTIVE leader-pose refresh — see refresh_leader_
        // pose's own comment (proxy_manager.cpp) for the mechanism. Its
        // bone-mapping can go stale independent of the mesh ASSET pointer
        // staying perfectly valid, so a component whose mesh asset never
        // reads null (do_component_drift_scan's hasMeshNow check above)
        // never reaches ANY repair call, reactive or otherwise — live-
        // confirmed real gap: local:Clothing_Legs (the actual visible
        // pants mesh) logged one "initial post-join state = SET" and
        // NOTHING else for 20+ minutes, while local:Legs (the hidden naked
        // mesh underneath, on the reactive path since clothing keeps its
        // own hasMeshNow reading false) got a leader-pose refresh ~63
        // times over the same window. This closes that gap by refreshing
        // EVERY tracked component unconditionally, not just ones whose
        // mesh asset happens to go null.
        //
        // 2026-08-16, throttle widened 1s -> 20s: both PC1 and PC2 hung
        // simultaneously shortly after this went live at 1/s/component —
        // ~20 tracked components per character means up to ~20 ProcessEvent
        // calls to SetLeaderPoseComponent per second per character, a large
        // increase over the previous (reactive-only, mostly-idle) call
        // volume, right when this project's own documented
        // SetSkinnedAssetAndUpdate/attach-vs-ParallelAnimUpdate lock-
        // contention hazard is the leading suspect for exactly this kind of
        // hang. Widening the throttle to 20s was NOT enough — the very next
        // live test crashed instead (0xe06d7363, an unhandled C++ exception,
        // stack running entirely through this mod's own module from the
        // WndProc path, while the player was idle — no dump was generated to
        // pin the exact throw site). Two different failure modes (hang, then
        // crash) from this same proactive addition across two consecutive
        // live tests is strong enough evidence the mechanism itself — not
        // just its frequency — is unsafe in some circumstance not yet
        // identified. Kill-switched back to OFF (matching this project's own
        // established pattern for exactly this situation) rather than
        // continuing to guess at tuning. The reactive-only path (this
        // function's "skipped mesh-repair...refreshed leader-pose link"
        // branch and do_body_part_repair's own leader-pose refresh) is
        // unaffected and was never implicated in either failure — it stays
        // on. This reopens the Clothing_Legs-never-revisited gap this was
        // built to close; that's a real, known, accepted regression until
        // this can be root-caused properly (ideally against an actual
        // symbolized dump) rather than live-guessed again.
        // 2026-08-16, re-enabled: the hang (1/s throttle) and the crash (20s
        // throttle, combined with the transform-snap unconditional test) both
        // predate two changes since found/added: (1) this call already runs
        // inside the WndProc clean-trigger's cleanContext-gated path — traced
        // 2026-08-16 that the crash's own stack ran through that exact path
        // already, so cleanContext does NOT by itself rule out a repeat (the
        // crash's real cause is still unidentified, not proven nesting-
        // related) — but its core invariant (t_processEventDepth==0 whenever
        // cleanContext is true) has now held 1852/1852 checks with zero
        // violations across a real 7-hour, 45-relaunch session, so at minimum
        // this isn't running from a provably-bad context anymore; (2)
        // do_game_tick_clean_ctx (the WndProc handler's entry point) is now
        // SEH-wrapped, added specifically after that crash — same catch-and-
        // log pattern proven repeatedly elsewhere in this codebase all
        // session — so a repeat of the same unhandled-C++-exception failure
        // mode should now be caught and logged instead of taking the process
        // down. Re-enabling on that basis, kept at the already-widened 20s
        // throttle, leader-pose refresh ONLY (the transform-snap unconditional
        // variant stays off — not part of this decision). If it fails again,
        // revert this one line; see this comment block and audit_todo.md
        // item 6 for the full reasoning either way.
        static constexpr bool kEnableProactiveLeaderPoseRefresh = true;
        if (kEnableProactiveLeaderPoseRefresh) {
            const uint64_t nowUsRefresh = sdb::now_micros();
            if (nowUsRefresh - ctx.lastLeaderPoseRefreshUs >= 20'000'000ULL) {
                ctx.lastLeaderPoseRefreshUs = nowUsRefresh;
                sdb::refresh_leader_pose(comp, leaderMesh);
            }
        }
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

// cleanContext: see do_game_tick's own comment — gates the actual
// check_attach_health/check_component_drift scan (ProcessEvent-heavy) below;
// everything else in this function (throttle, pawn lookup, reset-detection)
// still runs regardless so a just-changed actor's stale snapshot never
// lingers, and the rotation index stays correct once scanning resumes.
static void check_attach_health_trigger(bool cleanContext)
{
    static uint64_t s_lastCheckUs = 0;
    static std::unordered_map<std::string, std::vector<uintptr_t>> s_snapshots;
    static std::unordered_map<std::string, ComponentDriftCtx> s_driftSnapshots;
    static std::unordered_map<uintptr_t, bool> s_itemHadMesh;
    static std::unordered_map<uintptr_t, std::array<double, 3>> s_itemLastPos;
    static std::unordered_map<std::string, AActor*> s_lastActorForLabel;
    static size_t s_rotateIndex = 0;
    const uint64_t now = sdb::now_micros();
    // 2026-08-16: this throttle used to gate the WHOLE function, including
    // reset-detection/target-building that must run regardless of
    // cleanContext. That created a real starvation bug once the scan itself
    // became cleanContext-gated below: on_process_event_pre's do_game_tick
    // (false) fires far more often than the ~16ms WndProc ticker or
    // on_actor_tick, so a false-context call would almost always win this
    // timestamp first and reset it right before a true-context call
    // arrived — meaning the actual scan (only enabled for cleanContext)
    // almost never got a turn even though do_game_tick(true) calls were
    // confirmed happening regularly. Live-reported: meshes staying
    // "animation locked" after respawn — exactly what starving this scan
    // would produce. Fix: s_lastCheckUs/300ms now ONLY gates the scan
    // itself, checked and updated inside the cleanContext block below, so a
    // non-clean call can no longer consume this window. Reset-detection and
    // target-building run every call (do_game_tick's own 5ms throttle is
    // the only cadence limit on those) — cheap pointer comparisons, no
    // ProcessEvent calls, no reason to also throttle them here. localPawn
    // itself DOES still need its own guard though: find_local_pawn() is an
    // expensive UE4SS reflection scan (see s_drop_fn's comment elsewhere in
    // this file for the same warning), and removing the whole-function
    // throttle above means this would otherwise run on every do_game_tick()
    // call (~5ms) instead of at most once per 300ms — cached_find_local_
    // pawn() (100ms-cached, already used by do_game_tick itself) is the
    // right call here now, not the raw scan.
    AActor* localPawn = cached_find_local_pawn();
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
    //
    // 2026-08-16: ALSO gated on time-since-pawnValidSinceUs now (state.hpp's
    // own comment) — equipDataReady tracks network-replication readiness,
    // a different thing from asset-streaming readiness. Several separate
    // live freezes (death, save-load, new-character+move, a new-character/
    // death/save-load combo) all happened doing exactly this scan's repair
    // work (SetSkinnedAssetAndUpdate/leader-pose refresh/clothing OnRep/
    // off-socket reattachment) shortly after the local pawn became valid —
    // matching a known hazard already documented elsewhere in this file:
    // this class of call can block GameThread on a critical section shared
    // with FAsyncLoadingThread when the character's own mesh/material
    // assets are still mid-stream. 3s mirrors (slightly more generous than)
    // RemotePlayer::proxySpawnedAtUs's existing 2s grace period for
    // proxies, since a local player's own full character+equipment is a
    // heavier load than a single proxy visual.
    constexpr uint64_t kLocalPawnRepairGraceUs = 3'000'000ULL;
    const bool localPawnPastGrace =
        (now - sdb::g_state().pawnValidSinceUs.load(std::memory_order_relaxed)) >= kLocalPawnRepairGraceUs;
    const bool localEligible = localPawn && localPawnPastGrace &&
                                sdb::g_state().equipDataReady.load(std::memory_order_acquire);

    // 2026-08-16: this used to run check_attach_health + check_component_drift
    // for "local" AND every connected proxy, all in one do_game_tick()
    // invocation, every 300ms. Live-captured evidence the same morning (both
    // PC1 and PC2, each independently frozen) showed proxy0's
    // component_drift running immediately followed by local's in the exact
    // same burst, right before the identical GameThread-parked-in-
    // WaitUntilTasksComplete hang traced earlier this session. Rotating to
    // one target per call spreads that burst out, mirroring the same fix
    // already proven for ProxyManager::tick()'s equipment/weapon-attachment/
    // appearance sync (2026-08-15) — same risk class (ProcessEvent-heavy
    // component/mesh repair calls stacking up nested inside another
    // ProcessEvent dispatch), same fix shape. Reset-detection (cheap, no
    // ProcessEvent calls) still runs for every known target every tick, so a
    // just-changed actor's stale snapshot never lingers past its next
    // rotation turn; only the expensive scan itself is rotated.
    // Live-verified: this rotation has run continuously across the whole
    // 2026-08-16/17 marathon session (hours, dozens of relaunches) with no
    // recurrence of the hang this was built to prevent.
    struct AttachHealthTarget { std::string label; AActor* actor; };
    std::vector<AttachHealthTarget> targets;
    if (localEligible) targets.push_back({"local", localPawn});

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
            targets.push_back({label, proxyActor});
        }
        proxyCount = i;
    }

    // 2026-08-16: gated on cleanContext — rotating one target per tick
    // reduced frequency but didn't remove the underlying re-entrancy risk
    // (confirmed: froze again on both machines even with the rotation in
    // place). See the "Reliable GameThread Trigger via WndProc Subclass"
    // plan — this scan only runs from mod.cpp's WndProc-triggered
    // clean-context tick now, never from the nested on_process_event_pre
    // fallback. Not advancing s_rotateIndex when skipped means the next
    // allowed call picks up the same target that would have run, rather
    // than silently skipping it.
    //
    // 300ms throttle (2026-08-14's original rationale, see above) now lives
    // HERE specifically — checked and updated only on this cleanContext
    // path, so a non-clean call can never consume it. See this function's
    // own opening comment for the starvation bug this fixes.
    if (cleanContext && !targets.empty() && now - s_lastCheckUs >= 300'000ULL) {
        s_lastCheckUs = now;
        const auto& t = targets[s_rotateIndex % targets.size()];
        ++s_rotateIndex;
        check_attach_health(t.label, t.actor, s_snapshots, s_itemHadMesh, s_itemLastPos);
        check_component_drift(t.label, t.actor, s_driftSnapshots);
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
        // The 2s post-spawn grace period used elsewhere for proxy writes was
        // tested disabled here starting 2026-08-13 (read-only diagnostic, not
        // a write) — 4 days of live sessions with no incident. Kept off
        // permanently.
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
        // The 2s post-spawn grace period used elsewhere for proxy writes was
        // tested disabled here starting 2026-08-13 (read-only diagnostic, not
        // a write) — 4 days of live sessions with no incident. Kept off
        // permanently.
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
        // The 2s post-spawn grace period used elsewhere for proxy writes was
        // tested disabled here starting 2026-08-13 (read-only diagnostic, not
        // a write) — 4 days of live sessions with no incident. Kept off
        // permanently.
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

// Flag file content: two lines, class name then the delegate's own property
// name (e.g. "OnAttachmentsUpdated") — reads a live FMulticastScriptDelegate
// property's own storage directly as a TArray (data ptr, count, max), the
// same layout already live-confirmed once this session for a DIFFERENT
// delegate (OnEquipmentUpdated, read via IDA's debugger, InvocationList
// data_ptr=0x0 count=0 — see research/04_ida_investigation_log.md). Reuses
// GetValuePtrByPropertyNameInChain (the same reflection call this whole file
// already uses everywhere) instead of needing a raw byte offset for the
// property — it returns a pointer straight to the delegate's own storage,
// which per the same precedent IS the TArray, no extra indirection needed.
// Read-only, SEH-wrapped, flag-triggered — same safety class as every other
// diagnostic in this file; not the unconditional-repair-call pattern that
// caused tonight's hang/crash.
// 2026-08-17: watches Clothing_Gloves' and Hands' own raw bVisible flag
// (USceneComponent+0x188, already-proven offset — see
// research/04_ida_investigation_log.md's "Weapon-visual attempts 9-11")
// and SkinnedAsset pointer (+0x5B8, same offset convention used throughout
// this file, e.g. read_local_pawn_appearance) directly off each
// component's own live memory, every game tick, logging only on an actual
// change (edge-triggered — can't flood the log even at high tick rate).
// Built specifically to answer whether a live-reported "gloves flicker,
// skin showing through, roughly every ~10s" on the local character is a
// real state change (bVisible actually flips, or the mesh pointer actually
// changes) or a pure render-thread artifact where neither ever changes and
// only the pixels are wrong for a frame — every other diagnostic tried
// tonight (the game's own AttachClothing/EquipClothingToMesh hooks,
// component_drift's periodic scan, mesh-cleared detection) found nothing,
// so this is the most direct read available: no reflection/ProcessEvent
// round-trip that could itself introduce timing slop.
struct GlovesFlickerCompState { bool visible = false; uintptr_t meshPtr = 0; bool haveBaseline = false; };
static GlovesFlickerCompState s_glovesFlickerGloves;
static GlovesFlickerCompState s_glovesFlickerHands;

static void check_one_gloves_flicker_component(AActor* pawn, const wchar_t* propName,
                                                const char* label, GlovesFlickerCompState& st)
{
    auto* compSlot = static_cast<UObject**>(pawn->GetValuePtrByPropertyNameInChain(propName));
    UObject* comp = (compSlot && *compSlot) ? *compSlot : nullptr;
    if (!comp) return;

    struct ReadCtx { UObject* comp; bool visible; uintptr_t meshPtr; };
    ReadCtx ctx{ comp, false, 0 };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<ReadCtx*>(raw);
        c->visible = *reinterpret_cast<const uint8_t*>(reinterpret_cast<uintptr_t>(c->comp) + 0x188) != 0;
        c->meshPtr = *reinterpret_cast<const uintptr_t*>(reinterpret_cast<uintptr_t>(c->comp) + 0x5B8);
    }, &ctx);
    if (!ok) return;

    if (!st.haveBaseline) {
        st.haveBaseline = true;
        st.visible = ctx.visible;
        st.meshPtr = ctx.meshPtr;
        char buf[160];
        snprintf(buf, sizeof(buf), "gloves_flicker: %s baseline visible=%d meshPtr=0x%llx",
                 label, ctx.visible ? 1 : 0, static_cast<unsigned long long>(ctx.meshPtr));
        debug_log(buf);
        return;
    }

    if (ctx.visible != st.visible || ctx.meshPtr != st.meshPtr) {
        char buf[224];
        snprintf(buf, sizeof(buf),
            "gloves_flicker: %s CHANGED visible %d->%d meshPtr 0x%llx->0x%llx",
            label, st.visible ? 1 : 0, ctx.visible ? 1 : 0,
            static_cast<unsigned long long>(st.meshPtr), static_cast<unsigned long long>(ctx.meshPtr));
        debug_log(buf);
        st.visible = ctx.visible;
        st.meshPtr = ctx.meshPtr;
    }
}

static void check_gloves_visibility_flicker()
{
    AActor* pawn = cached_find_local_pawn();
    if (!pawn) return;
    check_one_gloves_flicker_component(pawn, L"Clothing_Gloves", "Clothing_Gloves", s_glovesFlickerGloves);
    check_one_gloves_flicker_component(pawn, L"Hands", "Hands", s_glovesFlickerHands);
}

static void check_dump_delegate_trigger()
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return;
    std::wstring flag = std::wstring(path, n) + L"\\SurrounDeadBridge\\dump_delegate.flag";
    if (GetFileAttributesW(flag.c_str()) == INVALID_FILE_ATTRIBUTES) return;

    std::ifstream in(flag, std::ios::binary);
    std::string classNameU8, propNameU8;
    std::getline(in, classNameU8);
    std::getline(in, propNameU8);
    in.close();
    DeleteFileW(flag.c_str());
    if (!classNameU8.empty() && classNameU8.back() == '\r') classNameU8.pop_back();
    if (!propNameU8.empty() && propNameU8.back() == '\r') propNameU8.pop_back();
    if (classNameU8.empty() || propNameU8.empty()) {
        debug_log("dump_delegate: flag file missing class/property name lines");
        return;
    }

    auto widen = [](const std::string& s) {
        std::wstring w(s.size(), L'\0');
        int wn = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), (int)w.size());
        w.resize(wn);
        return w;
    };
    std::wstring className = widen(classNameU8);
    std::wstring propName  = widen(propNameU8);

    UObject* obj = UObjectGlobals::FindFirstOf(className.c_str());
    if (!obj) { debug_log("dump_delegate: " + classNameU8 + " instance/CDO not found"); return; }

    struct DumpCtx { UObject* obj; const wchar_t* prop; uintptr_t delegateAddr; uint64_t dataPtr; int32_t count; int32_t max; bool found; };
    DumpCtx dctx{ obj, propName.c_str(), 0, 0, 0, 0, false };
    auto dumpFn = [](void* raw) {
        auto* c = static_cast<DumpCtx*>(raw);
        void* slot = c->obj->GetValuePtrByPropertyNameInChain(c->prop);
        if (!slot) return;
        c->delegateAddr = reinterpret_cast<uintptr_t>(slot);
        c->dataPtr = *reinterpret_cast<const uint64_t*>(c->delegateAddr);
        c->count   = *reinterpret_cast<const int32_t*>(c->delegateAddr + 8);
        c->max     = *reinterpret_cast<const int32_t*>(c->delegateAddr + 12);
        c->found = true;
    };
    if (!seh_invoke(dumpFn, &dctx)) {
        debug_log("dump_delegate: " + classNameU8 + "." + propNameU8 + " crashed reading, caught via SEH");
        return;
    }
    if (!dctx.found) {
        debug_log("dump_delegate: " + propNameU8 + " property not found on " + classNameU8);
        return;
    }
    char buf[192];
    snprintf(buf, sizeof(buf),
             "dump_delegate: %s.%s @0x%llx InvocationList data_ptr=0x%llx count=%d max=%d",
             classNameU8.c_str(), propNameU8.c_str(),
             static_cast<unsigned long long>(dctx.delegateAddr),
             static_cast<unsigned long long>(dctx.dataPtr), dctx.count, dctx.max);
    debug_log(buf);
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

    // Testing escape hatch (2026-08-17) — drop a `disable_auto_continue.flag`
    // file in %APPDATA%\SurrounDeadBridge\ to skip the auto-click below
    // entirely and leave the menu alone, e.g. for testing menu-level
    // ProcessEvent calls (SaveMenu_C::LoadGameFromSlot etc.) that need to
    // observe/interact with the actual interactive main menu rather than
    // being yanked straight into gameplay. Checked once per call (cheap
    // file-existence check), not cached, so it can be toggled without a
    // relaunch.
    {
        wchar_t path[MAX_PATH];
        DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
        if (n != 0 && n < MAX_PATH) {
            std::wstring disableFlag = std::wstring(path, n) + L"\\SurrounDeadBridge\\disable_auto_continue.flag";
            if (GetFileAttributesW(disableFlag.c_str()) != INVALID_FILE_ATTRIBUTES) return false;
        }
    }

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
// dump_animbp_mutables (2026-08-13, removed 2026-08-17): the one-off,
// purpose-fulfilled diagnostic that found Player_AnimBP_C's "Speed"
// scratch DoubleProperty sits at object-relative offset 23232 off the
// AnimInstance base (compiler-anonymized in this Shipping build's FModel
// export, so found empirically via bytecode_dump.flag + mem_dump.flag +
// resolve_fname.flag rather than a named UPROPERTY lookup). That offset's
// own consumer (writing Speed directly) was itself later found not to work
// and removed — see proxy_manager.cpp's apply_proxy_speed_safe history —
// leaving this dump with no remaining purpose. Removed rather than kept
// dormant like the reusable flag-file diagnostics elsewhere in this file,
// since it was single-purpose from the start.

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

// World/weather actors are stable per-level singletons (unlike the pawn,
// which can be destroyed/respawned), so a slower 2s re-check interval is
// plenty — just enough to pick up a fresh instance after a level reload.
static AActor* cached_find_sky_actor()
{
    static std::atomic<uint64_t> s_last_try_us{0};
    static AActor* s_cached = nullptr;
    const uint64_t now = sdb::now_micros();
    const uint64_t last = s_last_try_us.load(std::memory_order_relaxed);
    if (last == 0 || now - last >= 2'000'000ULL) {
        s_last_try_us.store(now, std::memory_order_relaxed);
        s_cached = static_cast<AActor*>(UObjectGlobals::FindFirstOf(STR("Ultra_Dynamic_Sky_C")));
    }
    return s_cached;
}

static AActor* cached_find_weather_actor()
{
    static std::atomic<uint64_t> s_last_try_us{0};
    static AActor* s_cached = nullptr;
    const uint64_t now = sdb::now_micros();
    const uint64_t last = s_last_try_us.load(std::memory_order_relaxed);
    if (last == 0 || now - last >= 2'000'000ULL) {
        s_last_try_us.store(now, std::memory_order_relaxed);
        s_cached = static_cast<AActor*>(UObjectGlobals::FindFirstOf(STR("Ultra_Dynamic_Weather_C")));
    }
    return s_cached;
}

// World/weather/time sync — applies the server-authoritative WorldState
// broadcast (host-agent.js runs an independent time simulation and
// broadcasts it every worldStateIntervalMs; see server/src/host-agent.js's
// _broadcastWorldState) into the live Ultra Dynamic Sky / Ultra Dynamic
// Weather actors. This mod bypasses UE's real replication entirely (see
// [[project_sdo_bytecode_decode_pipeline]] investigation 2026-08-17 into how
// little of the engine's native networking SDO actually uses) — UDS's own
// "Replicated Time of Day" property is dead weight here, so this writes
// straight into the raw live properties the sky/weather systems read every
// frame instead. Offsets are from a raw member dump (research/
// CXXHeaderDump/Ultra_Dynamic_Sky.hpp's "Time of Day" @0x0320,
// Ultra_Dynamic_Weather.hpp's Cloud Coverage/Rain/Snow/Thunder/Wind
// Intensity/Fog @0x0310-0x0360), not guessed — but NOT yet live-verified
// (written while the game wasn't running this session). Verify these read
// back a plausible time-of-day/weather range before trusting further.
static uint32_t s_lastAppliedWorldRevision = 0;

static void apply_world_state()
{
    sdb::WorldState ws;
    bool valid = false;
    {
        std::lock_guard<std::mutex> lk(sdb::g_state().worldMtx);
        valid = sdb::g_state().worldStateValid;
        if (valid) ws = sdb::g_state().worldState;
    }
    if (!valid || ws.revision == s_lastAppliedWorldRevision) return;
    s_lastAppliedWorldRevision = ws.revision;

    if (AActor* sky = cached_find_sky_actor()) {
        const auto base = reinterpret_cast<uintptr_t>(sky);
        // Live-confirmed 2026-08-17, two rounds: a raw write to "Time of
        // Day" alone caused visible oscillation (UDS's own "Time Speed"
        // kept advancing it locally between our ~2.2s applies, so each
        // write briefly won before local drift crept past it again).
        // Freezing "Time Speed" to 0 fixed the oscillation but broke the
        // watch UI entirely — it stopped updating altogether, meaning
        // whatever refreshes that widget is gated on UDS's own change-
        // notification pipeline, not a raw memory poke, and that pipeline
        // apparently no-ops when Time Speed is 0 ("nothing is supposed to
        // be changing, skip the notify"). Fix: keep Time Speed frozen (no
        // competing local driver) but go through UDS's real public
        // "Transition Time of Day" UFunction instead of writing the raw
        // property directly — that's the actual intended API for external
        // callers, so it should carry the same notify/refresh side effects
        // the watch depends on. A 2s transition duration (matching the
        // server's ~2.2s broadcast cadence) also gives smooth per-frame
        // motion between our discrete updates instead of a step function.
        //
        // 2026-08-17, later: briefly reverted this whole ProcessEvent
        // approach back to raw writes after a player's pawn appearance got
        // corrupted and needed their save deleted. Root cause was NOT
        // confirmed to be these calls specifically — a same-position
        // server-side DB position edit happened around the same time and is
        // an equally live suspect (see
        // feedback_sdo_live_data_corruption_incident memory) — restored on
        // explicit user instruction. Still genuinely unverified in
        // isolation; treat with real caution until it gets a clean,
        // isolated live test.
        *reinterpret_cast<double*>(base + 0x0A48) = 0.0;
        if (UFunction* fn = sky->GetFunctionByNameInChain(L"Transition Time of Day")) {
            struct Params {
                double  NewTimeOfDay             = 0.0;
                double  TransitionDurationSeconds = 0.0;
                uint8_t EasingFunction            = 0;   // TEnumAsByte<EEasingFunc::Type>, Linear=0
                double  EasingExponent            = 2.0;
                bool    AllowTimeGoingBackwards   = true; // needed for the 2400->0 midnight wrap
            } params;
            params.NewTimeOfDay              = static_cast<double>(ws.timeOfDay);
            params.TransitionDurationSeconds = 2.0;
            sky->ProcessEvent(fn, &params);
        } else {
            // Fallback so time sync doesn't silently stop working if the
            // function name/signature ever changes — not yet observed live.
            *reinterpret_cast<double*>(base + 0x0320) = static_cast<double>(ws.timeOfDay);
            debug_log("apply_world_state: Transition Time of Day not found, used raw write fallback");
        }
    }
    if (AActor* weather = cached_find_weather_actor()) {
        // Weather (unlike time) has no single "SetWeather(floats)" entry
        // point — UDW models weather as a target *state object*
        // (UUDS_Weather_Settings_C*) you populate then transition to.
        // Persistent across calls (not reallocated every apply):
        // "Construct...if Invalid" only actually constructs the first time,
        // matching the pattern of reusing one object as a scratch target.
        static void* s_weatherStateObj = nullptr;

        if (UFunction* ctorFn = weather->GetFunctionByNameInChain(L"Construct Weather State Object if Invalid")) {
            struct ConstructParams { void* State = nullptr; } cparams;
            cparams.State = s_weatherStateObj;
            weather->ProcessEvent(ctorFn, &cparams);
            s_weatherStateObj = cparams.State;
        }

        if (s_weatherStateObj) {
            if (UFunction* popFn = weather->GetFunctionByNameInChain(L"Populate Weather State")) {
                struct PopulateParams {
                    void*  State         = nullptr;
                    double CloudCoverage = 0.0;
                    double Rain          = 0.0;
                    double Snow          = 0.0;
                    double Thunder       = 0.0;
                    double WindIntensity = 0.0;
                    double Fog           = 0.0;
                    double Dust          = 0.0;
                    double MaterialWetness = 0.0;
                    double MaterialSnow    = 0.0;
                    double MaterialDust    = 0.0;
                } pparams;
                pparams.State         = s_weatherStateObj;
                pparams.CloudCoverage = static_cast<double>(ws.cloudCover);
                pparams.Rain          = static_cast<double>(ws.rain);
                pparams.Snow          = static_cast<double>(ws.snow);
                pparams.Thunder       = static_cast<double>(ws.thunder);
                pparams.WindIntensity = static_cast<double>(ws.wind);
                pparams.Fog           = static_cast<double>(ws.fog);
                // Dust/Material* aren't in our protocol's WorldState — left
                // at 0, not guessed correlated values.
                weather->ProcessEvent(popFn, &pparams);
            }
            if (UFunction* changeFn = weather->GetFunctionByNameInChain(L"Change Weather")) {
                struct ChangeWeatherParams {
                    void*  NewWeatherType      = nullptr;
                    double TransitionSeconds   = 0.0;
                } chparams;
                chparams.NewWeatherType    = s_weatherStateObj;
                chparams.TransitionSeconds = 2.0; // matches the sky's transition duration
                weather->ProcessEvent(changeFn, &chparams);
            }
        } else {
            // Fallback so weather sync doesn't silently stop working if the
            // state-object API ever changes — not yet observed live.
            const auto base = reinterpret_cast<uintptr_t>(weather);
            *reinterpret_cast<double*>(base + 0x0310) = static_cast<double>(ws.cloudCover);
            *reinterpret_cast<double*>(base + 0x0320) = static_cast<double>(ws.rain);
            *reinterpret_cast<double*>(base + 0x0330) = static_cast<double>(ws.snow);
            *reinterpret_cast<double*>(base + 0x0340) = static_cast<double>(ws.thunder);
            *reinterpret_cast<double*>(base + 0x0350) = static_cast<double>(ws.wind);
            *reinterpret_cast<double*>(base + 0x0360) = static_cast<double>(ws.fog);
            debug_log("apply_world_state: weather state-object API unavailable, used raw write fallback");
        }
    }

    char line[256];
    snprintf(line, sizeof(line),
             "apply_world_state: revision=%u timeOfDay=%.2f rain=%.2f snow=%.2f fog=%.2f cloud=%.2f wind=%.2f thunder=%.2f"
             " sky=%s weather=%s",
             ws.revision, ws.timeOfDay, ws.rain, ws.snow, ws.fog, ws.cloudCover, ws.wind, ws.thunder,
             cached_find_sky_actor() ? "found" : "MISSING",
             cached_find_weather_actor() ? "found" : "MISSING");
    debug_log(line);
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
// Live-verified: this project's own later notes recorded 857/857 spawners
// successfully suppressed across every attempt in a full session, zero
// issues — run once, throttled-retried like every other one-shot resolution
// in this file, since spawner actors only exist once the level has finished
// loading.
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

// Detects a UWorld destroy+recreate (loading a save, level travel) and safely
// drops every remote player's and world entity's now-dangling actor pointer
// without touching it — the old UWorld tore down every actor in it, so
// calling ProcessEvent/destroy_actor_safe on these pointers would touch freed
// memory. Root-caused 2026-08-15/16: a live-reproduced freeze on loading a
// save to switch characters showed the identical GameThread-parked-in-
// FTaskGraphImplementation::WaitUntilTasksComplete signature already traced
// for the MsgType::Death self-proxy bug earlier this session (decompiled
// shipping-binary stack cross-referenced against real UE 5.3.2 TaskGraph.cpp
// source) — consistent with ProxyManager::tick()/EntityManager::tick()
// touching a stale actor pointer into a torn-down world and the engine's own
// mesh/component teardown never returning when re-entered from that context.
// Cached transform/equipment/appearance data is deliberately KEPT (only the
// actor pointers are cleared) and every dirty flag force-set, so the
// existing "!actor -> spawn" paths in both managers respawn fresh actors in
// the new world and immediately re-dress them from data already on hand,
// with no need to wait on the server to resend anything. NOT YET LIVE-
// VERIFIED — next step after this build is repeating the same save-load
// repro that found the bug.
static void* g_last_world = nullptr;

static void reset_stale_actors_on_world_change(UWorld* world)
{
    if (!world || world == g_last_world) { g_last_world = world; return; }
    const bool firstWorld = (g_last_world == nullptr);
    g_last_world = world;
    if (firstWorld) return; // nothing to reset on the very first tick

    auto& st = sdb::g_state();
    size_t playerCount = 0, entityCount = 0;
    {
        std::lock_guard<std::mutex> lk(st.playersMtx);
        for (auto& [id, player] : st.players) {
            player.proxyActor                 = nullptr;
            player.primaryWeaponVisualActor   = nullptr;
            player.facewearVisualActor        = nullptr;
            player.headwearVisualActor        = nullptr;
            player.eyewearVisualActor         = nullptr;
            player.backpackVisualActor        = nullptr;
            player.secondaryWeaponVisualActor = nullptr;
            player.sidearmVisualActor         = nullptr;
            player.meleeVisualActor           = nullptr;
            player.weaponAttachmentActors.clear();
            player.activeWeaponSlot    = 0xFF;
            player.handAttachedSlot    = 0xFF;
            player.appearanceSyncStage = 0;
            player.appliedAppearanceKey.clear();
            player.deathStateApplied = false; // freshly-spawned proxy hasn't had DeathState re-applied yet
            player.equipmentDirty         = true;
            player.weaponAttachmentsDirty = true;
            player.appearanceDirty        = true;
            player.proxySpawnedAtUs = 0; // treat as brand-new for the 2s grace period too
        }
        playerCount = st.players.size();
    }
    {
        std::lock_guard<std::mutex> lk(st.entityMtx);
        for (auto& [id, entity] : st.entities) {
            entity.actor             = nullptr;
            entity.lastActorAttemptUs = 0;
        }
        entityCount = st.entities.size();
    }
    debug_log("reset_stale_actors_on_world_change: world changed, reset " +
              std::to_string(playerCount) + " player(s), " +
              std::to_string(entityCount) + " entit(y/ies)");
}

// cleanContext: true when this call is known NOT to be nested inside another
// ProcessEvent dispatch — either from on_actor_tick (never reached from
// inside ProcessEvent at all) or the new WndProc-triggered tick (mod.cpp's
// "Reliable GameThread Trigger via WndProc Subclass" plan; UE5's own
// Windows-message pump runs once per frame outside any ProcessEvent chain).
// False from on_process_event_pre's fallback — that path starts execution
// from inside another call's pre-hook, so it's unavoidably nested even
// before this function's own ProcessEvent-based work begins (see that call
// site's own comment for the full derivation). Threaded through to
// ProxyManager::tick()'s allowDirtyStateSync and
// check_attach_health_trigger()'s own gate — the specific ProcessEvent-heavy
// calls (SetSkinnedAssetAndUpdate/SetLeaderPoseComponent/clothing OnRep_*)
// confirmed present in every live-captured freeze this session only run
// when cleanContext is true. Everything else in this function (movement
// send, teleport/velocity/rotation writes, the various check_*_trigger
// diagnostic reads) is unaffected — none of those has ever shown up as the
// hang site, so there's no reason to gate them.
static void do_game_tick(bool cleanContext)
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

    // Verification signal for cleanContext, not the gate itself — see
    // t_processEventDepth's own comment and the "Reliable GameThread
    // Trigger via WndProc Subclass" plan. depth should be 0 every single
    // time cleanContext is true (on_actor_tick or the new WndProc trigger);
    // an unthrottled warning here (this branch should be rare/never) is
    // worth more than a throttled one that could mask the first occurrence.
    // The periodic confirmation heartbeat (throttled — this runs every 5ms)
    // is so a clean run is visibly confirmed in the log, not just assumed
    // from the absence of a warning.
    if (cleanContext) {
        if (t_processEventDepth != 0) {
            debug_log("do_game_tick: cleanContext=true but t_processEventDepth=" +
                       std::to_string(t_processEventDepth) +
                       " — the 'message pump is clean' assumption is WRONG, investigate");
        } else {
            static uint64_t s_lastCleanConfirmUs = 0;
            if (now - s_lastCleanConfirmUs >= 10'000'000ULL) {
                s_lastCleanConfirmUs = now;
                debug_log("do_game_tick: cleanContext=true, t_processEventDepth=0 confirmed");
            }
        }
    }

    check_trace_trigger();
    check_bytecode_dump_trigger();
    // SEH-wrapped: brand-new, never-yet-fired ProcessEvent calls
    // (WidgetBlueprintLibrary::Create, AddToViewport) with a hand-built
    // params struct that hasn't been live-verified — same caution as the
    // watch_* diagnostics just below, applied proactively here rather than
    // after a crash, given the recent live data-corruption incident from an
    // unverified ProcessEvent call.
    if (!seh_invoke([](void*) { check_open_barber_menu_trigger(); }, nullptr))
        debug_log("open_barber_menu: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_restore_camera_trigger(); }, nullptr))
        debug_log("restore_camera: crashed, caught via SEH");
    check_barber_widget_removed_poller();
    if (!seh_invoke([](void*) { check_dump_playerstarts_trigger(); }, nullptr))
        debug_log("dump_playerstarts: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_teleport_random_spawn_trigger(); }, nullptr))
        debug_log("teleport_random_spawn: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_reset_player_stats_trigger(); }, nullptr))
        debug_log("reset_player_stats: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_max_vitals_trigger(); }, nullptr))
        debug_log("max_vitals: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_load_game_from_slot_trigger(); }, nullptr))
        debug_log("load_game_from_slot: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_set_current_save_slot_trigger(); }, nullptr))
        debug_log("set_current_save_slot: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_read_current_save_slot_trigger(); }, nullptr))
        debug_log("read_current_save_slot: crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_set_save_name_direct_trigger(); }, nullptr))
        debug_log("set_save_name_direct: crashed, caught via SEH");
    check_resolve_fname_trigger();
    check_resolve_ptr_trigger();
    check_resolve_fprop_trigger();
    // 2026-08-16: SEH-wrapped — live-crashed (EXCEPTION_ACCESS_VIOLATION,
    // real crash log with symbolized stack) inside check_watch_activeslot_
    // trigger, reading a proxy's player.proxyActor pointer that had gone
    // stale (world torn down by a save-load) before reset_stale_actors_on_
    // world_change() got a chance to null it out — that cleanup only runs
    // once a NEW valid local pawn is found, further down in this function,
    // while these four watch_* diagnostics run here, earlier, unconditional
    // on pawn state. All four share the identical pattern (dereferencing
    // player.proxyActor with no validity check — see check_watch_rotation_
    // trigger/log_lefthand_values/log_activeslot_values/log_aimoffset_values),
    // so all four get the same guard rather than fixing just the one that
    // happened to be caught. These are flag-file-gated diagnostic tools, not
    // core gameplay logic — SEH-wrapping the call site is the right scope
    // for the fix, not restructuring when world-change cleanup runs.
    if (!seh_invoke([](void*) { check_watch_aimoffset_trigger(); }, nullptr))
        debug_log("do_game_tick: check_watch_aimoffset_trigger crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_watch_rotation_trigger(); }, nullptr))
        debug_log("do_game_tick: check_watch_rotation_trigger crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_watch_lefthand_trigger(); }, nullptr))
        debug_log("do_game_tick: check_watch_lefthand_trigger crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_watch_activeslot_trigger(); }, nullptr))
        debug_log("do_game_tick: check_watch_activeslot_trigger crashed, caught via SEH");
    // 2026-08-16: SEH-wrapped, same audit that found the check_watch_*
    // crash above. check_attach_health_trigger's rotated scan reads
    // player.proxyActor and calls check_attach_health/check_component_drift
    // (ProcessEvent-heavy repair calls) on it with no staleness guard — this
    // one runs UNCONDITIONALLY every 300ms whenever a proxy exists (no flag
    // file gate, unlike the watch_* diagnostics), making it the single
    // highest-frequency touch of a possibly-stale proxyActor pointer in the
    // whole tick, and the leading suspect for the second (unsymbolized)
    // "loading a save after already joining" crash on PC2 — that repro has
    // no diagnostic flag involved, which the four already-fixed watch_*
    // triggers all require. check_active_weapon_trigger has the identical
    // pattern (reads player.proxyActor via a flag-gated diagnostic) — same
    // fix for the same reason, lower frequency since it needs its flag set.
    if (!seh_invoke([](void* raw) { check_attach_health_trigger(*static_cast<bool*>(raw)); }, &cleanContext))
        debug_log("do_game_tick: check_attach_health_trigger crashed, caught via SEH");
    if (!seh_invoke([](void*) { check_active_weapon_trigger(); }, nullptr))
        debug_log("do_game_tick: check_active_weapon_trigger crashed, caught via SEH");
    check_current_actor_diagnostic(); // self-SEH-wrapped internally, see its own comment
    check_item_freeze_diagnostic(); // self-SEH-wrapped internally, see its own comment
    check_fabrik_dump_trigger();
    check_widget_scan_trigger();
    check_scan_pickup_class_trigger();
    check_dump_clothing_table_trigger();
    check_call_trigger();
    check_mem_dump_trigger();
    check_dump_delegate_trigger();
    check_gloves_visibility_flicker(); // unconditional, cheap raw reads — see its own comment

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
    // 2026-08-16 audit: SEH-wrapped after a live crash — EXCEPTION_MSVC_CXX
    // (0xE06D7363, a genuine thrown C++ exception, not an access violation)
    // inside ProxyManager::on_movement(), reached from here with no guard at
    // all. This whole dispatch path (every inbound Movement/Equipment/
    // WeaponAttachments/PawnAppearance/PlayMontage/Death/Respawn/WorldState/
    // PlayerDamage/entity-lifecycle message) had ZERO exception protection
    // before this — unlike every local-side proxy/actor touch this project
    // has spent tonight hardening, a single bad inbound frame could crash
    // the client outright. Wrapping per-frame (not the whole loop) so one
    // bad frame doesn't stop the rest of the batch from being processed.
    for (const auto& f : g_tcp.recv_all()) {
        const sdb::Frame* fptr = &f;
        if (!seh_invoke([](void* raw) { dispatch_frame(*static_cast<const sdb::Frame*>(raw)); }, const_cast<sdb::Frame*>(fptr)))
            debug_log("do_game_tick: dispatch_frame crashed on an inbound frame, caught via SEH");
    }

    // 1b. First-join flow: server's FirstJoin message (authoritative —
    // gateway.js sends it exactly when this playerId has no saved
    // progress) means this is a genuinely new player. Deferred here rather
    // than handled inline in dispatch_frame since a live pawn may not
    // exist yet the instant the message arrives. Retired the old 6s-
    // timeout heuristic + SDOnline Lua script hand-off this replaced
    // (2026-08-17) — see pendingFirstJoin's own comment in state.hpp.
    {
        auto& st2 = sdb::g_state();
        if (st2.pendingFirstJoin.load(std::memory_order_acquire) && cached_find_local_pawn()) {
            st2.pendingFirstJoin.store(false, std::memory_order_release);
            st2.inFirstJoinFlow = true;
            Output::send<LogLevel::Normal>(
                STR("SDB: first join — opening barber menu for character customization\n"));
            if (!seh_invoke([](void*) { do_open_barber_menu(); }, nullptr))
                debug_log("do_game_tick: do_open_barber_menu (first-join) crashed, caught via SEH");
        }
    }

    // 2. Find local pawn.
    auto& st    = sdb::g_state();
    AActor* pawn = cached_find_local_pawn();

    if (pawn) {
        g_local_helper_ptr.store(*reinterpret_cast<uintptr_t*>(
            reinterpret_cast<uintptr_t>(pawn) + 0x700), std::memory_order_relaxed);

        static bool s_actorTickRegistered = false;
        if (!s_actorTickRegistered) {
            s_actorTickRegistered = true;
            fixup_and_register_actor_tick_hook(pawn);
        }

        ensure_hwnd_ticker_started(); // idempotent — only does real work the first time, see its own comment
        ensure_parallel_anim_eval_disabled(pawn); // idempotent — see its own comment (UE-191796 candidate fix)

        // 2026-08-16: health-based death detection. Root-caused via a live
        // zombie-kill retest: cached_find_local_pawn() NEVER returned null
        // for a real death all session (server log showed zero DeathRequest
        // frames ever sent, despite multiple confirmed deaths) — the pawn
        // persists, just frozen in place (matches an earlier finding: a real
        // death "froze in place, still standing, not ragdolled"), so the
        // !pawn branch below never has a chance to fire for a genuine
        // combat/zombie death, only for an actual pawn-instance swap (level
        // reload, new character). Reads the same proven-safe offset the
        // vitals-restore path already uses (pawn+0x7D0 -> +0xD0, Medical
        // Component.Health) via read_local_health_only() — NOT the full
        // read_local_progress(), which also does a FindFirstOf scan; see
        // that dedicated helper's own comment for why (live-reproduced 1 FPS
        // regression). Treats health<=0 as death independently of pawn-
        // nullness — see state.hpp's sentDeathByHealth comment for why this
        // is a separate flag rather than merged into hasPawn/sentDeath.
        const double localHealth = read_local_health_only(pawn);
        const bool healthDead = localHealth <= 0.0;
        const bool wasHealthDead = st.sentDeathByHealth.exchange(healthDead, std::memory_order_relaxed);
        if (healthDead && !wasHealthDead) {
            send_header_only(sdb::MsgType::DeathRequest);
            debug_log("do_game_tick: health<=0 (" + std::to_string(localHealth) + ") detected, sent DeathRequest");
        } else if (!healthDead && wasHealthDead) {
            send_header_only(sdb::MsgType::RespawnRequest);
            debug_log("do_game_tick: health recovered (" + std::to_string(localHealth) + "), sent RespawnRequest");
        }
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

        sdb::g_proxy_manager().tick(nullptr, nullptr, cleanContext);
        return;
    }

    const bool was_dead = st.sentDeath.exchange(false);
    if (!st.hasPawn.exchange(true) || was_dead) {
        st.noPlayerSinceUs.store(0);
        st.pawnValidSinceUs.store(now, std::memory_order_relaxed);
        if (was_dead) send_header_only(sdb::MsgType::RespawnRequest);
        // New/respawned pawn — RepActorsData hasn't necessarily replicated
        // back in yet. See state.hpp's equipDataReady comment.
        st.equipDataReady.store(false, std::memory_order_release);
    }
    // Belt-and-braces alongside the hasPawn-transition check above: character
    // creation may hand back a genuinely new pawn INSTANCE without hasPawn
    // ever observably going false in between (no guarantee cached_find_local_
    // pawn() returns null even briefly during that specific transition,
    // unlike a real death/respawn or level reload). Comparing the raw
    // pointer catches that case too — same pawnValidSinceUs refresh either
    // way.
    {
        static AActor* s_lastPawnPtr = nullptr;
        if (pawn != s_lastPawnPtr) {
            s_lastPawnPtr = pawn;
            st.pawnValidSinceUs.store(now, std::memory_order_relaxed);
        }
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
    reset_stale_actors_on_world_change(world);
    sdb::g_proxy_manager().tick(world, pawn, cleanContext);

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

    // 9. Flashlight/NVG toggle sync — unthrottled (called every tick, not
    // gated behind the 2s equip cadence above): two raw bool reads plus a
    // comparison, negligible cost, and send_player_lights is internally
    // change-detected so this can't spam the network — only sends the
    // instant either value actually flips, for near-immediate proxy sync on
    // a discrete player action instead of riding the slower 2s equip cycle.
    send_player_lights(pawn);

    // 9b. Weapon-fire first-shot detection — same unthrottled cadence as
    // step 9 (see check_weapon_fire_edge's own comment for why this exists
    // instead of a ProcessEvent hook).
    check_weapon_fire_edge(pawn);

    // 9c. TEMPORARY head-look diagnostic — see check_head_rot_diagnostic's
    // own comment. Remove once the real mapping is confirmed.
    check_head_rot_diagnostic(pawn);

    // 10. World/weather/time sync — unthrottled like step 9: a single
    // revision-number comparison, apply_world_state itself only does real
    // work when the server's periodic WorldState broadcast actually changed
    // something.
    apply_world_state();
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

// See on_process_event_pre's weapon-fire-sync comment for the full story —
// resolved once off any live BP_FirearmPickup_C instance (or its CDO),
// shared per-class same as every other UFunction* pointer in this file.
static UFunction* s_fireBullet_fn = nullptr;

// 2026-08-21: Local_StartShooting's decoded bytecode (research/bytecode/
// ubergraph_decoded/BP_FirearmPickup_C_Local_StartShooting.decoded.txt)
// shows the FIRST bullet of every trigger pull — single-fire's only bullet,
// and full-auto's first round alike — is fired via two back-to-back
// EX_LocalVirtualFunction calls (FireBullet, then Event_FireRate). Only
// once IsWeaponAutomatic? is true does it arm K2_SetTimer(Self,
// "FireBullet", rate, looping=true); that timer's repeat callbacks DO
// dispatch via real ProcessEvent, which is what s_fireBullet_fn was
// actually catching all along — explaining "single click = nothing, full
// auto = works" (and meaning full-auto's own first round has always been
// silently missed too, just unnoticed among many).
//
// Tried hooking Svr_WeaponShot(bool Started?) instead (plain EX_VirtualFunction,
// same object) — resolved fine but never matched live. Tried hooking
// Local_StartShooting itself next: traced its real caller in
// BP_PlayerCharacter_C's Ubergraph (InpActEvt_IA_PrimaryAction_..._64, entry
// 60116/0xead4) and confirmed it's invoked via EX_Context{object_expr=
// CurrentFiringWeapon} + EX_LocalVirtualFunction — i.e. even a genuine
// cross-object call (character → weapon actor) goes through the "Local"
// opcode and does NOT re-enter ProcessEvent. Live-tested 2026-08-21: resolved
// fine, never matched despite repeated confirmed firing. So neither the
// Local/plain-Virtual distinction nor same-object/cross-object is what
// determines ProcessEvent visibility — only a genuine external→ProcessEvent
// root (Enhanced Input calling the InpActEvt wrapper itself, or the Timer
// Manager calling FireBullet) is visible; anything that function calls
// internally, however it's wrapped, is invisible to this hook.
//
// Given three straight hook attempts failed, switched approach entirely:
// this project's own established, *proven* pattern for "detect a local
// player state change and sync it" is direct polling of a persistent
// instance property (see read_local_player_lights's FlashlightOn?/
// NightVisionOn? — raw offset reads, edge-detected against a static
// last-value, no ProcessEvent involved at all). BP_PlayerCharacter.hpp lists
// `CurrentFiringWeapon` (ABP_FirearmPickup_C*, offset 0x0930) — set to the
// weapon actor by this exact Ubergraph entry right before calling
// Local_StartShooting, and cleared to null by Local_StopShooting's entry
// (59833/0xe9b9). Polling this pointer for a null→non-null edge each tick
// (see check_weapon_fire_edge, mod.cpp) catches the first bullet of every
// trigger pull — single or auto — with the same accepted dry-fire tradeoff
// (ammo isn't checked until inside Local_StartShooting) any of the three
// hook attempts would have had.

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

// handle_montage_play_engine_hook and handle_play_montage_hook (both
// 2026-08-13, removed 2026-08-17): both hooks fired correctly, but
// UFunction* is per-CLASS not per-instance, so the same pointer resolved
// off the local player also matched PlayMontage/Montage_Play calls made on
// PROXY actors of the same class — once on_play_montage (proxy_manager.cpp)
// started actually calling PlayMontage to apply a received montage on a
// proxy, these hooks fired for that call too, treated it as a fresh local
// action, and re-broadcast it: client A's own montage bounced to client B's
// proxy-of-A, which re-broadcast back to A applied to B's proxy, forever,
// amplifying with whatever both sides had recently played. Looked like
// severe jitter/desync live (send_play_montage spamming the same ~9-montage
// sequence every ~30ms). check_local_montage_change() (the
// GetCurrentMontage() poll, below) supersedes both entirely and is
// correctly scoped — called directly on a known local-pawn pointer, never
// resolved as a bare class-wide UFunction* compared against every
// ProcessEvent call in the process.

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
// 2026-08-16: extended with a second, independently-refreshed region for
// the weapon/item-attach desync investigation (shotgun/pistol rendering
// stuck on the ground while still genuinely AttachParent'd — see
// do_attach_health_scan's DETACHED-diff comment). [0,5) stays the original
// clothing watch (pawn/torso/legs/feet/helper, set_recent_calls_watch,
// unchanged); [5,21) holds the local player's currently-attached top-level
// item children, refreshed every check_attach_health("local", ...) poll via
// set_item_recent_calls_watch below — that scan already computes this exact
// list for its own detach-diffing, so this just mirrors it into the watch.
// Without this, the DETACHED handler's dump_recent_calls() call has nothing
// to show for a weapon/item hit (live-confirmed 2026-08-16: a real shotgun/
// pistol detach produced a dump with zero matching entries, since neither
// was ever in the old 5-slot clothing-only watch).
static UObject* s_watchObjs[21] = {};
static constexpr int kItemWatchBase = 5;
static constexpr int kItemWatchMax  = 16;

static void set_recent_calls_watch(UObject* pawn, UObject* torso, UObject* legs, UObject* feet, UObject* helper)
{
    s_watchObjs[0] = pawn;
    s_watchObjs[1] = torso;
    s_watchObjs[2] = legs;
    s_watchObjs[3] = feet;
    s_watchObjs[4] = helper;
}

static void set_item_recent_calls_watch(const std::vector<uintptr_t>& children)
{
    const int n = static_cast<int>((std::min)(children.size(), static_cast<size_t>(kItemWatchMax)));
    for (int i = 0; i < n; ++i)
        s_watchObjs[kItemWatchBase + i] = reinterpret_cast<UObject*>(children[i]);
    for (int i = n; i < kItemWatchMax; ++i)
        s_watchObjs[kItemWatchBase + i] = nullptr;
}

static void record_recent_call(UObject* obj, UFunction* func)
{
    bool watched = false;
    for (UObject* w : s_watchObjs) {
        if (w == obj) { watched = true; break; }
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

// Temporary diagnostic (2026-08-17): trying to find the real FGameplayTag
// "Option" value BP_Barber_C::OnServerExecuteInteract receives when a real
// player interaction opens CharacterBarberMenu — needed to eventually
// force-open that menu for a first-time joiner. The Ubergraph decode
// (research/bytecode/pc1_decoded/, ExecuteUbergraph_BP_Barber) dispatches
// via a computed jump read from a local variable, not a simple offset
// table, so reversing the exact tag from static bytecode alone wasn't
// practical — capturing the real live value from an actual interaction is
// the same "verify, don't guess" approach used throughout this project.
// Remove once the real tag value has been captured and used.
// Broadened after two guessed functions (OnServerExecuteInteract,
// OnExecuteInteract) both failed to fire despite the menu genuinely being
// open live — rather than keep guessing param layouts one function at a
// time, just detect which of BP_Barber_C's interact-related functions
// fires AT ALL first (name + obj only, no param decoding yet), then do one
// precise follow-up pass once the real one is known.
static constexpr const wchar_t* kBarberCandidateFns[] = {
    L"OnBeginInteract", L"OnEndInteract", L"OnExecuteInteract",
    L"OnExecuteInteractDialogue", L"OnExecuteInteractEnded",
    L"OnRequestServerInteract", L"OnServerExecuteInteract", L"OnStopExecuteInteract",
};
static UFunction* s_barberCandidateFnPtrs[std::size(kBarberCandidateFns)] = {};

static void check_barber_interact_diagnostic(UObject* obj, UFunction* func, void* /*params*/)
{
    bool anyUnresolved = false;
    for (auto* p : s_barberCandidateFnPtrs) if (!p) { anyUnresolved = true; break; }
    if (anyUnresolved) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (UObject* barber = UObjectGlobals::FindFirstOf(STR("BP_Barber_C"))) {
                for (size_t i = 0; i < std::size(kBarberCandidateFns); ++i) {
                    if (s_barberCandidateFnPtrs[i]) continue;
                    s_barberCandidateFnPtrs[i] = barber->GetFunctionByNameInChain(kBarberCandidateFns[i]);
                }
                debug_log("check_barber_interact_diagnostic: resolve pass complete");
            }
        }
    }
    if (!func) return;
    for (size_t i = 0; i < std::size(kBarberCandidateFns); ++i) {
        if (func != s_barberCandidateFnPtrs[i]) continue;
        std::string objClass = obj ? short_object_name(obj) : "<null>";
        char line[256];
        int need = WideCharToMultiByte(CP_UTF8, 0, kBarberCandidateFns[i], -1, nullptr, 0, nullptr, nullptr);
        std::string fnName(need > 1 ? need - 1 : 0, '\0');
        if (need > 0) WideCharToMultiByte(CP_UTF8, 0, kBarberCandidateFns[i], -1, fnName.data(), need, nullptr, nullptr);
        snprintf(line, sizeof(line),
                 "check_barber_interact_diagnostic: FIRED %s obj=0x%llx objClass=%s",
                 fnName.c_str(), reinterpret_cast<unsigned long long>(obj), objClass.c_str());
        debug_log(line);
    }
}

// Automatic camera/input restore when the barber menu's real Exit button is
// clicked — user-requested 2026-08-17 after live-testing showed clicking
// the widget's own exit left the camera stuck (check_open_barber_menu_
// trigger only replicates the game's *open* sequence, so nothing was
// wired to reverse it on close).
//
// Two wrong guesses before this, both from reasoning off the static decode
// alone instead of re-verifying live — the actual lesson of this whole
// saga:
// 1. Hooked BP_Barber_C's own "Event_Exit" (bound via EX_BindDelegate/
//    EX_AddMulticastDelegate to the widget's "ExitMenu" multicast delegate
//    in the real open flow, per BP_Barber_C's Ubergraph). Fired once live,
//    then appeared to go silent on the next test — but see below, it was
//    never actually broken.
// 2. Theorized (wrongly) that check_open_barber_menu_trigger's widget never
//    gets Event_Exit bound to its ExitMenu delegate (since the bind step
//    was never replicated), so switched to hooking
//    CharacterBarberMenu_C::Exit() directly (the 18-byte trampoline that
//    calls ExecuteUbergraph_CharacterBarberMenu(1385)). This was wrong too
//    — confirmed via a real trace_trigger.flag full-ProcessEvent-trace
//    (2026-08-17) that Exit() never fires at all; the real button-click
//    handler is a UMG-generated bound-event function named
//    BndEvt__BackButton_K2Node_ComponentBoundEvent_13_
//    OnButtonClickedEvent__DelegateSignature, which dispatches into the
//    Ubergraph internally (EX_LocalFinalFunction, no separate reflected
//    ProcessEvent call for Exit() itself). That SAME trace also showed
//    BP_Barber_C::Event_Exit DOES fire correctly right after the click —
//    proving guess #1 was actually right all along, and the "silent"
//    result before was something else (most likely a stale non-rebuilt
//    DLL still deployed, or restore_camera.flag manual testing masking
//    whether the automatic hook had actually run) rather than a real gap
//    in the delegate-binding replication theory.
// Back to hooking BP_Barber_C::Event_Exit, now trace-confirmed rather than
// assumed. Resolved once per live barber instance (same throttled-retry-
// then-cache shape as kBarberCandidateFns/s_barberCandidateFnPtrs above)
// and matched by function pointer only — see
// [[feedback_sdo_ufunction_shared_per_class]] for why that caveat doesn't
// apply here (local-player-only interaction).
static UFunction* s_barberEventExitFn = nullptr;

static void check_barber_exit_hook(UObject* obj, UFunction* func)
{
    if (!s_barberEventExitFn) {
        static std::atomic<uint64_t> s_lastTryUs{0};
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_lastTryUs.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_lastTryUs.store(now, std::memory_order_relaxed);
            if (UObject* barber = UObjectGlobals::FindFirstOf(STR("BP_Barber_C"))) {
                s_barberEventExitFn = barber->GetFunctionByNameInChain(L"Event_Exit");
                debug_log(s_barberEventExitFn ? "check_barber_exit_hook: Event_Exit resolved"
                                               : "check_barber_exit_hook: Event_Exit NOT FOUND");
            }
        }
    }
    if (!func || func != s_barberEventExitFn) return;
    debug_log("check_barber_exit_hook: FIRED Event_Exit obj=0x" +
              std::to_string(reinterpret_cast<uintptr_t>(obj)) + " — restoring camera/input");
    if (!seh_invoke([](void*) { do_restore_camera_and_input(); }, nullptr))
        debug_log("check_barber_exit_hook: do_restore_camera_and_input crashed, caught via SEH");
}

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
    ++t_processEventDepth; // see t_processEventDepth's own comment — always first, always paired with on_process_event_post's decrement

    // 2026-08-15: REVERTED the GameThread-only guard that was here —
    // live-reported immediately after deploy: on_unreal_init's own
    // "hooks registered" log line was the LAST thing ever logged, meaning
    // g_game_thread_id (captured in on_unreal_init) does NOT actually match
    // the thread this hook fires on for real gameplay calls — UE4SS
    // apparently calls on_unreal_init from a different (loader?) thread
    // than the one driving ProcessEvent/tick, disproving the assumption
    // this guard was built on. The underlying finding (a hang dump showed
    // ProcessEvent reaching our code from a Background Worker thread) may
    // still be real, but needs a reliable GameThread anchor that isn't
    // on_unreal_init before trying this again — not attempted further
    // tonight given how badly the wrong anchor broke things. See
    // g_game_thread_id's own comment for the original (now-disproven)
    // reasoning.
    record_recent_call(obj, func);
    check_load_data_requested_hook(obj, func);
    check_set_sex_mesh_hook(obj, func);
    check_attach_clothing_hooks(obj, func);
    check_barber_interact_diagnostic(obj, func, params);
    check_barber_exit_hook(obj, func);

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

    // 2026-08-20: weapon-fire sync. Live trace_trigger.flag capture during a
    // real shot showed firing never plays a UAnimMontage at all (nothing
    // check_local_montage_change's GetCurrentMontage() poll could ever
    // catch) — the real visual/audio trigger is BP_FirearmPickup_C::
    // FireBullet(), which calls MuzzleEffects() then StartRecoil()
    // unconditionally on every real trigger pull, before any ammo/damage
    // logic (research/bytecode/ubergraph_decoded/BP_FirearmPickup_C_
    // FireBullet.decoded.txt). Resolving FireBullet's UFunction* once (any
    // live instance or the CDO — the pointer is shared per-class, same
    // proven-safe resolve pattern as s_drop_fn just above) is enough to
    // detect ANY weapon firing, local or proxy — unlike the PlayMontage/
    // Montage_Play hooks abandoned 2026-08-17 (see feedback_sdo_ufunction_
    // shared_per_class memory), this doesn't risk an echo loop: the receive
    // side (ProxyManager::on_weapon_fired) calls MuzzleEffects/StartRecoil
    // directly, never FireBullet itself, so a proxy replay can never
    // re-trigger this hook. Still gated on GetOwner()==local pawn (the same
    // GetOwner-via-ProcessEvent pattern do_weapon_attach_scan already uses)
    // so only the local player's own shots get broadcast, not every
    // proxy's.
    static std::atomic<uint64_t> s_last_fire_bullet_fn_try_us{0};
    if (func && !s_fireBullet_fn) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_fire_bullet_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_fire_bullet_fn_try_us.store(now, std::memory_order_relaxed);
            UObject* firearm = UObjectGlobals::FindFirstOf(L"BP_FirearmPickup_C");
            if (!firearm) {
                UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
                firearm = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__BP_FirearmPickup_C");
            }
            if (firearm) {
                s_fireBullet_fn = firearm->GetFunctionByNameInChain(L"FireBullet");
                debug_log(s_fireBullet_fn ? "on_process_event_pre: FireBullet resolved"
                                           : "on_process_event_pre: FireBullet NOT FOUND on BP_FirearmPickup_C");
            } else {
                debug_log("on_process_event_pre: BP_FirearmPickup_C instance/CDO not found (fire-bullet resolve)");
            }
        }
    }
    if (func && func == s_fireBullet_fn && obj) {
        UFunction* getOwnerFn = obj->GetFunctionByNameInChain(L"GetOwner");
        AActor* owner = nullptr;
        if (getOwnerFn) obj->ProcessEvent(getOwnerFn, &owner);
        AActor* localPawn = find_local_pawn();
        debug_log("on_process_event_pre: FireBullet MATCHED obj=0x" +
                  std::to_string(reinterpret_cast<uintptr_t>(obj)) +
                  " getOwnerFn=" + std::to_string(getOwnerFn != nullptr) +
                  " owner=0x" + std::to_string(reinterpret_cast<uintptr_t>(owner)) +
                  " localPawn=0x" + std::to_string(reinterpret_cast<uintptr_t>(localPawn)) +
                  " isLocal=" + std::to_string(owner && localPawn && owner == localPawn));
        if (owner && localPawn && owner == localPawn) {
            send_header_only(sdb::MsgType::WeaponFired);
            debug_log("on_process_event_pre: local FireBullet detected, sent WeaponFired");
        }
    }


    // 2026-08-16: purely observational — correlates the periodic scene-wide
    // item-detach bursts (research/04_ida_investigation_log.md Session 60:
    // proxy's ENTIRE loadout and the local player's own equipped knife
    // detached within 0.5s of each other, at irregular-but-roughly-15-minute
    // intervals, ruling out any of this mod's own per-tick proxy code as the
    // cause since it never touches the local player's real pawn) against
    // this game's own confirmed AutoSaveGame/Event_AutoSave/EventSave
    // functions (BP_SurroundeadGameMode_C, bp_catalog_player_core.md). Each
    // client runs its own local UE5 world (this project's own custom TCP
    // layer syncs state between separate local sessions, not native UE
    // networking) — proxies are locally-spawned actors in THIS client's own
    // world, so a local save/serialize pass hitting the whole scene would
    // explain hitting both the real pawn's and the proxy's attachments
    // identically. Same throttled-FindFirstOf-then-cache-pointer shape as
    // s_drop_fn/s_buildFn above — a per-call string lookup here would repeat
    // the FindFirstOf-every-tick FPS regression already hit once tonight.
    static UFunction* s_autoSaveGameFn = nullptr;
    static UFunction* s_eventAutoSaveFn = nullptr;
    static UFunction* s_eventSaveFn = nullptr;
    static std::atomic<uint64_t> s_last_gamemode_fn_try_us{0};
    if (func && (!s_autoSaveGameFn || !s_eventAutoSaveFn || !s_eventSaveFn)) {
        const uint64_t now = sdb::now_micros();
        const uint64_t last = s_last_gamemode_fn_try_us.load(std::memory_order_relaxed);
        if (last == 0 || now - last >= 1'000'000ULL) {
            s_last_gamemode_fn_try_us.store(now, std::memory_order_relaxed);
            if (UObject* gm = UObjectGlobals::FindFirstOf(STR("BP_SurroundeadGameMode_C"))) {
                if (!s_autoSaveGameFn)  s_autoSaveGameFn  = gm->GetFunctionByNameInChain(L"AutoSaveGame");
                if (!s_eventAutoSaveFn) s_eventAutoSaveFn = gm->GetFunctionByNameInChain(L"Event_AutoSave");
                if (!s_eventSaveFn)     s_eventSaveFn     = gm->GetFunctionByNameInChain(L"EventSave");
                debug_log(std::string("on_process_event_pre: GameMode save fns AutoSaveGame=") +
                          (s_autoSaveGameFn ? "resolved" : "NOT FOUND") + " Event_AutoSave=" +
                          (s_eventAutoSaveFn ? "resolved" : "NOT FOUND") + " EventSave=" +
                          (s_eventSaveFn ? "resolved" : "NOT FOUND"));
            }
        }
    }
    if (func && (func == s_autoSaveGameFn || func == s_eventAutoSaveFn || func == s_eventSaveFn)) {
        debug_log("SAVE_EVENT_FIRED: " + std::string(
            func == s_autoSaveGameFn ? "AutoSaveGame" : func == s_eventAutoSaveFn ? "Event_AutoSave" : "EventSave"));
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

    // The PlayMontage hook that used to fire here was removed 2026-08-17 —
    // per-class UFunction* echo-loop bug (a proxy's PlayMontage call shared
    // the same UFunction* as the local player's, so applying a received
    // montage to a proxy re-triggered this hook and re-broadcast it,
    // forever). Full history preserved next to check_local_montage_change's
    // declaration, which supersedes it.

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

    // The Montage_Play engine hook that used to fire here was removed
    // 2026-08-17 — same per-class UFunction* echo-loop bug as the PlayMontage
    // hook above. Superseded by check_local_montage_change() below.

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

    // 2026-08-15: on_actor_tick reliability watchdog + conditional fallback.
    // Historically this called do_game_tick() unconditionally here because
    // on_actor_tick can go silent (root cause never reverse-engineered —
    // see register_actor_tick_hook's comment). The problem: THIS call site
    // is itself nested inside UE4SS's ProcessEvent pre-hook, i.e. inside the
    // engine's own outer ProcessEvent dispatch for whatever unrelated
    // function triggered this sampled tick — a live-captured hang dump
    // proved a single SetSkinnedAssetAndUpdate call made from exactly this
    // nested position can block GameThread on a critical section shared
    // with FAsyncLoadingThread (RtlEnterCriticalSection at the top of the
    // stack, FAsyncLoadingThread the only non-idle thread in the dump) —
    // a risk that exists independent of call volume, so every earlier
    // mitigation tonight (spreading bursts, capping retries) reduced
    // frequency but couldn't eliminate it. on_actor_tick's own dispatch
    // point doesn't have this problem — it isn't reached from inside
    // another ProcessEvent call. So: only use this risky path when the
    // clean one is confirmed NOT currently working (silent for >500ms
    // despite having fired at least once before), and actively try to
    // recover the clean path rather than just permanently relying on this
    // fallback.
    const uint64_t lastActorTick = g_last_actor_tick_us.load(std::memory_order_relaxed);
    const bool actorTickWorking = g_actor_tick_ever_fired.load(std::memory_order_relaxed) &&
                                   (now - lastActorTick < 500'000ULL);
    if (!actorTickWorking) {
        // Recovery attempt, throttled to once per 2s so a persistently
        // broken hook doesn't get re-registered on every single sample.
        // Skipped entirely before on_actor_tick has EVER fired once —
        // nothing to recover yet this early (e.g. still on the main menu,
        // no world/actors ticking at all), and hammering the registration
        // call before UE4SS's own hook table is ready is untested territory
        // this session didn't need to risk.
        if (g_actor_tick_ever_fired.load(std::memory_order_relaxed)) {
            const uint64_t lastReregister = g_last_actor_tick_reregister_us.load(std::memory_order_relaxed);
            if (now - lastReregister >= 2'000'000ULL) {
                g_last_actor_tick_reregister_us.store(now, std::memory_order_relaxed);
                const bool reregistered = register_actor_tick_hook();
                debug_log("watchdog: on_actor_tick silent for " + std::to_string((now - lastActorTick) / 1000) +
                          "ms, re-registered=" + std::to_string(reregistered));
            }
        }
        do_game_tick(false); // nested inside this ProcessEvent's own pre-hook — see do_game_tick's own comment
    }
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
    const bool isAds = (player.movState & 0x02) != 0;
    if (auto* ads = static_cast<uint8_t*>(obj->GetValuePtrByPropertyNameInChain(L"IsADS")))
        *ads = isAds ? 1 : 0;
    if (auto* falling = static_cast<uint8_t*>(obj->GetValuePtrByPropertyNameInChain(L"Falling")))
        *falling = (player.movState & 0x04) ? 1 : 0;

    // Head-look sync (2026-08-21): Player_AnimBP_C::GetHeadRot (decoded via
    // bytecode_dump.flag, research/bytecode/ubergraph_decoded/
    // Player_AnimBP_C_GetHeadRot.decoded.txt) computes HeadRotation/HeadQuat
    // from NormalizedDeltaRotator(ControlRotation, ActorRotation) every
    // frame — the exact same "reads a real Controller a proxy doesn't have"
    // problem already fixed above for Pitch/Yaw (GetAimOffset), just for a
    // separate property pair never touched before. Decoded logic: clamp
    // Pitch to [-60,60], clamp Yaw to [-40,40] then again to [-90,30] (net
    // effect [-40,30]), RInterpTo-smooth (not re-implemented here —
    // player.renderAimPitch/renderAimYaw are already smoothed upstream by
    // ProxyManager::update_proxy_render_smoothing), and force (0,0,0)
    // whenever IsADS && !InMeleeStance (melee-stance exception skipped —
    // ADS-while-melee isn't a real combination in this game). HeadQuat is
    // Conv_RotatorToQuaternion(HeadRotation) in the real function; computed
    // here via FRotator::Quaternion()'s standard formula (stable across UE
    // versions) rather than calling the native function live. Sign
    // (positive renderAimYaw, unlike Pitch/Yaw's negated write above) is
    // NOT yet live-confirmed — NormalizedDeltaRotator is a plain rotation
    // delta, not an authored blendspace sample, so it shouldn't carry this
    // game's blendspace-specific inversion, but flip if a live test shows
    // the head turning the wrong way.
    struct FRotatorD { double Pitch, Yaw, Roll; };
    struct FQuatD    { double X, Y, Z, W; };
    if (auto* headRot = static_cast<FRotatorD*>(obj->GetValuePtrByPropertyNameInChain(L"HeadRotation"))) {
        FRotatorD r{ 0.0, 0.0, 0.0 };
        if (!isAds) {
            r.Pitch = std::clamp(static_cast<double>(player.renderAimPitch), -60.0, 60.0);
            r.Yaw   = std::clamp(static_cast<double>(player.renderAimYaw),   -40.0, 30.0);
        }
        *headRot = r;
        if (auto* headQuat = static_cast<FQuatD*>(obj->GetValuePtrByPropertyNameInChain(L"HeadQuat"))) {
            constexpr double kDegToRad = 3.14159265358979323846 / 180.0;
            const double sp = std::sin(r.Pitch * kDegToRad * 0.5), cp = std::cos(r.Pitch * kDegToRad * 0.5);
            const double sy = std::sin(r.Yaw   * kDegToRad * 0.5), cy = std::cos(r.Yaw   * kDegToRad * 0.5);
            const double sr = std::sin(r.Roll  * kDegToRad * 0.5), cr = std::cos(r.Roll  * kDegToRad * 0.5);
            headQuat->X =  cr*sp*sy - sr*cp*cy;
            headQuat->Y = -cr*sp*cy - sr*cp*sy;
            headQuat->Z =  cr*cp*sy - sr*sp*cy;
            headQuat->W =  cr*cp*cy + sr*sp*sy;
        }
    }
}

static void on_process_event_post(UObject* obj, UFunction* func, void* /*params*/)
{
    // Always first, always pairs with on_process_event_pre's increment —
    // see t_processEventDepth's own comment. Placed before the early return
    // below so it fires unconditionally, matching UE4SS's own guarantee
    // (DetourInstance.hpp's Invoke()) that every Pre callback invocation is
    // followed by a Post invocation regardless of what happens in between.
    // The underflow guard is defensive only — this thread_local starts at 0
    // and every decrement should have already been preceded by an
    // increment, but this hook fires thousands of times a frame and a
    // silent wrap-to-huge-number on any missed pairing would be far worse
    // than a one-line guard.
    if (t_processEventDepth > 0) --t_processEventDepth;

    // 2026-08-15: REVERTED — see on_process_event_pre's matching comment.
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

        // ProxyManager::tick()'s own 2s post-spawn grace period
        // (RemotePlayer::proxySpawnedAtUs) guards its heavy equipment/
        // weapon-attachment/appearance sync burst — that one turned out to
        // be load-bearing (disabling it 2026-08-13 caused a real live freeze
        // two days later, re-enabled 2026-08-15). This aim pitch/yaw path is
        // a much lighter operation (two pointer writes, tested disabled
        // separately since 2026-08-13) and has run 4 days of live sessions
        // with no incident — kept off permanently, but this is NOT the same
        // proven-safe conclusion as the heavier sync burst above; revisit if
        // proxies ever crash/deadlock right after spawn again.

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
    const uint64_t now = sdb::now_micros();
    g_last_actor_tick_us.store(now, std::memory_order_relaxed);
    g_actor_tick_ever_fired.store(true, std::memory_order_relaxed);
    do_game_tick(true); // never reached from inside a ProcessEvent dispatch — genuinely clean, see do_game_tick's own comment
}

// Root-caused 2026-08-16 (real UE4SS 3.0.1 source, deps/first/Unreal/src/
// UnrealInitializer.cpp:846 + Metadata.hpp:229-236): UE4SS resolves the
// AActorTick hook's target address from AActor's own Class Default Object
// (/Script/Engine.Default__Actor) — the BASE, un-overridden Tick vtable
// slot. ACharacter/APawn (the real native ancestors of every playable
// character class) virtually always override Tick, so the actual player
// pawn's vtable slot for Tick points somewhere else entirely — the hook
// UE4SS installs is simply never reached by the player's own pawn, or by
// any other Character/Pawn-derived actor. Whatever occasionally satisfied
// the existing silence-watchdog (register_actor_tick_hook, below) before
// was coincidental: some plain AActor-derived, non-overriding actor
// happening to tick in whichever level was loaded. A level transition
// changing which such actors exist fully explains "on_actor_tick goes
// silent after level transitions" without anything actually breaking.
//
// Fix: before EVER calling RegisterAActorTickPreCallback for the first
// time (which is what actually installs the binary detour, using whatever
// address AActor::TickInternal currently resolves to — see
// TDetourInstance::InstallHook, DetourInstance.hpp:110), overwrite that
// resolved address with the REAL player pawn's own vtable-slot value for
// Tick. AActor::TickInternal is exported by UE4SS.dll as raw static data
// (confirmed directly against the DLL's export table, not guessed) with
// UE4SS's own RC::Function<void(AActor*,float)> layout (deps/first/
// Function/include/Function/Function.hpp): three pointers then a bool
// (active func / stored-original func / address / is_ready). The vtable
// slot's offset isn't separately exported, so it's found empirically: scan
// the CDO's own vtable for whichever slot currently matches
// AActor::TickInternal's already-resolved (CDO-based) address, then read
// that same slot index off the real pawn's vtable. Slot values are run
// through the same "resolve past a potential jmp thunk" step UE4SS itself
// uses (ASM::resolve_function_address_from_potential_jmp) so a thunked
// vtable entry doesn't produce a false non-match.
// NOT YET LIVE-VERIFIED — next step after this build is repeating the
// same "new character, move around" repro that found the underlying
// nested-ProcessEvent hang, watching for on_actor_tick to now stay live
// instead of falling back to the risky nested on_process_event_pre path.
struct RawUE4SSFunctionLayout { void* active_func; void* stored_original_func; void* function_address; bool is_ready; };

static void* resolve_potential_jmp_thunk(void* addr)
{
    if (!addr) return nullptr;
    auto* bytes = static_cast<uint8_t*>(addr);
    if (bytes[0] == 0xE9) { // near relative jmp — same opcode UE4SS's own resolver checks for
        const int32_t rel = *reinterpret_cast<int32_t*>(bytes + 1);
        return bytes + 5 + rel;
    }
    return addr;
}

struct TickVTableFixupCtx { AActor* pawn; bool success = false; };

static void do_fixup_actor_tick_hook_target(void* ctxRaw)
{
    auto* ctx = static_cast<TickVTableFixupCtx*>(ctxRaw);
    auto* ue4ss = GetModuleHandleW(L"UE4SS.dll");
    if (!ue4ss) return;

    auto* tickInternal = reinterpret_cast<RawUE4SSFunctionLayout*>(GetProcAddress(ue4ss,
        "?TickInternal@AActor@Unreal@RC@@2V?$Function@$$A6AXPEAVAActor@Unreal@RC@@M@Z@3@A"));
    if (!tickInternal || !tickInternal->is_ready || !tickInternal->function_address) {
        debug_log("fixup_actor_tick: AActor::TickInternal export not found or not yet resolved");
        return;
    }

    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* cdo = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"/Script/Engine.Default__Actor");
    if (!cdo) {
        debug_log("fixup_actor_tick: Default__Actor CDO not found");
        return;
    }

    void** cdoVtable  = *reinterpret_cast<void***>(cdo);
    void** pawnVtable = *reinterpret_cast<void***>(ctx->pawn);
    void*  baseAddr   = tickInternal->function_address;

    constexpr int kMaxVtableScan = 600; // generous — AActor's own vtable is nowhere near this large
    int matchIndex = -1;
    for (int i = 0; i < kMaxVtableScan; ++i) {
        if (resolve_potential_jmp_thunk(cdoVtable[i]) == baseAddr) { matchIndex = i; break; }
    }
    if (matchIndex < 0) {
        debug_log("fixup_actor_tick: couldn't find Tick's vtable slot on Default__Actor (scanned " +
                  std::to_string(kMaxVtableScan) + " slots)");
        return;
    }

    void* pawnAddr = resolve_potential_jmp_thunk(pawnVtable[matchIndex]);
    char addrBuf[96];
    snprintf(addrBuf, sizeof(addrBuf), "fixup_actor_tick: Tick vtable slot=%d base(CDO)=0x%llx pawn=0x%llx",
              matchIndex,
              static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(baseAddr)),
              static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(pawnAddr)));
    debug_log(std::string(addrBuf) +
              (pawnAddr == baseAddr ? " (same — pawn doesn't override Tick, nothing to fix)"
                                    : " (DIFFERENT — pawn overrides Tick, correcting hook target)"));
    if (!pawnAddr || pawnAddr == baseAddr) { ctx->success = true; return; } // nothing to change, not a failure

    tickInternal->active_func          = pawnAddr;
    tickInternal->stored_original_func = pawnAddr;
    tickInternal->function_address     = pawnAddr;
    tickInternal->is_ready             = true;
    ctx->success = true;
}

// 2026-08-15: factored out of on_unreal_init so the reliability watchdog
// (on_process_event_pre) can call the exact same registration again later.
// Root cause of "on_actor_tick is not reliable after level transitions"
// was never actually reverse-engineered this session (would need static
// analysis of UE4SS.dll's own hook implementation, not attempted) — this
// takes the pragmatic path instead: detect the SYMPTOM (the hook goes
// silent) and re-register, which recovers regardless of the underlying
// mechanism, without needing to understand it. Safe to call repeatedly —
// UE4SS's own registration just installs/overwrites the callback pointer,
// no observed teardown step required first.
static bool register_actor_tick_hook()
{
    auto* ue4ss = GetModuleHandleW(L"UE4SS.dll");
    using RegTick = void(*)(Hook::AActorTickFn);
    auto* fn_tick = ue4ss ? reinterpret_cast<RegTick>(GetProcAddress(ue4ss,
        "?RegisterAActorTickPreCallback@Hook@Unreal@RC@@YAXV?$function"
        "@$$A6AXPEAVAActor@Unreal@RC@@M@Z@std@@@Z")) : nullptr;
    if (!fn_tick) return false;
    fn_tick(on_actor_tick);
    return true;
}

// One-time entry point: corrects AActor::TickInternal's resolved address
// (see do_fixup_actor_tick_hook_target's own comment) and ONLY THEN makes
// the very first call to register_actor_tick_hook — the first call to
// RegisterAActorTickPreCallback for this detour target is what actually
// installs the binary patch, at whatever address is currently resolved, so
// the fixup has to land before it, not after. Called once, the first time
// do_game_tick() has a real pawn to read a vtable from; on_unreal_init()
// runs far too early for that (no pawn exists yet), which is exactly the
// timing gap this fixup needs.
static void fixup_and_register_actor_tick_hook(AActor* pawn)
{
    TickVTableFixupCtx ctx{ pawn };
    if (!seh_invoke(do_fixup_actor_tick_hook_target, &ctx))
        debug_log("fixup_and_register_actor_tick_hook: vtable fixup crashed, caught via SEH — registering with UE4SS's original (possibly-wrong) address instead");
    else if (!ctx.success)
        debug_log("fixup_and_register_actor_tick_hook: vtable fixup did not find a corrected address — registering with UE4SS's original address");

    if (!register_actor_tick_hook())
        Output::send<LogLevel::Error>(STR("SDB: RegisterAActorTickPreCallback not found\n"));
}

// ── Reliable clean-context tick trigger ("Reliable GameThread Trigger via
// WndProc Subclass" plan, 2026-08-16) ──────────────────────────────────────
// AActorTick's silence (see register_actor_tick_hook's own comment) and
// on_process_event_pre's inherent nesting (see do_game_tick's own comment)
// leave no reliable, genuinely non-nested way to drive the risky
// (SetSkinnedAssetAndUpdate/SetLeaderPoseComponent/clothing OnRep_*-class)
// per-tick work — confirmed the hard way: 10+ live-captured freezes across
// two machines, byte-identical GameThread-parked-in-WaitUntilTasksComplete
// signature every time, even after real mitigations (grace period, one-
// target-per-tick rotation) that reduced frequency but not the underlying
// risk. UE4SS exports no EngineTick/AsyncTask-equivalent hook either
// (checked directly against UE4SS.dll's PE export table).
//
// This uses a mechanism outside UE4SS's hook system entirely: a background
// thread's ONLY job is periodically posting a custom message to the game's
// own window (PostMessage is explicitly documented as thread-safe from any
// thread); a subclassed WndProc catches it and runs the risky tick work
// from there. UE5's standalone-game Windows message pump runs once per
// frame from early in the engine loop, dispatching messages from a point
// that is NOT nested inside any ProcessEvent dispatch — structurally the
// same kind of clean entry point AActorTick provides when it fires, just
// reached through the OS message queue instead of depending on UE4SS's own
// (unreliable, for AActorTick; unexported, for EngineTick) hook resolution.
//
// SetWindowLongPtr-based WndProc subclassing is a standard, well-understood
// Win32 technique (used internally by MFC and many frameworks) — a
// fundamentally lower risk class than the raw inline-hooking/vtable-
// patching this project has used elsewhere for genuinely unavoidable cases.
//
// Live-verified 2026-08-17: t_processEventDepth (see do_game_tick's own
// comment) read 0 on every single check across a real 7-hour, 45-relaunch
// session — 1852/1852, zero violations — confirming this really is a clean,
// non-nested entry point in practice, not just in theory.
static constexpr UINT kCleanTickMessage = WM_APP + 1;

static std::atomic<bool> g_hwndTickerRunning{false};
static std::atomic<bool> g_hwndTickerStop{false};
static std::thread       g_hwndTickerThread;
static HWND              g_gameHwnd = nullptr;
static WNDPROC           g_originalWndProc = nullptr;

// 2026-08-16: defensive SEH wrap — added after a live crash (0xe06d7363,
// unhandled C++ exception) whose stack ran entirely through this mod's own
// module via this exact WndProc path. do_game_tick(true) was previously
// called directly here with no outer guard, unlike every other entry point
// into this mod's tick logic; this is the newest, least-proven trigger
// mechanism (added this session), so bringing it in line with the rest of
// the codebase's belt-and-suspenders SEH coverage is warranted regardless
// of whether this specific wrap would have caught that exact crash.
static void do_game_tick_clean_ctx(void*) { do_game_tick(true); }

static LRESULT CALLBACK sdb_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == kCleanTickMessage) {
        if (!seh_invoke(do_game_tick_clean_ctx, nullptr))
            debug_log("sdb_wnd_proc: do_game_tick(true) crashed, caught via SEH");
    }
    // Always call through — must never disrupt normal input/resize/close
    // handling. CallWindowProc (not calling g_originalWndProc directly) is
    // the standard, correct way to chain a subclassed WndProc.
    return CallWindowProcW(g_originalWndProc, hwnd, msg, wParam, lParam);
}

namespace {
struct FindHwndCtx { DWORD pid; HWND result; };
BOOL CALLBACK find_hwnd_for_pid(HWND hwnd, LPARAM lParam)
{
    auto* ctx = reinterpret_cast<FindHwndCtx*>(lParam);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == ctx->pid && IsWindowVisible(hwnd)) {
        ctx->result = hwnd;
        return FALSE; // stop enumeration — found it
    }
    return TRUE;
}
} // namespace

// Idempotent — safe to call every time a pawn is found; only does real work
// the first time (guarded by do_game_tick's own s_actorTickRegistered-style
// static at its call site). On any failure (no visible window found yet, or
// SetWindowLongPtr fails), the risky work simply keeps running through the
// existing on_process_event_pre fallback rather than never running at all —
// same fallback-not-silent-failure principle as fixup_and_register_actor_
// tick_hook's own error paths.
static void ensure_hwnd_ticker_started()
{
    static bool s_attempted = false;
    if (s_attempted) return;
    s_attempted = true;

    FindHwndCtx ctx{ GetCurrentProcessId(), nullptr };
    EnumWindows(find_hwnd_for_pid, reinterpret_cast<LPARAM>(&ctx));
    if (!ctx.result) {
        debug_log("hwnd_ticker: no visible top-level window found for this process yet — risky tick work stays on the existing nested fallback");
        return;
    }
    g_gameHwnd = ctx.result;

    g_originalWndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(g_gameHwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(sdb_wnd_proc)));
    if (!g_originalWndProc) {
        debug_log("hwnd_ticker: SetWindowLongPtrW failed, GetLastError=" + std::to_string(GetLastError()) +
                   " — risky tick work stays on the existing nested fallback");
        g_gameHwnd = nullptr;
        return;
    }

    g_hwndTickerThread = std::thread([] {
        while (!g_hwndTickerStop.load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            if (g_gameHwnd) PostMessageW(g_gameHwnd, kCleanTickMessage, 0, 0);
        }
    });
    g_hwndTickerRunning.store(true, std::memory_order_relaxed);

    char buf[96];
    snprintf(buf, sizeof(buf), "hwnd_ticker: started, hwnd=0x%llx",
             static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(g_gameHwnd)));
    debug_log(buf);
}

// 2026-08-17: candidate root-cause fix for tonight's whole render/animation-
// desync investigation (mag/laser/shotgun floating, PC2's helmet, PC1's
// shotgun+pistol, PC2's frozen-but-attached body animation) — researched
// against real UE5 engine behavior rather than guessed further. Confirmed
// Epic-tracked bug UE-191796, affecting exactly engine versions 5.2 and 5.3
// (this game is 5.3.2): in 5.2 the bone-buffer swap for a
// USkinnedMeshComponent moved from the sequential PrePhysics tick to the
// *parallel* end-of-frame render-data update, which now runs concurrently
// across every mesh component in the scene. That creates a race — on some
// frames a leader-pose FOLLOWER's parallel task can run before its LEADER's
// buffer swap finishes, so the follower reads last frame's stale bone data.
// Matches every symptom chased tonight: attachment/transform machinery is
// completely untouched (why every one of this project's own AttachParent/
// socket-distance checks reads clean), only the bone data itself goes stale
// — and a stale-boned socket's own world transform can read wrong too, which
// plausibly explains why items looked misplaced even while "properly
// attached." A related Epic forum report describes the identical symptom
// class (random skeletal-mesh detach + animation stop) specifically on
// *packaged* builds with *networked/proxy* characters — this project's exact
// setup. Epic has only a tentative source-level fix, not yet in a shipped
// 5.3.2 patch — nothing this project can pull in directly.
//
// The one lever actually available from here: `a.ParallelAnimEvaluation 0`
// forces all animation evaluation back onto the game thread sequentially,
// eliminating the race by construction (no more parallel tasks to race)
// instead of the detect-and-repair-after-the-fact approach every other fix
// tonight has taken. Set via UKismetSystemLibrary::ExecuteConsoleCommand —
// a standard BlueprintCallable static (Engine.hpp), called through this
// project's own established GetFunctionByNameInChain+ProcessEvent pattern on
// the class CDO, same as every other native-function call in this codebase.
// Retried at most every 2s until it succeeds once, then never again — not a
// recurring per-frame repair operation, a fundamentally different (and much
// lower) risk class than tonight's earlier proactive-repair attempts.
// Trade-off: parallel anim evaluation exists to spread
// animation cost across worker threads, so this does cost some CPU; likely
// immaterial for a 1-2 player session, but worth knowing if a future session
// ever needs to revisit it.
static void ensure_parallel_anim_eval_disabled(AActor* worldContextActor)
{
    static bool s_done = false;
    if (s_done) return;

    // Retried every 2s rather than a true one-shot, though in practice the
    // fix below (2026-08-17, second pass) should now succeed on the first
    // try — kept as a safety margin regardless. Same throttled-retry-until-
    // success shape as s_last_drop_fn_try_us elsewhere in this file, not a
    // tight per-tick loop.
    static uint64_t s_lastTryUs = 0;
    const uint64_t nowUs = sdb::now_micros();
    if (nowUs - s_lastTryUs < 2'000'000ULL) return;
    s_lastTryUs = nowUs;

    // 2026-08-17, second pass: FindFirstOf(L"KismetSystemLibrary") never
    // found anything live-tested (6+ retries over 12+ real seconds of active
    // play on both machines, well past when any lazy-construction theory
    // should have kicked in) — FindFirstOf matches by an object's CLASS
    // name, but a class-default-object is never returned that way here; it
    // needs to be looked up by its own actual object name. Every UObject CDO
    // in UE is unconditionally named "Default__<ClassName>"
    // (UClass::GetDefaultObjectName(), a hardcoded engine-wide convention,
    // not something guessed per-game) — so look it up directly by that exact
    // name via the same UObjectGlobals::FindObject(Class=nullptr,
    // InOuter=ANY_PACKAGE, Name) call resolve_class_by_name already uses
    // successfully elsewhere in this codebase (proxy_manager.cpp) for
    // exact-path lookups FindAllOf/FindFirstOf can't do. Kept the original
    // FindFirstOf as a fallback in case this build's CDO is ever found the
    // other way for some reason.
    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    UObject* sysLib = UObjectGlobals::FindObject(nullptr, kAnyPackage, L"Default__KismetSystemLibrary");
    if (!sysLib) sysLib = UObjectGlobals::FindFirstOf(L"KismetSystemLibrary");
    if (!sysLib) {
        debug_log("parallel_anim_eval: KismetSystemLibrary CDO not found yet, will retry");
        return;
    }
    s_done = true;
    UFunction* execFn = sysLib->GetFunctionByNameInChain(L"ExecuteConsoleCommand");
    if (!execFn) {
        debug_log("parallel_anim_eval: ExecuteConsoleCommand UFunction not found, skipped");
        return;
    }

    static const wchar_t kCmd[] = L"a.ParallelAnimEvaluation 0";
    struct ExecConsoleCommandParams {
        UObject*             WorldContextObject = nullptr;
        native::UnrealFString CommandStr{};
        AActor*              SpecificPlayer = nullptr;
    } params;
    params.WorldContextObject = worldContextActor;
    params.CommandStr = native::UnrealFString{ const_cast<wchar_t*>(kCmd),
                                                static_cast<int32_t>(std::size(kCmd)),
                                                static_cast<int32_t>(std::size(kCmd)) };

    struct CallCtx { UObject* obj; UFunction* fn; ExecConsoleCommandParams* p; };
    CallCtx ctx{ sysLib, execFn, &params };
    const bool ok = seh_invoke([](void* raw) {
        auto* c = static_cast<CallCtx*>(raw);
        c->obj->ProcessEvent(c->fn, c->p);
    }, &ctx);

    debug_log(ok ? "parallel_anim_eval: executed 'a.ParallelAnimEvaluation 0'"
                 : "parallel_anim_eval: ExecuteConsoleCommand crashed, caught via SEH");
}

// ── UE4SS 3.x mod class ───────────────────────────────────────────────────

class SDBMod : public RC::CppUserModBase {

public:
    SDBMod() { ModName = STR("SurrounDeadBridge"); }

    void on_unreal_init() override
    {
        // Diagnostic (2026-08-16): debug_log's own file (%APPDATA%\
        // SurrounDeadBridge\debug.log) stopped receiving any writes at all
        // partway through tonight — no new lines across the last several
        // launches, including ones that clearly reached real gameplay per a
        // live screenshot. Windows Defender's Controlled Folder Access was
        // suspected but directly ruled out (zero 1123/1124 block events in
        // Microsoft-Windows-Windows Defender/Operational, which has 11k+
        // other records, so the log itself is capturing things fine). This
        // uses UE4SS's own Output::send instead of debug_log specifically
        // so it can't be affected by whatever debug_log's specific file-I/O
        // path is hitting — Output::send is what populates UE4SS.log, and
        // that log demonstrably keeps updating every launch. If this line
        // shows up in UE4SS.log but debug_log's lines still don't appear in
        // debug.log, the problem is narrowly in debug_log's own file
        // handling; if THIS line is also missing, on_unreal_init itself
        // isn't being reached at all, which would be a much bigger problem
        // than a broken log file.
        Output::send<LogLevel::Normal>(STR("SDB: on_unreal_init ENTRY (Output::send diagnostic)\n"));
        debug_log("on_unreal_init: entered");
        // See g_game_thread_id's own comment — captured here since UE4SS
        // only ever calls on_unreal_init from GameThread.
        g_game_thread_id = GetCurrentThreadId();

        auto sc = load_session_config();
        merge_command_line_args(sc);

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

        // AActorTick registration is deliberately NOT done here — see
        // fixup_and_register_actor_tick_hook's own comment. No real pawn
        // exists this early to read a corrected hook target from; that
        // first registration now happens from do_game_tick() instead, the
        // first time a real pawn is available.

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
