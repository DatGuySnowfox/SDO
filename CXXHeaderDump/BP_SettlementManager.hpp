#ifndef UE4SS_SDK_BP_SettlementManager_HPP
#define UE4SS_SDK_BP_SettlementManager_HPP

class ABP_SettlementManager_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    TArray<class ABP_SettlementCampSpawner_C*> Camps;                                 // 0x02A8 (size: 0x10)
    int32 CampToSpawn;                                                                // 0x02B8 (size: 0x4)
    bool SettlementSpawned?;                                                          // 0x02BC (size: 0x1)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void OnCompleted_BE652D6747526F18549A3683E8CAC5D2();
    void ActorLoaded();
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void ReceiveBeginPlay();
    void SpawnSettlement();
    void ExecuteUbergraph_BP_SettlementManager(int32 EntryPoint);
}; // Size: 0x2BD

#endif
