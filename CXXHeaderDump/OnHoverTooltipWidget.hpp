#ifndef UE4SS_SDK_OnHoverTooltipWidget_HPP
#define UE4SS_SDK_OnHoverTooltipWidget_HPP

class UOnHoverTooltipWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* descr;                                                          // 0x02C8 (size: 0x8)
    class UBorder* Divider;                                                           // 0x02D0 (size: 0x8)
    class UTextBlock* DurText;                                                        // 0x02D8 (size: 0x8)
    class UTextBlock* NameText;                                                       // 0x02E0 (size: 0x8)
    class UTextBlock* PriceText;                                                      // 0x02E8 (size: 0x8)
    class UTextBlock* RarityText;                                                     // 0x02F0 (size: 0x8)
    class UVerticalBox* StatsVB;                                                      // 0x02F8 (size: 0x8)
    class UUniformGridPanel* TextStatsGrid;                                           // 0x0300 (size: 0x8)
    class UTextBlock* TypeText;                                                       // 0x0308 (size: 0x8)
    class UTextBlock* WeightText;                                                     // 0x0310 (size: 0x8)
    FSlateFontInfo DefaultFont;                                                       // 0x0318 (size: 0x58)
    class UJSI_Slot_C* ItemRef;                                                       // 0x0370 (size: 0x8)
    int32 GridRow;                                                                    // 0x0378 (size: 0x4)
    int32 GridColumn;                                                                 // 0x037C (size: 0x4)

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
}; // Size: 0x380

#endif
