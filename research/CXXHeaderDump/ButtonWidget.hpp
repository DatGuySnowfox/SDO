#ifndef UE4SS_SDK_ButtonWidget_HPP
#define UE4SS_SDK_ButtonWidget_HPP

class UButtonWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ButtonText;                                                     // 0x0348 (size: 0x8)
    class UButton* Button;                                                            // 0x0350 (size: 0x8)
    FText Text;                                                                       // 0x0358 (size: 0x10)
    FButtonWidget_CButtonPressed ButtonPressed;                                       // 0x0368 (size: 0x10)
    void ButtonPressed();
    float TextSize;                                                                   // 0x0378 (size: 0x4)

    void BndEvt__ButtonWidget_Button_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void PreConstruct(bool IsDesignTime);
    void SetSelected(bool Selected?);
    void ExecuteUbergraph_ButtonWidget(int32 EntryPoint);
    void ButtonPressed__DelegateSignature();
}; // Size: 0x37C

#endif
