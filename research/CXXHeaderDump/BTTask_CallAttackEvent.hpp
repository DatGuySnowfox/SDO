#ifndef UE4SS_SDK_BTTask_CallAttackEvent_HPP
#define UE4SS_SDK_BTTask_CallAttackEvent_HPP

class UBTTask_CallAttackEvent_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_CallAttackEvent(int32 EntryPoint);
}; // Size: 0xB0

#endif
