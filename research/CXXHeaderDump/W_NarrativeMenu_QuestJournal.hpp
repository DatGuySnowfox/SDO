#ifndef UE4SS_SDK_W_NarrativeMenu_QuestJournal_HPP
#define UE4SS_SDK_W_NarrativeMenu_QuestJournal_HPP

class UW_NarrativeMenu_QuestJournal_C : public UWBP_NarrativeMenu_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0448 (size: 0x8)
    class UScrollBox* ActiveQuestsBox;                                                // 0x0450 (size: 0x8)
    class UImage* BG_Objectives;                                                      // 0x0458 (size: 0x8)
    class UScrollBox* BranchesBox;                                                    // 0x0460 (size: 0x8)
    class UButton* Button_Exit;                                                       // 0x0468 (size: 0x8)
    class UCommonTextBlock* CommonText_Header;                                        // 0x0470 (size: 0x8)
    class UScrollBox* FinishedQuestsBox;                                              // 0x0478 (size: 0x8)
    class UImage* Image_Exit;                                                         // 0x0480 (size: 0x8)
    class UTextBlock* QuestTitle;                                                     // 0x0488 (size: 0x8)
    class URichTextBlock* RichText_CurrentStateDescription;                           // 0x0490 (size: 0x8)
    class URichTextBlock* RichText_QuestDescription;                                  // 0x0498 (size: 0x8)
    class UTextBlock* Text_ActiveQuestCount;                                          // 0x04A0 (size: 0x8)
    class UTextBlock* Text_FinishedQuestCount;                                        // 0x04A8 (size: 0x8)
    class UCheckBox* UseShared?;                                                      // 0x04B0 (size: 0x8)
    class UNarrativeComponent* NarrativeComp;                                         // 0x04B8 (size: 0x8)
    class UQuest* CurrentQuest;                                                       // 0x04C0 (size: 0x8)
    TArray<class UQuestState*> Reached States;                                        // 0x04C8 (size: 0x10)
    TArray<class UBP_QuestJournalQuest_C*> AllJournalButtons;                         // 0x04D8 (size: 0x10)
    FDataTableRowHandle TogglePartyQuestsAction;                                      // 0x04E8 (size: 0x10)
    FInputActionBindingHandle TogglePartyQuestsActionHandle;                          // 0x04F8 (size: 0x4)

    class UBP_QuestJournalQuest_C* CreateQuestWidgetButton(class UQuest* Quest);
    void OnTogglePartyQuestsAction(FName ActionName);
    void RegisterActions();
    class UWidget* BP_GetDesiredFocusTarget();
    void Initialize(class UNarrativeComponent* Narrative);
    void Construct();
    void Show Quest(class UQuest* Quest, class UBP_QuestJournalQuest_C* JournalButton);
    void BndEvt__BP_QuestJournal_UseShared?_K2Node_ComponentBoundEvent_1_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void Clear Quest();
    void BndEvt__BP_QuestJournal_Button_Exit_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void Party Quests Toggled(bool PartyQuests);
    void ExecuteUbergraph_W_NarrativeMenu_QuestJournal(int32 EntryPoint);
}; // Size: 0x4FC

#endif
