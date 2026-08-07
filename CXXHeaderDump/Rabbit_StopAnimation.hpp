#ifndef UE4SS_SDK_Rabbit_StopAnimation_HPP
#define UE4SS_SDK_Rabbit_StopAnimation_HPP

class URabbit_StopAnimation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_Rabbit_StopAnimation(int32 EntryPoint);
}; // Size: 0xB0

#endif
