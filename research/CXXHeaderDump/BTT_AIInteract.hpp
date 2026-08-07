#ifndef UE4SS_SDK_BTT_AIInteract_HPP
#define UE4SS_SDK_BTT_AIInteract_HPP

class UBTT_AIInteract_C : public UBTTask_BlueprintBase
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A8 (size: 0x8)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x00B0 (size: 0x8)
    class UBP_SmartAIComponent_C* Move To Actor AI Component;                         // 0x00B8 (size: 0x8)
    double Time;                                                                      // 0x00C0 (size: 0x8)
    FTimerHandle Check Timer;                                                         // 0x00C8 (size: 0x8)
    int32 Current Anim Index;                                                         // 0x00D0 (size: 0x4)
    FS_AIInteractAnim Current Anims;                                                  // 0x00D8 (size: 0x10)
    FTimerHandle Finish Timer;                                                        // 0x00E8 (size: 0x8)

    void Convert To Behaviour(TEnumAsByte<E_StartingAIBehaviours::Type> S Behaviour, TEnumAsByte<E_AIBehaviour::Type>& Behaviour);
    void Next Animation();
    void End AI Interaction();
    void Check If Still Can Interact();
    void ReceiveAbortAI(class AAIController* OwnerController, class APawn* ControlledPawn);
    void Reset AI();
    void Clear AI Interact();
    void ReceiveExecute(class AActor* OwnerActor);
    void ExecuteUbergraph_BTT_AIInteract(int32 EntryPoint);
}; // Size: 0xF0

#endif
