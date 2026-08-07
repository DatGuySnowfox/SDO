#ifndef UE4SS_SDK_BP_SkillTreeAsset_HPP
#define UE4SS_SDK_BP_SkillTreeAsset_HPP

class UBP_SkillTreeAsset_C : public UTechnologyAsset
{
    TEnumAsByte<Enum_SkillType::Type> Type;                                           // 0x0100 (size: 0x1)
    FText PrimaryText;                                                                // 0x0108 (size: 0x18)
    int32 Cost;                                                                       // 0x0120 (size: 0x4)
    FText ToolTipText;                                                                // 0x0128 (size: 0x18)

}; // Size: 0x140

#endif
