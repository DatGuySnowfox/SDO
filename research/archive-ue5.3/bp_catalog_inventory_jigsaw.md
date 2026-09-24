# BP Catalog: JigSInventory + Inventory (equip/multiplayer-item system)

Static catalog from FModel JSON export. No bytecode — function name + `FunctionFlags` only.
Scope: `Content/JigSInventory/Jigsaw/{Components,Pickup,Data,ItemInspector}` and `Content/Inventory/{BP_Ingame,BP_Inventory,Containers,Crafting,Items,Traders}`.

---

## HIGH-VALUE FOR THE EQUIP/CLOTHING INVESTIGATION

### `BP_JigHelperComp_C`
`JigSInventory/Jigsaw/Components/BP_JigHelperComp.json`

This is almost certainly the actual object behind the `BP_JigHelperComp_C` component seen attached to `BP_PlayerCharacter_C` in the investigation log. It owns the equip-slot state directly.

**Replicated properties (class-level, on the `BlueprintGeneratedClass` entry, all `RepNotify` except where noted):**
| Property | Type | Flags |
|---|---|---|
| `RepActorsData` | ArrayProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| RepNotify` — **`RepActorsData` (0x0AE0, size 0x10)**, `TArray<FS_RepActorData>` — confirmed live via `CXXHeaderDump/BP_JigHelperComp.hpp` |
| `RepPrimitiveActorsData` | ArrayProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| RepNotify` — **`RepPrimitiveActorsData` (0x0AD0, size 0x10)**, `TArray<FS_RepNonActorData>` — confirmed live via `CXXHeaderDump/BP_JigHelperComp.hpp` (matches investigation-log finding) |
| `ActiveWeapon` | StructProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| RepNotify` — **`ActiveWeapon` (0x0B98, size 0x8)**, `FGameplayTag` |

**REAL OFFSETS from `CXXHeaderDump/BP_JigHelperComp.hpp`** (`class UBP_JigHelperComp_C : public UActorComponent`, total size `0xC40`):
| Property | Type | Offset (size) |
|---|---|---|
| `UberGraphFrame` | `FPointerToUberGraphFrame` | 0x00A0 (0x8) |
| `EquipmentUIDs` | `TMap<FGameplayTag, FGuid>` | 0x00A8 (0x50) |
| `ServerEquippedItems` | `FS_ServerEquippedItems` | 0x00F8 (0x9D8) |
| `RepPrimitiveActorsData` | `TArray<FS_RepNonActorData>` | **0x0AD0 (0x10)** |
| `RepActorsData` | `TArray<FS_RepActorData>` | **0x0AE0 (0x10)** |
| `TraceToActors?` | bool | 0x0AF0 (0x1) |
| `EquipmentIDSlotConfig` | `TMap<FGameplayTag, FS_EquipmentIDInfo>` | 0x0AF8 (0x50) |
| `SpawnedCustomPrimitives` | `TMap<FGameplayTag, ABP_CustomPrimitiveComp_C*>` | 0x0B48 (0x50) |
| `ActiveWeapon` | `FGameplayTag` | 0x0B98 (0x8) |
| `OnActiveWeaponSlotChanged` (delegate) | — | 0x0BA0 (0x10) |
| `PreviewChar` | `AActor*` | 0x0BB0 (0x8) |
| `CurrentTracActor` | `AActor*` | 0x0BB8 (0x8) |
| `CurrentInteractOptions` | `TMap<FGameplayTag, FText>` | 0x0BC0 (0x50) |
| `CurrentInteractOptionIndex` | int32 | 0x0C10 (0x4) |
| `InteractExecutedActor` | `AActor*` | 0x0C18 (0x8) |
| `OnPossessed` (delegate) | — | 0x0C20 (0x10) |
| `OnEquipmentUpdated` (delegate) | — | 0x0C30 (0x10) |

**`FS_RepActorData` layout** (`CXXHeaderDump/S_RepActorData.hpp`, size `0x10`): `Slot` (`FGameplayTag`, 0x0000, size 0x8), `Actor` (`AActor*`, 0x0008, size 0x8).

**`FS_RepNonActorData` layout** (`CXXHeaderDump/S_RepNonActorData.hpp`, size `0x18`): `Slot` (`FGameplayTag`, 0x0000, size 0x8), `DA` (`UJigsawItem_DataAsset_C*`, 0x0008, size 0x8), `Primitive` (`AActor*`, 0x0010, size 0x8). **Notable: `FS_RepNonActorData` is 0x18, one field (`Primitive`) longer than `FS_RepActorData`'s 0x10 — this is the actual "primitive" pointer this array's name refers to, i.e. `RepPrimitiveActorsData` carries a live `AActor* Primitive` per entry that `RepActorsData` does not. This directly substantiates the two-array-desync theory: `RepPrimitiveActorsData` isn't just a duplicate of `RepActorsData`, it's the array that owns the spawned mesh-actor pointer, so if it fails to replicate/update the primitive mesh legitimately won't show even when `RepActorsData`'s slot data is correct.**

`RepActorsData` matches the "equipped-item slot data" array named in the investigation brief exactly. `RepPrimitiveActorsData` is a second, parallel replicated array — worth checking whether it's kept in sync with `RepActorsData` or can desync (a plausible source of the intermittent detach bug: two separate replicated arrays that must stay consistent, only one of which may be updating on some code path).

**OnRep handlers (fire client-side on replication):**
- `OnRep_RepActorsData`
- `OnRep_RepPrimitiveActorsData`
- `OnRep_ActiveWeapon`

**Equip/attach-relevant functions (not RPCs themselves — this component has no `FUNC_Net` functions; it's data + local logic, called by `BP_JigMultiplayer_C`/native code):**
| Function | Flags (notable) |
|---|---|
| `Equip Actor to Socket` | BlueprintCallable/Event |
| `GetActiveWeapon`, `GetActiveWeaponSlot`, `SetActiveWeaponSlot` | Pure/Callable |
| `GetAllEquippedActors` | HasOutParms |
| `GetEquippedActorBySlot`, `GetEquippedInfoBySlot`, `SetEquippedInfoBySlot` | — |
| `GetPrimitiveBySlot`, `GetPrimitiveByUID`, `RemovePrimitiveActorBySlot` | operate on the *second* `RepPrimitiveActorsData` array — separate from the main equipped-actor array |
| `ForceRepPrimitiveActorSpawns` | explicitly forces re-replication of primitive actor spawns — name strongly suggests this exists because primitive actors (equipped meshes) sometimes fail to spawn/replicate normally. **High priority to bytecode-dump.** |
| `SpawnPrimitiveActor` | — |
| `OnPickupEquipped` | — |
| `PreviewAttachmentUpdated` | — |
| `UpdatePrevFromPrim` | name suggests reconciling "preview" state from "primitive" state — another sync point between two parallel data sources |
| `GetMeshFromOwner` | — |
| `TryPickup`, `TryInteract`, `StopInteraction` | — |

Purpose read: a per-character component holding two parallel replicated arrays of equipped items (`RepActorsData` = logical slot data, `RepPrimitiveActorsData` = spawned primitive/mesh actors), with explicit "force re-rep" and "update-from" reconciliation functions. This dual-array design is a strong candidate for the detach bug: if `RepPrimitiveActorsData` and `RepActorsData` fall out of sync (e.g. one replicates before the other, or `ForceRepPrimitiveActorSpawns` isn't called reliably), you'd see exactly the symptom described — equipped items/meshes intermittently failing to apply despite slot data being correct.

### `BP_JigMultiplayer_C`
`JigSInventory/Jigsaw/Components/BP_JigMultiplayer.json` — 292 functions, the multiplayer-item-sync hub named in the investigation. Present as a component on nearly every interactable/container/pickup/trader class in scope (loot containers, `BP_TraderMaster`, `BP_SkeletalMeshPickup`, `BP_StaticMeshPickup`, all clothing pickups, etc.) — this is the shared item-transaction engine, not player-specific.

**Replicated properties:**
| Property | Type | Flags |
|---|---|---|
| `MainContainersIDs` | ArrayProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| RepNotify` — **`MainContainersIDs` (0x00E0, size 0x10)**, `TArray<FGuid>` — confirmed via `CXXHeaderDump/BP_JigMultiplayer.hpp` |
| `MPComponentType` | StructProperty | `Edit \| BlueprintVisible \| Net` — **`MPComponentType` (0x00C8, size 0x8)**, `FGameplayTag` |

