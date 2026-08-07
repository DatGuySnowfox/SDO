#ifndef UE4SS_SDK_Buildable_DecontaminationShower_HPP
#define UE4SS_SDK_Buildable_DecontaminationShower_HPP

class ABuildable_DecontaminationShower_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UStaticMeshComponent* Light2;                                               // 0x0438 (size: 0x8)
    class UStaticMeshComponent* Light;                                                // 0x0440 (size: 0x8)
    class UParticleSystemComponent* FX_Water2;                                        // 0x0448 (size: 0x8)
    class UParticleSystemComponent* FX_Water;                                         // 0x0450 (size: 0x8)
    class USceneComponent* Trace;                                                     // 0x0458 (size: 0x8)
    double Cooldown;                                                                  // 0x0460 (size: 0x8)
    bool CooldownRunning?;                                                            // 0x0468 (size: 0x1)

    void OnRep_CooldownRunning?();
    void DecontaminatePlayer();
    void ReceiveBeginPlay();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnInteractBuildable(class AActor* Actor);
    void Event_Cooldown();
    void On();
    void Off();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_DecontaminationShower(int32 EntryPoint);
}; // Size: 0x469

#endif
