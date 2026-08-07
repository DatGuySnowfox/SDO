#ifndef UE4SS_SDK_W_DeadLargeAIUI_HPP
#define UE4SS_SDK_W_DeadLargeAIUI_HPP

class UW_DeadLargeAIUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_66;                                                         // 0x02C8 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x02D0 (size: 0x8)
    class UTextBlock* LootAllText;                                                    // 0x02D8 (size: 0x8)
    TArray<class UJSI_Slot_C*> Array Of Items;                                        // 0x02E0 (size: 0x10)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void BndEvt__DeadZombieUI_Button_66_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void OnInitialized();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_W_DeadLargeAIUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif
