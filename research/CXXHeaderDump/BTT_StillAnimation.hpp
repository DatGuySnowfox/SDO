#ifndef UE4SS_SDK_BTT_StillAnimation_HPP
#define UE4SS_SDK_BTT_StillAnimation_HPP

class UBTT_StillAnimation_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    class UAnimMontage* Current Anim;                                                 // 0x00B8 (size: 0x8)
    int32 Current Anim Index;                                                         // 0x00C0 (size: 0x4)

    void Next Animation();
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_StillAnimation(int32 EntryPoint);
}; // Size: 0xC4

#endif
