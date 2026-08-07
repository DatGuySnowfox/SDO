#ifndef UE4SS_SDK_ZombieBoss_Chase_HPP
#define UE4SS_SDK_ZombieBoss_Chase_HPP

class UZombieBoss_Chase_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Target;                                                    // 0x00B0 (size: 0x28)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_ZombieBoss_Chase(int32 EntryPoint);
}; // Size: 0xD8

#endif
