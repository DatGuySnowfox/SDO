#ifndef UE4SS_SDK_AI_FindWaypoint_HPP
#define UE4SS_SDK_AI_FindWaypoint_HPP

class UAI_FindWaypoint_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AI_FindWaypoint(int32 EntryPoint);
}; // Size: 0xB0

#endif
