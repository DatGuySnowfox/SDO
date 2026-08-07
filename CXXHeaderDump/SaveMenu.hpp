#ifndef UE4SS_SDK_SaveMenu_HPP
#define UE4SS_SDK_SaveMenu_HPP

class USaveMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_BackFromMenu;                                 // 0x02C8 (size: 0x8)
    class UButton* CheckSave_NO;                                                      // 0x02D0 (size: 0x8)
    class UButton* CheckSave_YES;                                                     // 0x02D8 (size: 0x8)
    class UCanvasPanel* CheckSaveFilePanel;                                           // 0x02E0 (size: 0x8)
    class UTextBlock* DateText;                                                       // 0x02E8 (size: 0x8)
    class UButton* Delete_NO;                                                         // 0x02F0 (size: 0x8)
    class UButton* Delete_YES;                                                        // 0x02F8 (size: 0x8)
    class UButton* DeleteSaveButton;                                                  // 0x0300 (size: 0x8)
    class UCanvasPanel* DeleteSavePanel;                                              // 0x0308 (size: 0x8)
    class UTextBlock* DeleteText_1;                                                   // 0x0310 (size: 0x8)
    class UEditableText* EditableText_All;                                            // 0x0318 (size: 0x8)
    class UEditableText* EditableText_Chars;                                          // 0x0320 (size: 0x8)
    class UEditableTextBox* EditableTextBox_135;                                      // 0x0328 (size: 0x8)
    class UImage* Image_112;                                                          // 0x0330 (size: 0x8)
    class UButton* Load_NO;                                                           // 0x0338 (size: 0x8)
    class UButton* Load_YES;                                                          // 0x0340 (size: 0x8)
    class UButton* LoadGameButton;                                                    // 0x0348 (size: 0x8)
    class UCanvasPanel* LoadGamePanel;                                                // 0x0350 (size: 0x8)
    class UTextBlock* LoadText;                                                       // 0x0358 (size: 0x8)
    class UCanvasPanel* MenuCanvasPanel;                                              // 0x0360 (size: 0x8)
    class UTextBlock* NameText;                                                       // 0x0368 (size: 0x8)
    class UVerticalBox* NewGameBox;                                                   // 0x0370 (size: 0x8)
    class UTextBlock* NoText;                                                         // 0x0378 (size: 0x8)
    class UTextBlock* NoText_1;                                                       // 0x0380 (size: 0x8)
    class UTextBlock* NoText_2;                                                       // 0x0388 (size: 0x8)
    class UTextBlock* NoText_3;                                                       // 0x0390 (size: 0x8)
    class UButton* Save_NO;                                                           // 0x0398 (size: 0x8)
    class UButton* Save_YES;                                                          // 0x03A0 (size: 0x8)
    class UButton* SaveGameButton;                                                    // 0x03A8 (size: 0x8)
    class UScrollBox* SaveGameList;                                                   // 0x03B0 (size: 0x8)
    class UCanvasPanel* SaveGamePanel;                                                // 0x03B8 (size: 0x8)
    class UTextBlock* SaveText;                                                       // 0x03C0 (size: 0x8)
    class UButton* SetCurrentButton;                                                  // 0x03C8 (size: 0x8)
    class USettingRow_C* SettingRow;                                                  // 0x03D0 (size: 0x8)
    class USettingRow_C* SettingRow_1;                                                // 0x03D8 (size: 0x8)
    class USettingRow_C* SettingRow_12;                                               // 0x03E0 (size: 0x8)
    class UTextBlock* SurvivedDaysText;                                               // 0x03E8 (size: 0x8)
    class URichTextBlock* TextBlockCheckSave;                                         // 0x03F0 (size: 0x8)
    class URichTextBlock* TextBlockDelete;                                            // 0x03F8 (size: 0x8)
    class URichTextBlock* TextBlockLoad;                                              // 0x0400 (size: 0x8)
    class URichTextBlock* TextBlockSave;                                              // 0x0408 (size: 0x8)
    class UTextBlock* YesText;                                                        // 0x0410 (size: 0x8)
    class UTextBlock* YesText_1;                                                      // 0x0418 (size: 0x8)
    class UTextBlock* YesText_2;                                                      // 0x0420 (size: 0x8)
    class UTextBlock* YesText_3;                                                      // 0x0428 (size: 0x8)
    class USaveGameSlot_C* CurrentSelectedSlot;                                       // 0x0430 (size: 0x8)
    bool PauseMenu?;                                                                  // 0x0438 (size: 0x1)
    FSaveMenu_CReturnToMenu ReturnToMenu;                                             // 0x0440 (size: 0x10)
    void ReturnToMenu();
    FSaveMenu_CRemoveUI RemoveUI;                                                     // 0x0450 (size: 0x10)
    void RemoveUI();
    bool PlayerDeath?;                                                                // 0x0460 (size: 0x1)
    FSaveMenu_CReturnToPauseScreen ReturnToPauseScreen;                               // 0x0468 (size: 0x10)
    void ReturnToPauseScreen();

    void SearchCharacters(FText Text);
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SearchSaves(FText Text);
    void SetCurrentSlot(class USaveGameSlot_C* CurrentSelectedSlot);
    void PauseMenuCheck();
    void Set Info(class USaveGameSlot_C* Slot);
    FText SetCurrentSaveName();
    void LoadGameFromSlot(bool PauseMenu?, FName Level);
    void SaveGameToSlot();
    void DeleteSlot();
    void SelectCurrentSlot();
    void AddAndUpdateSlotWidgets();
    void CreateNewSlotAndCheckName(FString Slot Name);
    void SetCurrentSaveSlotByName(FString SaveGameName);
    void OnFailed_4EBFD5F145FA3C71E69D3BB7D87BBC9D();
    void OnCompleted_4EBFD5F145FA3C71E69D3BB7D87BBC9D();
    void BndEvt__SaveMenu_CheckSave_YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_CheckSave_NO_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_Load_YES_K2Node_ComponentBoundEvent_2_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__SaveMenu_Load_NO_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__SaveMenu_Load_NO_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_Load_YES_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_Save_YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_Save_NO_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_Save_NO_K2Node_ComponentBoundEvent_4_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__SaveMenu_Save_YES_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__NO_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__YES_K2Node_ComponentBoundEvent_4_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__NO_K2Node_ComponentBoundEvent_6_OnButtonHoverEvent__DelegateSignature();
    void Construct();
    void CloseMenu();
    void Add Widget Slot(FString Name);
    void Make And Save New Slot ();
    void Make New Slot ();
    void SelectSaveSlot(class USaveGameSlot_C* SaveSlot);
    void BndEvt__SaveMenu_DeleteSaveButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_LoadGameButton_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_SaveGameButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_SetCurrentButton_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__SaveMenu_EditableText_All_K2Node_ComponentBoundEvent_4_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__SaveMenu_ButtonWidget_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void SetCurrentSaveSlot(class USaveGameSlot_C* CurrentSelectedSlot);
    void BndEvt__SaveMenu_EditableText_All_1_K2Node_ComponentBoundEvent_5_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void ManualCurrentSaveCallback(bool NewValue);
    void ExecuteUbergraph_SaveMenu(int32 EntryPoint);
    void ReturnToPauseScreen__DelegateSignature();
    void RemoveUI__DelegateSignature();
    void ReturnToMenu__DelegateSignature();
}; // Size: 0x478

#endif
