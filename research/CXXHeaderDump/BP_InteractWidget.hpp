#ifndef UE4SS_SDK_BP_InteractWidget_HPP
#define UE4SS_SDK_BP_InteractWidget_HPP

class UBP_InteractWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TxtButton;                                                      // 0x0348 (size: 0x8)
    class UVerticalBox* StatOptions;                                                  // 0x0350 (size: 0x8)
    class UTextBlock* ObjectName;                                                     // 0x0358 (size: 0x8)
    class UVerticalBox* InteractOptions;                                              // 0x0360 (size: 0x8)
    class UTextBlock* AdditionalTxt;                                                  // 0x0368 (size: 0x8)
    FLinearColor DefColor;                                                            // 0x0370 (size: 0x10)
    TArray<UBP_InteractWidgetOption_C*> Options;                                      // 0x0380 (size: 0x10)
    TMap<FGameplayTag, FText> OptionMap;                                              // 0x0390 (size: 0x50)
    TMap<FGameplayTag, UBP_InteractWidgetOption_C*> OptionW;                          // 0x03E0 (size: 0x50)
    FText Name;                                                                       // 0x0430 (size: 0x10)
    TArray<UBP_InteractWidgetStat_C*> Stats;                                          // 0x0440 (size: 0x10)
    TMap<FName, FText> StatMap;                                                       // 0x0450 (size: 0x50)
    TMap<FName, UBP_InteractWidgetStat_C*> StatsW;                                    // 0x04A0 (size: 0x50)

    void RemoveInteractOption(FGameplayTag Option);
    void AddInteractOption(const TMap<FGameplayTag, FText>& Option, TMap<FGameplayTag, FText>& OptionAdded);
    void SetOption(FGameplayTag Option);
    void SetupWidget(const TMap<FGameplayTag, FText> Options, TMap<FName, FText> Stats);
    void UpdateOptionText(FGameplayTag Option, FText NewText);
    void Construct();
    void Destruct();
    void ClearOptions();
    void ExecuteUbergraph_BP_InteractWidget(int32 EntryPoint);
}; // Size: 0x4F0

#endif
