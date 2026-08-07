#ifndef UE4SS_SDK_ZombieBoss_Attack_HPP
#define UE4SS_SDK_ZombieBoss_Attack_HPP

class UZombieBoss_Attack_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_ZombieBoss_Attack(int32 EntryPoint);
}; // Size: 0xB0

#endif
