#ifndef UE4SS_SDK_BP_AnimalDeer_HPP
#define UE4SS_SDK_BP_AnimalDeer_HPP

class ABP_AnimalDeer_C : public ABP_Animal_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x06A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x06A8 (size: 0x8)
    class UAudioComponent* Deer_Noises;                                               // 0x06B0 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x06B8 (size: 0x8)
    class UNavigationInvokerComponent* NavigationInvoker;                             // 0x06C0 (size: 0x8)
    class UStaticMeshComponent* Antlers;                                              // 0x06C8 (size: 0x8)
    double Health;                                                                    // 0x06D0 (size: 0x8)
    bool IsDead?;                                                                     // 0x06D8 (size: 0x1)
    class AActor* CurrentActor;                                                       // 0x06E0 (size: 0x8)
    FText AIName;                                                                     // 0x06E8 (size: 0x18)
    FVector InteractingActorLoc;                                                      // 0x0700 (size: 0x18)
    bool XpPopUpEnabled?;                                                             // 0x0718 (size: 0x1)
    class UWidgetComponent* Marker;                                                   // 0x0720 (size: 0x8)
    bool CanAddMarker?;                                                               // 0x0728 (size: 0x1)
    FTimerHandle MarkerTimer;                                                         // 0x0730 (size: 0x8)
    bool Is Burning?;                                                                 // 0x0738 (size: 0x1)
    class UParticleSystemComponent* BurningEffect;                                    // 0x0740 (size: 0x8)
    FTimerHandle BurningTimer;                                                        // 0x0748 (size: 0x8)
    bool Looted;                                                                      // 0x0750 (size: 0x1)

    void IsAIBurning?(bool& IsBurning?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
    void CanStompAI?(bool& Stomp?);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void JigMP_OnPickupAdded(FRepItemInfo Info, FGuid ItemUID, FGuid ToContainerUID, FName ToContainer, bool& Result);
    void JigMP_OnPickupEquipped(class AActor* ActorRef, FName ToContainerName, FGuid UID, FGuid ToContainerUID, FRepItemInfo Info, bool& Result, class AActor*& OverrideActor);
    void JigMP_OnRequestDropItem(FRepItemInfo ItemInfo, FName ContainerName, class AActor*& ActorRef);
    void JigMP_OnItemConsumed(FRepItemInfo ConsumedItem, int32 Amount, int32 Remaining, FRepItemInfo InsideOf, FName FromContainer, FString CustomData, bool CallbackToPlayer?, bool& Result);
    void JigMP_OnUpdateChamberUID(FGuid NewUID, bool& Result);
    void JigMP_OnTwoContainersSwap(FName FromContainer, FName ToContainer, bool& Result);
    void JigMP_OnItemMoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, bool& Result);
    void JigMP_OnItemAdded(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* FromComponent, bool& Result);
    void JigMP_OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void JigMP_OnItemDropped(FGuid ItemUID, FGuid FromContainerUID, class AActor* ActorRef, bool& Result);
    void JigMP_OnContainersSwap(class UJSIContainer_C* Container1, class UJSIContainer_C* Container2, bool& Result);
    void JigMP_GetLootWidget(class UWidget*& Result, bool& Valid?);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void PlayMontage();
    void Change Speed(double Speed);
    void GetMoveLocation(FVector& RandomLocation);
    void F_IsDead?();
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void SetMeshesAndMaterial();
    void UserConstructionScript();
    void HitByVehicle(double Speed, FVector Direction);
    void OverrideName(FString Name);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void AddMarker(class UWidgetComponent* Marker);
    void Event_Marker();
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void Event_Fire();
    void StopBurning();
    void Despawn();
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void OnExecuteInteractEnded();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_XP(bool NewValue);
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_AnimalDeer(int32 EntryPoint);
}; // Size: 0x751

#endif
