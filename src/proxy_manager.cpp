#include "proxy_manager.hpp"
#include "debug_log.hpp"

#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/UObject.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/DynamicOutput/Output.hpp>

#include <windows.h>
#include <cmath>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

// UE5 LWC FTransform's real memory layout: three SIMD-aligned 32-byte blocks
// (Rotation quat XYZW, Translation XYZ+pad, Scale3D XYZ+pad), each stored as
// doubles — confirmed via IDA decompile of the engine's own Kismet exec thunk
// for BeginDeferredActorSpawnFromClass, which builds an identical 96-byte
// default-identity transform from three constant xmmwords before Blueprint
// pins override individual fields. Not the vendored SDK's FVector/FRotator —
// those are just X/Y/Z doubles with no matching FTransform type at all.
struct NativeFTransform {
    double rotX = 0.0, rotY = 0.0, rotZ = 0.0, rotW = 1.0;
    double locX = 0.0, locY = 0.0, locZ = 0.0, locPad = 0.0;
    double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0, scalePad = 0.0;
};

// UE4SS.dll's own UWorld::SpawnActor wrapper unconditionally returns nullptr
// on this build — live IDA tracing (research/04_ida_investigation_log.md
// Session 40) followed its real call chain (UWorld::SpawnActor ->
// RC::Unreal::UGameplayStatics::BeginDeferredActorSpawnFromClass) down into
// an internal UE4SS reflection/type-cache lookup (FNV-1a hash + hashtable
// probe) that never resolves, leaving the result permanently null — confirmed
// live for every real spawn_proxy() attempt this session, independent of
// class or world validity. Bypasses that broken wrapper entirely by calling
// the game's own native engine function directly (resolved by RVA, same
// pattern as get_class_private() above). This native
// BeginDeferredActorSpawnFromClass was separately verified via full
// Hex-Rays decompile: it resolves WorldContextObject -> UWorld via
// UEngine::GetWorldFromContextObject, then calls the real, stock
// UWorld::SpawnActor (confirmed 100% vanilla UE5 with no SurrounDead-specific
// gate) with bDeferConstruction=true.
static void* call_begin_deferred_spawn(void* world_context, void* actor_class,
                                        const NativeFTransform* xform)
{
    // (WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride,
    //  Owner, <unidentified trailing byte, always 0 for our use>)
    using Fn = void*(__fastcall*)(void*, void*, const NativeFTransform*, char, void*, char);
    static Fn fn = reinterpret_cast<Fn>(
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)) + 0x2E80E80);
    return fn(world_context, actor_class, xform, 0, nullptr, 0);
}

// AActor::FinishSpawning()'s real native implementation — identified via its
// signature match to UE5 source (a one-time bHasFinishedSpawning-style guard
// flag at offset+92, then a full parent-relative transform composition before
// applying it), found as the callee of the Kismet exec thunk for the
// "Finish Spawning Actor" node. Real UGameplayStatics::FinishSpawningActor
// is just `Actor->FinishSpawning(...); return Actor;` — FinishSpawning
// itself is void in UE5 source, so its return value here is not used.
static void call_finish_spawning(void* actor, const NativeFTransform* xform)
{
    // (this=Actor, SpawnTransform, bIsDefaultTransform, InstanceDataCache, TransformScaleMethod)
    using Fn = void*(__fastcall*)(void*, const NativeFTransform*, char, void*, char);
    static Fn fn = reinterpret_cast<Fn>(
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)) + 0x2AAAB90);
    fn(actor, xform, 0, nullptr, 0);
}

// UObject::GetClassPrivate() in the vendored UE4SS stub header is declared
// on the wrong class for its own mangled name (the header's comment shows
// the true export as `UObjectBase::GetClassPrivate`, but the C++ declaration
// puts it on `UObject`, so the import lib entry the linker generates never
// matches the DLL's actual export and this fails LNK2019). Resolved by
// address instead, same pattern already used in mod.cpp for the two
// RegisterXPreCallback exports that aren't in the stub at all. MSVC returns
// a reference (UClass*&) as a pointer to the referenced storage in RAX, so
// the resolved signature returns UClass** here, not UClass*.
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

// ── Equipment sync: FGameplayTag construction & item-asset lookup ─────────
// research/04_ida_investigation_log.md Session 43/44. GetEquippedInfoBySlot/
// SetEquippedInfoBySlot are Blueprint-authored (BP_JigHelperComp_C), so they
// have to go through UObject::GetFunctionByNameInChain + ProcessEvent (the
// vendored wrappers, called exactly like the working example at
// mod.cpp:847's try_open_world()) rather than a by-address native bypass.
//
// Session 44 spent three live attempts trying to call GetFunctionByNameInChain
// via manual register-hijacking from the IDA debugger and crashed every time;
// disassembly of its real entry point found a global lazy-init flag that only
// gets set up correctly when the function is reached through its normal
// caller — a manually hijacked call has no way to replicate that state. Real
// compiled C++ (this file) goes through the normal path and does not have
// this problem, matching mod.cpp:847's already-working call.
namespace equip_native {

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

// Same FName::ToString / FMemory::Free pair as mod.cpp's native::fname_to_string
// (research/04_ida_investigation_log.md Session 9/34) — duplicated here
// rather than shared across translation units, matching this file's existing
// pattern of resolving its own native addresses independently (see
// call_begin_deferred_spawn/get_class_private above).
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

} // namespace equip_native

