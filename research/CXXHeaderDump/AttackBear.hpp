#ifndef UE4SS_SDK_AttackBear_HPP
#define UE4SS_SDK_AttackBear_HPP

class UAttackBear_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AttackBear(int32 EntryPoint);
}; // Size: 0xB0

#endif