**REAL OFFSETS from `CXXHeaderDump/BP_JigMultiplayer.hpp`** (`class UBP_JigMultiplayer_C : public UActorComponent`; partial — first ~30 fields captured):
`UberGraphFrame` 0x00A0 (0x8), `MainJigContainers` (`TArray<FS_ReplicatedContainerInfo>`) 0x00A8 (0x10), `LocalJSIContainers` (`TArray<UJSIContainer_C*>`) 0x00B8 (0x10), `MPComponentType` (`FGameplayTag`) 0x00C8 (0x8), `PendingRequests` (`TArray<UJSI_Slot_C*>`) 0x00D0 (0x10), `MainContainersIDs` (`TArray<FGuid>`) 0x00E0 (0x10), `ContainersSettings` (`TArray<FS_JigCompContentSettings>`) 0x00F0 (0x10), `RefillContainerTimerInSeconds` (double) 0x0100 (0x8), `InventoryWidgetClass` 0x0108 (0x8), `InventoryWidgetRef` 0x0110 (0x8), `PendingActors` (`TArray<AActor*>`) 0x0118 (0x10), `DoesContainerRefillLoot?` (bool) 0x0128 (0x1), `IfPickup_CanLoot?` (bool) 0x0129 (0x1), `AllowDroppingItems` (bool) 0x012A (0x1), `PickupInfo` (`FContainerPickupsInfo`) 0x0130 (0xD8), `VendorCurrentCurrencyAmount` (double) 0x0208 (0x8), `VendorMaxCurrencyAmount` (`FVector2D`) 0x0210 (0x10), `VendorAcceptedCurrencyID` 0x0220 (0x8), `VendorReSellSoldItems?` 0x0228 (0x1), `AllowSellingItemsToVendor?` 0x0229 (0x1), `ItemsToCraft` (`TArray<FS_JigCrafting>`) 0x0230 (0x10), `ExcludeMainContainerIndexesFromLoot` (`TArray<int32>`) 0x0240 (0x10), `MonitorContainerUID` (`FGuid`) 0x0250 (0x10), `PendingVendorContainerUID` (`FGuid`) 0x0260 (0x10), `PendingVendorToIndex` (int32) 0x0270 (0x4), `PendingVendorFinalRot` (bool) 0x0274 (0x1), `ServerVendorRequestedBuyAmount` (int32) 0x0278 (0x4), `InventoryWeight` (`TArray<FS_InvWeight>`) 0x0280 (0x10), `ExcludeMainContainerIndexesFromVendorCurrency` (`TArray<int32>`) 0x0290 (0x10), `DebugContent?` 0x02A0 (0x1), `Looted?` 0x02A1 (0x1), `RefillTimerHandle` (`FTimerHandle`) 0x02A8 (0x8), `PendingCraftingItems` (`TArray<FContainerPickupsInfo>`) 0x02B0 (0x10), `PendingCraftingTimers` (`TArray<double>`) 0x02C0 (0x10), `CraftingTimerHandle` (`FTimerHandle`) 0x02D0 (0x8) — full class continues past this point (292 functions total, not all fields listed here; see the .hpp directly for the remainder).

