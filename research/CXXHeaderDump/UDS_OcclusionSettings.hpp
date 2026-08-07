#ifndef UE4SS_SDK_UDS_OcclusionSettings_HPP
#define UE4SS_SDK_UDS_OcclusionSettings_HPP

class UUDS_OcclusionSettings_C : public UPrimaryDataAsset
{
    TEnumAsByte<UDS_Occlusion_Mode::Type> Occlusion Mode;                             // 0x0030 (size: 0x1)
    TEnumAsByte<UDS_OcclusionSamplingLocation::Type> Occlusion Sampling Location;     // 0x0031 (size: 0x1)
    bool Use GPU Distance Field Collision;                                            // 0x0032 (size: 0x1)
    double Max Trace Distance;                                                        // 0x0038 (size: 0x8)
    int32 Horizontal Trace Steps;                                                     // 0x0040 (size: 0x4)
    int32 Vertical Trace Steps;                                                       // 0x0044 (size: 0x4)
    TEnumAsByte<ETraceTypeQuery> Trace Channel;                                       // 0x0048 (size: 0x1)
    double Update Period;                                                             // 0x0050 (size: 0x8)
    double Global Occlusion Min Fraction;                                             // 0x0058 (size: 0x8)
    double Global Occlusion Max Fraction;                                             // 0x0060 (size: 0x8)
    bool Query Occlusion Portals;                                                     // 0x0068 (size: 0x1)
    int32 GPU Distance Field Sample Count;                                            // 0x006C (size: 0x4)
    bool Show Trace Debugs;                                                           // 0x0070 (size: 0x1)
    TEnumAsByte<UDS_Occlusion_Water_Behavior::Type> Occlusion Behavior Under Water;   // 0x0071 (size: 0x1)

}; // Size: 0x72

#endif
