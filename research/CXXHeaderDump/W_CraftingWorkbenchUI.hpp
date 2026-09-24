#ifndef UE4SS_SDK_W_CraftingWorkbenchUI_HPP
#define UE4SS_SDK_W_CraftingWorkbenchUI_HPP

class UW_CraftingWorkbenchUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UThrobber* Throbber_567;                                                    // 0x0348 (size: 0x8)
    class UTextBlock* TextBlock_75;                                                   // 0x0350 (size: 0x8)
    class UBorder* LoadingT;                                                          // 0x0358 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x0360 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x0368 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_1;                                // 0x0370 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle;                                  // 0x0378 (size: 0x8)
    class UButton* Button_Increase;                                                   // 0x0380 (size: 0x8)
    class UButton* Button_Decrease;                                                   // 0x0388 (size: 0x8)
    class UButton* Button_81;                                                         // 0x0390 (size: 0x8)
    TMap<UJSI_Slot_C*, int32> AddedItems;                                             // 0x0398 (size: 0x50)
    class UJSI_Slot_C* CurrentFaded;                                                  // 0x03E8 (size: 0x8)
    class UBP_JigComponent_C* MPComponent;                                            // 0x03F0 (size: 0x8)
    TArray<FGuid> RequestConsume;                                                     // 0x03F8 (size: 0x10)
    int32 CraftIndex;                                                                 // 0x0408 (size: 0x4)
    class AActor* StationOwner;                                                       // 0x0410 (size: 0x8)
    class UBP_JigComponent_C* PlayerMPComp;                                           // 0x0418 (size: 0x8)
    int32 CountIndex;                                                                 // 0x0420 (size: 0x4)
    bool ItemCrafted?;                                                                // 0x0424 (size: 0x1)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetListOfContainers(TArray<UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetCraftableItems();
    FText GetText();
    void CheckIngredientsAvailability(bool& Proceed);
    void AddRequiredItems();
    void InitializeInventory();
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void BindEvents();
    void EventOnMouseButtonDown(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void Construct();
    void BndEvt__BP_CraftingWidget_Button_Decrease_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_CraftingWidget_Button_Increase_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Button_81_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_W_CraftingWorkbenchUI(int32 EntryPoint);
}; // Size: 0x425

#endif
