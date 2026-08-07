#ifndef UE4SS_SDK_CommandButton_HPP
#define UE4SS_SDK_CommandButton_HPP

class UCommandButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button;                                                            // 0x02C8 (size: 0x8)
    class UImage* ButtonIcon;                                                         // 0x02D0 (size: 0x8)
    class UTextBlock* ButtonTitle;                                                    // 0x02D8 (size: 0x8)
    class UOverlay* Overlay_80;                                                       // 0x02E0 (size: 0x8)
    FCommandButton_COnPressed OnPressed;                                              // 0x02E8 (size: 0x10)
    void OnPressed();
    class UTexture2D* Icon;                                                           // 0x02F8 (size: 0x8)
    FText Text;                                                                       // 0x0300 (size: 0x18)
    bool isClickable;                                                                 // 0x0318 (size: 0x1)

    void PreConstruct(bool IsDesignTime);
    void BndEvt__Button_K2Node_ComponentBoundEvent_1_OnButtonPressedEvent__DelegateSignature();
    void UpdateIcon(class UTexture2D* Texture);
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_CommandButton(int32 EntryPoint);
    void OnPressed__DelegateSignature();
}; // Size: 0x319

#endif
