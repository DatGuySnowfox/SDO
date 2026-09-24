#ifndef UE4SS_SDK_DifficultySettingsMenu_HPP
#define UE4SS_SDK_DifficultySettingsMenu_HPP

class UDifficultySettingsMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UDifficultySettingsPage_C* DifficultySettingsPage;                          // 0x0348 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_Back;                                         // 0x0350 (size: 0x8)
    FDifficultySettingsMenu_CReturnToMenu ReturnToMenu;                               // 0x0358 (size: 0x10)
    void ReturnToMenu();

    void BndEvt__SettingsMenu_ButtonWidget_Back_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void ExecuteUbergraph_DifficultySettingsMenu(int32 EntryPoint);
    void ReturnToMenu__DelegateSignature();
}; // Size: 0x368

#endif
