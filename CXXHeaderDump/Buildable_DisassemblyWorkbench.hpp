#ifndef UE4SS_SDK_Buildable_DisassemblyWorkbench_HPP
#define UE4SS_SDK_Buildable_DisassemblyWorkbench_HPP

class ABuildable_DisassemblyWorkbench_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0438 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0448 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0458 (size: 0x10)

    void ReceiveBeginPlay();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnInteractBuildable(class AActor* Actor);
    void ActorPreSave();
    void ActorLoaded();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_DisassemblyWorkbench(int32 EntryPoint);
}; // Size: 0x468

#endif
