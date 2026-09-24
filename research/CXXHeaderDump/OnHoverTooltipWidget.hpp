#ifndef UE4SS_SDK_OnHoverTooltipWidget_HPP
#define UE4SS_SDK_OnHoverTooltipWidget_HPP

class UOnHoverTooltipWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* WeightText;                                                     // 0x0348 (size: 0x8)
    class UTextBlock* TypeText;                                                       // 0x0350 (size: 0x8)
    class UUniformGridPanel* TextStatsGrid;                                           // 0x0358 (size: 0x8)
    class UVerticalBox* StatsVB;                                                      // 0x0360 (size: 0x8)
    class UTextBlock* RarityText;                                                     // 0x0368 (size: 0x8)
    class UTextBlock* PriceText;                                                      // 0x0370 (size: 0x8)
    class UTextBlock* NameText;                                                       // 0x0378 (size: 0x8)
    class UTextBlock* DurText;                                                        // 0x0380 (size: 0x8)
    class UBorder* Divider;                                                           // 0x0388 (size: 0x8)
    class UImage* DisassemblyIcon;                                                    // 0x0390 (size: 0x8)
    class UTextBlock* descr;                                                          // 0x0398 (size: 0x8)
    FSlateFontInfo DefaultFont;                                                       // 0x03A0 (size: 0x60)
    class UJSI_Slot_C* ItemRef;                                                       // 0x0400 (size: 0x8)
    int32 GridRow;                                                                    // 0x0408 (size: 0x4)
    int32 GridColumn;                                                                 // 0x040C (size: 0x4)

    void GetType(FGameplayTag Type, FText& TypeTxt);
    void GetRarity(FGameplayTag Rarity, FText& RarityTxt);
    void IsVendor(bool& Result);
    void GetPrice(double& Price, double& ExtraPrice);
    FText GetWeight(double Weight);
    void CalculateWeight(double& FinalWeight);
    void Construct();
    void Update(class UJSI_Slot_C* ItemRef);
    void RefreshStats();
    void ExecuteUbergraph_OnHoverTooltipWidget(int32 EntryPoint);
}; // Size: 0x410

#endif
