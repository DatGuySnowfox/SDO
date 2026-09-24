#ifndef UE4SS_SDK_UDS_Occlusion_Portal_HPP
#define UE4SS_SDK_UDS_Occlusion_Portal_HPP

class UUDS_Occlusion_Portal_C : public UStaticMeshComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0610 (size: 0x8)
    double Sample Range;                                                              // 0x0618 (size: 0x8)
    double Distance Falloff Exponent;                                                 // 0x0620 (size: 0x8)
    double Directional Occlusion;                                                     // 0x0628 (size: 0x8)
    double Global Occlusion;                                                          // 0x0630 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_UDS_Occlusion_Portal(int32 EntryPoint);
}; // Size: 0x638

#endif
