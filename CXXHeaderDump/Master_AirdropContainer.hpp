#ifndef UE4SS_SDK_Master_AirdropContainer_HPP
#define UE4SS_SDK_Master_AirdropContainer_HPP

class AMaster_AirdropContainer_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UWidgetComponent* magnifier;                                                // 0x02A8 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02B0 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Parachute;                                            // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C8 (size: 0x8)
    class AActor* CurrentActor;                                                       // 0x02D0 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x02D8 (size: 0x18)
    bool IsHeld;                                                                      // 0x02F0 (size: 0x1)
    TEnumAsByte<Enum_ContainerLootTables::Type> LootTable;                            // 0x02F1 (size: 0x1)
    double DurationToHold;                                                            // 0x02F8 (size: 0x8)
    bool PlayerClose?;                                                                // 0x0300 (size: 0x1)
    bool Inspected;                                                                   // 0x0301 (size: 0x1)
    FTimerHandle HoldToInteractTimer;                                                 // 0x0308 (size: 0x8)
    FTimerHandle OutlineTimer;                                                        // 0x0310 (size: 0x8)
    double ChanceToProduceNoise;                                                      // 0x0318 (size: 0x8)
    FString Text;                                                                     // 0x0320 (size: 0x10)
    FLinearColor Color;                                                               // 0x0330 (size: 0x10)
    class ABP_Marker_C* Marker;                                                       // 0x0340 (size: 0x8)

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
    void NoiseEvent();
    void HoldToInteract();
    void DistanceOutline();
    void SetInspected(FText InText, bool Inspected?);
    void SetInteractDistance(class AActor* Target);
    void CheckDistanceFromActor();
    void UserConstructionScript();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
    void ReceiveBeginPlay();
    void AirdropNotification(bool NewValue);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void PauseInteract();
    void Event_Outline();
    void OpenContainer();
    void Event_HoldToInteract();
    void OnExecuteInteractEnded();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void ExecuteUbergraph_Master_AirdropContainer(int32 EntryPoint);
}; // Size: 0x348

#endif
