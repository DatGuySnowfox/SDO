#ifndef UE4SS_SDK_BP_QuestJournalQuest_HPP
#define UE4SS_SDK_BP_QuestJournalQuest_HPP

class UBP_QuestJournalQuest_C : public UWBP_NarrativeButton_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x1560 (size: 0x8)
    class UTextBlock* QuestText;                                                      // 0x1568 (size: 0x8)
    class UButton* Button_Quest;                                                      // 0x1570 (size: 0x8)
    class UQuest* Quest;                                                              // 0x1578 (size: 0x8)
    class UW_NarrativeMenu_QuestJournal_C* QuestJournalWidget;                        // 0x1580 (size: 0x8)
    FSlateColor SelectedColor;                                                        // 0x1588 (size: 0x14)
    FSlateColor UnselectedColor;                                                      // 0x159C (size: 0x14)

    void BndEvt__BP_QuestJournalQuest_Button_Quest_K2Node_ComponentBoundEvent_0_OnButtonPressedEvent__DelegateSignature();
    void Construct();
    void Set Selected(bool Selected);
    void ExecuteUbergraph_BP_QuestJournalQuest(int32 EntryPoint);
}; // Size: 0x15B0

#endif
