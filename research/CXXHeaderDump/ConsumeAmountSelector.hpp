#ifndef UE4SS_SDK_ConsumeAmountSelector_HPP
#define UE4SS_SDK_ConsumeAmountSelector_HPP

class UConsumeAmountSelector_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USpinBox* SpinBox;                                                          // 0x0348 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x0350 (size: 0x8)
    class UButton* ConfirmBtn;                                                        // 0x0358 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x0360 (size: 0x8)
    class UJSI_Slot_C* ItemRef;                                                       // 0x0368 (size: 0x8)
    int32 MaxCount;                                                                   // 0x0370 (size: 0x4)
    int32 CurrentValue;                                                               // 0x0374 (size: 0x4)
    FConsumeAmountSelector_COnConsumeConfirm OnConsumeConfirm;                        // 0x0378 (size: 0x10)
    void OnConsumeConfirm(class UJSI_Slot_C* ItemRef, class UJSI_Slot_C* ItemToConsume, class UJSIContainer_C* Rootcon, int32 Amount);
    FConsumeAmountSelector_COnsumeCanceled OnsumeCanceled;                            // 0x0388 (size: 0x10)
    void OnsumeCanceled();
    class UJSIContainer_C* RootContainer;                                             // 0x0398 (size: 0x8)
    class UJSI_Slot_C* ItemToConsume;                                                 // 0x03A0 (size: 0x8)

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Construct();
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__ConfirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DropItemAmountSelector_SpinBox_143_K2Node_ComponentBoundEvent_9_OnSpinBoxValueChangedEvent__DelegateSignature(float InValue);
    void ExecuteUbergraph_ConsumeAmountSelector(int32 EntryPoint);
    void OnsumeCanceled__DelegateSignature();
    void OnConsumeConfirm__DelegateSignature(class UJSI_Slot_C* ItemRef, class UJSI_Slot_C* ItemToConsume, class UJSIContainer_C* Rootcon, int32 Amount);
}; // Size: 0x3A8

#endif
