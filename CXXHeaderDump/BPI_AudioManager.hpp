#ifndef UE4SS_SDK_BPI_AudioManager_HPP
#define UE4SS_SDK_BPI_AudioManager_HPP

class IBPI_AudioManager_C : public IInterface
{

    void UpdateCombatStatus(TEnumAsByte<E_Combat::Type> CombatStatus);
    void SetInside(bool bInside, double AmbientMultiplier, FReverbSettings ReverbSettings, FName ReverbAreaName, double ReverbPriority);
    void UpdateTimeOfDay(double Time, TEnumAsByte<E_TimeOfDay::Type> Night?);
}; // Size: 0x28

#endif
