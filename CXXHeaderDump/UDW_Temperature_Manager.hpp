#ifndef UE4SS_SDK_UDW_Temperature_Manager_HPP
#define UE4SS_SDK_UDW_Temperature_Manager_HPP

class UUDW_Temperature_Manager_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x00A8 (size: 0x8)
    class UUDS_Weather_Settings_C* Weather State;                                     // 0x00B0 (size: 0x8)
    FVector2D Temperature Range;                                                      // 0x00B8 (size: 0x10)
    double Update Period;                                                             // 0x00C8 (size: 0x8)
    bool For WOV;                                                                     // 0x00D0 (size: 0x1)
    class AVolume* Volume;                                                            // 0x00D8 (size: 0x8)
    double Last Temperature Update Time;                                              // 0x00E0 (size: 0x8)
    double Last Temperature;                                                          // 0x00E8 (size: 0x8)
    double Target Temperature;                                                        // 0x00F0 (size: 0x8)

    void Calculate Temperature(class UUDS_Weather_Settings_C* Settings, double& Temperature);
    void Get Temperature(double& Out);
    void Get Current Min and Max Temperature(FVector2D Spring Min and Max, FVector2D Summer Min and Max, FVector2D Autumn Min and Max, FVector2D Winter Min and Max, FVector2D& Range);
    void Update Temperature Range();
    void Update Target Temperature();
    void Runtime Start Temperature Manager();
    void Set Up Temperature Manager(class AUltra_Dynamic_Weather_C* UDW, class UUDS_Weather_Settings_C* Weather State);
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void ExecuteUbergraph_UDW_Temperature_Manager(int32 EntryPoint);
}; // Size: 0xF8

#endif