// Raw FGameplayTag = plain 8-byte FName{int32 ComparisonIndex; int32 Number}
// (research/04_ida_investigation_log.md Session 43 — confirmed by reading
// BP_JigHelperComp_C.ActiveWeapon, a bare FGameplayTag field, directly as
// this layout).
struct RawFGameplayTag { int32_t ComparisonIndex = 0; int32_t Number = 0; };

// Raw FRepItemInfo, matching research/CXXHeaderDump/RepItemInfo.hpp exactly
// (0x78 bytes total). Only ItemID and Count are populated for proxy
// appearance sync — the rest (weight/price/durability/stats/custom data)
// aren't tracked for remote players and are left zeroed, which is fine for a
// purely cosmetic, non-interactable proxy actor.
struct RawFRepItemInfo {
    void*    ItemID = nullptr;      // 0x00 UJigsawItem_DataAsset_C*
    int32_t  Count = 0;             // 0x08
    double   ItemVecX = 0, ItemVecY = 0; // 0x10 FVector2D
    double   Weight = 0;            // 0x20
    double   Price = 0;             // 0x28
    double   DurabilityX = 0, DurabilityY = 0; // 0x30 FVector2D
    uint64_t StatsPtr = 0; int32_t StatsNum = 0, StatsMax = 0; // 0x40 TArray<FS_ItemStat>
    double   Pending = 0;           // 0x50
    uint64_t CustomKeyPtr = 0; int32_t CustomKeyNum = 0, CustomKeyMax = 0;   // 0x58 TArray<FString>
    uint64_t CustomValPtr = 0; int32_t CustomValNum = 0, CustomValMax = 0;   // 0x68 TArray<FString>
};
static_assert(sizeof(RawFRepItemInfo) == 0x78, "RawFRepItemInfo must match FRepItemInfo's real layout");

struct RawFGuid { uint32_t A = 0, B = 0, C = 0, D = 0; };

// TODO(next IDA session, read-only + safe): fill in the real ComparisonIndex
// for every slot. Session 43 live-resolved all 21 real tag strings (FName::
// ToString round trip against BP_JigHelperComp_C.EquipmentIDSlotConfig,
// helper+0xAF8 — see "UE5 TMap/TSet raw memory layout" in
// research/04_ida_investigation_log.md for the TMap's raw layout) but only
// recorded the resolved strings, not the raw indices, and the game/IDA
// session ended (Session 44) before they could be re-extracted. To refill:
// reattach IDA's debugger to the running game, read the 21 TMap keys at
// helper+0xAF8, and call FName::ToString on each key (safe — proven 21/21
// with zero crashes in Session 43, unlike GetFunctionByNameInChain) to match
// against the slot order in protocol.hpp's EquipmentSlot comment. Only
// PrimaryWeapon is filled in below (confirmed live, Session 43). Slots with
// index 0 here will silently fail slot_tag() below (returns false) rather
// than send a bogus tag.
// All 21 real values, live-verified 2026-08-11 via a raw memory walk of
// BP_JigHelperComp_C.EquipmentIDSlotConfig (TMap<FGameplayTag,
// FS_EquipmentIDInfo> @helper+0xAF8) — the actual authoritative source
// SetEquippedInfoBySlot/GetEquippedInfoBySlot validate against, not the
// "Jig.PlayerSlot.*" tags found earlier by name-matching (which turned out
// to be a different, unrelated tag family used for the active-weapon-slot
// UI switching, not equipment slot identity). TSparseArray element stride
// empirically determined to be 28 bytes (8 more than the raw 8+12=20-byte
// TPair<FGameplayTag,FS_EquipmentIDInfo> — extra padding from the
// allocator, found by testing candidate strides against the modular
// distribution of plausible-CI hit offsets until one cleanly explained all
// 21 entries with zero stragglers). Replaces the single Primary value
// (1730659) recorded in an earlier, separate session — that number never
// matched this TMap's own PrimaryWeapon entry (1730576) even before tonight,
// so it was very likely from that same wrong tag family, not a real
// regression. Live-tested only as of this write — not yet confirmed to
// persist correctly for slots 0-10/12-20 the way slot 11 was in Session 45.
static constexpr int32_t kSlotTagComparisonIndex[EQUIPMENT_SLOT_COUNT] = {
    1730464, // 0  Facewear
    1730538, // 1  Headwear
    1730452, // 2  Eyewear
    1730374, // 3  Accessory
    1730635, // 4  Torso
    1730516, // 5  Gloves
    1730551, // 6  Legs
    1730477, // 7  Feet
    1730439, // 8  Container
    1730414, // 9  BodyArmor
    1730387, // 10 Backpack
    1730576, // 11 Primary
    1730591, // 12 Secondary
    1730607, // 13 Sidearm
    1730562, // 14 Melee
    1730622, // 15 Throwable
    1730502, // 16 Flashlight
    1730400, // 17 Binoculars
    1730528, // 18 GPS
    1730427, // 19 Compass
    1730488, // 20 FishingRod
};

// Returns false (and leaves *out untouched) for any slot whose
// ComparisonIndex hasn't been filled in above yet.
static bool slot_tag(uint8_t slotIndex, RawFGameplayTag& out)
{
    if (slotIndex >= EQUIPMENT_SLOT_COUNT) return false;
    const int32_t ci = kSlotTagComparisonIndex[slotIndex];
    if (ci == 0) return false;
    out = RawFGameplayTag{ ci, 0 };
    return true;
}

