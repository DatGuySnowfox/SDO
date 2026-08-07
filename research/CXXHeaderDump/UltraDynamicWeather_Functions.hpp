#ifndef UE4SS_SDK_UltraDynamicWeather_Functions_HPP
#define UE4SS_SDK_UltraDynamicWeather_Functions_HPP

class UUltraDynamicWeather_Functions_C : public UBlueprintFunctionLibrary
{

    void Change Wind Direction · 𝖴𝖣𝖶(double New Wind Direction, double Change Duration, class UObject* __WorldContext);
    void Flash Lightning · 𝖴𝖣𝖶(bool Use Custom Lightning Location, FVector Custom Lightning Location, FVector Custom Target Location, int32 Lightning Bolt Seed, class UObject* __WorldContext);
    void Get Season · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Season, TEnumAsByte<UDS_Season::Type>& Season Enum);
    void Change to Random Weather Variation · 𝖴𝖣𝖶(double Time to Transition to Random Weather (Seconds), class UObject* __WorldContext);
    void Change Weather · 𝖴𝖣𝖶(class UUDS_Weather_Settings_C* New Weather Type, double Time To Transition To New Weather (Seconds), class UObject* __WorldContext);
    void Get Display Name for Current Weather · 𝖴𝖣𝖶(class UObject* __WorldContext, FString& As String, TEnumAsByte<UDS_Weather_Display_Names::Type>& As Enumerator);
    void Get Current Temperature · 𝖴𝖣𝖶(TEnumAsByte<UDS_Temperature_Sample_Location::Type> Sample Location, FVector Custom Sample Location, TEnumAsByte<UDS_TemperatureType::Type> Scale, class UObject* __WorldContext, double& Temperature);
    void Get Local Weather State · 𝖴𝖣𝖶(class UObject* __WorldContext, class UUDS_Weather_Settings_C*& Local Weather State);
    void Get Wind Direction Vector · 𝖴𝖣𝖶(class UObject* __WorldContext, FVector& Wind Direction);
    void Get Material Dust Coverage · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Material Dust Coverage);
    void Get Material Snow Coverage · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Material Snow Coverage);
    void Get Material Wetness · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Material Wetness);
    void Get Dust/Sand Amount · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Dust);
    void Get Fog · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Fog);
    void Get Thunder/Lightning · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Thunder/Lightning);
    void Get Snow Amount · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Snow);
    void Get Rain Amount · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Rain);
    void Get Wind Intensity · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Wind Intensity);
    void Get Cloud Coverage · 𝖴𝖣𝖶(class UObject* __WorldContext, double& Cloud Coverage);
    void Get Ultra Dynamic Weather(class UObject* __WorldContext, class AUltra_Dynamic_Weather_C*& UDW, bool& Valid);
}; // Size: 0x28

#endif
