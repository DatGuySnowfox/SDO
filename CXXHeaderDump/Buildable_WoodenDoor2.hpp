#ifndef UE4SS_SDK_Buildable_WoodenDoor2_HPP
#define UE4SS_SDK_Buildable_WoodenDoor2_HPP

class ABuildable_WoodenDoor2_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UBoxComponent* NavObstacleBox1;                                             // 0x0438 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0440 (size: 0x8)
    float Timeline_OpenDoor_Alpha_338471374D1910D3506CC1B51ECDD906;                   // 0x0448 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline_OpenDoor__Direction_338471374D1910D3506CC1B51ECDD906; // 0x044C (size: 0x1)
    class UTimelineComponent* Timeline_OpenDoor;                                      // 0x0450 (size: 0x8)
    bool IsClosed;                                                                    // 0x0458 (size: 0x1)
    double DotProduct;                                                                // 0x0460 (size: 0x8)

    void Timeline_OpenDoor__FinishedFunc();
    void Timeline_OpenDoor__UpdateFunc();
    void ReceiveBeginPlay();
    void OnInteractBuildable(class AActor* Actor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void Event_OpenDoor();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_WoodenDoor2(int32 EntryPoint);
}; // Size: 0x468

#endif
