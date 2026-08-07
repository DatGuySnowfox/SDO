#ifndef UE4SS_SDK_BP_InteractWidgetStat_HPP
#define UE4SS_SDK_BP_InteractWidgetStat_HPP

class UBP_InteractWidgetStat_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Option;                                                            // 0x02C8 (size: 0x8)
    class UTextBlock* StatNameTxt;                                                    // 0x02D0 (size: 0x8)
    class UTextBlock* StatValueTxt;                                                   // 0x02D8 (size: 0x8)
    FName StatName;                                                                   // 0x02E0 (size: 0x8)
    FText StatValue;                                                                  // 0x02E8 (size: 0x18)
    FBP_InteractWidgetStat_COnSelected OnSelected;                                    // 0x0300 (size: 0x10)
    void OnSelected(class UBP_InteractWidgetOption_C* Option);

    void Construct();
    void UpdateStatValue(FText OptionName);
    void ExecuteUbergraph_BP_InteractWidgetStat(int32 EntryPoint);
    void OnSelected__DelegateSignature(class UBP_InteractWidgetOption_C* Option);
}; // Size: 0x310

#endif
