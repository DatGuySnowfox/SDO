#ifndef UE4SS_SDK_BTTask_SetBehaviour_HPP
#define UE4SS_SDK_BTTask_SetBehaviour_HPP

class UBTTask_SetBehaviour_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    TEnumAsByte<Enum_AIBehviour::Type> Behaviour;                                     // 0x00B0 (size: 0x1)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_SetBehaviour(int32 EntryPoint);
}; // Size: 0xB1

#endif
