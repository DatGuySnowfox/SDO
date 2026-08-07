#ifndef UE4SS_SDK_Buildable_SpikedWall_HPP
#define UE4SS_SDK_Buildable_SpikedWall_HPP

class ABuildable_SpikedWall_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0438 (size: 0x8)

    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_SpikedWall(int32 EntryPoint);
}; // Size: 0x440

#endif
