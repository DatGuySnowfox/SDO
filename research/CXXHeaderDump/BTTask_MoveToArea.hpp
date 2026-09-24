#ifndef UE4SS_SDK_BTTask_MoveToArea_HPP
#define UE4SS_SDK_BTTask_MoveToArea_HPP

class UBTTask_MoveToArea_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Area;                                                      // 0x00B0 (size: 0x28)
    float AreaRadius;                                                                 // 0x00D8 (size: 0x4)
    bool UsePathfinding;                                                              // 0x00DC (size: 0x1)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTTask_MoveToArea(int32 EntryPoint);
}; // Size: 0xDD

#endif
