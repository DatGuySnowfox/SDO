#ifndef UE4SS_SDK_Buildable_TrashBin_HPP
#define UE4SS_SDK_Buildable_TrashBin_HPP

class ABuildable_TrashBin_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    TArray<int32> BatteryUIDs;                                                        // 0x0438 (size: 0x10)

    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_TrashBin(int32 EntryPoint);
}; // Size: 0x448

#endif
