#ifndef UE4SS_SDK_Journal_QuestButtonUI_HPP
#define UE4SS_SDK_Journal_QuestButtonUI_HPP

class UJournal_QuestButtonUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* QuestText;                                                      // 0x0348 (size: 0x8)
    class UButton* Button_Quest;                                                      // 0x0350 (size: 0x8)
    class UQuest* Quest;                                                              // 0x0358 (size: 0x8)
    class UJournal_QuestsUI_C* QuestJournalWidget;                                    // 0x0360 (size: 0x8)

    void BndEvt__BP_QuestJournalQuest_Button_Quest_K2Node_ComponentBoundEvent_0_OnButtonPressedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_Journal_QuestButtonUI(int32 EntryPoint);
}; // Size: 0x368

#endif
