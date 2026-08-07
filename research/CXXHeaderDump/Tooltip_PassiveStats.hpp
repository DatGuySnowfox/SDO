#ifndef UE4SS_SDK_Tooltip_PassiveStats_HPP
#define UE4SS_SDK_Tooltip_PassiveStats_HPP

class UTooltip_PassiveStats_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* DescriptionText;                                                // 0x02C8 (size: 0x8)
    class UImage* Image_176;                                                          // 0x02D0 (size: 0x8)
    class UTextBlock* TitleText;                                                      // 0x02D8 (size: 0x8)
    FText Title;                                                                      // 0x02E0 (size: 0x18)
    FText Description;                                                                // 0x02F8 (size: 0x18)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_Tooltip_PassiveStats(int32 EntryPoint);
}; // Size: 0x310

#endif
