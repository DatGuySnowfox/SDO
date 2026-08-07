#ifndef UE4SS_SDK_UDW_Material_State_Manager_HPP
#define UE4SS_SDK_UDW_Material_State_Manager_HPP

class UUDW_Material_State_Manager_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x00A8 (size: 0x8)
    class UUDS_Weather_Settings_C* Weather State;                                     // 0x00B0 (size: 0x8)
    class UUDW_Temperature_Manager_C* Temperature Manager;                            // 0x00B8 (size: 0x8)
    double Replicated Wetness;                                                        // 0x00C0 (size: 0x8)
    double Replicated Material Snow;                                                  // 0x00C8 (size: 0x8)
    double Replicated Material Dust;                                                  // 0x00D0 (size: 0x8)
    bool First Step;                                                                  // 0x00D8 (size: 0x1)
    double Wetness Change Speed;                                                      // 0x00E0 (size: 0x8)
    double Snow Change Speed;                                                         // 0x00E8 (size: 0x8)
    double Dust Change Speed;                                                         // 0x00F0 (size: 0x8)

    void Apply New State(double Snow, double Wetness, double Dust);
    void Update Replicated State();
    void Current Dust Change Speed(double& Out);
    void Current Wetness Change Speed(double& Out);
    void Current Snow Change Speed(double& Out);
    void Update Change Speeds();
    void Increment Material State(bool& Changed);
    void Start Material State Sim(class AUltra_Dynamic_Weather_C* UDW, class UUDS_Weather_Settings_C* Weather State, class UUDW_Temperature_Manager_C* Temp Manager);
    void Queue Speed Update();
    void ExecuteUbergraph_UDW_Material_State_Manager(int32 EntryPoint);
}; // Size: 0xF8

#endif
