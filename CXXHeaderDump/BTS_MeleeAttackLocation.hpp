#ifndef UE4SS_SDK_BTS_MeleeAttackLocation_HPP
#define UE4SS_SDK_BTS_MeleeAttackLocation_HPP

class UBTS_MeleeAttackLocation_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)
    FTimerHandle Search Timer;                                                        // 0x00A8 (size: 0x8)
    class AActor* Last Attack Target;                                                 // 0x00B0 (size: 0x8)
    bool Hit Attack Target;                                                           // 0x00B8 (size: 0x1)
    int32 Animation Index;                                                            // 0x00BC (size: 0x4)
    FTimerHandle Moving Melee Timer;                                                  // 0x00C0 (size: 0x8)
    class UAnimMontage* AttackMontage;                                                // 0x00C8 (size: 0x8)
    bool Use Anim Time;                                                               // 0x00D0 (size: 0x1)
    double Timer Length;                                                              // 0x00D8 (size: 0x8)

    void Melee Attack Frequancy(bool& Use Anim Time, double& Range Attack Frequency);
    void Get Lower Melee Montage(class UAnimMontage*& Montage);
    void Get Melee Montage(class UAnimMontage*& Montage);
    void Moving Melee Attack();
    void Lower Melee Hit Check();
    void Start Moving Melee Attack();
    void Search Timer Event();
    void ReceiveTickAI(class AAIController* OwnerController, class APawn* ControlledPawn, float DeltaSeconds);
    void ExecuteUbergraph_BTS_MeleeAttackLocation(int32 EntryPoint);
}; // Size: 0xE0

#endif
