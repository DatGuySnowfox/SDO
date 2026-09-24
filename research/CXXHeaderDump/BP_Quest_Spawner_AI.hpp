#ifndef UE4SS_SDK_BP_Quest_Spawner_AI_HPP
#define UE4SS_SDK_BP_Quest_Spawner_AI_HPP

class ABP_Quest_Spawner_AI_C : public ABP_MasterQuestObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UBoxComponent* SpawningBox;                                                 // 0x02D8 (size: 0x8)
    class UTextRenderComponent* TextRender;                                           // 0x02E0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02E8 (size: 0x8)
    double ZOffset;                                                                   // 0x02F0 (size: 0x8)
    int32 AmountToSpawn;                                                              // 0x02F8 (size: 0x4)
    FTimerHandle TimerHandle_CheckSpawnRadius;                                        // 0x0300 (size: 0x8)
    FBP_Quest_Spawner_AI_CAIDeath AIDeath;                                            // 0x0308 (size: 0x10)
    void AIDeath();
    FName QuestTag;                                                                   // 0x0318 (size: 0x8)
    TSubclassOf<class APrefabMaster_C> PrefabToSpawn;                                 // 0x0320 (size: 0x8)
    int32 SpawnAmount;                                                                // 0x0328 (size: 0x4)
    TArray<TSubclassOf<class AActor>> AIToSpawn;                                      // 0x0330 (size: 0x10)
    TArray<FVector> AITransform;                                                      // 0x0340 (size: 0x10)
    double AISpawnerExtent;                                                           // 0x0350 (size: 0x8)
    double AISpawnerDepth;                                                            // 0x0358 (size: 0x8)
    double RadiusToSpawn;                                                             // 0x0360 (size: 0x8)
    FVector AISpawnerOffset;                                                          // 0x0368 (size: 0x18)
    double AISpawnerRotation;                                                         // 0x0380 (size: 0x8)

    void 3_Spawn Prefab For Testing();
    void ProjectLocation(FVector& LocationToProject, bool& Success, FVector& ProjectedLocation);
    void GetRandomSpawnPoint(FVector& SpawnPoint, bool& Success);
    void 2_Clear Spawn Points();
    void 1_Generate Spawn Points();
    void IsAnyInvokerInRadius(double Radius, bool& Result);
    void UserConstructionScript();
    void SpawnOnRadius();
    void CheckRadius();
    void SpawnAI();
    void EventDeath(class AActor* DmgCauser, bool Headshot);
    void Quest_Spawn(int32 AmountToSpawn);
    void ExecuteUbergraph_BP_Quest_Spawner_AI(int32 EntryPoint);
    void AIDeath__DelegateSignature();
}; // Size: 0x388

#endif
