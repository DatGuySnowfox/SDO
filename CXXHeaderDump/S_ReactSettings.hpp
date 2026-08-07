#ifndef UE4SS_SDK_S_ReactSettings_HPP
#define UE4SS_SDK_S_ReactSettings_HPP

struct FS_ReactSettings
{
    TArray<FName> ReactTags_3_0B1EB31340205B1B774807A2583CC984;                       // 0x0000 (size: 0x10)
    TEnumAsByte<E_StimuliReaction::Type> BehaviourResponse_7_39DE9D5C4EE333FAEDB4D88DEF230D9D; // 0x0010 (size: 0x1)
    bool IsResponseTemporary_15_F309DD014070D1A5AF5EB0829E0C66C8;                     // 0x0011 (size: 0x1)
    float TemporaryTime_13_36D1E215449F5B7C7273F08E43A1C251;                          // 0x0014 (size: 0x4)
    bool CancelOverrideBehaviour_17_29C1086A470F9A4B442B5D83A0727F04;                 // 0x0018 (size: 0x1)
    TArray<class UAnimMontage*> Animations_21_084E2E78404169BBDDC195966E7815D4;       // 0x0020 (size: 0x10)
    TArray<class USoundCue*> Audio_24_146DA9FE474D0ECA6B72F4B597EFBCA7;               // 0x0030 (size: 0x10)
    bool IgnoreReactCooldown_28_C412BA8447018B48C8623F9EFD398DFF;                     // 0x0040 (size: 0x1)

}; // Size: 0x41

#endif
