#ifndef UE4SS_SDK_AI_MoveToSpecificLoc_HPP
#define UE4SS_SDK_AI_MoveToSpecificLoc_HPP

class UAI_MoveToSpecificLoc_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Location;                                                  // 0x00B0 (size: 0x28)

    void GetDistance&Direction(FVector TargetLoc, FVector CurrentLoc, double& Distance, FVector& Direction);
    void GetTargetLoc(FVector Target, class APawn* Controlled Pawn, FVector& RandomLoc);
    void OnFail_23691D184123CB040E4C31A41B25C185(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnSuccess_23691D184123CB040E4C31A41B25C185(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnFail_413338964241E2AC5210E1B19DC04362(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnSuccess_413338964241E2AC5210E1B19DC04362(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AI_MoveToSpecificLoc(int32 EntryPoint);
}; // Size: 0xD8

#endif
