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

// Shared spawn helper — same begin/finish pattern as spawn_proxy() below,
// factored out so entity_manager.cpp (world-entity ground pickups) doesn't
// need its own copy of the raw RVA-offset native calls.
AActor* spawn_actor_at(UWorld* world, UClass* actorClass, float x, float y, float z, float yaw)
{
    if (!world || !actorClass) return nullptr;

    const double yawRad = static_cast<double>(yaw) * (3.14159265358979323846 / 180.0);
    NativeFTransform xform;
    xform.rotZ = std::sin(yawRad * 0.5);
    xform.rotW = std::cos(yawRad * 0.5);
    xform.locX = static_cast<double>(x);
    xform.locY = static_cast<double>(y);
    xform.locZ = static_cast<double>(z);

    void* pending = call_begin_deferred_spawn(world, actorClass, &xform);
    if (!pending) return nullptr;

    call_finish_spawning(pending, &xform);
    return static_cast<AActor*>(pending);
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
void* resolve_item_asset(const std::string& itemId)
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

static std::string narrow(const std::wstring& w)
{
    if (w.empty()) return {};
    const int needed = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string s(needed > 0 ? static_cast<size_t>(needed - 1) : 0, '\0');
    if (needed > 0) WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, s.data(), needed, nullptr, nullptr);
    return s;
}

// See proxy_manager.hpp. UDataTableFunctionLibrary::GetDataTableRowNames
// (research/CXXHeaderDump/Engine.hpp) is a plain BlueprintCallable library
// function — any live instance works as the ProcessEvent target since library
// functions take all real data via explicit params, so the CDO (found the
// same way every other CDO/singleton lookup in this file works, via
// FindFirstOf matching the class's own name) is enough.
void dump_clothing_table(const wchar_t* tableName)
{
    // FindFirstOf/FindAllOf match by *class* name, not instance/asset name —
    // every DataTable asset shares the class "DataTable" (confirmed live:
    // FindFirstOf(L"DT_Clothing") found nothing even though the asset exists
    // at /Game/PlayerModel/DT_Clothing.DT_Clothing), so the target table has
    // to be found by scanning all DataTable instances and matching the
    // trailing ".<Name>" component of GetFullName() instead.
    std::vector<UObject*> tables;
    UObjectGlobals::FindAllOf(L"DataTable", tables);

    const std::string wanted = narrow(tableName);
    UObject* table = nullptr;
    for (UObject* t : tables) {
        std::string full = narrow(t->GetFullName());
        const auto dot = full.find_last_of('.');
        std::string shortName = (dot == std::string::npos) ? full : full.substr(dot + 1);
        if (shortName == wanted) { table = t; break; }
    }

    if (!table) {
        debug_log("dump_clothing_table: table \"" + wanted + "\" not found; listing all " +
                   std::to_string(tables.size()) + " loaded DataTable instances");
        for (UObject* t : tables)
            debug_log("  table: " + narrow(t->GetFullName()));
        return;
    }

    // FindFirstOf(L"DataTableFunctionLibrary") found nothing live-tested
    // 2026-08-12 — plausibly filtered as a CDO-only class (Blueprint function
    // libraries are essentially never instantiated beyond their CDO). Try
    // FindAllOf (in case it doesn't share FindFirstOf's filtering) and the
    // U-prefixed spelling before giving up.
    UObject* lib = UObjectGlobals::FindFirstOf(L"DataTableFunctionLibrary");
    if (!lib) {
        std::vector<UObject*> libs;
        UObjectGlobals::FindAllOf(L"DataTableFunctionLibrary", libs);
        debug_log("dump_clothing_table: FindFirstOf(DataTableFunctionLibrary) null; FindAllOf found " +
                   std::to_string(libs.size()));
        if (!libs.empty()) lib = libs.front();
    }
    if (!lib) {
        lib = UObjectGlobals::FindFirstOf(L"UDataTableFunctionLibrary");
        debug_log(std::string("dump_clothing_table: FindFirstOf(UDataTableFunctionLibrary) ") +
                   (lib ? "found" : "also null"));
    }
    if (!lib) {
        debug_log("dump_clothing_table: no DataTableFunctionLibrary instance found by any name");
        return;
    }
    UFunction* fn = lib->GetFunctionByNameInChain(L"GetDataTableRowNames");
    if (!fn) {
        debug_log("dump_clothing_table: GetDataTableRowNames not found");
        return;
    }

    // TArray<FName>, same raw {Data;Num;Max} shape as every other TArray in
    // this codebase (see UnrealFString above) — FName elements are the same
    // 8-byte RawFGameplayTag shape used throughout this file.
    struct RawTArrayFName { RawFGameplayTag* Data = nullptr; int32_t Num = 0; int32_t Max = 0; };
    struct Params { UObject* Table = nullptr; RawTArrayFName OutRowNames; } params;
    static_assert(offsetof(Params, Table) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, OutRowNames) == 0x08, "Kismet param layout");

    params.Table = table;
    lib->ProcessEvent(fn, &params);

    debug_log("dump_clothing_table: row count=" + std::to_string(params.OutRowNames.Num));
    std::unordered_map<std::string, bool> rowNames;
    for (int32_t i = 0; i < params.OutRowNames.Num; ++i) {
        std::string row = equip_native::fname_to_string(
            reinterpret_cast<uintptr_t>(&params.OutRowNames.Data[i]));
        rowNames.emplace(row, true);
        debug_log("  row: " + row);
    }

    // Every loaded item DataAsset's ItemId, filtered to ones that look
    // wearable (ClothingSettings.BodyPartSettings @ +0x448 has at least one
    // non-null Torso mesh — research/CXXHeaderDump/JigsawItem_DataAsset.hpp
    // ClothingSettings@0x430, BodyPartSettings@0x18 within it, MaleTorsoMesh@
    // 0x00 / FemaleTorsoMesh@0x40 within that), flagged against the row list.
    std::vector<UObject*> items;
    UObjectGlobals::FindAllOf(L"JigsawItem_DataAsset_C", items);
    debug_log("dump_clothing_table: scanning " + std::to_string(items.size()) + " item assets");
    for (UObject* obj : items) {
        const auto addr = reinterpret_cast<uintptr_t>(obj);
        std::string itemId = equip_native::fname_to_string(addr + 0x30);
        if (itemId.empty()) continue;

        void* maleTorso   = *reinterpret_cast<void**>(addr + 0x448);
        void* femaleTorso = *reinterpret_cast<void**>(addr + 0x448 + 0x40);
        if (!maleTorso && !femaleTorso) continue;

        const bool matches = rowNames.count(itemId) != 0;
        debug_log(std::string("  item: itemId=\"") + itemId + "\"" +
                  (matches ? "  [matches row]" : "  [** NO MATCHING ROW **]"));
    }
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

// BP_JigHelperComp_C.EquipActorToSocket(AActor* ActorRef, bool IsSecondary) —
// found by decompiling OnRep_FacewearEquipped?'s bytecode (research/
// 04_ida_investigation_log.md Session 49): the real game-native function
// that attaches an equipped item's actor for visual display, used for
// Facewear/Headwear/Eyewear/Accessory instead of a mesh-swap. Trying it here
// as a direct replacement for spawn_and_attach_weapon_visual's own manual
// K2_AttachTo dance, which mechanically succeeds but has never produced a
// visible weapon across six prior attempts — this is the actual function the
// game itself uses to attach an equipped actor, not a guess at engine-level
// attach primitives.
static bool equip_actor_to_socket(AActor* actor, AActor* itemActor, bool isSecondary)
{
    if (!actor || !itemActor) return false;

    const uintptr_t helper = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(actor) + 0x700);
    if (!helper) return false;

    auto* helperObj = reinterpret_cast<UObject*>(helper);
    UFunction* fn = helperObj->GetFunctionByNameInChain(L"Equip Actor to Socket");
    if (!fn) {
        debug_log("equip_actor_to_socket: EquipActorToSocket NOT FOUND");
        return false;
    }

    struct Params {
        AActor* ActorRef = nullptr;
        bool    IsSecondary = false;
    } params;
    static_assert(offsetof(Params, ActorRef) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, IsSecondary) == 0x08, "Kismet param layout");

    params.ActorRef = itemActor;
    params.IsSecondary = isSecondary;
    helperObj->ProcessEvent(fn, &params);
    debug_log("equip_actor_to_socket: called");
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

// Session 51: this used to be a separate, manual attach implementation
// (spawn_and_attach_weapon_visual — 11 attempts across 4 sessions, always
// ended with AttachParent NULL). Replaced by spawn_and_equip_item_visual
// below, the same function that fixed facewear — see its own comment for the
// full mechanism (ItemDataAsset population + the real native
// EquipActorToSocket + JigSetCanInteract). Weapons now go through it too
// (sync_equipment's Primary-slot handler, passing isSecondary=false).

// BP_JigPickupComponent_C.ItemDataAsset (research/CXXHeaderDump/
// BP_JigPickupComponent.hpp @0x0A8, cross-checked against the FModel export
// research/Exports/.../BP_JigPickupComponent.json which lists the same
// "ItemDataAsset" property name) — a plain, non-replicated UPROPERTY the real
// "dropped in world"/crafted-item spawn flow sets, never populated by our own
// BeginDeferredActorSpawnFromClass spawn alone. The FModel export of
// BP_JigHelperComp.json's "Equip Actor to Socket" function lists
// CallFunc_GetComponentByClass_ReturnValue typed as BP_JigPickupComponent_C —
// i.e. EquipActorToSocket itself fetches ActorRef's own pickup component and
// (per its other locals, CallFunc_GetMeshFromOwner_Mesh + a Select(Name) node
// feeding K2_AttachToComponent) almost certainly reads ItemDataAsset's
// EquipSocket/PrimaryUnequipSocket from there to pick the attach socket. This
// was never populated in Session 49/50's weapon-visual attempt, which is the
// likely real reason EquipActorToSocket was found "actively harmful" then —
// not a fundamentally broken function, just fed a null ItemDataAsset. Populate
// it before calling equip_actor_to_socket.
static bool set_pickup_item_data(AActor* pickupActor, void* itemAsset)
{
    if (!pickupActor || !itemAsset) return false;

    auto** pickupCompSlot = static_cast<UObject**>(
        pickupActor->GetValuePtrByPropertyNameInChain(L"BP_JigPickupComponent"));
    UObject* pickupComp = (pickupCompSlot && *pickupCompSlot) ? *pickupCompSlot : nullptr;
    if (!pickupComp) {
        debug_log("set_pickup_item_data: BP_JigPickupComponent property not found or null");
        return false;
    }

    *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pickupComp) + 0x0A8) = itemAsset;

    UFunction* setCountFn = pickupComp->GetFunctionByNameInChain(L"SetCount");
    if (setCountFn) {
        struct Params { int32_t OverrideCount = 1; } params;
        pickupComp->ProcessEvent(setCountFn, &params);
    }

    debug_log("set_pickup_item_data: wrote ItemDataAsset onto BP_JigPickupComponent");
    return true;
}

