#ifndef UE4SS_SDK_BP_POIManager_HPP
#define UE4SS_SDK_BP_POIManager_HPP

class ABP_POIManager_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USceneComponent* BossLoc;                                                   // 0x02A8 (size: 0x8)
    class USphereComponent* DiscoveryOverlap;                                         // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* Crate;                                                // 0x02B8 (size: 0x8)
    class USphereComponent* LootOverlap;                                              // 0x02C0 (size: 0x8)
    float LootRadius;                                                                 // 0x02C8 (size: 0x4)
    FVector SphereOffset;                                                             // 0x02D0 (size: 0x18)
    FStruct_POI POI;                                                                  // 0x02E8 (size: 0x70)
    TArray<FName> UniqueLootSets;                                                     // 0x0358 (size: 0x10)
    bool PreAddMarker?;                                                               // 0x0368 (size: 0x1)
    bool Explored?;                                                                   // 0x0369 (size: 0x1)
    bool UniqueContainerLooted?;                                                      // 0x036A (size: 0x1)
    FVector BossOffset;                                                               // 0x0370 (size: 0x18)
    float ExploredCheckRadius;                                                        // 0x0388 (size: 0x4)
    FTransform ContainerLocation;                                                     // 0x0390 (size: 0x60)
    class UW_POIMarker_C* Marker;                                                     // 0x03F0 (size: 0x8)
    TArray<FString> Names;                                                            // 0x03F8 (size: 0x10)
    TArray<class TSubclassOf<AActor>> BossToSpawn;                                    // 0x0408 (size: 0x10)
    int32 SpawnAmount;                                                                // 0x0418 (size: 0x4)
    FTimerHandle TimerHandle_CheckSpawnRadius;                                        // 0x0420 (size: 0x8)
    float InitialSpawnDelay;                                                          // 0x0428 (size: 0x4)
    double RandomSpawnRadius;                                                         // 0x0430 (size: 0x8)
    int32 BossesDefeated;                                                             // 0x0438 (size: 0x4)
    FBP_POIManager_COnActorSpawned OnActorSpawned;                                    // 0x0440 (size: 0x10)
    void OnActorSpawned(class AActor* SpawnedActor);
    bool InsidePOI?;                                                                  // 0x0450 (size: 0x1)
    FBP_POIManager_CEnteringPOI EnteringPOI;                                          // 0x0458 (size: 0x10)
    void EnteringPOI(bool Entered?);
    class AContainer_POICrate_C* Container;                                           // 0x0468 (size: 0x8)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void BossSpawnCheckDebug();
    bool CheckLoc();
    void RandomiseSpawn(class USceneComponent* SceneComp, FTransform& Transform);
    void IsAnyInvokerInRadius(double Radius, bool& Result);
    void DistanceCheck();
    void UserConstructionScript();
    void OnCompleted_DE194BCB4F4F1980C26078A8AEC5DE86();
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void ReceiveBeginPlay();
    void RespawnMarker();
    void Event_Looted();
    void ActorLoaded();
    void BossDeath();
    void SpawnBosses();
    void CheckRadius();
    void SpawnBossOnRadius();
    void ExecuteUbergraph_BP_POIManager(int32 EntryPoint);
    void EnteringPOI__DelegateSignature(bool Entered?);
    void OnActorSpawned__DelegateSignature(class AActor* SpawnedActor);
}; // Size: 0x470

#endif
