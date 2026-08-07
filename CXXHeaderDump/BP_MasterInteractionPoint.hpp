#ifndef UE4SS_SDK_BP_MasterInteractionPoint_HPP
#define UE4SS_SDK_BP_MasterInteractionPoint_HPP

class ABP_MasterInteractionPoint_C : public AActor
{
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x0298 (size: 0x8)
    class UBillboardComponent* Target Location;                                       // 0x02A0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02A8 (size: 0x8)
    class UCapsuleComponent* Capsule;                                                 // 0x02B0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02B8 (size: 0x8)
    TEnumAsByte<E_OrderOptions::Type> Animation Order;                                // 0x02C0 (size: 0x1)
    TArray<class UAnimMontage*> Interact Animations;                                  // 0x02C8 (size: 0x10)
    class UAnimMontage* Start Animation;                                              // 0x02D8 (size: 0x8)
    class UAnimMontage* End Animation;                                                // 0x02E0 (size: 0x8)
    bool Use Target Location;                                                         // 0x02E8 (size: 0x1)
    bool Loop Animations;                                                             // 0x02E9 (size: 0x1)
    double Animation location Speed;                                                  // 0x02F0 (size: 0x8)
    double Interact Time;                                                             // 0x02F8 (size: 0x8)
    double Interact Time Variation;                                                   // 0x0300 (size: 0x8)
    bool Occupied;                                                                    // 0x0308 (size: 0x1)
    FVector Inteaction Face Location;                                                 // 0x0310 (size: 0x18)
    class UAnimSequence* Debug Animation;                                             // 0x0328 (size: 0x8)
    bool Debug Target Location;                                                       // 0x0330 (size: 0x1)

    void UserConstructionScript();
}; // Size: 0x331

#endif
