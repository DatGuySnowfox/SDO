#ifndef UE4SS_SDK_BP_MasterScavenger_HPP
#define UE4SS_SDK_BP_MasterScavenger_HPP

class ABP_MasterScavenger_C : public ABP_MasterAICharacter_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0870 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x0878 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0880 (size: 0x8)
    class UStaticMeshComponent* BackpackMesh;                                         // 0x0888 (size: 0x8)
    class UStaticMeshComponent* HairMesh;                                             // 0x0890 (size: 0x8)
    class UStaticMeshComponent* BeardMesh;                                            // 0x0898 (size: 0x8)
    FText Name;                                                                       // 0x08A0 (size: 0x18)
    ETextGender Gender?;                                                              // 0x08B8 (size: 0x1)
    TArray<class TSubclassOf<ABP_AIWeapon_Master_C>> RangedWeapon;                    // 0x08C0 (size: 0x10)
    TArray<class TSubclassOf<ABP_AIWeapon_Master_C>> MeleeWeapon;                     // 0x08D0 (size: 0x10)
    bool StartWithRangeWeaponEquipped?;                                               // 0x08E0 (size: 0x1)
    TArray<class USkeletalMesh*> Meshes;                                              // 0x08E8 (size: 0x10)
    TArray<class UStaticMesh*> HairMeshes;                                            // 0x08F8 (size: 0x10)
    TArray<class UStaticMesh*> BeardMeshes;                                           // 0x0908 (size: 0x10)
    TMap<class UStaticMesh*, class FTransform> BackpackMeshes;                        // 0x0918 (size: 0x50)
    class AActor* CurrentActor;                                                       // 0x0968 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0970 (size: 0x18)
    bool CanAddMarker?;                                                               // 0x0988 (size: 0x1)
    class UWidgetComponent* Marker;                                                   // 0x0990 (size: 0x8)
    FTimerHandle MarkerTimer;                                                         // 0x0998 (size: 0x8)
    bool Is Burning?;                                                                 // 0x09A0 (size: 0x1)
    class UParticleSystemComponent* BurningEffect;                                    // 0x09A8 (size: 0x8)
    FTimerHandle BurningTimer;                                                        // 0x09B0 (size: 0x8)
    bool Looted;                                                                      // 0x09B8 (size: 0x1)

    void IsAIBurning?(bool& IsBurning?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void CanStompAI?(bool& Stomp?);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
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
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void SetWeapons();
    void Death(class AActor* Player);
    void SetMeshAndMaterial();
    void UserConstructionScript();
    void HitByVehicle(double Speed, FVector Direction);
    void OverrideName(FString Name);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void ReceiveBeginPlay();
    void AddMarker(class UWidgetComponent* Marker);
    void Event_Marker();
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void Event_Fire();
    void StopBurning();
    void OnExecuteInteractEnded();
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void HealthMultiplier(float NewValue);
    void AI_Health();
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_MasterScavenger(int32 EntryPoint);
}; // Size: 0x9B9

#endif
