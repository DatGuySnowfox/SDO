#ifndef UE4SS_SDK_BP_FishingBuoy_HPP
#define UE4SS_SDK_BP_FishingBuoy_HPP

class ABP_FishingBuoy_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Bait;                                                 // 0x02A8 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_FishingBuoy(int32 EntryPoint);
}; // Size: 0x2B0

#endif
