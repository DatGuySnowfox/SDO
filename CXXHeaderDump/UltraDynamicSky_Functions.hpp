#ifndef UE4SS_SDK_UltraDynamicSky_Functions_HPP
#define UE4SS_SDK_UltraDynamicSky_Functions_HPP

class UUltraDynamicSky_Functions_C : public UBlueprintFunctionLibrary
{

    void Apply Saved UDS and UDW State · 𝖴𝖣𝖲(FUDS_and_UDW_State State, class UObject* __WorldContext);
    void Create UDS and UDW State for Saving · 𝖴𝖣𝖲(class UObject* __WorldContext, FUDS_and_UDW_State& Packaged State);
    void Random Value in Float Range Structure(FFloatRange Range, FRandomStream Stream, class UObject* __WorldContext, double& Out);
    void Transition Time of Day · 𝖴𝖣𝖲(double New Time of Day, double Transition Duration (Seconds), TEnumAsByte<EEasingFunc::Type> Easing Function, double Easing Exponent, bool Allow Time Going Backwards, class UObject* __WorldContext);
    void Set Date and Time · 𝖴𝖣𝖲(FDateTime DateTime, class UObject* __WorldContext);
    void Set Time with Time Code · 𝖴𝖣𝖲(FTimecode Timecode, class UObject* __WorldContext);
    void Set Time of Day · 𝖴𝖣𝖲(double Time of Day, class UObject* __WorldContext);
    void Get Day of the Week · 𝖴𝖣𝖲(class UObject* __WorldContext, int32& Index, FString& Name);
    void Is It Daytime? · 𝖴𝖣𝖲(class UObject* __WorldContext, bool& Daytime);
    void Get TimeCode · 𝖴𝖣𝖲(class UObject* __WorldContext, FTimecode& Timecode);
    void Get DateTime · 𝖴𝖣𝖲(class UObject* __WorldContext, FDateTime& DateTime);
    void Get Time of Day · 𝖴𝖣𝖲(class UObject* __WorldContext, double& Time of Day);
    void Get Ultra Dynamic Sky(class UObject* __WorldContext, class AUltra_Dynamic_Sky_C*& UDS, bool& Valid);
}; // Size: 0x28

#endif
