#ifndef UE4SS_SDK_SelectorSlider_HPP
#define UE4SS_SDK_SelectorSlider_HPP

class USelectorSlider_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USizeBox* Valuecontainer-SizeBox;                                           // 0x0348 (size: 0x8)
    class USizeBox* TopSpacer;                                                        // 0x0350 (size: 0x8)
    class UTextBlock* SliderValue;                                                    // 0x0358 (size: 0x8)
    class USizeBox* SliderSizeBox;                                                    // 0x0360 (size: 0x8)
    class UTextBlock* SliderName;                                                     // 0x0368 (size: 0x8)
    class USlider* Slider;                                                            // 0x0370 (size: 0x8)
    class USizeBox* SizeBox;                                                          // 0x0378 (size: 0x8)
    class USpacer* RightMargin;                                                       // 0x0380 (size: 0x8)
    class USpacer* LeftMargin;                                                        // 0x0388 (size: 0x8)
    class UImage* img_right_btn;                                                      // 0x0390 (size: 0x8)
    class UImage* img_left_btn;                                                       // 0x0398 (size: 0x8)
    class UButton* btn_right;                                                         // 0x03A0 (size: 0x8)
    class UButton* btn_left;                                                          // 0x03A8 (size: 0x8)
    class USizeBox* BottomSpacer;                                                     // 0x03B0 (size: 0x8)
    class USizeBox* Bordercontainer-SizeBox;                                          // 0x03B8 (size: 0x8)
    class UBorder* Border;                                                            // 0x03C0 (size: 0x8)
    FText SliderTitle;                                                                // 0x03C8 (size: 0x10)
    FText Unit;                                                                       // 0x03D8 (size: 0x10)
    FText UnitPrefix;                                                                 // 0x03E8 (size: 0x10)
    int32 FontSize;                                                                   // 0x03F8 (size: 0x4)
    FText SliderDefaultValueTip;                                                      // 0x0400 (size: 0x10)
    bool ChangeTextColorWhenFocused?;                                                 // 0x0410 (size: 0x1)
    FLinearColor FocusedSliderAndTextColor;                                           // 0x0414 (size: 0x10)
    FLinearColor UnfocusedSliderAndTextColor;                                         // 0x0424 (size: 0x10)
    double TopMargin;                                                                 // 0x0438 (size: 0x8)
    double BottomMargin;                                                              // 0x0440 (size: 0x8)
    double LeftSideMargin;                                                            // 0x0448 (size: 0x8)
    double RightSideMargin;                                                           // 0x0450 (size: 0x8)
    double BoxSize;                                                                   // 0x0458 (size: 0x8)
    double DefaultValue;                                                              // 0x0460 (size: 0x8)
    TArray<FText> Options;                                                            // 0x0468 (size: 0x10)
    class UPhotoModeWidget_C* PhotoModeWidget;                                        // 0x0478 (size: 0x8)
    FSelectorSlider_CDispatcher Dispatcher;                                           // 0x0480 (size: 0x10)
    void Dispatcher(double Value);
    bool Use 0 as default option?;                                                    // 0x0490 (size: 0x1)
    FText Default option name (if boolean is true);                                   // 0x0498 (size: 0x10)
    FLinearColor BackgroundColor;                                                     // 0x04A8 (size: 0x10)
    bool Focus?;                                                                      // 0x04B8 (size: 0x1)
    FSelectorSlider_CResetDispatcher ResetDispatcher;                                 // 0x04C0 (size: 0x10)
    void ResetDispatcher();

    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    FSlateBrush SetRightButtonSettings();
    FSlateBrush SetLeftButtonSettings();
    void Set Focused or Unfocused Color(bool ChangeSliderColorWhenFocused?, bool ChangeTextColorWhenFocused?, class USlider* Slider, class UImage* imgLeftButton, class UImage* imgRightButton, class UWidget* SliderTitle, class UWidget* SliderValue, bool SetMargin, FSlateBrush& LeftButtonColor, FSlateBrush& RightButtonColor);
    FText setTxtValue();
    void PreConstruct(bool IsDesignTime);
    void Initialize(double DefaultValue, const TArray<FText>& Options, bool Use 0 as default option?, FText Default option name (if boolean is true), class UPhotoModeWidget_C* PhotoModeWidget (Self));
    void BndEvt__btn_left_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btn_right_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Slider_K2Node_ComponentBoundEvent_2_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void ResetSlider(bool Focus?, bool ResetThisSlider?);
    void SetSliderFocus();
    void ResetSliderFocus();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void ChangeValue(double Value, bool Focus?);
    void FocusWithMouseWheel();
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_SelectorSlider(int32 EntryPoint);
    void ResetDispatcher__DelegateSignature();
    void Dispatcher__DelegateSignature(double Value);
}; // Size: 0x4D0

#endif
