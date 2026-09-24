#ifndef UE4SS_SDK_LetterUI_HPP
#define UE4SS_SDK_LetterUI_HPP

class ULetterUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Image_38;                                                           // 0x0348 (size: 0x8)
    class UTextBlock* ExitText;                                                       // 0x0350 (size: 0x8)
    class UButton* ExitButton;                                                        // 0x0358 (size: 0x8)
    class UTextBlock* Description;                                                    // 0x0360 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0368 (size: 0x8)

    void BndEvt__LetterUI_Button_191_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__LetterUI_ExitButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void SetText(FText Description);
    void Construct();
    void Exit();
    void BndEvt__LetterUI_ExitButton_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_LetterUI(int32 EntryPoint);
}; // Size: 0x370

#endif
