#ifndef UE4SS_SDK_BP_AISpawningVolume_HPP
#define UE4SS_SDK_BP_AISpawningVolume_HPP

class ABP_AISpawningVolume_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USphereComponent* Proximity Deactivation Sphere;                            // 0x02A0 (size: 0x8)
    class USphereComponent* Proximity Activation Sphere;                              // 0x02A8 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02B0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B8 (size: 0x8)
    class UBoxComponent* AI Spawn Volume;                                             // 0x02C0 (size: 0x8)
    FS_AISpawner AI Spawn Element;                                                    // 0x02C8 (size: 0x10)
    bool Enable Spawn AI;                                                             // 0x02D8 (size: 0x1)
    TArray<FS_AISpawner> Spawning AI;                                                 // 0x02E0 (size: 0x10)
    double Spawn Time;                                                                // 0x02F0 (size: 0x8)
    double Spawn Time Deviation;                                                      // 0x02F8 (size: 0x8)
    double Overlap Size Check;                                                        // 0x0300 (size: 0x8)
    bool Random Rotations;                                                            // 0x0308 (size: 0x1)
    TArray<class AActor*> Floors;                                                     // 0x0310 (size: 0x10)
    int32 Spawn Retries;                                                              // 0x0320 (size: 0x4)
    TArray<class AActor*> Spawned AI;                                                 // 0x0328 (size: 0x10)
    bool Respawn AI;                                                                  // 0x0338 (size: 0x1)
    double Respawn Time;                                                              // 0x0340 (size: 0x8)
    double Respawn Time Variation;                                                    // 0x0348 (size: 0x8)
    TArray<FS_AIRespawn> AI Respawning Timers;                                        // 0x0350 (size: 0x10)
    bool Use Player Proximity Activation;                                             // 0x0360 (size: 0x1)
    double Player Activation Range;                                                   // 0x0368 (size: 0x8)
    bool Show Deactivation Proximity;                                                 // 0x0370 (size: 0x1)
    bool Show Activation Proximity;                                                   // 0x0371 (size: 0x1)
    bool Player In Area;                                                              // 0x0372 (size: 0x1)
    int32 AI Total;                                                                   // 0x0374 (size: 0x4)
    TArray<FS_AISpawner> Remaining Spawning AI;                                       // 0x0378 (size: 0x10)
    int32 Spawn Index;                                                                // 0x0388 (size: 0x4)
    bool Use Player Proximity Deactivation;                                           // 0x038C (size: 0x1)
    double Player Deactivation Range;                                                 // 0x0390 (size: 0x8)
    bool Override Starting Behaviour;                                                 // 0x0398 (size: 0x1)
    TEnumAsByte<E_StartingAIBehaviours::Type> Override Behaviour;                     // 0x0399 (size: 0x1)
    TArray<class ABP_MasterWayPoint_C*> Way Points;                                   // 0x03A0 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Activated Responses;                           // 0x03B0 (size: 0x10)
    TArray<FS_TriggerResponse> Trigger Deactivated Responses;                         // 0x03C0 (size: 0x10)
    FName StreamLevelPackageName;                                                     // 0x03D0 (size: 0x8)
    class AActor* AIBase;                                                             // 0x03D8 (size: 0x8)

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
    void Reset Proximity Activation();
    void BndEvt__Proximity Activation Sphere_K2Node_ComponentBoundEvent_1_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__Proximity Deactivation Sphere_K2Node_ComponentBoundEvent_2_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Reset Proximity Deactivation();
    void Restart Spawning();
    void Stop Spawning();
    void RespawnAI(class APawn* Pawn);
    void ExecuteUbergraph_BP_AISpawningVolume(int32 EntryPoint);
}; // Size: 0x3E0

#endif
