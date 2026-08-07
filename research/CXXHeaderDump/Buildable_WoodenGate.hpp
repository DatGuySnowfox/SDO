#ifndef UE4SS_SDK_Buildable_WoodenGate_HPP
#define UE4SS_SDK_Buildable_WoodenGate_HPP

class ABuildable_WoodenGate_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UBoxComponent* NavObstacleBox3;                                             // 0x0438 (size: 0x8)
    class UBoxComponent* NavObstacleBox2;                                             // 0x0440 (size: 0x8)
    class UBoxComponent* NavObstacleBox1;                                             // 0x0448 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0450 (size: 0x8)
    class UStaticMeshComponent* Gate_DoorRight;                                       // 0x0458 (size: 0x8)
    class UStaticMeshComponent* Gate_DoorLeft;                                        // 0x0460 (size: 0x8)
    float Timeline_1_Alpha_D747FEAD4957875BCD02E7BDA1BE3E11;                          // 0x0468 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_1__Direction_D747FEAD4957875BCD02E7BDA1BE3E11; // 0x046C (size: 0x1)
    class UTimelineComponent* Timeline_1;                                             // 0x0470 (size: 0x8)
    float Timeline_0_Alpha_39F73D344D4FC74CB7D7A4841DE6A312;                          // 0x0478 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_0__Direction_39F73D344D4FC74CB7D7A4841DE6A312; // 0x047C (size: 0x1)
    class UTimelineComponent* Timeline_0;                                             // 0x0480 (size: 0x8)
    bool IsClosed;                                                                    // 0x0488 (size: 0x1)
    double DotProduct;                                                                // 0x0490 (size: 0x8)

    void Timeline_1__FinishedFunc();
    void Timeline_1__UpdateFunc();
    void Timeline_0__FinishedFunc();
    void Timeline_0__UpdateFunc();
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_OpenDoor();
    void ExecuteUbergraph_Buildable_WoodenGate(int32 EntryPoint);
}; // Size: 0x498

#endif
