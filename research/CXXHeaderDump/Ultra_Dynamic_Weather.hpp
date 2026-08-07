#ifndef UE4SS_SDK_Ultra_Dynamic_Weather_HPP
#define UE4SS_SDK_Ultra_Dynamic_Weather_HPP

class AUltra_Dynamic_Weather_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UPostProcessComponent* PostProcess;                                         // 0x02A0 (size: 0x8)
    class UAudioComponent* Sound_Global;                                              // 0x02A8 (size: 0x8)
    class UAudioComponent* Sound_Directional_Y;                                       // 0x02B0 (size: 0x8)
    class UAudioComponent* Sound_Directional_X;                                       // 0x02B8 (size: 0x8)
    class UUDW_Temperature_Manager_C* Temperature Manager;                            // 0x02C0 (size: 0x8)
    class UUDW_Material_State_Manager_C* Material State Manager;                      // 0x02C8 (size: 0x8)
    class UUDW_Lightning_Spawn_Manager_C* Lightning Spawn Manager;                    // 0x02D0 (size: 0x8)
    class UUDS_PlayerOcclusion_C* Player Occlusion;                                   // 0x02D8 (size: 0x8)
    class URandom_Weather_Variation_C* Random Weather Manager;                        // 0x02E0 (size: 0x8)
    class UDirectionalLightComponent* Lightning Light;                                // 0x02E8 (size: 0x8)
    class UBillboardComponent* Root;                                                  // 0x02F0 (size: 0x8)
    class AUltra_Dynamic_Sky_C* UltraDynamicSky;                                      // 0x02F8 (size: 0x8)
    bool Refresh Settings;                                                            // 0x0300 (size: 0x1)
    class UUDS_Weather_Settings_C* Weather;                                           // 0x0308 (size: 0x8)
    double Cloud Coverage;                                                            // 0x0310 (size: 0x8)
    bool Cloud Coverage - Manual Override;                                            // 0x0318 (size: 0x1)
    double Rain;                                                                      // 0x0320 (size: 0x8)
    bool Rain - Manual Override;                                                      // 0x0328 (size: 0x1)
    double Snow;                                                                      // 0x0330 (size: 0x8)
    bool Snow - Manual Override;                                                      // 0x0338 (size: 0x1)
    double Thunder/Lightning;                                                         // 0x0340 (size: 0x8)
    bool Thunder/Lightning - Manual Override;                                         // 0x0348 (size: 0x1)
    double Wind Intensity;                                                            // 0x0350 (size: 0x8)
    bool Wind Intensity - Manual Override;                                            // 0x0358 (size: 0x1)
    double Fog;                                                                       // 0x0360 (size: 0x8)
    bool Fog - Manual Override;                                                       // 0x0368 (size: 0x1)
    double Dust;                                                                      // 0x0370 (size: 0x8)
    bool Dust - Manual Override;                                                      // 0x0378 (size: 0x1)
    double Material Wetness;                                                          // 0x0380 (size: 0x8)
    bool Material Wetness - Manual Override;                                          // 0x0388 (size: 0x1)
    double Material Snow Coverage;                                                    // 0x0390 (size: 0x8)
    bool Material Snow Coverage - Manual Override;                                    // 0x0398 (size: 0x1)
    double Material Dust Coverage;                                                    // 0x03A0 (size: 0x8)
    bool Material Dust Coverage - Manual Override;                                    // 0x03A8 (size: 0x1)
    double Wind Direction;                                                            // 0x03B0 (size: 0x8)
    double Transition Duration;                                                       // 0x03B8 (size: 0x8)
    double Transition Timer;                                                          // 0x03C0 (size: 0x8)
    TEnumAsByte<UDS_Particle_Collision_Mode::Type> Particle Collision Mode;           // 0x03C8 (size: 0x1)
    double Ceiling Check Height;                                                      // 0x03D0 (size: 0x8)
    double Spawn Direction Forward Bias;                                              // 0x03D8 (size: 0x8)
    double Spawn Box Height;                                                          // 0x03E0 (size: 0x8)
    double Max Spawn Distance;                                                        // 0x03E8 (size: 0x8)
    double Spawn Distance Distribution;                                               // 0x03F0 (size: 0x8)
    double Minimum Particle Distance;                                                 // 0x03F8 (size: 0x8)
    FVector World Spawn Offset;                                                       // 0x0400 (size: 0x18)
    double Camera Forward Spawn Offset;                                               // 0x0418 (size: 0x8)
    bool Use UDS Water Level;                                                         // 0x0420 (size: 0x1)
    TEnumAsByte<ECollisionChannel> Weather Particle Collision Channel;                // 0x0421 (size: 0x1)
    bool Enable Rain Particles;                                                       // 0x0422 (size: 0x1)
    double Rain Particle Spawn Count;                                                 // 0x0428 (size: 0x8)
    double Rain Drops Scale;                                                          // 0x0430 (size: 0x8)
    double Rain Velocity Randomization;                                               // 0x0438 (size: 0x8)
    FLinearColor Rain Particle Color Multiplier;                                      // 0x0440 (size: 0x10)
    double Rain Drops Alpha;                                                          // 0x0450 (size: 0x8)
    double Rain Refraction Intensity;                                                 // 0x0458 (size: 0x8)
    double Rain Ambient Light Intensity;                                              // 0x0460 (size: 0x8)
    double Rain Light Sparkle;                                                        // 0x0468 (size: 0x8)
    float Rain Particles Time Dilation;                                               // 0x0470 (size: 0x4)
    bool Render Raindrop Decals;                                                      // 0x0474 (size: 0x1)
    bool Rain Ripple Normal Decals;                                                   // 0x0475 (size: 0x1)
    double Splash Frequency;                                                          // 0x0478 (size: 0x8)
    double Splash Particles Scale;                                                    // 0x0480 (size: 0x8)
    double Splash Droplets Scale;                                                     // 0x0488 (size: 0x8)
    double Splash Particles Alpha;                                                    // 0x0490 (size: 0x8)
    double Splash Minimum Z Normal;                                                   // 0x0498 (size: 0x8)
    double Raindrop Decal Scale;                                                      // 0x04A0 (size: 0x8)
    TEnumAsByte<UDS_Rain_Splash_RenderMode::Type> Splash Particles Rendering Mode;    // 0x04A8 (size: 0x1)
    double Max Duplicate Splash Range;                                                // 0x04B0 (size: 0x8)
    bool Enable Snow Particles;                                                       // 0x04B8 (size: 0x1)
    double Snow Particle Spawn Count;                                                 // 0x04C0 (size: 0x8)
    double Snow Flakes Scale;                                                         // 0x04C8 (size: 0x8)
    double Snow Velocity Randomization;                                               // 0x04D0 (size: 0x8)
    double Snow Flakes Alpha;                                                         // 0x04D8 (size: 0x8)
    double Snow Ambient Light Intensity;                                              // 0x04E0 (size: 0x8)
    double Snow Flakes Surface Stick Duration;                                        // 0x04E8 (size: 0x8)
    double Snow Particles Time Dilation;                                              // 0x04F0 (size: 0x8)
    FLinearColor Snow Particle Color Multiplier;                                      // 0x04F8 (size: 0x10)
    bool Spawn Lightning Flashes;                                                     // 0x0508 (size: 0x1)
    double Lightning Flash Frequency;                                                 // 0x0510 (size: 0x8)
    double Lightning Flash Timing Randomization;                                      // 0x0518 (size: 0x8)
    FLinearColor Lightning Effect Tint Color;                                         // 0x0520 (size: 0x10)
    bool Lightning Flash Light Source;                                                // 0x0530 (size: 0x1)
    FLinearColor Lightning Flash Light Source Color;                                  // 0x0534 (size: 0x10)
    double Maximum Lightning Flash Light Intensity;                                   // 0x0548 (size: 0x8)
    bool Lightning Flashes Cast Shadows;                                              // 0x0550 (size: 0x1)
    bool Lightning Flashes Cast Light Shaft Bloom;                                    // 0x0551 (size: 0x1)
    double Lightning Flash Light Shaft Intensity;                                     // 0x0558 (size: 0x8)
    FFloatRange Lightning Flash Distance Range;                                       // 0x0560 (size: 0x10)
    FFloatRange Lightning Flash Duration;                                             // 0x0570 (size: 0x10)
    double Lightning Height Offset;                                                   // 0x0580 (size: 0x8)
    double Lightning Bolt Length;                                                     // 0x0588 (size: 0x8)
    double Lightning Bolt Thickness Scale;                                            // 0x0590 (size: 0x8)
    bool Replicate Lightning Direction;                                               // 0x0598 (size: 0x1)
    bool Enable Obscured Lightning;                                                   // 0x0599 (size: 0x1)
    double Obscured Lightning Spawn Rate;                                             // 0x05A0 (size: 0x8)
    bool Enable Fog Particles;                                                        // 0x05A8 (size: 0x1)
    double Max Fog Particle Percentage (Rain);                                        // 0x05B0 (size: 0x8)
    double Fog Particle Intensity (Rain);                                             // 0x05B8 (size: 0x8)
    double Max Fog Particle Percentage (Snow);                                        // 0x05C0 (size: 0x8)
    double Fog Particle Intensity (Snow);                                             // 0x05C8 (size: 0x8)
    double Max Fog Particle Percentage (Dust);                                        // 0x05D0 (size: 0x8)
    double Fog Particle Intensity (Dust);                                             // 0x05D8 (size: 0x8)
    double Fog Particles Draw Distance;                                               // 0x05E0 (size: 0x8)
    class AWindDirectionalSource* Wind Directional Source Actor;                      // 0x05E8 (size: 0x8)
    double Max Material Wetness;                                                      // 0x05F0 (size: 0x8)
    double Max Material Snow Coverage;                                                // 0x05F8 (size: 0x8)
    double Max Dust Coverage;                                                         // 0x0600 (size: 0x8)
    FLinearColor Material Snow Color;                                                 // 0x0608 (size: 0x10)
    FLinearColor Material Dust Color;                                                 // 0x0618 (size: 0x10)
    double Material Water Roughness;                                                  // 0x0628 (size: 0x8)
    double Tiling Raindrop Ripples Scale;                                             // 0x0630 (size: 0x8)
    bool Simulate Changing Material State Over Time;                                  // 0x0638 (size: 0x1)
    double Wetness Coverage Duration;                                                 // 0x0640 (size: 0x8)
    double Wetness Dry Duration;                                                      // 0x0648 (size: 0x8)
    double Wetness Dry Speed in Sunlight;                                             // 0x0650 (size: 0x8)
    double Wetness Dry Speed without Sunlight;                                        // 0x0658 (size: 0x8)
    double Snow Coverage Duration;                                                    // 0x0660 (size: 0x8)
    double Snow Melt Duration;                                                        // 0x0668 (size: 0x8)
    double Snow Melt Speed Above Freezing;                                            // 0x0670 (size: 0x8)
    double Snow Melt Speed Below Freezing;                                            // 0x0678 (size: 0x8)
    double Dust Coverage Duration;                                                    // 0x0680 (size: 0x8)
    double Dust Clear Duration;                                                       // 0x0688 (size: 0x8)
    double Dust Clear Speed when Windy;                                               // 0x0690 (size: 0x8)
    double Dust Clear Speed when Calm;                                                // 0x0698 (size: 0x8)
    double Melted Snow Coverage Contributes to Wetness;                               // 0x06A0 (size: 0x8)
    double Snow Coverage Lights Up Height Fog;                                        // 0x06A8 (size: 0x8)
    double Wind Force Scale;                                                          // 0x06B0 (size: 0x8)
    TEnumAsByte<UDS_RandomWeatherTiming::Type> Random Weather Variation;              // 0x06B8 (size: 0x1)
    FFloatRange Random Weather Change Interval;                                       // 0x06BC (size: 0x10)
    int32 Random Weather Change Hour;                                                 // 0x06CC (size: 0x4)
    double Transition Length;                                                         // 0x06D0 (size: 0x8)
    bool Begin Play Weather Is Random;                                                // 0x06D8 (size: 0x1)
    class UUDS_Climate_Preset_C* Apply Climate Preset;                                // 0x06E0 (size: 0x8)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Spring);       // 0x06E8 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Summer);       // 0x0738 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Autumn);       // 0x0788 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Winter);       // 0x07D8 (size: 0x50)
    int32 Random Weather Forecast Steps;                                              // 0x0828 (size: 0x4)
    bool Blend Season Probabilities;                                                  // 0x082C (size: 0x1)
    bool Avoid Extreme Cloud Coverage Shifts;                                         // 0x082D (size: 0x1)
    bool Avoid Repeating Weather Types;                                               // 0x082E (size: 0x1)
    bool Avoid Changing Directly from Snow to Rain (Or Vice Versa);                   // 0x082F (size: 0x1)
    TMap<UUDS_Weather_Settings_C*, double> Weather Specific Interval Multipliers;     // 0x0830 (size: 0x50)
    double Extreme Cloud Coverage Shift Theshold;                                     // 0x0880 (size: 0x8)
    bool Enable Weather Sound Effects;                                                // 0x0888 (size: 0x1)
    double Rain Volume;                                                               // 0x0890 (size: 0x8)
    double Distant Thunder Volume;                                                    // 0x0898 (size: 0x8)
    double Close Thunder Volume;                                                      // 0x08A0 (size: 0x8)
    double Close Thunder Delay Per KM;                                                // 0x08A8 (size: 0x8)
    double Wind Volume;                                                               // 0x08B0 (size: 0x8)
    double Wind Whistling Volume;                                                     // 0x08B8 (size: 0x8)
    double Fog Particle Camera Offset;                                                // 0x08C0 (size: 0x8)
    double Wind Directional Source Intensity Scale;                                   // 0x08C8 (size: 0x8)
    double Obscured Lightning Scale;                                                  // 0x08D0 (size: 0x8)
    double Lightning Flash Max Angle From Forward;                                    // 0x08D8 (size: 0x8)
    bool Show Lightning Flashes in Level Editor;                                      // 0x08E0 (size: 0x1)
    double Daytime Lightning Flash Intensity;                                         // 0x08E8 (size: 0x8)
    double Nighttime Lightning Flash Intensity;                                       // 0x08F0 (size: 0x8)
    bool Lightning Flash Active;                                                      // 0x08F8 (size: 0x1)
    double Lightning Flash Time Elapsed;                                              // 0x0900 (size: 0x8)
    double Lightning Flash Length;                                                    // 0x0908 (size: 0x8)
    TEnumAsByte<UDS_TemperatureType::Type> Temperature Scale;                         // 0x0910 (size: 0x1)
    FVector2D Summer Temperature Min and Max;                                         // 0x0918 (size: 0x10)
    FVector2D Autumn Temperature Min and Max;                                         // 0x0928 (size: 0x10)
    FVector2D Winter Temperature Min and Max;                                         // 0x0938 (size: 0x10)
    FVector2D Spring Temperature Min and Max;                                         // 0x0948 (size: 0x10)
    double Daytime Temperature Bias;                                                  // 0x0958 (size: 0x8)
    double Nighttime Temperature Bias;                                                // 0x0960 (size: 0x8)
    double Morning/Evening Temperature Bias;                                          // 0x0968 (size: 0x8)
    double Overcast Temperature Bias;                                                 // 0x0970 (size: 0x8)
    double Raining Temperature Bias;                                                  // 0x0978 (size: 0x8)
    double Snowing Temperature Bias;                                                  // 0x0980 (size: 0x8)
    double Foggy Temperature Bias;                                                    // 0x0988 (size: 0x8)
    double Dusty Temperature Bias;                                                    // 0x0990 (size: 0x8)
    double Randomize Temperature;                                                     // 0x0998 (size: 0x8)
    double Interior Temperature;                                                      // 0x09A0 (size: 0x8)
    double Interior Insulation;                                                       // 0x09A8 (size: 0x8)
    TEnumAsByte<UDS_ControlPointMode::Type> Control Point Location Source;            // 0x09B0 (size: 0x1)
    FVector Custom Control Point Location;                                            // 0x09B8 (size: 0x18)
    double Weather Speed;                                                             // 0x09D0 (size: 0x8)
    int32 Random Seed;                                                                // 0x09D8 (size: 0x4)
    double Snow Depth;                                                                // 0x09E0 (size: 0x8)
    double Dust Depth;                                                                // 0x09E8 (size: 0x8)
    double Snow/Dust Z Normal Cutoff;                                                 // 0x09F0 (size: 0x8)
    double Snow/Dust Z Normal Falloff;                                                // 0x09F8 (size: 0x8)
    double Snow/Dust Texture Scale;                                                   // 0x0A00 (size: 0x8)
    double Snow/Dust Interaction Render Target Area;                                  // 0x0A08 (size: 0x8)
    double Snow/Dust Interaction Fade Speed Idle;                                     // 0x0A10 (size: 0x8)
    double Snow/Dust Interaction Fade Speed Active;                                   // 0x0A18 (size: 0x8)
    FVector Player Camera Location;                                                   // 0x0A20 (size: 0x18)
    FVector Control Point Location;                                                   // 0x0A38 (size: 0x18)
    double Puddle Coverage;                                                           // 0x0A50 (size: 0x8)
    double Puddles Z Normal Cutoff;                                                   // 0x0A58 (size: 0x8)
    double Puddles Z Normal Falloff;                                                  // 0x0A60 (size: 0x8)
    double Base Wetness when Raining;                                                 // 0x0A68 (size: 0x8)
    double Base Wetness when Clear;                                                   // 0x0A70 (size: 0x8)
    double Puddle Sharpness;                                                          // 0x0A78 (size: 0x8)
    double Variation Clouds Scale;                                                    // 0x0A80 (size: 0x8)
    double Snow/Dust Interaction Edge Piling;                                         // 0x0A88 (size: 0x8)
    double Snow/Dust Interaction Depth;                                               // 0x0A90 (size: 0x8)
    double Snow/Dust Interaction Parallax Sample Scale;                               // 0x0A98 (size: 0x8)
    double Snow/Dust Interaction Scale;                                               // 0x0AA0 (size: 0x8)
    double Snow/Dust Interaction Scatter;                                             // 0x0AA8 (size: 0x8)
    double Snow/Dust Interaction Size Scatter;                                        // 0x0AB0 (size: 0x8)
    double Snow/Dust Interactions Update Period;                                      // 0x0AB8 (size: 0x8)
    TArray<class UPhysicalMaterial*> Physical Materials which disable Snow/Dust Sounds and Particles; // 0x0AC0 (size: 0x10)
    TArray<class UPhysicalMaterial*> Physical Materials which disable Puddle Sounds and Particles; // 0x0AD0 (size: 0x10)
    bool Enable WOV Material Effect Target;                                           // 0x0AE0 (size: 0x1)
    double WOV Target Size;                                                           // 0x0AE8 (size: 0x8)
    double WOV Target Update Threshold (Inside Volume);                               // 0x0AF0 (size: 0x8)
    double WOV Target Update Threshold (Outside Volume);                              // 0x0AF8 (size: 0x8)
    TArray<class AWeather_Override_Volume_C*> Weather Override Volumes;               // 0x0B00 (size: 0x10)
    bool Use Custom Lightning Location;                                               // 0x0B10 (size: 0x1)
    FVector Custom Lightning Location;                                                // 0x0B18 (size: 0x18)
    FVector Custom Lightning Target;                                                  // 0x0B30 (size: 0x18)
    double Current Lightning Intensity;                                               // 0x0B48 (size: 0x8)
    double Current Lightning Intensity Scale;                                         // 0x0B50 (size: 0x8)
    FVector Current Lightning Location;                                               // 0x0B58 (size: 0x18)
    double Current Lightning Angle;                                                   // 0x0B70 (size: 0x8)
    class AWeather_Override_Volume_C* Current Weather Override Volume;                // 0x0B78 (size: 0x8)
    TMap<AWeather_Override_Volume_C*, double> Current WOVs Applied;                   // 0x0B80 (size: 0x50)
    bool Currently in a Weather Override Volume;                                      // 0x0BD0 (size: 0x1)
    class UTextureRenderTarget2D* DLWE Mask Target;                                   // 0x0BD8 (size: 0x8)
    FVector DLWE Recenter Offset;                                                     // 0x0BE0 (size: 0x18)
    TArray<FVector2D> Cloud Reference Array;                                          // 0x0BF8 (size: 0x10)
    TArray<FVector4> DLWE Interactions Buffer;                                        // 0x0C08 (size: 0x10)
    bool Use Occlusion To Attenuate Sounds In Interiors;                              // 0x0C18 (size: 0x1)
    double Max Attenuation;                                                           // 0x0C20 (size: 0x8)
    double Min LPF Frequency;                                                         // 0x0C28 (size: 0x8)
    class UUDS_OcclusionSettings_C* Occlusion Settings;                               // 0x0C30 (size: 0x8)
    bool Enable Post Process Wind Fog;                                                // 0x0C38 (size: 0x1)
    double PPWF Intensity From Fog;                                                   // 0x0C40 (size: 0x8)
    double PPWF Intensity from Rain;                                                  // 0x0C48 (size: 0x8)
    double PPWF Intensity From Snow;                                                  // 0x0C50 (size: 0x8)
    double PPWF Intensity from Dust;                                                  // 0x0C58 (size: 0x8)
    double PPWF Intensity Scale;                                                      // 0x0C60 (size: 0x8)
    double PPWF Fade Distance;                                                        // 0x0C68 (size: 0x8)
    double PPWF Max Sample Distance;                                                  // 0x0C70 (size: 0x8)
    double PPWF Sample Step Distance;                                                 // 0x0C78 (size: 0x8)
    double PPWF 3D Noise Scale;                                                       // 0x0C80 (size: 0x8)
    class UMaterialInstanceDynamic* Post Process Wind Fog MID;                        // 0x0C88 (size: 0x8)
    double Temperature Update Period;                                                 // 0x0C90 (size: 0x8)
    double Time Random Offset;                                                        // 0x0C98 (size: 0x8)
    bool Enable Wind Direction Variation;                                             // 0x0CA0 (size: 0x1)
    double Maximum Wind Direction Variation;                                          // 0x0CA8 (size: 0x8)
    double Variation Speed;                                                           // 0x0CB0 (size: 0x8)
    double Current Wind Direction;                                                    // 0x0CB8 (size: 0x8)
    TMap<FName, TEnumAsByte<UDS_PropertyType::Type>> Properties;                      // 0x0CC0 (size: 0x50)
    FVector Occlusion Location;                                                       // 0x0D10 (size: 0x18)
    FUltra_Dynamic_Weather_CStarted Raining Started Raining;                          // 0x0D28 (size: 0x10)
    void Started Raining();
    FUltra_Dynamic_Weather_CStarted Snowing Started Snowing;                          // 0x0D38 (size: 0x10)
    void Started Snowing();
    FUltra_Dynamic_Weather_CFinished Raining Finished Raining;                        // 0x0D48 (size: 0x10)
    void Finished Raining();
    FUltra_Dynamic_Weather_CFinished Snowing Finished Snowing;                        // 0x0D58 (size: 0x10)
    void Finished Snowing();
    FUltra_Dynamic_Weather_CGetting Cloudy Getting Cloudy;                            // 0x0D68 (size: 0x10)
    void Getting Cloudy();
    FUltra_Dynamic_Weather_CClouds Clearing Clouds Clearing;                          // 0x0D78 (size: 0x10)
    void Clouds Clearing();
    bool ED_Raining;                                                                  // 0x0D88 (size: 0x1)
    bool ED_Snowy;                                                                    // 0x0D89 (size: 0x1)
    bool ED_Cloudy;                                                                   // 0x0D8A (size: 0x1)
    bool Enable Screen Droplets;                                                      // 0x0D8B (size: 0x1)
    double Screen Center Strength;                                                    // 0x0D90 (size: 0x8)
    double Screen Edge Strength;                                                      // 0x0D98 (size: 0x8)
    double Camera Facing Rain Bias;                                                   // 0x0DA0 (size: 0x8)
    double Droplet Tiling;                                                            // 0x0DA8 (size: 0x8)
    double Blur Radius;                                                               // 0x0DB0 (size: 0x8)
    bool Wet Screen when Emerging From Water;                                         // 0x0DB8 (size: 0x1)
    double Screen Droplets Clear Speed;                                               // 0x0DC0 (size: 0x8)
    bool Show Screen Droplets in Editor;                                              // 0x0DC8 (size: 0x1)
    class UMaterialInstanceDynamic* Screen Droplets MID;                              // 0x0DD0 (size: 0x8)
    double Drips Intensity;                                                           // 0x0DD8 (size: 0x8)
    double Drops Intensity;                                                           // 0x0DE0 (size: 0x8)
    double Drops Time;                                                                // 0x0DE8 (size: 0x8)
    TSoftObjectPtr<UMaterialInterface> Screen Droplets Parent Material;               // 0x0DF0 (size: 0x28)
    bool Obscured Lightning System Spawning;                                          // 0x0E18 (size: 0x1)
    bool Enable Dust Particles;                                                       // 0x0E19 (size: 0x1)
    double Dust Particle Spawn Count;                                                 // 0x0E20 (size: 0x8)
    double Dust Particle Scale;                                                       // 0x0E28 (size: 0x8)
    double Dust Velocity Randomization;                                               // 0x0E30 (size: 0x8)
    FLinearColor Dust Particle Color;                                                 // 0x0E38 (size: 0x10)
    double Dust Particle Alpha;                                                       // 0x0E48 (size: 0x8)
    bool Enable Wind Debris;                                                          // 0x0E50 (size: 0x1)
    double Wind Debris Particle Spawn Count;                                          // 0x0E58 (size: 0x8)
    double Debris Scale;                                                              // 0x0E60 (size: 0x8)
    double Debris Particle Alpha;                                                     // 0x0E68 (size: 0x8)
    TSoftObjectPtr<UTexture2D> Wind Debris Atlas;                                     // 0x0E70 (size: 0x28)
    class UTextureRenderTarget2D* Weather Mask Target;                                // 0x0E98 (size: 0x8)
    int32 Weather Mask Target Size;                                                   // 0x0EA0 (size: 0x4)
    bool Triggered Starting Dispatchers;                                              // 0x0EA4 (size: 0x1)
    class UUDS_Weather_Settings_C* Old Weather State;                                 // 0x0EA8 (size: 0x8)
    class UUDS_Weather_Settings_C* Global Weather State;                              // 0x0EB0 (size: 0x8)
    class UUDS_Weather_Settings_C* Local Weather State;                               // 0x0EB8 (size: 0x8)
    class UUDS_Weather_Settings_C* Update Buffer;                                     // 0x0EC0 (size: 0x8)
    float Dust Particles Time Dilation;                                               // 0x0EC8 (size: 0x4)
    TEnumAsByte<UDS_SeasonMode::Type> Season Mode;                                    // 0x0ECC (size: 0x1)
    double Season;                                                                    // 0x0ED0 (size: 0x8)
    TArray<double> Individual Seasons;                                                // 0x0ED8 (size: 0x10)
    class UUDS_Weather_Settings_C* Manual Weather State;                              // 0x0EE8 (size: 0x8)
    TSoftObjectPtr<UTexture2D> Clouds Diverse Texture;                                // 0x0EF0 (size: 0x28)
    TSoftObjectPtr<UNiagaraSystem> Rain Particle Niagara System;                      // 0x0F18 (size: 0x28)
    TSoftObjectPtr<UNiagaraSystem> Snow Particle Niagara System;                      // 0x0F40 (size: 0x28)
    TSoftObjectPtr<UNiagaraSystem> Dust Particle Niagara System;                      // 0x0F68 (size: 0x28)
    TSoftObjectPtr<UNiagaraSystem> Wind Debris Niagara System;                        // 0x0F90 (size: 0x28)
    TSoftObjectPtr<UNiagaraSystem> Obscured Lightning Niagara System;                 // 0x0FB8 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Post Process Wind Fog Parent Material;         // 0x0FE0 (size: 0x28)
    FUltra_Dynamic_Weather_CDust/Sand Forming Dust/Sand Forming;                      // 0x1008 (size: 0x10)
    void Dust/Sand Forming();
    FUltra_Dynamic_Weather_CDust/Sand Clearing Dust/Sand Clearing;                    // 0x1018 (size: 0x10)
    void Dust/Sand Clearing();
    bool ED_Dusty;                                                                    // 0x1028 (size: 0x1)
    bool Enable Rainbow;                                                              // 0x1029 (size: 0x1)
    double Max Rainbow Strength;                                                      // 0x1030 (size: 0x8)
    double Strength From Rain;                                                        // 0x1038 (size: 0x8)
    double Strength From Fog;                                                         // 0x1040 (size: 0x8)
    double Strength In Any Weather;                                                   // 0x1048 (size: 0x8)
    double Time to Last After Rain/Fog;                                               // 0x1050 (size: 0x8)
    double Mask Rainbow Above Clouds;                                                 // 0x1058 (size: 0x8)
    double Mask Rainbow Below Water;                                                  // 0x1060 (size: 0x8)
    TEnumAsByte<UDS_Weather_Display_Names::Type> ED_CurrentWeather;                   // 0x1068 (size: 0x1)
    FUltra_Dynamic_Weather_CWeather Display Name Changed Weather Display Name Changed; // 0x1070 (size: 0x10)
    void Weather Display Name Changed(TEnumAsByte<UDS_Weather_Display_Names::Type> Weather Name);
    FTimerHandle DLWE Interaction Timer;                                              // 0x1080 (size: 0x8)
    bool Support Virtual Heightfield Mesh;                                            // 0x1088 (size: 0x1)
    class ARuntimeVirtualTextureVolume* VHFM Runtime Virtual Texture Volume;          // 0x1090 (size: 0x8)
    double VHFM Invalidate Long Range Distance;                                       // 0x1098 (size: 0x8)
    double VHFM Invalidate Long Range Threshold;                                      // 0x10A0 (size: 0x8)
    double VHFM Invalidate Mid Range Distance;                                        // 0x10A8 (size: 0x8)
    double VHFM Invalidate Mid Range Threshold;                                       // 0x10B0 (size: 0x8)
    double VHFM Invalidate Short Range Distance;                                      // 0x10B8 (size: 0x8)
    double VHFM Invalidate Short Range Threshold;                                     // 0x10C0 (size: 0x8)
    FVector VHFM Invalidate Long Range Buffer;                                        // 0x10C8 (size: 0x18)
    FVector VHFM Invalidate Mid Range Buffer;                                         // 0x10E0 (size: 0x18)
    FVector VHFM Invalidate Short Range Buffer;                                       // 0x10F8 (size: 0x18)
    bool Fog Particles Active;                                                        // 0x1110 (size: 0x1)
    FVector Last Sparse Movement Update Location;                                     // 0x1118 (size: 0x18)
    class USoundMix* Outdoor Sound Mix Modifier;                                      // 0x1130 (size: 0x8)
    double Max Attenuation For UDS Outdoor Sound;                                     // 0x1138 (size: 0x8)
    TSoftObjectPtr<UTextureRenderTarget2D> Weather Volume Target;                     // 0x1140 (size: 0x28)
    class UMaterialInstanceDynamic* WOV Target Brush MID;                             // 0x1168 (size: 0x8)
    bool WOV Material Effect Target Active;                                           // 0x1170 (size: 0x1)
    bool Shutting Down;                                                               // 0x1171 (size: 0x1)
    FString UDS Version;                                                              // 0x1178 (size: 0x10)
    class UUDS_VersionInfo_C* UDS Version Info;                                       // 0x1188 (size: 0x8)
    int32 Anti Aliasing Method;                                                       // 0x1190 (size: 0x4)
    int32 Season Day Offset;                                                          // 0x1194 (size: 0x4)
    FVector2D WOV Wind Direction;                                                     // 0x1198 (size: 0x10)
    FVector Current Lightning Target Offset;                                          // 0x11A8 (size: 0x18)
    TSoftObjectPtr<UMaterial> Splash Material Decal;                                  // 0x11C0 (size: 0x28)
    TSoftObjectPtr<UMaterial> Splash Material Translucent;                            // 0x11E8 (size: 0x28)
    class UMaterialInstanceDynamic* Rainbow MID;                                      // 0x1210 (size: 0x8)
    double Current Rainbow Strength;                                                  // 0x1218 (size: 0x8)
    double Target Rainbow Strength;                                                   // 0x1220 (size: 0x8)
    TSoftObjectPtr<UMaterialInterface> Rainbow Material 2D;                           // 0x1228 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Rainbow Material Volumetric;                   // 0x1250 (size: 0x28)
    double Weather Particle Motion Stretch;                                           // 0x1278 (size: 0x8)
    double Particle Sprite Motion Blur With Camera Movement;                          // 0x1280 (size: 0x8)
    uint8 Previous Temperature Scale;                                                 // 0x1288 (size: 0x1)
    bool Apply Changes Above Volumetric Cloud Layer;                                  // 0x1289 (size: 0x1)
    double Rain Multiplier Above Clouds;                                              // 0x1290 (size: 0x8)
    double Snow Multiplier Above Clouds;                                              // 0x1298 (size: 0x8)
    double Dust Multiplier Above Clouds;                                              // 0x12A0 (size: 0x8)
    double Fog Multiplier Above Clouds;                                               // 0x12A8 (size: 0x8)
    double Thunder/Lightning Multiplier Above Clouds;                                 // 0x12B0 (size: 0x8)
    double Wind Multiplier Above Clouds;                                              // 0x12B8 (size: 0x8)
    FUltra_Dynamic_Weather_CSeason Changed Season Changed;                            // 0x12C0 (size: 0x10)
    void Season Changed(TEnumAsByte<UDS_Season::Type> Season);
    int32 ED_Season;                                                                  // 0x12D0 (size: 0x4)
    bool Transition Active;                                                           // 0x12D4 (size: 0x1)
    double Transition Alpha;                                                          // 0x12D8 (size: 0x8)
    bool Rain Update Needed;                                                          // 0x12E0 (size: 0x1)
    bool Snow Update Needed;                                                          // 0x12E1 (size: 0x1)
    bool Thunder/Lightning Update Needed;                                             // 0x12E2 (size: 0x1)
    bool Wind Intensity Update Needed;                                                // 0x12E3 (size: 0x1)
    bool Cloud Coverage Update Needed;                                                // 0x12E4 (size: 0x1)
    bool Fog Update Needed;                                                           // 0x12E5 (size: 0x1)
    bool Dust Update Needed;                                                          // 0x12E6 (size: 0x1)
    bool Material Wetness Update Needed;                                              // 0x12E7 (size: 0x1)
    bool Material Snow Update Needed;                                                 // 0x12E8 (size: 0x1)
    bool Material Dust Update Needed;                                                 // 0x12E9 (size: 0x1)
    bool Wind Direction Update Needed;                                                // 0x12EA (size: 0x1)
    double Wind Direction Update Buffer;                                              // 0x12F0 (size: 0x8)
    double Wind Direction Last Frame Buffer;                                          // 0x12F8 (size: 0x8)
    TArray<class AWeather_Override_Volume_C*> Nearby Weather Override Volumes;        // 0x1300 (size: 0x10)
    double Current PPWF Intensity;                                                    // 0x1310 (size: 0x8)
    double Target PPWF Intensity;                                                     // 0x1318 (size: 0x8)
    bool Screen Droplets Active;                                                      // 0x1320 (size: 0x1)
    double Screen Droplets Target Drips Intensity;                                    // 0x1328 (size: 0x8)
    FWeightedBlendable Screen Droplets WB;                                            // 0x1330 (size: 0x10)
    double Screen Droplets Camera Exposure;                                           // 0x1340 (size: 0x8)
    FVector Applied Rain Velocity;                                                    // 0x1348 (size: 0x18)
    double Water Level Material Falloff;                                              // 0x1360 (size: 0x8)
    bool Player Camera Underwater;                                                    // 0x1368 (size: 0x1)
    double Shared Material Parameter Snowy;                                           // 0x1370 (size: 0x8)
    double Shared Material Parameter Dusty;                                           // 0x1378 (size: 0x8)
    TSoftObjectPtr<UMaterial> Splash Droplet Material Decal;                          // 0x1380 (size: 0x28)
    TSoftObjectPtr<UMaterial> Splash Droplet Material Translucent;                    // 0x13A8 (size: 0x28)
    TArray<class UWeather_Mask_Projection_Box_Component_C*> Last Update Projection Box Components; // 0x13D0 (size: 0x10)
    double Periodic Updates Timer;                                                    // 0x13E0 (size: 0x8)
    int32 Periodic Update Step;                                                       // 0x13E8 (size: 0x4)
    TArray<class UPrimitiveComponent*> Last Update Mask Components;                   // 0x13F0 (size: 0x10)
    FRandomStream Random Stream;                                                      // 0x1400 (size: 0x8)
    double Weather Particle DOF Scale;                                                // 0x1408 (size: 0x8)
    double CPU Particle Buffer Length;                                                // 0x1410 (size: 0x8)
    double Scale Distant Particle Alpha;                                              // 0x1418 (size: 0x8)
    double Distant Particle Range;                                                    // 0x1420 (size: 0x8)
    bool Warm Up Weather Particles On Begin Play;                                     // 0x1428 (size: 0x1)
    bool Apply Custom Weather Particle Camera Transform;                              // 0x1429 (size: 0x1)
    FVector Custom Weather Particle Camera Location;                                  // 0x1430 (size: 0x18)
    FVector Custom Weather Particle Camera Forward Vector;                            // 0x1448 (size: 0x18)
    bool Global Weather Updated;                                                      // 0x1460 (size: 0x1)
    bool Local Weather Updated;                                                       // 0x1461 (size: 0x1)
    TMap<UUDS_Weather_Settings_C*, double> Old State Source Map;                      // 0x1468 (size: 0x50)
    bool Call "Custom Weather Behavior" Functions;                                    // 0x14B8 (size: 0x1)
    TArray<class UUDS_Weather_Settings_C*> Last Update Sources;                       // 0x14C0 (size: 0x10)
    bool Enable Heat Distortion;                                                      // 0x14D0 (size: 0x1)
    double Heat Distortion Max Intensity;                                             // 0x14D8 (size: 0x8)
    double Heat Distortion From Temperature;                                          // 0x14E0 (size: 0x8)
    FFloatRange Heat Distortion Temperature Range;                                    // 0x14E8 (size: 0x10)
    double Manual Heat Distortion;                                                    // 0x14F8 (size: 0x8)
    double Heat DIstortion Start Distance;                                            // 0x1500 (size: 0x8)
    double Heat DIstortion Falloff;                                                   // 0x1508 (size: 0x8)
    double Heat Distortion Tiling;                                                    // 0x1510 (size: 0x8)
    double Heat Distortion Speed;                                                     // 0x1518 (size: 0x8)
    double Heat Distortion Chromatic Separation;                                      // 0x1520 (size: 0x8)
    double Heat Distortion Zenith Mask;                                               // 0x1528 (size: 0x8)
    double Heat Distortion Horizon Mask;                                              // 0x1530 (size: 0x8)
    double Heat Distortion Horizon Exponent;                                          // 0x1538 (size: 0x8)
    double Heat Distortion Temporal Dither;                                           // 0x1540 (size: 0x8)
    bool Show Heat Distortion in Editor;                                              // 0x1548 (size: 0x1)
    double Target Heat Distortion Value;                                              // 0x1550 (size: 0x8)
    double Current Heat Distortion Value;                                             // 0x1558 (size: 0x8)
    TSoftObjectPtr<UMaterialInterface> Heat Distortion Parent Material;               // 0x1560 (size: 0x28)
    class UMaterialInstanceDynamic* Heat Distortion MID;                              // 0x1588 (size: 0x8)
    FUltra_Dynamic_Weather_CState Change - Rain State Change - Rain;                  // 0x1590 (size: 0x10)
    void State Change - Rain();
    FUltra_Dynamic_Weather_CState Change - Snow State Change - Snow;                  // 0x15A0 (size: 0x10)
    void State Change - Snow();
    FUltra_Dynamic_Weather_CState Change - Wind Intensity State Change - Wind Intensity; // 0x15B0 (size: 0x10)
    void State Change - Wind Intensity();
    FUltra_Dynamic_Weather_CState Change - Dust State Change - Dust;                  // 0x15C0 (size: 0x10)
    void State Change - Dust();
    FUltra_Dynamic_Weather_CState Change - Fog State Change - Fog;                    // 0x15D0 (size: 0x10)
    void State Change - Fog();
    FUltra_Dynamic_Weather_CState Change - Thunder/Lightning State Change - Thunder/Lightning; // 0x15E0 (size: 0x10)
    void State Change - Thunder/Lightning();
    FUltra_Dynamic_Weather_CState Change - Wind Direction State Change - Wind Direction; // 0x15F0 (size: 0x10)
    void State Change - Wind Direction();
    FUltra_Dynamic_Weather_CState Change - Cloud Coverage State Change - Cloud Coverage; // 0x1600 (size: 0x10)
    void State Change - Cloud Coverage();
    FUltra_Dynamic_Weather_CState Change - Material Wetness State Change - Material Wetness; // 0x1610 (size: 0x10)
    void State Change - Material Wetness();
    FUltra_Dynamic_Weather_CState Change - Material Snow State Change - Material Snow; // 0x1620 (size: 0x10)
    void State Change - Material Snow();
    FUltra_Dynamic_Weather_CState Change - Material Dust State Change - Material Dust; // 0x1630 (size: 0x10)
    void State Change - Material Dust();
    bool Tick Behavior Active;                                                        // 0x1640 (size: 0x1)
    FVector Editor Camera Position;                                                   // 0x1648 (size: 0x18)
    TEnumAsByte<UDS_RunContext::Type> Run Context;                                    // 0x1660 (size: 0x1)
    bool Sharing Occlusion With UDS;                                                  // 0x1661 (size: 0x1)
    class UUDS_OcclusionState_C* Weather Occlusion State;                             // 0x1668 (size: 0x8)
    double Time of Last Sound Effects Cache;                                          // 0x1670 (size: 0x8)
    double Last Editor Tick Periodic Update;                                          // 0x1678 (size: 0x8)
    double Last Editor Tick Sound Occlusion Update;                                   // 0x1680 (size: 0x8)
    double Last Editor Tick Time;                                                     // 0x1688 (size: 0x8)
    class UUDS_PlayerOcclusion_C* Active Occlusion Component;                         // 0x1690 (size: 0x8)
    class UUDS_OcclusionState_C* Sky Occlusion State;                                 // 0x1698 (size: 0x8)
    float Tick Delta Seconds;                                                         // 0x16A0 (size: 0x4)
    FRotator Player Camera Rotation;                                                  // 0x16A8 (size: 0x18)
    TSoftClassPtr<UObject> Editor Utility Opener Class;                               // 0x16C0 (size: 0x28)
    double Raining Dispatcher Threshold;                                              // 0x16E8 (size: 0x8)
    double Snowing Dispatcher Threshold;                                              // 0x16F0 (size: 0x8)
    double Dust/Sand Dispatcher Threshold;                                            // 0x16F8 (size: 0x8)
    double Cloudy Dispatcher Threshold;                                               // 0x1700 (size: 0x8)
    class UUDS_RenderTarget_State_C* WOV Render Target State;                         // 0x1708 (size: 0x8)
    class UUDS_RenderTarget_State_C* Weather Mask Render Target State;                // 0x1710 (size: 0x8)
    class UUDS_RenderTarget_State_C* DLWE Render Target State;                        // 0x1718 (size: 0x8)
    double Level Editor Lightning Timer;                                              // 0x1720 (size: 0x8)
    double Level Editor Lightning Period;                                             // 0x1728 (size: 0x8)
    FRotator Editor Camera Rotation;                                                  // 0x1730 (size: 0x18)
    double Level Editor Lightning Interrupt Timer;                                    // 0x1748 (size: 0x8)
    double Sound State Update Period;                                                 // 0x1750 (size: 0x8)
    TSoftObjectPtr<USoundBase> Directional Sound Asset;                               // 0x1758 (size: 0x28)
    TSoftObjectPtr<USoundBase> Global Sound Asset;                                    // 0x1780 (size: 0x28)
    int32 Custom Lightning Seed;                                                      // 0x17A8 (size: 0x4)
    FUltra_Dynamic_Weather_CTemperature Range Update Temperature Range Update;        // 0x17B0 (size: 0x10)
    void Temperature Range Update();
    double Last Season Value;                                                         // 0x17C0 (size: 0x8)
    double Season Change Weather Refresh Threshold;                                   // 0x17C8 (size: 0x8)
    FUltra_Dynamic_Weather_CRandom Weather Season Refresh Random Weather Season Refresh; // 0x17D0 (size: 0x10)
    void Random Weather Season Refresh();
    FUDW_WeatherState_Structure Old Weather State Struct;                             // 0x17E0 (size: 0x50)
    bool DLWE Snow Trails Activated;                                                  // 0x1830 (size: 0x1)
    bool Dynamic Puddles Active;                                                      // 0x1831 (size: 0x1)
    int32 Dynamic Snow/Dust;                                                          // 0x1834 (size: 0x4)
    class UUDS_Weather_Settings_C* Transition Weather State B;                        // 0x1838 (size: 0x8)
    FVector Local Weather Location;                                                   // 0x1840 (size: 0x18)
    TArray<class AWeather_Override_Volume_C*> Last Applied WOVs;                      // 0x1858 (size: 0x10)
    bool Weather Mask Target Active;                                                  // 0x1868 (size: 0x1)
    bool Mobile;                                                                      // 0x1869 (size: 0x1)
    bool Last Update Manual Weather Overridden;                                       // 0x186A (size: 0x1)
    bool Lightning System Toggle;                                                     // 0x186B (size: 0x1)
    bool Clear Weather Mask Prep;                                                     // 0x186C (size: 0x1)
    FUltra_Dynamic_Weather_CLightning Flash Started Lightning Flash Started;          // 0x1870 (size: 0x10)
    void Lightning Flash Started();
    TArray<class AActor*> Radial Storm Actors;                                        // 0x1880 (size: 0x10)
    bool Applied Radial Storms Last Update;                                           // 0x1890 (size: 0x1)
    bool Apply Version Specific Correction;                                           // 0x1891 (size: 0x1)
    bool WOV Material Effect Target Allowed;                                          // 0x1892 (size: 0x1)
    class UMaterialInstanceDynamic* Radial Storm Target Draw MID;                     // 0x1898 (size: 0x8)
    class UStaticMeshComponent* Rainbow Mesh;                                         // 0x18A0 (size: 0x8)
    class UNiagaraComponent* Wind Debris Particles;                                   // 0x18A8 (size: 0x8)
    FWeightedBlendable Post Process Wind Fog WB;                                      // 0x18B0 (size: 0x10)
    FWeightedBlendable Heat Distortion WB;                                            // 0x18C0 (size: 0x10)
    TArray<bool> Post Process Mats Toggle State;                                      // 0x18D0 (size: 0x10)
    TSoftObjectPtr<UMaterialInterface> Rain Particle Mat (AMB);                       // 0x18E0 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Rain Particle Mat (ADOF);                      // 0x1908 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Snow Particle Mat (AMB);                       // 0x1930 (size: 0x28)
    TSoftObjectPtr<UMaterialInterface> Snow Particle Mat (ADOF);                      // 0x1958 (size: 0x28)
    bool Motion Blur;                                                                 // 0x1980 (size: 0x1)
    double Wind Direction Target;                                                     // 0x1988 (size: 0x8)
    double Wind Direction Change Speed;                                               // 0x1990 (size: 0x8)
    double UDS Cloud Coverage;                                                        // 0x1998 (size: 0x8)
    double UDS Fog;                                                                   // 0x19A0 (size: 0x8)
    double UDS Dust Amount;                                                           // 0x19A8 (size: 0x8)
    double UDS Cloud Direction;                                                       // 0x19B0 (size: 0x8)
    double UDS Cloud Speed;                                                           // 0x19B8 (size: 0x8)
    double UDS Fog Vertical Velocity;                                                 // 0x19C0 (size: 0x8)
    TArray<class AActor*> Nearby Radial Storm Actors;                                 // 0x19C8 (size: 0x10)
    bool Enable Screen Frost;                                                         // 0x19D8 (size: 0x1)
    double Screen Frost From Snow;                                                    // 0x19E0 (size: 0x8)
    double Screen Frost From Material Snow;                                           // 0x19E8 (size: 0x8)
    double Manual Screen Frost;                                                       // 0x19F0 (size: 0x8)
    double Screen Frost Multiplier in Interior;                                       // 0x19F8 (size: 0x8)
    double Screen Frost Form Duration;                                                // 0x1A00 (size: 0x8)
    double Screen Frost Clear Duration;                                               // 0x1A08 (size: 0x8)
    FLinearColor Screen Frost Color;                                                  // 0x1A10 (size: 0x10)
    double Screen Frost Falloff Shape;                                                // 0x1A20 (size: 0x8)
    double Screen Frost Falloff Exponent;                                             // 0x1A28 (size: 0x8)
    double Screen Frost Center Mask;                                                  // 0x1A30 (size: 0x8)
    double Screen Frost Edge Mask;                                                    // 0x1A38 (size: 0x8)
    double Screen Frost Distortion Strength;                                          // 0x1A40 (size: 0x8)
    double Screen Frost Distortion Alpha;                                             // 0x1A48 (size: 0x8)
    double Screen Frost Texture Scale;                                                // 0x1A50 (size: 0x8)
    double Screen Frost Scatter;                                                      // 0x1A58 (size: 0x8)
    double Screen Frost Sharpness;                                                    // 0x1A60 (size: 0x8)
    bool Randomize Screen Frost Texture at Runtime;                                   // 0x1A68 (size: 0x1)
    TSoftObjectPtr<UTexture2D> Screen Frost Scatter Texture;                          // 0x1A70 (size: 0x28)
    TSoftObjectPtr<UTexture2D> Screen Frost Normal Texture;                           // 0x1A98 (size: 0x28)
    bool Show Screen Frost in Editor;                                                 // 0x1AC0 (size: 0x1)
    bool Screen Frost Active;                                                         // 0x1AC1 (size: 0x1)
    double Screen Frost Strength Target;                                              // 0x1AC8 (size: 0x8)
    double Current Screen Frost Strength;                                             // 0x1AD0 (size: 0x8)
    class UMaterialInstanceDynamic* Screen Frost MID;                                 // 0x1AD8 (size: 0x8)
    FWeightedBlendable Screen Frost WB;                                               // 0x1AE0 (size: 0x10)
    TSoftObjectPtr<UMaterialInterface> Screen Frost Parent Material;                  // 0x1AF0 (size: 0x28)
    bool Manual Overrides Applied;                                                    // 0x1B18 (size: 0x1)
    TArray<class UObject*> Loaded Objects;                                            // 0x1B20 (size: 0x10)
    class UNiagaraComponent* Rain Particles;                                          // 0x1B30 (size: 0x8)
    class UNiagaraComponent* Snow Particles;                                          // 0x1B38 (size: 0x8)
    class UNiagaraComponent* Dust Particles;                                          // 0x1B40 (size: 0x8)
    class UNiagaraComponent* Obscured Lightning Particles;                            // 0x1B48 (size: 0x8)
    class UNiagaraComponent* Lightning Flash 1;                                       // 0x1B50 (size: 0x8)
    class UNiagaraComponent* Lightning Flash 2;                                       // 0x1B58 (size: 0x8)
    class USceneCaptureComponent2D* Projection Box Scene Capture Component;           // 0x1B60 (size: 0x8)
    class UUDS_Weather_Settings_C* Transient Weather State;                           // 0x1B68 (size: 0x8)
    class UUDS_Weather_Settings_C* Temperature Weather State;                         // 0x1B70 (size: 0x8)
    class UMaterialInstanceDynamic* DLWE Trail Brush MID;                             // 0x1B78 (size: 0x8)

    void UDS Reconstruct(bool& Success);
    void Report Removed Radial Storm(class AActor* Storm, bool& Success);
    void Get UDS Weather Override Bool(bool& Cloud Coverage, bool& Fog, bool& Dust);
    void UDS Weather Variable Overrides(bool Override Clouds, double Cloud Coverage, bool Override Fog, double Fog, bool Override Dust, double Dust, bool& Success);
    void UDW State Apply(FUDS_and_UDW_State State, bool& Completed);
    void Get UDS Values Controlled by UDW(double& Cloud Coverage, double& Fog, double& Dust Amount, double& Cloud Direction, double& Wind Speed Multiplier, double& Fog Vertical Velocity);
    void Editor Tick(FVector Editor Camera Location, FRotator Editor Camera Rotation, double Delta Time, bool& Completed);
    void Get Local Weather State Values(double& Cloud Coverage, double& Wind Intensity, double& Rain, double& Snow, double& Dust, double& Fog, double& Thunder/Lightning);
    void Get Control Point Location(FVector& Location);
    void Get UDW State for Saving(FUDS_and_UDW_State& UDW State);
    void Impact Snow/ Puddle/ Dust at Location(FVector Location, double Radius, double Speed, bool Affect Snow/Dust, bool Affect Puddles, bool& Hit Puddle, bool& Hit Snow/Dust);
    void Update Custom Weather Particle Camera();
    void Generate Weather State At Location(FVector Location, class UUDS_Weather_Settings_C* Settings Object, TMap<AWeather_Override_Volume_C*, double>& WOVs Applied at Location);
    void Wind Debris Niagara System Asset(TSoftObjectPtr<UNiagaraSystem>& Out);
    void Dust Niagara System Asset(TSoftObjectPtr<UNiagaraSystem>& Out);
    void Snow Niagara System Asset(TSoftObjectPtr<UNiagaraSystem>& Out);
    void Rain Niagara System Asset(TSoftObjectPtr<UNiagaraSystem>& Out);
    void Make Lightning Flash Systems();
    void Obscured Lightning System Finished(class UNiagaraComponent* PSystem);
    double Obscured Lightning Current Spawn Rate();
    void Make Obscured Lightning Component();
    void Wind Debris System Finished(class UNiagaraComponent* PSystem);
    void Snow System Finished(class UNiagaraComponent* PSystem);
    void Make Snow Component();
    void Rain System Finished(class UNiagaraComponent* PSystem);
    void Screen Frost Startup();
    void Update Target Screen Frost();
    double Target Screen Frost Strength();
    void Update Screen Frost();
    void Randomize Screen Frost Offset();
    void 📘 Screen Frost();
    void Filter Radial Storm Array();
    void Static Mode Tick();
    void Static Properties - Screen Frost();
    void Startup Static Mode();
    void Increment Wind Direction Transition();
    void Change Wind Direction(double New Wind Direction, double Change Duration);
    void Game User Settings Update();
    void Bind to Game User Settings();
    void Copy Manual State Object to Variables();
    void Static Properties - Post Processing();
    void Manual Override to State Value(class UUDS_Weather_Settings_C* Target, int32 Index);
    void Toggle Post Process Material(int32 Index, bool Enabled);
    void Add Constructed Components();
    void Apply Sound Update Periods();
    void Material Effect Draw Color from State(class UUDS_Weather_Settings_C* State, double Alpha, FLinearColor& Color);
    void Version Specific Correction();
    void Apply Radial Storm Actors to Local Weather(bool For Local Weather);
    void Get Starting Radial Storms();
    double Lightning Intensity Day/Night Scale();
    void Set Startup Variables();
    void Get Weather Presets Used By Local Weather(TMap<UUDS_Weather_Settings_C*, double>& Sources);
    void Apply Weather Override Volume State(class AWeather_Override_Volume_C* WOV, double Alpha, bool For Local Weather);
    void Lerp State Source Maps(class UUDS_Weather_Settings_C* A, class UUDS_Weather_Settings_C* B, double Alpha, class UUDS_Weather_Settings_C* Target Object);
    void Set DLWE Snow/Dust Parameters();
    bool Any Manual Overrides Applied();
    void Copy Weather State Structure to Object(class UUDS_Weather_Settings_C* State, FUDW_WeatherState_Structure Struct);
    void Weather State Object to Structure(class UUDS_Weather_Settings_C* State, FUDW_WeatherState_Structure& Structure);
    void Apply Manual State Override Values(bool For Local Weather);
    void Get Random Weather Forecast(TArray<class UUDS_Weather_Settings_C*>& Upcoming Weather Objects);
    void Get Season(double& Season, TEnumAsByte<UDS_Season::Type>& Season Enum);
    void Daily Season Update();
    void Hourly Season Update(int32 Hour);
    void Bind to UDS Dispatchers();
    void Set Season(double Season);
    void Level Editor Lightning Flash Spawning();
    void Clear All Render Targets();
    bool Allow Render Target Drawing();
    void Start Up Render Targets();
    void Check for Render Target Recentering();
    void 📘 Event Dispatchers();
    void Construct Weather State Object if Invalid(class UUDS_Weather_Settings_C*& State);
    void Static Properties - Post Process Wind Fog();
    void Static Properties - Screen Droplets();
    void Static Properties - Heat Distortion();
    void Static Properties - Rainbow();
    void Static Properties - Sound Effects();
    void Static Properties - DLWE();
    void Static Properties - Occlusion();
    void Static Properties - Lightning();
    void Static Properties - Dust();
    void Static Properties - Wind Debris();
    void Static Properties - Snow();
    void Static Properties - Rain();
    void Static Properties - Shared Particles();
    void Static Properties - Material Effects();
    void Active Occlusion State(class UUDS_OcclusionState_C*& State);
    bool Runtime Or Initializing();
    void Set All Update Checks(bool On);
    void Call Editor Dispatchers();
    void 📘 Volumetric Fog Particles();
    void 📘 Post Process Wind Fog();
    void 📘 Wind Directional Source();
    void 📘 Wind Debris();
    void 📘 Wind Direction();
    void 📘 Weather Mask Tools();
    void Get Projection Box Scene Capture 2D(class USceneCaptureComponent2D*& Out);
    void 📘 Weather Override Volumes();
    void 📘 Weather Above Volumetric Clouds();
    void 📘 Temperature();
    void 📘 Water Level();
    void 📘 Rainbow();
    void 📘 Heat Distortion();
    void 📘 Screen Droplets();
    void 📘 Sound Occlusion();
    void 📘 Sound Effects();
    void 📘 Dynamic Landscape Weather Effects();
    void 📘 Material Effects();
    void 📘 Lightning();
    void 📘 Random Weather Variation();
    void 📘 Season();
    void 📘 Weather Particles();
    void 📘 Dust Particles();
    void 📘 Snow Particles();
    void 📘 Rain Particles();
    void 📘 Manual Weather State();
    void 📘 Weather Documentation();
    void Open Editor Readme Entry Set(const TArray<FName>& Entries);
    void Open Editor Readme Entry(FString Entry Row);
    void Get Target Heat Distortion();
    void Update Heat Distortion();
    void Call Custom Weather Behavior();
    void Make Random Stream();
    void Clear All Material Parameters to Zero Coverage();
    void Instant Sound Update();
    void Get Sound Upward Occlusion(double& Out);
    void Get Sound Directional Occlusion(TArray<double>& Out);
    void Get Sound Global Occlusion(double& Out);
    void Update Underwater State();
    void Screen Droplets Periodic Updates();
    void Populate Weather State(class UUDS_Weather_Settings_C* State, double Cloud Coverage, double Rain, double Snow, double Thunder/Lightning, double Wind Intensity, double Fog, double Dust, double Material Wetness, double Material Snow, double Material Dust);
    double Rainbow Strength();
    void Fog Vertical Velocity(double& Out);
    void Update Wind Directional Source Actor();
    void Update Obscured Lightning Parameters();
    double Snow / Dust Velocity Randomization(double Low Wind, double High Wind);
    double Dust Sprite Alpha();
    double Dust Spawn Rate();
    double Snow Spawn Rate();
    double Rain Spawn Rate();
    void Make Rain Component();
    double Wind Debris Spawn Rate();
    FVector Wind Force Vector();
    void Apply Weather Changes Above Cloud Layer(bool For Local Weather);
    void Sparse Movement Updates();
    double Sky Cloud Speed();
    void Fill Starting Update Buffer();
    void Check for Weather Value Update Threshold(double New Value, double& Buffer Value, double Threshold, double Range Max, bool& Update Needed Bool, TEnumAsByte<UDS_Weather_State_Variable::Type> State Variable);
    void Monitor Local Weather Changes();
    void Warm Up Niagara Systems();
    void Increment Transition Between States();
    void Start Active Timers();
    void Initialize Random Weather Variation();
    void Set Random Time Offset();
    void Set UDS Reference();
    void Set Weather Values Prepped for UDS();
    void Apply Climate Preset Object(class UUDS_Climate_Preset_C* Climate);
    void Apply Interior Temperature with Occlusion(double Temp, double Occlusion, double& Out);
    void Check to Change Temperature Scale();
    void Convert Temperature Scale(double Input, TEnumAsByte<UDS_TemperatureType::Type> In Scale, TEnumAsByte<UDS_TemperatureType::Type> Out Scale, bool Relative Degrees, bool Snap, double& Output);
    void Apply Local Temperature Effects(double Temp, double Occlusion, FVector Location, double& Temp Out);
    void Trace Bounds for Exposure Values(FVector Bounds Origin, FVector Bounds Extent, TArray<class AActor*>& Actors to Ignore, bool Test Weather, double& Exposure Value);
    void Test Component for Wind Exposure(class UPrimitiveComponent* Component, double& Wind Exposure);
    void Get Normalized Wind Direction(FVector& Wind Vector);
    void Update Lightning Flash Light();
    void Lerp Yaw Angles(double Angle A, double Angle B, double Alpha, double& Out);
    double Combined Wind Direction();
    void Lightning Bolt Target Offset(FVector& Out);
    void Report Removal Of Mask Component(class UWeatherMask_C* Component);
    void Query Project Settings();
    void Apply Max to Material Effects(class UUDS_Weather_Settings_C* Weather, double& Snow Coverage, double& Wetness, double& Dust);
    void Update Outdoor Sound Mix();
    void Make Outdoor Sound Mix();
    void Update WOV Render Target();
    void Level Editor WOV Update();
    void Apply Weather Override Volumes(bool For Local Weather, TMap<AWeather_Override_Volume_C*, double>& WOVs Applied);
    void Set WOV Render Target Mapping();
    void PPWF Intensity(double& Out);
    void Lightning Flash Location(bool& Found Valid Location, FVector& Loc);
    void Lightning World Height(double& Height);
    void Invalidate VHFM Level(double Threshold, double Distance, FVector& Buffer, FVector Current, bool& Continue);
    void Invalidate VHFM With Material States();
    void Update Rainbow();
    void Initialize Occlusion();
    void Get Current Sound Occlusion Values(double& Non-Directional Occlusion, double& X+ Occlusion, double& Y+ Occlusion, double& X- Occlusion, double& Y- Occlusion, double& Upward Occlusion);
    void Get Display Name for Current Weather(FString& As String, TEnumAsByte<UDS_Weather_Display_Names::Type>& As Enumerator);
    void Make Manual State();
    void Update Season();
    void Update Fog Particle Parameters(class UFXSystemComponent* Target, double Max Particle Percentage, double Weather Intensity, double Particle Intensity);
    void Update Active Dust Parameters();
    void Update Active Wind Debris Parameters();
    void Update Active Snow Parameters();
    void Update Active Rain Parameters();
    void Update Old State With Replicated Variables();
    void Set Replicated Old Weather State();
    void Copy Weather State(class UUDS_Weather_Settings_C* Source, class UUDS_Weather_Settings_C* Target, bool Set Material Effects, bool Copy Sources);
    void Construct All Weather State Objects();
    void Lerp Between Weather States(class UUDS_Weather_Settings_C* A, class UUDS_Weather_Settings_C* B, double Alpha, class UUDS_Weather_Settings_C* Target State, bool Set Material Effects, bool Use Bias for Material Effects, bool Lerp Sources);
    void Currently Dusty(bool& Yes);
    void Close Thunder Sound Delay(double& Delay);
    void Apply Sound Effects Volume Levels();
    void Update Sound Occlusion Parameters();
    void Update Weather Mask Target();
    void Construct Weather Mask Target State();
    void Currently Cloudy(bool& Yes);
    void Currently Snowing(bool& Yes);
    void Currently Raining(bool& Yes);
    void Wind Rotation(FRotator& Rot);
    void Update DLWE Interaction Mode();
    void Lightning Distance Range(double& Range);
    void Current Dust Velocity(FVector& Velocity);
    void Dust System Finished(class UNiagaraComponent* PSystem);
    void Make Dust Component();
    void Current Wind Debris Velocity(FVector& Velocity);
    void Make Wind Debris Component();
    void Current Snow Velocity(FVector& Velocity);
    void Current Rain Velocity(FVector& Velocity);
    void Set Shared Weather Particle Parameters(class UNiagaraComponent* System);
    void Construct WOV Render Target State();
    void Filter Weather Override Volumes Array();
    void Add Weather Override Volume to Array(class AWeather_Override_Volume_C* Weather Override Volume);
    void Sort Weather Override Volumes();
    void Get Starting Weather Override Volumes();
    void Get WOVs Applied to Location(FVector Location, TArray<class AWeather_Override_Volume_C*>& WOV Array, TMap<AWeather_Override_Volume_C*, double>& Applied WOV Map);
    void Update Screen Droplets();
    void Check for Events to Dispatch();
    void Test Actor for Weather Exposure(class AActor* Actor, bool Test Colliding Components Only, class USceneComponent* Custom Component for Bounds, double& Rain Exposure, double& Snow Exposure, double& Wind Exposure, double& Dust Exposure);
    void Update Post Process Wind Fog();
    void Lightning Flash Period(double& Period);
    void Start Lightning Flash();
    void Update Material Effect Parameters();
    void Second Frame Startup Functions();
    void Create Current Local Weather State(FVector Test Location);
    void Set Current Global Weather State();
    void Update Current Global And Local Weather State();
    void Get New Target Global Weather State(class UUDS_Weather_Settings_C*& Out, bool& Changing, bool& Manual State);
    void Check Point for Puddles Snow Or Dust(FVector Location, FVector Ground Normal, class UPhysicalMaterial* Physical Material, double& Puddle Depth, double& Snow Depth, double& Dust Depth);
    void Create Cloud Reference Array();
    void Fade DLWE Target Over Time();
    void Update DLWE Snow Compressions();
    void DLWE Active Update();
    void Compress DLWE Snow/Dust(FVector Location, double Size);
    void Recenter DLWE Render Target();
    void Start Up DLWE Interaction System();
    void Get Current Temperature(TEnumAsByte<UDS_Temperature_Sample_Location::Type> Sample Location, FVector Custom Sample Location, TEnumAsByte<UDS_TemperatureType::Type> Scale, double& Output);
    void Filter Probability Map(TMap<UUDS_Weather_Settings_C*, double> Probability Map, class UUDS_Weather_Settings_C* Current Random Type, TMap<UUDS_Weather_Settings_C*, double>& Filtered Probability Map);
    void Weather Startup Functions();
    void Construction Script Function();
    void Load Soft Object Array(TArray<TSoftObjectPtr<UObject>>& In);
    void Load Required Assets();
    void Check UDS Version();
    void Update Sounds With Weather State();
    void Increment Global Material Effects();
    void Update Static Variables();
    void Update Active Variables();
    void Set Current Control Point Location();
    void Get Current Player Camera/ Pawn Locations();
    void Tick Function();
    void UserConstructionScript();
    void Latent WOV Target Update();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Change Weather(class UUDS_Weather_Settings_C* New Weather Type, double Time To Transition To New Weather (Seconds));
    void Clients Transition Start(double Duration);
    void Change to Random Weather Variation(double Time to Transition to Random Weather (Seconds), TEnumAsByte<UDS_RandomWeatherTiming::Type> Random Weather Mode);
    void Restart Random Weather Variation();
    void Latent Weather Mask Update();
    void Flash Lightning(double Angle, bool Use Custom Lightning Location, FVector Custom Lightning Location, FVector Custom Target Location, int32 Lightning Bolt Seed);
    void Global Lightning Internal(double Angle, double Thunder/Lightning Threshold);
    void Editor Lightning Internal();
    void Initialize Weather(class AUltra_Dynamic_Sky_C* UDS);
    void ReceiveBeginPlay();
    void Force Tick();
    void UDW Runtime Tick(double Delta Time);
    void ExecuteUbergraph_Ultra_Dynamic_Weather(int32 EntryPoint);
    void Lightning Flash Started__DelegateSignature();
    void Random Weather Season Refresh__DelegateSignature();
    void Temperature Range Update__DelegateSignature();
    void State Change - Material Dust__DelegateSignature();
    void State Change - Material Snow__DelegateSignature();
    void State Change - Material Wetness__DelegateSignature();
    void State Change - Cloud Coverage__DelegateSignature();
    void State Change - Wind Direction__DelegateSignature();
    void State Change - Thunder/Lightning__DelegateSignature();
    void State Change - Fog__DelegateSignature();
    void State Change - Dust__DelegateSignature();
    void State Change - Wind Intensity__DelegateSignature();
    void State Change - Snow__DelegateSignature();
    void State Change - Rain__DelegateSignature();
    void Season Changed__DelegateSignature(TEnumAsByte<UDS_Season::Type> Season);
    void Weather Display Name Changed__DelegateSignature(TEnumAsByte<UDS_Weather_Display_Names::Type> Weather Name);
    void Dust/Sand Clearing__DelegateSignature();
    void Dust/Sand Forming__DelegateSignature();
    void Clouds Clearing__DelegateSignature();
    void Getting Cloudy__DelegateSignature();
    void Finished Snowing__DelegateSignature();
    void Finished Raining__DelegateSignature();
    void Started Snowing__DelegateSignature();
    void Started Raining__DelegateSignature();
}; // Size: 0x1B80

#endif
