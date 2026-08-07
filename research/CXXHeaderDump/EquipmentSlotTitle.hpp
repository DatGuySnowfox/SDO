#ifndef UE4SS_SDK_EquipmentSlotTitle_HPP
#define UE4SS_SDK_EquipmentSlotTitle_HPP

class UEquipmentSlotTitle_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Border_0;                                                          // 0x02C8 (size: 0x8)
    class UTextBlock* TheSlotName;                                                    // 0x02D0 (size: 0x8)
    FText SlotName;                                                                   // 0x02D8 (size: 0x18)
    int32 FontSize;                                                                   // 0x02F0 (size: 0x4)
    FEquipmentSlotTitle_CItemDropped ItemDropped;                                     // 0x02F8 (size: 0x10)
    void ItemDropped(class UJSIContainer_C* FromContainer, class UEquipmentSlotTitle_C* EquipmentSlotRef, class UJSI_Slot_C* NewSlotRef, class UJigsawItem_DataAsset_C* PickupItemInfo);
    bool IsEmpty;                                                                     // 0x0308 (size: 0x1)
    FSlateColor Color;                                                                // 0x030C (size: 0x14)
    FLinearColor BackColor;                                                           // 0x0320 (size: 0x10)
    FMargin TextPadding;                                                              // 0x0330 (size: 0x10)
    FEquipmentSlotTitle_COnWidgetMouseButtonDown OnWidgetMouseButtonDown;             // 0x0340 (size: 0x10)
    void OnWidgetMouseButtonDown(FKey Button);

    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void IsEmptyEquipmentSlot(bool& IsEmpty);
    void PreConstruct(bool IsDesignTime);
    void UpdateName(FText SlotName);
    void UpdateTextColor(FSlateColor Color);
    void SetDefaultColor();
    void ExecuteUbergraph_EquipmentSlotTitle(int32 EntryPoint);
    void OnWidgetMouseButtonDown__DelegateSignature(FKey Button);
    void ItemDropped__DelegateSignature(class UJSIContainer_C* FromContainer, class UEquipmentSlotTitle_C* EquipmentSlotRef, class UJSI_Slot_C* NewSlotRef, class UJigsawItem_DataAsset_C* PickupItemInfo);
}; // Size: 0x350

#endif
