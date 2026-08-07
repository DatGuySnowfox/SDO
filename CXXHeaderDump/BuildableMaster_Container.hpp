#ifndef UE4SS_SDK_BuildableMaster_Container_HPP
#define UE4SS_SDK_BuildableMaster_Container_HPP

class ABuildableMaster_Container_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class USceneComponent* DropItemLocation;                                          // 0x0438 (size: 0x8)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0440 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0450 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0460 (size: 0x10)
    FText ContainerName;                                                              // 0x0470 (size: 0x18)

    void DropAll();
    void GetName(FText& Name);
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
    void OverrideBuildName(FText Name);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BuildableMaster_Container(int32 EntryPoint);
}; // Size: 0x488

#endif
