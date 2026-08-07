#ifndef UE4SS_SDK_UDS_Weather_Settings_HPP
#define UE4SS_SDK_UDS_Weather_Settings_HPP

class UUDS_Weather_Settings_C : public UPrimaryDataAsset
{
    double Cloud Coverage;                                                            // 0x0030 (size: 0x8)
    double Rain;                                                                      // 0x0038 (size: 0x8)
    double Snow;                                                                      // 0x0040 (size: 0x8)
    double Thunder/Lightning;                                                         // 0x0048 (size: 0x8)
    double Wind Intensity;                                                            // 0x0050 (size: 0x8)
    double Fog;                                                                       // 0x0058 (size: 0x8)
    double Dust;                                                                      // 0x0060 (size: 0x8)
    double Material Wetness;                                                          // 0x0068 (size: 0x8)
    double Material Snow Coverage;                                                    // 0x0070 (size: 0x8)
    double Material Dust Coverage;                                                    // 0x0078 (size: 0x8)
    TSoftObjectPtr<UTexture2D> Icon Texture;                                          // 0x0080 (size: 0x28)
    FName User Friendly Name;                                                         // 0x00A8 (size: 0x8)
    TMap<UUDS_Weather_Settings_C*, double> State Sources;                             // 0x00B0 (size: 0x50)

    void Get State Sources(TMap<UUDS_Weather_Settings_C*, double>& Sources);
    void Custom Weather Behavior(double Alpha, class AActor* UDS, class AActor* UDW);
}; // Size: 0x100

#endif