**Equip-specific RPCs and functions (full function list is large — filtered to equip/attach-relevant; verbatim `FunctionFlags` shown):**
| Function | FunctionFlags |
|---|---|
| `SERVER_RequestEquipActorToContainer` | `FUNC_Net \| FUNC_NetReliable \| FUNC_NetServer \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `CLIENT_EquipActorSuccess` | `FUNC_Net \| FUNC_NetReliable \| FUNC_HasOutParms \| FUNC_NetClient \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `CLIENT_RequestEquipResponseFailed` | `FUNC_Net \| FUNC_NetReliable \| FUNC_NetClient \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `ServerFuncHandleEquipActor` | `FUNC_Public \| FUNC_HasDefaults \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` (not itself an RPC — likely called from the `SERVER_...` RPC above) |
| `HandleActorEquipped` | `FUNC_Public \| FUNC_HasOutParms \| FUNC_HasDefaults \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `JigMP_OnPickupEquipped` | interface event (from `BP_JigMPComponentInterface_C`) |
| `JSI_TryEquipToMain` | `FUNC_Public \| FUNC_HasOutParms \| FUNC_HasDefaults \| ...` |
| `Context_TryEquip` / `Context_TryUnequip` | `FUNC_Public \| FUNC_HasDefaults \| ...` |
| `JigComp_ContextTryEquip` / `JigComp_ContextTryUnequip` | wrapper/forwarding versions |
| `AdjustEquipToContainerSettings` | — |
| `GetEquippedItemByContainerUID` | — |
| `WaitFullReplicationOfUIDs` | **notable**: a function explicitly named for waiting on replication to complete before proceeding — this is exactly the kind of race-condition mitigation you'd expect near a "sometimes items don't apply" bug. Worth checking what calls it and whether the equip-attach path actually awaits it. |
| `FixUpReferences` | generic "repair broken references" function — also worth checking if/when it's invoked relative to equip |
| `DetectDuplicateIDs` | integrity-check function; presence suggests known duplicate/desync issues in this system historically |