// Caches UJigsawItem_DataAsset_C* by itemId (its FName at +0x30, read the
// same way as mod.cpp's read_local_equipment()/read_local_inventory() decode
// it) so equipment writes can turn a wire itemId string back into a live
// pointer. Data assets are loaded once and never destroyed during play, so a
// one-time scan is enough — rebuilt lazily on first lookup miss in case new
// assets stream in later.
static std::unordered_map<std::string, void*>& item_asset_cache()
{
    static std::unordered_map<std::string, void*> cache;
    return cache;
}

static void rebuild_item_asset_cache()
{
    std::vector<UObject*> assets;
    UObjectGlobals::FindAllOf(L"JigsawItem_DataAsset_C", assets);

    auto& cache = item_asset_cache();
    cache.clear();
    cache.reserve(assets.size());
    for (UObject* obj : assets) {
        const auto addr = reinterpret_cast<uintptr_t>(obj);
        std::string itemId = equip_native::fname_to_string(addr + 0x30);
        if (!itemId.empty()) cache.emplace(std::move(itemId), obj);
    }
}

// Returns nullptr for an empty itemId (unequip) or an itemId not found in
// the cache (also nullptr — a missing DataAsset means the item can't be
// visually equipped, not that the request should be dropped: SetEquippedInfoBySlot
// is still called with a null ItemID, matching how an empty slot looks in
// read_local_equipment()).
static void* resolve_item_asset(const std::string& itemId)
{
    if (itemId.empty()) return nullptr;

    auto& cache = item_asset_cache();
    if (cache.empty()) rebuild_item_asset_cache();

    auto it = cache.find(itemId);
    if (it == cache.end()) {
        // Retry once with a fresh scan in case this asset streamed in after
        // the cache was built.
        rebuild_item_asset_cache();
        it = cache.find(itemId);
    }
    return it != cache.end() ? it->second : nullptr;
}

// Confirmed live (2026-08-10): get_equipped_info_by_slot() returns ok=1 for
// slot 11 (Primary Weapon, the only slot with a real tag mapped so far) via
// real compiled code — GetFunctionByNameInChain + ProcessEvent succeed with
// no crash, unlike the manual IDA register-hijack attempts earlier the same
// day (research/04_ida_investigation_log.md Session 44). Enabling the write
// as the next incremental step; still only meaningful for slot 11 until the
// other 20 ComparisonIndex values are filled in (see kSlotTagComparisonIndex
// above) — slot_tag() safely no-ops for everything else.
static constexpr bool kEnableEquipmentWrite = true;

// BP_JigHelperComp_C.GetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo& Info, bool& Equipped) — read-only.
// Kismet params buffer layout (UHT sequential-alignment convention): Slot
// FGameplayTag @0x00 (align 4, size 8), Info FRepItemInfo @0x08 (align 8,
// size 0x78), Equipped bool @0x80 (align 1, size 1). Total 0x81, rounded up.
static bool get_equipped_info_by_slot(AActor* actor, uint8_t slotIndex,
                                       RawFRepItemInfo& outInfo, bool& outEquipped)
{
    if (!actor) return false;
    RawFGameplayTag tag;
    if (!slot_tag(slotIndex, tag)) return false;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return false;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"GetEquippedInfoBySlot");
    if (!fn) return false;

    struct Params {
        RawFGameplayTag Slot;
        RawFRepItemInfo Info;
        bool Equipped = false;
    } params;
    static_assert(offsetof(Params, Slot) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, Info) == 0x08, "Kismet param layout");
    static_assert(offsetof(Params, Equipped) == 0x80, "Kismet param layout");

    params.Slot = tag;
    helperObj->ProcessEvent(fn, &params);

    outInfo     = params.Info;
    outEquipped = params.Equipped;
    return true;
}

// BP_JigHelperComp_C.SetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo Info, FGuid UID, bool SkipUID) —
// the actual write. NOT called anywhere yet (see kEnableEquipmentWrite
// above) — implemented and ready, pending live verification of the getter
// above first. Buffer layout: Slot @0x00 (8), Info @0x08 (0x78), UID @0x80
// (align 4, size 0x10), SkipUID bool @0x90. SkipUID=true since remote
// players' items don't have a real FGuid known to this process — passing a
// zero UID with SkipUID=true asks the function not to validate/require a
// UID match, appropriate for a purely cosmetic proxy.
static bool set_equipped_info_by_slot(AActor* actor, uint8_t slotIndex, const std::string& itemId)
{
    if (!actor) return false;
    RawFGameplayTag tag;
    if (!slot_tag(slotIndex, tag)) return false;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return false;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"SetEquippedInfoBySlot");
    if (!fn) return false;

    struct Params {
        RawFGameplayTag Slot;
        RawFRepItemInfo Info;
        RawFGuid UID;
        bool SkipUID = true;
    } params;
    static_assert(offsetof(Params, Slot) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, Info) == 0x08, "Kismet param layout");
    static_assert(offsetof(Params, UID) == 0x80, "Kismet param layout");
    static_assert(offsetof(Params, SkipUID) == 0x90, "Kismet param layout");

    params.Slot = tag;
    params.Info.ItemID = resolve_item_asset(itemId);
    params.Info.Count  = params.Info.ItemID ? 1 : 0;

    {
        char buf[160];
        snprintf(buf, sizeof(buf),
                 "set_equipped_info_by_slot: itemId=%s resolved_ptr=0x%llx cache_size=%zu",
                 itemId.c_str(),
                 reinterpret_cast<unsigned long long>(params.Info.ItemID),
                 item_asset_cache().size());
        debug_log(buf);
    }

    helperObj->ProcessEvent(fn, &params);
    return true;
}

