#ifndef UE4SS_SDK_IRadialInput_HPP
#define UE4SS_SDK_IRadialInput_HPP

class IIRadialInput_C : public IInterface
{

    void GetJoystickDirection(TEnumAsByte<EJoystickTypes::Type> Stick, FVector2D& StickInput);
}; // Size: 0x28

#endif