**Other full RPC set (item movement/container sync, for context — not clothing-specific but shares the same transport as equip):** `SERVER_RequestAddActorToContainer`/`CLIENT_AddActorToContainerSUCCESS`, `SERVER_RequestMoveItemToAnotherComp`, `SERVER_RequestSwapContainers`/`CLIENT_SwapContainersResponse`, `SERVER_RequestDropItem`/`CLIENT_DropRequestFailed`, `SERVER_RequestReload`/`CLIENT_ReloadResponse`, `MC_AddNewItem`, `MC_ItemRemoved`, `MC_NewItemAdded`, `MC_UpdateCount(s)`, `MC_UpdateDurability`, `MC_UpdateStack`, `MC_MoveItemToIndex`, `MC_SameCompUpdateSplitRequest`, `MC_OnPendingDone`, `SERVER_RequestSaveGame`, `SERVER_LoadSaveDataFromClientSide`, `SERVER_RequestDataFromSave`, `SERVER_RequestData`.

### `BP_PlayerCharacter_C` (already known — recap from investigation log, not re-exported here since it's outside this scope's folder, see `Blueprints/BP_PlayerCharacter.json`)
`Svr_AttachClothing` (`FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent`) and `MC_AttachClothing` (`FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent`) confirmed live tonight to be thin stubs wrapping a native C++ call — the real appearance-attach logic is invisible to this static export. Both take `Clothing (SkinnedMeshComponent)`, `Mesh (SkinnedAsset)`, `Parts (BodyPartSettings struct)`, `IsPlayerMale? (bool)`.

