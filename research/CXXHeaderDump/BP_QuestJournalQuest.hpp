#ifndef UE4SS_SDK_BP_QuestJournalQuest_HPP
#define UE4SS_SDK_BP_QuestJournalQuest_HPP

class UBP_QuestJournalQuest_C : public UWBP_NarrativeButton_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x15B8 (size: 0x8)
    class UButton* Button_Quest;                                                      // 0x15C0 (size: 0x8)
    class UTextBlock* QuestText;                                                      // 0x15C8 (size: 0x8)
    class UQuest* Quest;                                                              // 0x15D0 (size: 0x8)
    class UW_NarrativeMenu_QuestJournal_C* QuestJournalWidget;                        // 0x15D8 (size: 0x8)
    FSlateColor SelectedColor;                                                        // 0x15E0 (size: 0x14)
    FSlateColor UnselectedColor;                                                      // 0x15F4 (size: 0x14)

    void BndEvt__BP_QuestJournalQuest_Button_Quest_K2Node_ComponentBoundEvent_0_OnButtonPressedEvent__DelegateSignature();
    void Construct();
    void Set Selected(bool Selected);
    void ExecuteUbergraph_BP_QuestJournalQuest(int32 EntryPoint);
}; // Size: 0x1608

#endif
