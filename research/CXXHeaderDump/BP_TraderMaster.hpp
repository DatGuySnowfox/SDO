#ifndef UE4SS_SDK_BP_TraderMaster_HPP
#define UE4SS_SDK_BP_TraderMaster_HPP

class ABP_TraderMaster_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* Hair;                                                 // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* Beard;                                                // 0x02B0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02C0 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02C8 (size: 0x8)
    bool SkipSave?;                                                                   // 0x02D0 (size: 0x1)
    FText VendorName;                                                                 // 0x02D8 (size: 0x18)
    TSubclassOf<class UDialogue> Dialogue;                                            // 0x02F0 (size: 0x8)
    class AActor* CurrentActor;                                                       // 0x02F8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0300 (size: 0x18)
    TEnumAsByte<Enum_VendorTypes::Type> VendorTable;                                  // 0x0318 (size: 0x1)
    TArray<class UStaticMesh*> HairMeshes;                                            // 0x0320 (size: 0x10)
    TArray<class UStaticMesh*> BeardMeshes;                                           // 0x0330 (size: 0x10)
    bool CurrentlyInteracting?;                                                       // 0x0340 (size: 0x1)
    double CurrencyAmount;                                                            // 0x0348 (size: 0x8)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
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
    void FacePlayer(class AActor* Target);
    void CheckDistanceFromActor();
    void UserConstructionScript();
    void ActorPreLoad();
    void ActorSaved();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void ActorPreSave();
    void ActorLoaded();
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void BndEvt__BP_MPVendor_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_Reset(int32 Hour);
    void ExecuteUbergraph_BP_TraderMaster(int32 EntryPoint);
}; // Size: 0x350

#endif
