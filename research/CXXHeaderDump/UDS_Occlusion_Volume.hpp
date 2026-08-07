#ifndef UE4SS_SDK_UDS_Occlusion_Volume_HPP
#define UE4SS_SDK_UDS_Occlusion_Volume_HPP

class AUDS_Occlusion_Volume_C : public AUDS_Volume_Actor_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02F0 (size: 0x8)
    bool Affects Sound Occlusion / Interior Adjusments;                               // 0x02F8 (size: 0x1)
    double Occlusion Multiplier;                                                      // 0x0300 (size: 0x8)
    bool Block Weather Particles;                                                     // 0x0308 (size: 0x1)

    void Query Occlusion Multiplier(FVector Location, double& Multiplier);
    void Configure Weather Particle Blocking();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_UDS_Occlusion_Volume(int32 EntryPoint);
}; // Size: 0x309

#endif
