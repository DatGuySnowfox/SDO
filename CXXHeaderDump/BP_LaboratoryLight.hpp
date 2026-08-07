#ifndef UE4SS_SDK_BP_LaboratoryLight_HPP
#define UE4SS_SDK_BP_LaboratoryLight_HPP

class ABP_LaboratoryLight_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B8 (size: 0x8)
    float Intensity;                                                                  // 0x02C0 (size: 0x4)
    bool LightOn?;                                                                    // 0x02C4 (size: 0x1)
    bool EnableShadows?;                                                              // 0x02C5 (size: 0x1)

    void OnRep_LightOn?();
    void UserConstructionScript();
    void BndEvt__BP_LaboratoryLight_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryLight(int32 EntryPoint);
}; // Size: 0x2C6

#endif
