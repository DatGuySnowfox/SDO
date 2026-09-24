#ifndef UE4SS_SDK_WB_Particle_HPP
#define UE4SS_SDK_WB_Particle_HPP

class UWB_Particle_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* ParticleImage;                                                      // 0x0348 (size: 0x8)
    class UOverlay* OV_ParticleFX;                                                    // 0x0350 (size: 0x8)
    class UBorder* B_Particle;                                                        // 0x0358 (size: 0x8)
    class UBorder* B_Color;                                                           // 0x0360 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0368 (size: 0x8)
    double PlaybackSpeed;                                                             // 0x0370 (size: 0x8)
    bool bCanTrigger;                                                                 // 0x0378 (size: 0x1)
    double Value;                                                                     // 0x0380 (size: 0x8)
    double RandomScale;                                                               // 0x0388 (size: 0x8)
    class UWB_Effect_C* Parent;                                                       // 0x0390 (size: 0x8)
    FVector2D RandomTranslation;                                                      // 0x0398 (size: 0x10)
    FVector2D Scale;                                                                  // 0x03A8 (size: 0x10)
    FVector2D Translation;                                                            // 0x03B8 (size: 0x10)
    FVector2D Shear;                                                                  // 0x03C8 (size: 0x10)
    double Angle;                                                                     // 0x03D8 (size: 0x8)
    FVector2D Size;                                                                   // 0x03E0 (size: 0x10)
    TEnumAsByte<ETriggerMethod::Type> TriggerMethod;                                  // 0x03F0 (size: 0x1)
    bool IsDesignTime;                                                                // 0x03F1 (size: 0x1)
    double RandomPlaybackspeed;                                                       // 0x03F8 (size: 0x8)
    bool TriggerActive;                                                               // 0x0400 (size: 0x1)
    double ParticleSpread;                                                            // 0x0408 (size: 0x8)
    bool UseParticleRotation;                                                         // 0x0410 (size: 0x1)
    double RandomAngle;                                                               // 0x0418 (size: 0x8)

    void SetScaleV2D(FVector2D Scale);
    double CountValue(double Time);
    double GetClampedMinSize(double Multiplicator);
    void GetTranslation(FVector2D& Translation);
    void SetTranslation(FVector2D Translation);
    double GetMaxSize();
    double GetMinSize();
    bool IsTriggerMethodAlways();
    void Anim_Fade(double PlaybackSpeed);
    void SetAngle(double Angle);
    void SetScale(double Scale);
    void CreateParticle(class UObject* Image, double PlaybackSpeed, FLinearColor Color, FVector2D Scale, FVector2D Translation, FVector2D Shear, double Angle, FVector2D Size, TEnumAsByte<ETriggerMethod::Type> TriggerMethod, bool bIsDesignTime, double ParticleSpread, bool UseParticleRotation);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void TriggerParticle();
    void SetParticleInfo();
    void StopTrigger();
    void StartTrigger();
    void OnAnimationFinished_Event_0();
    void ExecuteUbergraph_WB_Particle(int32 EntryPoint);
}; // Size: 0x420

#endif