// Facewear/Headwear/Eyewear/Accessory/Backpack are represented by spawned
// actors + BP_JigHelperComp_C.EquipActorToSocket (research/
// 04_ida_investigation_log.md Session 49/51's OnRep_FacewearEquipped?
// decompile), not the Clothing_* mesh-swap that handles Torso/Gloves/Legs/
// Feet/BodyArmor. Reuses the same BeginDeferredActorSpawnFromClass spawn as
// spawn_and_attach_weapon_visual, but calls the real native
// EquipActorToSocket instead of a manual K2_AttachToComponent dance — now
// that set_pickup_item_data above gives it a real, non-null ItemDataAsset to
// find via GetComponentByClass, on the theory (confirmed against the FModel
// export, not just guessed) that this — not EquipActorToSocket itself — was
// the missing piece in the weapon-visual investigation.
// Applies JigsawItem_DataAsset_C::EquippedTransform (FTransform @0x0220) as
// itemRoot's relative transform on top of whatever socket it was just
// SnapToTarget-attached to — a SnapToTarget attach resets relative
// location/rotation to identity, discarding any per-item correction, so this
// needs to be re-applied after *every* re-attach, not just the initial
// spawn. Session 51 confirmed (via the FModel export of DA_BlackFaceMask)
// this field carries a real non-identity Rotation meant for exactly this.
static void apply_item_equipped_transform(UObject* itemRoot, void* itemAsset)
{
    if (!itemRoot || !itemAsset) return;

    // K2_SetRelativeTransform takes a by-ref FHitResult mid-parameter-list
    // whose true engine size isn't documented anywhere in this project —
    // guessing it risks a ProcessEvent params-buffer layout mismatch. Write
    // RelativeLocation/RelativeRotation directly instead; USceneComponent
    // recomputes ComponentToWorld from these each tick for an attached
    // component, so no separate "refresh" call is needed.
    const auto* eq = reinterpret_cast<const NativeFTransform*>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x220);

    auto* relLoc = reinterpret_cast<double*>(reinterpret_cast<uintptr_t>(itemRoot) + 0x0128);
    relLoc[0] = eq->locX;
    relLoc[1] = eq->locY;
    relLoc[2] = eq->locZ;

    // FRotator (Pitch/Yaw/Roll, USceneComponent::RelativeRotation @0x0140)
    // from the quaternion stored in EquippedTransform — standard UE
    // FQuat::Rotator() conversion (handles the +-90 pitch singularity).
    const double qx = eq->rotX, qy = eq->rotY, qz = eq->rotZ, qw = eq->rotW;
    const double singularityTest = qz * qx - qw * qy;
    const double yawY = 2.0 * (qw * qz + qx * qy);
    const double yawX = 1.0 - 2.0 * (qy * qy + qz * qz);
    constexpr double kRad2Deg = 180.0 / 3.14159265358979323846;
    constexpr double kSingularityThreshold = 0.4999995;
    double pitch, yaw, roll;
    if (singularityTest < -kSingularityThreshold) {
        pitch = -90.0;
        yaw = std::atan2(yawY, yawX) * kRad2Deg;
        roll = -yaw - (2.0 * std::atan2(qx, qw) * kRad2Deg);
    } else if (singularityTest > kSingularityThreshold) {
        pitch = 90.0;
        yaw = std::atan2(yawY, yawX) * kRad2Deg;
        roll = yaw - (2.0 * std::atan2(qx, qw) * kRad2Deg);
    } else {
        pitch = std::asin(2.0 * singularityTest) * kRad2Deg;
        yaw = std::atan2(yawY, yawX) * kRad2Deg;
        roll = std::atan2(-2.0 * (qw * qx + qy * qz), 1.0 - 2.0 * (qx * qx + qy * qy)) * kRad2Deg;
    }
    auto* relRot = reinterpret_cast<double*>(reinterpret_cast<uintptr_t>(itemRoot) + 0x0140);
    relRot[0] = pitch;
    relRot[1] = yaw;
    relRot[2] = roll;

    char buf[160];
    snprintf(buf, sizeof(buf),
             "apply_item_equipped_transform: loc=(%.1f,%.1f,%.1f) rot(P/Y/R)=(%.1f,%.1f,%.1f)",
             eq->locX, eq->locY, eq->locZ, pitch, yaw, roll);
    debug_log(buf);
}

// Re-issues BP_SkeletalMeshPickup_C::JigSetCanInteract(false, false) on an
// already-equipped visual actor. Session 51 found the game silently
// re-asserts physics/interactability on our spawned pickup sometime after
// SetSimulatePhysics(false) + the initial JigSetCanInteract call
// (spawn_and_equip_item_visual below) — theorized as some other internal
// tick/timer on the pickup's own components re-running its "loose world
// item" state. The original fix only ever called this once, at spawn time;
// if the game's re-assertion can happen again later in a long play session
// (not proven, but the leading theory for the "meshes just detach"
// reports — Session 54), a one-shot fix can't catch that. Cheap enough
// (single ProcessEvent, no spawn/attach work) to re-call every time
// sync_equipment sees this slot, including the "already applied, skip the
// rest" fast path — unlike the write-side equip pipeline, this isn't worth
// gating on a change flag since it's a no-op state re-assertion, not a
// visible action.
static void reassert_no_interact(AActor* itemActor)
{
    if (!itemActor) return;
    UFunction* canInteractFn = itemActor->GetFunctionByNameInChain(L"JigSetCanInteract");
    if (!canInteractFn) return;
    struct Params { bool CanInteract = false; bool EnablePhysics = false; bool Result = false; } params;
    itemActor->ProcessEvent(canInteractFn, &params);
}

static AActor* spawn_and_equip_item_visual(AActor* actor, void* itemAsset, bool isSecondary,
                                            bool preferBackpackSocket = false)
{
    if (!actor || !itemAsset) return nullptr;

    auto* pickupClass = *reinterpret_cast<UClass**>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x128);
    if (!pickupClass) {
        debug_log("spawn_and_equip_item_visual: PickupClass is null on item asset");
        return nullptr;
    }

    UWorld* world = actor->GetWorld();
    if (!world) {
        debug_log("spawn_and_equip_item_visual: actor->GetWorld() is null");
        return nullptr;
    }

    // Session 51: AttachParent now confirmed non-null (a real attach, not a
    // silent no-op) yet the item still visually falls through the map —
    // ruling out both the wrong-component and physics-simulation theories.
    // Remaining leading theory: EquipActorToSocket's internal
    // K2_AttachToComponent may use a KeepWorldTransform-style rule rather
    // than SnapToTarget, in which case spawning at a literal identity
    // transform (world origin, 0,0,0 — frequently below the map) becomes the
    // preserved world position after attach instead of being overridden by
    // the socket snap. Spawn at the character's actual current location
    // instead, so even a KeepWorldTransform-style attach lands somewhere
    // sane near the player rather than at/under world origin.
    NativeFTransform xform;
    {
        UFunction* getLocFn = actor->GetFunctionByNameInChain(L"K2_GetActorLocation");
        if (getLocFn) {
            struct { double X = 0, Y = 0, Z = 0; } loc;
            actor->ProcessEvent(getLocFn, &loc);
            xform.locX = loc.X;
            xform.locY = loc.Y;
            xform.locZ = loc.Z;
            char buf[96];
            snprintf(buf, sizeof(buf), "spawn_and_equip_item_visual: spawning at actor location (%.1f, %.1f, %.1f)",
                     loc.X, loc.Y, loc.Z);
            debug_log(buf);
        } else {
            debug_log("spawn_and_equip_item_visual: K2_GetActorLocation NOT FOUND, spawning at world origin");
        }
    }
    void* pending = call_begin_deferred_spawn(world, pickupClass, &xform);
    if (!pending) {
        debug_log("spawn_and_equip_item_visual: BeginDeferredActorSpawnFromClass returned null");
        return nullptr;
    }
    call_finish_spawning(pending, &xform);

    auto* itemActor = static_cast<AActor*>(pending);
    itemActor->SetActorEnableCollision(false);
    itemActor->SetActorHiddenInGame(false);

    set_pickup_item_data(itemActor, itemAsset);

    // "Falls through the floor" (both this and the earlier weapon-visual
    // attempts) is consistent with the spawned pickup's root component still
    // simulating physics/gravity — SetActorEnableCollision(false) only stops
    // collision response, not the physics simulation itself, and
    // AttachToComponent's default WeldSimulatedBodies=false (both our own
    // manual calls and, per the FModel export, EquipActorToSocket's internal
    // one) means a simulating body keeps free-falling under its own physics
    // regardless of whether the attach itself succeeded. Disable physics on
    // the root/mesh component directly before equipping, same getter fallback
    // chain as spawn_and_attach_weapon_visual.
    UFunction* itemRootFn = itemActor->GetFunctionByNameInChain(L"GetSkeletalMeshComponent");
    if (!itemRootFn) itemRootFn = itemActor->GetFunctionByNameInChain(L"K2_GetRootComponent");
    UObject* itemRoot = nullptr;
    if (itemRootFn) itemActor->ProcessEvent(itemRootFn, &itemRoot);
    if (itemRoot) {
        UFunction* simFn = itemRoot->GetFunctionByNameInChain(L"SetSimulatePhysics");
        if (simFn) {
            struct Params { bool bSimulate = false; } params;
            itemRoot->ProcessEvent(simFn, &params);
        }
        char buf[96];
        snprintf(buf, sizeof(buf), "spawn_and_equip_item_visual: itemRoot=0x%llx SetSimulatePhysics(false) fn=%d",
                 reinterpret_cast<unsigned long long>(itemRoot), simFn != nullptr);
        debug_log(buf);
    } else {
        debug_log("spawn_and_equip_item_visual: no root/mesh component found on item actor");
    }

    const bool equipped = equip_actor_to_socket(actor, itemActor, isSecondary);

    // EquipActorToSocket's own internal socket-select (research/
    // 04_ida_investigation_log.md's live bytecode decode of "Equip Actor to
    // Socket") only branches on IsSecondary (PrimaryUnequipSocket vs
    // SecondaryUnequipSocket) — it has no idea whether a backpack is
    // currently equipped, so it always lands weapons on the no-backpack
    // socket ("PrimaryWeapon"/"SecondaryWeapon" on the skeleton) even when
    // one is worn. The skeleton has distinct dedicated sockets for the
    // backpack-worn case (confirmed via FModel export of
    // SK_Chr_ToplessMale_01_Skeleton.json: "PrimaryWeaponBackpack",
    // "SecondaryWeaponBackpack", both real sockets), and ItemDataAsset
    // carries the matching FName in PrimaryUnequipSocketBackpack (@0x290)/
    // SecondaryUnequipSocketBackpack (@0x3F8) (research/CXXHeaderDump/
    // JigsawItem_DataAsset.hpp) — read the right one ourselves and redo the
    // attach manually when the proxy currently has a backpack equipped.
    if (preferBackpackSocket && itemRoot) {
        const RawFGameplayTag backpackSocket = *reinterpret_cast<RawFGameplayTag*>(
            reinterpret_cast<uintptr_t>(itemAsset) + (isSecondary ? 0x3F8 : 0x290));
        auto** meshSlot = static_cast<UObject**>(actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
        UObject* rootMesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
        UFunction* reAttachFn = rootMesh ? itemRoot->GetFunctionByNameInChain(L"K2_AttachToComponent") : nullptr;
        if (reAttachFn) {
            struct Params {
                UObject*        Parent = nullptr;
                RawFGameplayTag SocketName;
                uint8_t         LocationRule = 2;   // SnapToTarget
                uint8_t         RotationRule = 2;   // SnapToTarget
                uint8_t         ScaleRule = 1;       // KeepWorld — matches EquipActorToSocket's own params
                bool            WeldSimulatedBodies = true; // matches EquipActorToSocket's own params
                bool            ReturnValue = false;
            } rparams;
            rparams.Parent = rootMesh;
            rparams.SocketName = backpackSocket;
            itemRoot->ProcessEvent(reAttachFn, &rparams);
            char buf[128];
            snprintf(buf, sizeof(buf),
                     "spawn_and_equip_item_visual: backpack re-attach socket_ci=%d returned %d",
                     backpackSocket.ComparisonIndex, rparams.ReturnValue);
            debug_log(buf);
        } else {
            debug_log("spawn_and_equip_item_visual: backpack re-attach skipped (no rootMesh or K2_AttachToComponent)");
        }
    }

    // Every slot now attaches mechanically correctly (RelativeLocation
    // consistently (0,0,0) — a clean SnapToTarget) but several items
    // (Eyewear/Glasses, Backpack, the melee axe) still visually render at an
    // odd orientation — "not in the right spot" despite a correct attach.
    // See apply_item_equipped_transform's own comment for what this corrects.
    apply_item_equipped_transform(itemRoot, itemAsset);

    // Session 51: the attach is now confirmed correct at the instant it
    // happens (RelativeLocation (0,0,0), sane ComponentToWorld) yet the item
    // still visibly detaches and falls moments later, even with our own
    // SetSimulatePhysics(false) already applied and even after gating out
    // redundant re-processing of an unchanged slot. Remaining theory: a real,
    // naturally-spawned pickup only stops acting like a "world item" (no
    // physics, not interactable) once something explicitly tells it so —
    // ABP_SkeletalMeshPickup_C's own JigSetCanInteract(CanInteract,
    // EnablePhysics, Result) UFUNCTION (research/CXXHeaderDump/
    // BP_SkeletalMeshPickup.hpp) looks like exactly that "this is now
    // equipped, not a loose pickup" signal — our own SetSimulatePhysics call
    // is a raw engine-level override, not the same as this Blueprint-level
    // state transition, so some other internal tick/timer logic on the
    // pickup (CheckDistanceFromActor, etc. on BP_JigPickupComponent) may be
    // re-asserting physics/interactability against our override. Call it.
    reassert_no_interact(itemActor);

    if (itemRoot) {
        const void* attachParent = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(itemRoot) + 0xB0);
        char buf[128];
        snprintf(buf, sizeof(buf), "spawn_and_equip_item_visual: spawned=0x%llx equip_actor_to_socket=%d AttachParent=0x%llx",
                 reinterpret_cast<unsigned long long>(itemActor), equipped,
                 reinterpret_cast<unsigned long long>(attachParent));
        debug_log(buf);

        // AttachParent alone doesn't prove it renders in the right place —
        // read the actual relative offset (USceneComponent::RelativeLocation,
        // research/CXXHeaderDump/Engine.hpp @0x0128) plus the fully-resolved
        // world transform (K2_GetComponentToWorld) to see exactly where the
        // engine thinks this component is, instead of inferring it from
        // "falls through the map" descriptions alone.
        const auto* relLoc = reinterpret_cast<const double*>(
            reinterpret_cast<uintptr_t>(itemRoot) + 0x0128);
        char buf2[128];
        snprintf(buf2, sizeof(buf2), "spawn_and_equip_item_visual: RelativeLocation=(%.1f, %.1f, %.1f)",
                 relLoc[0], relLoc[1], relLoc[2]);
        debug_log(buf2);

        UFunction* toWorldFn = itemRoot->GetFunctionByNameInChain(L"K2_GetComponentToWorld");
        if (toWorldFn) {
            struct Params { NativeFTransform ReturnValue; } wparams;
            itemRoot->ProcessEvent(toWorldFn, &wparams);
            char buf3[128];
            snprintf(buf3, sizeof(buf3), "spawn_and_equip_item_visual: ComponentToWorld=(%.1f, %.1f, %.1f)",
                     wparams.ReturnValue.locX, wparams.ReturnValue.locY, wparams.ReturnValue.locZ);
            debug_log(buf3);
        } else {
            debug_log("spawn_and_equip_item_visual: K2_GetComponentToWorld NOT FOUND");
        }
    }

    return itemActor;
}

