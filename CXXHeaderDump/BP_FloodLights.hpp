#ifndef UE4SS_SDK_BP_FloodLights_HPP
#define UE4SS_SDK_BP_FloodLights_HPP

class ABP_FloodLights_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Cube1;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Cube;                                                 // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Light;                                                // 0x02C0 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02C8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02D0 (size: 0x8)
    float Timeline_TurnOff_Intensity_88264E574157CCA15B807CB065659552;                // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOff__Direction_88264E574157CCA15B807CB065659552; // 0x02DC (size: 0x1)
    class UTimelineComponent* Timeline_TurnOff;                                       // 0x02E0 (size: 0x8)
    float Timeline_TurnOn_Intensity_5E6A4D3F47BD1CD5EA658FBB63E55535;                 // 0x02E8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOn__Direction_5E6A4D3F47BD1CD5EA658FBB63E55535; // 0x02EC (size: 0x1)
    class UTimelineComponent* Timeline_TurnOn;                                        // 0x02F0 (size: 0x8)

    void Timeline_TurnOn__FinishedFunc();
    void Timeline_TurnOn__UpdateFunc();
    void Timeline_TurnOff__FinishedFunc();
    void Timeline_TurnOff__UpdateFunc();
    void ReceiveBeginPlay();
    void Event_TurnOn();
    void Event_TurnOff();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_FloodLights(int32 EntryPoint);
}; // Size: 0x2F8

#endif
