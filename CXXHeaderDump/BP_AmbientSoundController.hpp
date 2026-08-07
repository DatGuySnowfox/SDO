#ifndef UE4SS_SDK_BP_AmbientSoundController_HPP
#define UE4SS_SDK_BP_AmbientSoundController_HPP

class ABP_AmbientSoundController_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UUDS_PlayerOcclusion_C* UDS_PlayerOcclusion;                                // 0x02A8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02B0 (size: 0x8)
    float Tick Rate;                                                                  // 0x02B8 (size: 0x4)
    bool Day;                                                                         // 0x02BC (size: 0x1)
    class USoundBase* DaySound;                                                       // 0x02C0 (size: 0x8)
    bool Raining;                                                                     // 0x02C8 (size: 0x1)
    bool Snowing;                                                                     // 0x02C9 (size: 0x1)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x02D0 (size: 0x8)
    class AUltra_Dynamic_Sky_C* UDS;                                                  // 0x02D8 (size: 0x8)
    bool Runtime;                                                                     // 0x02E0 (size: 0x1)
    class USoundBase* NightSound;                                                     // 0x02E8 (size: 0x8)
    bool Foggy;                                                                       // 0x02F0 (size: 0x1)
    double Target Volume Multiplier;                                                  // 0x02F8 (size: 0x8)
    double Daytime Volume Multiplier;                                                 // 0x0300 (size: 0x8)
    double Nighttime Volume Multiplier;                                               // 0x0308 (size: 0x8)
    bool Dusty;                                                                       // 0x0310 (size: 0x1)
    bool No UDS or UDW;                                                               // 0x0311 (size: 0x1)
    double No Weather Volume Multiplier;                                              // 0x0318 (size: 0x8)
    double Rainy Volume Multiplier;                                                   // 0x0320 (size: 0x8)
    double Snowy Volume Multiplier;                                                   // 0x0328 (size: 0x8)
    double Dusty Volume Multiplier;                                                   // 0x0330 (size: 0x8)
    double Foggy Volume Multiplier;                                                   // 0x0338 (size: 0x8)

    void State Change Fog();
    void Check if UDS and UDW Are Both Gone();
    void UDS End Play(class AActor* Actor, TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void UDW End Play(class AActor* Actor, TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Start Up Sound();
    void Get UDS and UDW Reference();
    void Get Starting Dispatchers State();
    void Bind to Dispatchers();
    void Finished Dust();
    void Started Dust();
    void Update Volume Multiplier();
    void Finished Raining();
    void Finished Snowing();
    void Started Raining();
    void Started Snowing();
    void Sunset();
    void Sunrise();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void UDS Starting Up(class AUltra_Dynamic_Sky_C* UDS);
    void UDW Starting Up(class AUltra_Dynamic_Weather_C* UDW);
    void UDS Ending Play();
    void UDW Ending Play();
    void ExecuteUbergraph_BP_AmbientSoundController(int32 EntryPoint);
}; // Size: 0x340

#endif
