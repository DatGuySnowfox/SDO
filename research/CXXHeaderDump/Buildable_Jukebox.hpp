#ifndef UE4SS_SDK_Buildable_Jukebox_HPP
#define UE4SS_SDK_Buildable_Jukebox_HPP

class ABuildable_Jukebox_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x0438 (size: 0x8)
    class URadioComponent_C* RadioComponent;                                          // 0x0440 (size: 0x8)
    class UMaterialInstanceDynamic* MaterialInstance;                                 // 0x0448 (size: 0x8)

    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnDestroy(class AActor* Actor);
    void TurnedOn();
    void TurnedOff();
    void EventPoweredOff();
    void ReceiveDestroyed();
    void On();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Jukebox(int32 EntryPoint);
}; // Size: 0x450

#endif
