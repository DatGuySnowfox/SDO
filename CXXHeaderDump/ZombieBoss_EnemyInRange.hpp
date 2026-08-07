#ifndef UE4SS_SDK_ZombieBoss_EnemyInRange_HPP
#define UE4SS_SDK_ZombieBoss_EnemyInRange_HPP

class UZombieBoss_EnemyInRange_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    FBlackboardKeySelector InRange?;                                                  // 0x00A0 (size: 0x28)
    FBlackboardKeySelector AttackTarget;                                              // 0x00C8 (size: 0x28)

    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_ZombieBoss_EnemyInRange(int32 EntryPoint);
}; // Size: 0xF0

#endif
