#ifndef UE4SS_SDK_AI_FindRandomLocation_HPP
#define UE4SS_SDK_AI_FindRandomLocation_HPP

class UAI_FindRandomLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Vector;                                                    // 0x00B0 (size: 0x28)
    float Radius;                                                                     // 0x00D8 (size: 0x4)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_AI_FindRandomLocation(int32 EntryPoint);
}; // Size: 0xDC

#endif
