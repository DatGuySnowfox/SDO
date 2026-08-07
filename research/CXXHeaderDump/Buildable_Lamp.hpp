#ifndef UE4SS_SDK_Buildable_Lamp_HPP
#define UE4SS_SDK_Buildable_Lamp_HPP

class ABuildable_Lamp_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x0438 (size: 0x8)
    bool LightOn;                                                                     // 0x0440 (size: 0x1)

    void ReceiveBeginPlay();
    void On();
    void Off();
    void OnInteractBuildable(class AActor* Actor);
    void LightCallback(bool NewValue);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Lamp(int32 EntryPoint);
}; // Size: 0x441

#endif
