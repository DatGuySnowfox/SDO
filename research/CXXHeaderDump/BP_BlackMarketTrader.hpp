#ifndef UE4SS_SDK_BP_BlackMarketTrader_HPP
#define UE4SS_SDK_BP_BlackMarketTrader_HPP

class ABP_BlackMarketTrader_C : public ABP_TraderMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0388 (size: 0x8)
    class UStaticMeshComponent* StaticMesh2;                                          // 0x0390 (size: 0x8)
    class UStaticMeshComponent* StaticMesh1;                                          // 0x0398 (size: 0x8)
    class UStaticMeshComponent* StaticMesh_0;                                         // 0x03A0 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_BlackMarketTrader(int32 EntryPoint);
}; // Size: 0x3A8

#endif
