#ifndef UE4SS_SDK_BuildableMaster_Display_HPP
#define UE4SS_SDK_BuildableMaster_Display_HPP

class ABuildableMaster_Display_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    TArray<FS_ReplicatedContainerInfo> MainJigContainers;                             // 0x0438 (size: 0x10)
    TArray<FGuid> MainUIDs;                                                           // 0x0448 (size: 0x10)
    TArray<FS_InvWeight> Weights;                                                     // 0x0458 (size: 0x10)
    FText ContainerName;                                                              // 0x0468 (size: 0x18)
    class USkeletalMeshComponent* SKMComp;                                            // 0x0480 (size: 0x8)
    class UStaticMeshComponent* SMComp;                                               // 0x0488 (size: 0x8)
    TArray<class ABP_AMainLocalAttachment_C*> CurrentAttachments;                     // 0x0490 (size: 0x10)
    bool Displayed;                                                                   // 0x04A0 (size: 0x1)

    void JigMP_OnItemRemoved(FGuid ItemUID, FGuid FromContainerUID, FGuid ToContainerUID, class UActorComponent* ToComponent, bool& Result);
    void JigMP_OnMainContainerItemAdded(FGuid ToContainerUID, FGuid ItemUID, FName ToContainerName, FRepItemInfo ItemInfo, FName FromMain, bool& Result, class AActor*& ActorRef);
    void JigMP_OnMainContainerItemRemoved(FGuid FromContainerUID, FGuid ItemUID, FName ContainerName, FRepItemInfo ItemRemovedInfo, bool& Result);
    void GetActiveComponent(class USceneComponent*& Comp);
    void RemoveIronsights(bool Hide?, class ABP_AMainLocalAttachment_C* Attachment);
    void InstallAttachments(FGuid UID);
    void DisplayItem(bool Display?, FGuid& UID);
    void OnServerExecuteInteract(FGameplayTag Option, class AActor* ByActor, FS_JigPayload Payload);
    void OnInteractBuildable(class AActor* Actor);
    void ActorPreSave();
    void ActorLoaded();
    void ReceiveBeginPlay();
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BuildableMaster_Display(int32 EntryPoint);
}; // Size: 0x4A1

#endif
