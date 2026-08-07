#ifndef UE4SS_SDK_SettingsMenu_HPP
#define UE4SS_SDK_SettingsMenu_HPP

class USettingsMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UAudioSettingsPage_C* AudioSettingsPage;                                    // 0x02C8 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_Back;                                         // 0x02D0 (size: 0x8)
    class UVerticalRadioSelect_C* CategorySelect;                                     // 0x02D8 (size: 0x8)
    class UWidgetSwitcher* CategorySwitcher;                                          // 0x02E0 (size: 0x8)
    class UGameplaySettingsPage_C* GameplaySettingsPage;                              // 0x02E8 (size: 0x8)
    class UInputSettingsPage_C* InputSettingsPageNew;                                 // 0x02F0 (size: 0x8)
    class UVideoSettingsPage_C* VideoSettingsPage;                                    // 0x02F8 (size: 0x8)
    FSettingsMenu_CReturnToMenu ReturnToMenu;                                         // 0x0300 (size: 0x10)
    void ReturnToMenu();

    void CategoryChanged(FString Value);
    void Construct();
    void BndEvt__SettingsMenu_ButtonWidget_Back_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void ExecuteUbergraph_SettingsMenu(int32 EntryPoint);
    void ReturnToMenu__DelegateSignature();
}; // Size: 0x310

#endif