// Weapon attachments (scopes/grips/mags/etc.) are their own actor class —
// ABP_AMainLocalAttachment_C (research/CXXHeaderDump/BP_AMainLocalAttachment.hpp)
// — not a variant of the same spawn-and-EquipActorToSocket path used for
// equipment/weapons above. Each attachment DataAsset carries its own
// Local_ActorClass (TSubclassOf<ABP_AMainLocalAttachment_C> @0x03D8) and
// Local_AttachSocket (FName @0x0398, research/CXXHeaderDump/
// JigsawItem_DataAsset.hpp) — a socket on the *weapon's own mesh*, not the
// character's — so no GetMeshFromOwner/EquipActorToSocket involvement at
// all; just spawn Local_ActorClass, initialize it via the real
// Jig_SetAttachmentInfo(UID, AttachmentInfo) UFUNCTION, and
// K2_AttachToComponent it onto the weapon's root at Local_AttachSocket.
static AActor* spawn_and_attach_weapon_attachment(AActor* weaponActor, void* attachmentItemAsset)
{
    if (!weaponActor || !attachmentItemAsset) return nullptr;

    auto* actorClass = *reinterpret_cast<UClass**>(
        reinterpret_cast<uintptr_t>(attachmentItemAsset) + 0x3D8);
    if (!actorClass) {
        debug_log("spawn_and_attach_weapon_attachment: Local_ActorClass is null on item asset");
        return nullptr;
    }

    UWorld* world = weaponActor->GetWorld();
    if (!world) return nullptr;

    NativeFTransform xform;
    void* pending = call_begin_deferred_spawn(world, actorClass, &xform);
    if (!pending) {
        debug_log("spawn_and_attach_weapon_attachment: BeginDeferredActorSpawnFromClass returned null");
        return nullptr;
    }
    call_finish_spawning(pending, &xform);

    auto* attachmentActor = static_cast<AActor*>(pending);
    attachmentActor->SetActorEnableCollision(false);
    attachmentActor->SetActorHiddenInGame(false);

    UFunction* setInfoFn = attachmentActor->GetFunctionByNameInChain(L"Jig_SetAttachmentInfo");
    if (setInfoFn) {
        struct Params {
            RawFGuid UID;
            UObject* AttachmentInfo = nullptr;
            bool     Result         = false;
        } params;
        params.AttachmentInfo = static_cast<UObject*>(attachmentItemAsset);
        attachmentActor->ProcessEvent(setInfoFn, &params);
    } else {
        debug_log("spawn_and_attach_weapon_attachment: Jig_SetAttachmentInfo NOT FOUND");
    }

    // Weapon's own root (same GetSkeletalMeshComponent/K2_GetRootComponent
    // fallback chain used everywhere else in this file).
    UFunction* weaponRootFn = weaponActor->GetFunctionByNameInChain(L"GetSkeletalMeshComponent");
    if (!weaponRootFn) weaponRootFn = weaponActor->GetFunctionByNameInChain(L"K2_GetRootComponent");
    UObject* weaponRoot = nullptr;
    if (weaponRootFn) weaponActor->ProcessEvent(weaponRootFn, &weaponRoot);
    if (!weaponRoot) {
        debug_log("spawn_and_attach_weapon_attachment: weapon actor has no root/mesh component");
        return attachmentActor;
    }

    UFunction* attachmentRootFn = attachmentActor->GetFunctionByNameInChain(L"K2_GetRootComponent");
    UObject* attachmentRoot = nullptr;
    if (attachmentRootFn) attachmentActor->ProcessEvent(attachmentRootFn, &attachmentRoot);
    if (!attachmentRoot) {
        debug_log("spawn_and_attach_weapon_attachment: attachment actor has no root component");
        return attachmentActor;
    }

    const RawFGameplayTag socket = *reinterpret_cast<RawFGameplayTag*>(
        reinterpret_cast<uintptr_t>(attachmentItemAsset) + 0x398);

    UFunction* attachFn = attachmentRoot->GetFunctionByNameInChain(L"K2_AttachToComponent");
    if (attachFn) {
        struct Params {
            UObject*        Parent = nullptr;
            RawFGameplayTag SocketName;
            uint8_t         LocationRule = 2;   // SnapToTarget
            uint8_t         RotationRule = 2;   // SnapToTarget
            uint8_t         ScaleRule = 2;        // SnapToTarget
            bool            WeldSimulatedBodies = true;
            bool            ReturnValue = false;
        } params;
        params.Parent = weaponRoot;
        params.SocketName = socket;
        attachmentRoot->ProcessEvent(attachFn, &params);
        char buf[128];
        snprintf(buf, sizeof(buf),
                 "spawn_and_attach_weapon_attachment: spawned=0x%llx socket_ci=%d returned=%d",
                 reinterpret_cast<unsigned long long>(attachmentActor),
                 socket.ComparisonIndex, params.ReturnValue);
        debug_log(buf);
    } else {
        debug_log("spawn_and_attach_weapon_attachment: K2_AttachToComponent NOT FOUND");
    }

    return attachmentActor;
}

