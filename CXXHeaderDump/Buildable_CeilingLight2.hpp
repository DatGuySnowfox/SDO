#ifndef UE4SS_SDK_Buildable_CeilingLight2_HPP
#define UE4SS_SDK_Buildable_CeilingLight2_HPP

class ABuildable_CeilingLight2_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x0438 (size: 0x8)
    bool LightOn;                                                                     // 0x0440 (size: 0x1)

    void GetPickupName(FText& Result);
    void On();
    void Off();
    void ReceiveBeginPlay();
    void LightCallback(bool NewValue);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_CeilingLight2(int32 EntryPoint);
}; // Size: 0x441

#endif
