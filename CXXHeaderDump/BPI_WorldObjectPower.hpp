#ifndef UE4SS_SDK_BPI_WorldObjectPower_HPP
#define UE4SS_SDK_BPI_WorldObjectPower_HPP

class IBPI_WorldObjectPower_C : public IInterface
{

    void CallInteractOnObject();
    void SetPower_Warning(bool SetWarning);
    void SetPower_TurnedOn(bool TurnOn);
}; // Size: 0x28

#endif
