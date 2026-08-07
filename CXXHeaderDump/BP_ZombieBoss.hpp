#ifndef UE4SS_SDK_BP_ZombieBoss_HPP
#define UE4SS_SDK_BP_ZombieBoss_HPP

class ABP_ZombieBoss_C : public ABP_MainEnemy_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0690 (size: 0x8)
    class UDamageComponent_C* DamageComponent;                                        // 0x0698 (size: 0x8)
    class USD_AIComponent_C* SD_AIComponent;                                          // 0x06A0 (size: 0x8)
    class USceneComponent* ThrowLoc;                                                  // 0x06A8 (size: 0x8)
    class UStaticMeshComponent* EyeRightMesh;                                         // 0x06B0 (size: 0x8)
    class UStaticMeshComponent* EyeLeftMesh;                                          // 0x06B8 (size: 0x8)
    class UAudioComponent* Zombie_Cue;                                                // 0x06C0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x06C8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x06D0 (size: 0x8)
    class UNavigationInvokerComponent* NavigationInvoker;                             // 0x06D8 (size: 0x8)
    bool DamageNumbersEnabled;                                                        // 0x06E0 (size: 0x1)
    bool IsDead?;                                                                     // 0x06E1 (size: 0x1)
    bool BloodSplatterEnabled;                                                        // 0x06E2 (size: 0x1)
    TEnumAsByte<Enum_AILootTables::Type> LootTable;                                   // 0x06E3 (size: 0x1)
    TArray<FName> Unqiue Items Set;                                                   // 0x06E8 (size: 0x10)
    FText AIName;                                                                     // 0x06F8 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x0710 (size: 0x8)
    double DamageToDo;                                                                // 0x0718 (size: 0x8)
    bool Start_Attacking;                                                             // 0x0720 (size: 0x1)
    TArray<class UAnimMontage*> AttackMontage;                                        // 0x0728 (size: 0x10)
    class USoundBase* HitSound;                                                       // 0x0738 (size: 0x8)
    TArray<class TSubclassOf<AActor>> ClassesToIgnore;                                // 0x0740 (size: 0x10)
    FVector InteractingActorLoc;                                                      // 0x0750 (size: 0x18)
    bool XpPopUpEnabled?;                                                             // 0x0768 (size: 0x1)
    bool HealthBarUI;                                                                 // 0x0769 (size: 0x1)
    bool OutlineEnabled;                                                              // 0x076A (size: 0x1)
    bool HealthBarEnabled;                                                            // 0x076B (size: 0x1)
    float DamageMultiplier;                                                           // 0x076C (size: 0x4)
    float SpeedMultiplier;                                                            // 0x0770 (size: 0x4)
    bool CanAddMarker?;                                                               // 0x0774 (size: 0x1)
    class UWidgetComponent* Marker;                                                   // 0x0778 (size: 0x8)
    FTimerHandle MarkerTimer;                                                         // 0x0780 (size: 0x8)
    bool Is Burning?;                                                                 // 0x0788 (size: 0x1)
    class UParticleSystemComponent* BurningEffect;                                    // 0x0790 (size: 0x8)
    FTimerHandle BurningTimer;                                                        // 0x0798 (size: 0x8)
    FBP_ZombieBoss_CBossDeath BossDeath;                                              // 0x07A0 (size: 0x10)
    void BossDeath();
    bool Looted;                                                                      // 0x07B0 (size: 0x1)

    void IsAIBurning?(bool& IsBurning?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void CanStompAI?(bool& Stomp?);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
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
    void RightArmTrace();
    void LeftArmTrace();
    void RadiationDamage(class AActor* Object);
    void Change Speed(double Speed);
    void OnRep_IsDead?();
    void PlayAttack();
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void UserConstructionScript();
    void HitByVehicle(double Speed, FVector Direction);
    void AI Dead();
    void Actor Aim Focus(class AActor* Instigator);
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void Despawn();
    void Death(class AActor* Player);
    void MeleeAttack();
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void RangedAttack(FVector Target);
    void AddMarker(class UWidgetComponent* Marker);
    void Event_Marker();
    void Event_OnHit(const FHitResult& Hit);
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void Event_Fire();
    void StopBurning();
    void OverrideName(FString Name);
    void OnExecuteInteractEnded();
    void OnEndInteract();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void Event_HealthMultiplier(float NewValue);
    void Event_SpeedMultiplier(float NewValue);
    void Event_DamageMultiplier(float NewValue);
    void Outline(bool NewValue);
    void Event_XP(bool NewValue);
    void Event_Despawn();
    void BndEvt__BP_MasterZombie_DamageComponent_K2Node_ComponentBoundEvent_1_Death__DelegateSignature(class AActor* DmgCauser, bool Headshot);
    void BndEvt__DamageTester_DamageComponent_K2Node_ComponentBoundEvent_0_StandardDamage__DelegateSignature(double Dmg, class AActor* DmgCauser);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void ExecuteUbergraph_BP_ZombieBoss(int32 EntryPoint);
    void BossDeath__DelegateSignature();
}; // Size: 0x7B1

#endif
