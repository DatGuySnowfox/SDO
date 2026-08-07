#ifndef UE4SS_SDK_BTT_MoveToTarget_HPP
#define UE4SS_SDK_BTT_MoveToTarget_HPP

class UBTT_MoveToTarget_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Blackboard;                                                // 0x00B0 (size: 0x28)
    TEnumAsByte<E_Radius::Type> Acceptance Radius;                                    // 0x00D8 (size: 0x1)
    class APawn* AI;                                                                  // 0x00E0 (size: 0x8)
    class UBP_SmartAIComponent_C* SmartAI;                                            // 0x00E8 (size: 0x8)
    double DefaultRadius;                                                             // 0x00F0 (size: 0x8)
    class AActor* LastMoveToActor;                                                    // 0x00F8 (size: 0x8)

    void OnFail_47035ED04D354CCEC9F56C8A50D4A3DC(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnSuccess_47035ED04D354CCEC9F56C8A50D4A3DC(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnFail_A17F5E824072E2CCDFD139900D14DFDE(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void OnSuccess_A17F5E824072E2CCDFD139900D14DFDE(TEnumAsByte<EPathFollowingResult::Type> MovementResult);
    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void Move();
    void MoveToCheck();
    void ExecuteUbergraph_BTT_MoveToTarget(int32 EntryPoint);
}; // Size: 0x100

#endif