### Interfaces worth noting
- `BPI_PreviewChar_C` (`JigSInventory/Jigsaw/Components/Interfaces/BPI_PreviewChar.json`) — `PreviewOnWeaponEquipped`, `PreviewAttachmentUpdated`, `Preview Set Equipped Primitive By Slot`, `SetPreviewPawnOwner`. This is the character-preview-mannequin equip interface (inventory screen preview doll), a parallel/lighter-weight equip-visual path from the live in-world one. If the bug also affects the preview doll, that would narrow scope toward `BP_JigHelperComp`/data layer; if only the live character is affected, it points more toward the native attach call.
- `BP_JigMPComponentInterface_C` — declares `JigMP_OnPickupEquipped` and the rest of the MP event contract implemented by `BP_JigMultiplayer_C` and every container-ish actor.
- `BP_JigCharacterInterface_C` — `OnSpecialContainerattachmentUpdated`, `SetPreviewActor` — character-level attachment/preview hooks.

### `BP_JigPickupComponent_C`
`JigSInventory/Jigsaw/Pickup/BP_JigPickupComponent.json` — component present on every pickup actor (clothing, weapons, attachments, etc.), 37 functions, no RPCs of its own (delegates networking to the sibling `BP_JigMultiplayer_C` component).

**Replicated properties:**
| Property | Type | Flags |
|---|---|---|
| `EnablePhysics?` | BoolProperty | `Net \| DisableEditOnInstance \| RepNotify` — **(0x00D0, size 0x1)** |
| `CanBePicked?` | BoolProperty | `Net \| DisableEditOnInstance \| RepNotify` — **(0x00D1, size 0x1)** |
| `RepAttachments` | StructProperty | `Net \| DisableEditOnInstance \| RepNotify` — **(0x0110, size 0x20)**, `FS_RepWeaponAttachment` — **replicated attachment data on the pickup itself** (weapon attachments / clothing sub-parts before pickup); relevant if desync happens before an item is even equipped. |
| `RepCapacity` | IntProperty | `Net \| DisableEditOnInstance` (no RepNotify) — **(0x0134, size 0x4)** |
| `OverrideCount` | IntProperty | `Net` — **(0x00B0, size 0x4)** |

**REAL OFFSETS from `CXXHeaderDump/BP_JigPickupComponent.hpp`** (`class UBP_JigPickupComponent_C : public UActorComponent`): `UberGraphFrame` 0x00A0 (0x8), `ItemDataAsset` 0x00A8 (0x8), `OverrideCount` 0x00B0 (0x4), `RandomStatConfig` (`FS_RandomStatsConfig`) 0x00B8 (0x18), `EnablePhysics?` 0x00D0 (0x1), `CanBePicked?` 0x00D1 (0x1), `OwnerMPComp` (`UBP_JigMultiplayer_C*`) 0x00D8 (0x8), `CurrentActor` (`AActor*`) 0x00E0 (0x8), `InteractingActorLoc` (`FVector`) 0x00E8 (0x18), `InstalledAttachments` (`TArray<ABP_AMainLocalAttachment_C*>`) 0x0100 (0x10), `RepAttachments` (`FS_RepWeaponAttachment`) 0x0110 (0x20), `ProcessAttachments?` 0x0130 (0x1), `RepCapacity` 0x0134 (0x4), `OnAttachmentsUpdated` (delegate) 0x0138 (0x10), `CustomData` (`TMap<FString,FString>`) 0x0148 (0x50).

Functions: `FindAttachmentActorByType(Arr)`, `FindAttachmentByDataAsset`, `FindLocalAttachmentByUID`, `GetAttachSocketByInContainerIndex`, `CheckMismatch` (name suggests a known consistency-check need). Local-attachment resolution lives here, feeding into `BP_AMainLocalAttachment_C`/`BP_WeaponAttachments_C` below.

### `BP_AMainLocalAttachment_C` / `BP_WeaponAttachments_C`
`JigSInventory/Jigsaw/Pickup/BP_AMainLocalAttachment.json`, `BP_WeaponAttachments.json` — near-identical 22-24 function sets: `Jig_AttachmentInstalled`, `Jig_AttachmentRemoved`, `Jig_SetAttachmentState`, `Jig_GetAttachmentActiveState`/`SetAttachmentActiveState`/`ToggleAttachmentActiveState`, `Jig_GetPrimitiveComponent`. These are the actual "local attachment" (visual weapon-attachment mesh, e.g. scope/muzzle) install/remove handlers — the weapon-side analog of clothing attach. All leaf attachment blueprints (e.g. `BP_HoloSightLocalAttachment_C`) are empty subclasses with zero own functions, so all logic lives in these two base classes.

