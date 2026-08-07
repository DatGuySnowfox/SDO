#ifndef UE4SS_SDK_VehicleVendorButton_HPP
#define UE4SS_SDK_VehicleVendorButton_HPP

class UVehicleVendorButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* Border_0;                                                          // 0x02C8 (size: 0x8)
    class UButton* Button;                                                            // 0x02D0 (size: 0x8)
    class UTextBlock* PriceTxt;                                                       // 0x02D8 (size: 0x8)
    class UTextBlock* VehicleTxt;                                                     // 0x02E0 (size: 0x8)
    TSubclassOf<class AActor> Vehicle;                                                // 0x02E8 (size: 0x8)
    FText VehicleName;                                                                // 0x02F0 (size: 0x18)
    FText Price;                                                                      // 0x0308 (size: 0x18)
    class UVehicleTooltip_C* VehicleTooltip;                                          // 0x0320 (size: 0x8)
    class UTexture* Img;                                                              // 0x0328 (size: 0x8)

    class UWidget* GetToolTipWidget();
    FText GetPrice();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_2_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__VehicleVendorButton_Button_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_VehicleVendorButton(int32 EntryPoint);
}; // Size: 0x330

#endif
