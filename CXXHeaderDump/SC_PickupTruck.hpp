#ifndef UE4SS_SDK_SC_PickupTruck_HPP
#define UE4SS_SDK_SC_PickupTruck_HPP

class USC_PickupTruck_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x02C8 (size: 0x8)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetItemReference(class UJSI_Slot_C* ItemRef);
    void SetActorOwner(class AActor* ActorRef);
    void JSICheckStatus();
    void JSIOnWeightUpdated(double NewWeight);
    void JigSetLootContent(class UUserWidget* Widget, FText Name);
    void ForceInitSpecialcontainer();
    void SetInspectorRef(class UBP_InspectorWindowWidget_C* Inspector);
    void OnCreatedFromUtility();
    void ExecuteUbergraph_SC_PickupTruck(int32 EntryPoint);
}; // Size: 0x2D0

#endif
