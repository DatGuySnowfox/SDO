#ifndef UE4SS_SDK_BTT_RangeMoveTo_HPP
#define UE4SS_SDK_BTT_RangeMoveTo_HPP

class UBTT_RangeMoveTo_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FVector Attack Target Location;                                                   // 0x00B0 (size: 0x18)
    FVector EQS Result;                                                               // 0x00C8 (size: 0x18)
    class UAITask_MoveTo* Last Seen Task;                                             // 0x00E0 (size: 0x8)
    class AAIController* Owner Controller;                                            // 0x00E8 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI;                                           // 0x00F0 (size: 0x8)
    class AActor* Array Element L;                                                    // 0x00F8 (size: 0x8)
    double SamePositionCheck;                                                         // 0x0100 (size: 0x8)

    void FindTargets(class AActor*& Closest Attack Target);
    bool RangePointCheck();
    void OnMoveFinished_147596BE4CB673FA8562839726C4DD21(TEnumAsByte<EPathFollowingResult::Type> Result, class AAIController* AIController);
    void OnRequestFailed_147596BE4CB673FA8562839726C4DD21();
    void OnMoveFinished_751C6ACC423B5631FA126CBB0CF58B1C(TEnumAsByte<EPathFollowingResult::Type> Result, class AAIController* AIController);
    void OnRequestFailed_751C6ACC423B5631FA126CBB0CF58B1C();
    void Run Range Move EQS();
    void Range Move EQS Done(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, TEnumAsByte<EEnvQueryStatus::Type> QueryStatus);
    void Run Cover Move EQS();
    void Cover Move EQS Done(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, TEnumAsByte<EEnvQueryStatus::Type> QueryStatus);
    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTT_RangeMoveTo(int32 EntryPoint);
}; // Size: 0x108

#endif
