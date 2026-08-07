#ifndef UE4SS_SDK_BP_HarvestableObject_ScrapMetal_HPP
#define UE4SS_SDK_BP_HarvestableObject_ScrapMetal_HPP

class ABP_HarvestableObject_ScrapMetal_C : public ABP_HarvestableObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D8 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02E0 (size: 0x8)
    class AActor* Current Actor;                                                      // 0x02E8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02F0 (size: 0x18)
    bool IsHeld;                                                                      // 0x0308 (size: 0x1)
    bool Salvaged?;                                                                   // 0x0309 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void MC_PlaySound(FVector Location);
    void Svr_PlaySound(FVector Location);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void Event_HoldTimer();
    void Event_StopTimer();
    void Event_UpdateTimer();
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_HarvestableObject_ScrapMetal(int32 EntryPoint);
}; // Size: 0x30A

#endif
