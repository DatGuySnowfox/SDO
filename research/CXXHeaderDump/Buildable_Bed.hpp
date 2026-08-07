#ifndef UE4SS_SDK_Buildable_Bed_HPP
#define UE4SS_SDK_Buildable_Bed_HPP

class ABuildable_Bed_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x0438 (size: 0x8)
    class USceneComponent* RespawnLocation;                                           // 0x0440 (size: 0x8)
    class USleepingUI_C* SleepUI;                                                     // 0x0448 (size: 0x8)

    void SetSpawnPoint();
    void ReceiveBeginPlay();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Bed(int32 EntryPoint);
}; // Size: 0x450

#endif
