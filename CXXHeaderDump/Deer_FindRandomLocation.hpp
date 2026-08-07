#ifndef UE4SS_SDK_Deer_FindRandomLocation_HPP
#define UE4SS_SDK_Deer_FindRandomLocation_HPP

class UDeer_FindRandomLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    FBlackboardKeySelector Vector;                                                    // 0x00B0 (size: 0x28)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_Deer_FindRandomLocation(int32 EntryPoint);
}; // Size: 0xD8

#endif