// BP_PlayerCharacter_C.EquipClothingToMesh(FName ItemId, AActor* ActorRef,
// USkinnedMeshComponent* ClothingRef, FName BodyPart) internally does a
// GetDataTableRowFromName lookup against the DT_Clothing DataTable, keyed by
// the item's own ItemId (research/04_ida_investigation_log.md Session 49's
// bytecode decompile) — but that table is a genuine base-game content gap,
// missing rows for roughly half of all real clothing/armor items (confirmed
// 2026-08-12 via FModel export + cross-reference: 50/97 items have no
// matching row at all, e.g. every "Makeshift"-prefixed crafted item and most
// color-variant items like BrownHeavyJeans/BlueGloves). Every item DataAsset
// carries its own Male/FemaleMesh directly in ClothingSettings (@0x430,
// research/CXXHeaderDump/JigsawItem_DataAsset.hpp) regardless of whether a
// DT_Clothing row exists for it (confirmed live in the FModel export:
// BrownHeavyJeans has no row but has real Male/FemaleMesh SkeletalMesh
// references) — so read that mesh directly and push it onto the target
// Clothing_* component via SetSkinnedAssetAndUpdate instead, sidestepping
// the incomplete table entirely. IsPlayerMale? is BP_PlayerCharacter_C's own
// field (research/CXXHeaderDump/BP_PlayerCharacter.hpp @0x15A0).
static bool equip_clothing_to_mesh(AActor* actor, void* itemAsset, uintptr_t clothingCompOffset)
{
    if (!actor || !itemAsset) return false;

    // Session 51 diagnostic: a freeze was observed immediately after a
    // facewear item's spawn_and_equip_item_visual() call for the first time
    // ever succeeded in actually setting AttachParent (real
    // EquipActorToSocket attach) — the debug.log went completely silent
    // right after processing the next Torso slot, with no way to tell from
    // the existing logging alone whether this function was even entered, or
    // where exactly inside it things stopped. Bracket every step so the next
    // live repro pinpoints the exact call.
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "equip_clothing_to_mesh: enter offset=0x%llx",
                 static_cast<unsigned long long>(clothingCompOffset));
        debug_log(buf);
    }

    auto* clothingComp = *reinterpret_cast<UObject**>(
        reinterpret_cast<uintptr_t>(actor) + clothingCompOffset);
    if (!clothingComp) {
        debug_log("equip_clothing_to_mesh: target Clothing_* component is null");
        return false;
    }

    const bool isMale = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(actor) + 0x15A0);
    UObject* mesh = *reinterpret_cast<UObject**>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x430 + (isMale ? 0x00 : 0x08));
    if (!mesh) {
        debug_log("equip_clothing_to_mesh: itemId=\"" +
            equip_native::fname_to_string(reinterpret_cast<uintptr_t>(itemAsset) + 0x30) +
            "\" has no " + std::string(isMale ? "male" : "female") + " mesh");
        return false;
    }

    UFunction* fn = clothingComp->GetFunctionByNameInChain(L"SetSkinnedAssetAndUpdate");
    if (!fn) {
        debug_log("equip_clothing_to_mesh: SetSkinnedAssetAndUpdate NOT FOUND");
        return false;
    }

    struct Params {
        UObject* NewMesh = nullptr;
        bool     bReinitPose = true;
    } params;
    static_assert(offsetof(Params, NewMesh) == 0x00, "Kismet param layout");
    static_assert(offsetof(Params, bReinitPose) == 0x08, "Kismet param layout");

    params.NewMesh = mesh;
    {
        char buf[96];
        snprintf(buf, sizeof(buf), "equip_clothing_to_mesh: about to ProcessEvent SetSkinnedAssetAndUpdate clothingComp=0x%llx",
                 reinterpret_cast<unsigned long long>(clothingComp));
        debug_log(buf);
    }
    clothingComp->ProcessEvent(fn, &params);
    debug_log("equip_clothing_to_mesh: ProcessEvent returned");

    // The unequip-clear path hides this component (SetVisibility(false)) —
    // re-show it here in case a previous unequip left it hidden, otherwise
    // re-equipping the same slot after taking it off once would silently
    // stay invisible forever.
    UFunction* visFn = clothingComp->GetFunctionByNameInChain(L"SetVisibility");
    if (visFn) {
        struct VisParams { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
        clothingComp->ProcessEvent(visFn, &vparams);
    }

    debug_log("equip_clothing_to_mesh: itemId=\"" +
        equip_native::fname_to_string(reinterpret_cast<uintptr_t>(itemAsset) + 0x30) +
        "\" set mesh directly (bypassing DT_Clothing)");
    return true;
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

// __try/__except can't share a stack frame with C++ objects that need
// unwinding (MSVC C2712), so the guarded call is split into a plain function
// invoked through this trampoline — same pattern as mod.cpp's seh_invoke.
// Guards K2_DestroyActor() against a stale primaryWeaponVisualActor pointer:
// live-crashed 2026-08-12 when the "respawn treadmill" (spawn_and_attach_
// weapon_visual spawning a new actor on nearly every equip-sync cycle
// instead of only on real item changes — a separate, not-yet-fixed bug)
// left the tracked pointer referring to an already-gone actor by the time
// an unequip tried to tear it down.
static bool seh_invoke(void (*fn)(void*), void* ctx)
{
    __try {
        fn(ctx);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

static void do_destroy_actor(void* ctx)
{
    static_cast<AActor*>(ctx)->K2_DestroyActor();
}

static void destroy_actor_safe(void*& actorPtr)
{
    if (!actorPtr) return;
    if (!seh_invoke(do_destroy_actor, actorPtr))
        debug_log("destroy_actor_safe: K2_DestroyActor crashed on a stale pointer, caught via SEH");
    actorPtr = nullptr;
}

// "Respawn treadmill" mitigation — see RemotePlayer::lastVisualRespawnUs's
// own comment. The itemId-change check that gates every visual respawn call
// site isn't reliable enough on its own (live-tested 2026-08-12: the same
// itemId respawned dozens of times a minute for no real equip change), so
// cap actual respawns to at most once per 2s per slot regardless of what
// the comparison says — cheap, safe, and independent of whatever the real
// underlying cause of the itemId flicker turns out to be.
static bool respawn_cooldown_ok(RemotePlayer& player, uint8_t slotIndex)
{
    const uint64_t now = now_micros();
    uint64_t& last = player.lastVisualRespawnUs[slotIndex];
    if (now - last < 2'000'000ULL) return false;
    last = now;
    return true;
}

void ProxyManager::sync_equipment(AActor* actor, RemotePlayer& player)
{
    if (!actor || !player.equipmentDirty) return;
    player.equipmentDirty = false;

    // The loop below only iterates slots present in the latest Equipment
    // frame (the wire format omits empty slots entirely). Unequipped slots
    // — present in player.appliedSlotsMask from a prior sync but missing
    // here — are handled separately at the end of this function.

    // One-shot verification pass: for every slot we're about to touch, read
    // it back first via the safe, read-only getter and log the result. This
    // never calls the write path — see kEnableEquipmentWrite above for why.
    for (const auto& slot : player.equipment) {
        // Session 51: the facewear visual (spawn_and_equip_item_visual) now
        // genuinely attaches (AttachParent set, RelativeLocation (0,0,0),
        // correct world position confirmed live) but still visibly detaches
        // and falls a moment later. The wire layer resends a full Equipment
        // snapshot periodically even when nothing changed, and this loop
        // previously called SetEquippedInfoBySlot again on every single one
        // of those resends, unconditionally, for every slot — including one
        // already correctly attached. BP_JigPickupComponent_C has functions
        // literally named ValidateAttachedActor/CheckMismatch; a redundant
        // SetEquippedInfoBySlot call re-triggering that kind of validation
        // (which doesn't know about our manually-attached actor) is the
        // leading theory for the detach. Skip all processing for slot 0 once
        // its item is already applied and attached, instead of redoing the
        // full equip dance on every resend.
        if (slot.slotIndex == 0 && !slot.itemId.empty() && slot.itemId == player.facewearVisualItemId) {
            reassert_no_interact(static_cast<AActor*>(player.facewearVisualActor));
            continue;
        }
        if (slot.slotIndex == 1 && !slot.itemId.empty() && slot.itemId == player.headwearVisualItemId) {
            reassert_no_interact(static_cast<AActor*>(player.headwearVisualActor));
            continue;
        }
        if (slot.slotIndex == 2 && !slot.itemId.empty() && slot.itemId == player.eyewearVisualItemId) {
            reassert_no_interact(static_cast<AActor*>(player.eyewearVisualActor));
            continue;
        }
        if (slot.slotIndex == 10 && !slot.itemId.empty() && slot.itemId == player.backpackVisualItemId) {
            reassert_no_interact(static_cast<AActor*>(player.backpackVisualActor));
            continue;
        }

        RawFRepItemInfo info{};
        bool equipped = false;
        const bool ok = get_equipped_info_by_slot(actor, slot.slotIndex, info, equipped);
        Output::send<LogLevel::Normal>(
            STR("SDB: equip-getter slot={:d} itemId={} ok={:d} equipped={:d}\n"),
            slot.slotIndex, widen(slot.itemId), ok, equipped);

        // Skip the write-side pipeline entirely when this slot's itemId
        // hasn't actually changed since we last applied it — see
        // RemotePlayer::appliedEquipItemId's comment. get_equipped_info_by_slot
        // above always reports equipped=false for a proxy regardless, so it
        // can't be used as the "already applied" signal itself.
        const bool equipItemChanged =
            player.appliedEquipItemId[slot.slotIndex] != slot.itemId;

        // Weapon slots (11-14) go through the full write pipeline below only
        // when equipItemChanged — an already-applied, unchanged weapon slot
        // never re-touches its visual actor at all otherwise, same gap as
        // the facewear/headwear/eyewear/backpack early-continues above (see
        // reassert_no_interact's own comment). Cover it here since the
        // weapon path's spawn/attach logic is nested too deep inside the
        // write pipeline below to cleanly add an else-branch there.
        if (!equipItemChanged && !slot.itemId.empty()) {
            switch (slot.slotIndex) {
                case 11: reassert_no_interact(static_cast<AActor*>(player.primaryWeaponVisualActor));   break;
                case 12: reassert_no_interact(static_cast<AActor*>(player.secondaryWeaponVisualActor)); break;
                case 13: reassert_no_interact(static_cast<AActor*>(player.sidearmVisualActor));         break;
                case 14: reassert_no_interact(static_cast<AActor*>(player.meleeVisualActor));           break;
                default: break;
            }
        }

        if (kEnableEquipmentWrite && equipItemChanged) {
            player.appliedEquipItemId[slot.slotIndex] = slot.itemId;
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

                    // Session 50: tried skipping this call entirely (it runs
                    // every sync_equipment() pass, not gated on item change,
                    // unlike the spawn below) on the theory that its real
                    // delegate handler was detaching our manually-spawned
                    // weapon visual on each subsequent cycle — a direct
                    // memory read afterward still showed AttachParent as
                    // NULL, ruling this out too. Restored.
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
                        if (slot.itemId != player.primaryWeaponVisualItemId &&
                            respawn_cooldown_ok(player, slot.slotIndex)) {
                            destroy_actor_safe(player.primaryWeaponVisualActor);
                            void* itemAsset = resolve_item_asset(slot.itemId);
                            // Session 51: switched from the old manual
                            // spawn_and_attach_weapon_visual (K2_AttachToComponent
                            // dance, 11 attempts across 4 sessions, always
                            // AttachParent=NULL) to spawn_and_equip_item_visual
                            // — the same function that just fixed facewear,
                            // using the real native EquipActorToSocket path
                            // plus the two other required pieces (ItemDataAsset
                            // population, JigSetCanInteract) that were missing
                            // the whole time. See spawn_and_equip_item_visual's
                            // own comment for the full mechanism.
                            AActor* visual = itemAsset
                                ? spawn_and_equip_item_visual(actor, itemAsset, false,
                                                               !player.backpackVisualItemId.empty())
                                : nullptr;
                            player.primaryWeaponVisualActor = visual;
                            player.primaryWeaponVisualItemId = slot.itemId;
                            Output::send<LogLevel::Normal>(
                                STR("SDB: equip-visual slot={:d} itemId={} spawnedPtr=0x{:x}\n"),
                                slot.slotIndex, widen(slot.itemId),
                                reinterpret_cast<uintptr_t>(visual));
                        }
                    }

                    // Secondary/Sidearm/Melee (slots 12-14) — same visual
                    // path as Primary above, minus the PrimaryWeaponEquipped?
                    // bookkeeping (that's genuinely Primary-only). isSecondary
                    // is only true for slot 12 — ItemDataAsset only defines
                    // Primary-family vs Secondary-family unequip sockets
                    // (research/CXXHeaderDump/JigsawItem_DataAsset.hpp), so
                    // Sidearm/Melee use the same "Primary" family as a
                    // reasonable default.
                    void** slotVisualActor = nullptr;
                    std::string* slotVisualItemId = nullptr;
                    switch (slot.slotIndex) {
                        case 12: slotVisualActor = &player.secondaryWeaponVisualActor; slotVisualItemId = &player.secondaryWeaponVisualItemId; break;
                        case 13: slotVisualActor = &player.sidearmVisualActor;          slotVisualItemId = &player.sidearmVisualItemId;          break;
                        case 14: slotVisualActor = &player.meleeVisualActor;            slotVisualItemId = &player.meleeVisualItemId;            break;
                        default: break;
                    }
                    if (slotVisualActor && slot.itemId != *slotVisualItemId &&
                        respawn_cooldown_ok(player, slot.slotIndex)) {
                        destroy_actor_safe(*slotVisualActor);
                        void* itemAsset = resolve_item_asset(slot.itemId);
                        AActor* visual = itemAsset
                            ? spawn_and_equip_item_visual(actor, itemAsset, slot.slotIndex == 12,
                                                           !player.backpackVisualItemId.empty())
                            : nullptr;
                        *slotVisualActor = visual;
                        *slotVisualItemId = slot.itemId;
                        Output::send<LogLevel::Normal>(
                            STR("SDB: equip-visual slot={:d} itemId={} spawnedPtr=0x{:x}\n"),
                            slot.slotIndex, widen(slot.itemId),
                            reinterpret_cast<uintptr_t>(visual));
                    }
                }
            } else if (wrote) {
                // Facewear (0) / Eyewear (2) / Backpack (10) — actor-spawn +
                // real-native-EquipActorToSocket path (see
                // spawn_and_equip_item_visual's comment), confirmed working
                // live for Facewear. Only respawn when the equipped item
                // actually changes, same guard as the weapon slots above.
                {
                    void** slotVisualActor = nullptr;
                    std::string* slotVisualItemId = nullptr;
                    switch (slot.slotIndex) {
                        case 0:  slotVisualActor = &player.facewearVisualActor; slotVisualItemId = &player.facewearVisualItemId; break;
                        case 1:  slotVisualActor = &player.headwearVisualActor; slotVisualItemId = &player.headwearVisualItemId; break;
                        case 2:  slotVisualActor = &player.eyewearVisualActor;  slotVisualItemId = &player.eyewearVisualItemId;  break;
                        case 10: slotVisualActor = &player.backpackVisualActor; slotVisualItemId = &player.backpackVisualItemId; break;
                        default: break;
                    }
                    if (slotVisualActor && slot.itemId != *slotVisualItemId &&
                        respawn_cooldown_ok(player, slot.slotIndex)) {
                        destroy_actor_safe(*slotVisualActor);
                        void* itemAsset = resolve_item_asset(slot.itemId);
                        AActor* visual = itemAsset
                            ? spawn_and_equip_item_visual(actor, itemAsset, false)
                            : nullptr;
                        *slotVisualActor = visual;
                        *slotVisualItemId = slot.itemId;
                        Output::send<LogLevel::Normal>(
                            STR("SDB: equip-visual slot={:d} itemId={} spawnedPtr=0x{:x}\n"),
                            slot.slotIndex, widen(slot.itemId),
                            reinterpret_cast<uintptr_t>(visual));
                    }
                }

                // Clothing slots — push the item's own mesh directly onto one
                // of the character's own pre-existing Clothing_* components
                // (research/CXXHeaderDump/BP_PlayerCharacter.hpp) instead of
                // the weapon path's spawn-a-real-actor dance. Only the 5
                // slots with a confirmed matching component are wired up;
                // Headwear/Accessory/Backpack still need the same treatment
                // as Facewear/Eyewear above.
                uintptr_t clothingOffset = 0;
                switch (slot.slotIndex) {
                    case 4:  clothingOffset = 0x0770; break; // Torso
                    case 5:  clothingOffset = 0x0780; break; // Gloves
                    case 6:  clothingOffset = 0x0768; break; // Legs
                    case 7:  clothingOffset = 0x0760; break; // Feet
                    case 9:  clothingOffset = 0x07B8; break; // BodyArmor
                    default: break;
                }
                if (clothingOffset) {
                    void* itemAsset = resolve_item_asset(slot.itemId);
                    const bool called = itemAsset && equip_clothing_to_mesh(actor, itemAsset, clothingOffset);
                    Output::send<LogLevel::Normal>(
                        STR("SDB: equip-clothing slot={:d} itemId={} ok={:d}\n"),
                        slot.slotIndex, widen(slot.itemId), called);

                    // Gloves-vs-bare-hands z-fight (live-tested 2026-08-13:
                    // only became visible once the proxy started actually
                    // animating — the bare "Hands" body-part mesh (always
                    // shown, offset 0x7B0, see sync_pawn_appearance) and the
                    // Clothing_Gloves mesh both deform with the walk cycle
                    // and flicker against each other). Same "sync needs a
                    // clear case" fix as the naked-body/clothing pulse from
                    // earlier tonight: hide the bare hands whenever gloves
                    // are genuinely equipped.
                    if (called && slot.slotIndex == 5) {
                        auto* handsComp = *reinterpret_cast<UObject**>(
                            reinterpret_cast<uintptr_t>(actor) + 0x07B0);
                        if (handsComp) {
                            UFunction* visFn = handsComp->GetFunctionByNameInChain(L"SetVisibility");
                            if (visFn) {
                                struct Params { bool bNewVisibility = false; bool bPropagateToChildren = false; } vparams;
                                handsComp->ProcessEvent(visFn, &vparams);
                            }
                        }
                    }
                }
            }
        }
    }

    // Clear any slot that was written on a previous sync but is missing from
    // this frame — the wire format omits empty slots entirely (see
    // read_local_equipment()), so a slot going from equipped to unequipped
    // simply vanishes from the next frame rather than arriving with an
    // empty itemId. Without this, nothing ever detects an unequip and the
    // proxy keeps showing stale state forever (the gap noted above/in
    // earlier sessions). All 21 slots have real tags now (Session 47), so
    // this applies uniformly rather than just to Primary.
    uint32_t newMask = 0;
    for (const auto& slot : player.equipment) newMask |= (1u << slot.slotIndex);
    uint32_t effectiveMask = newMask;

    if (kEnableEquipmentWrite) {
        for (uint8_t i = 0; i < EQUIPMENT_SLOT_COUNT; ++i) {
            const uint32_t bit = 1u << i;
            if (newMask & bit) {
                player.missingSlotStreak.erase(i);
                continue;
            }
            if (player.appliedSlotsMask & bit) {
                // Missing this frame — see RemotePlayer::missingSlotStreak's
                // comment. Debounce: only treat it as a real unequip once
                // it's been missing for 2 consecutive frames in a row, so a
                // single-frame sender read glitch doesn't clear+reapply.
                int& streak = player.missingSlotStreak[i];
                ++streak;
                if (streak < 2) {
                    effectiveMask |= bit;
                    continue;
                }
                player.missingSlotStreak.erase(i);
                const bool cleared = set_equipped_info_by_slot(actor, i, "");
                Output::send<LogLevel::Normal>(
                    STR("SDB: equip-clear slot={:d} ok={:d}\n"), i, cleared);
                player.appliedEquipItemId.erase(i);

                // Primary's spawned weapon-visual actor doesn't tear itself
                // down just because the underlying data got cleared — do it
                // explicitly, same as the "item changed" path above.
                if (cleared && i == 11 && player.primaryWeaponVisualActor) {
                    destroy_actor_safe(player.primaryWeaponVisualActor);
                    player.primaryWeaponVisualItemId.clear();
                }
                if (cleared && i == 0 && player.facewearVisualActor) {
                    destroy_actor_safe(player.facewearVisualActor);
                    player.facewearVisualItemId.clear();
                }
                if (cleared && i == 1 && player.headwearVisualActor) {
                    destroy_actor_safe(player.headwearVisualActor);
                    player.headwearVisualItemId.clear();
                }
                if (cleared && i == 2 && player.eyewearVisualActor) {
                    destroy_actor_safe(player.eyewearVisualActor);
                    player.eyewearVisualItemId.clear();
                }
                if (cleared && i == 10 && player.backpackVisualActor) {
                    destroy_actor_safe(player.backpackVisualActor);
                    player.backpackVisualItemId.clear();
                }
                if (cleared && i == 12 && player.secondaryWeaponVisualActor) {
                    destroy_actor_safe(player.secondaryWeaponVisualActor);
                    player.secondaryWeaponVisualItemId.clear();
                }
                if (cleared && i == 13 && player.sidearmVisualActor) {
                    destroy_actor_safe(player.sidearmVisualActor);
                    player.sidearmVisualItemId.clear();
                }
                if (cleared && i == 14 && player.meleeVisualActor) {
                    destroy_actor_safe(player.meleeVisualActor);
                    player.meleeVisualItemId.clear();
                }

                // Clothing slots (Torso/Gloves/Legs/Feet/BodyArmor) never had
                // an unequip-clear case at all — equip_clothing_to_mesh only
                // ever pushes a mesh onto the Clothing_* component, nothing
                // ever reset it, so taking an item off (pants, live-tested
                // 2026-08-12) left the last-equipped mesh visible forever.
                // Same "sync needs a clear case" lesson as tonight's
                // accessory/beard fixes — hide the Clothing_* component
                // instead of trying to clear its mesh; the naked body-part
                // component underneath (Torso/Legs/Feet/etc.) is a separate,
                // already-present component that shows through once the
                // clothing layer is hidden.
                uintptr_t clearClothingOffset = 0;
                switch (i) {
                    case 4:  clearClothingOffset = 0x0770; break; // Torso
                    case 5:  clearClothingOffset = 0x0780; break; // Gloves
                    case 6:  clearClothingOffset = 0x0768; break; // Legs
                    case 7:  clearClothingOffset = 0x0760; break; // Feet
                    case 9:  clearClothingOffset = 0x07B8; break; // BodyArmor
                    default: break;
                }
                if (cleared && clearClothingOffset) {
                    auto* clothingComp = *reinterpret_cast<UObject**>(
                        reinterpret_cast<uintptr_t>(actor) + clearClothingOffset);
                    if (clothingComp) {
                        UFunction* visFn = clothingComp->GetFunctionByNameInChain(L"SetVisibility");
                        if (visFn) {
                            struct Params { bool bNewVisibility = false; bool bPropagateToChildren = false; } vparams;
                            clothingComp->ProcessEvent(visFn, &vparams);
                        }
                    }
                }

                // Mirror of the hide-on-equip above: gloves coming off means
                // the bare "Hands" body-part mesh needs to be shown again.
                if (cleared && i == 5) {
                    auto* handsComp = *reinterpret_cast<UObject**>(
                        reinterpret_cast<uintptr_t>(actor) + 0x07B0);
                    if (handsComp) {
                        UFunction* visFn = handsComp->GetFunctionByNameInChain(L"SetVisibility");
                        if (visFn) {
                            struct Params { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
                            handsComp->ProcessEvent(visFn, &vparams);
                        }
                    }
                }
            }
        }
    }
    player.appliedSlotsMask = effectiveMask;
}

// Re-attaches an already-spawned weapon visual actor's root/mesh component
// onto a named socket on the character's own Mesh component. Same
// SnapToTarget + WeldSimulatedBodies=true K2_AttachToComponent parameters
// spawn_and_equip_item_visual's own manual backpack-socket re-attach already
// uses successfully — SocketName is accepted as a RawFGameplayTag purely
// because that's an 8-byte {ComparisonIndex,Number} layout identical to
// FName's own raw layout (both back onto the same engine name table), not
// because sockets are actually FGameplayTag-typed; JigsawItem_DataAsset's
// EquipSocket/UnequipSocket fields are declared FName in
// research/CXXHeaderDump/JigsawItem_DataAsset.hpp.
static bool reattach_weapon_visual_to_socket(AActor* characterActor, AActor* itemActor, RawFGameplayTag socketName)
{
    if (!characterActor || !itemActor) return false;

    UFunction* itemRootFn = itemActor->GetFunctionByNameInChain(L"GetSkeletalMeshComponent");
    if (!itemRootFn) itemRootFn = itemActor->GetFunctionByNameInChain(L"K2_GetRootComponent");
    UObject* itemRoot = nullptr;
    if (itemRootFn) itemActor->ProcessEvent(itemRootFn, &itemRoot);
    if (!itemRoot) return false;

    auto** meshSlot = static_cast<UObject**>(characterActor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* rootMesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!rootMesh) return false;

    UFunction* attachFn = itemRoot->GetFunctionByNameInChain(L"K2_AttachToComponent");
    if (!attachFn) return false;

    struct Params {
        UObject*        Parent = nullptr;
        RawFGameplayTag SocketName;
        uint8_t         LocationRule = 2;   // SnapToTarget
        uint8_t         RotationRule = 2;   // SnapToTarget
        uint8_t         ScaleRule = 1;      // KeepWorld
        bool            WeldSimulatedBodies = true;
        bool            ReturnValue = false;
    } params;
    params.Parent = rootMesh;
    params.SocketName = socketName;
    itemRoot->ProcessEvent(attachFn, &params);
    return params.ReturnValue;
}

// isSecondary convention matching the existing spawn_and_equip_item_visual
// call sites above: only slot 12 (Secondary) is treated as the "Secondary"
// unequip-socket family; Sidearm(13)/Melee(14) use the Primary family as a
// reasonable default, same as Primary(11) itself.
static bool weapon_slot_is_secondary_family(uint8_t slotIndex)
{
    return slotIndex == 12;
}

void ProxyManager::sync_active_weapon_hand(AActor* actor, RemotePlayer& player)
{
    if (!actor) return;

    const uint8_t newActive = player.activeWeaponSlot;
    if (newActive == player.handAttachedSlot) return;

    auto weaponVisualActorFor = [&](uint8_t slotIndex) -> void** {
        switch (slotIndex) {
            case 11: return &player.primaryWeaponVisualActor;
            case 12: return &player.secondaryWeaponVisualActor;
            case 13: return &player.sidearmVisualActor;
            case 14: return &player.meleeVisualActor;
            default: return nullptr;
        }
    };

    // Looks up an already-spawned item actor's root/mesh component the same
    // way spawn_and_equip_item_visual does — needed here because a
    // SnapToTarget re-attach resets relative transform to identity, so
    // apply_item_equipped_transform has to be re-run after every re-attach,
    // not just the original spawn.
    auto findItemRoot = [](AActor* itemActor) -> UObject* {
        UFunction* fn = itemActor->GetFunctionByNameInChain(L"GetSkeletalMeshComponent");
        if (!fn) fn = itemActor->GetFunctionByNameInChain(L"K2_GetRootComponent");
        UObject* root = nullptr;
        if (fn) itemActor->ProcessEvent(fn, &root);
        return root;
    };

    // Revert the previously hand-attached weapon back to its holster socket
    // via the same real EquipActorToSocket path sync_equipment used to put
    // it there originally, then reapply its EquippedTransform correction
    // (equip_actor_to_socket's own SnapToTarget attach resets it to
    // identity, same as any other re-attach).
    if (player.handAttachedSlot != 0xFF) {
        void** prevSlot = weaponVisualActorFor(player.handAttachedSlot);
        if (prevSlot && *prevSlot) {
            auto* prevActor = static_cast<AActor*>(*prevSlot);
            equip_actor_to_socket(actor, prevActor,
                                   weapon_slot_is_secondary_family(player.handAttachedSlot));

            std::string prevItemId;
            for (const auto& slot : player.equipment) {
                if (slot.slotIndex == player.handAttachedSlot) { prevItemId = slot.itemId; break; }
            }
            void* prevItemAsset = prevItemId.empty() ? nullptr : resolve_item_asset(prevItemId);
            if (prevItemAsset) apply_item_equipped_transform(findItemRoot(prevActor), prevItemAsset);
        }
    }

    player.handAttachedSlot = newActive;
    if (newActive == 0xFF) return;

    void** newSlot = weaponVisualActorFor(newActive);
    if (!newSlot || !*newSlot) return;

    std::string itemId;
    for (const auto& slot : player.equipment) {
        if (slot.slotIndex == newActive) { itemId = slot.itemId; break; }
    }
    if (itemId.empty()) return;

    void* itemAsset = resolve_item_asset(itemId);
    if (!itemAsset) return;

    const RawFGameplayTag equipSocket = *reinterpret_cast<RawFGameplayTag*>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x280);

    auto* newActor = static_cast<AActor*>(*newSlot);
    const bool attached = reattach_weapon_visual_to_socket(actor, newActor, equipSocket);
    if (attached) apply_item_equipped_transform(findItemRoot(newActor), itemAsset);
    Output::send<LogLevel::Normal>(
        STR("SDB: hand-attach slot={:d} itemId={} socket_ci={:d} ok={:d}\n"),
        newActive, widen(itemId), equipSocket.ComparisonIndex, attached);
}

