#ifndef UE4SS_SDK_WeatherMask_HPP
#define UE4SS_SDK_WeatherMask_HPP

class UWeatherMask_C : public UStaticMeshComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0630 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x0638 (size: 0x8)
    double Mask Snow/Dust;                                                            // 0x0640 (size: 0x8)
    double Mask Wetness;                                                              // 0x0648 (size: 0x8)
    FVector2D Brush Location;                                                         // 0x0650 (size: 0x10)
    FVector2D Brush Scale;                                                            // 0x0660 (size: 0x10)
    double Brush Yaw;                                                                 // 0x0670 (size: 0x8)
    FVector2D Center Location;                                                        // 0x0678 (size: 0x10)
    double Max Distance;                                                              // 0x0688 (size: 0x8)
    bool Ready for Drawing;                                                           // 0x0690 (size: 0x1)
    bool Spawned At Runtime;                                                          // 0x0691 (size: 0x1)

    void Configure Collision();
    void Calculate Masking At Location(FVector Location, FVector2D& Mask, bool& Cancel All Masks);
    void Enable();
    void Disable();
    void Force Update();
    void Editor Update();
    void Get Max Distance(double& Out);
    void Get Brush Yaw(double& Out);
    void Get Center Location(FVector2D& Out);
    void Get Brush Location(FVector2D& Out);
    void Get Brush Scale(FVector2D& Out);
    void Prepare for Drawing(bool Runtime, class AUltra_Dynamic_Weather_C* UDW);
    bool Component Generally In Range();
    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void ExecuteUbergraph_WeatherMask(int32 EntryPoint);
}; // Size: 0x692

#endif
