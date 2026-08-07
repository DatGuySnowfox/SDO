#ifndef UE4SS_SDK_BP_AIManager_HPP
#define UE4SS_SDK_BP_AIManager_HPP

class ABP_AIManager_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02A0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    FS_AIOptimization AI Optimization;                                                // 0x02B0 (size: 0xC)
    bool Enable Spawn AI;                                                             // 0x02BC (size: 0x1)
    TArray<FS_AISpawner> Spawn AI;                                                    // 0x02C0 (size: 0x10)
    double Overlap Check;                                                             // 0x02D0 (size: 0x8)
    double Spawn Time;                                                                // 0x02D8 (size: 0x8)
    double Spawn Time Variation;                                                      // 0x02E0 (size: 0x8)
    bool Random Rotations;                                                            // 0x02E8 (size: 0x1)
    FRotator AI Rotation;                                                             // 0x02F0 (size: 0x18)
    int32 Spawn Retries;                                                              // 0x0308 (size: 0x4)
    bool Respawn AI;                                                                  // 0x030C (size: 0x1)
    double Respawn Time;                                                              // 0x0310 (size: 0x8)
    double Respawn Time Variation;                                                    // 0x0318 (size: 0x8)
    TArray<FS_AIRespawn> Respawn Timers;                                              // 0x0320 (size: 0x10)
    TArray<class APawn*> Spawned AI;                                                  // 0x0330 (size: 0x10)
    class ANavMeshBoundsVolume* Nav Bounds;                                           // 0x0340 (size: 0x8)
    FS_AISpawner Spawn Element;                                                       // 0x0348 (size: 0x10)
    TEnumAsByte<EDrawDebugTrace::Type> Spawn Collision Trace;                         // 0x0358 (size: 0x1)
    TArray<FS_AISpawner> Current Spawning AI;                                         // 0x0360 (size: 0x10)
    TArray<class ABP_MasterWayPoint_C*> WayPoints;                                    // 0x0370 (size: 0x10)

    void Return AI Manager Settings(FS_AIOptimization& AI Settings);
    void Return AI Manager(class ABP_AIManager_C*& AI Manager);
    void Respawn AI At Location(FVector Location, class APawn* Pawn);
    void Find Respawn Location(bool& Location Found, FVector& OutHit);
    void Spawn AI At Location();
    void Respawn Timer();
    void ReceiveBeginPlay();
    void Add AI Respawn Timer(class ACharacter* AI Character);
    void AI Respawn Timer Finished(class ACharacter* AI Pawn);
    void Next Spawn AI();
    void RespawnAI(class APawn* Pawn);
    void Reset Respawn();
    void ExecuteUbergraph_BP_AIManager(int32 EntryPoint);
}; // Size: 0x380

#endif
