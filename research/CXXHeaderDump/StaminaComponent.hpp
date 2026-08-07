#ifndef UE4SS_SDK_StaminaComponent_HPP
#define UE4SS_SDK_StaminaComponent_HPP

class UStaminaComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double MaxStamina;                                                                // 0x00C0 (size: 0x8)
    double CurrentStamina;                                                            // 0x00C8 (size: 0x8)
    bool CurrentlySprinting;                                                          // 0x00D0 (size: 0x1)
    double SprintSpeed;                                                               // 0x00D8 (size: 0x8)
    double WalkSpeed;                                                                 // 0x00E0 (size: 0x8)
    FTimerHandle StaminaSprintTimer;                                                  // 0x00E8 (size: 0x8)
    double StaminaDrain;                                                              // 0x00F0 (size: 0x8)
    FTimerHandle RecoverTimer;                                                        // 0x00F8 (size: 0x8)
    double StaminaRecoverAmount;                                                      // 0x0100 (size: 0x8)
    FStaminaComponent_CNoStamina NoStamina;                                           // 0x0108 (size: 0x10)
    void NoStamina();
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x0118 (size: 0x8)

    void Increase Stamina(double Amount);
    void ReduceStamina(double StaminaDrain);
    void f_CheckStamina();
    void f_SprintOnOff(bool State);
    void ComponentLoaded();
    void ComponentPreSave();
    void ToggleSprint(bool State);
    void Server_ToggleSprint(bool State);
    void RecoverStamina();
    void LoadComponent();
    void ExecuteUbergraph_StaminaComponent(int32 EntryPoint);
    void NoStamina__DelegateSignature();
}; // Size: 0x120

#endif
