#ifndef UE4SS_SDK_BP_AISpawner_Master_HPP
#define UE4SS_SDK_BP_AISpawner_Master_HPP

class ABP_AISpawner_Master_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A8 (size: 0x8)
    class USphereComponent* SpawnRadius_DebugSphere;                                  // 0x02B0 (size: 0x8)
    class UBoxComponent* SpawningBox;                                                 // 0x02B8 (size: 0x8)
    TArray<FAIOPendingSpawnGroup> SpawnGroupsQueue;                                   // 0x02C0 (size: 0x10)
    TArray<FAIOPendingRespawnGroup> RespawnGroupsQueue;                               // 0x02D0 (size: 0x10)
    TArray<FAIOSubjectHandle> SpawnedActorsHandles;                                   // 0x02E0 (size: 0x10)
    TArray<class AActor*> SpawnedActors;                                              // 0x02F0 (size: 0x10)
    FTimerHandle TimerHandle_CheckSpawnRadius;                                        // 0x0300 (size: 0x8)
    FTimerHandle TimeHandle_UpdateRespawningGroups;                                   // 0x0308 (size: 0x8)
    double SpawnRadius;                                                               // 0x0310 (size: 0x8)
    double Box Extent X;                                                              // 0x0318 (size: 0x8)
    double Box Extent Y;                                                              // 0x0320 (size: 0x8)
    double Box Extent Z;                                                              // 0x0328 (size: 0x8)
    int32 SpawnAmount;                                                                // 0x0330 (size: 0x4)
    bool RandomisedSpawnCount;                                                        // 0x0334 (size: 0x1)
    TEnumAsByte<Enum_SpawnType::Type> SpawnType;                                      // 0x0335 (size: 0x1)
    TSubclassOf<class AActor> SpawnedActorClass;                                      // 0x0338 (size: 0x8)
    double InitialSpawnDelay;                                                         // 0x0340 (size: 0x8)
    double SpawningInterval;                                                          // 0x0348 (size: 0x8)
    bool bIsSpawning;                                                                 // 0x0350 (size: 0x1)
    int32 TotalAliveActors;                                                           // 0x0354 (size: 0x4)
    int32 TotalSpawnedActors;                                                         // 0x0358 (size: 0x4)
    EAIOSelectSpawnPointsMethod SelectingSpawnPointsMethod;                           // 0x035C (size: 0x1)
    bool SkipNoSpawnAreaCheck?;                                                       // 0x035D (size: 0x1)
    bool SkipMeshes?;                                                                 // 0x035E (size: 0x1)
    TArray<class AActor*> MeshesToSpawnOn;                                            // 0x0360 (size: 0x10)
    int32 SpawnRetries;                                                               // 0x0370 (size: 0x4)
    float DontSpawnCloseToPlayerRange;                                                // 0x0374 (size: 0x4)
    EAIORespawnMethod RespawnMethod;                                                  // 0x0378 (size: 0x1)
    bool DisplayDebugInfo;                                                            // 0x0379 (size: 0x1)
    TEnumAsByte<ETraceTypeQuery> ProjectionChannel;                                   // 0x037A (size: 0x1)
    double ProjectionRadius;                                                          // 0x0380 (size: 0x8)
    double ProjectionZOffset;                                                         // 0x0388 (size: 0x8)
    TArray<FAIOSpawnPoint> SpawnPoints;                                               // 0x0390 (size: 0x10)
    bool IsSpawningStopped;                                                           // 0x03A0 (size: 0x1)
    bool WasSpawningInitialized;                                                      // 0x03A1 (size: 0x1)
    int32 InvokersInsideRegion;                                                       // 0x03A4 (size: 0x4)
    int32 SpawningCapacity;                                                           // 0x03A8 (size: 0x4)
    int32 ActorsSpawnedThisFrame;                                                     // 0x03AC (size: 0x4)
    FBP_AISpawner_Master_COnActorSpawned OnActorSpawned;                              // 0x03B0 (size: 0x10)
    void OnActorSpawned(class AActor* SpawnedActor);
    TEnumAsByte<Enum_AIBehviour::Type> Starting Behaviour;                            // 0x03C0 (size: 0x1)
    FVector LocationToGoTo;                                                           // 0x03C8 (size: 0x18)
    class ABP_AIWaypoint_C* Waypoint;                                                 // 0x03E0 (size: 0x8)
    FString ArgumentForQuest;                                                         // 0x03E8 (size: 0x10)
    bool IsAtSettlementSpawn?;                                                        // 0x03F8 (size: 0x1)
    FBP_AISpawner_Master_CSettlementSpawn SettlementSpawn;                            // 0x0400 (size: 0x10)
    void SettlementSpawn();
    bool SkipPlayerCheck?;                                                            // 0x0410 (size: 0x1)

    void SetSpawnCount(int32& Count);
    void GetSpawnPointAtIndex(int32 Index, bool& Success, FAIOSpawnPoint& SpawnPoint);
    void GenerateSpawnPoints();
    void ClearSpawnPoints();
    void ProjectSpawnPoints();
    void TraceForPlayer(FVector Loc, bool& PlayerHit?);
    void OverrideBehaviour(class AActor* Actor);
    void GetCVarType(FName& CVar);
    void IsSpawnedClassReplicated(bool& Replicates);
    void SetWasSpawningInitialized(bool IsInitialized);
    void SetIsSpawningStopped(bool Stop);
    void ConvertWorldToLocalSpawnPoint(FAIOSpawnPoint& SpawnPoint_WorldSpace, FAIOSpawnPoint& SpawnPoint_LocalSpace);
    void ConvertLocalToWorldSpawnPoint(FAIOSpawnPoint& SpawnPoint_LocalSpace, FAIOSpawnPoint& SpawnPoint_WorldSpace);
    void ConvertWorldToLocalLocation(FVector& Point_Local, FVector& Point_World);
    void ConvertLocalToWorldLocation(FVector& Point_Local, FVector& Point_World);
    void ProjectLocation(FVector& LocationToProject, bool& Success, FVector& ProjectedLocation);
    void GetRandomSpawnPoint(FAIOSpawnPoint& SpawnPoint, bool& Success);
    void UpdateSpawnerAfterActorWasKilled(int32 KilledAIAmount);
    void OnSpawnedActorKilled(class AActor* KilledAI);
    void CanSpawnerRespawnActors(bool& Result);
    void CanSpawnActors(bool& Result);
    bool IsAnyInvokerInSpawnRegion();
    void IsAnyInvokerInRadius(double Radius, bool& Result);
    void UpdateSpawnerAfterSpawnActor(bool& FinishedSpawningGroup);
    void SpawnActor();
    void UserConstructionScript();
    void AddGroupToSpawn(int32 AmountToSpawn);
    void TryToSpawnGroup();
    void SpawningLoop();
    void FinishedSpawningGroup();
    void ReceiveBeginPlay();
    void KillSpawnedActors(bool AllowRespawn);
    void UpdateRespawningGroups();
    void AddGroupToRepsawn(double Delay, int32 Amount);
    void CheckStartSpawningWithRadius();
    void ResetSpawner(bool ResetIsSpawningStopped);
    void InitSpawner();
    void InitSpawnByRadius();
    void OnSubjectSpawnedByOptimizerSubsystem(class UAIOSubjectComponent* SpawnedSubjectComponent);
    void OnSubjectDespawnedByOptimizerSubsystem(class UAIOSubjectComponent* DespawnedSubjectComponent);
    void PostSpawn(class AActor* SpawnedActor);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void EventSettlement();
    void ExecuteUbergraph_BP_AISpawner_Master(int32 EntryPoint);
    void SettlementSpawn__DelegateSignature();
    void OnActorSpawned__DelegateSignature(class AActor* SpawnedActor);
}; // Size: 0x411

#endif