---

## Recommended next live bytecode-dump targets (priority order)

1. **`BP_JigHelperComp_C::ForceRepPrimitiveActorSpawns`, `UpdatePrevFromPrim`, `OnRep_RepActorsData`, `OnRep_RepPrimitiveActorsData`** — the two-array (`RepActorsData` vs `RepPrimitiveActorsData`) reconciliation logic is the most concrete, novel lead this pass turned up, and it wasn't previously known from the investigation log.
2. **`BP_JigMultiplayer_C::ServerFuncHandleEquipActor` and `HandleActorEquipped`** — the actual server-side body behind `SERVER_RequestEquipActorToContainer`, to see exactly what it does before/after calling into the native `Svr_AttachClothing` stub.
3. **`BP_JigMultiplayer_C::WaitFullReplicationOfUIDs`** — check whether the equip path actually calls this before finalizing, or whether it's called elsewhere and equip races ahead of it.
4. **`BP_JigPickupComponent_C::CheckMismatch`** — sounds like an existing detector for exactly this class of bug; may reveal what conditions the developers already anticipated.

---

## Full class catalog

### JigSInventory/Jigsaw/Components

| Class | File | Functions | Notes |
|---|---|---|---|
| `BP_JigHelperComp_C` | `BP_JigHelperComp.json` | 48 | See high-value section above. |
| `BP_JigMultiplayer_C` | `BP_JigMultiplayer.json` | 292 | See high-value section above. |
| `BP_JigControllerSupport_C` | `BP_JigControllerSupport.json` | 1 (`IsControllerEnabled`, Pure) | purpose unclear from export beyond the one query function — likely a small helper checked before allowing input-driven equip/interact actions. |
| `BP_JigMacroCompLib_C` | `BP_JigMacroCompLib.json` | 0 | Function library asset with no exported functions in this file (may be macros only, not reflected as UFunctions). |
| `BP_JigMPComponentSave_C` | `Save/BP_JigMPComponentSave.json` | 0 | Save-data component, no own functions exported; likely pure data container for a save subsystem. |

**Interfaces** (all `_C` interface BPs, functions are the interface contract only — no bodies, no flags of interest beyond `BlueprintCallable/Event`):
| Interface | File | # Fns | Contract theme |
|---|---|---|---|
| `BPI_JigCompInfo_C` | `Interfaces/BPI_JigCompInfo.json` | 16 | Generic "what kind of Jig component is this" queries (`JigCom_IsPickup`, `JigCom_IsVendor`, etc.) — subset of `BP_JigMultiplayer_C`'s own functions, confirming it implements this interface. |
| `BPI_PreviewChar_C` | `Interfaces/BPI_PreviewChar.json` | 8 | Preview-mannequin equip/render contract — see high-value section. |
| `BP_JigCharacterInterface_C` | `Interfaces/BP_JigCharacterInterface.json` | 15 | Character-level Jig contract: exfil, save, interact-distance, `SetPreviewActor`, `OnSpecialContainerattachmentUpdated`. |
| `BP_JigMPComponentInterface_C` | `Interfaces/BP_JigMPComponentInterface.json` | 14 | The MP-event contract (`JigMP_On...`) implemented by `BP_JigMultiplayer_C` and every container/pickup/trader actor. |
| `BP_MpInteractInterface_C` | `Interfaces/BP_MpInteractInterface.json` | 17 | Interact contract (`OnBeginInteract`, `OnExecuteInteract`, `OnRequestServerInteract`, `JigCanInteract`) — implemented by loot containers, traders, pickups. |

