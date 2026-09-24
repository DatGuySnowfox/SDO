#ifndef UE4SS_SDK_Journal_SkillTreeTooltip_HPP
#define UE4SS_SDK_Journal_SkillTreeTooltip_HPP

class UJournal_SkillTreeTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* SkillText;                                                      // 0x0348 (size: 0x8)
    class UTextBlock* PointText;                                                      // 0x0350 (size: 0x8)
    class UImage* Image_176;                                                          // 0x0358 (size: 0x8)
    class UTextBlock* DescriptionText;                                                // 0x0360 (size: 0x8)
    FText SkillName;                                                                  // 0x0368 (size: 0x10)
    FText SkillPointsNeeded;                                                          // 0x0378 (size: 0x10)
    FText SkillDescription;                                                           // 0x0388 (size: 0x10)
    FSlateColor TitleColor;                                                           // 0x0398 (size: 0x14)

    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_Journal_SkillTreeTooltip(int32 EntryPoint);
}; // Size: 0x3AC

#endif
