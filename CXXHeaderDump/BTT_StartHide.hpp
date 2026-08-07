#ifndef UE4SS_SDK_BTT_StartHide_HPP
#define UE4SS_SDK_BTT_StartHide_HPP

class UBTT_StartHide_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)

    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_StartHide(int32 EntryPoint);
}; // Size: 0xB8

#endif
