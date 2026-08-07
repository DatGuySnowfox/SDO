#ifndef UE4SS_SDK_BTS_CanPath_HPP
#define UE4SS_SDK_BTS_CanPath_HPP

class UBTS_CanPath_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)
    class UBP_SmartAIComponent_C* Smart AI;                                           // 0x00A8 (size: 0x8)

    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ReceiveTickAI(class AAIController* OwnerController, class APawn* ControlledPawn, float DeltaSeconds);
    void ExecuteUbergraph_BTS_CanPath(int32 EntryPoint);
}; // Size: 0xB0

#endif
