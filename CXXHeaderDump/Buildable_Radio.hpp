#ifndef UE4SS_SDK_Buildable_Radio_HPP
#define UE4SS_SDK_Buildable_Radio_HPP

class ABuildable_Radio_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x0438 (size: 0x8)
    class URadioComponent_C* RadioComponent;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnDestroy(class AActor* Actor);
    void ReceiveDestroyed();
    void On();
    void Off();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Radio(int32 EntryPoint);
}; // Size: 0x448

#endif
