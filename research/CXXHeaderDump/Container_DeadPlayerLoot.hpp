#ifndef UE4SS_SDK_Container_DeadPlayerLoot_HPP
#define UE4SS_SDK_Container_DeadPlayerLoot_HPP

class AContainer_DeadPlayerLoot_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UWidgetComponent* magnifier;                                                // 0x02A8 (size: 0x8)
    class UWidgetComponent* DeathWidget;                                              // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C8 (size: 0x8)
    FText ContainerName;                                                              // 0x02D0 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x02E8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02F0 (size: 0x18)
    FContainer_DeadPlayerLoot_CSetCustomDepth_True SetCustomDepth_True;               // 0x0308 (size: 0x10)
    void SetCustomDepth_True();
    FContainer_DeadPlayerLoot_CSetCustomDepth_False SetCustomDepth_False;             // 0x0318 (size: 0x10)
    void SetCustomDepth_False();
    FTimerHandle OutlineTimer;                                                        // 0x0328 (size: 0x8)
    bool PlayerClose?;                                                                // 0x0330 (size: 0x1)
    class ABP_Marker_C* DeathMarker;                                                  // 0x0338 (size: 0x8)
    bool Looted?;                                                                     // 0x0340 (size: 0x1)

    void JigMP_GetLootWidget(class UWidget*& Result, bool& Valid?);
    void JigMP_OnContainersSwap(class UJSIContainer_C* Container1, class UJSIContainer_C* Container2, bool& Result);
    void JigMP_OnItemDropped(FGuid ItemUID, FGuid FromContainerUID, class AActor* ActorRef, bool& Result);
    void JigMP_OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void JigMP_OnItemAdded(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* FromComponent, bool& Result);
    void JigMP_OnItemMoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, bool& Result);
    void JigMP_OnTwoContainersSwap(FName FromContainer, FName ToContainer, bool& Result);
    void JigMP_OnUpdateChamberUID(FGuid NewUID, bool& Result);
    void JigMP_OnItemConsumed(FRepItemInfo ConsumedItem, int32 Amount, int32 Remaining, FRepItemInfo InsideOf, FName FromContainer, FString CustomData, bool CallbackToPlayer?, bool& Result);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void JigMP_OnPickupAdded(FRepItemInfo Info, FGuid ItemUID, FGuid ToContainerUID, FName ToContainer, bool& Result);
    void JigMP_OnPickupEquipped(class AActor* ActorRef, FName ToContainerName, FGuid UID, FGuid ToContainerUID, FRepItemInfo Info, bool& Result, class AActor*& OverrideActor);
    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void JigCanInteract(bool& Result);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void TraceForFloor();
    void DistanceOutline();
    void SetInspected(FText InText);
    void SetInteractDistance(class AActor* Target);
    void CheckDistanceFromActor();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Event_Outline();
    void OnExecuteInteractEnded();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Container_DeadPlayerLoot(int32 EntryPoint);
    void SetCustomDepth_False__DelegateSignature();
    void SetCustomDepth_True__DelegateSignature();
}; // Size: 0x341

#endif
