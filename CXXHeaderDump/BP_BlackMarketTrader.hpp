#ifndef UE4SS_SDK_BP_BlackMarketTrader_HPP
#define UE4SS_SDK_BP_BlackMarketTrader_HPP

class ABP_BlackMarketTrader_C : public ABP_TraderMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    class UStaticMeshComponent* StaticMesh2;                                          // 0x0358 (size: 0x8)
    class UStaticMeshComponent* StaticMesh1;                                          // 0x0360 (size: 0x8)
    class UStaticMeshComponent* StaticMesh_0;                                         // 0x0368 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_BlackMarketTrader(int32 EntryPoint);
}; // Size: 0x370

#endif
