#ifndef UE4SS_SDK_BP_MainJigInventory_HPP
#define UE4SS_SDK_BP_MainJigInventory_HPP

class UBP_MainJigInventory_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    TArray<UJSIContainer_C*> Priv_Containers;                                         // 0x0348 (size: 0x10)
    TArray<UJSIContainer_C*> Priv_ExcludedContainers;                                 // 0x0358 (size: 0x10)
    class UUserWidget* Priv_CurrentLootWidget;                                        // 0x0368 (size: 0x8)
    class UBP_EquipToContent_C* Priv_LootContentTarget;                               // 0x0370 (size: 0x8)
    class AActor* Priv_PreviewChar;                                                   // 0x0378 (size: 0x8)
    TArray<UBP_EquipToContent_C*> Priv_ContentAdded;                                  // 0x0380 (size: 0x10)
    class UPanelWidget* Priv_EquipmentContentPanel;                                   // 0x0390 (size: 0x8)
    class UBP_CharPreview_C* Priv_CharPreview;                                        // 0x0398 (size: 0x8)
    TMap<UJSIContainer_C*, UJSIContainer_C*> Priv_SwapContainers;                     // 0x03A0 (size: 0x50)
    TSubclassOf<class AActor> Priv_PreviewCharClass;                                  // 0x03F0 (size: 0x8)
    bool Priv_OnCloseClearLootWidget?;                                                // 0x03F8 (size: 0x1)

    void GetListOfContainers(TArray<UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetValidReloadContainers(TArray<UJSIContainer_C*>& Containers);
    void GetLootContent(class UUserWidget*& Widget);
    void HandlePrimarySecondarySwap(class UJSI_Slot_C* ItemReceiver, class UJSI_Slot_C* ItemDropped, class UJSIContainer_C* ItemReceiverContainer, class UJSIContainer_C* ItemDroppedContainer);
    void SpawnCharacterPreview();
    void GetContentPanelBySlot(FGameplayTag Tag, class UPanelWidget*& Result);
    void UpdatePreviewCharacter(class UJSIContainer_C* ContainerRef, class UJSIContainer_C* ToContainer, bool Equipped?);
    void GetInventoryData(TArray<UJSIContainer_C*>& Containers, TArray<UJSIContainer_C*>& ExcludedContainers, class UBP_EquipToContent_C*& LootContentTarget, bool& OnCloseClearLootWidget?, class UPanelWidget*& EquipmentContentPanel, TMap<UJSIContainer_C*, UJSIContainer_C*>& SwapContainers, class UBP_CharPreview_C*& CharacterPreviewWidget, TSubclassOf<class AActor>& PreviewCharClass);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void ForceInitSpecialcontainer();
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void Destruct();
    void DropInfo_OnItemEquippedChange_Event(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* SlotRef, bool Equipped?);
    void HandleEquipmentContent(class UJSIContainer_C* Target);
    void Construct();
    void Drop_ItemOverItem_Event_0(class UJSI_Slot_C* ItemReceiver, class UJSI_Slot_C* ItemDropped, class UJSIContainer_C* ItemReceiverContainer, class UJSIContainer_C* ItemDroppedContainer);
    void OnLootAdded/Removed(bool Added?);
    void ExecuteUbergraph_BP_MainJigInventory(int32 EntryPoint);
}; // Size: 0x3F9

#endif
