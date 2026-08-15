# Blueprint/DataAsset Catalog — PlayerModel

Reference catalog of `Content/PlayerModel/` (489 files). Generated from FModel JSON exports; no
bytecode available, so this covers names/flags/properties/data only — not implementation.

## Folder contents summary

489 files, almost all raw art assets: 128 `SkeletalMesh`, 159 `BodySetup`, 105
`AnimCurveMetaData`, 35 `PhysicsAsset`, 25 `Skeleton`, 20 `MaterialInstanceConstant`, 8
`Texture2D`, 4 `Material` — these are the actual body-part/clothing meshes under
`BodyParts/` (Torso, Arms, Biceps, Hands, Legs, LowerLegs, LowerThighs, Feet, Head+Features, split
Male/Female) and `Clothes/` (Armor, Civilian outfit sets, Gloves, Military, Paramedic, Police —
each split Male/Female where relevant), plus `Hair/`, `Items/`, `Materials/`, `Textures/`. None of
these carry structure/logic worth cataloging individually — noted here as confirmed present and
skipped per scope.

**Only 5 files hold actual structure/logic**, all directly in `PlayerModel/` or `PlayerModel/Other/`:
`BP_CharacterCreator.json`, `DT_Clothing.json`, `Other/BodyPartSettings.json`,
`Other/ClothingSettings.json`, `Other/Enum_Occupation.json`.

## MOST IMPORTANT FINDING — the FBodyPartSettings struct, found and fully enumerated

