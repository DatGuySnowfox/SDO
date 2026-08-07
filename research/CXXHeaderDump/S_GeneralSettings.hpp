#ifndef UE4SS_SDK_S_GeneralSettings_HPP
#define UE4SS_SDK_S_GeneralSettings_HPP

struct FS_GeneralSettings
{
    bool UseGeneralSettings_1_34451C7C4409593AED51F3B3E745EC23;                       // 0x0000 (size: 0x1)
    bool UseAudioReaction_8_10D65367410D004C11F2298C0904D830;                         // 0x0001 (size: 0x1)
    TArray<FS_ReactSettings> AudioReactions_9_9F68933146B06EA6F5D62EB5E5822ACC;       // 0x0008 (size: 0x10)
    bool UseSightReactions_13_7B8E557842E4006DA471D6B8FD8DD197;                       // 0x0018 (size: 0x1)
    TArray<FS_ReactSettings> SightReactions_17_FC023EA447F72317482A669D75BF2D42;      // 0x0020 (size: 0x10)
    bool UseDamageReactions_43_651510D34745B3E71DFBA898CC66E10E;                      // 0x0030 (size: 0x1)
    TArray<FS_ReactSettings> DamageReactions_47_0171D092410A910D027CCF8E4DBF0ADD;     // 0x0038 (size: 0x10)
    bool UseAimedFocusReactions_21_3FB701744BFF4A199FA8E0BC1B56F96F;                  // 0x0048 (size: 0x1)
    bool AimedFocusRequiresSight_24_304EB221463960ADCDB9B283F7EA2C17;                 // 0x0049 (size: 0x1)
    TArray<FS_ReactSettings> AimedFocusReactions_28_B5A136154388B5E2D151B884544FCA8D; // 0x0050 (size: 0x10)
    bool StopMovementInTransitions_39_8824967345143DE490031A998B1ADF76;               // 0x0060 (size: 0x1)
    FS_AnimStartEnd AnimationBehaviourTransitions_34_940A69AD4E3A01C4C46A72AF087872AD; // 0x0068 (size: 0x20)
    FS_AudioStartEnd AudioBehaviourTransitions_40_FE92B55C4D0108CEB4801D9BA35D4E08;   // 0x0088 (size: 0x20)

}; // Size: 0xA8

#endif