// BP_JigHelperComp_C.SetActiveWeaponSlot(FGameplayTag NewSlot) — SetEquippedInfoBySlot
// only updates which item occupies a slot; it doesn't touch the separate
// ActiveWeapon property that actually drives which weapon mesh is visually
// drawn (research/04_ida_investigation_log.md Session 43 found ActiveWeapon
// as a distinct FGameplayTag field, confirmed against a real drawn weapon).
// Live-tested 2026-08-10: SetEquippedInfoBySlot alone (ok=1, no crash) did
// NOT make the proxy visually hold the weapon — this is the follow-up call
// needed to actually trigger the visual attach via OnActiveWeaponSlotChanged.
// Single 8-byte FGameplayTag param, no output params.
static bool set_active_weapon_slot(AActor* actor, uint8_t slotIndex)
{
    if (!actor) return false;
    RawFGameplayTag tag;
    if (!slot_tag(slotIndex, tag)) return false;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return false;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"SetActiveWeaponSlot");
    if (!fn) return false;

    RawFGameplayTag params = tag;
    helperObj->ProcessEvent(fn, &params);
    return true;
}

// BP_JigHelperComp_C.OnRep_ActiveWeapon() — no parameters. Real networked
// players never call SetActiveWeaponSlot's visual effects directly; UE5's
// replication system calls this automatically on remote clients when the
// ActiveWeapon property changes over the network. Our proxy actor has no
// real replication happening (it's a locally-spawned stand-in, not a
// networked actor from the engine's point of view), so that automatic call
// never happens no matter what we set the property to — has to be invoked
// manually to run the same spawn/attach-visual-mesh logic a real replicated
// change would trigger.
//
// Live-tested 2026-08-10 (two-client test, PC1 AK15 -> PC2's view of PC1's
// proxy): getter/setter/activate/onrep all report ok=1 but nothing visually
// attaches. Bytecode decode of OnRep_ActiveWeapon (112 bytes) explains why:
// it calls GetOwner()->HasAuthority(), and if true, returns immediately
// *without* broadcasting the OnActiveWeaponSlotChanged multicast delegate
// that actually drives the visual — the HasAuthority() branch is meant to
// distinguish server/client for a real replicated OnRep, which normally
// only ever fires client-side anyway. Our proxy is SpawnActor'd locally and
// isn't part of any replication graph, so it has local authority by
// definition, always taking the early-return branch. Calling this function
// is therefore a guaranteed no-op for a proxy — kept here for reference/
// documentation, superseded by call_on_active_weapon_slot_changed below,
// which calls the delegate's bound handler directly and skips this gate.
static bool call_on_rep_active_weapon(AActor* actor)
{
    if (!actor) return false;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return false;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"OnRep_ActiveWeapon");
    if (!fn) return false;

    helperObj->ProcessEvent(fn, nullptr);
    return true;
}

// BP_PlayerCharacter_C.PrimaryWeaponEquipped? — plain bool @0x1DC0 directly on
// the character actor (not through the +0x700 helper), with its own
// OnRep_PrimaryWeaponEquipped? (913 bytes, decoded 2026-08-10). Unlike
// OnRep_ActiveWeapon, this one has no HasAuthority() gate — it branches
// directly on the property's own current value (JumpIfNot at the very top),
// taking a real visual-setup path (interface casts, calls referencing our
// known Primary-slot tag literal 1730659) only when true. We've never
// written this property from any existing call, so on a fresh proxy it's
// still false, meaning even calling the OnRep would hit the empty/cleanup
// branch — has to be set directly first.
static bool set_primary_weapon_equipped(AActor* actor, bool value)
{
    if (!actor) return false;
    *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(actor) + 0x1DC0) = value;
    return true;
}

// BP_PlayerCharacter_C.GetCurrentActiveWeapon(AActor*& EquippedWeapon) —
// read-only diagnostic. Single AActor*-sized out param. Added to check
// whether anything downstream of our whole write chain (setter, activate,
// onrep, notify, PrimaryWeaponEquipped set+onrep — all reporting ok=1) ever
// actually produces a real weapon actor reference, or whether the chain is
// silently no-op'ing on some other missing piece of proxy setup.
static AActor* get_current_active_weapon(AActor* actor)
{
    if (!actor) return nullptr;
    UFunction* fn = actor->GetFunctionByNameInChain(L"GetCurrentActiveWeapon");
    if (!fn) return nullptr;
    AActor* out = nullptr;
    actor->ProcessEvent(fn, &out);
    return out;
}

// BP_JigHelperComp_C.GetActiveWeapon(AActor*& Actor) and GetEquippedActorBySlot
// (research/CXXHeaderDump/BP_JigHelperComp.hpp) — both read-only diagnostics,
// on the +0x700 helper component rather than the character actor. Checking
// whether an equipped-actor reference exists *here* (the helper's own
// bookkeeping, populated by HandleActorEquipped per Session 46's decode) even
// though GetCurrentActiveWeapon on the character itself returned null.
static AActor* get_helper_active_weapon(AActor* actor)
{
    if (!actor) return nullptr;
    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return nullptr;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"GetActiveWeapon");
    if (!fn) return nullptr;
    AActor* out = nullptr;
    helperObj->ProcessEvent(fn, &out);
    return out;
}

