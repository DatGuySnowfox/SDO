#ifndef UE4SS_SDK_BTTask_StopCurrentAnimation_HPP
#define UE4SS_SDK_BTTask_StopCurrentAnimation_HPP

class UBTTask_StopCurrentAnimation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_StopCurrentAnimation(int32 EntryPoint);
}; // Size: 0xB0

#endif
