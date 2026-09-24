#ifndef UE4SS_SDK_VehicleUI_HPP
#define UE4SS_SDK_VehicleUI_HPP

class UVehicleUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* SpeedText;                                                      // 0x0348 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x0350 (size: 0x8)
    class UProgressBar* HealthBar;                                                    // 0x0358 (size: 0x8)
    class UProgressBar* FuelBar;                                                      // 0x0360 (size: 0x8)
    class UBorder* Border;                                                            // 0x0368 (size: 0x8)
    class UWidgetAnimation* OnConstruct;                                              // 0x0370 (size: 0x8)
    class ABP_VehicleMaster_C* Vehicle;                                               // 0x0378 (size: 0x8)

    FText GetName();
    void Construct();
    void UpdateFuel(double Fuel, double MaxFuel);
    void UpdateHealth(double Health, double MaxHealth);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_VehicleUI(int32 EntryPoint);
}; // Size: 0x380

#endif