`Other/BodyPartSettings.json` **is** the `FBodyPartSettings` struct referenced throughout the
`04_ida_investigation_log.md` appearance-bug investigation. It is a Blueprint `UserDefinedStruct`
(not a native C++ struct — likely why the IDA log couldn't pin its native layout), package
`SurrounDead/Content/PlayerModel/Other/BodyPartSettings`. It is a flat, all-optional bag of 16
`ObjectProperty` fields, each an `ObjectProperty` → `SkeletalMesh` reference, `Edit |
BlueprintVisible` (no `Net`/replication flag on a struct field — structs don't carry per-field
replication anyway; the question is whether the *container variable* holding this struct is
replicated — see below):

```
MaleTorsoMesh (0x0000, size 0x8), MaleArmsMesh (0x0008, size 0x8), MaleBicepsMesh (0x0010, size 0x8),
MaleHandsMesh (0x0018, size 0x8), MaleLegsMesh (0x0020, size 0x8), MaleLowerThighsMesh (0x0028, size 0x8),
MaleLowerLegsMesh (0x0030, size 0x8), MaleFeetMesh (0x0038, size 0x8),
FemaleTorsoMesh (0x0040, size 0x8), FemaleArmsMesh (0x0048, size 0x8), FemaleBicepsMesh (0x0050, size 0x8),
FemaleHandsMesh (0x0058, size 0x8), FemaleLegsMesh (0x0060, size 0x8), FemaleLowerThighsMesh (0x0068, size 0x8),
FemaleLowerLegsMesh (0x0070, size 0x8), FemaleFeetMesh (0x0078, size 0x8)
```
(Real offsets confirmed via `CXXHeaderDump/BodyPartSettings.hpp` — `FBodyPartSettings`, total size 0x80.)

Every field is nullable/optional (`SkeletalMesh` object reference, defaults to `null` in most
data-table rows — see below). This struct is purely a "which mesh(es) does this clothing item
override, per sex, per body segment" descriptor — it is a *delta/override* payload, not the
character's full current appearance state.

### ClothingSettings — the wrapping struct actually used as row type

`Other/ClothingSettings.json` is a second `UserDefinedStruct` that wraps `BodyPartSettings`:
```
MaleMesh          ObjectProperty -> SkeletalMesh   (the clothing item's own mesh, male)               (0x0000, size 0x8)
FemaleMesh        ObjectProperty -> SkeletalMesh   (the clothing item's own mesh, female)              (0x0008, size 0x8)
UpdateAllBodyParts?  BoolProperty                  (bIsNativeBool; toggles whether to also swap the nested BodyPartSettings segment meshes)  (0x0010, size 0x1)
BodyPartSettings  StructProperty -> BodyPartSettings (nested, 16-field override struct above)          (0x0018, size 0x80)
```
(Real offsets confirmed via `CXXHeaderDump/ClothingSettings.hpp` — `FClothingSettings`, total size 0x98.)
This is the actual per-clothing-item appearance descriptor: a top-level garment mesh (male/female)
plus an optional flag+struct to also override specific underlying body-part meshes (e.g. long
sleeves replacing the bare-arms mesh). This confirms the investigation's model: clothing
application is data-driven per-item, and "which underlying body meshes get swapped" is itself
conditional on `UpdateAllBodyParts?` — a gap here (client not receiving/evaluating this flag
correctly, or the RPC firing before this row is resolved from `DT_Clothing`) is a very plausible
root cause for intermittent mesh non-application, independent of any replication-flag gap.

### DT_Clothing — the DataTable, 80 rows

`DT_Clothing.json` is a `DataTable` with `RowStruct = ClothingSettings`. **80 rows**, one per
clothing item across every category (armor, coats, pants, shirts, boots, gloves, etc. — e.g.
`CivilianBodyArmor`, `MilitaryHeavyArmor`, `HeavyCoat`, `RiotPoliceArmor`, `WinterCoat`...).

Full sweep of all 80 rows (not just a sample):
- `UpdateAllBodyParts? == true` in exactly **1** row: `Robe`.
- Rows with at least one non-null nested `BodyPartSettings` mesh field (i.e. actually using the
  per-segment override): **20 of 80** —
  `FingerlessGlovesBlack/Blue/Green/Orange`, `BlueShorts/GreenShorts/OrangeShorts/RedShorts`,
  `BlackPlaidShirt/BluePlaidShirt/OrangePlaidShirt/RedPlaidShirt`,
  `BlueShirt/GreenShirt/OrangeShirt`, `HawaiianShirt`, `SwimmingTrunks`, `Slippers`, `Robe`,
  `OilRigJacket`.
- The other 60 rows rely solely on the top-level `MaleMesh`/`FemaleMesh` garment mesh, with the
  nested struct entirely null.

So the nested override mechanism **is** used, by a meaningful minority (25%) of items — mostly
short-sleeve/short-leg garments (shorts, short-sleeve/plaid/Hawaiian shirts, fingerless gloves,
swim trunks, slippers, a robe, an oil-rig jacket) that need to swap in bare-arm/bare-leg segment
meshes underneath. Only `Robe` sets `UpdateAllBodyParts? = true`; the other 19 rows populate
individual nested mesh fields directly while leaving that bool `false` — meaning the bool's exact
semantics (gate vs. redundant/legacy flag) aren't fully clear from data alone. **These 20 items are
the highest-risk candidates for the intermittent appearance-clearing bug**, since they're the only
ones exercising the more complex nested-override code path through
`Svr_AttachClothing`/`MC_AttachClothing`/`EquipClothingToMesh` — worth prioritizing them in live
repro testing over single-mesh items like plain pants/boots/armor.

**Neither `BodyPartSettings` nor `ClothingSettings` nor `DT_Clothing` carries any replication
metadata themselves** — they're pure data assets (struct/table definitions with static default
data), not replicated UObject properties. This is expected: the actual `Net`/`RepNotify` flags
live on the *character's* variables that hold references into this data (as already documented in
`bp_catalog_player_core.md`: `BP_PlayerCharacter_C`'s `Clothing_*`/mesh properties are **not**
replicated, only the paired `*Equipped?` bools are `Net | RepNotify`). This folder's data confirms
the *shape* of what's being applied but doesn't itself add a new replication gap — it reinforces
that appearance state travels as one-shot RPC payloads (`Svr_AttachClothing`/`MC_AttachClothing`)
resolved against this table, with no replicated property fallback if a client misses the RPC.

### Enum_Occupation

`Other/Enum_Occupation.json` — Blueprint `UserDefinedEnum`, 13 values (Police Officer, Soldier,
Pizza Deliverer, Lawyer, Student, Mechanic, Fire Officer, Medic, Priest, Teacher, Retired,
Unemployed, Other). Cosmetic/flavor enum, likely a starting-occupation selector in character
creation; not appearance-application logic itself, no properties/functions to catalog.

