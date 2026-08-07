#ifndef UE4SS_SDK_PauseScreen_HPP
#define UE4SS_SDK_PauseScreen_HPP

class UPauseScreen_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_BackFromDesktop;                              // 0x02C8 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_BackFromMenu;                                 // 0x02D0 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_BackFromSuicide;                              // 0x02D8 (size: 0x8)
    class UButton* DifficultySettings;                                                // 0x02E0 (size: 0x8)
    class UDifficultySettingsMenu_C* DifficultySettingsMenu;                          // 0x02E8 (size: 0x8)
    class UButton* MainMenu;                                                          // 0x02F0 (size: 0x8)
    class UOverlay* PauseWidget;                                                      // 0x02F8 (size: 0x8)
    class UButton* QuickSave;                                                         // 0x0300 (size: 0x8)
    class UButton* Quit;                                                              // 0x0308 (size: 0x8)
    class UButtonWidget_C* QuitToDesktopNoButton;                                     // 0x0310 (size: 0x8)
    class UCanvasPanel* QuitToDesktopWidget;                                          // 0x0318 (size: 0x8)
    class UButtonWidget_C* QuitToDesktopYesButton;                                    // 0x0320 (size: 0x8)
    class UButtonWidget_C* QuitToMainMenuNoButton;                                    // 0x0328 (size: 0x8)
    class UCanvasPanel* QuitToMainMenuWidget;                                         // 0x0330 (size: 0x8)
    class UButtonWidget_C* QuitToMainMenuYesButton;                                   // 0x0338 (size: 0x8)
    class UButton* Resume;                                                            // 0x0340 (size: 0x8)
    class UButton* Save;                                                              // 0x0348 (size: 0x8)
    class UButton* Settings;                                                          // 0x0350 (size: 0x8)
    class USettingsMenu_C* SettingsMenu;                                              // 0x0358 (size: 0x8)
    class UButton* Suicide;                                                           // 0x0360 (size: 0x8)
    class UButtonWidget_C* SuicideNoButton;                                           // 0x0368 (size: 0x8)
    class UCanvasPanel* SuicideWidget;                                                // 0x0370 (size: 0x8)
    class UButtonWidget_C* SuicideYesButton;                                          // 0x0378 (size: 0x8)

    void ReturnToMenu();
    void SavePressed();
    void BindEvents();
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    void EscapePressed(FKey Key);
    void RemoveSaveAndSuicideButton();
    void BndEvt__PauseScreen_QuitToMainMenuYesButton_K2Node_ComponentBoundEvent_5_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_QuitToMainMenuNoButton_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_QuitToDesktopYesButton_K2Node_ComponentBoundEvent_1_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_QuitToDesktopNoButton_K2Node_ComponentBoundEvent_2_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_ButtonWidget_BackFromMenu_K2Node_ComponentBoundEvent_4_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_ButtonWidget_BackFromDesktop_K2Node_ComponentBoundEvent_6_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_ButtonWidget_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_SuicideNoButton_K2Node_ComponentBoundEvent_1_ButtonPressed__DelegateSignature();
    void BndEvt__PauseScreen_ButtonWidget_BackFromSuicide_K2Node_ComponentBoundEvent_3_ButtonPressed__DelegateSignature();
    void BndEvt__ResumeButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__MainMenuButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__QuitButton_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Settings_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__PauseScreen_Suicide_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PauseScreen_Save_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BindReturnToPauseFromSave();
    void BndEvt__PauseScreen_DifficultySettings_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_PauseScreen(int32 EntryPoint);
}; // Size: 0x380

#endif
