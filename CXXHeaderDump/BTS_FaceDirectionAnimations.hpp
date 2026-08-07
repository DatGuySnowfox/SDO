#ifndef UE4SS_SDK_BTS_FaceDirectionAnimations_HPP
#define UE4SS_SDK_BTS_FaceDirectionAnimations_HPP

class UBTS_FaceDirectionAnimations_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)
    FTimerHandle Animation Timer;                                                     // 0x00A8 (size: 0x8)
    int32 Current Anim Index;                                                         // 0x00B0 (size: 0x4)
    class UAnimMontage* Current Animation;                                            // 0x00B8 (size: 0x8)
    FTimerHandle Animation Done Timer;                                                // 0x00C0 (size: 0x8)

    void Next Animation();
    void Animation Done();
    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void Reset();
    void ExecuteUbergraph_BTS_FaceDirectionAnimations(int32 EntryPoint);
}; // Size: 0xC8

#endif
