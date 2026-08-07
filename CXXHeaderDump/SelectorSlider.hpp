#ifndef UE4SS_SDK_SelectorSlider_HPP
#define UE4SS_SDK_SelectorSlider_HPP

class USelectorSlider_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Border;                                                            // 0x02C8 (size: 0x8)
    class USizeBox* Bordercontainer-SizeBox;                                          // 0x02D0 (size: 0x8)
    class USizeBox* BottomSpacer;                                                     // 0x02D8 (size: 0x8)
    class UButton* btn_left;                                                          // 0x02E0 (size: 0x8)
    class UButton* btn_right;                                                         // 0x02E8 (size: 0x8)
    class UImage* img_left_btn;                                                       // 0x02F0 (size: 0x8)
    class UImage* img_right_btn;                                                      // 0x02F8 (size: 0x8)
    class USpacer* LeftMargin;                                                        // 0x0300 (size: 0x8)
    class USpacer* RightMargin;                                                       // 0x0308 (size: 0x8)
    class USizeBox* SizeBox;                                                          // 0x0310 (size: 0x8)
    class USlider* Slider;                                                            // 0x0318 (size: 0x8)
    class UTextBlock* SliderName;                                                     // 0x0320 (size: 0x8)
    class USizeBox* SliderSizeBox;                                                    // 0x0328 (size: 0x8)
    class UTextBlock* SliderValue;                                                    // 0x0330 (size: 0x8)
    class USizeBox* TopSpacer;                                                        // 0x0338 (size: 0x8)
    class USizeBox* Valuecontainer-SizeBox;                                           // 0x0340 (size: 0x8)
    FText SliderTitle;                                                                // 0x0348 (size: 0x18)
    FText Unit;                                                                       // 0x0360 (size: 0x18)
    FText UnitPrefix;                                                                 // 0x0378 (size: 0x18)
    int32 FontSize;                                                                   // 0x0390 (size: 0x4)
    FText SliderDefaultValueTip;                                                      // 0x0398 (size: 0x18)
    bool ChangeTextColorWhenFocused?;                                                 // 0x03B0 (size: 0x1)
    FLinearColor FocusedSliderAndTextColor;                                           // 0x03B4 (size: 0x10)
    FLinearColor UnfocusedSliderAndTextColor;                                         // 0x03C4 (size: 0x10)
    double TopMargin;                                                                 // 0x03D8 (size: 0x8)
    double BottomMargin;                                                              // 0x03E0 (size: 0x8)
    double LeftSideMargin;                                                            // 0x03E8 (size: 0x8)
    double RightSideMargin;                                                           // 0x03F0 (size: 0x8)
    double BoxSize;                                                                   // 0x03F8 (size: 0x8)
    double DefaultValue;                                                              // 0x0400 (size: 0x8)
    TArray<FText> Options;                                                            // 0x0408 (size: 0x10)
    class UPhotoModeWidget_C* PhotoModeWidget;                                        // 0x0418 (size: 0x8)
    FSelectorSlider_CDispatcher Dispatcher;                                           // 0x0420 (size: 0x10)
    void Dispatcher(double Value);
    bool Use 0 as default option?;                                                    // 0x0430 (size: 0x1)
    FText Default option name (if boolean is true);                                   // 0x0438 (size: 0x18)
    FLinearColor BackgroundColor;                                                     // 0x0450 (size: 0x10)
    bool Focus?;                                                                      // 0x0460 (size: 0x1)
    FSelectorSlider_CResetDispatcher ResetDispatcher;                                 // 0x0468 (size: 0x10)
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
}; // Size: 0x478

#endif
