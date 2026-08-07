#ifndef UE4SS_SDK_UDW_WOV_State_HPP
#define UE4SS_SDK_UDW_WOV_State_HPP

struct FUDW_WOV_State
{
    FRandomWeatherVariation_State RandomWeatherState_33_83DFCBE54A683EDAC05BE790FB5F3EA0; // 0x0000 (size: 0x40)
    FUDW_WeatherState_Structure WeatherState_34_4FFDBC3242825222AAC2499AB18E9F1F;     // 0x0040 (size: 0x50)
    bool TransitioningWeather_35_30791493496F900274FC4E992E495ED2;                    // 0x0090 (size: 0x1)
    double TransitionTimeRemaining_36_D121C14845888F83FB2D79B02E8E7C1F;               // 0x0098 (size: 0x8)
    double TransitionTimerLength_37_EA0E1D744D16061342667B81CE2960BF;                 // 0x00A0 (size: 0x8)
    bool ChangingFromRandomWeather_38_81A770004EE629A4300BE98B923413E2;               // 0x00A8 (size: 0x1)
    bool ChangingToRandomWeather_39_88E2A2CD468A5A629EB814ADE20B933A;                 // 0x00A9 (size: 0x1)
    class UObject* Weather_25_CD874C774ED188AF4D11E58D7A34175A;                       // 0x00B0 (size: 0x8)
    class UObject* TransitionWeatherA_40_258D58714970D7F4E69BB8BD5521E9B5;            // 0x00B8 (size: 0x8)
    class UObject* TransitionWeatherB_41_51640EB94AA5AC815DC5569687237618;            // 0x00C0 (size: 0x8)
    double VolumeAlpha_42_4C5D2EB945C4BC5FFA0D688B3137FD43;                           // 0x00C8 (size: 0x8)

}; // Size: 0xD0

#endif
