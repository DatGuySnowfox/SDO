#ifndef UE4SS_SDK_S_Effects_HPP
#define UE4SS_SDK_S_Effects_HPP

struct FS_Effects
{
    bool IsEnabled_18_5F5859E6449CA2118889C29B812D8161;                               // 0x0000 (size: 0x1)
    TEnumAsByte<ETriggerMethod::Type> TriggerMethod_50_78811DED4A74E896F3726B82ADFE2980; // 0x0001 (size: 0x1)
    float SpecificPercentValue_13_5C754BFC45B9763D27A6A9AB9187390F;                   // 0x0004 (size: 0x4)
    FVector2D EffectScale_61_63D82B6D464CD3CCEC1B078074C08549;                        // 0x0008 (size: 0x10)
    FVector2D EffectTranslation_62_B2FC126540FD38111139ECA600383ADB;                  // 0x0018 (size: 0x10)
    FVector2D EffectShear_60_AAC8D1A74D4F67E95F82FE9EB0D3581A;                        // 0x0028 (size: 0x10)
    float EffectAngle_67_9654D26045ED5561CC56AC9DF3981F59;                            // 0x0038 (size: 0x4)
    TEnumAsByte<EEffectLayer::Type> EffectLayer_22_3309B47F49D95F295EC24789EAAF4815;  // 0x003C (size: 0x1)
    TEnumAsByte<EEffectType::Type> EffectType_78_DA737BD8465723F56B6C5DAFC5F147B5;    // 0x003D (size: 0x1)
    int32 NumParticles_70_F3941F714B9C03FD09B052B91754A004;                           // 0x0040 (size: 0x4)
    class UObject* CustomEffectTexture_37_071362A64918FCBA33D4FF85BB9ABA70;           // 0x0048 (size: 0x8)
    FLinearColor EffectColor_49_B8696A40425D278129557CB171157594;                     // 0x0050 (size: 0x10)
    float PlaybackSpeed_63_895E2BF0490600F327E6D3A40CD1FF43;                          // 0x0060 (size: 0x4)
    float ParticleSpread_77_91AAD9984A826CA627C0E894AAE3D287;                         // 0x0064 (size: 0x4)
    bool AddRotationtoParticles_76_244A1C394C00164B10A2AB9E13ED2914;                  // 0x0068 (size: 0x1)
    class USoundBase* SoundEffect_81_D5F6F19D40A0B92BB0598E8C416B50E0;                // 0x0070 (size: 0x8)
    float SoundVolumeMultiplier_85_6B4801114C18FDBC28D92AABF034AFE0;                  // 0x0078 (size: 0x4)
    float DecayTime_88_B89D47A944D4AEE43F918889E359E9D2;                              // 0x007C (size: 0x4)
    bool UseStretchedParticle_90_1C1A4D3543806DE1B5BD93909DA565BB;                    // 0x0080 (size: 0x1)

}; // Size: 0x81

#endif
