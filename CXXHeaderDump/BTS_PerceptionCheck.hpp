#ifndef UE4SS_SDK_BTS_PerceptionCheck_HPP
#define UE4SS_SDK_BTS_PerceptionCheck_HPP

class UBTS_PerceptionCheck_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)

    void Check If Seen(bool& Seen);
    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTS_PerceptionCheck(int32 EntryPoint);
}; // Size: 0xA8

#endif
