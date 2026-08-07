#ifndef UE4SS_SDK_BTT_BeginFlee_HPP
#define UE4SS_SDK_BTT_BeginFlee_HPP

class UBTT_BeginFlee_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    bool First Flee EQS;                                                              // 0x00B8 (size: 0x1)
    class UEnvQueryInstanceBlueprintWrapper* Flee EQS;                                // 0x00C0 (size: 0x8)
    FVector Last Location;                                                            // 0x00C8 (size: 0x18)

    void SortFleeArray(TArray<class AActor*>& Actors);
    void Check If Seen(bool& Seen, class AActor*& Actor);
    void EQS Done(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, TEnumAsByte<EEnvQueryStatus::Type> QueryStatus);
    void ReceiveAbortAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_BeginFlee(int32 EntryPoint);
}; // Size: 0xE0

#endif
