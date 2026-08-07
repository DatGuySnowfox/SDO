#ifndef UE4SS_SDK_DropItemAmountSelector_HPP
#define UE4SS_SDK_DropItemAmountSelector_HPP

class UDropItemAmountSelector_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x02C8 (size: 0x8)
    class UButton* confirmBtn;                                                        // 0x02D0 (size: 0x8)
    class UTextBlock* CountText;                                                      // 0x02D8 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x02E0 (size: 0x8)
    class USlider* Slider_0;                                                          // 0x02E8 (size: 0x8)
    class UDropItemBackGwidget_C* DropBackGRef;                                       // 0x02F0 (size: 0x8)
    class UJSI_Slot_C* ItemRef;                                                       // 0x02F8 (size: 0x8)
    int32 MaxCount;                                                                   // 0x0300 (size: 0x4)
    int32 CurrentValue;                                                               // 0x0304 (size: 0x4)
    bool IsVendor?;                                                                   // 0x0308 (size: 0x1)
    class UJSIContainer_C* ToContainerRef;                                            // 0x0310 (size: 0x8)

    void SetMax(int32& OutMax);
    void GetMaxCount(int32 Max, int32& FinalCount);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Construct();
    void BndEvt__Slider_0_K2Node_ComponentBoundEvent_0_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__confirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_DropItemAmountSelector(int32 EntryPoint);
}; // Size: 0x318

#endif
