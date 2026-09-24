#ifndef UE4SS_SDK_W_NarrativeMenu_QuestJournal_HPP
#define UE4SS_SDK_W_NarrativeMenu_QuestJournal_HPP

class UW_NarrativeMenu_QuestJournal_C : public UWBP_NarrativeMenu_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x04E8 (size: 0x8)
    class UCheckBox* UseShared?;                                                      // 0x04F0 (size: 0x8)
    class UCommonRichTextBlock* RichText_QuestDescription;                            // 0x04F8 (size: 0x8)
    class UCommonRichTextBlock* RichText_CurrentStateDescription;                     // 0x0500 (size: 0x8)
    class UCommonTextBlock* QuestTitle;                                               // 0x0508 (size: 0x8)
    class UCommonTextBlock* ObjectivesText;                                           // 0x0510 (size: 0x8)
    class UWBP_NarrativeButton_C* NB_Track;                                           // 0x0518 (size: 0x8)
    class UImage* Image_Exit;                                                         // 0x0520 (size: 0x8)
    class UScrollBox* FinishedQuestsBox;                                              // 0x0528 (size: 0x8)
    class UCommonTextBlock* FinishedQuestCount;                                       // 0x0530 (size: 0x8)
    class UCommonTextBlock* CommonText_Header;                                        // 0x0538 (size: 0x8)
    class UButton* Button_Exit;                                                       // 0x0540 (size: 0x8)
    class UScrollBox* BranchesBox;                                                    // 0x0548 (size: 0x8)
    class UImage* BG_Objectives;                                                      // 0x0550 (size: 0x8)
    class UCommonTextBlock* ActiveQuestsCount;                                        // 0x0558 (size: 0x8)
    class UScrollBox* ActiveQuestsBox;                                                // 0x0560 (size: 0x8)
    class UNarrativeComponent* NarrativeComp;                                         // 0x0568 (size: 0x8)
    class UQuest* CurrentQuest;                                                       // 0x0570 (size: 0x8)
    TArray<UQuestState*> Reached States;                                              // 0x0578 (size: 0x10)
    TArray<UBP_QuestJournalQuest_C*> AllJournalButtons;                               // 0x0588 (size: 0x10)
    FDataTableRowHandle TogglePartyQuestsAction;                                      // 0x0598 (size: 0x10)
    FInputActionBindingHandle TogglePartyQuestsActionHandle;                          // 0x05A8 (size: 0x4)

    ESlateVisibility Get_NB_Track_Visibility();
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
    void BndEvt__W_NarrativeMenu_QuestJournal_NB_Track_K2Node_ComponentBoundEvent_0_CommonButtonBaseClicked__DelegateSignature(class UCommonButtonBase* Button);
    void ExecuteUbergraph_W_NarrativeMenu_QuestJournal(int32 EntryPoint);
}; // Size: 0x5AC

#endif