---

## BP_CharacterCreator_C

`PlayerModel/BP_CharacterCreator.json` — Actor Blueprint (`SuperStruct: Class'Actor'`), 47 total
entries. This is the **character-creator preview mannequin actor** — a standalone Actor (not the
player Character/Pawn class) with one `SkeletalMeshComponent` per body segment
(`Torso`, `Arms`, `Hands`, `Legs`, `LowerLegs`, `Feet`, `Head`, plus a base `SkeletalMesh` — no
Biceps/LowerThighs component despite those existing as mesh slots in `BodyPartSettings`, so this
preview actor's component set is a subset of the full segment list) and `StaticMeshComponent`s for
`HairMesh`, `BeardMesh`, `EyebrowsMesh`, `Mouth`, `Accessory1/2/3`. Also has `PointLight1`,
`PointLight` (two point lights + a `CurveFloat`/`TimelineTemplate` for a light-intensity timeline)
and a `DefaultSceneRoot`.

Real offsets confirmed via `CXXHeaderDump/BP_CharacterCreator.hpp` (`ABP_CharacterCreator_C : public AActor`, size 0x340):
`Torso (0x02D8, size 0x8)`, `SkeletalMesh (0x02E0, size 0x8)`, `Arms (0x0318, size 0x8)`,
`Hands (0x0320, size 0x8)`, `Legs (0x0300, size 0x8)`, `LowerLegs (0x02F8, size 0x8)`,
`Feet (0x0308, size 0x8)`, `head (0x0310, size 0x8)`, `HairMesh (0x02E8, size 0x8)`,
`BeardMesh (0x02F0, size 0x8)`, `EyebrowsMesh (0x02D0, size 0x8)`, `Mouth (0x02B0, size 0x8)`,
`Accessory1 (0x02C8, size 0x8)`, `Accessory2 (0x02C0, size 0x8)`, `Accessory3 (0x02B8, size 0x8)`,
`PointLight1 (0x02A0, size 0x8)`, `PointLight (0x02A8, size 0x8)`, `DefaultSceneRoot (0x0328, size 0x8)`.

Functions (all local, no RPCs — this actor is a client-side-only preview widget-world actor, not
networked):
```
ExecuteUbergraph_BP_CharacterCreator   FUNC_Final | FUNC_UbergraphFunction
LightTL__FinishedFunc                  FUNC_BlueprintEvent
LightTL__UpdateFunc                    FUNC_BlueprintEvent
SetDefault                             FUNC_BlueprintCallable | FUNC_BlueprintEvent
ToggleLight                            FUNC_BlueprintCallable | FUNC_BlueprintEvent
UserConstructionScript                 FUNC_Event | FUNC_Public | FUNC_BlueprintCallable | FUNC_BlueprintEvent
```
`SetDefault` is the only function suggestive of appearance logic (likely resets the preview
mannequin's meshes to a default set) — no bytecode available to confirm what it actually writes.
Not networked, so out of scope for the live 2-player replication bug, but useful as a reference for
the full canonical component/segment naming scheme the game uses elsewhere.

---

## Bottom line for the investigation

- The `FBodyPartSettings`/`ClothingSettings` structs and `DT_Clothing` table are confirmed found
  and fully enumerated — pure static data, 16 optional per-sex-per-segment mesh override fields
  wrapped by a garment-mesh + toggle struct, 80 data rows.
- No replication flags exist here because this is data-asset content, not a UObject with
  replicated properties — consistent with prior findings that the replication gap lives on
  `BP_PlayerCharacter_C`'s variables, not in this data layer.
- New, useful detail for the investigation: 20 of 80 clothing rows (shorts, short-sleeve/plaid
  shirts, fingerless gloves, swim trunks, slippers, robe, oil-rig jacket) actively use the nested
  per-body-part override mechanism — these are the highest-risk items for the intermittent
  mesh-clearing bug and should be prioritized in live repro testing.
