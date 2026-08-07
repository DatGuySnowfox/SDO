#ifndef UE4SS_SDK_BP_FlickeringLightSparks_HPP
#define UE4SS_SDK_BP_FlickeringLightSparks_HPP

class ABP_FlickeringLightSparks_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02A8 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02B0 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02B8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C0 (size: 0x8)
    float LightFlicker_Flicker_46544F49447690B8CFED588112D65D0C;                      // 0x02C8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> LightFlicker__Direction_46544F49447690B8CFED588112D65D0C; // 0x02CC (size: 0x1)
    class UTimelineComponent* LightFlicker;                                           // 0x02D0 (size: 0x8)
    double IntensityMultiplier;                                                       // 0x02D8 (size: 0x8)
    double PointLightLength;                                                          // 0x02E0 (size: 0x8)
    double PointLightRadius;                                                          // 0x02E8 (size: 0x8)
    FVector ParticlesLocation;                                                        // 0x02F0 (size: 0x18)

    void UserConstructionScript();
    void LightFlicker__FinishedFunc();
    void LightFlicker__UpdateFunc();
    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_FlickeringLightSparks(int32 EntryPoint);
}; // Size: 0x308

#endif
