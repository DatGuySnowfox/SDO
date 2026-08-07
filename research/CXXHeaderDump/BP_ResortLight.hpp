#ifndef UE4SS_SDK_BP_ResortLight_HPP
#define UE4SS_SDK_BP_ResortLight_HPP

class ABP_ResortLight_C : public ABP_MasterLight_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Light;                                                // 0x02B8 (size: 0x8)
    float Timeline_TurnOff_Intensity_55494F92401FAB9CA11D539A62884D93;                // 0x02C0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOff__Direction_55494F92401FAB9CA11D539A62884D93; // 0x02C4 (size: 0x1)
    class UTimelineComponent* Timeline_TurnOff;                                       // 0x02C8 (size: 0x8)
    float Timeline_TurnOn_Intensity_AC8A7F314EE014C9CF5B23B01E7564D6;                 // 0x02D0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_TurnOn__Direction_AC8A7F314EE014C9CF5B23B01E7564D6; // 0x02D4 (size: 0x1)
    class UTimelineComponent* Timeline_TurnOn;                                        // 0x02D8 (size: 0x8)

    void Timeline_TurnOn__FinishedFunc();
    void Timeline_TurnOn__UpdateFunc();
    void Timeline_TurnOff__FinishedFunc();
    void Timeline_TurnOff__UpdateFunc();
    void ReceiveBeginPlay();
    void Event_TurnOn();
    void Event_TurnOff();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_ResortLight(int32 EntryPoint);
}; // Size: 0x2E0

#endif
