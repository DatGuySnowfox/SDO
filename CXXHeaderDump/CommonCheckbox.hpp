#ifndef UE4SS_SDK_CommonCheckbox_HPP
#define UE4SS_SDK_CommonCheckbox_HPP

class UCommonCheckbox_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class USizeBox* BottomSpacer;                                                     // 0x02C8 (size: 0x8)
    class UTextBlock* CheckboxName;                                                   // 0x02D0 (size: 0x8)
    class UTextBlock* CheckboxValue;                                                  // 0x02D8 (size: 0x8)
    class USpacer* LeftMargin;                                                        // 0x02E0 (size: 0x8)
    class USpacer* RightMargin;                                                       // 0x02E8 (size: 0x8)
    class UCheckBox* Toggle;                                                          // 0x02F0 (size: 0x8)
    class USizeBox* TopSpacer;                                                        // 0x02F8 (size: 0x8)
    FText ToggleTitle;                                                                // 0x0300 (size: 0x18)
    int32 FontSize;                                                                   // 0x0318 (size: 0x4)
    FText CheckboxDefaultValueTip;                                                    // 0x0320 (size: 0x18)
    bool ChangeCheckboxColorWhenFocused?;                                             // 0x0338 (size: 0x1)
    bool ChangeTextColorWhenFocused?;                                                 // 0x0339 (size: 0x1)
    FLinearColor FocusedToggleAndTextColor;                                           // 0x033C (size: 0x10)
    FLinearColor UnfocusedToggleAndTextColor;                                         // 0x034C (size: 0x10)
    double TopMargin;                                                                 // 0x0360 (size: 0x8)
    double BottomMargin;                                                              // 0x0368 (size: 0x8)
    double LeftSideMargin;                                                            // 0x0370 (size: 0x8)
    double RightSideMargin;                                                           // 0x0378 (size: 0x8)
    bool DefaultValue;                                                                // 0x0380 (size: 0x1)
    class UPhotoModeWidget_C* PhotoModeWidget;                                        // 0x0388 (size: 0x8)
    FText OptionFalse;                                                                // 0x0390 (size: 0x18)
    FText OptionTrue;                                                                 // 0x03A8 (size: 0x18)
    FCommonCheckbox_CCheckboxDispatcher CheckboxDispatcher;                           // 0x03C0 (size: 0x10)
    void CheckboxDispatcher(bool Value);
    bool Focus?;                                                                      // 0x03D0 (size: 0x1)

    class UWidget* ToggleUsingArrows(EUINavigation Navigation);
    void Set Focused or Unfocused Color(bool ChangeCheckboxColorWhenFocused?, bool ChangeTextColorWhenFocused?, class UCheckBox* CheckBox, class UWidget* CheckboxTitle, class UWidget* CheckboxValue);
    FText setTxtValue();
    void PreConstruct(bool IsDesignTime);
    void Initialize(bool DefaultValue, class UPhotoModeWidget_C* PhotoModeWidget (Self));
    void ResetCheckbox(bool Focus?);
    void ChangeValue(bool Value, bool Focus?);
    void BndEvt__ToggleDoF_1_K2Node_ComponentBoundEvent_3_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void SetToggleFocus();
    void ResetToggleFocus();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_CommonCheckbox(int32 EntryPoint);
    void CheckboxDispatcher__DelegateSignature(bool Value);
}; // Size: 0x3D1

#endif
