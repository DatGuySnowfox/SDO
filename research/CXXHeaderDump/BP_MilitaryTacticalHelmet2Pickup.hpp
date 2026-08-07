#ifndef UE4SS_SDK_BP_MilitaryTacticalHelmet2Pickup_HPP
#define UE4SS_SDK_BP_MilitaryTacticalHelmet2Pickup_HPP

class ABP_MilitaryTacticalHelmet2Pickup_C : public ABP_StaticMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0310 (size: 0x8)

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
    void Jig_OnMontageNotify(FName NotifyName);
    void Jig_OnAttachmentSnapInspector();
    void Jig_SetAttachmentActiveState(FGameplayTag Value);
    void Jig_ToggleAttachmentActiveState();
    void Jig_SetAttachmentState(const TArray<FGuid>& UID, FGameplayTag NewState, bool PreExecuteClientSide?);
    void ExecuteUbergraph_BP_MilitaryTacticalHelmet2Pickup(int32 EntryPoint);
}; // Size: 0x318

#endif
