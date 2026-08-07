#ifndef UE4SS_SDK_BP_FlickeringLight_HPP
#define UE4SS_SDK_BP_FlickeringLight_HPP

class ABP_FlickeringLight_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Cube1;                                                // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Cube;                                                 // 0x02B0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02B8 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02C0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* Pole;                                                 // 0x02D0 (size: 0x8)
    float LightFlicker_Flicker_16C525594E394E9E6929C6BA39D659B1;                      // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> LightFlicker__Direction_16C525594E394E9E6929C6BA39D659B1; // 0x02DC (size: 0x1)
    class UTimelineComponent* LightFlicker;                                           // 0x02E0 (size: 0x8)
    double IntensityMultiplier;                                                       // 0x02E8 (size: 0x8)

    void LightFlicker__FinishedFunc();
    void LightFlicker__UpdateFunc();
    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_FlickeringLight(int32 EntryPoint);
}; // Size: 0x2F0

#endif
