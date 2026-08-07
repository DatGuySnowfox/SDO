#ifndef UE4SS_SDK_Random_Weather_Variation_HPP
#define UE4SS_SDK_Random_Weather_Variation_HPP

class URandom_Weather_Variation_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x00A8 (size: 0x8)
    class UUDS_Weather_Settings_C* Target Random Weather Type;                        // 0x00B0 (size: 0x8)
    double Current Lerp Alpha;                                                        // 0x00B8 (size: 0x8)
    double Current Timer Length;                                                      // 0x00C0 (size: 0x8)
    class AWeather_Override_Volume_C* Volume;                                         // 0x00C8 (size: 0x8)
    class UUDS_Weather_Settings_C* Last Random Weather Type;                          // 0x00D0 (size: 0x8)
    class UUDS_Weather_Settings_C* Current Random Weather State;                      // 0x00D8 (size: 0x8)
    double Change Timer;                                                              // 0x00E0 (size: 0x8)
    bool Started;                                                                     // 0x00E8 (size: 0x1)
    double Eased Lerp Alpha;                                                          // 0x00F0 (size: 0x8)
    bool Changing Weather;                                                            // 0x00F8 (size: 0x1)
    bool For WOV;                                                                     // 0x00F9 (size: 0x1)
    TArray<class UUDS_Weather_Settings_C*> Weather Forecast;                          // 0x0100 (size: 0x10)
    bool First Type Selected;                                                         // 0x0110 (size: 0x1)

    void Instant Material Update();
    void Get Material State Manager(class UUDW_Material_State_Manager_C*& Material State Manager);
    void Check For Season Instant Refresh();
    void Clear and Restart();
    void Recalculate Forecast();
    void Fill Weather Forecast();
    void Select Random Weather Type(TMap<UUDS_Weather_Settings_C*, double> Probability Map, class UUDS_Weather_Settings_C* Previous Weather, class UUDS_Weather_Settings_C*& Out);
    void Filter Probability Map(TMap<UUDS_Weather_Settings_C*, double> Probability Map, class UUDS_Weather_Settings_C* Current Random Type, TMap<UUDS_Weather_Settings_C*, double>& Filtered Probability Map);
    void Set New Random Interval Timer();
    void Get Season Probability Map(int32 Season, TMap<UUDS_Weather_Settings_C*, double>& Map);
    void Random Weather Mode(TEnumAsByte<UDS_RandomWeatherTiming::Type>& Mode);
    double Get Current Lerp Alpha();
    void Hourly(int32 Hour);
    void Current Hour Changed(int32 Hour);
    void Bind to UDS();
    void State Source Map(TMap<UUDS_Weather_Settings_C*, double>& Source Map);
    void Start Up Random Weather Variation(class AUltra_Dynamic_Weather_C* UDW, class AWeather_Override_Volume_C* Volume, bool Start with a Random Type, class UUDS_Weather_Settings_C* Starting Weather Settings);
    void Make Climate Probability Map(class UUDS_Climate_Preset_C* Climate, int32 Season, TMap<UUDS_Weather_Settings_C*, double>& Probability Map);
    void Get Climate Temperature Ranges(class UUDS_Climate_Preset_C* Climate Object, TEnumAsByte<UDS_TemperatureType::Type> Scale, FVector2D& Summer Range, FVector2D& Autumn Range, FVector2D& Winter Range, FVector2D& Spring Range);
    void Create Composite Probability Map(TMap<UUDS_Weather_Settings_C*, double>& Out);
    void Apply State(FRandomWeatherVariation_State Random Weather State);
    void Get State for Saving(FRandomWeatherVariation_State& State);
    void Change to Next Random Weather Type();
    void Increment Change Timer();
    void Increment Random Weather();
    void ReceiveTick(float DeltaSeconds);
    void Reset Timer();
    void ExecuteUbergraph_Random_Weather_Variation(int32 EntryPoint);
}; // Size: 0x111

#endif
