#ifndef UE4SS_SDK_BP_MainSpecialContainer_HPP
#define UE4SS_SDK_BP_MainSpecialContainer_HPP

class UBP_MainSpecialContainer_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UJSI_Slot_C* ItemOwner;                                                     // 0x0348 (size: 0x8)
    class UJSIContainer_C* TempChamberContainer;                                      // 0x0350 (size: 0x8)
    class UJSIContainer_C* TempMagContainer;                                          // 0x0358 (size: 0x8)
    TArray<UJSIContainer_C*> AllContainers;                                           // 0x0360 (size: 0x10)
    class UWidget* Temp;                                                              // 0x0370 (size: 0x8)
    class UBP_InspectorWindowWidget_C* InspectorParent;                               // 0x0378 (size: 0x8)

    void GetLootContent(class UUserWidget*& Widget);
    void GetListOfContainers(TArray<UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetValidReloadContainers(TArray<UJSIContainer_C*>& Containers);
    void BackpackSettings(bool Disabled?);
    void SetActorOwner(class AActor* ActorRef);
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void OnCreatedFromUtility();
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetContainerSnapUpdate(const TArray<UJSIContainer_C*>& Containers, const TArray<UJSIContainer_C*>& ContentContainers);
    void EventOnAttachmentEquipped(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void OnHandguardEquipped(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void UpdateSnap();
    void OnHandguardAttachment(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void PreInitSpecialContainer();
    void ForceInitSpecialcontainer();
    void Destruct();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void UpdateAmmoCounter(class UJSIContainer_C* Mag, class UJSIContainer_C* Chamber);
    void DropInfo_OnItemEquippedChange_Event_0(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void JSICheckStatus();
    void DropInfo_OnItemEquippedChange_Event_1(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void ExecuteUbergraph_BP_MainSpecialContainer(int32 EntryPoint);
}; // Size: 0x380

#endif
