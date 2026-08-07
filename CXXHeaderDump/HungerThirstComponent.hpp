#ifndef UE4SS_SDK_HungerThirstComponent_HPP
#define UE4SS_SDK_HungerThirstComponent_HPP

class UHungerThirstComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double MaxHunger;                                                                 // 0x00C0 (size: 0x8)
    double CurrentHunger;                                                             // 0x00C8 (size: 0x8)
    double MaxThirst;                                                                 // 0x00D0 (size: 0x8)
    double CurrentThirst;                                                             // 0x00D8 (size: 0x8)
    FTimerHandle HungerHandle;                                                        // 0x00E0 (size: 0x8)
    double ReduceHungerAmount;                                                        // 0x00E8 (size: 0x8)
    double ReduceThirstAmount;                                                        // 0x00F0 (size: 0x8)
    FTimerHandle ThirstHandle;                                                        // 0x00F8 (size: 0x8)
    FTimerHandle ThirstDamageHandle;                                                  // 0x0100 (size: 0x8)
    FTimerHandle HungerDamageHandle;                                                  // 0x0108 (size: 0x8)
    double HungerMultplier;                                                           // 0x0110 (size: 0x8)
    double ThirstMultiplier;                                                          // 0x0118 (size: 0x8)
    bool HungerDebuff;                                                                // 0x0120 (size: 0x1)
    bool ThirstDebuff;                                                                // 0x0121 (size: 0x1)

    void SetThirstDebuff(bool Debuff?);
    void SetHungerDebuff(bool Debuff?);
    void Increase Hunger(double Amount);
    void IncreaseThirst(double Amount);
    void ApplyHungerDamage();
    void ApplyThirstDamage();
    void ThirstTimer();
    void HungerTimer();
    void ComponentLoaded();
    void ComponentPreSave();
    void UpdateHunger();
    void UpdateThirst();
    void CheckStats();
    void LoadComponent();
    void Event_HungerDebuff(bool Add?);
    void Event_ThirstDebuff(bool Add?);
    void Event_ThirstX(float NewValue);
    void Event_HungerX(float NewValue);
    void ExecuteUbergraph_HungerThirstComponent(int32 EntryPoint);
}; // Size: 0x122

#endif
