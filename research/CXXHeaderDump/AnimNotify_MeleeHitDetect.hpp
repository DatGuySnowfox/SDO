#ifndef UE4SS_SDK_AnimNotify_MeleeHitDetect_HPP
#define UE4SS_SDK_AnimNotify_MeleeHitDetect_HPP

class UAnimNotify_MeleeHitDetect_C : public UAnimNotifyState
{
    bool PowerAttack?;                                                                // 0x0030 (size: 0x1)

    bool Received_NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
    bool Received_NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
}; // Size: 0x31

#endif
