#ifndef UE4SS_SDK_JSISpecialWidgetInterface_HPP
#define UE4SS_SDK_JSISpecialWidgetInterface_HPP

class IJSISpecialWidgetInterface_C : public IInterface
{

    void GetLootContent(class UUserWidget*& Widget);
    void OnCreatedFromUtility();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void ForceInitSpecialcontainer();
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void JSIOnWeightUpdated(double NewWeight);
    void GetAllAttachments(TArray<FName>& Attachments);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void JSICheckStatus();
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void SetActorOwner(class AActor* ActorRef);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
}; // Size: 0x28

#endif