**Data structs** (`Data/S_*.json`) — all `UserDefinedStruct`, no functions, fields not individually enumerable from this export shape (structs don't list `ChildProperties` the same way as classes in this export — would need per-struct inspection if field-level detail is needed later): `S_ContainerSlots`, `S_EquipmentIDInfo`, `S_JigCompContentSettings`, `S_RepActorData` (the struct type used inside `RepActorsData`'s array — name confirms it's the equipped-item element type), `S_ReplicatedContainerInfo`, `S_RepNonActorData`, `S_SubContainerInfo`, `S_UpdateCount`.

### JigSInventory/Jigsaw/Pickup

| Class | File | Functions | Notes |
|---|---|---|---|
| `BP_JigPickupComponent_C` | `BP_JigPickupComponent.json` | 37 | See high-value section. |
| `BP_AMainLocalAttachment_C` | `BP_AMainLocalAttachment.json` | 24 | See high-value section. |
| `BP_WeaponAttachments_C` | `BP_WeaponAttachments.json` | 22 | See high-value section (near-duplicate of above, likely older/base version). |
| `BP_CustomPrimitiveComp_C` | `BP_CustomPrimitiveComp.json` | not captured in detail | name suggests this is the actual primitive-mesh component spawned/tracked by `RepPrimitiveActorsData` / `SpawnedCustomPrimitives` map on `BP_JigHelperComp_C` — worth a follow-up pass if the two-array-desync theory needs deeper support. |
| `S_LocalAttContainerIndexSocket` | struct | — | maps container index to attach socket for local attachments. |

### JigSInventory/Jigsaw/Data

Mostly `UserDefinedStruct`/DataTable-row struct definitions (`S_ItemStat`, `S_ItemRandomStat`, `S_JigCrafting`, `S_JigPayload`, `S_RandomStatsConfig`, `S_RepAttachmentInfo`, `S_RepWeaponAttachment`, `S_ServerEquippedItems`, `S_InvWeight`, `S_ItemStatText`) plus `JigsawItem_DataAsset_C` (base DataAsset for item definitions, no own functions) and small data tables (`ContainerPickupsInfo`, `CraftingItem`, `DefaultAttachments`/`DefaultAttachments2`, `DefaultItemInfo`, `RandomContainerItem`, `RarityColors`, `RepItemInfo`). `S_ServerEquippedItems` and `S_RepWeaponAttachment` are the most directly equip-relevant struct names but their fields aren't enumerable from this export shape — flag for follow-up if struct-field-level detail becomes necessary.

### JigSInventory/Jigsaw/ItemInspector

UI/render-focused item-inspection viewer (a 3D "inspect this item" camera rig), not gameplay-networking related: `BP_InspectorCanvas_C`, `BP_InspectorHelper_C`, `BP_InspectorWindowWidget_C`, `BP_ItemInspector_C`, `BP_ItemSnapCaptor_C` (scene-capture actor). No RPCs found; purpose unclear beyond visual inspection — low priority for this investigation.

### Inventory/ (top-level)

| Class | File | Notes |
|---|---|---|
| `BP_Ingame_C` | `BP_Ingame.json` | HUD widget (54 functions) — ammo/health/hunger/crosshair UI. Not equip-networking, but has `UnequipExisting` (`FUNC_Public \| FUNC_HasDefaults \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent`) — client-side UI-triggered unequip call, worth a look if the bug correlates with UI-driven unequip specifically. |
| `BP_Inventory_C` (root: `BP_EquipToContent_C`) | `BP_Inventory.json` | Main inventory widget, 28 functions, no RPCs (all client UI, forwards to `BP_JigMultiplayer_C` server calls). |

### Inventory/Containers

| Class | File | Notes |
|---|---|---|
| `BPI_LootContainers_C` | `BPI_LootContainers.json` | Small interface: `GetSettings`, `IsHeld?`. |
| `BP_LootContainer_C` | `BP_LootContainer.json` | 58 functions; composed of `BP_JigMultiplayer_C` + `BP_MpInteractInterface_C` + `BP_JigMPComponentInterface_C` contracts. No unique RPCs — networking delegated to `BP_JigMultiplayer_C`. |
| `Container_AnimalMeat_C`, `Container_DeadPlayerLoot_C` | — | Thin subclasses of the same pattern; `Container_DeadPlayerLoot_C` is notable as the actor that inherits a dead player's equipped items into a lootable container — a natural place where equip-state serialization bugs would surface, but no own equip-transfer function is exported here (likely handled by `BP_JigMultiplayer_C`'s generic container-fill logic). |

