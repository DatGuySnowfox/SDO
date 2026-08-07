#ifndef UE4SS_SDK_W_LargeLootContainerUI_HPP
#define UE4SS_SDK_W_LargeLootContainerUI_HPP

class UW_LargeLootContainerUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_66;                                                         // 0x02C8 (size: 0x8)
    class UTextBlock* LootAllText;                                                    // 0x02D0 (size: 0x8)
    class UJSIContainer_C* MainContainer;                                             // 0x02D8 (size: 0x8)
    TArray<class UJSI_Slot_C*> Array Of Items;                                        // 0x02E0 (size: 0x10)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetAllAttachments(TArray<FName>& Attachments);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void BndEvt__LargeContainer_Widget_Button_66_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void OnInitialized();
    void ExecuteUbergraph_W_LargeLootContainerUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif
