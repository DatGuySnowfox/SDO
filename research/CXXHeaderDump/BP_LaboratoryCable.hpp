#ifndef UE4SS_SDK_BP_LaboratoryCable_HPP
#define UE4SS_SDK_BP_LaboratoryCable_HPP

class ABP_LaboratoryCable_C : public ABP_LaboratoryMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class USplineComponent* Spline;                                                   // 0x02B0 (size: 0x8)

    void UserConstructionScript();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_LaboratoryCable(int32 EntryPoint);
}; // Size: 0x2B8

#endif
