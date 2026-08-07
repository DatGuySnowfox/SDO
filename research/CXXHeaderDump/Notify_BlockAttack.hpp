#ifndef UE4SS_SDK_Notify_BlockAttack_HPP
#define UE4SS_SDK_Notify_BlockAttack_HPP

class UNotify_BlockAttack_C : public UAnimNotify
{
    bool Directional Block;                                                           // 0x0038 (size: 0x1)
    TEnumAsByte<E_BlockType::Type> Attack Type;                                       // 0x0039 (size: 0x1)

    bool Received_Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
}; // Size: 0x3A

#endif
