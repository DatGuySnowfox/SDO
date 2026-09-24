#ifndef UE4SS_SDK_CommonSlider_HPP
#define UE4SS_SDK_CommonSlider_HPP

class UCommonSlider_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USizeBox* TopSpacer;                                                        // 0x0348 (size: 0x8)
    class USpacer* SpacerFill;                                                        // 0x0350 (size: 0x8)
    class UTextBlock* SliderValue;                                                    // 0x0358 (size: 0x8)
    class USizeBox* SliderSizeBox;                                                    // 0x0360 (size: 0x8)
    class UTextBlock* SliderName;                                                     // 0x0368 (size: 0x8)
    class USlider* Slider;                                                            // 0x0370 (size: 0x8)
    class USizeBox* SizeBox;                                                          // 0x0378 (size: 0x8)
    class USpacer* RightMargin;                                                       // 0x0380 (size: 0x8)
    class UImage* LockImage;                                                          // 0x0388 (size: 0x8)
    class USizeBox* Locked;                                                           // 0x0390 (size: 0x8)
    class USpacer* LeftMargin;                                                        // 0x0398 (size: 0x8)
    class UImage* img_right_btn;                                                      // 0x03A0 (size: 0x8)
    class UImage* img_left_btn;                                                       // 0x03A8 (size: 0x8)
    class UHorizontalBox* HorizontalBox-ElementsContainer;                            // 0x03B0 (size: 0x8)
    class UButton* btn_right;                                                         // 0x03B8 (size: 0x8)
    class UButton* btn_left;                                                          // 0x03C0 (size: 0x8)
    class USizeBox* BottomSpacer;                                                     // 0x03C8 (size: 0x8)
    FText SliderTitle;                                                                // 0x03D0 (size: 0x10)
    FText Unit;                                                                       // 0x03E0 (size: 0x10)
    FText UnitPrefix;                                                                 // 0x03F0 (size: 0x10)
    int32 FontSize;                                                                   // 0x0400 (size: 0x4)
    double SliderMinimumValue;                                                        // 0x0408 (size: 0x8)
    double SliderMaximumValue;                                                        // 0x0410 (size: 0x8)
    double StepSize;                                                                  // 0x0418 (size: 0x8)
    FText SliderDefaultValueTip;                                                      // 0x0420 (size: 0x10)
    bool Integer?;                                                                    // 0x0430 (size: 0x1)
    bool UseGrouping?;                                                                // 0x0431 (size: 0x1)
    int32 MinimumFractionalDigits;                                                    // 0x0434 (size: 0x4)
    int32 MaximumFractionalDigits;                                                    // 0x0438 (size: 0x4)
    bool ChangeSliderColorWhenFocused?;                                               // 0x043C (size: 0x1)
    bool ChangeTextColorWhenFocused?;                                                 // 0x043D (size: 0x1)
    FLinearColor FocusedSliderHandleAndTextColor;                                     // 0x0440 (size: 0x10)
    FLinearColor UnfocusedSliderHandleAndTextColor;                                   // 0x0450 (size: 0x10)
    double TopMargin;                                                                 // 0x0460 (size: 0x8)
    double BottomMargin;                                                              // 0x0468 (size: 0x8)
    double LeftSideMargin;                                                            // 0x0470 (size: 0x8)
    double RightSideMargin;                                                           // 0x0478 (size: 0x8)
    double SliderSize;                                                                // 0x0480 (size: 0x8)
    double DefaultValue;                                                              // 0x0488 (size: 0x8)
    class UPhotoModeWidget_C* PhotoModeWidget;                                        // 0x0490 (size: 0x8)
    FCommonSlider_CSliderDispatcher SliderDispatcher;                                 // 0x0498 (size: 0x10)
    void SliderDispatcher(double SliderValue);
    bool Focus?;                                                                      // 0x04A8 (size: 0x1)
    FLinearColor FocusedSliderBarColor;                                               // 0x04AC (size: 0x10)
    FLinearColor UnfocusedSliderBarColor;                                             // 0x04BC (size: 0x10)

    FEventReply OnMouseWheel(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    ESlateVisibility TextValueVisibility();
    ESlateVisibility LockVisibility();
    FSlateBrush SetRightButtonSettings();
    FSlateBrush SetLeftButtonSettings();
    void Set Focused or Unfocused Color(bool ChangeSliderColorWhenFocused?, bool ChangeTextColorWhenFocused?, class USlider* Slider, class UImage* imgLeftButton, class UImage* imgRightButton, class UWidget* SliderTitle, class UWidget* SliderValue, bool SetMargin, FSlateBrush& LeftButtonColor, FSlateBrush& RightButtonColor);
    FText setTxtValue();
    void PreConstruct(bool IsDesignTime);
    void Initialize(double DefaultValue, class UPhotoModeWidget_C* PhotoModeWidget (Self));
    void BndEvt__btn_left_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__btn_right_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Slider_K2Node_ComponentBoundEvent_2_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void ResetSlider(bool Focus?);
    void ChangeValue(double Value, bool SetFocus?);
    void EnableDisableWidget(bool Enable?);
    void SetSliderFocus();
    void ResetSliderFocus();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void FocusWithMouseWheel();
    void ExecuteUbergraph_CommonSlider(int32 EntryPoint);
    void SliderDispatcher__DelegateSignature(double SliderValue);
}; // Size: 0x4CC

#endif
