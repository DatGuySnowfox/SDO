#ifndef UE4SS_SDK_BP_MasterZombie_HPP
#define UE4SS_SDK_BP_MasterZombie_HPP

class ABP_MasterZombie_C : public ABP_Zombie_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0690 (size: 0x8)
    class UDamageComponent_C* DamageComponent;                                        // 0x0698 (size: 0x8)
    class USD_AIComponent_C* SD_AIComponent;                                          // 0x06A0 (size: 0x8)
    class UStaticMeshComponent* EyeRightMesh;                                         // 0x06A8 (size: 0x8)
    class UStaticMeshComponent* EyeLeftMesh;                                          // 0x06B0 (size: 0x8)
    class UAudioComponent* Zombie_Cue;                                                // 0x06B8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x06C0 (size: 0x8)
    class UCCCollisionHandlerComponent* CCCollisionHandler;                           // 0x06C8 (size: 0x8)
    class UNavigationInvokerComponent* NavigationInvoker;                             // 0x06D0 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x06D8 (size: 0x8)
    FName AttackSocketName;                                                           // 0x06E0 (size: 0x8)
    TArray<class UAnimMontage*> AttackMontage;                                        // 0x06E8 (size: 0x10)
    class USoundBase* AttackSound;                                                    // 0x06F8 (size: 0x8)
    bool Start_Attacking;                                                             // 0x0700 (size: 0x1)
    class USoundBase* DeathSound;                                                     // 0x0708 (size: 0x8)
    double DamageToDo;                                                                // 0x0710 (size: 0x8)
    bool IsDead?;                                                                     // 0x0718 (size: 0x1)
    TEnumAsByte<Enum_AILootTables::Type> LootTable;                                   // 0x0719 (size: 0x1)
    FText Name;                                                                       // 0x0720 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x0738 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0740 (size: 0x18)
    double Chance for Rad Damage;                                                     // 0x0758 (size: 0x8)
    double Radiation Damage;                                                          // 0x0760 (size: 0x8)
    class UMaterialInstance* EyeMaterial;                                             // 0x0768 (size: 0x8)
    TArray<class USkeletalMesh*> Meshes;                                              // 0x0770 (size: 0x10)
    TArray<class UMaterialInstance*> Materials;                                       // 0x0780 (size: 0x10)
    class UAnimMontage* HitAnimation;                                                 // 0x0790 (size: 0x8)
    class UAnimMontage* ShovedAnimation;                                              // 0x0798 (size: 0x8)
    double DamageToDo_Default;                                                        // 0x07A0 (size: 0x8)
    double Roaming Speed;                                                             // 0x07A8 (size: 0x8)
    double Alert Speed;                                                               // 0x07B0 (size: 0x8)
    double Alert Speed_Default;                                                       // 0x07B8 (size: 0x8)
    double Attack Speed;                                                              // 0x07C0 (size: 0x8)
    bool CanCauseRadDamage?;                                                          // 0x07C8 (size: 0x1)
    bool XpPopUpEnabled?;                                                             // 0x07C9 (size: 0x1)
    bool OutlineEnabled;                                                              // 0x07CA (size: 0x1)
    float DamageMultiplier;                                                           // 0x07CC (size: 0x4)
    float SpeedMultiplier;                                                            // 0x07D0 (size: 0x4)
    TArray<FName> Medical Effects;                                                    // 0x07D8 (size: 0x10)
    double Chance for Bleed;                                                          // 0x07E8 (size: 0x8)
    double Chance for Heavy Bleed;                                                    // 0x07F0 (size: 0x8)
    double Chance for Broken Bone;                                                    // 0x07F8 (size: 0x8)
    bool CanAttack?;                                                                  // 0x0800 (size: 0x1)
    double HealthDeviation;                                                           // 0x0808 (size: 0x8)
    FBP_MasterZombie_CDeathEvent DeathEvent;                                          // 0x0810 (size: 0x10)
    void DeathEvent(bool Headshot);
    FString ArgumentForQuest;                                                         // 0x0820 (size: 0x10)
    bool CanAddMarker?;                                                               // 0x0830 (size: 0x1)
    class UWidgetComponent* Marker;                                                   // 0x0838 (size: 0x8)
    FTimerHandle MarkerTimer;                                                         // 0x0840 (size: 0x8)
    TArray<class TSubclassOf<AActor>> ClassesToIgnore;                                // 0x0848 (size: 0x10)
    float TimeBetweenAttacksMin;                                                      // 0x0858 (size: 0x4)
    float TimeBetweenAttacksMax;                                                      // 0x085C (size: 0x4)
    double Attack Speed_Default;                                                      // 0x0860 (size: 0x8)
    double XPMultiplier_Min;                                                          // 0x0868 (size: 0x8)
    double XPMultiplier_Max;                                                          // 0x0870 (size: 0x8)
    bool Is Burning?;                                                                 // 0x0878 (size: 0x1)
    class UParticleSystemComponent* BurningEffect;                                    // 0x0880 (size: 0x8)
    FTimerHandle BurningTimer;                                                        // 0x0888 (size: 0x8)
    class UParticleSystemComponent* MouthFX;                                          // 0x0890 (size: 0x8)
    class UParticleSystemComponent* ZombieMist;                                       // 0x0898 (size: 0x8)
    FTimerHandle RadTimer;                                                            // 0x08A0 (size: 0x8)
    bool Looted;                                                                      // 0x08A8 (size: 0x1)

    void IsAIBurning?(bool& IsBurning?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void CanStompAI?(bool& Stomp?);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
    void IsObjectDamageable?(bool& Damageable?);
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
    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void ActorDead?(bool& Dead?);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void AttackTrace();
    void RightArmTrace();
    void LeftArmTrace();
    void Death(class AActor* Actor, bool Headshot);
    void Change Speed(double Speed);
    void SetMeshAndMaterial();
    void OnRep_IsDead?();
    void PlayZombieAttack();
    void CheckDistanceFromActor();
    void SetInteractDistance(class AActor* Target);
    void UserConstructionScript();
    void OnNotifyEnd_CC0DAA454A9DD390ABC0838BEAEB95EC(FName NotifyName);
    void OnNotifyBegin_CC0DAA454A9DD390ABC0838BEAEB95EC(FName NotifyName);
    void OnInterrupted_CC0DAA454A9DD390ABC0838BEAEB95EC(FName NotifyName);
    void OnBlendOut_CC0DAA454A9DD390ABC0838BEAEB95EC(FName NotifyName);
    void OnCompleted_CC0DAA454A9DD390ABC0838BEAEB95EC(FName NotifyName);
    void OverrideName(FString Name);
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
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
    void GetQuestArgument(FString Argument);
    void AddMarker(class UWidgetComponent* Marker);
    void Event_Marker();
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void Event_Fire();
    void StopBurning();
    void HitByVehicle(double Speed, FVector Direction);
    void OnExecuteInteractEnded();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void Event_HealthMultiplier(float NewValue);
    void Event_SpeedMultiplier(float NewValue);
    void Event_DamageMultiplier(float NewValue);
    void Outline(bool NewValue);
    void Event_XP(bool NewValue);
    void BndEvt__BP_Zombie_Master_AIOSubject_K2Node_ComponentBoundEvent_2_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_Despawn();
    void BndEvt__BP_MasterZombie_DamageComponent_K2Node_ComponentBoundEvent_1_Death__DelegateSignature(class AActor* DmgCauser, bool Headshot);
    void BndEvt__DamageTester_DamageComponent_K2Node_ComponentBoundEvent_0_StandardDamage__DelegateSignature(double Dmg, class AActor* DmgCauser);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Event_OnHit(const FHitResult& Hit);
    void AttackPlayer();
    void Damage_Shoved(bool Anim, double Force);
    void Event_BloodMoon(bool Start?);
    void Archetype_Sprinter();
    void Event_SpecialDeath(bool Headshot);
    void Archetype_Acidic();
    void Event_Rad();
    void Archetype_UltraRadiated();
    void Archetype_ToughenedCrawler();
    void Archetype_Bomber();
    void ExecuteUbergraph_BP_MasterZombie(int32 EntryPoint);
    void DeathEvent__DelegateSignature(bool Headshot);
}; // Size: 0x8A9

#endif
