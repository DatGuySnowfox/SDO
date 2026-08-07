#ifndef UE4SS_SDK_BTT_Interact_HPP
#define UE4SS_SDK_BTT_Interact_HPP

class UBTT_Interact_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    TArray<class UAnimMontage*> Interaction Animations;                               // 0x00B0 (size: 0x10)
    int32 Current Anim Index;                                                         // 0x00C0 (size: 0x4)
    class UAnimMontage* Current Animation;                                            // 0x00C8 (size: 0x8)
    FTimerHandle End Timer;                                                           // 0x00D0 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00D8 (size: 0x8)
    FTimerHandle End Animation Timer;                                                 // 0x00E0 (size: 0x8)
    FTimerHandle Start Animation Timer;                                               // 0x00E8 (size: 0x8)

    void Next Animation();
    void End Animation Done();
    void Start Animation End();
    void ReceiveAbort(class AActor* OwnerActor);
    void End Interact();
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_Interact(int32 EntryPoint);
}; // Size: 0xF0

#endif
