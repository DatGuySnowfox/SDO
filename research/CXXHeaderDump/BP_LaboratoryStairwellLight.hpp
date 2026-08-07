#ifndef UE4SS_SDK_BP_LaboratoryStairwellLight_HPP
#define UE4SS_SDK_BP_LaboratoryStairwellLight_HPP

class ABP_LaboratoryStairwellLight_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02B8 (size: 0x8)

    void UserConstructionScript();
    void BndEvt__BP_LaboratoryLight_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryStairwellLight(int32 EntryPoint);
}; // Size: 0x2C0

#endif
