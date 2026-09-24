#ifndef UE4SS_SDK_StaminaComponent_HPP
#define UE4SS_SDK_StaminaComponent_HPP

class UStaminaComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    double CurrentStamina;                                                            // 0x00D8 (size: 0x8)
    double MaxStamina;                                                                // 0x00E0 (size: 0x8)
    double WalkSpeed;                                                                 // 0x00E8 (size: 0x8)
    double SprintSpeed;                                                               // 0x00F0 (size: 0x8)
    bool CurrentlySprinting;                                                          // 0x00F8 (size: 0x1)
    FTimerHandle RecoverTimer;                                                        // 0x0100 (size: 0x8)
    FTimerHandle StaminaSprintTimer;                                                  // 0x0108 (size: 0x8)

    void ClearTimers();
    void ReduceStaminaDuringSprinting();
    void IncreaseStamina(double Amount);
    void ReduceStamina(double StaminaDrain);
    void SetSprintState(bool State);
    void ComponentLoaded();
    void ComponentPreLoad();
    void ComponentPreSave();
    void ComponentSaved();
    void ToggleSprint(bool State);
    void Server_ToggleSprint(bool State);
    void RecoverStamina();
    void LoadComponent();
    void ExecuteUbergraph_StaminaComponent(int32 EntryPoint);
}; // Size: 0x110

#endif
