#ifndef UE4SS_SDK_RandomWeatherVariation_State_HPP
#define UE4SS_SDK_RandomWeatherVariation_State_HPP

struct FRandomWeatherVariation_State
{
    TEnumAsByte<UDS_RandomWeatherTiming::Type> Mode_29_E53E8F9C40E9C74B1489D0A581FF28D2; // 0x0000 (size: 0x1)
    class UObject* TargetRandomWeatherType_37_BBFDBC5E46C25F1DFB0037A44AB6D268;       // 0x0008 (size: 0x8)
    double CurrentLerpAlpha_38_659CCE944698D027E31F1C9F648EBD85;                      // 0x0010 (size: 0x8)
    double CurrentTimerLength_39_6CC4C61F4528647B419BE189C0944ADF;                    // 0x0018 (size: 0x8)
    double ChangeTimer_40_28CED07F4F2AE061871DB595D0DD4C66;                           // 0x0020 (size: 0x8)
    class UObject* LastRandomWeatherType_41_4367B1AC4E64A2564E3960B3ED6E25D8;         // 0x0028 (size: 0x8)
    TArray<class UObject*> Forecast_36_FB829F034AD4245050A506B528B86CC0;              // 0x0030 (size: 0x10)

}; // Size: 0x40

#endif
