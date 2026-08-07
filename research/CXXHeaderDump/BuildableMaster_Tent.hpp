#ifndef UE4SS_SDK_BuildableMaster_Tent_HPP
#define UE4SS_SDK_BuildableMaster_Tent_HPP

class ABuildableMaster_Tent_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    FStruct_WorldActorInventory TentInventory;                                        // 0x0438 (size: 0x30)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0468 (size: 0x10)
    TArray<FGuid> MainContainersIDs;                                                  // 0x0478 (size: 0x10)
    TArray<FS_InvWeight> Inventory Weight;                                            // 0x0488 (size: 0x10)

    void DropAll();
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void ActorPreSave();
    void ActorLoaded();
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void UpdateStorageStat();
    void Event_BeginInteract();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void PickupContainer();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BuildableMaster_Tent(int32 EntryPoint);
}; // Size: 0x498

#endif
