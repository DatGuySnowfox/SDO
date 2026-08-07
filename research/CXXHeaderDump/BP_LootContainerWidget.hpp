#ifndef UE4SS_SDK_BP_LootContainerWidget_HPP
#define UE4SS_SDK_BP_LootContainerWidget_HPP

class UBP_LootContainerWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* AdditionalTxt;                                                  // 0x02C8 (size: 0x8)
    class UTextBlock* ContainerLootTypeTxt;                                           // 0x02D0 (size: 0x8)
    class UImage* Image_62;                                                           // 0x02D8 (size: 0x8)
    class UVerticalBox* InteractOptions;                                              // 0x02E0 (size: 0x8)
    class UTextBlock* InteractTxt;                                                    // 0x02E8 (size: 0x8)
    class UTextBlock* QuickSearch;                                                    // 0x02F0 (size: 0x8)
    class USquareProgressBar_C* SquareProgressBar;                                    // 0x02F8 (size: 0x8)
    class UTextBlock* txt;                                                            // 0x0300 (size: 0x8)
    FLinearColor DefColor;                                                            // 0x0308 (size: 0x10)
    double CurrentTime;                                                               // 0x0318 (size: 0x8)
    double MaxTime;                                                                   // 0x0320 (size: 0x8)
    double Multiplier;                                                                // 0x0328 (size: 0x8)
    bool CanCauseNoise?;                                                              // 0x0330 (size: 0x1)
    class AActor* ContainerRef;                                                       // 0x0338 (size: 0x8)
    TMap<class FGameplayTag, class FText> OptionMap;                                  // 0x0340 (size: 0x50)
    TMap<class FGameplayTag, class UBP_InteractWidgetOption_C*> OptionW;              // 0x0390 (size: 0x50)
    TArray<class UBP_InteractWidgetOption_C*> Options;                                // 0x03E0 (size: 0x10)
    FText ContainerName;                                                              // 0x03F0 (size: 0x18)
    FText LootType;                                                                   // 0x0408 (size: 0x18)

    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void Construct();
    void SetOption(FGameplayTag Option);
    void SetupWidget(const TMap<class FGameplayTag, class FText> Options);
    void UpdateOptionText(FGameplayTag Option, FText NewText);
    void ExecuteUbergraph_BP_LootContainerWidget(int32 EntryPoint);
}; // Size: 0x420

#endif
