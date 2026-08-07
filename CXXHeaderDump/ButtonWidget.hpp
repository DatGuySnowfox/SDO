#ifndef UE4SS_SDK_ButtonWidget_HPP
#define UE4SS_SDK_ButtonWidget_HPP

class UButtonWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button;                                                            // 0x02C8 (size: 0x8)
    class UTextBlock* ButtonText;                                                     // 0x02D0 (size: 0x8)
    FText Text;                                                                       // 0x02D8 (size: 0x18)
    FButtonWidget_CButtonPressed ButtonPressed;                                       // 0x02F0 (size: 0x10)
    void ButtonPressed();
    float TextSize;                                                                   // 0x0300 (size: 0x4)

    void BndEvt__ButtonWidget_Button_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void PreConstruct(bool IsDesignTime);
    void SetSelected(bool Selected?);
    void ExecuteUbergraph_ButtonWidget(int32 EntryPoint);
    void ButtonPressed__DelegateSignature();
}; // Size: 0x304

#endif
