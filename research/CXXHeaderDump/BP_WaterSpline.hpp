#ifndef UE4SS_SDK_BP_WaterSpline_HPP
#define UE4SS_SDK_BP_WaterSpline_HPP

class ABP_WaterSpline_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USplineComponent* Spline;                                                   // 0x02A8 (size: 0x8)
    bool ShallowWater?;                                                               // 0x02B0 (size: 0x1)
    class UMaterialInterface* Material;                                               // 0x02B8 (size: 0x8)
    FVector2D Start Scale;                                                            // 0x02C0 (size: 0x10)
    FVector2D End Scale;                                                              // 0x02D0 (size: 0x10)
    class USplineMeshComponent* SplineComp;                                           // 0x02E0 (size: 0x8)
    bool Radiated?;                                                                   // 0x02E8 (size: 0x1)

    void SetCollisionType();
    void UserConstructionScript();
    void BndEvt__BP_WaterSplineTEST_SplineComp_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_WaterSplineTEST_SplineComp_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_WaterSpline(int32 EntryPoint);
}; // Size: 0x2E9

#endif