void ProxyManager::sync_weapon_attachments(RemotePlayer& player)
{
    if (!player.weaponAttachmentsDirty) return;
    player.weaponAttachmentsDirty = false;

    // Every slot mod.cpp's read_local_weapon_attachments() checks
    // (Facewear/Headwear/Eyewear/Backpack + the 4 weapon slots) — the only
    // slots with a real spawned visual actor to attach onto. Not
    // weapon-specific: night vision on a helmet or a battery in a flashlight
    // uses the same BP_JigPickupComponent/RepAttachments mechanism as a
    // weapon scope, just a different owning slot.
    struct SlotInfo { uint8_t slotIndex; void** weaponActor; };
    const SlotInfo slots[] = {
        {0,  &player.facewearVisualActor},
        {1,  &player.headwearVisualActor},
        {2,  &player.eyewearVisualActor},
        {10, &player.backpackVisualActor},
        {11, &player.primaryWeaponVisualActor},
        {12, &player.secondaryWeaponVisualActor},
        {13, &player.sidearmVisualActor},
        {14, &player.meleeVisualActor},
    };

    for (const auto& s : slots) {
        auto* weaponActor = static_cast<AActor*>(*s.weaponActor);
        auto& appliedKey = player.weaponAttachmentsAppliedKey[s.slotIndex];
        auto& actors = player.weaponAttachmentActors[s.slotIndex];

        if (!weaponActor) {
            // No weapon actor for this slot right now (unequipped, or not
            // yet spawned this cycle) — clear any stale attachment actors.
            if (!actors.empty()) {
                for (void*& a : actors) destroy_actor_safe(a);
                actors.clear();
            }
            appliedKey.clear();
            continue;
        }

        // Cheap change-signature (concatenated itemIds) — avoids respawning
        // every attachment on every resend when nothing actually changed,
        // same lesson as the redundant-resend gate for Facewear/Eyewear/
        // Backpack above.
        std::string key;
        std::vector<const WeaponAttachmentEntry*> entries;
        for (const auto& e : player.weaponAttachments) {
            if (e.weaponSlotIndex != s.slotIndex) continue;
            entries.push_back(&e);
            key += e.itemId;
            key += '|';
        }
        if (key == appliedKey) continue;

        for (void*& a : actors) destroy_actor_safe(a);
        actors.clear();

        for (const WeaponAttachmentEntry* e : entries) {
            void* itemAsset = resolve_item_asset(e->itemId);
            if (!itemAsset) continue;
            AActor* attachmentActor = spawn_and_attach_weapon_attachment(weaponActor, itemAsset);
            if (attachmentActor) actors.push_back(attachmentActor);
        }

        appliedKey = key;
    }
}

