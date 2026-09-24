#ifndef UE4SS_SDK_JigSplitWidget_HPP
#define UE4SS_SDK_JigSplitWidget_HPP

class UJigSplitWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USlider* SplitSlider;                                                       // 0x0348 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x0350 (size: 0x8)
    class UTextBlock* countText;                                                      // 0x0358 (size: 0x8)
    class UButton* ConfirmBtn;                                                        // 0x0360 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x0368 (size: 0x8)
    class UJSIContainer_C* ContainerRec;                                              // 0x0370 (size: 0x8)
    class UJSI_Slot_C* SlotRef;                                                       // 0x0378 (size: 0x8)
    int32 MaxCount;                                                                   // 0x0380 (size: 0x4)
    int32 CurrentValue;                                                               // 0x0384 (size: 0x4)
    int32 ToSlot;                                                                     // 0x0388 (size: 0x4)
    bool DroppedRotated?;                                                             // 0x038C (size: 0x1)

    void Construct();
    void BndEvt__SplitSlider_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void BndEvt__ConfirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_JigSplitWidget(int32 EntryPoint);
}; // Size: 0x38D

#endif
