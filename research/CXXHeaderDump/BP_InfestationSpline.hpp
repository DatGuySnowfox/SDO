#ifndef UE4SS_SDK_BP_InfestationSpline_HPP
#define UE4SS_SDK_BP_InfestationSpline_HPP

class ABP_InfestationSpline_C : public ABP_MasterObject_C
{
    class USplineComponent* Spline;                                                   // 0x02A0 (size: 0x8)
    class USplineMeshComponent* SplineMesh;                                           // 0x02A8 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x2B0

#endif
