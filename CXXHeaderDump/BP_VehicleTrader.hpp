#ifndef UE4SS_SDK_BP_VehicleTrader_HPP
#define UE4SS_SDK_BP_VehicleTrader_HPP

class ABP_VehicleTrader_C : public ABP_TraderMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    class UStaticMeshComponent* Vehicle;                                              // 0x0358 (size: 0x8)

    void BndEvt__BP_MPVendor_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_VehicleTrader(int32 EntryPoint);
}; // Size: 0x360

#endif
