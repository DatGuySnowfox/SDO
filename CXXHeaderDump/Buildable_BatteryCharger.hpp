#ifndef UE4SS_SDK_Buildable_BatteryCharger_HPP
#define UE4SS_SDK_Buildable_BatteryCharger_HPP

class ABuildable_BatteryCharger_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    TArray<FGuid> BatteryUIDs;                                                        // 0x0438 (size: 0x10)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0448 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0458 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0468 (size: 0x10)

    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void Event_PowerOn();
    void Event_TimerPower();
    void ActorPreSave();
    void ActorLoaded();
    void Event_PowerOff();
    void UpdateStorageStat();
    void Event_BeginInteract();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_BatteryCharger(int32 EntryPoint);
}; // Size: 0x478

#endif
