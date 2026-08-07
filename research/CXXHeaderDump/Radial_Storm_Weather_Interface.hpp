#ifndef UE4SS_SDK_Radial_Storm_Weather_Interface_HPP
#define UE4SS_SDK_Radial_Storm_Weather_Interface_HPP

class IRadial_Storm_Weather_Interface_C : public IInterface
{

    void Is Storm in Effective Range?(FVector Location, bool& In Range);
    void Get Radial Storm Material Target Data(bool& Draw, FVector& Location, double& Outer Radius, double& Soft Fraction, FLinearColor& Color);
    void Get Radial Storm Weather Data(FVector& World Location, double& Outer Radius, double& Inner Radius, double& Alpha, class UUDS_Weather_Settings_C*& Weather State, bool& Affect Material State);
}; // Size: 0x28

#endif
