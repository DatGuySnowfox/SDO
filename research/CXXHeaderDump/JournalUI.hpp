#ifndef UE4SS_SDK_JournalUI_HPP
#define UE4SS_SDK_JournalUI_HPP

class UJournalUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* SkillsText;                                                     // 0x0348 (size: 0x8)
    class UButton* SkillsButton;                                                      // 0x0350 (size: 0x8)
    class UTextBlock* QuestsText;                                                     // 0x0358 (size: 0x8)
    class UButton* QuestsButton;                                                      // 0x0360 (size: 0x8)
    class UJournal_SkillsUI_C* Journal_SkillsUI;                                      // 0x0368 (size: 0x8)
    class UJournal_QuestsUI_C* Journal_QuestsUI;                                      // 0x0370 (size: 0x8)
    class UJournal_InfoUI_C* Journal_InfoUI;                                          // 0x0378 (size: 0x8)
    class UTextBlock* InfoText;                                                       // 0x0380 (size: 0x8)
    class UButton* InfoButton;                                                        // 0x0388 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0390 (size: 0x8)
    FJournalUI_CJournalOpened JournalOpened;                                          // 0x0398 (size: 0x10)
    void JournalOpened();

    void BndEvt__InfoSkillQuestUI_Button_56_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__InfoSkillQuestUI_Button_56_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature();
    void OnInitialized();
    void BndEvt__JournalUI_Test_SkillsButton_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_QuestsButton_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_SkillsButton_K2Node_ComponentBoundEvent_8_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_QuestsButton_K2Node_ComponentBoundEvent_9_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_InfoButton_K2Node_ComponentBoundEvent_10_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_SkillsButton_K2Node_ComponentBoundEvent_11_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__JournalUI_Test_QuestsButton_K2Node_ComponentBoundEvent_12_OnButtonHoverEvent__DelegateSignature();
    void Begin();
    void ExecuteUbergraph_JournalUI(int32 EntryPoint);
    void JournalOpened__DelegateSignature();
}; // Size: 0x3A8

#endif
