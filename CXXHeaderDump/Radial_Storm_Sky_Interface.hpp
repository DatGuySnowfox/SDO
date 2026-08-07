#ifndef UE4SS_SDK_Radial_Storm_Sky_Interface_HPP
#define UE4SS_SDK_Radial_Storm_Sky_Interface_HPP

class IRadial_Storm_Sky_Interface_C : public IInterface
{

    void Update with Sky Mode Change(bool& Successful);
    void Get Cloud Coverage Brush Data(class AUltra_Dynamic_Sky_C* UDS, double& Soft Fraction, FLinearColor& RGBA, FVector& World Location, double& Size, double& UDS Coverage Level);
}; // Size: 0x28

#endif
