#ifndef UE4SS_SDK_WBP_NarrativeButton_HPP
#define UE4SS_SDK_WBP_NarrativeButton_HPP

class UWBP_NarrativeButton_C : public UCommonButtonBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x1530 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_ButtonText;                               // 0x1538 (size: 0x8)
    class UWidgetAnimation* Clicked;                                                  // 0x1540 (size: 0x8)
    FText Text;                                                                       // 0x1548 (size: 0x10)
    TEnumAsByte<EHorizontalAlignment> TextAlignment;                                  // 0x1558 (size: 0x1)

    void PreConstruct(bool IsDesignTime);
    void Set Button Text(FText Text);
    void BP_OnClicked();
    void OnCurrentTextStyleChanged();
    void ExecuteUbergraph_WBP_NarrativeButton(int32 EntryPoint);
}; // Size: 0x1559

#endif
