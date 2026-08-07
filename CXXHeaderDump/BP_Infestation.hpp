#ifndef UE4SS_SDK_BP_Infestation_HPP
#define UE4SS_SDK_BP_Infestation_HPP

class ABP_Infestation_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UNiagaraComponent* NS_InfestationDust;                                      // 0x02A8 (size: 0x8)
    class UPostProcessComponent* PostProcess;                                         // 0x02B0 (size: 0x8)
    class USphereComponent* Sphere;                                                   // 0x02B8 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x02C0 (size: 0x8)
    class UDamageComponent_C* DamageComponent;                                        // 0x02C8 (size: 0x8)
    class UStaticMeshComponent* ARMOR;                                                // 0x02D0 (size: 0x8)
    class USceneComponent* ExplosionLoc;                                              // 0x02D8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02E0 (size: 0x8)
    class UParticleSystemComponent* Flies;                                            // 0x02E8 (size: 0x8)
    class UPointLightComponent* PurpleLight;                                          // 0x02F0 (size: 0x8)
    class UPointLightComponent* RedLight;                                             // 0x02F8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x0300 (size: 0x8)
    class UStaticMesh* NormalMesh;                                                    // 0x0308 (size: 0x8)
    class UStaticMesh* DestroyedMesh;                                                 // 0x0310 (size: 0x8)
    bool PrimaryInfestation?;                                                         // 0x0318 (size: 0x1)
    class ABP_Marker_C* MarkerRef;                                                    // 0x0320 (size: 0x8)
    bool IsDestroyed?;                                                                // 0x0328 (size: 0x1)
    class AActor* CurrentActor;                                                       // 0x0330 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0338 (size: 0x18)
    FText Name;                                                                       // 0x0350 (size: 0x18)
    bool ShowMarker?;                                                                 // 0x0368 (size: 0x1)
    float PostProcessRadius;                                                          // 0x036C (size: 0x4)
    TEnumAsByte<Enum_ContainerLootTables::Type> LootTable;                            // 0x0370 (size: 0x1)
    TArray<FName> UniqueLootSets;                                                     // 0x0378 (size: 0x10)
    TArray<FTransform> AISpawningTransforms;                                          // 0x0388 (size: 0x10)
    double SpawnZOffset;                                                              // 0x0398 (size: 0x8)
    FStruct_WaveInfo WavesOfEnemies;                                                  // 0x03A0 (size: 0x10)
    int32 CurrentWave;                                                                // 0x03B0 (size: 0x4)
    TArray<class ABP_Infestation_C*> SecondaryInfestations;                           // 0x03B8 (size: 0x10)
    FBP_Infestation_CInfestationDestroyed InfestationDestroyed;                       // 0x03C8 (size: 0x10)
    void InfestationDestroyed(class AActor* Causer, bool CalledFromGameLoad?);
    bool CanDamagePrimary?;                                                           // 0x03D8 (size: 0x1)
    TArray<FTransform> DecorationTransforms;                                          // 0x03E0 (size: 0x10)
    TArray<class UStaticMesh*> DecorationMeshes;                                      // 0x03F0 (size: 0x10)
    TArray<class ABP_InfestationSpline_C*> Splines;                                   // 0x0400 (size: 0x10)
    bool ForceLock?;                                                                  // 0x0410 (size: 0x1)
    bool RandomInfestation?;                                                          // 0x0411 (size: 0x1)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0418 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0428 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0438 (size: 0x10)

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
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void IsObjectDamageable?(bool& Damageable?);
    void AIHealthBarInfo(bool& Visible?, FString& Name, bool& Boss?);
    void CanStompAI?(bool& Stomp?);
    void GetMovementSpeeds(double& RoamingSpeed, double& AlertSpeed, double& AttackSpeed);
    void GetMesh(class USkeletalMeshComponent*& Mesh);
    void CanAddMarkerToAI?(bool& CanAdd?);
    void GetMarker(class UTexture2D*& Texture, FLinearColor& Color, double& MarkerOffset);
    void IsAIBurning?(bool& IsBurning?);
    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void TestSpawns();
    void AreSecondariesDestroyed?(bool& Destroyed);
    void LocToSpawn(FTransform& Loc);
    void GetInfestationName(FText& Name);
    void SetInteractDistance(class AActor* Target);
    void CheckDistanceFromActor();
    void UserConstructionScript();
    void OnCompleted_65AED6CA4B50BD98E50AE693BFCF19E5();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void SetInteractOption(FGameplayTag Option);
    void PickupBuildFromGround();
    void Damage_Shoved(bool Anim, double Force);
    void AddMarker(class UWidgetComponent* Marker);
    void SetBurning(bool Burning?, double Dmg, class AActor* Actor);
    void HitByVehicle(double Speed, FVector Direction);
    void OverrideName(FString Name);
    void ActorPreLoad();
    void ActorSaved();
    void ReceiveBeginPlay();
    void ActorLoaded();
    void ActorPreSave();
    void BndEvt__DamageTester_DamageComponent_K2Node_ComponentBoundEvent_0_StandardDamage__DelegateSignature(double Dmg, class AActor* DmgCauser);
    void BndEvt__BP_MasterZombie_DamageComponent_K2Node_ComponentBoundEvent_1_Death__DelegateSignature(class AActor* DmgCauser, bool Headshot);
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void ReceiveDestroyed();
    void OnExecuteInteractEnded();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void OnEndInteract();
    void SpawnWave();
    void Event_OnDestroyed(class AActor* Causer, bool CalledFromGameLoad?);
    void Event_SecondaryDestroyed(class AActor* Causer, bool CalledFromGameLoad?);
    void ExecuteUbergraph_BP_Infestation(int32 EntryPoint);
    void InfestationDestroyed__DelegateSignature(class AActor* Causer, bool CalledFromGameLoad?);
}; // Size: 0x448

#endif
