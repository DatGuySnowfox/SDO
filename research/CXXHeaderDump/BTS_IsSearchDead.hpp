#ifndef UE4SS_SDK_BTS_IsSearchDead_HPP
#define UE4SS_SDK_BTS_IsSearchDead_HPP

class UBTS_IsSearchDead_C : public UBTService_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0098 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00A0 (size: 0x8)

    void ReceiveTick(class AActor* OwnerActor, float DeltaSeconds);
    void End Search();
    void ReceiveActivationAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ExecuteUbergraph_BTS_IsSearchDead(int32 EntryPoint);
}; // Size: 0xA8

#endif
