#ifndef UE4SS_SDK_Buildable_WoodenFence2_HPP
#define UE4SS_SDK_Buildable_WoodenFence2_HPP

class ABuildable_WoodenFence2_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)
    class UChildActorComponent* Fence2;                                               // 0x0448 (size: 0x8)
    class UChildActorComponent* Fence1;                                               // 0x0450 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0458 (size: 0x8)

    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_WoodenFence2(int32 EntryPoint);
}; // Size: 0x460

#endif
