#ifndef UE4SS_SDK_BP_AISpawningVolume_RandomChanceToSpawn_HPP
#define UE4SS_SDK_BP_AISpawningVolume_RandomChanceToSpawn_HPP

class ABP_AISpawningVolume_RandomChanceToSpawn_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* Radius;                                                   // 0x02A8 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02B0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B8 (size: 0x8)
    class UBoxComponent* AI Spawn Volume;                                             // 0x02C0 (size: 0x8)
    FS_AISpawner AI Spawn Element;                                                    // 0x02C8 (size: 0x10)
    bool Enable Spawn AI;                                                             // 0x02D8 (size: 0x1)
    bool IgnoreNoSpawnArea?;                                                          // 0x02D9 (size: 0x1)
    bool SkipPlayerCheck?;                                                            // 0x02DA (size: 0x1)
    TArray<FS_AISpawner> Spawning AI;                                                 // 0x02E0 (size: 0x10)
    double InitialSpawnDelay;                                                         // 0x02F0 (size: 0x8)
    double Spawn Time;                                                                // 0x02F8 (size: 0x8)
    double Spawn Time Deviation;                                                      // 0x0300 (size: 0x8)
    double Overlap Size Check;                                                        // 0x0308 (size: 0x8)
    bool Random Rotations;                                                            // 0x0310 (size: 0x1)
    TArray<class AActor*> Floors;                                                     // 0x0318 (size: 0x10)
    int32 Spawn Retries;                                                              // 0x0328 (size: 0x4)
    TArray<class AActor*> Spawned AI;                                                 // 0x0330 (size: 0x10)
    bool Respawn AI;                                                                  // 0x0340 (size: 0x1)
    double Respawn Time Variation;                                                    // 0x0348 (size: 0x8)
    TArray<FS_AIRespawn> AI Respawning Timers;                                        // 0x0350 (size: 0x10)
    bool Show Deactivation Proximity;                                                 // 0x0360 (size: 0x1)
    bool Show Activation Proximity;                                                   // 0x0361 (size: 0x1)
    int32 AI Total;                                                                   // 0x0364 (size: 0x4)
    TArray<FS_AISpawner> Remaining Spawning AI;                                       // 0x0368 (size: 0x10)
    int32 Spawn Index;                                                                // 0x0378 (size: 0x4)
    bool Override Starting Behaviour;                                                 // 0x037C (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Override Behaviour;                     // 0x037D (size: 0x1)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x0380 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Activated Responses;                           // 0x0390 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Responses;                         // 0x03A0 (size: 0x10)
    FName StreamLevelPackageName;                                                     // 0x03B0 (size: 0x8)
    class AActor* AIBase;                                                             // 0x03B8 (size: 0x8)
    TArray<FName> UniqueLoot;                                                         // 0x03C0 (size: 0x10)
    double Chance;                                                                    // 0x03D0 (size: 0x8)
    TArray<FAIOSubjectHandle> SpawnedActorHandles;                                    // 0x03D8 (size: 0x10)
    FTimerHandle TimerHandle_CheckSpawnRadius;                                        // 0x03E8 (size: 0x8)
    double SpawnRadius;                                                               // 0x03F0 (size: 0x8)

    void IsAnyInvokerInRadius?(double Radius, bool& Result);
    void TraceForPlayer(FVector Loc, bool& PlayerHit?);
    void ConvertLocalToWorldLocation(FVector& Point_Local, FVector& Point_World);
    bool RandomSpawn();
    void Reset AI(const FHitResult& Hit Result, class ACharacter* AI Character);
    bool Find Spawn Location(FHitResult& Hit Result);
    void Respawn Timer();
    void Spawn Location Trace(int32 Index);
    void UserConstructionScript();
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
    void ExecuteUbergraph_BP_AISpawningVolume_RandomChanceToSpawn(int32 EntryPoint);
}; // Size: 0x3F8

#endif
