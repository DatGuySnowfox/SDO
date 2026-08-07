#ifndef UE4SS_SDK_BP_CustomDialogueOption_HPP
#define UE4SS_SDK_BP_CustomDialogueOption_HPP

class UBP_CustomDialogueOption_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* PopIn;                                                    // 0x02C8 (size: 0x8)
    class UButton* ReplyButton;                                                       // 0x02D0 (size: 0x8)
    class URichTextBlock* ReplyTxt;                                                   // 0x02D8 (size: 0x8)
    class UTextBlock* SelectorIcon;                                                   // 0x02E0 (size: 0x8)
    class UDialogueNode_Player* PlayerReply;                                          // 0x02E8 (size: 0x8)
    class UBP_CustomNarrativeDefaultUI_C* OwningWidget;                               // 0x02F0 (size: 0x8)
    FLinearColor AlreadySelectedColor;                                                // 0x02F8 (size: 0x10)

    void SetSelection(bool Selected);
    void BndEvt__BP_DialogueOption_ReplyButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Select Reply();
    void Simulate Clicked();
    void BndEvt__BP_DialogueOption_ReplyButton_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__BP_DialogueOption_ReplyButton_K2Node_ComponentBoundEvent_2_OnButtonHoverEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_BP_CustomDialogueOption(int32 EntryPoint);
}; // Size: 0x308

#endif
