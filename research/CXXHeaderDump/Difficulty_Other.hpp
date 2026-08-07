#ifndef UE4SS_SDK_Difficulty_Other_HPP
#define UE4SS_SDK_Difficulty_Other_HPP

class UDifficulty_Other_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* EasyButton;                                                        // 0x02C8 (size: 0x8)
    class UTextBlock* EasyText;                                                       // 0x02D0 (size: 0x8)
    class UButton* HardenedButton;                                                    // 0x02D8 (size: 0x8)
    class UTextBlock* HardenedText;                                                   // 0x02E0 (size: 0x8)
    class UButton* NightmareButton;                                                   // 0x02E8 (size: 0x8)
    class UTextBlock* NightmareText;                                                  // 0x02F0 (size: 0x8)
    class UButton* PeacefulButton;                                                    // 0x02F8 (size: 0x8)
    class UTextBlock* PeacefulText;                                                   // 0x0300 (size: 0x8)
    class USettingRow_C* SettingRow_425;                                              // 0x0308 (size: 0x8)
    class UButton* StandardButton;                                                    // 0x0310 (size: 0x8)
    class UTextBlock* StandardText;                                                   // 0x0318 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x0320 (size: 0x14)

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
}; // Size: 0x334

#endif
