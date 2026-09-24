#ifndef UE4SS_SDK_BP_PrefabSpawner_HPP
#define UE4SS_SDK_BP_PrefabSpawner_HPP

class ABP_PrefabSpawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UTextRenderComponent* TextRender;                                           // 0x02B8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02C0 (size: 0x8)
    FText Text;                                                                       // 0x02C8 (size: 0x10)
    bool RandomPossibilityToSpawn?;                                                   // 0x02D8 (size: 0x1)
    double Chance;                                                                    // 0x02E0 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> Prefab;                                        // 0x02E8 (size: 0x8)
    double DebugOffset;                                                               // 0x02F0 (size: 0x8)
    bool DynamicEvent?;                                                               // 0x02F8 (size: 0x1)
    bool BypassSpawnChecks?;                                                          // 0x02F9 (size: 0x1)
    float DelayStart;                                                                 // 0x02FC (size: 0x4)
    TEnumAsByte<Enum_PrefabType::Type> PrefabType;                                    // 0x0300 (size: 0x1)
    class USoundBase* SoundOnSpawn;                                                   // 0x0308 (size: 0x8)
    bool Spawned?;                                                                    // 0x0310 (size: 0x1)
    bool Marker?;                                                                     // 0x0311 (size: 0x1)
    class UTexture2D* Marker Texture;                                                 // 0x0318 (size: 0x8)
    FText Marker Title;                                                               // 0x0320 (size: 0x10)
    FText Marker Description;                                                         // 0x0330 (size: 0x10)
    class ABP_Marker_C* Marker;                                                       // 0x0340 (size: 0x8)
    double DistanceToDiscoverPrefab;                                                  // 0x0348 (size: 0x8)

    void IsDynamicEvent?(bool& Dynamic?);
    void IsEventSpawned?(bool& Spawned?);
    void DistanceToPrefab();
    void PrefabTypeChanceMulitplier(double& Chance);
    void CheckSurroundingArea(bool& Spawn?);
    void Random Check(bool& Spawn);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void SpawnRandomEvent();
    void SpawnMarker();
    void ExecuteUbergraph_BP_PrefabSpawner(int32 EntryPoint);
}; // Size: 0x350

#endif
