#ifndef UE4SS_SDK_BP_SkillTreeAsset_HPP
#define UE4SS_SDK_BP_SkillTreeAsset_HPP

class UBP_SkillTreeAsset_C : public UTechnologyAsset
{
    TEnumAsByte<Enum_SkillType::Type> Type;                                           // 0x00E0 (size: 0x1)
    FText PrimaryText;                                                                // 0x00E8 (size: 0x10)
    int32 Cost;                                                                       // 0x00F8 (size: 0x4)
    FText ToolTipText;                                                                // 0x0100 (size: 0x10)

}; // Size: 0x110

#endif
