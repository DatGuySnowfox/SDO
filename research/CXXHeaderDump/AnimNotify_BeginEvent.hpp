#ifndef UE4SS_SDK_AnimNotify_BeginEvent_HPP
#define UE4SS_SDK_AnimNotify_BeginEvent_HPP

class UAnimNotify_BeginEvent_C : public UAnimNotifyState
{
    FString EventName;                                                                // 0x0030 (size: 0x10)

    bool Received_NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
    bool Received_NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
}; // Size: 0x40

#endif
