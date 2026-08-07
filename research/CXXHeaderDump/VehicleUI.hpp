#ifndef UE4SS_SDK_VehicleUI_HPP
#define UE4SS_SDK_VehicleUI_HPP

class UVehicleUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* OnConstruct;                                              // 0x02C8 (size: 0x8)
    class UProgressBar* FuelBar;                                                      // 0x02D0 (size: 0x8)
    class UProgressBar* HealthBar;                                                    // 0x02D8 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x02E0 (size: 0x8)
    class UTextBlock* SpeedText;                                                      // 0x02E8 (size: 0x8)
    class ABP_VehicleMaster_C* Vehicle;                                               // 0x02F0 (size: 0x8)

    FText GetName();
    void Construct();
    void UpdateFuel(double Fuel, double MaxFuel);
    void UpdateHealth(double Health, double MaxHealth);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_VehicleUI(int32 EntryPoint);
}; // Size: 0x2F8

#endif
