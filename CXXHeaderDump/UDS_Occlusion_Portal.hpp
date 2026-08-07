#ifndef UE4SS_SDK_UDS_Occlusion_Portal_HPP
#define UE4SS_SDK_UDS_Occlusion_Portal_HPP

class UUDS_Occlusion_Portal_C : public UStaticMeshComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0630 (size: 0x8)
    double Sample Range;                                                              // 0x0638 (size: 0x8)
    double Distance Falloff Exponent;                                                 // 0x0640 (size: 0x8)
    double Directional Occlusion;                                                     // 0x0648 (size: 0x8)
    double Global Occlusion;                                                          // 0x0650 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_UDS_Occlusion_Portal(int32 EntryPoint);
}; // Size: 0x658

#endif
