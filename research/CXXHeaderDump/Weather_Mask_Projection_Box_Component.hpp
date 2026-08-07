#ifndef UE4SS_SDK_Weather_Mask_Projection_Box_Component_HPP
#define UE4SS_SDK_Weather_Mask_Projection_Box_Component_HPP

class UWeather_Mask_Projection_Box_Component_C : public UWeatherMask_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0698 (size: 0x8)
    class USceneCaptureComponent2D* SceneCaptureComponent2D;                          // 0x06A0 (size: 0x8)
    double Blur Radius;                                                               // 0x06A8 (size: 0x8)
    double Blur Slope (Wetness);                                                      // 0x06B0 (size: 0x8)
    double Blur Slope (Snow/Dust);                                                    // 0x06B8 (size: 0x8)
    int32 Capture Pixel Size;                                                         // 0x06C0 (size: 0x4)
    TArray<class AActor*> Exclude Actors from Occlusion;                              // 0x06C8 (size: 0x10)
    class UMaterialInstanceDynamic* Mask_MID;                                         // 0x06D8 (size: 0x8)
    class UMaterialInstanceDynamic* Height_MID;                                       // 0x06E0 (size: 0x8)
    double Top Height;                                                                // 0x06E8 (size: 0x8)
    FVector Capture Box Size;                                                         // 0x06F0 (size: 0x18)
    class UTextureRenderTarget2D* Depth Render Target;                                // 0x0708 (size: 0x8)
    int32 Capture X Resolution;                                                       // 0x0710 (size: 0x4)
    double Height Dilation;                                                           // 0x0718 (size: 0x8)
    bool Recycle Scene Capture for Repeated Mask Draws;                               // 0x0720 (size: 0x1)
    bool UDW Runtime;                                                                 // 0x0721 (size: 0x1)

    void Calculate Masking At Location(FVector Location, FVector2D& Mask, bool& Cancel All Masks);
    void Unready();
    void Get Brush Scale(FVector2D& Out);
    void Recycle Render Target();
    void Update Capture();
    void Prepare for Drawing(bool Runtime, class AUltra_Dynamic_Weather_C* UDW);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Weather_Mask_Projection_Box_Component(int32 EntryPoint);
}; // Size: 0x722

#endif
