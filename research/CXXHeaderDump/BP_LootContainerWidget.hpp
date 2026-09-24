#ifndef UE4SS_SDK_BP_LootContainerWidget_HPP
#define UE4SS_SDK_BP_LootContainerWidget_HPP

class UBP_LootContainerWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* txt;                                                            // 0x0348 (size: 0x8)
    class USquareProgressBar_C* SquareProgressBar;                                    // 0x0350 (size: 0x8)
    class UTextBlock* QuickSearch;                                                    // 0x0358 (size: 0x8)
    class UTextBlock* InteractTxt;                                                    // 0x0360 (size: 0x8)
    class UVerticalBox* InteractOptions;                                              // 0x0368 (size: 0x8)
    class UImage* Image_62;                                                           // 0x0370 (size: 0x8)
    class UTextBlock* ContainerLootTypeTxt;                                           // 0x0378 (size: 0x8)
    class UTextBlock* AdditionalTxt;                                                  // 0x0380 (size: 0x8)
    FLinearColor DefColor;                                                            // 0x0388 (size: 0x10)
    double CurrentTime;                                                               // 0x0398 (size: 0x8)
    double MaxTime;                                                                   // 0x03A0 (size: 0x8)
    double Multiplier;                                                                // 0x03A8 (size: 0x8)
    bool CanCauseNoise?;                                                              // 0x03B0 (size: 0x1)
    class AActor* ContainerRef;                                                       // 0x03B8 (size: 0x8)
    TMap<FGameplayTag, FText> OptionMap;                                              // 0x03C0 (size: 0x50)
    TMap<FGameplayTag, UBP_InteractWidgetOption_C*> OptionW;                          // 0x0410 (size: 0x50)
    TArray<UBP_InteractWidgetOption_C*> Options;                                      // 0x0460 (size: 0x10)
    FText ContainerName;                                                              // 0x0470 (size: 0x10)
    FText LootType;                                                                   // 0x0480 (size: 0x10)

    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void Construct();
    void SetOption(FGameplayTag Option);
    void SetupWidget(const TMap<FGameplayTag, FText> Options);
    void UpdateOptionText(FGameplayTag Option, FText NewText);
    void ExecuteUbergraph_BP_LootContainerWidget(int32 EntryPoint);
}; // Size: 0x490

#endif
