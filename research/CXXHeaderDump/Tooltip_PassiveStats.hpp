#ifndef UE4SS_SDK_Tooltip_PassiveStats_HPP
#define UE4SS_SDK_Tooltip_PassiveStats_HPP

class UTooltip_PassiveStats_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TitleText;                                                      // 0x0348 (size: 0x8)
    class UImage* Image_176;                                                          // 0x0350 (size: 0x8)
    class UTextBlock* DescriptionText;                                                // 0x0358 (size: 0x8)
    FText Title;                                                                      // 0x0360 (size: 0x10)
    FText Description;                                                                // 0x0370 (size: 0x10)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_Tooltip_PassiveStats(int32 EntryPoint);
}; // Size: 0x380

#endif
