#ifndef UE4SS_SDK_WBP_QuantitySelector_HPP
#define UE4SS_SDK_WBP_QuantitySelector_HPP

class UWBP_QuantitySelector_C : public UWBP_NarrativeMenu_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0448 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Instruction;                              // 0x0450 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Max;                                      // 0x0458 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Min;                                      // 0x0460 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Quantity;                                 // 0x0468 (size: 0x8)
    class UAnalogSlider* Slider_Quantity;                                             // 0x0470 (size: 0x8)
    int32 MinAmount;                                                                  // 0x0478 (size: 0x4)
    int32 MaxAmount;                                                                  // 0x047C (size: 0x4)
    FDataTableRowHandle ConfirmBinding;                                               // 0x0480 (size: 0x10)
    FInputActionBindingHandle ConfirmHandle;                                          // 0x0490 (size: 0x4)
    FWBP_QuantitySelector_COnConfirmed OnConfirmed;                                   // 0x0498 (size: 0x10)
    void OnConfirmed(int32 Quantity);
    FText InstructionText;                                                            // 0x04A8 (size: 0x18)
    int32 SelectedAmount;                                                             // 0x04C0 (size: 0x4)

    void OnConfirm(FName ActionName);
    void RegisterActions();
    class UWidget* BP_GetDesiredFocusTarget();
    void BndEvt__WBP_QuantitySelector_Slider_Quantity_K2Node_ComponentBoundEvent_0_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void Destruct();
    void Initialize(int32 MinAmount, int32 MaxAmount, FText InstructionText);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void Update Drop Amount(double A);
    void ExecuteUbergraph_WBP_QuantitySelector(int32 EntryPoint);
    void OnConfirmed__DelegateSignature(int32 Quantity);
}; // Size: 0x4C4

#endif
