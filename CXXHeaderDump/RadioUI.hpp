#ifndef UE4SS_SDK_RadioUI_HPP
#define UE4SS_SDK_RadioUI_HPP

class URadioUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_430;                                                        // 0x02C8 (size: 0x8)
    class UButton* ButtonVol_High;                                                    // 0x02D0 (size: 0x8)
    class UButton* ButtonVol_Low;                                                     // 0x02D8 (size: 0x8)
    class UButton* ButtonVol_Med;                                                     // 0x02E0 (size: 0x8)
    class UButton* ButtonVol_Mute;                                                    // 0x02E8 (size: 0x8)
    class UComboBoxString* ComboBoxString_165;                                        // 0x02F0 (size: 0x8)
    class UTextBlock* CurrentSongText;                                                // 0x02F8 (size: 0x8)
    class UButton* NextButton;                                                        // 0x0300 (size: 0x8)
    class UButton* PauseButton;                                                       // 0x0308 (size: 0x8)
    class UImage* PauseImg;                                                           // 0x0310 (size: 0x8)
    class UTextBlock* PowerText;                                                      // 0x0318 (size: 0x8)
    class UButton* PreviousButton;                                                    // 0x0320 (size: 0x8)
    class UTextBlock* TextBlock_52;                                                   // 0x0328 (size: 0x8)
    class UButton* TurnOnButton;                                                      // 0x0330 (size: 0x8)
    FRadioUI_CCorrectCodeEntered CorrectCodeEntered;                                  // 0x0338 (size: 0x10)
    void CorrectCodeEntered();
    FRadioUI_CIncorrectCodeEntered IncorrectCodeEntered;                              // 0x0348 (size: 0x10)
    void IncorrectCodeEntered();
    FString CurrentSong;                                                              // 0x0358 (size: 0x10)
    class AActor* Owner;                                                              // 0x0368 (size: 0x8)
    class URadioComponent_C* RadioComp;                                               // 0x0370 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x0378 (size: 0x8)
    FString ObjectName;                                                               // 0x0380 (size: 0x10)

    FSlateBrush Get_PauseImg_Brush();
    FText Get_PowerText_Text();
    void NextSong();
    void SetCurrentSong(FString CurrentSong);
    void BndEvt__RadioUI_TurnOnButton_K2Node_ComponentBoundEvent_11_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__RadioUI_Button_430_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_ButtonVol_Mute_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_ButtonVol_Low_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_ButtonVol_Med_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_ButtonVol_High_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void EndReached();
    void BindEvents();
    void BndEvt__RadioUI_ComboBoxString_165_K2Node_ComponentBoundEvent_6_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__RadioUI_NextButton_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_PreviousButton_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RadioUI_PauseButton_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_RadioUI(int32 EntryPoint);
    void IncorrectCodeEntered__DelegateSignature();
    void CorrectCodeEntered__DelegateSignature();
}; // Size: 0x390

#endif
