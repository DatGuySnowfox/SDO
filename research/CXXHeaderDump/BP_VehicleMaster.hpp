#ifndef UE4SS_SDK_BP_VehicleMaster_HPP
#define UE4SS_SDK_BP_VehicleMaster_HPP

class ABP_VehicleMaster_C : public AWheeledVehiclePawn
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0330 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x0338 (size: 0x8)
    class USceneComponent* ExitPoint_DriverSide;                                      // 0x0340 (size: 0x8)
    class USceneComponent* ExitPoint_PassengerSide;                                   // 0x0348 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x0350 (size: 0x8)
    class URadioComponent_C* RadioComponent;                                          // 0x0358 (size: 0x8)
    class UParticleSystemComponent* FireEffect;                                       // 0x0360 (size: 0x8)
    class UParticleSystemComponent* SmokeEffect;                                      // 0x0368 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x0370 (size: 0x8)
    class UBP_JigMultiplayer_C* BP_JigMultiplayer;                                    // 0x0378 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x0380 (size: 0x8)
    class UAudioComponent* EngineSound;                                               // 0x0388 (size: 0x8)
    class UVehicleHealthComponent_C* VehicleHealthComponent;                          // 0x0390 (size: 0x8)
    class UWidgetComponent* RepairWidget;                                             // 0x0398 (size: 0x8)
    class UBoxComponent* RepairBoxCollision;                                          // 0x03A0 (size: 0x8)
    class UWidgetComponent* FuelWidget;                                               // 0x03A8 (size: 0x8)
    class UBoxComponent* FuelBoxCollision;                                            // 0x03B0 (size: 0x8)
    class UBoxComponent* HitBoxCollision;                                             // 0x03B8 (size: 0x8)
    class USpotLightComponent* LightLeft;                                             // 0x03C0 (size: 0x8)
    class USpotLightComponent* LightRight;                                            // 0x03C8 (size: 0x8)
    class UVehicleFuelComponent_C* FuelComponent;                                     // 0x03D0 (size: 0x8)
    class USceneComponent* PlayerLocation;                                            // 0x03D8 (size: 0x8)
    bool Outline?;                                                                    // 0x03E0 (size: 0x1)
    class UVehicleUI_C* VehicleUI;                                                    // 0x03E8 (size: 0x8)
    FString Name;                                                                     // 0x03F0 (size: 0x10)
    bool LightsOn?;                                                                   // 0x0400 (size: 0x1)
    bool UniqueVehicle?;                                                              // 0x0401 (size: 0x1)
    TMap<class FGameplayTag, class FText> InteractOptions;                            // 0x0408 (size: 0x50)
    TArray<class USpotLightComponent*> Lights;                                        // 0x0458 (size: 0x10)
    bool Flipped;                                                                     // 0x0468 (size: 0x1)
    bool InRefuellingArea?;                                                           // 0x0469 (size: 0x1)
    bool InRepairingArea?;                                                            // 0x046A (size: 0x1)
    bool InStorageArea?;                                                              // 0x046B (size: 0x1)
    bool IsHeld;                                                                      // 0x046C (size: 0x1)
    double Duration;                                                                  // 0x0470 (size: 0x8)
    FTimerHandle NoiseTimer;                                                          // 0x0478 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0480 (size: 0x18)
    class AActor* CurrentActor;                                                       // 0x0498 (size: 0x8)
    bool GetOutPasseenger;                                                            // 0x04A0 (size: 0x1)
    bool IsHit?;                                                                      // 0x04A1 (size: 0x1)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x04A8 (size: 0x10)
    TArray<FGuid> MainContainersIDs;                                                  // 0x04B8 (size: 0x10)
    TArray<FS_InvWeight> InventoryWeight;                                             // 0x04C8 (size: 0x10)
    bool EngineOn?;                                                                   // 0x04D8 (size: 0x1)
    TArray<class UMaterialInterface*> Colors;                                         // 0x04E0 (size: 0x10)
    int32 VehicleColor;                                                               // 0x04F0 (size: 0x4)
    bool RandomColorSelected?;                                                        // 0x04F4 (size: 0x1)
    FBP_VehicleMaster_CSpecialInteractionOption SpecialInteractionOption;             // 0x04F8 (size: 0x10)
    void SpecialInteractionOption(class AActor* InteractingActor);
    bool UnderWater;                                                                  // 0x0508 (size: 0x1)
    bool CanPush?;                                                                    // 0x0509 (size: 0x1)
    bool IsAir;                                                                       // 0x050A (size: 0x1)
    class UPhysicsConstraintComponent* PhysicsConstraintComp;                         // 0x0510 (size: 0x8)
    bool TowingVehicle?;                                                              // 0x0518 (size: 0x1)
    FVector2D DmgOnVehicleCollision;                                                  // 0x0520 (size: 0x10)

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
    void GetVehicleMesh(class USkeletalMeshComponent*& SkelMesh);
    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void GetMainSceneComp(class USceneComponent*& Comp);
    void GetInteractOptions(TMap<class FGameplayTag, class FText>& Options);
    void OnRequestServerInteract(class AActor* Actor, FGameplayTag Option, FS_JigPayload Payload, bool& Result);
    void GetJigMultiplayerComponent(class UActorComponent*& JigComp);
    void GetItemInfo(class UJigsawItem_DataAsset_C*& Info, int32& Count, FS_RandomStatsConfig& RandomStatsConfig, TMap<class FString, class FString>& CustomData);
    void JigCanInteract(bool& Result);
    void JigSetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void SetPickupCount(int32 NewCount, bool& Result);
    void TraceForVehicleToTow(class AActor*& HitActor);
    void IsInAir?();
    void UseGasCan();
    void VehicleHitAudio();
    void CheckWater();
    void VehicleHealth();
    void VehicleSound();
    void VehicleFire(bool Visible);
    void SetInteractDistance(class AActor* Target);
    void CheckDistanceFromActor();
    void Horn();
    void VehicleSmoke(bool Visible);
    void ApplyDamageToVehicleBasedOnSpeed();
    void ApplyDamageToAIBasedOnSpeed(class AActor* DamagedActor);
    void ExitVehicle(bool& CanExit?);
    void TurnOnOffLights();
    void CheckCanExitCarPassengerSide(bool& Can Exiting Car?);
    void CheckCanExitCarDriverSide(bool& Can Exiting Car?);
    void OnBeginStorageInteract(class AActor* Actor);
    void OnEndStorageInteract(class AActor* Actor);
    void ActorPreLoad();
    void ActorSaved();
    void OnStopExecuteInteract(class AActor* InteractingActor);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnExecuteInteractDialogue(class AActor* InteractingActor);
    void OnExecuteInteractEnded();
    void PickupBuildFromGround();
    void ReceiveBeginPlay();
    void ActorPreSave();
    void ActorLoaded();
    void Svr_UpdateFuel(double Fuel);
    void Svr_UpdateEngine(double Fuel);
    void UpdateStorageStat();
    void SetSaveProperties();
    void Interact_PushVehicle();
    void Interact_OpenStorage(class AActor* Actor);
    void Interact_GetInVehicle();
    void SetInteractOption(FGameplayTag Option);
    void OnEndRepairInteract(class AActor* Actor);
    void OnEndFuelInteract(class AActor* Actor);
    void OnBeginFuelInteract(class AActor* Actor);
    void OnBeginRepairInteract(class AActor* Actor);
    void BndEvt__BP_MasterVehicle_RepairBoxCollision_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void BndEvt__BP_MasterVehicle_RepairBoxCollision_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__BP_MasterVehicle_FuelBoxCollision_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void BndEvt__BP_MasterVehicle_FuelBoxCollision_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void Event_UpdateTimer();
    void Event_StopTimer();
    void Event_HoldTimer();
    void OnExecuteInteract(class AActor* InteractingActor, FGameplayTag Option);
    void OnEndInteract();
    void OnBeginInteract(class AActor* InteractingActor, const TMap<class FGameplayTag, class FText> Options);
    void Event_Noise();
    void Event_ExitVehicle();
    void Event_FlipVehicle();
    void BndEvt__BP_VehicleMaster_Mesh_K2Node_ComponentBoundEvent_0_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void BndEvt__BP_MasterVehicle_HitBoxCollision_K2Node_ComponentBoundEvent_1_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void Event_Landed();
    void RemoveTowingComp();
    void AddTowingComp();
    void ExecuteUbergraph_BP_VehicleMaster(int32 EntryPoint);
    void SpecialInteractionOption__DelegateSignature(class AActor* InteractingActor);
}; // Size: 0x530

#endif