### Inventory/Traders

`BP_TraderMaster_C` (43 functions) — same `BP_JigMultiplayer_C`-composed pattern as loot containers; vendor buy/sell, no unique equip logic. All the specific trader subclasses (`BP_MilitaryTrader_C` etc., not individually dumped) are expected to be thin DataTable-driven subclasses per the leaf-class pattern confirmed below.

### Inventory/Items

| Class | File | Notes |
|---|---|---|
| `BP_SkeletalMeshPickup_C` | `BP_SkeletalMeshPickup.json` | 33 functions, `BP_JigMultiplayer_C` + `BP_JigPickupComponent_C` composition — base class for skinned-mesh pickups (clothing, some weapons). No own RPCs. |
| `BP_StaticMeshPickup_C` | `BP_StaticMeshPickup.json` | 36 functions, same composition pattern, static-mesh pickups. |
| `BP_WeaponsPickupComponent_C` | `Other/Components/BP_WeaponsPickupComponent.json` | 9 functions — `GetSight`/`GetSuppressor`/`GetMuzzleBrake`/`SetCurrent...` — the weapon-attachment-slot accessor component, feeds `BP_WeaponAttachments_C`. |
| `FishingRodPickupComponent_C` | — | 0 functions exported, empty/data-only. |
| `BP_ProjectileMaster_C` | `Other/ItemProjectiles/BP_ProjectileMaster.json` | 1 function (`SpawnCombatText`), not equip-related. |

**Confirmed leaf-class pattern (checked directly, not guessed):** every per-item pickup blueprint sampled (28 files under `Pickups/Equipment/Clothing/Torso/`, spot-checked further afield) is an empty `BlueprintGeneratedClass` with **zero own `Function` entries** — it only instances `BP_JigMultiplayer_C` + `BP_JigPickupComponent_C` (± `SceneComponent`/`StaticMeshComponent`) and presumably a DataAsset reference. Same confirmed for `LocalAttachments/Sights/BP_HoloSightLocalAttachment_C` (subclass of `BP_AMainLocalAttachment_C`, 0 own functions). This means **all ~1,170 individual item/attachment blueprint files in `Items/Pickups/**` and `Items/LocalAttachments/**` carry no unique logic** — the entire equip/attach behavior for every item in the game funnels through the handful of base classes documented above (`BP_JigMultiplayer_C`, `BP_JigPickupComponent_C`, `BP_JigHelperComp_C`, `BP_AMainLocalAttachment_C`/`BP_WeaponAttachments_C`). This was not spot-checked exhaustively (580 pickup files + 590 DataAsset files exist), but the sampled pattern was fully consistent, so per-item enumeration was skipped as low-value.

---

## Notes / limitations

- This export has no bytecode — `FunctionFlags` and signatures only, no call graph or execution order. All "likely calls X" statements above are inferred from naming, not verified.
- Struct-internal field lists (e.g. exact fields of `S_RepActorData`, `S_ServerEquippedItems`) were not enumerated — the export's struct JSON shape for `UserDefinedStruct` wasn't drilled into for this pass. Flag for a follow-up query if exact field types matter.
- `Items/DataAssets/**` (590 files) were not enumerated individually — they are per-item data (stats, meshes, icons) referenced by the pickup/DataAsset system, not code.
- `BP_CustomPrimitiveComp_C` function list was noted as present but not fully extracted — recommend a dedicated look given its likely role in the primitive-actor spawn/sync path.
