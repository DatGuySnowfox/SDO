#ifndef UE4SS_SDK_W_ConstructionWorkbenchUI_HPP
#define UE4SS_SDK_W_ConstructionWorkbenchUI_HPP

class UW_ConstructionWorkbenchUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_81;                                                         // 0x02C8 (size: 0x8)
    class UButton* Button_Decrease;                                                   // 0x02D0 (size: 0x8)
    class UButton* Button_Increase;                                                   // 0x02D8 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle;                                  // 0x02E0 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_1;                                // 0x02E8 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x02F0 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x02F8 (size: 0x8)
    class UBorder* LoadingT;                                                          // 0x0300 (size: 0x8)
    class UEquipmentSlotTitle_C* ModularTitle;                                        // 0x0308 (size: 0x8)
    class UEquipmentSlotTitle_C* StandardTitle;                                       // 0x0310 (size: 0x8)
    class UTextBlock* TextBlock_75;                                                   // 0x0318 (size: 0x8)
    class UThrobber* Throbber_567;                                                    // 0x0320 (size: 0x8)
    TMap<UJSI_Slot_C*, int32> AddedItems;                                             // 0x0328 (size: 0x50)
    class UJSI_Slot_C* CurrentFaded;                                                  // 0x0378 (size: 0x8)
    class UBP_JigMultiplayer_C* MPComponent;                                          // 0x0380 (size: 0x8)
    TArray<FGuid> RequestConsume;                                                     // 0x0388 (size: 0x10)
    int32 CraftIndex;                                                                 // 0x0398 (size: 0x4)
    class ABuildable_ConstructionBench_C* StationOwner;                               // 0x03A0 (size: 0x8)
    class UBP_JigMultiplayer_C* PlayerMPComp;                                         // 0x03A8 (size: 0x8)
    int32 CountIndex;                                                                 // 0x03B0 (size: 0x4)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetCraftableItems();
    FText GetText();
    void CheckIngredientsAvailability(bool& Proceed);
    void Add Required Items();
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
    void HandleCraftingButtons();
    void OnStandardMouseButtonDown(FKey Button);
    void OnModularMouseButtonDown(FKey Button);
    void OnInitialized();
    void BndEvt__Button_81_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Destruct();
    void ExecuteUbergraph_W_ConstructionWorkbenchUI(int32 EntryPoint);
}; // Size: 0x3B4

#endif
