#ifndef UE4SS_SDK_JigItemOverItemComp_HPP
#define UE4SS_SDK_JigItemOverItemComp_HPP

class UJigItemOverItemComp_C : public UActorComponent
{
    FJigItemOverItemComp_COnOptionSelected OnOptionSelected;                          // 0x00A0 (size: 0x10)
    void OnOptionSelected(FGameplayTag Option, class UJSI_Slot_C* ItemRef, class UJSIContainer_C* ContainerMother);

    void PurifyWater(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped);
    void SwapItem(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped, class UJSIContainer_C* ReceiverContainer, class UJSIContainer_C* DroppedContainer, bool Equipped?, int32 AmountToDrop);
    void RefillRadFilter(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped);
    void PlaceBattery(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped, class UJSIContainer_C* ReceiverContainer, class UJSIContainer_C* DroppedContainer);
    void GetValid(class UJSI_Slot_C* Dropped, class UJSI_Slot_C* Receiver, bool& ShowHighlight?, bool& Valid?, FGameplayTag& Tag);
    void ArmorRepair(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped);
    void WeaponRepair(class UJSI_Slot_C* Receiver, class UJSI_Slot_C* Dropped, bool Attachment?);
    void GetOptionOnDrop(class UJSI_Slot_C* ReceiverItem, class UJSI_Slot_C* DroppedItem, class UJSIContainer_C* ReceiverContainer, class UJSIContainer_C* DroppedContainer);
    void OnOptionSelected__DelegateSignature(FGameplayTag Option, class UJSI_Slot_C* ItemRef, class UJSIContainer_C* ContainerMother);
}; // Size: 0xB0

#endif
