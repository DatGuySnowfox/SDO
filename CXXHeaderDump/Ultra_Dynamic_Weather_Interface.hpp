#ifndef UE4SS_SDK_Ultra_Dynamic_Weather_Interface_HPP
#define UE4SS_SDK_Ultra_Dynamic_Weather_Interface_HPP

class IUltra_Dynamic_Weather_Interface_C : public IInterface
{

    void UDS Reconstruct(bool& Success);
    void UDW Runtime Tick(double Delta Time);
    void Report Removed Radial Storm(class AActor* Storm, bool& Success);
    void Get UDS Weather Override Bool(bool& Cloud Coverage, bool& Fog, bool& Dust);
    void UDS Weather Variable Overrides(bool Override Clouds, double Cloud Coverage, bool Override Fog, double Fog, bool Override Dust, double Dust, bool& Success);
    void Get UDS Values Controlled by UDW(double& Cloud Coverage, double& Fog, double& Dust Amount, double& Cloud Direction, double& Wind Speed Multiplier, double& Fog Vertical Velocity);
    void Editor Tick(FVector Editor Camera Location, FRotator Editor Camera Rotation, double Delta Time, bool& Completed);
    void Get Local Weather State Values(double& Cloud Coverage, double& Wind Intensity, double& Rain, double& Snow, double& Dust, double& Fog, double& Thunder/Lightning);
    void Get Control Point Location(FVector& Location);
    void UDW State Apply(FUDS_and_UDW_State State, bool& Completed);
    void Get UDW State for Saving(FUDS_and_UDW_State& UDW State);
    void Initialize Weather(class AUltra_Dynamic_Sky_C* UDS);
}; // Size: 0x28

#endif
