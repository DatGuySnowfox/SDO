#ifndef UE4SS_SDK_BP_LaboratoryCorpse_HPP
#define UE4SS_SDK_BP_LaboratoryCorpse_HPP

class ABP_LaboratoryCorpse_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02B0 (size: 0x8)
    float AnimFloat;                                                                  // 0x02B8 (size: 0x4)
    bool EnableCollision?;                                                            // 0x02BC (size: 0x1)
    bool Scientist?;                                                                  // 0x02BD (size: 0x1)

    void UserConstructionScript();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryCorpse(int32 EntryPoint);
}; // Size: 0x2BE

#endif
