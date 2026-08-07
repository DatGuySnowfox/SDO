#ifndef UE4SS_SDK_BP_FirearmPickup_HPP
#define UE4SS_SDK_BP_FirearmPickup_HPP

class ABP_FirearmPickup_C : public ABP_SkeletalMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UBP_WeaponsPickupComponent_C* BP_WeaponsPickupComponent;                    // 0x0388 (size: 0x8)
    float ResetRecoilTL_Reset_0A95F0FE40AECD2F428F87B705900F3B;                       // 0x0390 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> ResetRecoilTL__Direction_0A95F0FE40AECD2F428F87B705900F3B; // 0x0394 (size: 0x1)
    class UTimelineComponent* ResetRecoilTL;                                          // 0x0398 (size: 0x8)
    float RecoilTL_Recoil_297C066B40C93F9CB632DC9EE7ECC8B0;                           // 0x03A0 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> RecoilTL__Direction_297C066B40C93F9CB632DC9EE7ECC8B0; // 0x03A4 (size: 0x1)
    class UTimelineComponent* RecoilTL;                                               // 0x03A8 (size: 0x8)
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x03B0 (size: 0x8)
    bool FiringAutomatically?;                                                        // 0x03B8 (size: 0x1)
    class UJSIContainer_C* ChamberContainer;                                          // 0x03C0 (size: 0x8)
    class UJSI_Slot_C* CurrentMag;                                                    // 0x03C8 (size: 0x8)
    class ABP_AMainLocalAttachment_C* PendingMag;                                     // 0x03D0 (size: 0x8)
    bool PlayingFX;                                                                   // 0x03D8 (size: 0x1)
    float ShotDistance;                                                               // 0x03DC (size: 0x4)
    bool HipFire?;                                                                    // 0x03E0 (size: 0x1)
    bool FireReady;                                                                   // 0x03E1 (size: 0x1)
    FGameplayTag PlayerActiveWeapon;                                                  // 0x03E4 (size: 0x8)
    class UActorComponent* GPlayerMPComp;                                             // 0x03F0 (size: 0x8)
    bool CurrentlyFiring?;                                                            // 0x03F8 (size: 0x1)
    float TempPitch;                                                                  // 0x03FC (size: 0x4)
    float TempYaw;                                                                    // 0x0400 (size: 0x4)
    class UMaterialInstanceDynamic* DamageMaterialInstance;                           // 0x0408 (size: 0x8)

    void Jig_AttachmentInstalled(FGuid MainUID, TArray<FS_RepAttachmentInfo>& Attachments, bool& Result, FGameplayTag& AttachmentType);
    void Jig_AttachmentRemoved(TArray<FS_RepAttachmentInfo>& Attachments, bool& Result, FGameplayTag& AttachmentType);
    void Jig_GetADSSocketLocation(FVector& SocketLoc, bool& SocketFound);
    void Jig_GetPrimitiveComponent(class UPrimitiveComponent*& Comp);
    void Jig_CheckCanReload(class UActorComponent* MPComp, FGameplayTag PlayerSlots, bool& Result, class UJSIContainer_C*& ContainerMag, class UAnimMontage*& ReloadMontage, bool& HasInternalMag, bool& CanChamber?, class UJSI_Slot_C*& ReloadWith, class UAnimMontage*& LoopReloadAnim);
    void Jig_StartReloading(class UActorComponent* MPCom, class UJSIContainer_C* MagContainer, class UJSI_Slot_C* WithMag, bool& Result);
    void Jig_GetEquipMontage(bool CanChamber?, class UAnimMontage*& EquipMontage);
    void Jig_ExecuteChamberWeapon(class UActorComponent* MPComp, FGameplayTag PlayerSlots, bool& Result, class UAnimMontage*& ChamberMontage, double& Speed);
    void Jig_CanChamberWeapon(class UActorComponent* MPComp, FGameplayTag PlayerSlots, bool& Result);
    void Jig_GetAttachmentInfo(FGuid& UID, class UJigsawItem_DataAsset_C*& AttachmentInfo);
    void Jig_SetAttachmentInfo(FGuid UID, class UJigsawItem_DataAsset_C* AttachmentInfo, bool& Result);
    void Jig_GetAttachmentByType(FGameplayTag Type, class ABP_AMainLocalAttachment_C*& Attachment, bool& Found);
    void Jig_SetPendingMag(class ABP_AMainLocalAttachment_C* MagRef, class UJigsawItem_DataAsset_C* DA, bool& Result);
    void Jig_GetPendingMag(bool FindMagIfNoPending?, class ABP_AMainLocalAttachment_C*& MagRef);
    void Jig_GetAttachmentActiveState(FGameplayTag& Result);
    void Jig_GetAvailableAttachmentStates(FGameplayTagContainer& States);
    void Jig_GetAttachmentArrayByType(FGameplayTag Type, TArray<class ABP_AMainLocalAttachment_C*>& Attachment, bool& Found);
    void DurabilityCheck(bool& CarryOn?);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void JigMP_OnUpdateChamberUID(FGuid NewUID, bool& Result);
    void JigMP_OnItemConsumed(FRepItemInfo ConsumedItem, int32 Amount, int32 Remaining, FRepItemInfo InsideOf, FName FromContainer, FString CustomData, bool CallbackToPlayer?, bool& Result);
    void MuzzleDurability(bool& AttachmentFound?);
    void GetNewMagInfo(class UJSI_Slot_C* NewMag, double& Multiplier);
    void ReduceDurability(bool& Reduced?);
    void GetVerticalRecoilStrength(double& Strength);
    void GetHorizontalRecoilStrength(double& Strength);
    void MuzzleEffects();
    void FindAttachmentStats(FGameplayTag Stat, bool& Found, double& StatValue);
    void UpdatePlayerPitch&Yaw(double Pitch, double Yaw);
    void FindSpecificAttachmentStat(FGameplayTag Attachment, FGameplayTag Stat, bool& Found?, double& StatValue);
    void UpdateStat(FGameplayTag StatTag, double NewStatValue, bool& Updated, double& StatValue);
    void FirearmTypeMultiplier(bool Headshot?, double& Multiplier);
    void DamageMultiplier(bool Headshot?, double& Damage);
    void FindStat(FGameplayTag StatTag, bool& Found, float& MinValue, double& MaxValue);
    void CheckAttachments();
    void Local_CheckCanReloadInternal(class UActorComponent* PlayerComp, FGameplayTag ActiveSlot, bool& Result, class UJSIContainer_C*& ContainerMag, bool& CanChamber, class UJSI_Slot_C*& ReloadWith);
    void Local_CheckCanReload(class UActorComponent* PlayerComp, FGameplayTag ActiveSlot, bool& Result, class UJSIContainer_C*& ContainerMag, bool& CanChamber?, class UJSI_Slot_C*& ReloadWith);
    void LocalStartReloadInternal(class UActorComponent* PlayerComp, class UJSIContainer_C* ContainerMag, bool& Result);
    void LocalStartReload(class UActorComponent* PlayerComp, class UJSIContainer_C* ContainerMag, class UJSI_Slot_C* WithMag, bool& Result);
    void ProjectileTraceFromCamera(FTransform& Transform);
    void ChamberWeapon(class UActorComponent* MPComp, FGameplayTag ActiveWeapons, bool FromFireFunction, bool& Result, FName& BulletID);
    void Local_StopShooting(bool DontSendRPC?);
    void CriticalHit(double Damage, double& NewDamage, FLinearColor& FloatingDmgColour);
    void EffectiveDamage(double Damage, double& NewDamage);
    void SpawnTrail(FVector MuzzleLocation, FVector EndPointLocation, FRotator Rotation);
    void WeaponRecoilValues(double& Pitch/Vertical, double& Yaw/Horizontal);
    void Local_StartShooting(class UBP_JigMultiplayer_C* PlayerMPComp, FGameplayTag ActiveWeapon, bool HipFire?);
    void FireSurfaceHit(FVector ImpactPoint, class AActor* HitActor, FRotator Rotation, TEnumAsByte<EPhysicalSurface> Surface);
    void FireTrace(double Spread);
    void FireBullet();
    void RecoilTL__FinishedFunc();
    void RecoilTL__UpdateFunc();
    void ResetRecoilTL__FinishedFunc();
    void ResetRecoilTL__UpdateFunc();
    void OnNotifyEnd_336AF8914848A4799421D89C590F8176(FName NotifyName);
    void OnNotifyBegin_336AF8914848A4799421D89C590F8176(FName NotifyName);
    void OnInterrupted_336AF8914848A4799421D89C590F8176(FName NotifyName);
    void OnBlendOut_336AF8914848A4799421D89C590F8176(FName NotifyName);
    void OnCompleted_336AF8914848A4799421D89C590F8176(FName NotifyName);
    void OnAttachmentsUpdated();
    void ReceiveBeginPlay();
    void Jig_SetAttachmentState(const TArray<FGuid>& UID, FGameplayTag NewState, bool PreExecuteClientSide?);
    void Svr_SpawnTrail(FVector MuzzleLocation, FVector EndPointLocation, FRotator Rotation);
    void Svr_WeaponEffects(class UParticleSystem* EmitterTemplate, class USceneComponent* AttachToComponent, FName AttachPointName);
    void MC_SpawnTrail(FVector MuzzleLocation, FVector EndPointLocation, FRotator Rotation);
    void MC_WeaponEffects(class UParticleSystem* EmitterTemplate, class USceneComponent* AttachToComponent, FName AttachPointName);
    void Event_FireRate();
    void MC_WeaponShot(bool Started?);
    void Svr_WeaponShot(bool Started?);
    void Jig_ToggleAttachmentActiveState();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Jig_OnAttachmentSnapInspector();
    void SERVER_DamageEvent(bool Headshot, class AActor* DamagedActor);
    void Jig_OnMontageNotify(FName NotifyName);
    void Trace_ImpactBulletHole(FVector Location, FRotator Rotation, class AActor* Actor);
    void Trace_Impact(FVector Location, FRotator Rotation, class AActor* Actor);
    void Trace_BodyImpact(FVector Location, FRotator Rotation, class AActor* Actor);
    void Trace_Headshot(FVector Location, FRotator Rotation, class AActor* Actor);
    void Client_HitUI(double Damage, FLinearColor Colour);
    void StartRecoil();
    void ExecuteUbergraph_BP_FirearmPickup(int32 EntryPoint);
}; // Size: 0x410

#endif
