#ifndef UE4SS_SDK_BP_MASTER_ToolRequiredContainer_HPP
#define UE4SS_SDK_BP_MASTER_ToolRequiredContainer_HPP

class ABP_MASTER_ToolRequiredContainer_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02C0 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x02D8 (size: 0x8)
    bool Outline?;                                                                    // 0x02E0 (size: 0x1)
    bool IsHeld;                                                                      // 0x02E1 (size: 0x1)
    TEnumAsByte<Enum_ContainerLootTables::Type> LootTable;                            // 0x02E2 (size: 0x1)
    double DurationToHold;                                                            // 0x02E8 (size: 0x8)
    double ChanceToMakeNoise;                                                         // 0x02F0 (size: 0x8)
    double DefaultChanceToProduceNoise;                                               // 0x02F8 (size: 0x8)
    FText ContainerName;                                                              // 0x0300 (size: 0x18)
    class UJigsawItem_DataAsset_C* ToolNeeded;                                        // 0x0318 (size: 0x8)
    bool Opened;                                                                      // 0x0320 (size: 0x1)
    FBP_MASTER_ToolRequiredContainer_COpenedObject OpenedObject;                      // 0x0328 (size: 0x10)
    void OpenedObject();
    bool ContainerLooted;                                                             // 0x0338 (size: 0x1)
    bool ToolNeeded?;                                                                 // 0x0339 (size: 0x1)
    FTimerHandle OutlineTimer;                                                        // 0x0340 (size: 0x8)
    bool PlayerClose?;                                                                // 0x0348 (size: 0x1)
    FTimerHandle HoldToInteractTimer;                                                 // 0x0350 (size: 0x8)
    class UAudioComponent* SearchSoundComp;                                           // 0x0358 (size: 0x8)
    double RefillTimer;                                                               // 0x0360 (size: 0x8)
    FTransform MagnifierTransform;                                                    // 0x0370 (size: 0x60)
    class UWidgetComponent* W_MagnifierUI;                                            // 0x03D0 (size: 0x8)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x03D8 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x03E8 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x03F8 (size: 0x10)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void GetSettings(double& ChanceToProduceNoise, double& Default_ChanceToProduceNoise, double& DurationToHold);
    void IsHeld?(bool& Held?);
    void JigMP_GetLootWidget(class UWidget*& Result, bool& Valid?);
    void JigMP_OnContainersSwap(class UJSIContainer_C* Container1, class UJSIContainer_C* Container2, bool& Result);
    void JigMP_OnItemDropped(FGuid ItemUID, FGuid FromContainerUID, class AActor* ActorRef, bool& Result);
    void JigMP_OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void JigMP_OnItemAdded(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* FromComponent, bool& Result);
    void JigMP_OnItemMoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, bool& Result);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void JigMP_OnPickupAdded(FRepItemInfo Info, FGuid ItemUID, FGuid ToContainerUID, FName ToContainer, bool& Result);
    void JigMP_OnPickupEquipped(class AActor* ActorRef, FName ToContainerName, FGuid UID, FGuid ToContainerUID, FRepItemInfo Info, bool& Result, class AActor*& OverrideActor);
    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void JigMP_OnItemConsumed(FRepItemInfo ConsumedItem, int32 Amount, int32 Remaining, FRepItemInfo InsideOf, FName FromContainer, FString CustomData, bool CallbackToPlayer?, bool& Result);
    void JigMP_OnUpdateChamberUID(FGuid NewUID, bool& Result);
    void JigMP_OnTwoContainersSwap(FName FromContainer, FName ToContainer, bool& Result);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void FindItem(class AActor* Actor, bool& Found?);
    void TraceToPlayer(bool& HitObject?);
    void SearchSound(bool Spawn?);
    void HoldToInteract();
    void TraceForActor(bool& Ouput);
    void DistanceOutline();
    void NoiseEvent();
    void SetInspected(bool Inspected);
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void UserConstructionScript();
    void ActorPreLoad();
    void ActorSaved();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void ActorPreSave();
    void ActorLoaded();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void PauseInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void Event_Outline();
    void OpenContainer();
    void Event_HoldToInteract();
    void OnExecuteInteractEnded();
    void Bind_Refill();
    void Event_MagnifierUI(bool Spawn);
    void ExecuteUbergraph_BP_MASTER_ToolRequiredContainer(int32 EntryPoint);
    void OpenedObject__DelegateSignature();
}; // Size: 0x408

#endif
