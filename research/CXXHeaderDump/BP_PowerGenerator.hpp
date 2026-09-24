#ifndef UE4SS_SDK_BP_PowerGenerator_HPP
#define UE4SS_SDK_BP_PowerGenerator_HPP

class ABP_PowerGenerator_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C8 (size: 0x8)
    bool GasAdded?;                                                                   // 0x02D0 (size: 0x1)
    bool TurnedOn?;                                                                   // 0x02D1 (size: 0x1)
    TArray<AActor*> Actors;                                                           // 0x02D8 (size: 0x10)
    FTimerHandle ShakeTimer;                                                          // 0x02E8 (size: 0x8)
    bool RequiredItemToPowerOn?;                                                      // 0x02F0 (size: 0x1)
    FText DisplayItemName;                                                            // 0x02F8 (size: 0x10)
    class UJigsawItem_DataAsset_C* RequiredItem;                                      // 0x0308 (size: 0x8)
    bool ConsumeItemOnUse?;                                                           // 0x0310 (size: 0x1)
    bool ItemUsed?;                                                                   // 0x0311 (size: 0x1)

    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<FGameplayTag, FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<FString, FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void Shake();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void PickupBuildFromGround();
    void Event_TurnOn(class AActor* Interactor);
    void Event_AddGas(class AActor* Interactor);
    void SetInteractOption(FGameplayTag Option);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<FGameplayTag, FText> Options);
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_PowerGenerator(int32 EntryPoint);
}; // Size: 0x312

#endif
