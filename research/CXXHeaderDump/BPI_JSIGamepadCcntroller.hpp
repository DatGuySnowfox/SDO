#ifndef UE4SS_SDK_BPI_JSIGamepadCcntroller_HPP
#define UE4SS_SDK_BPI_JSIGamepadCcntroller_HPP

class IBPI_JSIGamepadCcntroller_C : public IInterface
{

    void GetControllerAxis(FVector2D& Axis);
}; // Size: 0x28

#endif
