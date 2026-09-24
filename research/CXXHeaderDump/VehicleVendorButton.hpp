#ifndef UE4SS_SDK_VehicleVendorButton_HPP
#define UE4SS_SDK_VehicleVendorButton_HPP

class UVehicleVendorButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* VehicleTxt;                                                     // 0x0348 (size: 0x8)
    class UTextBlock* PriceTxt;                                                       // 0x0350 (size: 0x8)
    class UButton* Button;                                                            // 0x0358 (size: 0x8)
    class UBorder* Border_0;                                                          // 0x0360 (size: 0x8)
    TSubclassOf<class AActor> Vehicle;                                                // 0x0368 (size: 0x8)
    FText VehicleName;                                                                // 0x0370 (size: 0x10)
    FText Price;                                                                      // 0x0380 (size: 0x10)
    class UVehicleTooltip_C* VehicleTooltip;                                          // 0x0390 (size: 0x8)
    class UTexture* Img;                                                              // 0x0398 (size: 0x8)

    class UWidget* GetToolTipWidget();
    FText GetPrice();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_2_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_VehicleVendorButton(int32 EntryPoint);
}; // Size: 0x3A0

#endif
