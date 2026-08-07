#ifndef UE4SS_SDK_BP_Siren_HPP
#define UE4SS_SDK_BP_Siren_HPP

class ABP_Siren_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B8 (size: 0x8)
    bool Playing;                                                                     // 0x02C0 (size: 0x1)
    FTimerHandle SirenPlayingTimer;                                                   // 0x02C8 (size: 0x8)
    FText Name;                                                                       // 0x02D0 (size: 0x18)
    double RandomTimeMin;                                                             // 0x02E8 (size: 0x8)
    double RandomTimeMax;                                                             // 0x02F0 (size: 0x8)
    FTimerHandle RandomSirenTimer;                                                    // 0x02F8 (size: 0x8)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void SirenPlayTimer();
    void Event_Siren();
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void SirenNoise();
    void DeactivateSiren();
    void ActivateSiren();
    void ExecuteUbergraph_BP_Siren(int32 EntryPoint);
}; // Size: 0x300

#endif
