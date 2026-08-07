#ifndef UE4SS_SDK_BP_AISpawnPoint_HPP
#define UE4SS_SDK_BP_AISpawnPoint_HPP

class ABP_AISpawnPoint_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USphereComponent* Proximity Deactivation Sphere;                            // 0x02A0 (size: 0x8)
    class USphereComponent* Proximity Activation Sphere;                              // 0x02A8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B0 (size: 0x8)
    class UArrowComponent* Spawn Point Rotation;                                      // 0x02B8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02C0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02C8 (size: 0x8)
    bool Enable Spawn AI;                                                             // 0x02D0 (size: 0x1)
    TSubclassOf<class ABP_MasterAICharacter_C> Spawn AI;                              // 0x02D8 (size: 0x8)
    int32 Spawn AI Count;                                                             // 0x02E0 (size: 0x4)
    bool Change Default Behaviour;                                                    // 0x02E4 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Starting Behaviour;                     // 0x02E5 (size: 0x1)
    bool Random Rotation;                                                             // 0x02E6 (size: 0x1)
    double Spawn Time;                                                                // 0x02E8 (size: 0x8)
    double Spawn Time Deviation;                                                      // 0x02F0 (size: 0x8)
    bool Respawn AI;                                                                  // 0x02F8 (size: 0x1)
    double Respawn Time;                                                              // 0x0300 (size: 0x8)
    double Respawn Time Variation;                                                    // 0x0308 (size: 0x8)
    TArray<class AActor*> Spawned AI;                                                 // 0x0310 (size: 0x10)
    bool Show Rotation;                                                               // 0x0320 (size: 0x1)
    bool Use Player Proximity Activation;                                             // 0x0321 (size: 0x1)
    double Player Activation Range;                                                   // 0x0328 (size: 0x8)
    bool Show Deactivation Proximity;                                                 // 0x0330 (size: 0x1)
    bool Show Activation Proximity;                                                   // 0x0331 (size: 0x1)
    bool Use Player Proximity Deactivation;                                           // 0x0332 (size: 0x1)
    double Player Deactivation Range;                                                 // 0x0338 (size: 0x8)
    bool Player In Area;                                                              // 0x0340 (size: 0x1)
    int32 Current AI Spawned;                                                         // 0x0344 (size: 0x4)
    TArray<FS_AIRespawn> AI Respawning Timers;                                        // 0x0348 (size: 0x10)
    double AI Spawn Hight;                                                            // 0x0358 (size: 0x8)
    class AActor* AIBase;                                                             // 0x0360 (size: 0x8)
    TArray<FName> Spawned AI Tags;                                                    // 0x0368 (size: 0x10)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x0378 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Activated Responses;                           // 0x0388 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Responses;                         // 0x0398 (size: 0x10)
    FName StreamLevelPackageName;                                                     // 0x03A8 (size: 0x8)

    void PlayerInAreaCheck();
    void Respawn AI Actor(class ACharacter* AI Character);
    void Respawn Timer();
    void Spawn AI Actor();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void Start Spawning AI();
    void Reset Proximity Activation();
    void BndEvt__Proximity Activation Sphere_K2Node_ComponentBoundEvent_1_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__Proximity Deactivation Sphere_K2Node_ComponentBoundEvent_2_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Reset Proximity Deactivation();
    void Spawn Timer();
    void Add AI Respawn Timer(class ACharacter* AI Character);
    void AI Respawn Timer Finished(class ACharacter* AI Character);
    void RespawnAI(class APawn* Pawn);
    void ExecuteUbergraph_BP_AISpawnPoint(int32 EntryPoint);
}; // Size: 0x3B0

#endif
