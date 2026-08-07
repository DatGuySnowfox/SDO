#ifndef UE4SS_SDK_UDS_OcclusionState_HPP
#define UE4SS_SDK_UDS_OcclusionState_HPP

class UUDS_OcclusionState_C : public UPrimaryDataAsset
{
    double Increment Time;                                                            // 0x0030 (size: 0x8)
    int32 Traces Per Update;                                                          // 0x0038 (size: 0x4)
    double Global Occlusion Total;                                                    // 0x0040 (size: 0x8)
    double Upward Occlusion Total;                                                    // 0x0048 (size: 0x8)
    FLinearColor Directional Occlusion Total;                                         // 0x0050 (size: 0x10)
    double Directional Occlusion Max;                                                 // 0x0060 (size: 0x8)
    TArray<FLinearColor> Directional Trace Max Values;                                // 0x0068 (size: 0x10)
    double Upward Occlusion Max;                                                      // 0x0078 (size: 0x8)
    TArray<double> Upward Trace Max Values;                                           // 0x0080 (size: 0x10)
    int32 Trace Index;                                                                // 0x0090 (size: 0x4)
    TArray<FVector> Trace Offsets;                                                    // 0x0098 (size: 0x10)
    TArray<FLinearColor> Directional Occlusion History;                               // 0x00A8 (size: 0x10)
    TArray<double> Global Occlusion History;                                          // 0x00B8 (size: 0x10)
    TArray<double> Upward Occlusion History;                                          // 0x00C8 (size: 0x10)
    double Occlusion Multiplier;                                                      // 0x00D8 (size: 0x8)
    double Time of Last Trace;                                                        // 0x00E0 (size: 0x8)
    FVector Last Trace Location;                                                      // 0x00E8 (size: 0x18)
    FLinearColor Directional Portal Contribution;                                     // 0x0100 (size: 0x10)
    double Global Contribution;                                                       // 0x0110 (size: 0x8)
    FVector Last Portal Check Location;                                               // 0x0118 (size: 0x18)
    FLinearColor Current Directional Occlusion;                                       // 0x0130 (size: 0x10)
    double Current Global Occlusion;                                                  // 0x0140 (size: 0x8)
    double Current Upward Occlusion;                                                  // 0x0148 (size: 0x8)
    FLinearColor Target Directional Occlusion;                                        // 0x0150 (size: 0x10)
    double Target Global Occlusion;                                                   // 0x0160 (size: 0x8)
    class UUDS_OcclusionSettings_C* Occlusion Settings;                               // 0x0168 (size: 0x8)
    bool Unchanging Values Saved;                                                     // 0x0170 (size: 0x1)
    double Time of Last Periodic Context Check;                                       // 0x0178 (size: 0x8)
    double Time of Last Portal Component Check;                                       // 0x0180 (size: 0x8)
    double Time of Last Editor Cache;                                                 // 0x0188 (size: 0x8)

    void Save Unchanging Values();
    FVector Current Trace Direction(FVector Facing Direction, int32 Vertical Step);
    void Directional Vector Mask(FVector Direction, FLinearColor& Mask);
    double Directional Mask Value(FVector Direction, FVector Compare);
    FVector Current Facing Direction(int32 Step);
}; // Size: 0x190

#endif
