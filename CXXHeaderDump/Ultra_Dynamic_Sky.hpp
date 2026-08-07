#ifndef UE4SS_SDK_Ultra_Dynamic_Sky_HPP
#define UE4SS_SDK_Ultra_Dynamic_Sky_HPP

class AUltra_Dynamic_Sky_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UPostProcessComponent* PostProcess;                                         // 0x02A0 (size: 0x8)
    class UBillboardComponent* Sun Icon;                                              // 0x02A8 (size: 0x8)
    class UBillboardComponent* Moon Icon;                                             // 0x02B0 (size: 0x8)
    class USkyLightComponent* Captured Scene Sky Light;                               // 0x02B8 (size: 0x8)
    class UBillboardComponent* Root;                                                  // 0x02C0 (size: 0x8)
    class UVolumetricCloudComponent* VolumetricCloud;                                 // 0x02C8 (size: 0x8)
    class USkyLightComponent* Cubemap Sky Light;                                      // 0x02D0 (size: 0x8)
    class UExponentialHeightFogComponent* HeightFog;                                  // 0x02D8 (size: 0x8)
    class UDirectionalLightComponent* Moon;                                           // 0x02E0 (size: 0x8)
    class UDirectionalLightComponent* Sun;                                            // 0x02E8 (size: 0x8)
    class USkyAtmosphereComponent* SkyAtmosphere;                                     // 0x02F0 (size: 0x8)
    class UStaticMeshComponent* Sky_Sphere_Mesh;                                      // 0x02F8 (size: 0x8)
    class UMaterialInstanceDynamic* Sky Sphere MID;                                   // 0x0300 (size: 0x8)
    double Cloud Speed;                                                               // 0x0308 (size: 0x8)
    double Cloud Wisps Opacity (Clear);                                               // 0x0310 (size: 0x8)
    bool Refresh Settings;                                                            // 0x0318 (size: 0x1)
    double Time of Day;                                                               // 0x0320 (size: 0x8)
    TEnumAsByte<UDS_SkyMode::Type> Sky Mode;                                          // 0x0328 (size: 0x1)
    TEnumAsByte<UDS_ColorMode::Type> Color Mode;                                      // 0x0329 (size: 0x1)
    TEnumAsByte<UDS_Project_Mode::Type> Project Mode;                                 // 0x032A (size: 0x1)
    TEnumAsByte<UDS_RenderingFeatureLevel::Type> Feature Level;                       // 0x032B (size: 0x1)
    double Cloud Coverage;                                                            // 0x0330 (size: 0x8)
    double Fog;                                                                       // 0x0338 (size: 0x8)
    double Overall Intensity;                                                         // 0x0340 (size: 0x8)
    double Cloud Direction;                                                           // 0x0348 (size: 0x8)
    double Cloud Phase;                                                               // 0x0350 (size: 0x8)
    bool Manually Position Moon Target;                                               // 0x0358 (size: 0x1)
    bool Render Moon;                                                                 // 0x0359 (size: 0x1)
    double Moon Light Intensity;                                                      // 0x0360 (size: 0x8)
    TEnumAsByte<EComponentMobility::Type> Moon Mobility;                              // 0x0368 (size: 0x1)
    double Moon Scale;                                                                // 0x0370 (size: 0x8)
    double Moon Phase;                                                                // 0x0378 (size: 0x8)
    double Moon Yaw;                                                                  // 0x0380 (size: 0x8)
    double Moon Pitch;                                                                // 0x0388 (size: 0x8)
    double Moon Vertical Offset;                                                      // 0x0390 (size: 0x8)
    double Moon Orbit Offset;                                                         // 0x0398 (size: 0x8)
    bool Moon Casts Shadows;                                                          // 0x03A0 (size: 0x1)
    bool Moon Transmission;                                                           // 0x03A1 (size: 0x1)
    FLinearColor Moon Light Color;                                                    // 0x03A4 (size: 0x10)
    TSoftObjectPtr<UTexture2D> Moon Texture;                                          // 0x03B8 (size: 0x28)
    FLinearColor Moon Material Color;                                                 // 0x03E0 (size: 0x10)
    double Moon Texture Intensity (Night);                                            // 0x03F0 (size: 0x8)
    double Moon Texture Intensity (Day);                                              // 0x03F8 (size: 0x8)
    double Moon Glow Intensity;                                                       // 0x0400 (size: 0x8)
    bool Render Moon Phases;                                                          // 0x0408 (size: 0x1)
    double Moon Phase Contrast;                                                       // 0x0410 (size: 0x8)
    bool Moon Phase Changes Over Time;                                                // 0x0418 (size: 0x1)
    double Dark Side Brightness;                                                      // 0x0420 (size: 0x8)
    TSoftObjectPtr<UTexture2D> Moon Phase Normal;                                     // 0x0428 (size: 0x28)
    bool Manually Position Sun Target;                                                // 0x0450 (size: 0x1)
    double Sun Light Intensity;                                                       // 0x0458 (size: 0x8)
    TEnumAsByte<EComponentMobility::Type> Sun Mobility;                               // 0x0460 (size: 0x1)
    double Sun Yaw;                                                                   // 0x0468 (size: 0x8)
    double Contrast;                                                                  // 0x0470 (size: 0x8)
    double Sun Pitch;                                                                 // 0x0478 (size: 0x8)
    double Sun Vertical Offset;                                                       // 0x0480 (size: 0x8)
    bool Sun Casts Shadows;                                                           // 0x0488 (size: 0x1)
    bool Sun Transmission;                                                            // 0x0489 (size: 0x1)
    FLinearColor Sun Light Color;                                                     // 0x048C (size: 0x10)
    double Soften Cloud Layer 1;                                                      // 0x04A0 (size: 0x8)
    double Soften Cloud Layer 2;                                                      // 0x04A8 (size: 0x8)
    double Sharpen Outer Edge;                                                        // 0x04B0 (size: 0x8)
    double Stars Intensity;                                                           // 0x04B8 (size: 0x8)
    FLinearColor Stars Color;                                                         // 0x04C0 (size: 0x10)
    double Sun Scale;                                                                 // 0x04D0 (size: 0x8)
    class UMaterialInstanceDynamic* Cloud Shadows MID;                                // 0x04D8 (size: 0x8)
    bool Use Cloud Shadows;                                                           // 0x04E0 (size: 0x1)
    double Cloud Shadows Intensity when Sunny;                                        // 0x04E8 (size: 0x8)
    double Stars Speed;                                                               // 0x04F0 (size: 0x8)
    double New Moon Light Brightness;                                                 // 0x04F8 (size: 0x8)
    double Moon Source Angle Scale;                                                   // 0x0500 (size: 0x8)
    double Moon Texture Rotation;                                                     // 0x0508 (size: 0x8)
    TEnumAsByte<UDS_SkyLightMode::Type> Sky Light Mode;                               // 0x0510 (size: 0x1)
    bool Use Auroras;                                                                 // 0x0511 (size: 0x1)
    double Aurora Intensity;                                                          // 0x0518 (size: 0x8)
    double Aurora Speed;                                                              // 0x0520 (size: 0x8)
    double Cloud Shadows Intensity when Overcast;                                     // 0x0528 (size: 0x8)
    double Cloud Shadows Softness When Sunny;                                         // 0x0530 (size: 0x8)
    double Cloud Shadows Softness when Overcast;                                      // 0x0538 (size: 0x8)
    double Cloud Tiling;                                                              // 0x0540 (size: 0x8)
    double Soften Horizon;                                                            // 0x0548 (size: 0x8)
    bool One Cloud Layer;                                                             // 0x0550 (size: 0x1)
    double Cloud Height;                                                              // 0x0558 (size: 0x8)
    double Overcast Swirling Texture;                                                 // 0x0560 (size: 0x8)
    double Saturation;                                                                // 0x0568 (size: 0x8)
    double Directional Balance;                                                       // 0x0570 (size: 0x8)
    double Dawn Time;                                                                 // 0x0578 (size: 0x8)
    double Dusk Time;                                                                 // 0x0580 (size: 0x8)
    double Night Brightness;                                                          // 0x0588 (size: 0x8)
    double Sun Softness;                                                              // 0x0590 (size: 0x8)
    double Aurora Phase;                                                              // 0x0598 (size: 0x8)
    class UDirectionalLightComponent* Sun_LightComponent;                             // 0x05A0 (size: 0x8)
    class UDirectionalLightComponent* Moon_LightComponent;                            // 0x05A8 (size: 0x8)
    class USkyLightComponent* SkyLightComponent;                                      // 0x05B0 (size: 0x8)
    class UExponentialHeightFogComponent* Height Fog Component;                       // 0x05B8 (size: 0x8)
    bool Apply Exposure Settings;                                                     // 0x05C0 (size: 0x1)
    TEnumAsByte<EAutoExposureMethod> Exposure Metering Mode;                          // 0x05C1 (size: 0x1)
    class UCurveFloat* Exposure Compensation Curve;                                   // 0x05C8 (size: 0x8)
    bool Animate Time of Day;                                                         // 0x05D0 (size: 0x1)
    double Day Length;                                                                // 0x05D8 (size: 0x8)
    double Night Length;                                                              // 0x05E0 (size: 0x8)
    double Extend Dawn And Dusk;                                                      // 0x05E8 (size: 0x8)
    double Base Fog Density;                                                          // 0x05F0 (size: 0x8)
    double Foggy Density Contribution;                                                // 0x05F8 (size: 0x8)
    double Cloudy Density Contribution;                                               // 0x0600 (size: 0x8)
    double Dust Density Contribution;                                                 // 0x0608 (size: 0x8)
    double Stars Tiling;                                                              // 0x0610 (size: 0x8)
    double Stars Daytime Intensity;                                                   // 0x0618 (size: 0x8)
    double Twinkle Amount;                                                            // 0x0620 (size: 0x8)
    double Twinkle Speed;                                                             // 0x0628 (size: 0x8)
    TSoftObjectPtr<UTexture2D> Tiling Stars Texture;                                  // 0x0630 (size: 0x28)
    TSoftObjectPtr<UTexture2D> Real Stars Texture;                                    // 0x0658 (size: 0x28)
    double Stars Phase;                                                               // 0x0680 (size: 0x8)
    double Sky Light Intensity;                                                       // 0x0688 (size: 0x8)
    TEnumAsByte<EComponentMobility::Type> Sky Light Mobility;                         // 0x0690 (size: 0x1)
    double Sky Light Temperature;                                                     // 0x0698 (size: 0x8)
    FLinearColor Sky Light Color Multiplier (Day);                                    // 0x06A0 (size: 0x10)
    FLinearColor Sky Light Color Multiplier (Dawn/Dusk);                              // 0x06B0 (size: 0x10)
    FLinearColor Sky Light Color Multiplier (Night);                                  // 0x06C0 (size: 0x10)
    bool Real Time Capture;                                                           // 0x06D0 (size: 0x1)
    bool Move Sky Light Capture with Camera Location;                                 // 0x06D1 (size: 0x1)
    FLinearColor Sky Light Lower Hemisphere Tint (Capture Based);                     // 0x06D4 (size: 0x10)
    FLinearColor Sky Light Lower Hemisphere Tint (Cubemap);                           // 0x06E4 (size: 0x10)
    TSoftObjectPtr<UTextureCube> Sky Light Cubemap;                                   // 0x06F8 (size: 0x28)
    double Sky Light Cubemap Angle;                                                   // 0x0720 (size: 0x8)
    bool Render Sky Light;                                                            // 0x0728 (size: 0x1)
    class ASkyLight* Custom Sky Light Actor;                                          // 0x0730 (size: 0x8)
    bool Real Time Capture Uses Time Slicing;                                         // 0x0738 (size: 0x1)
    bool Static/Stationary Sky Light Casts Shadows;                                   // 0x0739 (size: 0x1)
    bool Movable Sky Light Casts Shadows (Enable DFAO);                               // 0x073A (size: 0x1)
    bool Recapture Sky Light Periodically;                                            // 0x073B (size: 0x1)
    double Sky Light Recapture Period (Seconds);                                      // 0x0740 (size: 0x8)
    bool Random Starting Time;                                                        // 0x0748 (size: 0x1)
    double Sunrise Event Offset;                                                      // 0x0750 (size: 0x8)
    double Sunset Event Offset;                                                       // 0x0758 (size: 0x8)
    FUltra_Dynamic_Sky_CSunset Sunset;                                                // 0x0760 (size: 0x10)
    void Sunset();
    FUltra_Dynamic_Sky_CSunrise Sunrise;                                              // 0x0770 (size: 0x10)
    void Sunrise();
    double Cloud Wisps Opacity (Cloudy);                                              // 0x0780 (size: 0x8)
    class UTexture2D* Cloud Wisps Texture;                                            // 0x0788 (size: 0x8)
    TSoftObjectPtr<UTexture2D> 2D Dynamic Cloud Formation Texture;                    // 0x0790 (size: 0x28)
    double Sun Shader Intensity;                                                      // 0x07B8 (size: 0x8)
    double Directional Lighting Intensity;                                            // 0x07C0 (size: 0x8)
    double Sun Highlight Radius Scale;                                                // 0x07C8 (size: 0x8)
    double Cloud Opacity;                                                             // 0x07D0 (size: 0x8)
    double Horizon Density Multiplier;                                                // 0x07D8 (size: 0x8)
    double Zenith Density Multiplier;                                                 // 0x07E0 (size: 0x8)
    double Latitude Gradient Position;                                                // 0x07E8 (size: 0x8)
    double Latitude Gradient Width;                                                   // 0x07F0 (size: 0x8)
    double Around Sun Density Multiplier;                                             // 0x07F8 (size: 0x8)
    double Around Sun Density Exponent;                                               // 0x0800 (size: 0x8)
    double Around Moon Density Multiplier;                                            // 0x0808 (size: 0x8)
    double Around Moon Density Exponent;                                              // 0x0810 (size: 0x8)
    double 2D Clouds Shading Offset;                                                  // 0x0818 (size: 0x8)
    class UCurveFloat* Shine Intensity Curve;                                         // 0x0820 (size: 0x8)
    class UCurveFloat* Sun Highlight Intensity Curve;                                 // 0x0828 (size: 0x8)
    class UCurveFloat* Sun Highlight Radius Curve;                                    // 0x0830 (size: 0x8)
    bool Adjust for Path Tracer;                                                      // 0x0838 (size: 0x1)
    double Lights Update Period;                                                      // 0x0840 (size: 0x8)
    double Lights Update Degree Threshold;                                            // 0x0848 (size: 0x8)
    bool Dim Directional Lights with Cloud Coverage;                                  // 0x0850 (size: 0x1)
    double Extra Night Brightness when Cloudy;                                        // 0x0858 (size: 0x8)
    double Directional Lights Absent Brightness;                                      // 0x0860 (size: 0x8)
    class UMaterialInstanceDynamic* Vol Clouds MID Complex;                           // 0x0868 (size: 0x8)
    TEnumAsByte<UDS_VolRT_Mode::Type> Volumetric Cloud Rendering Mode;                // 0x0870 (size: 0x1)
    double Bottom Altitude;                                                           // 0x0878 (size: 0x8)
    double Volumetric Clouds Scale;                                                   // 0x0880 (size: 0x8)
    double Layer Height Scale;                                                        // 0x0888 (size: 0x8)
    double Cloud Formation Texture Scale;                                             // 0x0890 (size: 0x8)
    double 3D Noise Scale;                                                            // 0x0898 (size: 0x8)
    double 3D Erosion Intensity;                                                      // 0x08A0 (size: 0x8)
    double High Frequency Noise Amount;                                               // 0x08A8 (size: 0x8)
    int32 High Frequency Levels;                                                      // 0x08B0 (size: 0x4)
    double Distort High Frequency Noise;                                              // 0x08B8 (size: 0x8)
    double Extinction Scale;                                                          // 0x08C0 (size: 0x8)
    double Base Floor Variation;                                                      // 0x08C8 (size: 0x8)
    double Overcast Floor Variation;                                                  // 0x08D0 (size: 0x8)
    double Floor Luminance Scale;                                                     // 0x08D8 (size: 0x8)
    double View Sample Scale (Day);                                                   // 0x08E0 (size: 0x8)
    double View Sample Scale (Night);                                                 // 0x08E8 (size: 0x8)
    double Shadow Sample Scale;                                                       // 0x08F0 (size: 0x8)
    double Shadow Tracing Distance;                                                   // 0x08F8 (size: 0x8)
    double Tracing Max Distance (Inside Cloud Layer);                                 // 0x0900 (size: 0x8)
    double Tracing Max Distance (Outside Cloud Layer);                                // 0x0908 (size: 0x8)
    FLinearColor Volumetric Cloud Color;                                              // 0x0910 (size: 0x10)
    TSoftObjectPtr<UTexture> Cloud Profile LUT;                                       // 0x0920 (size: 0x28)
    TSoftObjectPtr<UTexture2D> Volumetric Cloud Formation Texture;                    // 0x0948 (size: 0x28)
    double Minimum Erosion;                                                           // 0x0970 (size: 0x8)
    double Distance to Sample MaxCount;                                               // 0x0978 (size: 0x8)
    double Cloud Formation Texture Mip Level;                                         // 0x0980 (size: 0x8)
    double Reflection Sample Count Scale;                                             // 0x0988 (size: 0x8)
    double Reflection Shadow Sample Count Scale;                                      // 0x0990 (size: 0x8)
    double Volumetric Cloud Ambient Light Intensity;                                  // 0x0998 (size: 0x8)
    FLinearColor Volumetric Cloud Ambient Light (Day);                                // 0x09A0 (size: 0x10)
    FLinearColor Volumetric Cloud Ambient Light (Dawn/Dusk);                          // 0x09B0 (size: 0x10)
    FLinearColor Volumetric Cloud Ambient Light (Night);                              // 0x09C0 (size: 0x10)
    double Volumetric Cloud Ambient Light Saturation;                                 // 0x09D0 (size: 0x8)
    double Multiscattering Light Intensity;                                           // 0x09D8 (size: 0x8)
    double Multiscattering Light Occlusion;                                           // 0x09E0 (size: 0x8)
    class UMaterialInstanceDynamic* Vol Clouds MID Simple;                            // 0x09E8 (size: 0x8)
    double Scale Fog Density;                                                         // 0x09F0 (size: 0x8)
    double Fog Start Distance when Clear;                                             // 0x09F8 (size: 0x8)
    double Fog Density Where Start Distance Reaches Zero;                             // 0x0A00 (size: 0x8)
    float Formation Change Speed;                                                     // 0x0A08 (size: 0x4)
    bool Randomize Cloud Formation on Run;                                            // 0x0A0C (size: 0x1)
    double 3D Noise Vertical Movement;                                                // 0x0A10 (size: 0x8)
    double Macro Variation;                                                           // 0x0A18 (size: 0x8)
    double Macro Scale;                                                               // 0x0A20 (size: 0x8)
    double Multiscattering Phase 1;                                                   // 0x0A28 (size: 0x8)
    double Multiscattering Phase 2;                                                   // 0x0A30 (size: 0x8)
    double Multiscattering Eccentricity;                                              // 0x0A38 (size: 0x8)
    double Time Event Dispatcher Check Period;                                        // 0x0A40 (size: 0x8)
    double Time Speed;                                                                // 0x0A48 (size: 0x8)
    double Time Cycle Degrees;                                                        // 0x0A50 (size: 0x8)
    class AActor* Ultra Dynamic Weather;                                              // 0x0A58 (size: 0x8)
    double Tracing Max Start Distance;                                                // 0x0A60 (size: 0x8)
    double Aurora Shape Change Speed;                                                 // 0x0A68 (size: 0x8)
    double Aurora Texture Scale;                                                      // 0x0A70 (size: 0x8)
    double Daytime Aurora Intensity;                                                  // 0x0A78 (size: 0x8)
    FLinearColor Aurora Color 1;                                                      // 0x0A80 (size: 0x10)
    FLinearColor Aurora Color 2;                                                      // 0x0A90 (size: 0x10)
    FLinearColor Aurora Color 3;                                                      // 0x0AA0 (size: 0x10)
    class UMaterialInstanceDynamic* Volumetric Aurora MID;                            // 0x0AB0 (size: 0x8)
    bool Half Rate Tick;                                                              // 0x0AB8 (size: 0x1)
    int32 Half Rate Tick Framerate Threshold;                                         // 0x0ABC (size: 0x4)
    bool Disable All Runtime Updating;                                                // 0x0AC0 (size: 0x1)
    bool Use Sky Mode Scalability Map;                                                // 0x0AC1 (size: 0x1)
    TMap<int32, TEnumAsByte<UDS_SkyMode::Type>> Sky Mode Scalability Map;             // 0x0AC8 (size: 0x50)
    double Sun Disk Intensity;                                                        // 0x0B18 (size: 0x8)
    class UMaterialInstanceDynamic* Inside Clouds Fog MID;                            // 0x0B20 (size: 0x8)
    bool Enable Fog Inside Clouds;                                                    // 0x0B28 (size: 0x1)
    TSoftObjectPtr<UTexture2D> Aurora Texture;                                        // 0x0B30 (size: 0x28)
    double 2D Aurora Max Samples;                                                     // 0x0B58 (size: 0x8)
    double 2D Aurora Sample Step Size;                                                // 0x0B60 (size: 0x8)
    double 2D Aurora Line Exponent;                                                   // 0x0B68 (size: 0x8)
    double 2D Aurora Mask Exponent;                                                   // 0x0B70 (size: 0x8)
    double Volumetric Aurora Sample Count Scale;                                      // 0x0B78 (size: 0x8)
    double Volumetric Aurora Layer Height;                                            // 0x0B80 (size: 0x8)
    double Volumetric Aurora Bottom Altitude;                                         // 0x0B88 (size: 0x8)
    double Fog Density Daytime Mutliplier;                                            // 0x0B90 (size: 0x8)
    double Fog Density Nighttime Multiplier;                                          // 0x0B98 (size: 0x8)
    double Base Height Fog Falloff;                                                   // 0x0BA0 (size: 0x8)
    double Cloudy Height Fog Falloff;                                                 // 0x0BA8 (size: 0x8)
    double Foggy Height Fog Falloff;                                                  // 0x0BB0 (size: 0x8)
    double Inside Cloud Fog Strength;                                                 // 0x0BB8 (size: 0x8)
    FLinearColor Inside Cloud Fog Color;                                              // 0x0BC0 (size: 0x10)
    double 2D Overcast Turbulence;                                                    // 0x0BD0 (size: 0x8)
    double Close Fade Distance;                                                       // 0x0BD8 (size: 0x8)
    double Close Fade Offset;                                                         // 0x0BE0 (size: 0x8)
    bool Two Layers;                                                                  // 0x0BE8 (size: 0x1)
    double Layer 2 Coverage Scale;                                                    // 0x0BF0 (size: 0x8)
    double Layer 2 Extinction Scale;                                                  // 0x0BF8 (size: 0x8)
    FVector2D Layer 2 Cloud Scale;                                                    // 0x0C00 (size: 0x10)
    double Layer 2 Mip Offset;                                                        // 0x0C10 (size: 0x8)
    double Distance Between Layers;                                                   // 0x0C18 (size: 0x8)
    bool Enable Sun Light Shaft Bloom;                                                // 0x0C20 (size: 0x1)
    FVector2D Sun Light Shaft Bloom Scale;                                            // 0x0C28 (size: 0x10)
    FVector2D Sun Light Shaft Max Brightness;                                         // 0x0C38 (size: 0x10)
    FVector2D Sun Light Shaft Bloom Threshold;                                        // 0x0C48 (size: 0x10)
    FLinearColor Sun Light Shaft Tint Color;                                          // 0x0C58 (size: 0x10)
    bool Enable Moon Light Shaft Bloom;                                               // 0x0C68 (size: 0x1)
    FVector2D Moon Light Shaft Bloom Scale;                                           // 0x0C70 (size: 0x10)
    FVector2D Moon Light Shaft Max Brightness;                                        // 0x0C80 (size: 0x10)
    FVector2D Moon Light Shaft Bloom Threshold;                                       // 0x0C90 (size: 0x10)
    FLinearColor Moon Light Shaft Tint Color;                                         // 0x0CA0 (size: 0x10)
    TEnumAsByte<UDS_SkyMode::Type> Volumetric Clouds Replacement;                     // 0x0CB0 (size: 0x1)
    TEnumAsByte<UDS_SkyLightMode::Type> Real Time Capture Sky Light Replacement;      // 0x0CB1 (size: 0x1)
    TEnumAsByte<UDS_SkyMode::Type> Volumetric Aurora Replacement;                     // 0x0CB2 (size: 0x1)
    TMap<FString, TEnumAsByte<UDS_RenderingFeatureLevel::Type>> Platform Feature Levels; // 0x0CB8 (size: 0x50)
    bool Simulate Real Sun;                                                           // 0x0D08 (size: 0x1)
    bool Simulate Real Moon;                                                          // 0x0D09 (size: 0x1)
    bool Simulate Real Stars;                                                         // 0x0D0A (size: 0x1)
    TEnumAsByte<UDS_CityPresets::Type> Location Preset;                               // 0x0D0B (size: 0x1)
    double Latitude;                                                                  // 0x0D10 (size: 0x8)
    double Longitude;                                                                 // 0x0D18 (size: 0x8)
    double Time Zone;                                                                 // 0x0D20 (size: 0x8)
    int32 Year;                                                                       // 0x0D28 (size: 0x4)
    int32 Month;                                                                      // 0x0D2C (size: 0x4)
    int32 Day;                                                                        // 0x0D30 (size: 0x4)
    double North Yaw;                                                                 // 0x0D38 (size: 0x8)
    double Simulation Speed;                                                          // 0x0D40 (size: 0x8)
    bool Use System Time;                                                             // 0x0D48 (size: 0x1)
    bool Apply Daylight Savings Time;                                                 // 0x0D49 (size: 0x1)
    class UUDS_Calendar_C* Calendar;                                                  // 0x0D50 (size: 0x8)
    int32 DST Start Month;                                                            // 0x0D58 (size: 0x4)
    int32 DST Start Day;                                                              // 0x0D5C (size: 0x4)
    int32 DST End Month;                                                              // 0x0D60 (size: 0x4)
    int32 DST End Day;                                                                // 0x0D64 (size: 0x4)
    int32 DST Change Hour;                                                            // 0x0D68 (size: 0x4)
    double Stellar Calibration;                                                       // 0x0D70 (size: 0x8)
    double Lunar Orbit Calibration;                                                   // 0x0D78 (size: 0x8)
    double Lunar Inclination Calibration;                                             // 0x0D80 (size: 0x8)
    bool Daylight Savings Time;                                                       // 0x0D88 (size: 0x1)
    int32 Total Days Elapsed;                                                         // 0x0D8C (size: 0x4)
    bool Clouds Move with Time of Day;                                                // 0x0D90 (size: 0x1)
    double Time of Day Movement Multiplier;                                           // 0x0D98 (size: 0x8)
    TMap<class FString, class TSoftObjectPtr<UMaterialInstance>> Sky MID Parent Instances; // 0x0DA0 (size: 0x50)
    double Fog Shadows;                                                               // 0x0DF0 (size: 0x8)
    double Dust Shadows;                                                              // 0x0DF8 (size: 0x8)
    double 2D Cloud Shadows Speed;                                                    // 0x0E00 (size: 0x8)
    double 2D Cloud Shadows Scale;                                                    // 0x0E08 (size: 0x8)
    bool Disable Directional Light Shadows When Fully Shadowed By Clouds;             // 0x0E10 (size: 0x1)
    class UMaterialInterface* Custom Light Function Material;                         // 0x0E18 (size: 0x8)
    bool Force Light Functions On;                                                    // 0x0E20 (size: 0x1)
    double Time Of Day Replication Period;                                            // 0x0E28 (size: 0x8)
    double Replicated Time of Day;                                                    // 0x0E30 (size: 0x8)
    bool Render Moon Directional Light;                                               // 0x0E38 (size: 0x1)
    class ADirectionalLight* Custom Moon Light Actor;                                 // 0x0E40 (size: 0x8)
    double Scale Moon Radius As It Nears Horizon;                                     // 0x0E48 (size: 0x8)
    bool Fog Colors from Sky Atmosphere;                                              // 0x0E50 (size: 0x1)
    double Dusty Height Fog Falloff;                                                  // 0x0E58 (size: 0x8)
    double Moon Specular Scale;                                                       // 0x0E60 (size: 0x8)
    double Ambient Light From Sky Atmosphere;                                         // 0x0E68 (size: 0x8)
    double Volumetric Cloud Ambient Light Bottom Scale;                               // 0x0E70 (size: 0x8)
    double Volumetric Cloud Ambient Light Top Scale;                                  // 0x0E78 (size: 0x8)
    double High Frequency Disable Distance;                                           // 0x0E80 (size: 0x8)
    bool Change To Simplified Material When Overcast;                                 // 0x0E88 (size: 0x1)
    double Simplified Material Cloud Coverage Threshold;                              // 0x0E90 (size: 0x8)
    double Simplified Material Fade Length;                                           // 0x0E98 (size: 0x8)
    double Scale View Samples when Overcast;                                          // 0x0EA0 (size: 0x8)
    int32 Current Volumetric Clouds MID;                                              // 0x0EA8 (size: 0x4)
    FVector Clouds Position;                                                          // 0x0EB0 (size: 0x18)
    FVector Cloud Phase Vector Multiplier;                                            // 0x0EC8 (size: 0x18)
    FLinearColor Rayleigh Scattering Color (Day);                                     // 0x0EE0 (size: 0x10)
    FLinearColor Rayleigh Scattering Color (Dawn/Dusk);                               // 0x0EF0 (size: 0x10)
    FLinearColor Rayleigh Scattering Color (Night);                                   // 0x0F00 (size: 0x10)
    double Desaturate Rayleigh Scattering when Cloudy;                                // 0x0F10 (size: 0x8)
    double Current Time of Day Offset;                                                // 0x0F18 (size: 0x8)
    double Last Frame Time of Day;                                                    // 0x0F20 (size: 0x8)
    double Time of Day Change Speed;                                                  // 0x0F28 (size: 0x8)
    TMap<FName, TEnumAsByte<UDS_PropertyType::Type>> Properties;                      // 0x0F30 (size: 0x50)
    double Night Sky Glow;                                                            // 0x0F80 (size: 0x8)
    FLinearColor Night Sky Glow Color;                                                // 0x0F88 (size: 0x10)
    double Light Pollution Intensity;                                                 // 0x0F98 (size: 0x8)
    FLinearColor Light Pollution Color;                                               // 0x0FA0 (size: 0x10)
    class UMaterialInstanceDynamic* Cloud Fog Post Process MID;                       // 0x0FB0 (size: 0x8)
    FFloatRange Dimming Range;                                                        // 0x0FB8 (size: 0x10)
    double Dimming Range Exponent;                                                    // 0x0FC8 (size: 0x8)
    class UMaterialInterface* Custom Sky Sphere Material;                             // 0x0FD0 (size: 0x8)
    double Fog Snow Contribution;                                                     // 0x0FD8 (size: 0x8)
    FUltra_Dynamic_Sky_CMidnight Midnight;                                            // 0x0FE0 (size: 0x10)
    void Midnight();
    FLinearColor Sunset/Sunrise Color (Absorption);                                   // 0x0FF0 (size: 0x10)
    double Sunset/Sunrise Color Intensity (Absorption Scale);                         // 0x1000 (size: 0x8)
    double Simulated Sunrise Time;                                                    // 0x1008 (size: 0x8)
    double Simulated Sunset Time;                                                     // 0x1010 (size: 0x8)
    double Base Cloud Height;                                                         // 0x1018 (size: 0x8)
    bool Enable Sun Lens Flare;                                                       // 0x1020 (size: 0x1)
    TEnumAsByte<UDS_LensFlareType::Type> Lens Flare Type;                             // 0x1021 (size: 0x1)
    double Lens Flare Strength;                                                       // 0x1028 (size: 0x8)
    FLinearColor Lens Flare Tint;                                                     // 0x1030 (size: 0x10)
    class UMaterialInstanceDynamic* Lens Flare MID;                                   // 0x1040 (size: 0x8)
    double Lens Flare Brightness Threshold;                                           // 0x1048 (size: 0x8)
    class UMaterialInterface* Custom Lens Flare Parent Instance;                      // 0x1050 (size: 0x8)
    double Scale Flare Elements;                                                      // 0x1058 (size: 0x8)
    double Dynamic Sky Light Transition Time;                                         // 0x1060 (size: 0x8)
    double Target Sky Light Multiplier;                                               // 0x1068 (size: 0x8)
    double Dynamic Sky Light Multiplier;                                              // 0x1070 (size: 0x8)
    double Cloud Coverage 0-3;                                                        // 0x1078 (size: 0x8)
    double Global Overcast 0-1;                                                       // 0x1080 (size: 0x8)
    double Local Overcast 0-1;                                                        // 0x1088 (size: 0x8)
    bool Override with New Changes;                                                   // 0x1090 (size: 0x1)
    class UMaterialInstanceDynamic* Overcast Turbulence MID;                          // 0x1098 (size: 0x8)
    double Scale View Samples when Camera is In Cloud Layer;                          // 0x10A0 (size: 0x8)
    double Low Material Quality Sample Count Scale;                                   // 0x10A8 (size: 0x8)
    double Medium Material Quality Sample Count Scale;                                // 0x10B0 (size: 0x8)
    double Epic Material Quality Sample Count Scale;                                  // 0x10B8 (size: 0x8)
    TSoftObjectPtr<UVolumeTexture> Epic Quality Volume Texture;                       // 0x10C0 (size: 0x28)
    TSoftObjectPtr<UVolumeTexture> High Quality Volume Texture;                       // 0x10E8 (size: 0x28)
    TSoftObjectPtr<UVolumeTexture> Low Quality Volume Texture;                        // 0x1110 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Inside Cloud Fog Parent Material;              // 0x1138 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Post Process Cloud Fog Parent Material;        // 0x1160 (size: 0x28)
    TArray<class UMaterialInstanceDynamic*> Volumetric Cloud MIDs;                    // 0x1188 (size: 0x10)
    double Moon Light Current Intensity Value;                                        // 0x1198 (size: 0x8)
    FLinearColor Twilight Color (Absorption);                                         // 0x11A0 (size: 0x10)
    double Twilight Color Intensity (Absorption Scale);                               // 0x11B0 (size: 0x8)
    FLinearColor Base Sky Color (Day);                                                // 0x11B8 (size: 0x10)
    FLinearColor Base Sky Color (Dawn/Dusk);                                          // 0x11C8 (size: 0x10)
    FLinearColor Base Sky Color (Night);                                              // 0x11D8 (size: 0x10)
    FLinearColor Cloud Light Color (Day);                                             // 0x11E8 (size: 0x10)
    FLinearColor Cloud Light Color (Dawn/Dusk);                                       // 0x11F8 (size: 0x10)
    FLinearColor Cloud Light Color (Night);                                           // 0x1208 (size: 0x10)
    FLinearColor Cloud Dark Color (Day);                                              // 0x1218 (size: 0x10)
    FLinearColor Cloud Dark Color (Dawn/Dusk);                                        // 0x1228 (size: 0x10)
    FLinearColor Cloud Dark Color (Night);                                            // 0x1238 (size: 0x10)
    FLinearColor Sun Glow Color;                                                      // 0x1248 (size: 0x10)
    FLinearColor Moon Glow Tint;                                                      // 0x1258 (size: 0x10)
    class UCurveFloat* Directional Intensity Curve;                                   // 0x1268 (size: 0x8)
    class UCurveLinearColor* Directional Light Scattering Curve;                      // 0x1270 (size: 0x8)
    class UTexture2D* Atmospheric Scattering LUT Texture;                             // 0x1278 (size: 0x8)
    bool Render Exponential Height Fog;                                               // 0x1280 (size: 0x1)
    class AExponentialHeightFog* Custom Height Fog Actor;                             // 0x1288 (size: 0x8)
    TEnumAsByte<UDS_FogColorMode::Type> Fog Color Mode;                               // 0x1290 (size: 0x1)
    double Fog Color Intensity Scale;                                                 // 0x1298 (size: 0x8)
    double Overcast Brightness Day;                                                   // 0x12A0 (size: 0x8)
    double Overcast Brightness Night;                                                 // 0x12A8 (size: 0x8)
    double Sky Atmosphere Fog Influence Sun;                                          // 0x12B0 (size: 0x8)
    double Sky Atmosphere Fog Influence Moon;                                         // 0x12B8 (size: 0x8)
    double Sky Atmosphere Fog Influence None;                                         // 0x12C0 (size: 0x8)
    class UCurveLinearColor* Fog Scattering Color Curve;                              // 0x12C8 (size: 0x8)
    FLinearColor Fog Inscattering Sun;                                                // 0x12D0 (size: 0x10)
    FLinearColor Fog Inscattering Moon;                                               // 0x12E0 (size: 0x10)
    class UCurveLinearColor* Fog Directional Scattering Color Curve;                  // 0x12F0 (size: 0x8)
    FLinearColor Fog Directional Inscattering;                                        // 0x12F8 (size: 0x10)
    double Foggy Desaturation;                                                        // 0x1308 (size: 0x8)
    double Overcast Desaturation;                                                     // 0x1310 (size: 0x8)
    FLinearColor All Fog Colors Multiplier;                                           // 0x1318 (size: 0x10)
    double Dust Amount;                                                               // 0x1328 (size: 0x8)
    FLinearColor Dust Color;                                                          // 0x1330 (size: 0x10)
    bool Use Volumetric Fog;                                                          // 0x1340 (size: 0x1)
    double Volumetric Fog Distance;                                                   // 0x1348 (size: 0x8)
    double Volumetric Fog Extinction;                                                 // 0x1350 (size: 0x8)
    double Sun Volumetric Scattering Intensity;                                       // 0x1358 (size: 0x8)
    double Moon Volumetric Scattering Intensity;                                      // 0x1360 (size: 0x8)
    bool Disable Height Fog Above Volumetric Cloud Layer;                             // 0x1368 (size: 0x1)
    float Cached Height Fog Density;                                                  // 0x136C (size: 0x4)
    bool Using Cloud Fog Post Process;                                                // 0x1370 (size: 0x1)
    FVector Current Camera Location;                                                  // 0x1378 (size: 0x18)
    FLinearColor Ambient Fog Color;                                                   // 0x1390 (size: 0x10)
    FLinearColor Directional Base Fog Color;                                          // 0x13A0 (size: 0x10)
    FLinearColor Scaled Directional Base Fog Color;                                   // 0x13B0 (size: 0x10)
    TSoftObjectPtr<UMaterialInterface> Volumetric Aurora Parent Material;             // 0x13C0 (size: 0x28)
    bool Using Inside Cloud Fog;                                                      // 0x13E8 (size: 0x1)
    TSoftObjectPtr<UMaterialInterface> Overcast Turbulence Parent Material;           // 0x13F0 (size: 0x28)
    bool Forward Shading;                                                             // 0x1418 (size: 0x1)
    FUltra_Dynamic_Sky_CHourly Hourly;                                                // 0x1420 (size: 0x10)
    void Hourly(int32 Hour);
    int32 Event Hour;                                                                 // 0x1430 (size: 0x4)
    double Cloud Wisps Color Intensity;                                               // 0x1438 (size: 0x8)
    double Max Moon Source Angle Scale;                                               // 0x1440 (size: 0x8)
    double Transition Time A;                                                         // 0x1448 (size: 0x8)
    double Transition Time B;                                                         // 0x1450 (size: 0x8)
    bool Transitioning Time;                                                          // 0x1458 (size: 0x1)
    TEnumAsByte<EEasingFunc::Type> Transition Easing Function;                        // 0x1459 (size: 0x1)
    double Transition Easing Exponent;                                                // 0x1460 (size: 0x8)
    FTimerHandle Transition Timer;                                                    // 0x1468 (size: 0x8)
    double Transition Duration;                                                       // 0x1470 (size: 0x8)
    int32 Transition Days Counted;                                                    // 0x1478 (size: 0x4)
    bool Transition Can Go Backwards;                                                 // 0x147C (size: 0x1)
    double Moon Source Angle Softness;                                                // 0x1480 (size: 0x8)
    double Exposure Bias Day;                                                         // 0x1488 (size: 0x8)
    double Exposure Bias Night;                                                       // 0x1490 (size: 0x8)
    double Exposure Bias Cloudy;                                                      // 0x1498 (size: 0x8)
    double Exposure Bias Foggy;                                                       // 0x14A0 (size: 0x8)
    double Exposure Bias Dusty;                                                       // 0x14A8 (size: 0x8)
    FFloatRange Exposure Brightness Range;                                            // 0x14B0 (size: 0x10)
    bool Apply Interior Adjustments;                                                  // 0x14C0 (size: 0x1)
    double Extra Fog Start Distance In Interior;                                      // 0x14C8 (size: 0x8)
    double Fog Density Multiplier in Interior;                                        // 0x14D0 (size: 0x8)
    double Exposure Bias in Interior;                                                 // 0x14D8 (size: 0x8)
    double Sky Light Intensity Multiplier in Interiors;                               // 0x14E0 (size: 0x8)
    double Sun Light Intensity Multiplier In Interiors;                               // 0x14E8 (size: 0x8)
    double Moon Light Intensity Multiplier in Interiors;                              // 0x14F0 (size: 0x8)
    double Extra Sun Volumetric Scattering In Interiors;                              // 0x14F8 (size: 0x8)
    double Extra Moon Volumetric Scattering In Interiors;                             // 0x1500 (size: 0x8)
    double Cached Inverted Global Occlusion;                                          // 0x1508 (size: 0x8)
    class UUDS_OcclusionSettings_C* Occlusion Settings;                               // 0x1510 (size: 0x8)
    double Cloud Shadow Disable Threshold;                                            // 0x1518 (size: 0x8)
    double Cloud Bottom World Height;                                                 // 0x1520 (size: 0x8)
    double Cached Night Filter;                                                       // 0x1528 (size: 0x8)
    TArray<double> Cached Floats Old;                                                 // 0x1530 (size: 0x10)
    TArray<double> Cached Floats New;                                                 // 0x1540 (size: 0x10)
    double Cache Alpha;                                                               // 0x1550 (size: 0x8)
    bool Filling Starting Cache;                                                      // 0x1558 (size: 0x1)
    TArray<FLinearColor> Cached Colors Old;                                           // 0x1560 (size: 0x10)
    TArray<FLinearColor> Cached Colors New;                                           // 0x1570 (size: 0x10)
    TArray<FVector> Cached Vectors Old;                                               // 0x1580 (size: 0x10)
    TArray<FVector> Cached Vectors New;                                               // 0x1590 (size: 0x10)
    FVector Cached Sun Vector;                                                        // 0x15A0 (size: 0x18)
    FVector Cached Moon Vector;                                                       // 0x15B8 (size: 0x18)
    TArray<bool> Cached Value Changing;                                               // 0x15D0 (size: 0x10)
    double Max Property Cache Period;                                                 // 0x15E0 (size: 0x8)
    TMap<TEnumAsByte<UDS_CityPresets::Type>, FVector> Preset Location Coordinates;    // 0x15E8 (size: 0x50)
    bool Disable Image Based Lens Flares when Enabled;                                // 0x1638 (size: 0x1)
    double Starting Sky Atmosphere Height;                                            // 0x1640 (size: 0x8)
    TArray<FUDS_Post_Process_Stage> Post Process Components;                          // 0x1648 (size: 0x10)
    FPostProcessSettings Static Post Process Settings;                                // 0x1660 (size: 0x6E0)
    TArray<class UPostProcessComponent*> User Post Process Components;                // 0x1D40 (size: 0x10)
    bool Using Post Process Components;                                               // 0x1D50 (size: 0x1)
    FLinearColor Cloud Wisps Tint (Day);                                              // 0x1D54 (size: 0x10)
    FLinearColor Cloud Wisps Tint (Dawn/Dusk);                                        // 0x1D64 (size: 0x10)
    FLinearColor Cloud Wisps Tint (Night);                                            // 0x1D74 (size: 0x10)
    double Increase Wisps Brightness Around Sun;                                      // 0x1D88 (size: 0x8)
    double Increase Wisps Brightness Around Moon;                                     // 0x1D90 (size: 0x8)
    double Sun Source Angle Scale;                                                    // 0x1D98 (size: 0x8)
    bool Solar Eclipse;                                                               // 0x1DA0 (size: 0x1)
    FLinearColor Solar Eclipse Tint Color;                                            // 0x1DA4 (size: 0x10)
    double Solar Eclipse Intensity Multiplier;                                        // 0x1DB8 (size: 0x8)
    double Sun Specular Scale;                                                        // 0x1DC0 (size: 0x8)
    bool Render Sun Directional Light;                                                // 0x1DC8 (size: 0x1)
    class ADirectionalLight* Custom Sun Light Actor;                                  // 0x1DD0 (size: 0x8)
    double Max Sun Source Angle Scale;                                                // 0x1DD8 (size: 0x8)
    double Sun Source Angle Softness;                                                 // 0x1DE0 (size: 0x8)
    double Scale Sun Radius As It Nears Horizon;                                      // 0x1DE8 (size: 0x8)
    bool Fade Down High Sun Light Intensity Below Horizon;                            // 0x1DF0 (size: 0x1)
    bool Support Sky Atmo Affecting Height Fog;                                       // 0x1DF1 (size: 0x1)
    FString UDS Version;                                                              // 0x1DF8 (size: 0x10)
    class UUDS_VersionInfo_C* UDS Version Info;                                       // 0x1E08 (size: 0x8)
    double Clouds B Time;                                                             // 0x1E10 (size: 0x8)
    double Sky Atmosphere Overcast Luminance;                                         // 0x1E18 (size: 0x8)
    bool Keep Planet Top At Camera XY Location;                                       // 0x1E20 (size: 0x1)
    bool Control Sky Atmosphere Settings;                                             // 0x1E21 (size: 0x1)
    FVector Old Composite Weather;                                                    // 0x1E28 (size: 0x18)
    FVector Old Composite Context;                                                    // 0x1E40 (size: 0x18)
    double Composite Weather Change Speed;                                            // 0x1E58 (size: 0x8)
    double Composite Context Change Speed;                                            // 0x1E60 (size: 0x8)
    FVector Old Moon Target;                                                          // 0x1E68 (size: 0x18)
    FVector Old Sun Target;                                                           // 0x1E80 (size: 0x18)
    double Cache Current Timer;                                                       // 0x1E98 (size: 0x8)
    FVector Last Low Frequency Update Location;                                       // 0x1EA0 (size: 0x18)
    class UUDS_Planet_Preset_C* Add Planet/Moon Preset;                               // 0x1EB8 (size: 0x8)
    TArray<FUDS_Space_Planet> Planets/Moons;                                          // 0x1EC0 (size: 0x10)
    double Space Layer Brightness (Night);                                            // 0x1ED0 (size: 0x8)
    double Space Layer Brightness (Day);                                              // 0x1ED8 (size: 0x8)
    double Space Glow Brightness;                                                     // 0x1EE0 (size: 0x8)
    TArray<class UMaterialInstanceDynamic*> Space Planet MIDs;                        // 0x1EE8 (size: 0x10)
    TArray<class USceneComponent*> Space Scene Components;                            // 0x1EF8 (size: 0x10)
    TArray<class UMaterialInstanceDynamic*> Space Ring MIDs;                          // 0x1F08 (size: 0x10)
    TArray<class UMaterialInstanceDynamic*> Space Glow MIDs;                          // 0x1F18 (size: 0x10)
    TArray<class UStaticMeshComponent*> Space Planet Components;                      // 0x1F28 (size: 0x10)
    bool Space Layer Active;                                                          // 0x1F38 (size: 0x1)
    FLinearColor Cached Solar Eclipse Tint;                                           // 0x1F3C (size: 0x10)
    double Cached Sun Scale;                                                          // 0x1F50 (size: 0x8)
    double Cached Moon Scale;                                                         // 0x1F58 (size: 0x8)
    bool Sun - Lighting Channel 0;                                                    // 0x1F60 (size: 0x1)
    bool Sun - Lighting Channel 1;                                                    // 0x1F61 (size: 0x1)
    bool Sun - Lighting Channel 2;                                                    // 0x1F62 (size: 0x1)
    bool Moon - Lighting Channel 0;                                                   // 0x1F63 (size: 0x1)
    bool Moon - Lighting Channel 1;                                                   // 0x1F64 (size: 0x1)
    bool Moon - Lighting Channel 2;                                                   // 0x1F65 (size: 0x1)
    TArray<class USceneComponent*> Space Roots;                                       // 0x1F68 (size: 0x10)
    double Water Level;                                                               // 0x1F78 (size: 0x8)
    bool Render Water Caustics;                                                       // 0x1F80 (size: 0x1)
    double Caustics Intensity;                                                        // 0x1F88 (size: 0x8)
    double Caustics Falloff;                                                          // 0x1F90 (size: 0x8)
    double Deep Water Falloff;                                                        // 0x1F98 (size: 0x8)
    double Caustics Texture Scale;                                                    // 0x1FA0 (size: 0x8)
    double Moons Cloud Mask;                                                          // 0x1FA8 (size: 0x8)
    double Blur Caustics With Depth;                                                  // 0x1FB0 (size: 0x8)
    double Blur Caustics In Cloud Shadow;                                             // 0x1FB8 (size: 0x8)
    double Blur Caustics With Camera Distance;                                        // 0x1FC0 (size: 0x8)
    int32 Event Minute;                                                               // 0x1FC8 (size: 0x4)
    int32 Sunset Event State;                                                         // 0x1FCC (size: 0x4)
    bool Disable Sky/Fog Cloudiness Above Cloud Layer;                                // 0x1FD0 (size: 0x1)
    double Camera Cloud Layer Normalized Height;                                      // 0x1FD8 (size: 0x8)
    double Camera Sky Atmosphere Normalized Density;                                  // 0x1FE0 (size: 0x8)
    FLinearColor Moon Fog Base Color;                                                 // 0x1FE8 (size: 0x10)
    FLinearColor Sun Fog Base Color;                                                  // 0x1FF8 (size: 0x10)
    FVector Last High Frequency Update Location;                                      // 0x2008 (size: 0x18)
    double Old Cloud Layer Camera Level;                                              // 0x2020 (size: 0x8)
    double Cloud Camera Relative Change Speed;                                        // 0x2028 (size: 0x8)
    FLinearColor Cached Sky Atmosphere Absorption Color;                              // 0x2030 (size: 0x10)
    int32 Cloud Coverage Target Resolution;                                           // 0x2040 (size: 0x4)
    class UTextureRenderTarget2D* Cloud Coverage Render Target;                       // 0x2048 (size: 0x8)
    int32 Cloud Coverage Target Size;                                                 // 0x2050 (size: 0x4)
    double Painted Cloud Coverage Opacity;                                            // 0x2058 (size: 0x8)
    double Layer 2 Painted Cloud Coverage Opacity;                                    // 0x2060 (size: 0x8)
    double Painted Coverage Affects Global Values;                                    // 0x2068 (size: 0x8)
    TMap<class FIntPoint, class UUDS_Cloud_Paint_Cell_C*> Local Painting Cell Data;   // 0x2070 (size: 0x50)
    int32 Local Painted Cell Size;                                                    // 0x20C0 (size: 0x4)
    bool Cloud Painting Active;                                                       // 0x20C4 (size: 0x1)
    FVector2D Cloud Coverage Target Location;                                         // 0x20C8 (size: 0x10)
    FLinearColor Current Cloud Coverage Render Target Mapping;                        // 0x20D8 (size: 0x10)
    bool Cloud Coverage Target in Use;                                                // 0x20E8 (size: 0x1)
    TArray<TSoftObjectPtr<UObject>> Async Loading Queue;                              // 0x20F0 (size: 0x10)
    TArray<class UObject*> Async Loaded Objects;                                      // 0x2100 (size: 0x10)
    bool Async Loading Active;                                                        // 0x2110 (size: 0x1)
    double Cloud Coverage After Painting;                                             // 0x2118 (size: 0x8)
    bool Cloud Paint Can Add Coverage;                                                // 0x2120 (size: 0x1)
    bool Cloud Paint Can Subtract Coverage;                                           // 0x2121 (size: 0x1)
    bool Enable Volumetric Cloud Light Rays;                                          // 0x2122 (size: 0x1)
    double Light Ray Intensity;                                                       // 0x2128 (size: 0x8)
    FLinearColor Light Ray Tint Color;                                                // 0x2130 (size: 0x10)
    double Light Rays Point Spacing;                                                  // 0x2140 (size: 0x8)
    double Light Ray Length;                                                          // 0x2148 (size: 0x8)
    double Individual Clouds Light Rays;                                              // 0x2150 (size: 0x8)
    double Scale Light Ray Width;                                                     // 0x2158 (size: 0x8)
    double Light Rays Max Distance (Km);                                              // 0x2160 (size: 0x8)
    double Light Rays Depth Fade Distance;                                            // 0x2168 (size: 0x8)
    double Light Rays 3D Noise Scale;                                                 // 0x2170 (size: 0x8)
    TSoftObjectPtr<UNiagaraSystem> Volumetric Cloud Light Rays Niagara System;        // 0x2178 (size: 0x28)
    double Max Light Ray Length;                                                      // 0x21A0 (size: 0x8)
    double Cloud Top World Height;                                                    // 0x21A8 (size: 0x8)
    FVector Cached Light Ray Vector;                                                  // 0x21B0 (size: 0x18)
    bool Light Rays Using Sun;                                                        // 0x21C8 (size: 0x1)
    double Cached Light Ray Strength;                                                 // 0x21D0 (size: 0x8)
    class UMaterialInstanceDynamic* Cloud Paint Draw MID;                             // 0x21D8 (size: 0x8)
    FLinearColor Volumetric Fog Albedo (Day);                                         // 0x21E0 (size: 0x10)
    FLinearColor Volumetric Fog Albedo (Dawn/Dusk);                                   // 0x21F0 (size: 0x10)
    FLinearColor Volumetric Fog Albedo (Night);                                       // 0x2200 (size: 0x10)
    bool Render Global Volumetric Material;                                           // 0x2210 (size: 0x1)
    double Fog Noise Intensity;                                                       // 0x2218 (size: 0x8)
    double Fog Noise Scale;                                                           // 0x2220 (size: 0x8)
    double Fog Noise Max Samples;                                                     // 0x2228 (size: 0x8)
    double Fog Noise Speed Scale;                                                     // 0x2230 (size: 0x8)
    double Mask Fog with Weather Masks;                                               // 0x2238 (size: 0x8)
    bool Render Ground Fog (Fog Above Distance Fields);                               // 0x2240 (size: 0x1)
    double Ground Fog Extinction (Foggy);                                             // 0x2248 (size: 0x8)
    double Ground Fog Extinction (Dusty);                                             // 0x2250 (size: 0x8)
    double Ground Fog Extinction (Rainy);                                             // 0x2258 (size: 0x8)
    double Ground Fog Extinction (Snowy);                                             // 0x2260 (size: 0x8)
    double Ground Fog Extinction (Manual);                                            // 0x2268 (size: 0x8)
    double Ground Fog Height;                                                         // 0x2270 (size: 0x8)
    double Ground Fog Height Noise;                                                   // 0x2278 (size: 0x8)
    double Ground Fog Angle Threshold;                                                // 0x2280 (size: 0x8)
    double Ground Fog Hardness;                                                       // 0x2288 (size: 0x8)
    bool Apply Water Fog Values;                                                      // 0x2290 (size: 0x1)
    double Water Extinction;                                                          // 0x2298 (size: 0x8)
    FLinearColor Water Albedo;                                                        // 0x22A0 (size: 0x10)
    TSoftObjectPtr<UMaterialInterface> Global Volumetric Fog Parent Material;         // 0x22B0 (size: 0x28)
    class UMaterialInstanceDynamic* Global Volumetric Fog MID;                        // 0x22D8 (size: 0x8)
    TSoftObjectPtr<UMaterialInterface> Global Volumetric Fog (Ground Fog ) Parent Material; // 0x22E0 (size: 0x28)
    FVector Fog Position;                                                             // 0x2308 (size: 0x18)
    class UCurveLinearColor* Sun Disk Color Curve;                                    // 0x2320 (size: 0x8)
    FVector Sun Target;                                                               // 0x2328 (size: 0x18)
    FVector Moon Target;                                                              // 0x2340 (size: 0x18)
    TSoftObjectPtr<UTexture2D> Static Clouds Texture;                                 // 0x2358 (size: 0x28)
    double Static Clouds Rotation;                                                    // 0x2380 (size: 0x8)
    double Static Clouds Rotation Speed;                                              // 0x2388 (size: 0x8)
    float Static Clouds Color Intensity;                                              // 0x2390 (size: 0x4)
    double Cloud Movement Timer Length;                                               // 0x2398 (size: 0x8)
    FTimerHandle Cloud Movement Cache Timer;                                          // 0x23A0 (size: 0x8)
    double Clouds B Time Old;                                                         // 0x23A8 (size: 0x8)
    FLinearColor Clouds Position Color A;                                             // 0x23B0 (size: 0x10)
    FLinearColor Clouds Position Color B;                                             // 0x23C0 (size: 0x10)
    FLinearColor Fog Position Color A;                                                // 0x23D0 (size: 0x10)
    FLinearColor Fog Position Color B;                                                // 0x23E0 (size: 0x10)
    double Clouds Time Elapsed Last Update;                                           // 0x23F0 (size: 0x8)
    int32 Next Cache Step;                                                            // 0x23F8 (size: 0x4)
    TArray<bool> Cache Group Booleans;                                                // 0x2400 (size: 0x10)
    double Current Cache Timer Speed;                                                 // 0x2410 (size: 0x8)
    int32 Low Priority Update Step;                                                   // 0x2418 (size: 0x4)
    int32 Active Update Speed;                                                        // 0x241C (size: 0x4)
    int32 High Priority Update Step;                                                  // 0x2420 (size: 0x4)
    int32 Non Cached Update Step;                                                     // 0x2424 (size: 0x4)
    double Max Property Cache Period - Time Of Day Transition;                        // 0x2428 (size: 0x8)
    int32 Minimum Active Update Speed;                                                // 0x2430 (size: 0x4)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Moon;                 // 0x2438 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Fog;                  // 0x2448 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Sky Material;         // 0x2458 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Sky Atmosphere;       // 0x2468 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Simplified Color;     // 0x2478 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Sky Light;            // 0x2488 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Sun;                  // 0x2498 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group Volumetric Clouds;    // 0x24A8 (size: 0x10)
    TArray<TEnumAsByte<UDS_CachedProperties::Type>> Cache Group 2D Clouds;            // 0x24B8 (size: 0x10)
    double Wind Speed Multiplier;                                                     // 0x24C8 (size: 0x8)
    double Fog Vertical Velocity;                                                     // 0x24D0 (size: 0x8)
    double Volumetric Aurora Fade Distance (km);                                      // 0x24D8 (size: 0x8)
    double Cinematic Clouds View Sample Scale;                                        // 0x24E0 (size: 0x8)
    double Cinematic Clouds Shadow Sample Scale;                                      // 0x24E8 (size: 0x8)
    double Cinematic Clouds Tracing Max Distance;                                     // 0x24F0 (size: 0x8)
    int32 Cinematic Clouds View Ray Sample Max Count;                                 // 0x24F8 (size: 0x4)
    TArray<class UUDS_Modifier_C*> Current Modifiers;                                 // 0x2500 (size: 0x10)
    TArray<double> Modifier Alphas;                                                   // 0x2510 (size: 0x10)
    TArray<double> Modifier Targets;                                                  // 0x2520 (size: 0x10)
    TArray<double> Modifier Speeds;                                                   // 0x2530 (size: 0x10)
    TArray<class UUDS_Modifier_C*> Starting Modifiers;                                // 0x2540 (size: 0x10)
    bool Preview Starting Modifiers in Editor;                                        // 0x2550 (size: 0x1)
    bool Replicate Modifiers To Clients;                                              // 0x2551 (size: 0x1)
    bool Using Volumetric Light Rays;                                                 // 0x2552 (size: 0x1)
    int32 Last Material Quality Level;                                                // 0x2554 (size: 0x4)
    int32 Last Effects Quality Level;                                                 // 0x2558 (size: 0x4)
    bool Instant Exposure Adjustment on Begin Play;                                   // 0x255C (size: 0x1)
    class UPostProcessComponent* Instant Exposure Post Process;                       // 0x2560 (size: 0x8)
    class UObject* Editor Tick Handler;                                               // 0x2568 (size: 0x8)
    double Tick Delta Seconds;                                                        // 0x2570 (size: 0x8)
    FVector Editor Camera Position;                                                   // 0x2578 (size: 0x18)
    FVector Editor Camera Position Offset;                                            // 0x2590 (size: 0x18)
    double Last Editor Tick Time;                                                     // 0x25A8 (size: 0x8)
    TEnumAsByte<UDS_RunContext::Type> Run Context;                                    // 0x25B0 (size: 0x1)
    double Slow Falling Combined Change Speed;                                        // 0x25B8 (size: 0x8)
    double Combined Change Speed;                                                     // 0x25C0 (size: 0x8)
    class UStaticMesh* Custom Sky Sphere Static Mesh;                                 // 0x25C8 (size: 0x8)
    double Fallback Cloud Layer Altitude;                                             // 0x25D0 (size: 0x8)
    bool Level Editor Tick;                                                           // 0x25D8 (size: 0x1)
    double Time of Last Cloud Cache;                                                  // 0x25E0 (size: 0x8)
    class UUDS_OcclusionState_C* Occlusion State;                                     // 0x25E8 (size: 0x8)
    double Last Static Clouds Update Time;                                            // 0x25F0 (size: 0x8)
    double Static Clouds Dynamic Rotation;                                            // 0x25F8 (size: 0x8)
    FRotator Editor Camera Rotation;                                                  // 0x2600 (size: 0x18)
    double Cached Cloud Shadows Cancel Value;                                         // 0x2618 (size: 0x8)
    double Interior Occlusion Change Speed;                                           // 0x2620 (size: 0x8)
    double Time of Last Cache Timing Update;                                          // 0x2628 (size: 0x8)
    double Old Interior Occlusion;                                                    // 0x2630 (size: 0x8)
    bool Transitioning Sky Light Intensity;                                           // 0x2638 (size: 0x1)
    bool Cache Sun Cast Shadows;                                                      // 0x2639 (size: 0x1)
    double Eclipse Percent;                                                           // 0x2640 (size: 0x8)
    double Sun Shadows Cutoff Z;                                                      // 0x2648 (size: 0x8)
    FUltra_Dynamic_Sky_CCurrent Hour Changed Current Hour Changed;                    // 0x2650 (size: 0x10)
    void Current Hour Changed(int32 Hour);
    FUltra_Dynamic_Sky_CEvery Minute Every Minute;                                    // 0x2660 (size: 0x10)
    void Every Minute(int32 Minute (0-59));
    bool Called Starting Event Dispatchers;                                           // 0x2670 (size: 0x1)
    FVector World Origin Location;                                                    // 0x2678 (size: 0x18)
    TSoftClassPtr<UObject> Editor Tick Handler Class;                                 // 0x2690 (size: 0x28)
    TSoftClassPtr<UObject> Editor Utility Opener Class;                               // 0x26B8 (size: 0x28)
    FDateTime Event Date;                                                             // 0x26E0 (size: 0x8)
    FUltra_Dynamic_Sky_CDate Changed Date Changed;                                    // 0x26E8 (size: 0x10)
    void Date Changed();
    bool Initial Replication;                                                         // 0x26F8 (size: 0x1)
    FDateTime Starting Date;                                                          // 0x2700 (size: 0x8)
    double Clouds Time Based Movement Offset;                                         // 0x2708 (size: 0x8)
    TArray<double> Unmodified Floats;                                                 // 0x2710 (size: 0x10)
    TArray<FLinearColor> Unmodified Colors;                                           // 0x2720 (size: 0x10)
    bool Modifiers Ticking;                                                           // 0x2730 (size: 0x1)
    TArray<TEnumAsByte<UDS_Modifier_Float_Property::Type>> Modified Float Properties; // 0x2738 (size: 0x10)
    TArray<TEnumAsByte<UDS_Modifier_Color_Property::Type>> Modified Color Properties; // 0x2748 (size: 0x10)
    bool Move Sky Light Location;                                                     // 0x2758 (size: 0x1)
    bool Sky Light Movement uses Z Axis;                                              // 0x2759 (size: 0x1)
    double Weather Cloud Coverage;                                                    // 0x2760 (size: 0x8)
    double Weather Fog;                                                               // 0x2768 (size: 0x8)
    double Weather Dust Amount;                                                       // 0x2770 (size: 0x8)
    FLinearColor Sky Glow Fog Color;                                                  // 0x2778 (size: 0x10)
    FLinearColor Cached Night Sky Glow;                                               // 0x2788 (size: 0x10)
    FLinearColor Cached Light Pollution;                                              // 0x2798 (size: 0x10)
    double Local Cloud Coverage;                                                      // 0x27A8 (size: 0x8)
    class UMaterialInstanceDynamic* Storm Clouds Draw MID;                            // 0x27B0 (size: 0x8)
    bool Cloud Painting Present;                                                      // 0x27B8 (size: 0x1)
    bool Radial Storm Clouds Present;                                                 // 0x27B9 (size: 0x1)
    TArray<FRadialStorm_CoverageBrush> Radial Storm Cloud Coverage Cache;             // 0x27C0 (size: 0x10)
    TMap<FString, double> Cvar Cache;                                                 // 0x27D0 (size: 0x50)
    class AUDS_Cloud_Paint_Container_C* Cloud Paint Container;                        // 0x2820 (size: 0x8)
    TArray<class AActor*> Radial Storms;                                              // 0x2828 (size: 0x10)
    FRotator Sun World Rotation;                                                      // 0x2838 (size: 0x18)
    FRotator Moon World Rotation;                                                     // 0x2850 (size: 0x18)
    class USkyLightComponent* Path Tracer Sky Light;                                  // 0x2868 (size: 0x8)
    class UStaticMeshComponent* Compass Mesh;                                         // 0x2870 (size: 0x8)
    class UStaticMeshComponent* Global Volumetric Fog Mesh;                           // 0x2878 (size: 0x8)
    class UUDS_PlayerOcclusion_C* Player Occlusion;                                   // 0x2880 (size: 0x8)
    class UStaticMeshComponent* 2D Turbulence Disk;                                   // 0x2888 (size: 0x8)
    class UNiagaraComponent* Volumetric Cloud Light Rays;                             // 0x2890 (size: 0x8)
    class UStaticMeshComponent* Inside Cloud Fog Mesh;                                // 0x2898 (size: 0x8)
    double Volumetric Cloud Shadow Altitude;                                          // 0x28A0 (size: 0x8)
    TArray<bool> Post Process Mats Toggle State;                                      // 0x28A8 (size: 0x10)
    FWeightedBlendable Lens Flare WB;                                                 // 0x28B8 (size: 0x10)
    FWeightedBlendable Cloud Fog PP WB;                                               // 0x28C8 (size: 0x10)
    class UPostProcessComponent* Static PPM Component;                                // 0x28D8 (size: 0x8)
    TSoftObjectPtr<UMaterialInterface> Path Tracer Fog Parent Mat;                    // 0x28E0 (size: 0x28)
    class UMaterialInstanceDynamic* Path Tracer Fog MID;                              // 0x2908 (size: 0x8)
    FWeightedBlendable Path Tracer Fog WB;                                            // 0x2910 (size: 0x10)
    bool Render Height Fog in Path Tracer using Post Process;                         // 0x2920 (size: 0x1)
    class UTexture* Cloud Profile LUT Preview;                                        // 0x2928 (size: 0x8)
    bool Previewing Cloud Profile Editor;                                             // 0x2930 (size: 0x1)
    bool Alternate Tick;                                                              // 0x2931 (size: 0x1)
    class UCurveFloat* Sky Atmosphere Density Curve;                                  // 0x2938 (size: 0x8)
    bool Allow Disabling Directional Shadows with Coverage;                           // 0x2940 (size: 0x1)
    bool Apply Flat Cloudiness;                                                       // 0x2941 (size: 0x1)
    bool Using Sky Atmosphere;                                                        // 0x2942 (size: 0x1)
    double Cached Moon Effective Illumination 0-1;                                    // 0x2948 (size: 0x8)
    double Cached Absent Directional Lights Brightness;                               // 0x2950 (size: 0x8)
    double Cached Current Moon Lit Percent;                                           // 0x2958 (size: 0x8)
    bool Using Space Mode;                                                            // 0x2960 (size: 0x1)
    bool Using Volumetric Clouds;                                                     // 0x2961 (size: 0x1)
    bool Using Static Clouds;                                                         // 0x2962 (size: 0x1)
    bool Using 2D Dynamic Clouds;                                                     // 0x2963 (size: 0x1)
    bool Using Volumetric Aurora;                                                     // 0x2964 (size: 0x1)
    double Cached Lit Intensity;                                                      // 0x2968 (size: 0x8)
    double Cached Directional Light Dimming;                                          // 0x2970 (size: 0x8)
    bool Modifiers Animating;                                                         // 0x2978 (size: 0x1)
    double Cached Directional Inscattering Multiplier;                                // 0x2980 (size: 0x8)
    bool Moon Renders Behind Space Layer;                                             // 0x2988 (size: 0x1)
    FLinearColor Cached Fog Directional Inscattering;                                 // 0x298C (size: 0x10)
    double Sun Moon Delta;                                                            // 0x29A0 (size: 0x8)
    TEnumAsByte<ECollisionChannel> Weather Particle Collision Channel;                // 0x29A8 (size: 0x1)
    bool Use Legacy Cloud Coverage Range;                                             // 0x29A9 (size: 0x1)
    TArray<int32> Cache Group Countdowns;                                             // 0x29B0 (size: 0x10)

    void Get Cloud Coverage 0-10(double& Cloud Coverage);
    double Current Sun Moon Cache Delta();
    void Volumetric Light Ray Strength and Color(double& Ray Strength, FLinearColor& Ray Color);
    void Static Mode Cloud Tick();
    void Static Mode Startup();
    void Startup Tick();
    void Set Previous Weather Variables();
    void Update Path Tracer Fog();
    void Construct Path Tracer Fog();
    void Add Static Post Process Materials();
    void Check to Start Volumetric Cloud Light Rays();
    void Add Constructed Components();
    void Toggle Post Process Material(int32 Index, bool Enabled);
    void Get Starting Cloud Painting Actors();
    void Revert Changed Console Variables();
    void Reverse Day Ended();
    double Fog and Dust Shadow Value();
    void Simplified Cloud Light Color(FLinearColor& Out);
    void Show Editor Warning(FString Title, FString Message);
    void Apply Feature Level Mode Changes(bool& Made Changes);
    void Set Dust Amount(double Dust Amount);
    void Set Fog(double Fog);
    void Set Cloud Coverage(double Cloud Coverage);
    void Apply Editor Weather Override();
    void Apply Starting Modifiers();
    void Revert Modified Properties();
    void Set Startup Variables();
    void Update Total Days Elapsed();
    void Client Check Initial Replication();
    FDateTime Current Event Date();
    void Set Variables Controlled by Weather();
    void Composite Context Vector(FVector& Out);
    void Composite Weather Vector(FVector& Out);
    void Get Manual Target Change Speed(FVector& Old Vector, FVector New Vector);
    void Adjust for World Origin Rebasing();
    void Current Minute as Integer(int32& Minute);
    void Time Of Day to H/M/S(double Time, int32& Hour, int32& Minute, int32& Second, double& Second Fraction);
    void H/M/S/MS to Time of Day(int32 Hours, int32 Minutes, int32 Seconds, int32 Miliseconds, double& Time);
    double Moon Effective Illumination 0-1();
    void Update Water Level Parameter();
    void Is Cached Value Changing(TEnumAsByte<UDS_CachedProperties::Type> Enum, bool& Yes);
    void Set Time Cycle Degrees();
    void Current Sunset/Sunrise Event State(bool& Sun Up);
    bool Is Date and Time in Daylight Savings Time();
    void Apply System Time();
    void Editor Update from Weather();
    void Sky Startup Functions();
    void Static Properties - Occlusion();
    void Static Properties - Mode Derivatives();
    void Static Properties - Post Processing();
    bool Runtime Or Initializing();
    void Start Editor Tick Handler();
    void Editor Tick(FVector Editor Camera Location, FRotator Editor Camera Rotation, bool Sequencer Open);
    void Current Hour as Integer(int32& Hour);
    void Open Editor Readme Entry Set(TArray<FName>& Entry);
    void 📘 Configuring for Performance();
    void 📘 Underwater Caustics();
    void 📘 Screen Space Light Shafts();
    void 📘 Considerations for Mobile();
    void 📘 Interior Adjustments();
    void 📘 Cinematic / Offline Rendering();
    void 📘 Sky Modifiers();
    void 📘 Volumetric Cloud Painting();
    void 📘 Volumetric Cloud Light Rays();
    void 📘 Post Processing();
    void 📘 Sun Lens Flare();
    void 📘 Aurora();
    void 📘 Sky Glow();
    void 📘 Dust();
    void 📘 Space Layer();
    void 📘 Stars();
    void 📘 Simplified Color();
    FLinearColor Simplified Moon Scattering Color();
    void 📘 Sky Atmosphere();
    void 📘 Volumetric Fog();
    void 📘 Fog Density();
    void 📘 Fog Color();
    void 📘 Exposure();
    void 📘 Cloud Shadows();
    void 📘 Directional Light();
    void 📘 Sky Light();
    void 📘 Moon();
    void 📘 Sun();
    void 📘 Cloud Wisps();
    void 📘 Simulation();
    void 📘 Cloud Movement();
    void 📘Static Clouds();
    void Open Editor Readme Entry(FString Entry);
    void Start Instant Exposure();
    void 📘Time of Day();
    void 📘 UDS Documentation();
    void 📘2D Dynamic Clouds();
    void 📘Volumetric Clouds();
    void Disable Instant Exposure();
    void Static Properties - Stars();
    void Static Properties - Lens Flare();
    void Static Properties - Space Layer();
    void Static Properties - Static Clouds();
    void Static Properties - Aurora();
    void Static Properties - Sky Atmosphere();
    void Static Properties - Simplified Color();
    void Static Properties - Water Caustics();
    void Static Properties - Cloud Shadows();
    void Static Properties - 2D Dynamic Clouds();
    void Static Properties - Misc();
    void Static Properties - Sky Material();
    void Static Properties - Height Fog();
    void Static Properties - Sun();
    void Static Properties - Moon();
    void Static Properties - Sky Light();
    void Static Properties - Volumetric Cloud Light Rays();
    void Static Properties - Volumetric Clouds();
    void Static Properties - Cloud Movement();
    void Update Non-Cached Active Properties();
    void Apply Modifier Property Overrides(class UUDS_Modifier_C* Modifier, double Alpha);
    void Apply Modifiers();
    void Clear Modifiers();
    void Set Modifier State(class UUDS_Modifier_C* Modifier, double Alpha);
    void Remove Modifier(class UUDS_Modifier_C* Modifier, double Fade Out Time);
    void Add Modifier(class UUDS_Modifier_C* Modifier, double Fade In Time);
    void Set Unmodified Property Values();
    void Cinematic Mode Startup();
    double Get Inverted Global Occlusion();
    void Construct Inside Cloud Fog();
    void Configure Height Fog with Feature Toggle();
    void Configure Sky Light with Feature Toggle();
    void Configure Directional Light with Feature Toggle(bool Toggle, class UDirectionalLightComponent*& Selected Component Variable, class UDirectionalLightComponent* Built in Light Component, class ADirectionalLight* Custom Light Actor, int32 Atmospheric Index, TEnumAsByte<EComponentMobility::Type> Mobility, bool Light Visibility);
    void Construct Volumetric Aurora();
    void Construct Volumetric Clouds();
    void Construct Cloud Shadows MID();
    void Construct Sky Sphere and Material();
    void Number of Days in a Year(int32 Year, int32& Count);
    void Current Month Lengths(int32 Year, TArray<int32>& Lengths);
    void Day Count at the Start of a Month(int32 Year, int32 Month, int32& Count);
    void Days Since Y1D1M1(int32 Input Day, int32 Input Month, int32 Input Year, int32& Days);
    void Offset Date by a Number of Days(int32 Input Month, int32 Input Day, int32 Input Year, int32 Offset, int32& Output Month, int32& Output Day, int32& Output Year);
    void Applied Cloud Speed(double& Out);
    void Season Value for Weather from Date and Time(int32 Day Offset, double& Season);
    void Save Calendar Data();
    void Date and Time to Year Progress(int32 Month, int32 Day, double Time, double& Days from Start of Year);
    void Check If Year is Leap Year(int32 Year, bool& Leap Year);
    void Time of Day Animation();
    void Increment Time of Day Forward(double Amount);
    void Starting Animate Time of Day Offset();
    void Start Up UDW If it Exists();
    void Day Ended();
    bool Lights Update Degree Threshold Test(FRotator World Rotation, class USceneComponent* Light);
    void 2D Clouds Shading Offset Vector(FLinearColor& LinearColor);
    void Update Cache Group Boolean(TEnumAsByte<UDS_Cache_Group::Type> Group, TArray<TEnumAsByte<UDS_CachedProperties::Type>>& Properties);
    void Update High Priority Properties();
    void Update Low Priority Properties();
    void Increment Cloud Movement Cache();
    void Check for Time Event Dispatchers();
    void Update Lunar Phase(int32 Hour);
    void Start Active Timers();
    void Static Clouds Tint Color(FLinearColor& Light, FLinearColor& Shadow);
    void Randomize Time Of Day();
    void Cloud Texture Pan Scale(double& Scale);
    void Update Global Volumetric Fog Material();
    void Set Up Global Volumetric Fog Material();
    double Current Volumetric Cloud Sky Atmo Contribution();
    double Overcast Brightness();
    void Notify of Removed Cloud Paint Container();
    void Update Volumetric Cloud Light Rays();
    void Update Cloud Coverage After Painting();
    void Test Point for Painted Cloud Coverage(FVector Location, double& Cloud Coverage with Painting);
    void Update Sky Atmosphere Location(FVector Location);
    void Current Volumetric Cloud Multiscattering Intensity(double& Out);
    double Current Overcast Turbulence Strength();
    void Current Cloud Wisps Color(FLinearColor& Out);
    void Night Filtered Night Brightness(double& Multiplier);
    void Get Nearby Cloud Cells To Load Asynchronously();
    void Release Async Loaded Object(TSoftObjectPtr<UObject> Object);
    void Add Object to Async Loading Queue(TSoftObjectPtr<UObject> Object, bool High Priority);
    void Check for Cloud Coverage Target Recenter();
    void World Space to Drawn Target Pixel Space(FVector2D In, FVector2D& Out);
    void Cloud Coverage Target Mapping(FVector& Mapping);
    void Update Painted Cloud Coverage Target();
    void Current Sky Atmosphere Absorption Color(FLinearColor& Out);
    void Current Volumetric Cloud Inner Emit Limit(double& Out);
    FLinearColor Current Cloud Fog Post Process Color();
    double Inside Outside Cloud Layer();
    void Overcast Luminance Boost(double Multiplier, double& Out);
    void Volumetric Cloud Ambient Light Color(FLinearColor& Bottom, FLinearColor& Top);
    void Get Cloud Coverage Local(double& Local Height);
    void Get Cloud Coverage 0-3(double& Cloud Coverage);
    void Cloud Layer Top and Bottom World Height(double& Bottom World Height, double& Top World Height);
    void Camera Location Dependent Updates();
    void Get Editor Camera Location();
    void Get Runtime Camera Location();
    void Apply Light Shaft Settings(class UDirectionalLightComponent* Light, FVector2D Max Brightness, FVector2D Bloom Threshold, FVector2D Bloom Scale, FVector Forward Vector);
    void Check if Point is Exposed to Sun or Moon Light(FVector Location, double Distance to Trace, TArray<class AActor*>& Actors to Ignore in Trace, bool& Exposed to Sun, bool& Exposed to Moon, bool& Exposed to Either, double& Light Intensity (Lux));
    double Current Moons Cloud Mask();
    void Swap with Cinematic Runtime Value(double In, double Cine, bool Use Higher, double& Out);
    void Solar Eclipse Circle Mask(double Sun Angular Diameter, double Moon Angular Diameter, FVector Moon Vector, double Moon Softness, double& Fraction Showing);
    double Current Space Layer Brightness();
    void Space Planet Parent MID(FUDS_Space_Planet Planet, TSoftObjectPtr<UMaterialInterface>& Out);
    void Update Space Layer Vectors();
    void Moon Z Vector(FVector& Out);
    void Sun Z Vector(FVector& Out);
    void Construct Space Layer();
    void Increment Cache Timer();
    void Combined Night Brightness(double& Out);
    void Restart Real Time Sky Light Capture();
    void Days Since J2000(int32 Input Day, int32 Input Month, int32 Input Year, int32& Days);
    void Cloud Wisp Gradient Vector(FLinearColor& Out);
    double Current Directional Inscattering Exponent();
    void Current View Sample Scale(double& Out);
    void Three Time Floats(double Day, double Dawn/Dusk, double Night, double& Out);
    void Update Post Process Blend Weights();
    void Create Post Process Components();
    void Change Sky Mode at Runtime(TEnumAsByte<UDS_SkyMode::Type> New Sky Mode);
    void Apply Location Preset(TEnumAsByte<UDS_CityPresets::Type> Location);
    void Current Volumetric Cloud Shadow Tracing Distance(double& Out);
    FLinearColor Current Sky Atmosphere Luminance();
    void Get UDW Reference();
    void Volumetric Clouds SubNoise Scales(FLinearColor& High, FLinearColor& Low);
    void Volumetric Cloud Floor Variation(double& Height Clear, double& Height Cloudy, double& Color);
    void Static Clouds Lighting Mask(FLinearColor& XY Mask);
    void Starting Cloud Formation();
    void Simplified Sun Glow Color(FLinearColor& Out);
    void Size Cache Arrays();
    void Current Exposure Bias(double& Bias);
    void Monitor for Changes();
    void Cache Timer And Update Speed(bool& Hard Cache Reset);
    void Get Cached Vector(TEnumAsByte<UDS_CachedProperties::Type> Property, FVector& Value);
    void Cache Vector(TEnumAsByte<UDS_CachedProperties::Type> Property, FVector Set Value, double Change Tolerance);
    void Cache Sun and Moon Orientation();
    void Get Cached Color(TEnumAsByte<UDS_CachedProperties::Type> Property, FLinearColor& Value);
    void Cache Color(TEnumAsByte<UDS_CachedProperties::Type> Property, FLinearColor Set Value, double Change Tolerance);
    void Cache Float(TEnumAsByte<UDS_CachedProperties::Type> Property, double Set Value, double Change Tolerance);
    void Cache Properties(int32 Cache Group, bool Starting Cache Fill);
    void Get Cached Float(TEnumAsByte<UDS_CachedProperties::Type> Property, double& Value);
    void Hard Reset Cache();
    void Sun Height(bool Cached, double& Z);
    void Current Solar Eclipse Values(double& Eclipse Percent, FLinearColor& Tint Color);
    void Adjust Base Sun Light Intensity(FVector Sun Vector, double& Intensity, double& Multiplier);
    void Filtered Moon Light Intensity(double Unfiltered, double& Intensity);
    void Initialize Occlusion();
    void Tick Time Transition();
    void Finish Time Transition();
    void Directional Light Dimming(double& Out);
    void Lens Flare Parent Material(TSoftObjectPtr<UMaterialInterface>& Mat);
    void Cloud Shadows Parent Material(TSoftObjectPtr<UMaterialInterface>& Mat);
    void Load Required Assets();
    void Twilight Brightness Falloff(double Z, double& Scale);
    void Construct Overcast Turbulence();
    void Update Overcast Turbulence();
    void Update Current Volumetric Clouds MID();
    void Current Max Trace Distance(double& Out);
    void Directional Source Angle(double Disk Scale, double Max Scale Factor, double Scale Setting, double& Out);
    void Cloud Shadows Light Vector And Cancel Value(FLinearColor& Vector, double& Cancel Value);
    void Apply Console Variable with Check(FString CVar, double setting, int32 Type);
    void Current Moon Light Color(FLinearColor& Color);
    void 3 Color Time Blend(FLinearColor Day Color, FLinearColor Dawn/Dusk Color, FLinearColor Night Color, FLinearColor& Out);
    FLinearColor Simplfied Color Sun Scattering();
    void Directional Inscattering Multiplier(double& Multiplier);
    void Query Project Settings And UDS Version();
    void Sky Atmosphere Fog Contribution(double& Output);
    void Volumetric Cloud Layer Scale(double& Layer Scale);
    void Volumetric Cloud First Layer Top Altitude(double& Cloud Top Altitude);
    void Volumetric Cloud Layer Height(double Base Cloud Height, double& Layer Height);
    void Volumetric Cloud Base Cloud Height(double& Base Cloud Height);
    double Current Volumetric Multiscattering Phase 1();
    void Current Sky Ambient Color(FLinearColor& Sky Ambient Color);
    void Current Fog Directional Inscattering Color(FLinearColor& Directional Inscattering Color);
    void Current Fog Inscattering Color(FLinearColor& Inscattering Color);
    void Set Current Fog Base Colors();
    void Scale Sample Count(double In, double& Out);
    void Fire Editor Dispatchers();
    double Current Wisps Opacity();
    double Current Overall Intensity();
    double Current Lit Intensity();
    double Fog Start Distance();
    double Fog Height Falloff();
    double Current Fog Density();
    TSoftObjectPtr<UMaterialInterface> Get Sky MID Parent Material Instance();
    double Current Sunset Event Time();
    double Current Sunrise Event Time();
    double Clouds Time of Day Factor();
    double Total Time Elapsed();
    void Time of Day Offset(double& Per Second);
    FLinearColor Current 2D Cloud Tint();
    double Night Filter(bool Cached);
    double Absent Directional Lights Brightness();
    double Moon Phase Light Multiplier(double& Without Light Brightness);
    bool Dimming Directional Lights();
    FLinearColor Current Light Pollution();
    double Current Aurora Intensity();
    FLinearColor Current Night Sky Glow();
    FLinearColor Current Stars Color();
    FLinearColor Current Rayleigh Scattering Color();
    double Current Mie Scattering Scale();
    double Current Mie Anisotropy();
    void Get Current Sky Light Color and Intensity(double& Out Intensity, FLinearColor& Out Color);
    FLinearColor Current Sky Light Lower Hemisphere Color();
    FLinearColor Current Moon Light Material Color();
    double Current Moon Lit Percent();
    double Current Moon Material Intensity();
    FLinearColor Current Moon Phase Angle();
    void Current Moon Scale(double& Scale);
    double Moon Light Specular Scale();
    double Moon Light Volumetric Scattering Intensity();
    void Unfiltered Moon Light Intensity(double& Out);
    double Current Sun Radius();
    double Current Sun Light Intensity();
    double Current Sun Specular Scale();
    void Is Directional Light Casting Shadows(bool Force Disabled, FVector Light Vector, bool& Yes);
    FLinearColor Current Sun Light Color();
    FLinearColor Current Sun Disk Color();
    double Current Sun Disk Intensity();
    double Cloud Shadows Cloud Density();
    void Get Volumetric Cloud Emissive Colors(FLinearColor& Bottom, FLinearColor& Top);
    void All Volumetric Cloud MIDs(TArray<class UMaterialInstanceDynamic*>& MIDs);
    double Current Base Clouds Scale();
    void Current Lerp to Simplified Clouds(double& Alpha);
    void Volumetric Clouds Parent Materials(TSoftObjectPtr<UMaterialInterface>& Simplified, TSoftObjectPtr<UMaterialInterface>& Complex);
    double Current Volumetric Cloud Macro Variation();
    FLinearColor Current Volumetric Cloud Albedo();
    void Current Volumetric Cloud Multiscattering Occlusion(double& Occlusion);
    void Get Current Volumetric Cloud Extinction Scale(double& Top);
    void Current Volumetric Clouds Density(bool Layer 1, double& Out);
    void Update Dynamic Sky Light Multiplier();
    void Transition Sky Light Intensity(double New Sky Light Intensity Multiplier, double Transition Time);
    void Transition Time of Day(double New Time of Day, double Transition Duration (Seconds), TEnumAsByte<EEasingFunc::Type> Easing Function, double Easing Exponent, bool Allow Time Going Backwards);
    void Update Lens Flare();
    void Construct Lens Flare();
    void Apply Volumetric Mode(TEnumAsByte<UDS_VolRT_Mode::Type> Mode);
    void Find Real Sunset/Sunrise Times();
    void Apply Saved UDS and UDW State(FUDS_and_UDW_State State);
    void Create UDS and UDW State for Saving(FUDS_and_UDW_State& Packaged State);
    void Update Replicated Time();
    void OnRep_Replicated Time of Day();
    void Check for Daylight Savings Time(int32 Hour);
    void Offset Date(int32 Offset);
    void Force Valid Day();
    void Approximate Real Sun Moon and Stars(double Time of Day, bool Only Calculate Sun, FVector& Sun Vector, FVector& Moon Vector, double& Real Phase, FVector& Phase Alignment, FLinearColor& Celestial Yaw, double& Celestial Orbit);
    void Set Date and Time(FDateTime Date Time);
    void Update Directional Light Rotations();
    void Recapture Sky Light();
    void Update Settings Based on Scalability();
    void Construction Script Function();
    void Update Cloud Movement();
    FVector Cloud Texture Velocity();
    void Update Static Variables();
    void Set Time with Time Code(FTimecode Time Code);
    void Get Day of the Week(int32& Index, FString& Name);
    void Is it Daytime?(bool& Yes);
    void Get DateTime(FDateTime& Current Date and Time);
    void Get TimeCode(FTimecode& Time);
    void Update Active Variables();
    void Update Common Derivatives();
    void Tick Function();
    void Set Sun and Moon Root Rotation();
    void UserConstructionScript();
    void OnLoaded_AA91001A446E22425F2F54AAB2159C50(class UObject* Loaded);
    void UDW Instant Update();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void UDW Editor Update();
    void Replicate Modifier State(const TArray<class UUDS_Modifier_C*>& Modifiers, const TArray<double>& Modifier Alphas, const TArray<double>& Modifier Targets, const TArray<double>& Modifier Speeds, const bool Hard Reset);
    void Start Async Loader();
    void Timed Override with New Changes();
    void ReceiveTick(float DeltaSeconds);
    void ReceiveBeginPlay();
    void Force Startup();
    void ExecuteUbergraph_Ultra_Dynamic_Sky(int32 EntryPoint);
    void Date Changed__DelegateSignature();
    void Current Hour Changed__DelegateSignature(int32 Hour);
    void Every Minute__DelegateSignature(int32 Minute (0-59));
    void Hourly__DelegateSignature(int32 Hour);
    void Midnight__DelegateSignature();
    void Sunrise__DelegateSignature();
    void Sunset__DelegateSignature();
}; // Size: 0x29C0

#endif
