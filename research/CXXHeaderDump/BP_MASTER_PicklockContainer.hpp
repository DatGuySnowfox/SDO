#ifndef UE4SS_SDK_BP_MASTER_PicklockContainer_HPP
#define UE4SS_SDK_BP_MASTER_PicklockContainer_HPP

class ABP_MASTER_PicklockContainer_C : public ABP_LockPickObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02C8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02D0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02D8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02E0 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x02F8 (size: 0x8)
    TEnumAsByte<Enum_ContainerLootTables::Type> LootTable;                            // 0x0300 (size: 0x1)
    FText ContainerName;                                                              // 0x0308 (size: 0x18)
    FBP_MASTER_PicklockContainer_COpenedObject OpenedObject;                          // 0x0320 (size: 0x10)
    void OpenedObject();
    FTimerHandle OutlineTimer;                                                        // 0x0330 (size: 0x8)
    bool PlayerClose?;                                                                // 0x0338 (size: 0x1)
    bool ContainerLooted;                                                             // 0x0339 (size: 0x1)
    double DurationToHold;                                                            // 0x0340 (size: 0x8)
    double ChanceToBreakPicklock;                                                     // 0x0348 (size: 0x8)
    double DefaultChanceToBreakPicklock;                                              // 0x0350 (size: 0x8)
    double RefillTimer;                                                               // 0x0358 (size: 0x8)
    class UWidgetComponent* W_MagnifierUI;                                            // 0x0360 (size: 0x8)
    FTransform MagnifierTransform;                                                    // 0x0370 (size: 0x60)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x03D0 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x03E0 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x03F0 (size: 0x10)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
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
    void TraceToPlayer(bool& HitObject?);
    void TraceForActor(bool& Ouput);
    void DistanceOutline();
    void AdvancedLockCheck(bool& Check);
    void SetInspected(bool Inspected?);
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void UserConstructionScript();
    void ActorPreLoad();
    void ActorSaved();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void LockpickSuccessful();
    void ActorPreSave();
    void ActorLoaded();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void Event_Outline();
    void OpenContainer();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Bind_Refill();
    void Event_MagnifierUI(bool Spawn);
    void ExecuteUbergraph_BP_MASTER_PicklockContainer(int32 EntryPoint);
    void OpenedObject__DelegateSignature();
}; // Size: 0x400

#endif
