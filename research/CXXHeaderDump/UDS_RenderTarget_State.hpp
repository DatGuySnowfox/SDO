#ifndef UE4SS_SDK_UDS_RenderTarget_State_HPP
#define UE4SS_SDK_UDS_RenderTarget_State_HPP

class UUDS_RenderTarget_State_C : public UPrimaryDataAsset
{
    bool Is Active;                                                                   // 0x0030 (size: 0x1)
    FVector Center Location;                                                          // 0x0038 (size: 0x18)
    double Size;                                                                      // 0x0050 (size: 0x8)
    class UTextureRenderTarget2D* Render Target;                                      // 0x0058 (size: 0x8)
    class UTextureRenderTarget2D* Buffer Target;                                      // 0x0060 (size: 0x8)
    int32 Resolution;                                                                 // 0x0068 (size: 0x4)
    double Half Size;                                                                 // 0x0070 (size: 0x8)
    double Size to Res Ratio;                                                         // 0x0078 (size: 0x8)
    FVector Top Corner;                                                               // 0x0080 (size: 0x18)
    FVector2D Center 2D;                                                              // 0x0098 (size: 0x10)
    FVector2D Top Corner 2D;                                                          // 0x00A8 (size: 0x10)

    void Target Needs Recenter(FVector Control Location, FVector Axis Mask, bool& Yes);
    void Canvas Brush Size(FVector2D In, FVector2D& Out);
    void Canvas Brush Location(FVector2D In, FVector2D& Out);
    void Set Location(FVector Center Location, FLinearColor& Mapping Vector4);
    void Set Size(double Size);
    void Set Render Target(class UTextureRenderTarget2D* Render Target, class UTextureRenderTarget2D* Buffer Target);
    void Target Extent 2D(FVector2D& Extent);
}; // Size: 0xB8

#endif
