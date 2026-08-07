#ifndef UE4SS_SDK_LetterUI_HPP
#define UE4SS_SDK_LetterUI_HPP

class ULetterUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02C8 (size: 0x8)
    class UTextBlock* Description;                                                    // 0x02D0 (size: 0x8)
    class UButton* ExitButton;                                                        // 0x02D8 (size: 0x8)
    class UTextBlock* ExitText;                                                       // 0x02E0 (size: 0x8)
    class UImage* Image_38;                                                           // 0x02E8 (size: 0x8)

    void BndEvt__LetterUI_Button_191_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__LetterUI_ExitButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void SetText(FText Description);
    void Construct();
    void Exit();
    void BndEvt__LetterUI_ExitButton_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_LetterUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif
