#ifndef UE4SS_SDK_W_BatteryChargerUI_HPP
#define UE4SS_SDK_W_BatteryChargerUI_HPP

class UW_BatteryChargerUI_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle;                                  // 0x0308 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x0310 (size: 0x8)

    void GetLootContent(class UUserWidget*& Widget);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetListOfContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetContainerByAttachmentType(FGameplayTag Type, class UJSIContainer_C*& JigContainer, int32& ContainerIndex);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    void GetJSIContainerByPlayerSlots(FGameplayTag Slot, class UJSIContainer_C*& Container, class UJSI_Slot_C*& EquippedItem, bool& IsPending?);
    void GetListOfNonAddContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetAllAttachments(TArray<FName>& Attachments);
    void SetActionbarFollower(class UJSI_Slot_C* JigRef, bool& Return);
    void PreInitSpecialContainer();
    void ExecuteUbergraph_W_BatteryChargerUI(int32 EntryPoint);
}; // Size: 0x318

#endif
