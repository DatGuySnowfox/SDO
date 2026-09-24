#ifndef UE4SS_SDK_HungerThirstComponent_HPP
#define UE4SS_SDK_HungerThirstComponent_HPP

class UHungerThirstComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    double CurrentHunger;                                                             // 0x00D8 (size: 0x8)
    double MaxHunger;                                                                 // 0x00E0 (size: 0x8)
    double CurrentThirst;                                                             // 0x00E8 (size: 0x8)
    double MaxThirst;                                                                 // 0x00F0 (size: 0x8)
    FTimerHandle HungerHandle;                                                        // 0x00F8 (size: 0x8)
    FTimerHandle ThirstHandle;                                                        // 0x0100 (size: 0x8)
    FTimerHandle HungerDamageHandle;                                                  // 0x0108 (size: 0x8)
    FTimerHandle ThirstDamageHandle;                                                  // 0x0110 (size: 0x8)
    bool HungerDebuff;                                                                // 0x0118 (size: 0x1)
    bool ThirstDebuff;                                                                // 0x0119 (size: 0x1)

    void ClearTimers();
    void Increase Hunger(double Amount);
    void IncreaseThirst(double Amount);
    void HungerDamageTimer();
    void ThirstDamageTimer();
    void ThirstTimer();
    void HungerTimer();
    void ComponentLoaded();
    void ComponentPreLoad();
    void ComponentPreSave();
    void ComponentSaved();
    void UpdateHunger();
    void UpdateThirst();
    void LoadComponent();
    void ExecuteUbergraph_HungerThirstComponent(int32 EntryPoint);
}; // Size: 0x11A

#endif
