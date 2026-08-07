#ifndef UE4SS_SDK_Journal_SkillTreeTooltip_HPP
#define UE4SS_SDK_Journal_SkillTreeTooltip_HPP

class UJournal_SkillTreeTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* DescriptionText;                                                // 0x02C8 (size: 0x8)
    class UImage* Image_176;                                                          // 0x02D0 (size: 0x8)
    class UTextBlock* PointText;                                                      // 0x02D8 (size: 0x8)
    class UTextBlock* SkillText;                                                      // 0x02E0 (size: 0x8)
    FText SkillName;                                                                  // 0x02E8 (size: 0x18)
    FText SkillPointsNeeded;                                                          // 0x0300 (size: 0x18)
    FText SkillDescription;                                                           // 0x0318 (size: 0x18)
    FSlateColor TitleColor;                                                           // 0x0330 (size: 0x14)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_Journal_SkillTreeTooltip(int32 EntryPoint);
}; // Size: 0x344

#endif
