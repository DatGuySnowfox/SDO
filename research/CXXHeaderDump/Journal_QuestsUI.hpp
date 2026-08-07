#ifndef UE4SS_SDK_Journal_QuestsUI_HPP
#define UE4SS_SDK_Journal_QuestsUI_HPP

class UJournal_QuestsUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* ActiveButton;                                                      // 0x02C8 (size: 0x8)
    class UImage* ActiveQuests;                                                       // 0x02D0 (size: 0x8)
    class UScrollBox* ActiveQuestsBox;                                                // 0x02D8 (size: 0x8)
    class UTextBlock* ActiveText;                                                     // 0x02E0 (size: 0x8)
    class UScrollBox* BranchesBox;                                                    // 0x02E8 (size: 0x8)
    class UButton* Button_ForgetQuest;                                                // 0x02F0 (size: 0x8)
    class UButton* CompletedButton;                                                   // 0x02F8 (size: 0x8)
    class UImage* CompletedQuests;                                                    // 0x0300 (size: 0x8)
    class UScrollBox* CompletedQuestsBox;                                             // 0x0308 (size: 0x8)
    class UTextBlock* CompletedText;                                                  // 0x0310 (size: 0x8)
    class UImage* Page;                                                               // 0x0318 (size: 0x8)
    class UTextBlock* QuestDescription;                                               // 0x0320 (size: 0x8)
    class UTextBlock* QuestTitle;                                                     // 0x0328 (size: 0x8)
    class UImage* SaveEnabledIcon;                                                    // 0x0330 (size: 0x8)
    class UNarrativeComponent* NarrativeComp;                                         // 0x0338 (size: 0x8)
    class UQuest* CurrentQuest;                                                       // 0x0340 (size: 0x8)
    TArray<class UQuestState*> Reached States;                                        // 0x0348 (size: 0x10)
    class UTooltip_PassiveStats_C* Tooltip;                                           // 0x0358 (size: 0x8)

    class UWidget* Get_SaveEnabledIcon_ToolTipWidget();
    FText GetAllCompletedQuests();
    FText GetAllActiveQuests();
    void Initialize(class UNarrativeComponent* Narrative);
    void Show Quest(class UQuest* Quest);
    void Clear Quest();
    void BndEvt__Journal_QuestsUI_Button_Quest_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Initialize_Completed();
    void BndEvt__Journal_QuestsUI_ActiveButton_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Journal_QuestsUI_ActiveButton_K2Node_ComponentBoundEvent_4_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Journal_QuestsUI_CompletedButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Journal_QuestsUI_CompletedButton_K2Node_ComponentBoundEvent_6_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__Journal_QuestsUI_ActiveButton_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Journal_QuestsUI_CompletedButton_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void ForgetQuest();
    void Construct();
    void ExecuteUbergraph_Journal_QuestsUI(int32 EntryPoint);
}; // Size: 0x360

#endif
