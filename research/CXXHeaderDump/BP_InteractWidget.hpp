#ifndef UE4SS_SDK_BP_InteractWidget_HPP
#define UE4SS_SDK_BP_InteractWidget_HPP

class UBP_InteractWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* AdditionalTxt;                                                  // 0x02C8 (size: 0x8)
    class UVerticalBox* InteractOptions;                                              // 0x02D0 (size: 0x8)
    class UTextBlock* ObjectName;                                                     // 0x02D8 (size: 0x8)
    class UVerticalBox* StatOptions;                                                  // 0x02E0 (size: 0x8)
    class UTextBlock* TxtButton;                                                      // 0x02E8 (size: 0x8)
    FLinearColor DefColor;                                                            // 0x02F0 (size: 0x10)
    TArray<class UBP_InteractWidgetOption_C*> Options;                                // 0x0300 (size: 0x10)
    TMap<class FGameplayTag, class FText> OptionMap;                                  // 0x0310 (size: 0x50)
    TMap<class FGameplayTag, class UBP_InteractWidgetOption_C*> OptionW;              // 0x0360 (size: 0x50)
    FText Name;                                                                       // 0x03B0 (size: 0x18)
    TArray<class UBP_InteractWidgetStat_C*> Stats;                                    // 0x03C8 (size: 0x10)
    TMap<class FName, class FText> StatMap;                                           // 0x03D8 (size: 0x50)
    TMap<class FName, class UBP_InteractWidgetStat_C*> StatsW;                        // 0x0428 (size: 0x50)

    void SetOption(FGameplayTag Option);
    void SetupWidget(const TMap<class FGameplayTag, class FText> Options, TMap<class FName, class FText> Stats);
    void UpdateOptionText(FGameplayTag Option, FText NewText);
    void Construct();
    void Destruct();
    void ClearOptions();
    void ExecuteUbergraph_BP_InteractWidget(int32 EntryPoint);
}; // Size: 0x478

#endif
