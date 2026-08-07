#ifndef UE4SS_SDK_BP_AISpawningVolume_HumanQuest_HPP
#define UE4SS_SDK_BP_AISpawningVolume_HumanQuest_HPP

class ABP_AISpawningVolume_HumanQuest_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* Proximity Deactivation Sphere;                            // 0x02A8 (size: 0x8)
    class USphereComponent* Proximity Activation Sphere;                              // 0x02B0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02B8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02C0 (size: 0x8)
    class UBoxComponent* AI Spawn Volume;                                             // 0x02C8 (size: 0x8)
    FS_AISpawner AI Spawn Element;                                                    // 0x02D0 (size: 0x10)
    bool Enable Spawn AI;                                                             // 0x02E0 (size: 0x1)
    TArray<FS_AISpawner> Spawning AI;                                                 // 0x02E8 (size: 0x10)
    double InitialSpawnDelay;                                                         // 0x02F8 (size: 0x8)
    double Spawn Time;                                                                // 0x0300 (size: 0x8)
    double Spawn Time Deviation;                                                      // 0x0308 (size: 0x8)
    double Overlap Size Check;                                                        // 0x0310 (size: 0x8)
    bool Random Rotations;                                                            // 0x0318 (size: 0x1)
    bool Skip Meshes;                                                                 // 0x0319 (size: 0x1)
    TArray<class AActor*> Meshes;                                                     // 0x0320 (size: 0x10)
    int32 Spawn Retries;                                                              // 0x0330 (size: 0x4)
    TArray<class AActor*> Spawned AI;                                                 // 0x0338 (size: 0x10)
    bool Respawn AI;                                                                  // 0x0348 (size: 0x1)
    double Respawn Time Variation;                                                    // 0x0350 (size: 0x8)
    TArray<FS_AIRespawn> AI Respawning Timers;                                        // 0x0358 (size: 0x10)
    bool Use Player Proximity Activation;                                             // 0x0368 (size: 0x1)
    double Player Activation Range;                                                   // 0x0370 (size: 0x8)
    bool Show Deactivation Proximity;                                                 // 0x0378 (size: 0x1)
    bool Show Activation Proximity;                                                   // 0x0379 (size: 0x1)
    bool Player In Area;                                                              // 0x037A (size: 0x1)
    int32 AI Total;                                                                   // 0x037C (size: 0x4)
    TArray<FS_AISpawner> Remaining Spawning AI;                                       // 0x0380 (size: 0x10)
    int32 Spawn Index;                                                                // 0x0390 (size: 0x4)
    bool Use Player Proximity Deactivation;                                           // 0x0394 (size: 0x1)
    double Player Deactivation Range;                                                 // 0x0398 (size: 0x8)
    bool Override Starting Behaviour;                                                 // 0x03A0 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Override Behaviour;                     // 0x03A1 (size: 0x1)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x03A8 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Activated Responses;                           // 0x03B8 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Responses;                         // 0x03C8 (size: 0x10)
    FName StreamLevelPackageName;                                                     // 0x03D8 (size: 0x8)
    class AActor* AIBase;                                                             // 0x03E0 (size: 0x8)
    TArray<FAIOSubjectHandle> SpawnedActorHandles;                                    // 0x03E8 (size: 0x10)
    TArray<FName> UniqueLoot;                                                         // 0x03F8 (size: 0x10)
    FString QuestArgument;                                                            // 0x0408 (size: 0x10)
    double Box Extent X;                                                              // 0x0418 (size: 0x8)
    double Box Extent Y;                                                              // 0x0420 (size: 0x8)
    double Box Extent Z;                                                              // 0x0428 (size: 0x8)

    void TraceForPlayer(FVector Loc, bool& PlayerHit?);
    void PlayerInAreaCheck();
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
    void ExecuteUbergraph_BP_AISpawningVolume_HumanQuest(int32 EntryPoint);
}; // Size: 0x430

#endif