// Params: Slot @0x00 (FGameplayTag, 8), IncludePrimitive? @0x08 (bool, align
// 1), Actor @0x10 (AActor*, align 8 — 7 bytes padding after the bool), Array
// Index @0x18 (int32). Same UHT sequential-alignment convention as the other
// helper functions above.
static AActor* get_helper_equipped_actor_by_slot(AActor* actor, uint8_t slotIndex)
{
    if (!actor) return nullptr;
    RawFGameplayTag tag;
    if (!slot_tag(slotIndex, tag)) return nullptr;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return nullptr;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"GetEquippedActorBySlot");
    if (!fn) return nullptr;

    struct Params {
        RawFGameplayTag Slot;
        bool IncludePrimitive = false;
        AActor* Actor = nullptr;
        int32_t ArrayIndex = 0;
    } params;
    static_assert(offsetof(Params, Slot) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, IncludePrimitive) == 0x08, "Kismet param layout");
    static_assert(offsetof(Params, Actor) == 0x10, "Kismet param layout");
    static_assert(offsetof(Params, ArrayIndex) == 0x18, "Kismet param layout");

    params.Slot = tag;
    helperObj->ProcessEvent(fn, &params);
    return params.Actor;
}

// JigsawItem_DataAsset_C.PickupClass (TSubclassOf<AActor> @0x0128) and
// .EquipSocket (FName @0x0280) — research/CXXHeaderDump/JigsawItem_DataAsset.hpp.
// Session 46 conclusively found (three independent null getters —
// GetCurrentActiveWeapon, the helper's GetActiveWeapon, GetEquippedActorBySlot
// — after a fully successful setter/activate/onrep/notify/PrimaryWeaponEquipped
// chain) that the real equip-visual pipeline requires an actor that was
// already physically picked up from the world; a proxy synced purely from
// network data never has one, and simulating that whole pickup lifecycle just
// for a cosmetic stand-in isn't worth it. This sidesteps the real pipeline
// entirely: spawns the item's own designated pickup Blueprint directly (reusing
// the exact BeginDeferredActorSpawnFromClass/FinishSpawning native-call pattern
// already proven for spawn_proxy() above) and attaches it straight to the
// proxy's equip socket via the real K2_AttachToActor, still reflection-called
// like everything else in this file. Not yet live-verified — first real test
// of this whole function.
static AActor* spawn_and_attach_weapon_visual(AActor* actor, void* itemAsset)
{
    if (!actor || !itemAsset) return nullptr;

    auto* pickupClass = *reinterpret_cast<UClass**>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x128);
    if (!pickupClass) {
        debug_log("spawn_and_attach_weapon_visual: PickupClass is null on item asset");
        return nullptr;
    }

    {
        std::wstring wname = reinterpret_cast<UObject*>(pickupClass)->GetFullName();
        const int needed = WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string name(needed > 0 ? static_cast<size_t>(needed - 1) : 0, '\0');
        if (needed > 0) WideCharToMultiByte(CP_UTF8, 0, wname.c_str(), -1, name.data(), needed, nullptr, nullptr);
        debug_log("spawn_and_attach_weapon_visual: PickupClass=" + name);
    }

    UWorld* world = actor->GetWorld();
    if (!world) {
        debug_log("spawn_and_attach_weapon_visual: actor->GetWorld() is null");
        return nullptr;
    }

    NativeFTransform xform; // identity — final placement comes from the attach below
    void* pending = call_begin_deferred_spawn(world, pickupClass, &xform);
    if (!pending) {
        debug_log("spawn_and_attach_weapon_visual: BeginDeferredActorSpawnFromClass returned null");
        return nullptr;
    }
    call_finish_spawning(pending, &xform);

    auto* weaponActor = static_cast<AActor*>(pending);
    weaponActor->SetActorEnableCollision(false);

    const RawFGameplayTag socket = *reinterpret_cast<RawFGameplayTag*>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x280);
    {
        char buf[160];
        snprintf(buf, sizeof(buf),
                 "spawn_and_attach_weapon_visual: spawned=0x%llx socket_ci=%d socket_num=%d",
                 reinterpret_cast<unsigned long long>(weaponActor),
                 socket.ComparisonIndex, socket.Number);
        debug_log(buf);
    }

    // Session 47: abandoning JigMP_OnPickupEquipped entirely — it explicitly
    // rejected a synthetic call (Result=false) with no way to know what it's
    // validating, and K2_AttachToActor alone (root-to-root) already showed
    // nothing visually even before that was added, so it was never the
    // blocker. Root cause of the "still invisible" symptom is more likely
    // that K2_AttachToActor attaches relative to the *actor's* root
    // component (probably a capsule) — "Weapon_r" is a bone socket that
    // almost certainly only exists on a specific skeletal mesh component,
    // not the capsule. research/CXXHeaderDump/BP_PlayerCharacter.hpp lists
    // several per-body-part USkeletalMeshComponent* fields; "Arms" (@0x0788)
    // is the most plausible owner of hand/weapon-hold bones. Attaching the
    // weapon's own root *component* directly to that specific component
    // (component-to-component, not actor-to-actor) via the same-era
    // K2_AttachTo (component-level twin of K2_AttachToActor, same param
    // shape) instead. Not yet live-verified.
    auto* armsComp = *reinterpret_cast<UObject**>(
        reinterpret_cast<uintptr_t>(actor) + 0x788);
    if (!armsComp) {
        debug_log("spawn_and_attach_weapon_visual: proxy's Arms component is null");
        return weaponActor;
    }

    UFunction* getRootFn = weaponActor->GetFunctionByNameInChain(L"GetRootComponent");
    if (!getRootFn) {
        debug_log("spawn_and_attach_weapon_visual: GetRootComponent NOT FOUND");
        return weaponActor;
    }
    UObject* weaponRoot = nullptr;
    weaponActor->ProcessEvent(getRootFn, &weaponRoot);
    if (!weaponRoot) {
        debug_log("spawn_and_attach_weapon_visual: weapon actor's root component is null");
        return weaponActor;
    }

    UFunction* fn = weaponRoot->GetFunctionByNameInChain(L"K2_AttachTo");
    debug_log(fn ? "spawn_and_attach_weapon_visual: K2_AttachTo found"
                 : "spawn_and_attach_weapon_visual: K2_AttachTo NOT FOUND");
    if (fn) {
        struct Params {
            UObject*        InParent = nullptr;
            RawFGameplayTag InSocketName;              // FName is the same 8-byte shape
            uint8_t         AttachLocationType = 3;     // EAttachLocation::SnapToTarget
            bool            WeldSimulatedBodies = false;
        } params;
        static_assert(offsetof(Params, InParent) == 0x00, "Kismet param layout");
        static_assert(offsetof(Params, InSocketName) == 0x08, "Kismet param layout");
        static_assert(offsetof(Params, AttachLocationType) == 0x10, "Kismet param layout");
        static_assert(offsetof(Params, WeldSimulatedBodies) == 0x11, "Kismet param layout");

        params.InParent = armsComp;
        params.InSocketName = socket;
        weaponRoot->ProcessEvent(fn, &params);
    }

    return weaponActor;
}

