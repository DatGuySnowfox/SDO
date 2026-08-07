#ifndef UE4SS_SDK_Buildable_HordeBeacon_HPP
#define UE4SS_SDK_Buildable_HordeBeacon_HPP

class ABuildable_HordeBeacon_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UMusicLayeringComponent_C* MusicLayeringComponent;                          // 0x0438 (size: 0x8)
    class USceneComponent* RewardSpawnLoc;                                            // 0x0440 (size: 0x8)
    class UStaticMeshComponent* Cube;                                                 // 0x0448 (size: 0x8)
    class UStaticMeshComponent* Dish;                                                 // 0x0450 (size: 0x8)
    float Timeline_Rotate_Alpha_A94CB4934F9330DDB7DED1A3F2900B85;                     // 0x0458 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_A94CB4934F9330DDB7DED1A3F2900B85; // 0x045C (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x0460 (size: 0x8)
    FS_RandomSpawnerSettings Rewards;                                                 // 0x0468 (size: 0x18)
    float Spawn Radius;                                                               // 0x0480 (size: 0x4)
    double NoSpawnRadius;                                                             // 0x0488 (size: 0x8)
    FStruct_WaveInfo HordeWaveSettings;                                               // 0x0490 (size: 0x10)
    int32 CurrentWave;                                                                // 0x04A0 (size: 0x4)
    TArray<class AActor*> CurrentEnemy;                                               // 0x04A8 (size: 0x10)
    bool HordeStarted?;                                                               // 0x04B8 (size: 0x1)
    bool CanStopHorde?;                                                               // 0x04B9 (size: 0x1)
    FTimerHandle NotificationsTimer;                                                  // 0x04C0 (size: 0x8)
    int32 Spawn Retries;                                                              // 0x04C8 (size: 0x4)
    TEnumAsByte<Enum_AIBehviour::Type> Starting Behaviour;                            // 0x04CC (size: 0x1)

    void TraceForBuildables(bool& BuildableInRadius?);
    void ProjectLocation(FVector LocationToProject, bool& Success, FVector& ProjectedLocation);
    void SpawnRewards();
    void FinalWaveDefeated?();
    void EnemyDefeatedCheck(bool& All Defeated?);
    void FindLocation(bool& Success, FVector& Loc);
    void Stop Music?(bool Stop?);
    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void ReceiveBeginPlay();
    void Event_NextWave();
    void Event_StartHorde();
    void Event_StopHorde();
    void Event_PlayerDead();
    void Event_SpawnBeacon();
    void Event_DishMovement(bool Start?);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnDestroy(class AActor* Actor);
    void OnInteractBuildable(class AActor* Actor);
    void ExecuteUbergraph_Buildable_HordeBeacon(int32 EntryPoint);
}; // Size: 0x4CD

#endif
