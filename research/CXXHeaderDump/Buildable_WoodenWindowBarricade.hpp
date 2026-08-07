#ifndef UE4SS_SDK_Buildable_WoodenWindowBarricade_HPP
#define UE4SS_SDK_Buildable_WoodenWindowBarricade_HPP

class ABuildable_WoodenWindowBarricade_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)

    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_WoodenWindowBarricade(int32 EntryPoint);
}; // Size: 0x438

#endif