static bool call_on_rep_primary_weapon_equipped(AActor* actor)
{
    if (!actor) return false;
    UFunction* fn = actor->GetFunctionByNameInChain(L"OnRep_PrimaryWeaponEquipped?");
    if (!fn) return false;
    actor->ProcessEvent(fn, nullptr);
    return true;
}

// BP_PlayerCharacter_C.OnActiveWeaponSlotChanged_Event_0(FGameplayTag Slot) —
// the actual bound handler for BP_JigHelperComp_C's OnActiveWeaponSlotChanged
// multicast delegate (research/CXXHeaderDump/BP_PlayerCharacter.hpp), found
// by resolving OnRep_ActiveWeapon's own bytecode (see call_on_rep_active_weapon
// above) and cross-referencing the delegate name against the header dump.
// Lives on the character/proxy actor itself, NOT the +0x700 helper component
// — calling it directly skips OnRep_ActiveWeapon's HasAuthority() gate
// entirely, since we're invoking the delegate's consumer rather than the
// (never-fired, for a proxy) delegate broadcast itself. Single FGameplayTag
// param, no output params — same shape as set_active_weapon_slot.
static bool call_on_active_weapon_slot_changed(AActor* actor, uint8_t slotIndex)
{
    if (!actor) return false;
    RawFGameplayTag tag;
    if (!slot_tag(slotIndex, tag)) return false;

    UFunction* fn = actor->GetFunctionByNameInChain(L"OnActiveWeaponSlotChanged_Event_0");
    if (!fn) return false;

    RawFGameplayTag params = tag;
    actor->ProcessEvent(fn, &params);
    return true;
}

// Output::send's format strings are wide (STR macro); widen itemId (UTF-8)
// before interpolating it so as not to depend on the fmt backend's narrow/
// wide-argument mixing behavior, which no existing call site in this project
// relies on.
static std::wstring widen(const std::string& s)
{
    if (s.empty()) return {};
    const int needed = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(needed), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), w.data(), needed);
    return w;
}

