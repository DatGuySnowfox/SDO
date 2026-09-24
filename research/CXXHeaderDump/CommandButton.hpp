#ifndef UE4SS_SDK_CommandButton_HPP
#define UE4SS_SDK_CommandButton_HPP

class UCommandButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UOverlay* Overlay_80;                                                       // 0x0348 (size: 0x8)
    class UTextBlock* ButtonTitle;                                                    // 0x0350 (size: 0x8)
    class UImage* ButtonIcon;                                                         // 0x0358 (size: 0x8)
    class UButton* Button;                                                            // 0x0360 (size: 0x8)
    FCommandButton_COnPressed OnPressed;                                              // 0x0368 (size: 0x10)
    void OnPressed();
    class UTexture2D* Icon;                                                           // 0x0378 (size: 0x8)
    FText Text;                                                                       // 0x0380 (size: 0x10)
    bool isClickable;                                                                 // 0x0390 (size: 0x1)

    void PreConstruct(bool IsDesignTime);
    void BndEvt__Button_K2Node_ComponentBoundEvent_1_OnButtonPressedEvent__DelegateSignature();
    void UpdateIcon(class UTexture2D* Texture);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_CommandButton(int32 EntryPoint);
    void OnPressed__DelegateSignature();
}; // Size: 0x391

#endif
