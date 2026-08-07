#ifndef UE4SS_SDK_BP_AlternatePOI_HPP
#define UE4SS_SDK_BP_AlternatePOI_HPP

class ABP_AlternatePOI_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> Prefab;                                        // 0x02A8 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> AlternatePrefab;                               // 0x02B0 (size: 0x8)
    bool AlternatePrefab?;                                                            // 0x02B8 (size: 0x1)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void ReceiveBeginPlay();
    void SpawnPrefab();
    void ActorLoaded();
    void ExecuteUbergraph_BP_AlternatePOI(int32 EntryPoint);
}; // Size: 0x2B9

#endif