void ProxyManager::sync_equipment(AActor* actor, RemotePlayer& player)
{
    if (!actor || !player.equipmentDirty) return;
    player.equipmentDirty = false;

    // KNOWN GAP: only iterates slots present in the latest Equipment frame
    // (the wire format omits empty slots entirely — on_equipment()'s old
    // comment about this being fine assumed equipment was just a cached map,
    // never pushed onto a live actor). A slot that becomes unequipped won't
    // appear here, so nothing clears it on the proxy — it'll keep showing
    // whatever was last set. Needs a per-player "previously applied slots"
    // diff before kEnableEquipmentWrite is safe to flip; not solved here
    // since none of this has been live-verified yet either.

    // One-shot verification pass: for every slot we're about to touch, read
    // it back first via the safe, read-only getter and log the result. This
    // never calls the write path — see kEnableEquipmentWrite above for why.
    for (const auto& slot : player.equipment) {
        RawFRepItemInfo info{};
        bool equipped = false;
        const bool ok = get_equipped_info_by_slot(actor, slot.slotIndex, info, equipped);
        Output::send<LogLevel::Normal>(
            STR("SDB: equip-getter slot={:d} itemId={} ok={:d} equipped={:d}\n"),
            slot.slotIndex, widen(slot.itemId), ok, equipped);

        if (kEnableEquipmentWrite) {
            const bool wrote = set_equipped_info_by_slot(actor, slot.slotIndex, slot.itemId);
            Output::send<LogLevel::Normal>(
                STR("SDB: equip-setter slot={:d} itemId={} ok={:d}\n"),
                slot.slotIndex, widen(slot.itemId), wrote);

            // SetEquippedInfoBySlot alone doesn't visually draw the weapon —
            // ActiveWeapon is a separate property (see set_active_weapon_slot
            // above). Only meaningful for the four weapon slots; wire protocol
            // order per protocol.hpp's EquipmentSlot comment (11 Primary,
            // 12 Secondary, 13 Sidearm, 14 Melee).
            if (wrote && slot.slotIndex >= 11 && slot.slotIndex <= 14) {
                const bool activated = set_active_weapon_slot(actor, slot.slotIndex);
                Output::send<LogLevel::Normal>(
                    STR("SDB: equip-activate slot={:d} itemId={} ok={:d}\n"),
                    slot.slotIndex, widen(slot.itemId), activated);

                if (activated) {
                    const bool repped = call_on_rep_active_weapon(actor);
                    Output::send<LogLevel::Normal>(
                        STR("SDB: equip-onrep slot={:d} itemId={} ok={:d}\n"),
                        slot.slotIndex, widen(slot.itemId), repped);

                    // call_on_rep_active_weapon is a guaranteed no-op on a
                    // proxy (HasAuthority() gate — see its comment above);
                    // this is the call that actually reaches the visual
                    // attach, by invoking the delegate's bound handler
                    // directly instead of the delegate broadcast itself.
                    const bool notified = call_on_active_weapon_slot_changed(actor, slot.slotIndex);
                    Output::send<LogLevel::Normal>(
                        STR("SDB: equip-notify slot={:d} itemId={} ok={:d}\n"),
                        slot.slotIndex, widen(slot.itemId), notified);

                    // PrimaryWeaponEquipped? is specifically the Primary slot
                    // (slot 11) — only the slot with a real tag mapped so far
                    // anyway (see kSlotTagComparisonIndex), so this is a no-op
                    // for 12-14 today regardless, but gated by slot number to
                    // stay semantically correct once the other tags are filled in.
                    if (slot.slotIndex == 11) {
                        const bool setEquipped = set_primary_weapon_equipped(actor, !slot.itemId.empty());
                        const bool reppedPwe = setEquipped && call_on_rep_primary_weapon_equipped(actor);
                        Output::send<LogLevel::Normal>(
                            STR("SDB: equip-pwe slot={:d} itemId={} set={:d} rep={:d}\n"),
                            slot.slotIndex, widen(slot.itemId), setEquipped, reppedPwe);

                        AActor* activeWeapon = get_current_active_weapon(actor);
                        AActor* helperActiveWeapon = get_helper_active_weapon(actor);
                        AActor* equippedActorBySlot = get_helper_equipped_actor_by_slot(actor, slot.slotIndex);
                        Output::send<LogLevel::Normal>(
                            STR("SDB: equip-diag slot={:d} itemId={} activeWeaponPtr=0x{:x} helperActiveWeaponPtr=0x{:x} equippedActorBySlotPtr=0x{:x}\n"),
                            slot.slotIndex, widen(slot.itemId),
                            reinterpret_cast<uintptr_t>(activeWeapon),
                            reinterpret_cast<uintptr_t>(helperActiveWeapon),
                            reinterpret_cast<uintptr_t>(equippedActorBySlot));

                        // Direct visual spawn+attach, bypassing the real
                        // (proven non-functional for a proxy) equip pipeline
                        // entirely — see spawn_and_attach_weapon_visual's
                        // comment. Respawn only when the equipped item
                        // actually changes, not every sync_equipment() call.
                        if (slot.itemId != player.primaryWeaponVisualItemId) {
                            if (player.primaryWeaponVisualActor) {
                                static_cast<AActor*>(player.primaryWeaponVisualActor)->K2_DestroyActor();
                                player.primaryWeaponVisualActor = nullptr;
                            }
                            void* itemAsset = resolve_item_asset(slot.itemId);
                            AActor* visual = itemAsset
                                ? spawn_and_attach_weapon_visual(actor, itemAsset)
                                : nullptr;
                            player.primaryWeaponVisualActor = visual;
                            player.primaryWeaponVisualItemId = slot.itemId;
                            Output::send<LogLevel::Normal>(
                                STR("SDB: equip-visual slot={:d} itemId={} spawnedPtr=0x{:x}\n"),
                                slot.slotIndex, widen(slot.itemId),
                                reinterpret_cast<uintptr_t>(visual));
                        }
                    }
                }
            }
        }
    }
}

void ProxyManager::init()
{
    initialized_ = true;
}

void ProxyManager::on_player_connected(uint64_t playerId)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto& p   = g_state().players[playerId];
    p.playerId = playerId;
    Output::send<LogLevel::Normal>(STR("SDB: remote player connected {:d}\n"), playerId);
}

void ProxyManager::on_player_disconnected(uint64_t playerId)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    if (it->second.proxyActor)
        destroy_proxy(static_cast<AActor*>(it->second.proxyActor));

    g_state().players.erase(it);
    Output::send<LogLevel::Normal>(STR("SDB: remote player disconnected {:d}\n"), playerId);
}

