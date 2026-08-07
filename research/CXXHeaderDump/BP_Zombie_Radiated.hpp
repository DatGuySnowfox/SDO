#ifndef UE4SS_SDK_BP_Zombie_Radiated_HPP
#define UE4SS_SDK_BP_Zombie_Radiated_HPP

class ABP_Zombie_Radiated_C : public ABP_MasterZombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x08B0 (size: 0x8)

    void ReceiveBeginPlay();
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Zombie_Radiated(int32 EntryPoint);
}; // Size: 0x8B8

#endif
