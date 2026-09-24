#ifndef UE4SS_SDK_BP_WaterSpline_HPP
#define UE4SS_SDK_BP_WaterSpline_HPP

class ABP_WaterSpline_C : public ABP_MasterWaterActor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0330 (size: 0x8)
    class USplineComponent* Spline;                                                   // 0x0338 (size: 0x8)
    class USplineMeshComponent* SplineComp;                                           // 0x0340 (size: 0x8)

    void UserConstructionScript();
    void BndEvt__BP_WaterSplineTEST_SplineComp_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_WaterSplineTEST_SplineComp_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_WaterSpline(int32 EntryPoint);
}; // Size: 0x348

#endif
