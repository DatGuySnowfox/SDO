#ifndef UE4SS_SDK_JigSplitWidget_HPP
#define UE4SS_SDK_JigSplitWidget_HPP

class UJigSplitWidget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* CancelBtn;                                                         // 0x02C8 (size: 0x8)
    class UButton* confirmBtn;                                                        // 0x02D0 (size: 0x8)
    class UTextBlock* CountText;                                                      // 0x02D8 (size: 0x8)
    class UTextBlock* itemNametxt;                                                    // 0x02E0 (size: 0x8)
    class USlider* SplitSlider;                                                       // 0x02E8 (size: 0x8)
    class UJSIContainer_C* ContainerRec;                                              // 0x02F0 (size: 0x8)
    class UJSI_Slot_C* SlotRef;                                                       // 0x02F8 (size: 0x8)
    int32 MaxCount;                                                                   // 0x0300 (size: 0x4)
    int32 CurrentValue;                                                               // 0x0304 (size: 0x4)
    int32 ToSlot;                                                                     // 0x0308 (size: 0x4)
    bool DroppedRotated?;                                                             // 0x030C (size: 0x1)

    void Construct();
    void BndEvt__SplitSlider_K2Node_ComponentBoundEvent_1_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void BndEvt__confirmBtn_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__CancelBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_JigSplitWidget(int32 EntryPoint);
}; // Size: 0x30D

#endif
