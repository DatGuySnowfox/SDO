#ifndef UE4SS_SDK_BP_JigHelperComp_HPP
#define UE4SS_SDK_BP_JigHelperComp_HPP

class UBP_JigHelperComp_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    TMap<FGameplayTag, FGuid> EquipmentUIDs;                                          // 0x00C0 (size: 0x50)
    FS_ServerEquippedItems ServerEquippedItems;                                       // 0x0110 (size: 0x9D8)
    TArray<FS_RepNonActorData> RepPrimitiveActorsData;                                // 0x0AE8 (size: 0x10)
    TArray<FS_RepActorData> RepActorsData;                                            // 0x0AF8 (size: 0x10)
    bool TraceToActors?;                                                              // 0x0B08 (size: 0x1)
    TMap<FGameplayTag, FS_EquipmentIDInfo> EquipmentIDSlotConfig;                     // 0x0B10 (size: 0x50)
    TMap<FGameplayTag, ABP_CustomPrimitiveComp_C*> SpawnedCustomPrimitives;           // 0x0B60 (size: 0x50)
    FGameplayTag ActiveWeapon;                                                        // 0x0BB0 (size: 0x8)
    FBP_JigHelperComp_COnActiveWeaponSlotChanged OnActiveWeaponSlotChanged;           // 0x0BB8 (size: 0x10)
    void OnActiveWeaponSlotChanged(FGameplayTag Slot);
    class AActor* PreviewChar;                                                        // 0x0BC8 (size: 0x8)
    class AActor* CurrentTracActor;                                                   // 0x0BD0 (size: 0x8)
    TMap<FGameplayTag, FText> CurrentInteractOptions;                                 // 0x0BD8 (size: 0x50)
    int32 CurrentInteractOptionIndex;                                                 // 0x0C28 (size: 0x4)
    class AActor* InteractExecutedActor;                                              // 0x0C30 (size: 0x8)
    FBP_JigHelperComp_COnPossessed OnPossessed;                                       // 0x0C38 (size: 0x10)
    void OnPossessed(class AController* Controller);
    FBP_JigHelperComp_COnEquipmentUpdated OnEquipmentUpdated;                         // 0x0C48 (size: 0x10)
    void OnEquipmentUpdated();

    void GetCurrentTracActor(class AActor*& Actor, bool& Valid);
    void StopInteraction();
    void GetPrimitiveByUID(FGuid UID, class AActor*& Actor);
    void OnRep_RepPrimitiveActorsData();
    void TryPickup(class AActor* PickupRef, class UJSIContainer_C* TargetContainer, bool& Result);
    void RemovePrimitiveActorBySlot(FGameplayTag Slot);
    void GetPrimitiveBySlot(const FGameplayTag Slot, class ABP_CustomPrimitiveComp_C*& Value);
    void GetEquipmentUID(const FGameplayTag Slot, FGuid& Value);
    void UpdatePrevFromPrim();
    void ForceRepPrimitiveActorSpawns();
    void GetTraceIgnore(TArray<AActor*>& OutArr);
    void OnLoadDataRequested();
    void TryInteract();
    void GetActiveWeapon(class AActor*& Actor);
    void GetAllEquippedActors(TArray<AActor*>& OutArr);
    void TraceToWorld();
    void UpdateChamberUID(FGuid UID, bool& Result);
    FVector GetDropLocation();
    void RequestDropAsPickup(FRepItemInfo& ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void OnContainerSwap(FName From, FName To);
    void PreviewAttachmentUpdated(class UObject* Item, bool& Result);
    void OnItemMoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, bool& Result);
    void OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void OnItemAdded(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void OnMainRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo);
    void OnRep_ActiveWeapon();
    void GetEquippedActorBySlot(FGameplayTag Slot, bool IncludePrimitive?, class AActor*& Actor, int32& Array Index);
    void SetActiveWeaponSlot(FGameplayTag NewSlot);
    void GetActiveWeaponSlot(FGameplayTag& ActiveWeapon);
    void OnMainAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo& ItemInfo, FName FromMain, bool& Result, class AActor*& Actor);
    void Equip Actor to Socket(class AActor* ActorRef, bool IsSecondary);
    void GetMeshFromOwner(class USkeletalMeshComponent*& Mesh);
    void GetEquipConfigByName(FName& Name, FS_EquipmentIDInfo& Result, FGameplayTag& Slot);
    class AActor* SpawnPrimitiveActor(FS_RepNonActorData& Data, FGuid UID);
    void OnPickupEquipped(class AActor* ActorRef, FName ToContainerName, FGuid& UID, FGuid& ToContainerUID, FRepItemInfo& Info, class AActor*& OverrideActor);
    void OnRep_RepActorsData();
    void GetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo& Info, bool& Equipped);
    void SetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo Info, FGuid UID, bool SkipUID);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void SetPreviewChar(class AActor* PreviewChar);
    void ReceiveBeginPlay();
    void CycleOptions(bool Value);
    void OnInventoryOpenClose_Event_0(bool Opened?);
    void OnPawnControllerChangedDelegates_Event_0(class APawn* Pawn, class AController* Controller);
    void ExecuteUbergraph_BP_JigHelperComp(int32 EntryPoint);
    void OnEquipmentUpdated__DelegateSignature();
    void OnPossessed__DelegateSignature(class AController* Controller);
    void OnActiveWeaponSlotChanged__DelegateSignature(FGameplayTag Slot);
}; // Size: 0xC58

#endif
