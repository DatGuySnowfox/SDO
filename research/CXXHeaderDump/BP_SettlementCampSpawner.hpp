#ifndef UE4SS_SDK_BP_SettlementCampSpawner_HPP
#define UE4SS_SDK_BP_SettlementCampSpawner_HPP

class ABP_SettlementCampSpawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UTextRenderComponent* TextRender;                                           // 0x02B8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02C0 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> Settlement;                                    // 0x02C8 (size: 0x8)
    FText DebugText;                                                                  // 0x02D0 (size: 0x10)
    FVector DebugOffset;                                                              // 0x02E0 (size: 0x18)

    void UserConstructionScript();
    void SpawnSettlement();
    void ExecuteUbergraph_BP_SettlementCampSpawner(int32 EntryPoint);
}; // Size: 0x2F8

#endif