void ProxyManager::on_movement(uint64_t playerId, const Movement& m)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    auto& p     = it->second;
    p.x         = m.x;
    p.y         = m.y;
    p.z         = m.z;
    p.yaw       = m.yaw;
    p.aimYaw    = m.aimYaw;
    p.movState  = m.movementState;
    p.animState = m.animationState;
    p.updatedUs = now_micros();
}

void ProxyManager::on_equipment(uint64_t playerId, const Equipment& e)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    it->second.equipment = e.slots;
    it->second.equipmentDirty = true;
}

void ProxyManager::tick(UWorld* world, AActor* /*local_pawn*/)
{
    if (!initialized_) return;

    std::lock_guard<std::mutex> lock(g_state().playersMtx);

    for (auto& [id, player] : g_state().players) {
        if (!player.proxyActor) {
            // Retry at most every 5s on failure — do_game_tick() calls
            // tick() many times per second, and a failing spawn_proxy() with
            // no cooldown here was a real bug (Session 36): every failed
            // SpawnActor call on every tick tanked the frame rate even
            // though each individual call failed cleanly rather than crashing.
            const uint64_t now = now_micros();
            if (world && now - player.lastSpawnAttemptUs >= 5'000'000ULL) {
                player.lastSpawnAttemptUs = now;
                player.proxyActor = spawn_proxy(world,
                                                player.x, player.y, player.z,
                                                player.yaw);
            }
            continue;
        }

        if (!player.dead)
            teleport_proxy(static_cast<AActor*>(player.proxyActor),
                           player.x, player.y, player.z, player.yaw);

        sync_equipment(static_cast<AActor*>(player.proxyActor), player);
    }
}

void ProxyManager::teleport_proxy(AActor* actor, float x, float y, float z, float yaw)
{
    if (!actor) return;
    FVector  loc{ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
    FRotator rot{ 0.0, static_cast<double>(yaw), 0.0 };
    FHitResult hit{};
    actor->K2_SetActorLocationAndRotation(loc, rot, false, hit, true);
}

AActor* ProxyManager::spawn_proxy(UWorld* world, float x, float y, float z, float yaw)
{
    if (!world) return nullptr;

    // NOT WORKING YET (Session 36): both BP_PlayerCharacter_C and BP_Zombie_C
    // were rejected by SpawnActor 100% of the time in live testing (no crash,
    // clean nullptr every call). Zombies are definitely not CLASS_NotPlaceable
    // — the game's own spawner places them constantly — so that rules out the
    // leading hypothesis from Session 5's SpawnActor validation checklist and
    // points at something more fundamental in how this UE4SS SpawnActor
    // wrapper is being called, not which class is targeted. Needs IDA-level
    // investigation of the actual export before another live attempt is
    // worth making — see research/04_ida_investigation_log.md Session 36.
    // Left targeting BP_PlayerCharacter_C (the real desired proxy visual)
    // since the class choice isn't the blocker.
    static UClass* s_proxy_class = nullptr;
    if (!s_proxy_class) {
        UObject* obj = UObjectGlobals::FindFirstOf(STR("BP_PlayerCharacter_C"));
        if (obj) s_proxy_class = get_class_private(obj);
    }
    if (!s_proxy_class) return nullptr;

    // Pitch/roll are 0, so the FRotator->FQuat conversion collapses to a pure
    // Z-axis rotation: (0, 0, sin(yaw/2), cos(yaw/2)).
    const double yawRad = static_cast<double>(yaw) * (3.14159265358979323846 / 180.0);
    NativeFTransform xform;
    xform.rotZ = std::sin(yawRad * 0.5);
    xform.rotW = std::cos(yawRad * 0.5);
    xform.locX = static_cast<double>(x);
    xform.locY = static_cast<double>(y);
    xform.locZ = static_cast<double>(z);

    // world doubles as its own WorldContextObject: UWorld::GetWorld() (the
    // virtual call BeginDeferredActorSpawnFromClass resolves it through)
    // trivially returns `this` — confirmed live during the same trace.
    void* pending = call_begin_deferred_spawn(world, s_proxy_class, &xform);
    if (!pending) {
        Output::send<LogLevel::Warning>(STR("SDB: proxy spawn failed\n"));
        return nullptr;
    }

    call_finish_spawning(pending, &xform);

    // BP_PlayerCharacter_C's own death handling (loot-crate spawn, death-location
    // UI, input lock) turns out to be keyed off any instance's health reaching
    // zero, not off which instance is actually possessed by the local
    // PlayerController — the Blueprint was never designed to have a second,
    // locally-spawned instance coexisting. Live-confirmed 2026-08-10: killing a
    // proxy actor triggered the real player's own death sequence (their loot
    // crate, their death-location marker) while their own pawn remained
    // standing, frozen. The proxy is purely cosmetic and never needs to take
    // damage, so disabling its collision prevents any damage trace from ever
    // landing on it in the first place, sidestepping the whole class of bug
    // rather than trying to patch the Blueprint's death logic itself.
    static_cast<AActor*>(pending)->SetActorEnableCollision(false);

    Output::send<LogLevel::Normal>(STR("SDB: proxy spawned\n"));
    return static_cast<AActor*>(pending);
}

void ProxyManager::destroy_proxy(AActor* actor)
{
    if (!actor) return;
    actor->K2_DestroyActor();
}

} // namespace sdb
