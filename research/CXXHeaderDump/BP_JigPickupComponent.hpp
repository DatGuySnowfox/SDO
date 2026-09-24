#ifndef UE4SS_SDK_BP_JigPickupComponent_HPP
#define UE4SS_SDK_BP_JigPickupComponent_HPP

class UBP_JigPickupComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    class UJigsawItem_DataAsset_C* ItemDataAsset;                                     // 0x00C0 (size: 0x8)
    int32 OverrideCount;                                                              // 0x00C8 (size: 0x4)
    FS_RandomStatsConfig RandomStatConfig;                                            // 0x00D0 (size: 0x18)
    bool EnablePhysics?;                                                              // 0x00E8 (size: 0x1)
    bool CanBePicked?;                                                                // 0x00E9 (size: 0x1)
    class UBP_JigComponent_C* OwnerMPComp;                                            // 0x00F0 (size: 0x8)
    class AActor* CurrentActor;                                                       // 0x00F8 (size: 0x8)
    FVector InteractingActorLoc;                                                      // 0x0100 (size: 0x18)
    TArray<ABP_AMainLocalAttachment_C*> InstalledAttachments;                         // 0x0118 (size: 0x10)
    FS_RepWeaponAttachment RepAttachments;                                            // 0x0128 (size: 0x20)
    bool ProcessAttachments?;                                                         // 0x0148 (size: 0x1)
    int32 RepCapacity;                                                                // 0x014C (size: 0x4)
    FBP_JigPickupComponent_COnAttachmentsUpdated OnAttachmentsUpdated;                // 0x0150 (size: 0x10)
    void OnAttachmentsUpdated();
    TMap<FString, FString> CustomData;                                                // 0x0160 (size: 0x50)

    void GetCustomData(TArray<FString>& Keys, TArray<FString>& Values);
    void ValidateAttachedActor();
    void SetAttachmentsCustomDepth(bool bValue);
    void CheckMismatch();
    void GetCapacity(int32& RepCapacity);
    void UpdateAttachmentStateByUID(TArray<FGuid>& UID, FGameplayTag NewState, bool& Result);
    void OnRep_CanBePicked?();
    void FindAttachmentByDataAsset(class UJigsawItem_DataAsset_C* DA, FS_RepAttachmentInfo& Output, bool& Found);
    void FindAttachmentDataByType(FGameplayTag Type, FS_RepAttachmentInfo& Output, bool& Found);
    void FindAttachmentActorByContainerIndex(int32 Index, class ABP_AMainLocalAttachment_C*& Attachment, bool& Found);
    void UpdateAttachmentFromMP(class UBP_JigComponent_C* MPComp, FGuid ItemUID);
    void LogAttachments();
    void OnAttachmentInstalled(FGuid MainUID, TArray<FS_RepAttachmentInfo>& Attachments);
    void FindAttachmentActorByTypeArr(FGameplayTag Type, TArray<ABP_AMainLocalAttachment_C*>& Attachment, bool& Found);
    void FindAttachmentActorByType(FGameplayTag Type, class ABP_AMainLocalAttachment_C*& Attachment, bool& Found);
    void UpdateAttachments();
    void OnRep_RepAttachments();
    void ProcessAttachments();
    void GetAttachSocketByInContainerIndex(class UJigsawItem_DataAsset_C* Info, int32 InContainerIndex, FName& Socket);
    void FindLocalAttachmentByUID(FGuid UID, class ABP_AMainLocalAttachment_C*& Attachment);
    void SetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result);
    void CanInteract?(bool& CanBePicked?);
    void SetInteractDistance(class AActor* Target);
    void GetPickupName(FText& Result);
    void ModifyDurability(double MinusDurability);
    void CheckDistanceFromActor();
    void OnRep_EnablePhysics?();
    void ProcessStartingItems(class UBP_JigComponent_C* MPComp);
    void SetCount(int32 OverrideCount);
    void GetCount(int32& Count);
    void ReceiveBeginPlay();
    void OnExecuteInteract(class AActor* Actor, FGameplayTag Option);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void SERVER_UpdateAttachmentState(const TArray<FGuid>& UID, FGameplayTag NewState);
    void SetAttachmentState(TArray<FGuid>& UID, FGameplayTag NewState, bool PreExecuteClientSide?);
    void ExecuteUbergraph_BP_JigPickupComponent(int32 EntryPoint);
    void OnAttachmentsUpdated__DelegateSignature();
}; // Size: 0x1B0

#endif
