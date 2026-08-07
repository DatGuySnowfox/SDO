#ifndef UE4SS_SDK_BP_Lights_HPP
#define UE4SS_SDK_BP_Lights_HPP

class ABP_Lights_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USpotLightComponent* SpotLight;                                             // 0x02A8 (size: 0x8)
    class UAudioComponent* LightHum_Cue;                                              // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Light;                                                // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* Pole;                                                 // 0x02C8 (size: 0x8)
    float Timeline_TurnOff_Intensity_334B7A7D46D6C0473098E7BA36F6E19F;                // 0x02D0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOff__Direction_334B7A7D46D6C0473098E7BA36F6E19F; // 0x02D4 (size: 0x1)
    class UTimelineComponent* Timeline_TurnOff;                                       // 0x02D8 (size: 0x8)
    float Timeline_TurnOn_Intensity_E220CE844C4928F0DABF14B555D5FBF7;                 // 0x02E0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOn__Direction_E220CE844C4928F0DABF14B555D5FBF7; // 0x02E4 (size: 0x1)
    class UTimelineComponent* Timeline_TurnOn;                                        // 0x02E8 (size: 0x8)

    void Timeline_TurnOn__FinishedFunc();
    void Timeline_TurnOn__UpdateFunc();
    void Timeline_TurnOff__FinishedFunc();
    void Timeline_TurnOff__UpdateFunc();
    void ReceiveBeginPlay();
    void Event_TurnOn();
    void Event_TurnOff();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Lights(int32 EntryPoint);
}; // Size: 0x2F0

#endif
