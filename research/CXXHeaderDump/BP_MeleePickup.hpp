#ifndef UE4SS_SDK_BP_MeleePickup_HPP
#define UE4SS_SDK_BP_MeleePickup_HPP

class ABP_MeleePickup_C : public ABP_StaticMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0320 (size: 0x8)
    class UBP_EquipmentPickupComponent_C* BP_EquipmentPickupComponent;                // 0x0328 (size: 0x8)
    class UBP_WeaponsPickupComponent_C* BP_WeaponsPickupComponent;                    // 0x0330 (size: 0x8)
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x0338 (size: 0x8)

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
    void Jig_GetAttachmentArrayByType(FGameplayTag Type, TArray<ABP_AMainLocalAttachment_C*>& Attachment, bool& Found);
    void DamageCalc(double Dmg, bool Power?, bool Headshot?, class AActor* Target, double& NewDmg, FLinearColor& FloatingDmdColour);
    void DurabilityCheck(double Dmg, double& NewDmg);
    void MaxMeleeHits(class UBP_WeaponsPickupComponent_C* Comp, int32& MaxActorsToHitInSwing?);
    void ReduceDurability(double Multiplier);
    void HitObjectWorld(class AActor* Object, bool Power?);
    void HitHead(class AActor* DamagedActor, FVector HitLoc, bool Power?);
    void HitBody(class AActor* DamagedActor, FVector HitLoc, bool Power?);
    void MeleeTracePower();
    void MeleeTrace();
    void NightCreatureCheck(double Input, double& Ouput);
    void HitTrashObject(class AActor* Actor, double Dmg, bool& Hit?);
    void HitFoliageRock(class UObject* Object, FVector SpawnTransform_Location, class UPrimitiveComponent* PrimitiveComponent, int32 Instance, int32 Hits, bool& Hit?);
    void HitFoliageTree(class UObject* Object, FVector SpawnTransform_Location, class UPrimitiveComponent* PrimitiveComponent, int32 Instance, int32 Hits, bool& Hit?);
    void CriticalHit(double Damage, double& NewDamage, FLinearColor& FloatingDmgColour);
    void FindStat(FGameplayTag StatTag, bool& Found, float& MinStat, double& MaxStat);
    void Jig_OnAttachmentSnapInspector();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Jig_ToggleAttachmentActiveState();
    void Jig_SetAttachmentState(const TArray<FGuid>& UID, FGameplayTag NewState, bool PreExecuteClientSide?);
    void ReceiveBeginPlay();
    void Event_OnHitNormal(const FHitResult& Hit);
    void Event_OnHitPower(const FHitResult& Hit);
    void Jig_OnMontageNotify(FName NotifyName);
    void Client_StrengthXP(double float);
    void Client_HitUI(double Damage, FLinearColor Colour, FVector ImpactPoint);
    void ExecuteUbergraph_BP_MeleePickup(int32 EntryPoint);
}; // Size: 0x340

#endif
