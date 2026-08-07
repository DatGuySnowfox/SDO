#ifndef UE4SS_SDK_JigContextMenuComp_HPP
#define UE4SS_SDK_JigContextMenuComp_HPP

class UJigContextMenuComp_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class UJigContextMenuCanvas_C* ContextMRef;                                       // 0x00A8 (size: 0x8)
    FJigContextMenuComp_COnOptionSelected OnOptionSelected;                           // 0x00B0 (size: 0x10)
    void OnOptionSelected(FGameplayTag Option, class UJSI_Slot_C* ItemRef, class UJSIContainer_C* ContainerMother);
    class UJSI_Slot_C* CurrentItemRef;                                                // 0x00C0 (size: 0x8)
    class UActorComponent* LocalComp;                                                 // 0x00C8 (size: 0x8)
    class UJSIContainer_C* RootContainer;                                             // 0x00D0 (size: 0x8)
    class UBP_InspectorCanvas_C* InspectCanavs;                                       // 0x00D8 (size: 0x8)
    TArray<FKey> KeysToListenTo;                                                      // 0x00E0 (size: 0x10)
    class UJSI_Slot_C* ItemKeyListener;                                               // 0x00F0 (size: 0x8)

    void ContextRepairAttachment();
    void ContextSmoke();
    void ContextOpenLootBox();
    void ContextPlayMusic();
    void ContextRepairArmor();
    void ContextRepairWeapon();
    void ContextShowAttachments();
    void ContextScanLaptop();
    void ContextRipClothing();
    void ContextReadBook();
    void ContextFillBottle();
    void ContextConsumeDrink();
    void ContextConsumeCanned();
    void ContextConsumeNormal();
    void ContextConsume();
    void ContextPlaceItem(class UJSI_Slot_C* ItemRef, FGuid Guid);
    void ContextUseItem();
    void GetCanvas(class UBP_InspectorCanvas_C*& Output_Get);
    void HandleConsumeCapacityContainer(class UJSI_Slot_C* ItemRef);
    void SetItemRef(class UJSI_Slot_C* CurrentItemRef);
    void OnJigItemMouseEnterLeave(class UJSI_Slot_C* ItemRef, bool Enter?);
    void CreateInspectCanvas(class UBP_InspectorCanvas_C*& Output_Get);
    void OnDoubleClickInspect(class UJSI_Slot_C* ItemRef);
    class ABP_ItemInspector_C* SpawnItemInspector(class UJSI_Slot_C* ItemRef);
    void On Menu Option Selected(FGameplayTag Option);
    void ProcessMenuOptions(class UJSI_Slot_C* JigRef, bool& CanAdd?);
    void HandleMenuLocation(const FPointerEvent& Input, class UJSI_Slot_C* JigRef);
    void OnJigItemRightClick(class UJSI_Slot_C* ItemRef, FGeometry Geometry, FPointerEvent MouseEvent);
    void OneFrameProcess(const FPointerEvent& Input, class UJSI_Slot_C* JigRef);
    void SetComp();
    void ReceiveTick(float DeltaSeconds);
    void SetupConsumeRequest(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* RootContainer, class UJSI_Slot_C* ItemToConsume);
    void OnConsumeConfirm_Event_0(class UJSI_Slot_C* ItemRef, class UJSI_Slot_C* ItemToConsume, class UJSIContainer_C* Rootcon, int32 Amount);
    void ListenForDestructEvent(class UJSI_Slot_C* ItemRef);
    void InvalidateListenDesctructEvent();
    void OnItemDestructEvent_Event_0();
    void ExecuteUbergraph_JigContextMenuComp(int32 EntryPoint);
    void OnOptionSelected__DelegateSignature(FGameplayTag Option, class UJSI_Slot_C* ItemRef, class UJSIContainer_C* ContainerMother);
}; // Size: 0xF8

#endif
