#ifndef UE4SS_SDK_WBP_NarrativeButton_HPP
#define UE4SS_SDK_WBP_NarrativeButton_HPP

class UWBP_NarrativeButton_C : public UCommonButtonBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x1580 (size: 0x8)
    class UWidgetAnimation* Clicked;                                                  // 0x1588 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_ButtonText;                               // 0x1590 (size: 0x8)
    FText Text;                                                                       // 0x1598 (size: 0x18)
    TEnumAsByte<EHorizontalAlignment> TextAlignment;                                  // 0x15B0 (size: 0x1)

    void PreConstruct(bool IsDesignTime);
    void Set Button Text(FText Text);
    void BP_OnClicked();
    void OnCurrentTextStyleChanged();
    void ExecuteUbergraph_WBP_NarrativeButton(int32 EntryPoint);
}; // Size: 0x15B1

#endif
