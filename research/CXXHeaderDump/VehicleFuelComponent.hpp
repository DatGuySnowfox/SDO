#ifndef UE4SS_SDK_VehicleFuelComponent_HPP
#define UE4SS_SDK_VehicleFuelComponent_HPP

class UVehicleFuelComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    FTimerHandle FuelRestore_Timer;                                                   // 0x00C0 (size: 0x8)
    double CurrentFuel;                                                               // 0x00C8 (size: 0x8)
    double MaxFuel;                                                                   // 0x00D0 (size: 0x8)
    double WasteTime;                                                                 // 0x00D8 (size: 0x8)
    double WasteOfFuelPerTime;                                                        // 0x00E0 (size: 0x8)
    bool Empty;                                                                       // 0x00E8 (size: 0x1)
    FVehicleFuelComponent_CFuelUpdate FuelUpdate;                                     // 0x00F0 (size: 0x10)
    void FuelUpdate(double FuelInput);
    class ABP_VehicleMaster_C* Vehicle;                                               // 0x0100 (size: 0x8)

    void UpdateUIFuel(double Fuel, double MaxFuel);
    void FuelSpend();
    void StartFuelSpend();
    void AddFuel(double FuelPercent);
    void StopSpendFuel();
    void LoadComponent();
    void ExecuteUbergraph_VehicleFuelComponent(int32 EntryPoint);
    void FuelUpdate__DelegateSignature(double FuelInput);
}; // Size: 0x108

#endif
