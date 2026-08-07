#ifndef UE4SS_SDK_BTT_FindWayPoint_HPP
#define UE4SS_SDK_BTT_FindWayPoint_HPP

class UBTT_FindWayPoint_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    int32 Current Way Point;                                                          // 0x00B8 (size: 0x4)
    class ABP_MasterWayPoint_C* Way Point;                                            // 0x00C0 (size: 0x8)
    TArray<class ABP_MasterWayPoint_C*> Random Points;                                // 0x00C8 (size: 0x10)
    bool Backward;                                                                    // 0x00D8 (size: 0x1)

    void ReceiveAbort(class AActor* OwnerActor);
    void ReceiveAbortAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void AIKilled(class ACharacter* AI Character);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_FindWayPoint(int32 EntryPoint);
}; // Size: 0xD9

#endif