// Resolves an asset by its own short object name (e.g. "Chr_MaleHair3") via
// UObjectGlobals::FindObject against already-loaded objects — the small,
// fixed set of character-creation hair/beard styles is always resident once
// any character exists, so no on-demand loading is needed. Class=nullptr,
// InOuter=nullptr means "search everywhere, any class" — this is a narrow
// enough content domain (these specific style asset names) that a
// class-unfiltered name match is an acceptable risk, unlike guessing a raw
// memory offset.
static UObject* find_object_by_short_name(const std::wstring& name)
{
    if (name.empty()) return nullptr;
    // Live-tested 2026-08-12: InOuter=nullptr found nothing (SetStaticMesh/
    // SetMaterial UFUNCTIONs were both confirmed present, only the lookup
    // itself failed). Real UE5's "search every package" sentinel for
    // StaticFindObject-family calls is ANY_PACKAGE — a well-known, publicly
    // documented API constant, (UObject*)-1, not literally nullptr (nullptr
    // means "no outer at all", a much narrower search that excludes nearly
    // every real asset). Not a guess at internal layout like the ClassPrivate
    // incident — this is a stable, public UE convention.
    UObject* const kAnyPackage = reinterpret_cast<UObject*>(static_cast<intptr_t>(-1));
    return UObjectGlobals::FindObject(nullptr, kAnyPackage, name.c_str());
}

