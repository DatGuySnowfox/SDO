#ifndef UE4SS_SDK_BP_SpecialItemContainerSpawner_HPP
#define UE4SS_SDK_BP_SpecialItemContainerSpawner_HPP

class ABP_SpecialItemContainerSpawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Case4;                                                // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Case3;                                                // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Case2;                                                // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Case1;                                                // 0x02C0 (size: 0x8)
    class UBillboardComponent* Spawn4;                                                // 0x02C8 (size: 0x8)
    class UBillboardComponent* Spawn3;                                                // 0x02D0 (size: 0x8)
    class UBillboardComponent* Spawn2;                                                // 0x02D8 (size: 0x8)
    class UBillboardComponent* Spawn1;                                                // 0x02E0 (size: 0x8)
    FText Container Name;                                                             // 0x02E8 (size: 0x18)
    TArray<FName> UniqueLootSets;                                                     // 0x0300 (size: 0x10)

    void SpawnContainer();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_SpecialItemContainerSpawner(int32 EntryPoint);
}; // Size: 0x310

#endif
