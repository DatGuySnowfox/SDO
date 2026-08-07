#ifndef UE4SS_SDK_DropItemBackGwidget_HPP
#define UE4SS_SDK_DropItemBackGwidget_HPP

class UDropItemBackGwidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBackgroundBlur* BackgroundBlur_32;                                         // 0x02C8 (size: 0x8)
    FDropItemBackGwidget_CItemDropRequest ItemDropRequest;                            // 0x02D0 (size: 0x10)
    void ItemDropRequest(class UJSI_Slot_C* ItemRef, int32 Count, class UJSIContainer_C* Container);
    class UDropItemAmountSelector_C* AmountSelector;                                  // 0x02E0 (size: 0x8)
    double BlurValue;                                                                 // 0x02E8 (size: 0x8)

    void Show Vendor Amount Selected(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* ToContainerRef);
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, class UDragDropOperation* Operation);
    void Destruct();
    void Construct();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_DropItemBackGwidget(int32 EntryPoint);
    void ItemDropRequest__DelegateSignature(class UJSI_Slot_C* ItemRef, int32 Count, class UJSIContainer_C* Container);
}; // Size: 0x2F0

#endif