void ProxyManager::sync_pawn_appearance(AActor* actor, RemotePlayer& player)
{
    if (!actor || !player.appearanceDirty) return;
    player.appearanceDirty = false;

    const auto& a = player.appearance;
    std::string key = std::string(a.isMale ? "M" : "F") + "|" +
        a.hairMeshName + "|" + a.hairColorName + "|" + a.beardMeshName + "|" + a.beardColorName +
        "|" + a.skinColorName;
    for (const auto& s : a.bodyPartMeshNames) { key += "|"; key += s; }
    key += "|" + a.mouthMeshName + "|" + a.eyebrowsMeshName + "|" +
        a.accessory1MeshName + "|" + a.accessory2MeshName + "|" + a.accessory3MeshName;
    if (key == player.appliedAppearanceKey) return;
    player.appliedAppearanceKey = key;

    *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(actor) + 0x15A0) = a.isMale;

    // HairMesh/BeardMesh components (BP_PlayerCharacter.hpp @0x7C0/@0x7C8) —
    // same SetStaticMesh/SetMaterial UFUNCTIONs as any other
    // UStaticMeshComponent, research/CXXHeaderDump/Engine.hpp.
    struct PartInfo { uintptr_t compOffset; const std::string& meshName; const std::string& colorName; bool isBeard; };
    const PartInfo parts[] = {
        { 0x7C0, a.hairMeshName,  a.hairColorName,  false },
        { 0x7C8, a.beardMeshName, a.beardColorName, true },
    };
    for (const auto& part : parts) {
        auto* comp = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(actor) + part.compOffset);
        if (!comp) {
            debug_log("sync_pawn_appearance: component at offset 0x" +
                      [&]{ char b[8]; snprintf(b, sizeof(b), "%llx", (unsigned long long)part.compOffset); return std::string(b); }() +
                      " is null");
            continue;
        }

        // Beards are a male-only customization in this game — a female
        // character's BeardMesh component can still have some placeholder
        // mesh assigned even though the game itself keeps it hidden.
        // Forcing SetVisibility(true) unconditionally (added to fix the
        // beard never showing at all) incorrectly revealed it on female
        // proxies too. Explicitly hide it instead when the synced appearance
        // is female, regardless of what mesh name came across.
        if (part.isBeard && !a.isMale) {
            UFunction* visFn = comp->GetFunctionByNameInChain(L"SetVisibility");
            if (visFn) {
                struct Params { bool bNewVisibility = false; bool bPropagateToChildren = false; } vparams;
                comp->ProcessEvent(visFn, &vparams);
            }
            continue;
        }

        if (!part.meshName.empty()) {
            auto* mesh = find_object_by_short_name(widen(part.meshName));
            UFunction* fn = comp->GetFunctionByNameInChain(L"SetStaticMesh");
            std::string meshClass = mesh ? [&]{
                std::wstring wn = mesh->GetFullName();
                int need = WideCharToMultiByte(CP_UTF8, 0, wn.c_str(), -1, nullptr, 0, nullptr, nullptr);
                std::string s(need > 0 ? (size_t)(need - 1) : 0, '\0');
                if (need > 0) WideCharToMultiByte(CP_UTF8, 0, wn.c_str(), -1, s.data(), need, nullptr, nullptr);
                return s;
            }() : std::string("<null>");
            debug_log("sync_pawn_appearance: mesh=" + part.meshName + " found=" + std::to_string(mesh != nullptr) +
                      " resolved=" + meshClass + " SetStaticMesh_found=" + std::to_string(fn != nullptr));
            if (mesh && fn) {
                struct Params { UObject* NewMesh = nullptr; bool ReturnValue = false; } params;
                params.NewMesh = mesh;
                comp->ProcessEvent(fn, &params);
            }
            // The beard component in particular is plausibly hidden by
            // default on a fresh proxy spawn (no beard = hidden component) —
            // assigning a real mesh doesn't necessarily reveal it on its own.
            UFunction* visFn = comp->GetFunctionByNameInChain(L"SetVisibility");
            if (visFn) {
                struct Params { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
                comp->ProcessEvent(visFn, &vparams);
            }
        }
        if (!part.colorName.empty()) {
            auto* mat = find_object_by_short_name(widen(part.colorName));
            UFunction* fn = comp->GetFunctionByNameInChain(L"SetMaterial");
            std::string matClass = mat ? [&]{
                std::wstring wn = mat->GetFullName();
                int need = WideCharToMultiByte(CP_UTF8, 0, wn.c_str(), -1, nullptr, 0, nullptr, nullptr);
                std::string s(need > 0 ? (size_t)(need - 1) : 0, '\0');
                if (need > 0) WideCharToMultiByte(CP_UTF8, 0, wn.c_str(), -1, s.data(), need, nullptr, nullptr);
                return s;
            }() : std::string("<null>");
            debug_log("sync_pawn_appearance: color=" + part.colorName + " found=" + std::to_string(mat != nullptr) +
                      " resolved=" + matClass + " SetMaterial_found=" + std::to_string(fn != nullptr));
            if (mat && fn) {
                struct Params { int32_t ElementIndex = 0; UObject* Material = nullptr; } params;
                params.Material = mat;
                comp->ProcessEvent(fn, &params);
            }
        }
    }

    // Mouth/EyebrowsMesh/Accessory1-3 (BP_PlayerCharacter.hpp) — same
    // mesh-only swap as hair/beard's mesh half, no dedicated color property
    // for any of these (confirmed: no "Mouth Color"/"Accessory Color" etc.
    // fields anywhere in the header dump).
    struct MeshOnlyPart { uintptr_t compOffset; const std::string& meshName; };
    const MeshOnlyPart meshOnlyParts[] = {
        { 0x0740, a.mouthMeshName },
        { 0x0790, a.eyebrowsMeshName },
        { 0x0758, a.accessory1MeshName },
        { 0x0750, a.accessory2MeshName },
        { 0x0748, a.accessory3MeshName },
    };
    for (const auto& part : meshOnlyParts) {
        auto* comp = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(actor) + part.compOffset);
        if (!comp) continue;

        // Empty means the real player has nothing in this slot — the proxy
        // spawns as a generic BP_PlayerCharacter_C, which can have some
        // non-empty default already assigned (a scar accessory, live-tested
        // 2026-08-12) that a "do nothing when empty" gate would leave
        // stuck forever. Explicitly hide the component instead of skipping.
        if (part.meshName.empty()) {
            UFunction* hideFn = comp->GetFunctionByNameInChain(L"SetVisibility");
            if (hideFn) {
                struct Params { bool bNewVisibility = false; bool bPropagateToChildren = false; } hparams;
                comp->ProcessEvent(hideFn, &hparams);
            }
            continue;
        }

        auto* mesh = find_object_by_short_name(widen(part.meshName));
        UFunction* fn = comp->GetFunctionByNameInChain(L"SetStaticMesh");
        debug_log("sync_pawn_appearance: meshOnly=" + part.meshName + " found=" + std::to_string(mesh != nullptr) +
                  " SetStaticMesh_found=" + std::to_string(fn != nullptr));
        if (mesh && fn) {
            struct Params { UObject* NewMesh = nullptr; bool ReturnValue = false; } params;
            params.NewMesh = mesh;
            comp->ProcessEvent(fn, &params);
        }
        UFunction* visFn = comp->GetFunctionByNameInChain(L"SetVisibility");
        if (visFn) {
            struct Params { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
            comp->ProcessEvent(visFn, &vparams);
        }
    }

    // Naked-body SkeletalMeshComponents (BP_PlayerCharacter.hpp) — order
    // matches sdb::PawnAppearance::bodyPartMeshNames / mod.cpp's own copy of
    // this table exactly.
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

    // Just flipping IsPlayerMale? doesn't retroactively change which
    // body-shape mesh a proxy is using — it was spawned once at a fixed
    // default gender, and the actual SkeletalMesh per part has to be synced
    // too (same principle as hair/beard's own dedicated mesh sync). Set the
    // real mesh via SetSkinnedAssetAndUpdate — the same UFUNCTION
    // equip_clothing_to_mesh already uses successfully for Clothing_*
    // components — *before* applying SkinColor below, so the material
    // override isn't reset by a subsequent mesh change.
    for (int i = 0; i < sdb::BODY_PART_COUNT; ++i) {
        const auto& meshName = a.bodyPartMeshNames[i];
        auto* comp = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(actor) + kBodyPartOffsets[i]);
        if (!comp) continue;

        // Live-tested 2026-08-13: the real game clears a body-part slot's
        // own mesh to empty when clothing covers it (e.g. Torso goes from
        // "SK_Chr_Female_Torso" to empty the instant a shirt is equipped,
        // and back the instant it's removed) — read_local_pawn_appearance
        // already reports that correctly, but this loop only ever *applied*
        // a non-empty mesh and skipped empty ones entirely, never hiding
        // anything. A freshly-spawned proxy's default body-part mesh (e.g.
        // an underwear-style torso) was never cleared even once the real
        // player's shirt was on, so it stayed permanently visible and
        // z-fought with the Clothing_Torso mesh on top of it — this is what
        // showed up as the shirt/pants "pulsing". Same "sync needs a clear
        // case" fix as tonight's beard/accessory bugs, applied here too.
        if (meshName.empty()) {
            UFunction* visFn = comp->GetFunctionByNameInChain(L"SetVisibility");
            if (visFn) {
                struct Params { bool bNewVisibility = false; bool bPropagateToChildren = false; } vparams;
                comp->ProcessEvent(visFn, &vparams);
            }
            continue;
        }

        auto* mesh = find_object_by_short_name(widen(meshName));
        UFunction* fn = comp->GetFunctionByNameInChain(L"SetSkinnedAssetAndUpdate");
        debug_log("sync_pawn_appearance: bodyPart[" + std::to_string(i) + "]=" + meshName +
                  " found=" + std::to_string(mesh != nullptr) + " SetSkinnedAssetAndUpdate_found=" + std::to_string(fn != nullptr));
        if (mesh && fn) {
            struct Params { UObject* NewMesh = nullptr; bool bReinitPose = true; } params;
            params.NewMesh = mesh;
            comp->ProcessEvent(fn, &params);
        }

        // Re-show in case a previous cycle hid this component (the source
        // slot went from empty back to a real mesh — e.g. shirt removed).
        UFunction* visFn = comp->GetFunctionByNameInChain(L"SetVisibility");
        if (visFn) {
            struct Params { bool bNewVisibility = true; bool bPropagateToChildren = false; } vparams;
            comp->ProcessEvent(visFn, &vparams);
        }
    }

    // SkinColor (BP_PlayerCharacter.hpp @0x15A8) applies across all nine of
    // the same body-part components — same SetMaterial(0, ...) call as
    // hair/beard color.
    if (!a.skinColorName.empty()) {
        auto* skinMat = find_object_by_short_name(widen(a.skinColorName));
        debug_log("sync_pawn_appearance: skinColor=" + a.skinColorName + " found=" + std::to_string(skinMat != nullptr));
        if (skinMat) {
            int idx = 0;
            for (uintptr_t off : kBodyPartOffsets) {
                char pbuf[64];
                snprintf(pbuf, sizeof(pbuf), "sync_pawn_appearance: skinColor loop idx=%d off=0x%llx enter",
                         idx, static_cast<unsigned long long>(off));
                debug_log(pbuf);

                auto* comp = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(actor) + off);
                if (!comp) { debug_log("sync_pawn_appearance: skinColor loop comp is null, skip"); ++idx; continue; }
                UFunction* fn = comp->GetFunctionByNameInChain(L"SetMaterial");
                if (!fn) { debug_log("sync_pawn_appearance: skinColor loop SetMaterial NOT FOUND, skip"); ++idx; continue; }
                struct Params { int32_t ElementIndex = 0; UObject* Material = nullptr; } params;
                params.Material = skinMat;

                snprintf(pbuf, sizeof(pbuf), "sync_pawn_appearance: skinColor loop idx=%d comp=0x%llx about to ProcessEvent",
                         idx, reinterpret_cast<unsigned long long>(comp));
                debug_log(pbuf);
                comp->ProcessEvent(fn, &params);
                debug_log("sync_pawn_appearance: skinColor loop ProcessEvent returned");
                ++idx;
            }
        }
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "sync_pawn_appearance: isMale=%d hairMesh=%s hairColor=%s beardMesh=%s beardColor=%s skinColor=%s",
             a.isMale, a.hairMeshName.c_str(), a.hairColorName.c_str(), a.beardMeshName.c_str(), a.beardColorName.c_str(),
             a.skinColorName.c_str());
    debug_log(buf);
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

    // Every weapon/equipment visual and weapon attachment is a separately
    // spawned actor, just attached to the proxy — destroying the proxy
    // itself does not cascade-destroy attached *actors* in UE5 (only
    // attached *components* die with their owner), so each one was leaking
    // into the world forever on disconnect (backpack/weapons staying behind
    // when the pawn vanished, live-tested 2026-08-12). Clean up everything
    // this player's proxy ever spawned before erasing its state.
    auto& p = it->second;
    destroy_actor_safe(p.primaryWeaponVisualActor);
    destroy_actor_safe(p.facewearVisualActor);
    destroy_actor_safe(p.headwearVisualActor);
    destroy_actor_safe(p.eyewearVisualActor);
    destroy_actor_safe(p.backpackVisualActor);
    destroy_actor_safe(p.secondaryWeaponVisualActor);
    destroy_actor_safe(p.sidearmVisualActor);
    destroy_actor_safe(p.meleeVisualActor);
    for (auto& [slot, actors] : p.weaponAttachmentActors) {
        for (void*& a : actors) destroy_actor_safe(a);
    }

    if (p.proxyActor)
        destroy_proxy(static_cast<AActor*>(p.proxyActor));

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
    p.aimPitchByte = m.aimState;
    p.movState  = m.movementState;
    p.activeWeaponSlot = m.animationState;
    p.velocityX = m.velocityX;
    p.velocityY = m.velocityY;
    p.velocityZ = m.velocityZ;
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

void ProxyManager::on_weapon_attachments(uint64_t playerId, const WeaponAttachments& a)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    it->second.weaponAttachments = a.entries;
    it->second.weaponAttachmentsDirty = true;
}

void ProxyManager::on_pawn_appearance(uint64_t playerId, const PawnAppearance& a)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    it->second.appearance = a;
    it->second.appearanceDirty = true;
}

// Session 51 first tried writing ACharacter::CharacterMovement's Velocity
// (+0x328/+0xB8-C8) directly — crashed the mod once (SDB.log: "ready" then
// immediately "unloaded"), then crashed the real game process outright on a
// retry even with an SEH guard around the write (the fault was downstream,
// in a later engine tick reacting to the change, not synchronous in the
// write itself — SEH around just the write can't catch that). A read-only
// GetController() call on the proxy crashed the game a third time, enough
// to rule out CharacterMovementComponent/Controller entirely rather than
// keep probing it live.
//
// Session 52: found a different, verified-safe path instead. Player_AnimBP_C's
// "Speed" (the value its own GetSpeed&Direction function computes via
// VSize(Velocity) and that the AnimGraph's locomotion blend actually reads)
// is a private AnimInstance-instance scratch DoubleProperty — not part of
// CharacterMovementComponent, not replicated, no custom setter. Found via:
// (1) dumping GetSpeed&Direction's own Kismet bytecode (bytecode_dump.flag)
// to get the exact instance-property pointer it writes into that frame's
// FProperty*, not a byte offset; (2) live memory-dumping that FProperty's
// own struct (mem_dump.flag "abs <addr> <count>") and confirming its
// FField::NamePrivate (+0x20, same offset already established elsewhere in
// this project) resolves via resolve_fname.flag to literally "Speed"; (3)
// finding a plausible Offset_Internal value (23232) further into that same
// dump and validating it directly on the LOCAL player by reading a double
// at (AnimInstance-object-base + 23232) live while idle/walking/aim-walking
// — 0.0 / 400.0 / 250.0, all correct, including the aim-walking case that
// had broken an earlier, less rigorous live-correlation guess. Offset is
// object-relative (not relative to the "__AnimBlueprintMutables" struct
// pointer FModel's export groups it under), confirmed by testing against
// the AnimInstance object's own base address, not that struct's address.
//
// Writing here (rather than Velocity) never touches CharacterMovementComponent
// or Controller at all, so the specific mechanism behind all three crashes
// above shouldn't apply — still SEH-guarded regardless, given today's crash
// history, so a wrong assumption here degrades to "one bad tick logged" and
// not a repeat of any of those three.
constexpr uintptr_t kAnimBPSpeedOffset = 23232;

struct ProxySpeedCtx { AActor* actor; double speed; };

