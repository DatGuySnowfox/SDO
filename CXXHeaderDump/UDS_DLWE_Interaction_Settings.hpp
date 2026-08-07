#ifndef UE4SS_SDK_UDS_DLWE_Interaction_Settings_HPP
#define UE4SS_SDK_UDS_DLWE_Interaction_Settings_HPP

class UUDS_DLWE_Interaction_Settings_C : public UPrimaryDataAsset
{
    bool Affects DLWE Snow/Dust;                                                      // 0x0030 (size: 0x1)
    bool Affects DLWE Puddles;                                                        // 0x0031 (size: 0x1)
    bool Affect Puddle Fluid Volumes;                                                 // 0x0032 (size: 0x1)
    double Active Distance;                                                           // 0x0038 (size: 0x8)
    double Min Tick Interval (Close);                                                 // 0x0040 (size: 0x8)
    double Max Tick Interval (Far);                                                   // 0x0048 (size: 0x8)
    bool Enable Puddle Sound Effects;                                                 // 0x0050 (size: 0x1)
    bool Enable Snow Sound Effects;                                                   // 0x0051 (size: 0x1)
    bool Enable Dust Sound Effects;                                                   // 0x0052 (size: 0x1)
    bool Enable Puddle Splash Particles;                                              // 0x0053 (size: 0x1)
    bool Enable Snow Particles;                                                       // 0x0054 (size: 0x1)
    double Snow Sound Volume;                                                         // 0x0058 (size: 0x8)
    double Puddle Sound Volume;                                                       // 0x0060 (size: 0x8)
    double Dust Sound Volume;                                                         // 0x0068 (size: 0x8)
    double Snow/Dust Draw Threshold;                                                  // 0x0070 (size: 0x8)
    double Puddle Ripple Size;                                                        // 0x0078 (size: 0x8)
    double Puddle Draw Threshold;                                                     // 0x0080 (size: 0x8)
    double Material Check Distance;                                                   // 0x0088 (size: 0x8)
    double Collision Trace Movement Threshold;                                        // 0x0090 (size: 0x8)
    TArray<class UPhysicalMaterial*> Physical Materials which enable DLWE Interactions on non-Landscapes; // 0x0098 (size: 0x10)
    TMap<class UPhysicalMaterial*, class USoundBase*> General Surface Impact Sounds;  // 0x00A8 (size: 0x50)

}; // Size: 0xF8

#endif
