#ifndef UE4SS_SDK_UDS_Volume_Actor_HPP
#define UE4SS_SDK_UDS_Volume_Actor_HPP

class AUDS_Volume_Actor_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B0 (size: 0x8)
    TEnumAsByte<UDS_OcclusionShape::Type> Collision Shape;                            // 0x02B8 (size: 0x1)
    FVector Box Extent;                                                               // 0x02C0 (size: 0x18)
    double Sphere Radius;                                                             // 0x02D8 (size: 0x8)
    float Capsule Half Height;                                                        // 0x02E0 (size: 0x4)
    double Blend Radius;                                                              // 0x02E8 (size: 0x8)
    class UPrimitiveComponent* Collision Component;                                   // 0x02F0 (size: 0x8)
    TEnumAsByte<ECollisionChannel> Collision Object Type;                             // 0x02F8 (size: 0x1)

    void Disable Volume();
    void Enable Volume();
    void Query Blend Radius(FVector Location, double& Multiplier);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_UDS_Volume_Actor(int32 EntryPoint);
}; // Size: 0x2F9

#endif
