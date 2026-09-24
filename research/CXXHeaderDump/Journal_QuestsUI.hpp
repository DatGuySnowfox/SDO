#ifndef UE4SS_SDK_Journal_QuestsUI_HPP
#define UE4SS_SDK_Journal_QuestsUI_HPP

class UJournal_QuestsUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* QuestTitle;                                                     // 0x0348 (size: 0x8)
    class UTextBlock* QuestDescription;                                               // 0x0350 (size: 0x8)
    class UImage* Page;                                                               // 0x0358 (size: 0x8)
    class UTextBlock* CompletedText;                                                  // 0x0360 (size: 0x8)
    class UScrollBox* CompletedQuestsBox;                                             // 0x0368 (size: 0x8)
    class UImage* CompletedQuests;                                                    // 0x0370 (size: 0x8)
    class UButton* CompletedButton;                                                   // 0x0378 (size: 0x8)
    class UButton* Button_ForgetQuest;                                                // 0x0380 (size: 0x8)
    class UScrollBox* BranchesBox;                                                    // 0x0388 (size: 0x8)
    class UTextBlock* ActiveText;                                                     // 0x0390 (size: 0x8)
    class UScrollBox* ActiveQuestsBox;                                                // 0x0398 (size: 0x8)
    class UImage* ActiveQuests;                                                       // 0x03A0 (size: 0x8)
    class UButton* ActiveButton;                                                      // 0x03A8 (size: 0x8)
    class UNarrativeComponent* NarrativeComp;                                         // 0x03B0 (size: 0x8)
    class UQuest* CurrentQuest;                                                       // 0x03B8 (size: 0x8)
    TArray<UQuestState*> Reached States;                                              // 0x03C0 (size: 0x10)
    class UTooltip_PassiveStats_C* Tooltip;                                           // 0x03D0 (size: 0x8)

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
}; // Size: 0x3D8

#endif
