#ifndef UE4SS_SDK_Buildable_BearTrap_HPP
#define UE4SS_SDK_Buildable_BearTrap_HPP

class ABuildable_BearTrap_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UBoxComponent* Box_0;                                                       // 0x0438 (size: 0x8)
    class UStaticMeshComponent* SM_Prop_Bear_Trap_Jaw_02;                             // 0x0440 (size: 0x8)
    class UStaticMeshComponent* SM_Prop_Bear_Trap_Jaw_01;                             // 0x0448 (size: 0x8)
    float BearTrapTimeline_Track_F03079BE4DF4E85E091CDF8CCFAD5E3E;                    // 0x0450 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> BearTrapTimeline__Direction_F03079BE4DF4E85E091CDF8CCFAD5E3E; // 0x0454 (size: 0x1)
    class UTimelineComponent* BearTrapTimeline;                                       // 0x0458 (size: 0x8)

    void BearTrapTimeline__FinishedFunc();
    void BearTrapTimeline__UpdateFunc();
    void Event_CatchCharacter(class ACharacter* Character);
    void BndEvt__Buildable_BearTrap_Box_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void Event_OpenBearTrap();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_BearTrap(int32 EntryPoint);
}; // Size: 0x460

#endif
