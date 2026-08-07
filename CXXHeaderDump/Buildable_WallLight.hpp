#ifndef UE4SS_SDK_Buildable_WallLight_HPP
#define UE4SS_SDK_Buildable_WallLight_HPP

class ABuildable_WallLight_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x0438 (size: 0x8)
    bool LightOn;                                                                     // 0x0440 (size: 0x1)

    void GetPickupName(FText& Result);
    void ReceiveBeginPlay();
    void On();
    void Off();
    void LightCallback(bool NewValue);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_WallLight(int32 EntryPoint);
}; // Size: 0x441

#endif
