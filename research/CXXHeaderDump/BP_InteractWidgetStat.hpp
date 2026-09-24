#ifndef UE4SS_SDK_BP_InteractWidgetStat_HPP
#define UE4SS_SDK_BP_InteractWidgetStat_HPP

class UBP_InteractWidgetStat_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* StatValueTxt;                                                   // 0x0348 (size: 0x8)
    class UTextBlock* StatNameTxt;                                                    // 0x0350 (size: 0x8)
    class UBorder* Option;                                                            // 0x0358 (size: 0x8)
    FName StatName;                                                                   // 0x0360 (size: 0x8)
    FText StatValue;                                                                  // 0x0368 (size: 0x10)
    FBP_InteractWidgetStat_COnSelected OnSelected;                                    // 0x0378 (size: 0x10)
    void OnSelected(class UBP_InteractWidgetOption_C* Option);

    void Construct();
    void UpdateStatValue(FText OptionName);
    void ExecuteUbergraph_BP_InteractWidgetStat(int32 EntryPoint);
    void OnSelected__DelegateSignature(class UBP_InteractWidgetOption_C* Option);
}; // Size: 0x388

#endif
