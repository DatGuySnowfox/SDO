#ifndef UE4SS_SDK_BTS_RangeAttackLocation_HPP
#define UE4SS_SDK_BTS_RangeAttackLocation_HPP

class UBTS_RangeAttackLocation_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI;                                           // 0x00A0 (size: 0x8)
    int32 Animation Index;                                                            // 0x00A8 (size: 0x4)
    class UAnimMontage* Attack Montage;                                               // 0x00B0 (size: 0x8)
    bool Use Anim Time;                                                               // 0x00B8 (size: 0x1)
    FVector Last Seen;                                                                // 0x00C0 (size: 0x18)
    double Range Attack Time;                                                         // 0x00D8 (size: 0x8)
    bool In Attack;                                                                   // 0x00E0 (size: 0x1)
    FTimerHandle Search Timer;                                                        // 0x00E8 (size: 0x8)
    double AnimAttackTime;                                                            // 0x00F0 (size: 0x8)

    void Closest Attack Target(class AActor*& Closest Attack Target);
    void Range Attack Frequancy(bool& Use Anim Time, double& Range Attack Frequency);
    void Get Range Attack Montage(class UAnimMontage*& Montage);
    void Range Attack();
    void ReceiveTickAI(class AAIController* OwnerController, class APawn* ControlledPawn, float DeltaSeconds);
    void LOS Timer();
    void Search Timer Events();
    void ExecuteUbergraph_BTS_RangeAttackLocation(int32 EntryPoint);
}; // Size: 0xF8

#endif
