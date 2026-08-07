#ifndef UE4SS_SDK_BTT_EndFlee_HPP
#define UE4SS_SDK_BTT_EndFlee_HPP

class UBTT_EndFlee_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)

    void Check If Seen(bool& Seen);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_EndFlee(int32 EntryPoint);
}; // Size: 0xB8

#endif
