#ifndef UE4SS_SDK_BP_MapData_HPP
#define UE4SS_SDK_BP_MapData_HPP

class ABP_MapData_C : public AActor
{
    class USceneComponent* DefaultSceneRoot;                                          // 0x0298 (size: 0x8)
    class UTexture2D* Map Texture;                                                    // 0x02A0 (size: 0x8)
    class UTexture2D* Minimap Mask Texture;                                           // 0x02A8 (size: 0x8)
    double World Size;                                                                // 0x02B0 (size: 0x8)
    FVector2D World Middle Coordinations;                                             // 0x02B8 (size: 0x10)
    class UMaterialInstanceDynamic* Dynamic Minimap Material;                         // 0x02C8 (size: 0x8)
    class UMaterialInstanceDynamic* Dynamic Bigmap Material;                          // 0x02D0 (size: 0x8)
    double Minimap Zoom;                                                              // 0x02D8 (size: 0x8)
    bool Enable Fog Of War;                                                           // 0x02E0 (size: 0x1)
    int32 Fog Of War Mask Resolution;                                                 // 0x02E4 (size: 0x4)
    class UTextureRenderTarget2D* Fog Of War Mask;                                    // 0x02E8 (size: 0x8)
    class UTexture2D* Fog Of War Texture;                                             // 0x02F0 (size: 0x8)
    class UMaterialInstanceDynamic* Minimap Fog Of War Material;                      // 0x02F8 (size: 0x8)
    double Vision Radius;                                                             // 0x0300 (size: 0x8)
    double Fog Of War Update Time;                                                    // 0x0308 (size: 0x8)
    class UMaterialInstanceDynamic* Bigmap Fog Of War Material;                       // 0x0310 (size: 0x8)
    FLinearColor Zone Color;                                                          // 0x0318 (size: 0x10)

    void Set Settings For Gameplay();
    void Set Settings For Picture();
    void Update Player Vision();
    void Construct Minimap();
}; // Size: 0x328

#endif
