#ifndef UE4SS_SDK_BP_AISpawningVolume_Prefab_HPP
#define UE4SS_SDK_BP_AISpawningVolume_Prefab_HPP

class ABP_AISpawningVolume_Prefab_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02B8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02C0 (size: 0x8)
    class UBoxComponent* AI Spawn Volume;                                             // 0x02C8 (size: 0x8)
    FS_AISpawner AI Spawn Element;                                                    // 0x02D0 (size: 0x10)
    bool Enable Spawn AI;                                                             // 0x02E0 (size: 0x1)
    bool IgnoreNoSpawnArea?;                                                          // 0x02E1 (size: 0x1)
    TArray<FS_AISpawner> Spawning AI;                                                 // 0x02E8 (size: 0x10)
    double InitialSpawnDelay;                                                         // 0x02F8 (size: 0x8)
    double Spawn Time;                                                                // 0x0300 (size: 0x8)
    double Spawn Time Deviation;                                                      // 0x0308 (size: 0x8)
    double Overlap Size Check;                                                        // 0x0310 (size: 0x8)
    bool Random Rotations;                                                            // 0x0318 (size: 0x1)
    TArray<AActor*> Floors;                                                           // 0x0320 (size: 0x10)
    int32 Spawn Retries;                                                              // 0x0330 (size: 0x4)
    TArray<AActor*> Spawned AI;                                                       // 0x0338 (size: 0x10)
    bool Respawn AI;                                                                  // 0x0348 (size: 0x1)
    double Respawn Time Variation;                                                    // 0x0350 (size: 0x8)
    TArray<FS_AIRespawn> AI Respawning Timers;                                        // 0x0358 (size: 0x10)
    bool Show Deactivation Proximity;                                                 // 0x0368 (size: 0x1)
    bool Show Activation Proximity;                                                   // 0x0369 (size: 0x1)
    bool Player In Area;                                                              // 0x036A (size: 0x1)
    int32 AI Total;                                                                   // 0x036C (size: 0x4)
    TArray<FS_AISpawner> Remaining Spawning AI;                                       // 0x0370 (size: 0x10)
    int32 Spawn Index;                                                                // 0x0380 (size: 0x4)
    bool Override Starting Behaviour;                                                 // 0x0384 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Override Behaviour;                     // 0x0385 (size: 0x1)
    TArray<ABP_MasterWayPoint_C*> Way Points;                                         // 0x0388 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Activated Responses;                           // 0x0398 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Responses;                         // 0x03A8 (size: 0x10)
    FName StreamLevelPackageName;                                                     // 0x03B8 (size: 0x8)
    class AActor* AIBase;                                                             // 0x03C0 (size: 0x8)
    TArray<FName> UniqueLoot;                                                         // 0x03C8 (size: 0x10)
    TArray<FAIOSubjectHandle> SpawnedActorHandles;                                    // 0x03D8 (size: 0x10)
    double SpawnRadius;                                                               // 0x03E8 (size: 0x8)
    bool SkipFloorCheck?;                                                             // 0x03F0 (size: 0x1)
    FTimerHandle TimerHandle_CheckSpawnRadius;                                        // 0x03F8 (size: 0x8)

    void IsAnyInvokerInRadius?(double Radius, bool& Result);
    void TraceForPlayer(FVector Loc, bool& PlayerHit?);
    void ConvertLocalToWorldLocation(FVector& Point_Local, FVector& Point_World);
    void Reset AI(const FHitResult& Hit Result, class ACharacter* AI Character);
    bool Find Spawn Location(FHitResult& Hit Result);
    void Respawn Timer();
    void Spawn Location Trace(int32 Index);
    void ReceiveBeginPlay();
    void Add AI Respawn Timer(class ACharacter* AI Character);
    void AI Respawn Timer Finished(class ACharacter* AI Character);
    void Reset Find Respawn();
    void Restart Spawning();
    void Stop Spawning();
    void RespawnAI(class APawn* Pawn);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void KillSpawnedActors();
    void InitSpawnByRadius();
    void CheckSpawningRadius();
    void ExecuteUbergraph_BP_AISpawningVolume_Prefab(int32 EntryPoint);
}; // Size: 0x400

#endif
