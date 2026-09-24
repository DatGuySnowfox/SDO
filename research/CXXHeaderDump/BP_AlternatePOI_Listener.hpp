#ifndef UE4SS_SDK_BP_AlternatePOI_Listener_HPP
#define UE4SS_SDK_BP_AlternatePOI_Listener_HPP

class ABP_AlternatePOI_Listener_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> Base_Prefab;                                   // 0x02B8 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> Alternative_Prefab;                            // 0x02C0 (size: 0x8)

    void 2_TestSpawnAlternativePrefab();
    void 1_TestSpawnBasePrefab();
    void ReceiveBeginPlay();
    void SpawnPrefab(bool Alternative?);
    void ExecuteUbergraph_BP_AlternatePOI_Listener(int32 EntryPoint);
}; // Size: 0x2C8

#endif
