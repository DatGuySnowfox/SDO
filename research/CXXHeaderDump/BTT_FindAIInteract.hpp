#ifndef UE4SS_SDK_BTT_FindAIInteract_HPP
#define UE4SS_SDK_BTT_FindAIInteract_HPP

class UBTT_FindAIInteract_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    class UBP_SmartAIComponent_C* Array AI Component;                                 // 0x00B8 (size: 0x8)
    TArray<class AActor*> Pawns In Range;                                             // 0x00C0 (size: 0x10)

    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_FindAIInteract(int32 EntryPoint);
}; // Size: 0xD0

#endif
