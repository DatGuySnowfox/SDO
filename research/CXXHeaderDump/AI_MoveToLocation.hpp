#ifndef UE4SS_SDK_AI_MoveToLocation_HPP
#define UE4SS_SDK_AI_MoveToLocation_HPP

class UAI_MoveToLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Location;                                                  // 0x00B0 (size: 0x28)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AI_MoveToLocation(int32 EntryPoint);
}; // Size: 0xD8

#endif
