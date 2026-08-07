#ifndef UE4SS_SDK_BP_JigHelperComp_HPP
#define UE4SS_SDK_BP_JigHelperComp_HPP

class UBP_JigHelperComp_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    TMap<class FGameplayTag, class FGuid> EquipmentUIDs;                              // 0x00A8 (size: 0x50)
    FS_ServerEquippedItems ServerEquippedItems;                                       // 0x00F8 (size: 0x9D8)
    TArray<FS_RepNonActorData> RepPrimitiveActorsData;                                // 0x0AD0 (size: 0x10)
    TArray<FS_RepActorData> RepActorsData;                                            // 0x0AE0 (size: 0x10)
    bool TraceToActors?;                                                              // 0x0AF0 (size: 0x1)
    TMap<class FGameplayTag, class FS_EquipmentIDInfo> EquipmentIDSlotConfig;         // 0x0AF8 (size: 0x50)
    TMap<class FGameplayTag, class ABP_CustomPrimitiveComp_C*> SpawnedCustomPrimitives; // 0x0B48 (size: 0x50)
    FGameplayTag ActiveWeapon;                                                        // 0x0B98 (size: 0x8)
    FBP_JigHelperComp_COnActiveWeaponSlotChanged OnActiveWeaponSlotChanged;           // 0x0BA0 (size: 0x10)
    void OnActiveWeaponSlotChanged(FGameplayTag Slot);
    class AActor* PreviewChar;                                                        // 0x0BB0 (size: 0x8)
    class AActor* CurrentTracActor;                                                   // 0x0BB8 (size: 0x8)
    TMap<class FGameplayTag, class FText> CurrentInteractOptions;                     // 0x0BC0 (size: 0x50)
    int32 CurrentInteractOptionIndex;                                                 // 0x0C10 (size: 0x4)
    class AActor* InteractExecutedActor;                                              // 0x0C18 (size: 0x8)
    FBP_JigHelperComp_COnPossessed OnPossessed;                                       // 0x0C20 (size: 0x10)
    void OnPossessed(class AController* Controller);
    FBP_JigHelperComp_COnEquipmentUpdated OnEquipmentUpdated;                         // 0x0C30 (size: 0x10)
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
    void GetTraceIgnore(TArray<class AActor*>& OutArr);
    void OnLoadDataRequested();
    void TryInteract(double TraceDistance);
    void GetActiveWeapon(class AActor*& Actor);
    void GetAllEquippedActors(TArray<class AActor*>& OutArr);
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
}; // Size: 0xC40

#endif
