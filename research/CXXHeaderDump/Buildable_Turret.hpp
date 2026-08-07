#ifndef UE4SS_SDK_Buildable_Turret_HPP
#define UE4SS_SDK_Buildable_Turret_HPP

class ABuildable_Turret_C : public ABuildable_TurretMASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0480 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x0488 (size: 0x8)
    float Timeline_Rotate_Alpha_AFE5B1D24D7F05708878F0A1F04778DD;                     // 0x0490 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Timeline__Direction_AFE5B1D24D7F05708878F0A1F04778DD; // 0x0494 (size: 0x1)
    class UTimelineComponent* Timeline;                                               // 0x0498 (size: 0x8)

    void Timeline__FinishedFunc();
    void Timeline__UpdateFunc();
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Multicast Turret Idle Stop();
    void Multicast Turret Idle Start();
    void ReceiveBeginPlay();
    void Multicast Spawn Emitter();
    void OnInteractBuildable(class AActor* Actor);
    void BndEvt__Buildable_Turret_Sphere_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__Buildable_Turret_Sphere_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void On();
    void Off();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_Turret(int32 EntryPoint);
}; // Size: 0x4A0

#endif
