#ifndef UE4SS_SDK_ConsumeAmountSelector_HPP
#define UE4SS_SDK_ConsumeAmountSelector_HPP

class UConsumeAmountSelector_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x02C8 (size: 0x8)
    class UButton* confirmBtn;                                                        // 0x02D0 (size: 0x8)
    class UTextBlock* CountText;                                                      // 0x02D8 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x02E0 (size: 0x8)
    class USlider* Slider_0;                                                          // 0x02E8 (size: 0x8)
    class UJSI_Slot_C* ItemRef;                                                       // 0x02F0 (size: 0x8)
    int32 MaxCount;                                                                   // 0x02F8 (size: 0x4)
    int32 CurrentValue;                                                               // 0x02FC (size: 0x4)
    FConsumeAmountSelector_COnConsumeConfirm OnConsumeConfirm;                        // 0x0300 (size: 0x10)
    void OnConsumeConfirm(class UJSI_Slot_C* ItemRef, class UJSI_Slot_C* ItemToConsume, class UJSIContainer_C* Rootcon, int32 Amount);
    FConsumeAmountSelector_COnsumeCanceled OnsumeCanceled;                            // 0x0310 (size: 0x10)
    void OnsumeCanceled();
    class UJSIContainer_C* RootContainer;                                             // 0x0320 (size: 0x8)
    class UJSI_Slot_C* ItemToConsume;                                                 // 0x0328 (size: 0x8)

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void BndEvt__Slider_0_K2Node_ComponentBoundEvent_0_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__confirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_ConsumeAmountSelector(int32 EntryPoint);
    void OnsumeCanceled__DelegateSignature();
    void OnConsumeConfirm__DelegateSignature(class UJSI_Slot_C* ItemRef, class UJSI_Slot_C* ItemToConsume, class UJSIContainer_C* Rootcon, int32 Amount);
}; // Size: 0x330

#endif
