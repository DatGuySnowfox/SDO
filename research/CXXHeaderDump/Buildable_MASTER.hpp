#ifndef UE4SS_SDK_Buildable_MASTER_HPP
#define UE4SS_SDK_Buildable_MASTER_HPP

class ABuildable_MASTER_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBoxComponent* NavObstacleBox;                                              // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* DummySnapTester;                                      // 0x02A8 (size: 0x8)
    class UChildActorComponent* SnapPoints;                                           // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02C8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02D0 (size: 0x8)
    class UJigsawItem_DataAsset_C* PickupDataAsset;                                   // 0x02D8 (size: 0x8)
    TMap<class FGameplayTag, class FText> InteractOptions;                            // 0x02E0 (size: 0x50)
    TMap<class FName, class FText> StatOptions;                                       // 0x0330 (size: 0x50)
    double Health;                                                                    // 0x0380 (size: 0x8)
    bool RequiresPower?;                                                              // 0x0388 (size: 0x1)
    class AActor* CurrentActor;                                                       // 0x0390 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0398 (size: 0x18)
    bool IsHeld;                                                                      // 0x03B0 (size: 0x1)
    double OffSetDespawn;                                                             // 0x03B8 (size: 0x8)
    TArray<class TSubclassOf<AActor>> CraftingMaterials;                              // 0x03C0 (size: 0x10)
    bool PoweredOn?;                                                                  // 0x03D0 (size: 0x1)
    FBuildable_MASTER_CPoweredOn PoweredOn;                                           // 0x03D8 (size: 0x10)
    void PoweredOn();
    FBuildable_MASTER_CPoweredOff PoweredOff;                                         // 0x03E8 (size: 0x10)
    void PoweredOff();
    bool ShowGenerator;                                                               // 0x03F8 (size: 0x1)
    FBuildable_MASTER_CInteractWithObject InteractWithObject;                         // 0x0400 (size: 0x10)
    void InteractWithObject(class AActor* NewParam);
    bool SpecialDestroy?;                                                             // 0x0410 (size: 0x1)
    FBuildable_MASTER_CBeginInteract BeginInteract;                                   // 0x0418 (size: 0x10)
    void BeginInteract();
    double PickupTimer;                                                               // 0x0428 (size: 0x8)

    void GetBuildType(TEnumAsByte<Enum_BuildingSnapTypes::Type>& Type);
    void Requires Power?(bool& Required?);
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
    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void ReplacingObject();
    void RemoveFoliageComp(float Radius);
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void OnRep_PoweredOn?();
    void SpawnCraftingMaterials();
    bool GetAllItemsInContainer();
    void SpawnItemOnDestroy(TSubclassOf<class AActor> Class);
    void UpdateUIHealth(double Health);
    void OnDestroy(class AActor* Actor);
    void RemoveModularBuilds();
    void OverrideBuildName(FText Name);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void PickupBuildFromGround();
    void ActorLoaded();
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void Power On(class AActor* Actor);
    void Power Off(class AActor* Actor);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void Pickup(bool Quick?);
    void OnInteractBuildable(class AActor* Actor);
    void Event_UpdateTimer();
    void Event_StopTimer();
    void Event_HoldTimer();
    void MC_Destroy(class AActor* Actor);
    void MC_OnInteractBuildable(class AActor* Actor);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractEnded();
    void SetInteractOption(FGameplayTag Option);
    void Svr_Spawn();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void ExecuteUbergraph_Buildable_MASTER(int32 EntryPoint);
    void BeginInteract__DelegateSignature();
    void InteractWithObject__DelegateSignature(class AActor* NewParam);
    void PoweredOff__DelegateSignature();
    void PoweredOn__DelegateSignature();
}; // Size: 0x430

#endif
