#ifndef UE4SS_SDK_BP_InteractWidgetOption_HPP
#define UE4SS_SDK_BP_InteractWidgetOption_HPP

class UBP_InteractWidgetOption_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Option;                                                            // 0x02C8 (size: 0x8)
    class UTextBlock* txt;                                                            // 0x02D0 (size: 0x8)
    FText OptionName;                                                                 // 0x02D8 (size: 0x18)
    FGameplayTag OptionTag;                                                           // 0x02F0 (size: 0x8)
    FBP_InteractWidgetOption_COnSelected OnSelected;                                  // 0x02F8 (size: 0x10)
    void OnSelected(class UBP_InteractWidgetOption_C* Option);

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SetSelected(bool Value);
    void UpdateText(FText OptionName);
    void Construct();
    void ExecuteUbergraph_BP_InteractWidgetOption(int32 EntryPoint);
    void OnSelected__DelegateSignature(class UBP_InteractWidgetOption_C* Option);
}; // Size: 0x308

#endif
