#ifndef UE4SS_SDK_BTT_RoamWaitAnim_HPP
#define UE4SS_SDK_BTT_RoamWaitAnim_HPP

class UBTT_RoamWaitAnim_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    class UAnimMontage* Current Animation;                                            // 0x00B8 (size: 0x8)
    FTimerHandle Animation Done Timer;                                                // 0x00C0 (size: 0x8)
    int32 Current Anim Index;                                                         // 0x00C8 (size: 0x4)
    FTimerHandle Roam Wait Timer;                                                     // 0x00D0 (size: 0x8)

    void Next Animation();
    void Animation Done();
    void End Roam Wait();
    void ReceiveAbort(class AActor* OwnerActor);
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_RoamWaitAnim(int32 EntryPoint);
}; // Size: 0xD8

#endif
