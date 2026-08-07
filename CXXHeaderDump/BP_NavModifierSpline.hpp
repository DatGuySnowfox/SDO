#ifndef UE4SS_SDK_BP_NavModifierSpline_HPP
#define UE4SS_SDK_BP_NavModifierSpline_HPP

class ABP_NavModifierSpline_C : public ABP_MasterObject_C
{
    class USplineComponent* Spline;                                                   // 0x02A0 (size: 0x8)
    FVector2D Start Scale;                                                            // 0x02A8 (size: 0x10)
    FVector2D End Scale;                                                              // 0x02B8 (size: 0x10)
    class USplineMeshComponent* SplineComp;                                           // 0x02C8 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x2D0

#endif
