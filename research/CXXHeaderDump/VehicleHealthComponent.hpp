#ifndef UE4SS_SDK_VehicleHealthComponent_HPP
#define UE4SS_SDK_VehicleHealthComponent_HPP

class UVehicleHealthComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double CurrentHealth;                                                             // 0x00C0 (size: 0x8)
    double MaxHealth;                                                                 // 0x00C8 (size: 0x8)
    FVehicleHealthComponent_CFuelUpdate FuelUpdate;                                   // 0x00D0 (size: 0x10)
    void FuelUpdate(double FuelInput);
    class ABP_VehicleMaster_C* Vehicle;                                               // 0x00E0 (size: 0x8)

    void UpdateUIHealth(double Health, double MaxHealth);
    void AddHealth(double HealthPercent);
    void LoadComponent();
    void ExecuteUbergraph_VehicleHealthComponent(int32 EntryPoint);
    void FuelUpdate__DelegateSignature(double FuelInput);
}; // Size: 0xE8

#endif
