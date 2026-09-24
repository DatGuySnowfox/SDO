#ifndef UE4SS_SDK_SettingsMenu_HPP
#define UE4SS_SDK_SettingsMenu_HPP

class USettingsMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVideoSettingsPage_C* VideoSettingsPage;                                    // 0x0348 (size: 0x8)
    class UInputSettingsPage_C* InputSettingsPageNew;                                 // 0x0350 (size: 0x8)
    class UGameplaySettingsPage_C* GameplaySettingsPage;                              // 0x0358 (size: 0x8)
    class UWidgetSwitcher* CategorySwitcher;                                          // 0x0360 (size: 0x8)
    class UVerticalRadioSelect_C* CategorySelect;                                     // 0x0368 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_Back;                                         // 0x0370 (size: 0x8)
    class UAudioSettingsPage_C* AudioSettingsPage;                                    // 0x0378 (size: 0x8)
    FSettingsMenu_CReturnToMenu ReturnToMenu;                                         // 0x0380 (size: 0x10)
    void ReturnToMenu();

    void CategoryChanged(FString Value);
    void Construct();
    void BndEvt__SettingsMenu_ButtonWidget_Back_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void ExecuteUbergraph_SettingsMenu(int32 EntryPoint);
    void ReturnToMenu__DelegateSignature();
}; // Size: 0x390

#endif
