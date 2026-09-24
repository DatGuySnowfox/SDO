#ifndef UE4SS_SDK_W_VicinityLootUI_HPP
#define UE4SS_SDK_W_VicinityLootUI_HPP

class UW_VicinityLootUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UBorder* VicinityContent;                                                   // 0x0348 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x0350 (size: 0x8)
    TMap<UBP_JigPickupComponent_C*, UJSI_Slot_C*> Added;                              // 0x0358 (size: 0x50)
    bool Init;                                                                        // 0x03A8 (size: 0x1)
    FTimerHandle TimerHandle;                                                         // 0x03B0 (size: 0x8)

    class AActor* GetActorFromKey(class UJSI_Slot_C* JigRef, class UBP_JigPickupComponent_C*& Comp);
    void AddPickups();
    void OnInitialized();
    void Drop_ContainerOnContainerUnhandled_Event_0(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* DroppedSlotRef, class UJSI_Slot_C* SlotReceiverRef, int32 ToSlotIndex, bool Rotated?);
    void Construct();
    void Destruct();
    void OnItemStackRequest_Event_0(class UJSI_Slot_C* DroppedItem, class UJSI_Slot_C* ReceiverItem);
    void OnSlotMouseButtonDown_Event_0(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void ExecuteUbergraph_W_VicinityLootUI(int32 EntryPoint);
}; // Size: 0x3B8

#endif
