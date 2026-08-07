#ifndef UE4SS_SDK_BTT_StrafeLocation_HPP
#define UE4SS_SDK_BTT_StrafeLocation_HPP

class UBTT_StrafeLocation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI;                                           // 0x00B0 (size: 0x8)
    class APawn* Controlled Pawn;                                                     // 0x00B8 (size: 0x8)
    class AAIController* Owner Controller;                                            // 0x00C0 (size: 0x8)

    void ReceiveExecuteAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTT_StrafeLocation(int32 EntryPoint);
}; // Size: 0xC8

#endif
