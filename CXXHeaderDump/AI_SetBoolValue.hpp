#ifndef UE4SS_SDK_AI_SetBoolValue_HPP
#define UE4SS_SDK_AI_SetBoolValue_HPP

class UAI_SetBoolValue_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector BB;                                                        // 0x00B0 (size: 0x28)
    bool Value;                                                                       // 0x00D8 (size: 0x1)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AI_SetBoolValue(int32 EntryPoint);
}; // Size: 0xD9

#endif
