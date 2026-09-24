#ifndef UE4SS_SDK_Difficulty_Other_HPP
#define UE4SS_SDK_Difficulty_Other_HPP

class UDifficulty_Other_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* StandardText;                                                   // 0x0348 (size: 0x8)
    class UButton* StandardButton;                                                    // 0x0350 (size: 0x8)
    class USettingRow_C* SettingRow_425;                                              // 0x0358 (size: 0x8)
    class UTextBlock* PeacefulText;                                                   // 0x0360 (size: 0x8)
    class UButton* PeacefulButton;                                                    // 0x0368 (size: 0x8)
    class UTextBlock* NightmareText;                                                  // 0x0370 (size: 0x8)
    class UButton* NightmareButton;                                                   // 0x0378 (size: 0x8)
    class UTextBlock* HardenedText;                                                   // 0x0380 (size: 0x8)
    class UButton* HardenedButton;                                                    // 0x0388 (size: 0x8)
    class UTextBlock* EasyText;                                                       // 0x0390 (size: 0x8)
    class UButton* EasyButton;                                                        // 0x0398 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x03A0 (size: 0x14)

    void BndEvt__DifficultySettingsPage_PeacefulButton_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_StandardButton_K2Node_ComponentBoundEvent_10_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_HardenedButton_K2Node_ComponentBoundEvent_13_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_NightmareButton_K2Node_ComponentBoundEvent_14_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_PeacefulButton_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_PeacefulButton_K2Node_ComponentBoundEvent_2_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_StandardButton_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_StandardButton_K2Node_ComponentBoundEvent_4_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_HardenedButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_HardenedButton_K2Node_ComponentBoundEvent_6_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_NightmareButton_K2Node_ComponentBoundEvent_7_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__DifficultySettingsPage_NightmareButton_K2Node_ComponentBoundEvent_8_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Difficulty_Other_EasyButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Difficulty_Other_EasyButton_K2Node_ComponentBoundEvent_11_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Difficulty_Other_EasyButton_K2Node_ComponentBoundEvent_12_OnButtonHoverEvent__DelegateSignature();
    void ExecuteUbergraph_Difficulty_Other(int32 EntryPoint);
}; // Size: 0x3B4

#endif
