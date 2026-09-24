#ifndef UE4SS_SDK_BTTask_FleeFromPerceivedActors_HPP
#define UE4SS_SDK_BTTask_FleeFromPerceivedActors_HPP

class UBTTask_FleeFromPerceivedActors_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector PerceivedTargetLocation;                                   // 0x00B0 (size: 0x28)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_FleeFromPerceivedActors(int32 EntryPoint);
}; // Size: 0xD8

#endif
