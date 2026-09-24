#ifndef UE4SS_SDK_DropItemAmountSelector_HPP
#define UE4SS_SDK_DropItemAmountSelector_HPP

class UDropItemAmountSelector_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USpinBox* SpinBox;                                                          // 0x0348 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x0350 (size: 0x8)
    class UButton* ConfirmBtn;                                                        // 0x0358 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x0360 (size: 0x8)
    class UDropItemBackGwidget_C* DropBackGRef;                                       // 0x0368 (size: 0x8)
    class UJSI_Slot_C* ItemRef;                                                       // 0x0370 (size: 0x8)
    int32 MaxCount;                                                                   // 0x0378 (size: 0x4)
    int32 CurrentValue;                                                               // 0x037C (size: 0x4)
    bool IsVendor?;                                                                   // 0x0380 (size: 0x1)
    class UJSIContainer_C* ToContainerRef;                                            // 0x0388 (size: 0x8)

    void SetMax(int32& OutMax);
    void GetMaxCount(int32 max, int32& FinalCount);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Construct();
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__ConfirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DropItemAmountSelector_SpinBox_143_K2Node_ComponentBoundEvent_9_OnSpinBoxValueChangedEvent__DelegateSignature(float InValue);
    void ExecuteUbergraph_DropItemAmountSelector(int32 EntryPoint);
}; // Size: 0x390

#endif
