#ifndef UE4SS_SDK_UDS_Climate_Preset_HPP
#define UE4SS_SDK_UDS_Climate_Preset_HPP

class UUDS_Climate_Preset_C : public UPrimaryDataAsset
{
    double Winter Average High Temp (C);                                              // 0x0030 (size: 0x8)
    double Winter Average Low Temp (C);                                               // 0x0038 (size: 0x8)
    double Spring Average High Temp (C);                                              // 0x0040 (size: 0x8)
    double Spring Average Low Temp (C);                                               // 0x0048 (size: 0x8)
    double Summer Average High Temp (C);                                              // 0x0050 (size: 0x8)
    double Summer Average Low Temp (C);                                               // 0x0058 (size: 0x8)
    double Autumn Average High Temp (C);                                              // 0x0060 (size: 0x8)
    double Autumn Average Low Temp (C);                                               // 0x0068 (size: 0x8)
    double Winter Cloudy Percentage;                                                  // 0x0070 (size: 0x8)
    double Spring Cloudy Percentage;                                                  // 0x0078 (size: 0x8)
    double Summer Cloudy Percentage;                                                  // 0x0080 (size: 0x8)
    double Autumn Cloudy Percentage;                                                  // 0x0088 (size: 0x8)
    double Winter Rainfall (mm);                                                      // 0x0090 (size: 0x8)
    double Spring Rainfall (mm);                                                      // 0x0098 (size: 0x8)
    double Summer Rainfall (mm);                                                      // 0x00A0 (size: 0x8)
    double Autumn Rainfall (mm);                                                      // 0x00A8 (size: 0x8)
    double Winter Snowfall (mm);                                                      // 0x00B0 (size: 0x8)
    double Spring Snowfall (mm);                                                      // 0x00B8 (size: 0x8)
    double Summer Snowfall (mm);                                                      // 0x00C0 (size: 0x8)
    double Autumn Snowfall (mm);                                                      // 0x00C8 (size: 0x8)
    double Dust/Sand Present;                                                         // 0x00D0 (size: 0x8)
    FText Data Source;                                                                // 0x00D8 (size: 0x18)

    void Return Temperature Range in Scale(TEnumAsByte<UDS_TemperatureType::Type> Scale, int32 Season, double& Low, double& High);
}; // Size: 0xF0

#endif
