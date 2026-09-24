#ifndef UE4SS_SDK_WBP_QuantitySelector_HPP
#define UE4SS_SDK_WBP_QuantitySelector_HPP

class UWBP_QuantitySelector_C : public UWBP_NarrativeMenu_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x04E8 (size: 0x8)
    class UAnalogSlider* Slider_Quantity;                                             // 0x04F0 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Quantity;                                 // 0x04F8 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Min;                                      // 0x0500 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Max;                                      // 0x0508 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_Instruction;                              // 0x0510 (size: 0x8)
    int32 MinAmount;                                                                  // 0x0518 (size: 0x4)
    int32 MaxAmount;                                                                  // 0x051C (size: 0x4)
    FDataTableRowHandle ConfirmBinding;                                               // 0x0520 (size: 0x10)
    FInputActionBindingHandle ConfirmHandle;                                          // 0x0530 (size: 0x4)
    FWBP_QuantitySelector_COnConfirmed OnConfirmed;                                   // 0x0538 (size: 0x10)
    void OnConfirmed(int32 Quantity);
    FText InstructionText;                                                            // 0x0548 (size: 0x10)
    int32 SelectedAmount;                                                             // 0x0558 (size: 0x4)

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
}; // Size: 0x55C

#endif
