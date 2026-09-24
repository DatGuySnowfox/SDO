#ifndef UE4SS_SDK_BP_InfestationSpline_HPP
#define UE4SS_SDK_BP_InfestationSpline_HPP

class ABP_InfestationSpline_C : public ABP_MasterObject_C
{
    class USplineComponent* Spline;                                                   // 0x02B0 (size: 0x8)
    class USplineMeshComponent* SplineMesh;                                           // 0x02B8 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x2C0

#endif