static void do_apply_proxy_speed(void* ctxRaw)
{
    auto* ctx = static_cast<ProxySpeedCtx*>(ctxRaw);

    auto** meshSlot = static_cast<UObject**>(ctx->actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return;

    UFunction* getAnimFn = mesh->GetFunctionByNameInChain(L"GetAnimInstance");
    if (!getAnimFn) return;
    struct Params { UObject* ReturnValue = nullptr; } aparams;
    mesh->ProcessEvent(getAnimFn, &aparams);
    if (!aparams.ReturnValue) return;

    auto* speedPtr = reinterpret_cast<double*>(reinterpret_cast<uintptr_t>(aparams.ReturnValue) + kAnimBPSpeedOffset);
    *speedPtr = ctx->speed;

    static uint64_t s_lastLogUs = 0;
    const uint64_t nowLog = now_micros();
    if (nowLog - s_lastLogUs >= 1'000'000ULL) {
        s_lastLogUs = nowLog;
        char buf[128];
        snprintf(buf, sizeof(buf), "apply_proxy_speed_safe: animInstance=0x%llx wrote=%.2f readback=%.2f",
                 reinterpret_cast<unsigned long long>(aparams.ReturnValue), ctx->speed, *speedPtr);
        debug_log(buf);
    }
}

// Deadzone: a standing-still real player's read velocity isn't always a
// clean exact zero (residual/friction noise off ACharacter::
// CharacterMovement). Without this, a proxy would visibly "walk in place"
// while genuinely stationary.
static void apply_proxy_speed_safe(AActor* actor, float vx, float vy, float vz)
{
    constexpr double kVelocityDeadzone = 15.0; // UE units/s (cm/s)
    const double speed2 = static_cast<double>(vx) * vx + static_cast<double>(vy) * vy + static_cast<double>(vz) * vz;
    const double speed = speed2 < (kVelocityDeadzone * kVelocityDeadzone) ? 0.0 : std::sqrt(speed2);

    ProxySpeedCtx ctx{actor, speed};
    if (!seh_invoke(do_apply_proxy_speed, &ctx))
        debug_log("apply_proxy_speed_safe: crashed applying speed, caught via SEH");
}

// Session 52 (later): apply_proxy_speed_safe above is confirmed correct
// (write+readback both verified live, 400.00 while walking) but doesn't
// actually animate the proxy — because Player_AnimBP_C's own
// BlueprintThreadSafeUpdateAnimation calls GetSpeed&Direction every single
// frame, which unconditionally recomputes Speed fresh from the proxy's own
// (untouched, always-zero) CharacterMovementComponent::Velocity — stomping
// our injected value again before it's ever used for rendering. Writing
// Speed directly was fighting a per-frame race we can't win; the real fix
// is feeding the actual input (Velocity) so the engine's own computation
// produces the right Speed naturally, every frame, on its own.
//
// This is the same Velocity write reverted twice earlier tonight after two
// crashes and a deadlock — but the actual root cause of all three has since
// been found and fixed (do_game_tick's recursive re-entrancy through our
// own ProcessEvent hook, see that function's own comment) and was never
// specific to Velocity itself. Retrying now that the real cause is gone.
struct ProxyVelocityCtx { AActor* actor; double vx, vy, vz; };

static void do_apply_proxy_velocity(void* ctxRaw)
{
    auto* ctx = static_cast<ProxyVelocityCtx*>(ctxRaw);
    const auto moveComp = *reinterpret_cast<uintptr_t*>(
        reinterpret_cast<uintptr_t>(ctx->actor) + 0x328);
    if (!moveComp) return;
    *reinterpret_cast<double*>(moveComp + 0xB8) = ctx->vx;
    *reinterpret_cast<double*>(moveComp + 0xC0) = ctx->vy;
    *reinterpret_cast<double*>(moveComp + 0xC8) = ctx->vz;
}

static void apply_proxy_velocity_safe(AActor* actor, float vx, float vy, float vz)
{
    constexpr double kVelocityDeadzone = 15.0; // UE units/s (cm/s)
    const double speed2 = static_cast<double>(vx) * vx + static_cast<double>(vy) * vy + static_cast<double>(vz) * vz;
    const bool idle = speed2 < (kVelocityDeadzone * kVelocityDeadzone);

    ProxyVelocityCtx ctx{actor, idle ? 0.0 : static_cast<double>(vx),
                                idle ? 0.0 : static_cast<double>(vy),
                                idle ? 0.0 : static_cast<double>(vz)};
    if (!seh_invoke(do_apply_proxy_velocity, &ctx))
        debug_log("apply_proxy_velocity_safe: crashed applying velocity, caught via SEH");
}

// Look-direction sync (2026-08-13): applying the sender's real camera pitch
// (see mod.cpp's read_local_aim_pitch for how it's captured/quantized) to a
// proxy's AnimBP "Pitch" property from here (a same-tick write) turned out
// to be unwinnable — GetAimOffset hard-resets Pitch to 0 every single frame
// for a non-locally-controlled proxy (confirmed live via full bytecode
// tracing + direct value sampling: bSel8=0 on the actual proxy selects a
// branch that reads exactly (0,0,0), producing a deterministic zero every
// frame, not occasional garbage). A tick()-driven write here always loses
// that race before the next render. Moved to mod.cpp's
// on_process_event_post instead, which runs immediately after GetAimOffset
// itself completes and so always wins cleanly — see that function's comment
// for the full mechanism (UE4SS.dll's RegisterProcessEventPostCallback,
// verified live to actually exist in this build).

// Crouch/ADS/falling sync (2026-08-13): applied via mod.cpp's
// on_process_event_post instead of a tick()-driven write here — live
// testing showed a tick()-based write to IsCrouching/IsADS jittering
// (winning some frames, losing others) the same way Pitch lost its own
// race against GetAimOffset outright. See on_process_event_post's own
// comment for the fix (hooking GetLeftHandLoc, the last function in the
// AnimBP's known per-frame update sequence, to reapply all four proxy
// overrides — Pitch/IsCrouching/IsADS/Falling — after the whole block runs).

struct ProxyBodyYawCtx { AActor* actor; RemotePlayer* player; double desiredYaw; };

static void do_apply_proxy_body_yaw(void* ctxRaw)
{
    auto* ctx = static_cast<ProxyBodyYawCtx*>(ctxRaw);
    auto** meshSlot = static_cast<UObject**>(ctx->actor->GetValuePtrByPropertyNameInChain(L"Mesh"));
    UObject* mesh = (meshSlot && *meshSlot) ? *meshSlot : nullptr;
    if (!mesh) return;

    auto* relRot = reinterpret_cast<double*>(reinterpret_cast<uintptr_t>(mesh) + 0x0140);

    if (!ctx->player->meshBaselineCaptured) {
        ctx->player->meshBaselinePitch = relRot[0];
        ctx->player->meshBaselineYaw   = relRot[1];
        ctx->player->meshBaselineRoll  = relRot[2];
        ctx->player->meshBaselineCaptured = true;
    }

    relRot[0] = ctx->player->meshBaselinePitch;
    relRot[1] = ctx->player->meshBaselineYaw + ctx->desiredYaw;
    relRot[2] = ctx->player->meshBaselineRoll;
}

// Body-yaw sync (2026-08-13): the proxy's own CharacterMovementComponent has
// bOrientRotationToMovement=true (confirmed via FModel export of
// BP_PlayerCharacter's CDO), which resets the actor root's rotation from
// Velocity every physics tick — silently overriding any
// K2_SetActorLocationAndRotation rotation call (confirmed live: sampled
// actual actor yaw stayed pinned at 0.00 regardless of what teleport_proxy
// sent). Writing onto the Mesh component's own RelativeRotation instead
// sidesteps that mechanism, since bOrientRotationToMovement only touches
// the actor root, not the mesh's separate relative transform — same
// raw-offset RelativeRotation write (USceneComponent @ 0x0140) already
// proven safe for weapon-transform application elsewhere in this file. The
// mesh's own baked-in art-alignment offset (captured once as a baseline,
// see RemotePlayer::meshBaselineCaptured) is preserved, not overwritten.
static void apply_proxy_body_yaw_safe(AActor* actor, RemotePlayer& player, float desiredYawDegrees)
{
    ProxyBodyYawCtx ctx{actor, &player, static_cast<double>(desiredYawDegrees)};
    if (!seh_invoke(do_apply_proxy_body_yaw, &ctx))
        debug_log("apply_proxy_body_yaw_safe: crashed applying body yaw, caught via SEH");
}

// Smooths RemotePlayer::render{X,Y,Z,Yaw} toward the raw, packet-driven
// x/y/z/yaw each tick — see RemotePlayer::renderInitialized's own comment
// for why (raw fields only change once per network packet, ~50ms apart,
// producing a visible snap rather than continuous motion). Exponential
// smoothing (not a fixed-duration lerp) so it self-corrects regardless of
// actual packet jitter, with a distance-based snap for genuine teleports
// (respawn, etc.) so those don't visibly slide across the map.
static void update_proxy_render_smoothing(RemotePlayer& player)
{
    const uint64_t now = now_micros();

    if (!player.renderInitialized) {
        player.renderX = player.x;
        player.renderY = player.y;
        player.renderZ = player.z;
        player.renderYaw = player.yaw;
        player.renderInitialized = true;
        player.lastRenderTickUs = now;
        return;
    }

    const double dt = static_cast<double>(now - player.lastRenderTickUs) / 1'000'000.0;
    player.lastRenderTickUs = now;
    if (dt <= 0.0) return;

    constexpr double kTeleportDistSq = 500.0 * 500.0; // UE units — a real teleport/respawn, not normal movement
    const double dx = player.x - player.renderX;
    const double dy = player.y - player.renderY;
    const double dz = player.z - player.renderZ;
    if (dx * dx + dy * dy + dz * dz > kTeleportDistSq) {
        player.renderX = player.x;
        player.renderY = player.y;
        player.renderZ = player.z;
        player.renderYaw = player.yaw;
        return;
    }

    constexpr double kTau = 0.08; // seconds — smoothing time constant, tuned around the ~50ms packet interval
    const double factor = 1.0 - std::exp(-dt / kTau);
    player.renderX += static_cast<float>(dx * factor);
    player.renderY += static_cast<float>(dy * factor);
    player.renderZ += static_cast<float>(dz * factor);

    double yawDelta = static_cast<double>(player.yaw) - player.renderYaw;
    while (yawDelta > 180.0)  yawDelta -= 360.0;
    while (yawDelta < -180.0) yawDelta += 360.0;
    player.renderYaw += static_cast<float>(yawDelta * factor);
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
                if (player.proxyActor) player.proxySpawnedAtUs = now;
            }
            continue;
        }

        if (!player.dead) {
            update_proxy_render_smoothing(player);
            teleport_proxy(static_cast<AActor*>(player.proxyActor),
                           player.renderX, player.renderY, player.renderZ, player.renderYaw);
            apply_proxy_body_yaw_safe(static_cast<AActor*>(player.proxyActor),
                                       player, player.renderYaw);
        }

        // Session 52: two separate live crashes (once a real crash, once a
        // genuine deadlock traced via debug.log — see RemotePlayer::
        // proxySpawnedAtUs's own comment) both happened at DIFFERENT
        // specific call sites within this sync burst, both right after a
        // fresh proxy connected. That points at the proxy's own components
        // not being fully ready the instant it's spawned, not one
        // specific bad call — give it a short grace period before hitting
        // it with the full sync burst, same 2s throttle already used
        // elsewhere in this project for "don't hammer a freshly-changed
        // thing every tick".
        if (now_micros() - player.proxySpawnedAtUs < 2'000'000ULL) continue;

        // Writing real Velocity (not the AnimBP's own Speed scratch var —
        // see apply_proxy_velocity_safe's own comment for why) now that the
        // actual crash cause (do_game_tick's recursive re-entrancy through
        // our own ProcessEvent hook) is fixed, not specific to this write.
        apply_proxy_velocity_safe(static_cast<AActor*>(player.proxyActor),
                                   player.velocityX, player.velocityY, player.velocityZ);
        // Aim-pitch/crouch/ADS/falling are applied via mod.cpp's
        // on_process_event_post instead of here — GetAimOffset (and
        // apparently whatever sets IsCrouching/IsADS too) gets recomputed
        // every frame for a non-locally-controlled proxy, so a same-tick
        // write here would just lose that race. See on_process_event_post's
        // own comment.

        sync_equipment(static_cast<AActor*>(player.proxyActor), player);
        sync_active_weapon_hand(static_cast<AActor*>(player.proxyActor), player);
        sync_weapon_attachments(player);
        sync_pawn_appearance(static_cast<AActor*>(player.proxyActor), player);
    }
}

void ProxyManager::teleport_proxy(AActor* actor, float x, float y, float z, float yaw)
{
    if (!actor) return;
    FVector  loc{ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
    FRotator rot{ 0.0, static_cast<double>(yaw), 0.0 };
    FHitResult hit{};
    // Rotation portion is a no-op in practice — see apply_proxy_body_yaw_safe's
    // own comment for why — kept anyway since it's harmless and location
    // still needs setting through this same call.
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
