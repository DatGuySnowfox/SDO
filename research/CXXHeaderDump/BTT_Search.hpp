#ifndef UE4SS_SDK_BTT_Search_HPP
#define UE4SS_SDK_BTT_Search_HPP

class UBTT_Search_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    bool First Search Done;                                                           // 0x00B8 (size: 0x1)
    FVector Temp End;                                                                 // 0x00C0 (size: 0x18)
    FVector Search Radius Location;                                                   // 0x00D8 (size: 0x18)

    void ReceiveAbortAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void Receive Move Completed Bind(FAIRequestID RequestID, TEnumAsByte<EPathFollowingResult::Type> Result);
    void Move Completed(TEnumAsByte<EPathFollowingResult::Type> Result);
    void Finish Task(bool Success);
    void Unbind Events();
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_Search(int32 EntryPoint);
}; // Size: 0xF0

#endif
