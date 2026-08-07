#ifndef UE4SS_SDK_S_GenericAISettings_HPP
#define UE4SS_SDK_S_GenericAISettings_HPP

struct FS_GenericAISettings
{
    bool UseGeneralSettings_32_6A0BCE09431CD1D11E78E6B7C16B7861;                      // 0x0000 (size: 0x1)
    bool UseAudioReactionSettings_17_AC9F7411404780F54A4C5D8F8F7A4F4A;                // 0x0001 (size: 0x1)
    TMap<FName, TEnumAsByte<E_StimuliReaction::Type>> AudioReaction_12_3A45C6DE4FCB3C3E9A196FA39426A915; // 0x0008 (size: 0x50)
    bool UseSightReactionSettings_19_7CB58FA34CEB32F7352022BF60269826;                // 0x0058 (size: 0x1)
    TMap<FName, TEnumAsByte<E_StimuliReaction::Type>> SightReactions_24_04A41A7F4A066FADE635A0BEA073C012; // 0x0060 (size: 0x50)
    bool UseAimedReactions_50_E12984A945CE40555DD85A9A05D847B7;                       // 0x00B0 (size: 0x1)
    bool AimedFocusRequiresSight_52_608D2CC84EABA7877936CAB49A9D09AD;                 // 0x00B1 (size: 0x1)
    TMap<FName, TEnumAsByte<E_StimuliReaction::Type>> AimedFocusReactionSettings_46_FA275B1E47294E4C747585BCD87A9C4C; // 0x00B8 (size: 0x50)
    bool IsReactionTemp_26_8CE38C3E4866076BAD956FBF67B86739;                          // 0x0108 (size: 0x1)
    float TempTime_29_FECC46BD4E7279E3D4166588792D5303;                               // 0x010C (size: 0x4)
    TArray<class USoundCue*> AudioReactions_41_C11CB89343680758598383ACBFDB9E50;      // 0x0110 (size: 0x10)
    TArray<class UAnimMontage*> AnimReactions_40_229C1FB5485B00B5C0AC14B99350FB43;    // 0x0120 (size: 0x10)
    FS_AudioStartEnd AudioTransitions_2_B02F53AC4C75B0AD693D4D8C4FC0617B;             // 0x0130 (size: 0x20)
    FS_AnimStartEnd AnimationTransitions_5_AB0389CE4DE5DA576CF8848BBE8ADA44;          // 0x0150 (size: 0x20)
    bool StopMovementInTransition_15_48F51DFB4FB0AE61BCCF44B7F8FCD836;                // 0x0170 (size: 0x1)

}; // Size: 0x171

#endif
