#ifndef UE4SS_SDK_BTTask_MoveToActorLocation_HPP
#define UE4SS_SDK_BTTask_MoveToActorLocation_HPP

class UBTTask_MoveToActorLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Target;                                                    // 0x00B0 (size: 0x28)
    float AcceptanceRadius;                                                           // 0x00D8 (size: 0x4)
    bool UsePathfinding;                                                              // 0x00DC (size: 0x1)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_MoveToActorLocation(int32 EntryPoint);
}; // Size: 0xDD

#endif
