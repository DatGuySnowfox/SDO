#ifndef UE4SS_SDK_S_TriggerResponse_HPP
#define UE4SS_SDK_S_TriggerResponse_HPP

struct FS_TriggerResponse
{
    TArray<class ABP_AITrigger_C*> AITrigger_24_49DC991B4C17A8A3352D6BB4ABE6BE28;     // 0x0000 (size: 0x10)
    TArray<FName> ResponseTags_4_92A8027C49F240C6F7B4B78B97DF0993;                    // 0x0010 (size: 0x10)
    TEnumAsByte<E_StimuliReaction::Type> BehaviourResponse_9_296E761B4F70003F9210C7B59E7621AA; // 0x0020 (size: 0x1)
    bool IsResponseTemporary_42_E911759241E1887FA751C7BB242A8FB0;                     // 0x0021 (size: 0x1)
    float TemporaryTime_43_0CFFE63F4ED4315FF998F7AE8A4DD443;                          // 0x0024 (size: 0x4)
    bool CancelOverrideBehaviour_30_468531DB4F6419E97193D3AD594363E1;                 // 0x0028 (size: 0x1)
    TArray<class UAnimMontage*> Animations_38_2636D7774222DE75D311BDB7517DA641;       // 0x0030 (size: 0x10)
    TArray<class USoundCue*> Audio_41_35B9C49B456A90804F446E9F18B12287;               // 0x0040 (size: 0x10)
    bool IgnoreTriggerCooldown_32_A25B264A4DEE60CDBB6DB0B2E5824A03;                   // 0x0050 (size: 0x1)

}; // Size: 0x51

#endif
