#ifndef UE4SS_SDK_AnimNotify_ZombieAttackRightArm_HPP
#define UE4SS_SDK_AnimNotify_ZombieAttackRightArm_HPP

class UAnimNotify_ZombieAttackRightArm_C : public UAnimNotifyState
{

    bool Received_NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
    bool Received_NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
}; // Size: 0x30

#endif
