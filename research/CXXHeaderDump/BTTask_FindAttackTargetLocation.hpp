#ifndef UE4SS_SDK_BTTask_FindAttackTargetLocation_HPP
#define UE4SS_SDK_BTTask_FindAttackTargetLocation_HPP

class UBTTask_FindAttackTargetLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Actor;                                                     // 0x00B0 (size: 0x28)
    FName TargetTag;                                                                  // 0x00D8 (size: 0x8)
    class AActor* TargetActor;                                                        // 0x00E0 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_FindAttackTargetLocation(int32 EntryPoint);
}; // Size: 0xE8

#endif
