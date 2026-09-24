#ifndef UE4SS_SDK_WB_Effect_HPP
#define UE4SS_SDK_WB_Effect_HPP

class UWB_Effect_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UWidgetSwitcher* WS_EffectTextureType;                                      // 0x0348 (size: 0x8)
    class USizeBox* SB_Particle;                                                      // 0x0350 (size: 0x8)
    class USizeBox* SB_Content;                                                       // 0x0358 (size: 0x8)
    class UOverlay* OV_Stretched_Scale;                                               // 0x0360 (size: 0x8)
    class UOverlay* OV_Punctiform_Scale;                                              // 0x0368 (size: 0x8)
    class UOverlay* OV_Punctiform;                                                    // 0x0370 (size: 0x8)
    class UOverlay* OV_Particle_Container;                                            // 0x0378 (size: 0x8)
    class UOverlay* OV_Particle;                                                      // 0x0380 (size: 0x8)
    class UImage* ImageStretched;                                                     // 0x0388 (size: 0x8)
    class UImage* ImagePunctiform;                                                    // 0x0390 (size: 0x8)
    class UBorder* B_Stretched;                                                       // 0x0398 (size: 0x8)
    class UBorder* B_SoundEffect;                                                     // 0x03A0 (size: 0x8)
    class UBorder* B_Punctiform;                                                      // 0x03A8 (size: 0x8)
    class UBorder* B_Particle;                                                        // 0x03B0 (size: 0x8)
    class UBorder* b_HighlightLoop;                                                   // 0x03B8 (size: 0x8)
    class UBorder* B_Content;                                                         // 0x03C0 (size: 0x8)
    class UWidgetAnimation* FadeIn;                                                   // 0x03C8 (size: 0x8)
    class UWidgetAnimation* FadeHighlight1;                                           // 0x03D0 (size: 0x8)
    class UWidgetAnimation* FadeHighlight2;                                           // 0x03D8 (size: 0x8)
    class UWidgetAnimation* HighlightLoop;                                            // 0x03E0 (size: 0x8)
    TEnumAsByte<EProgressBarFillType::Type> FillType;                                 // 0x03E8 (size: 0x1)
    double CurrentPercent;                                                            // 0x03F0 (size: 0x8)
    FS_Effects Effect;                                                                // 0x03F8 (size: 0x88)
    FVector2D Size;                                                                   // 0x0480 (size: 0x10)
    double LastPercent;                                                               // 0x0490 (size: 0x8)
    bool bIsDesignTime;                                                               // 0x0498 (size: 0x1)
    bool IsActive;                                                                    // 0x0499 (size: 0x1)
    class UAudioComponent* UIAudio;                                                   // 0x04A0 (size: 0x8)
    bool EffectActive;                                                                // 0x04A8 (size: 0x1)
    FWB_Effect_CStartProgressChangeColor StartProgressChangeColor;                    // 0x04B0 (size: 0x10)
    void StartProgressChangeColor(FLinearColor Color);
    FWB_Effect_CStopProgressChangeColor StopProgressChangeColor;                      // 0x04C0 (size: 0x10)
    void StopProgressChangeColor();

    bool IsEffectTypeProgressChangeColor();
    void GetDecayTime(double& Decay Time);
    bool IsEffectTypeSoundEffectLooped();
    void GetSoundVolume(double& Sound Volume Multiplier);
    bool IsEffectTypeSoundEffect();
    void GetSoundEffect(class USoundBase*& Sound Effect);
    void GetAddRotationToParticles(bool& Add Rotation to Particles);
    void GetParticleSpread(double& Particle Spread);
    double FindMinSize(FVector2D Size);
    void AddAttributes(class UImage* Image);
    void GetNumParticles(int32& NumParticles);
    bool IsEffectTypeParticle();
    void GetPlaybackSpeed(double& Playback Speed);
    void GetEffectAngle(double& Effect Angle);
    void GetEffectShear(FVector2D& Effect Shear);
    bool IsTriggeredAlwaysOnSpecificPercentValue();
    bool IsTriggerSpecificPercent();
    void SwitchEffectType(int32 Index);
    void Anim_HighlightLoop(bool StartStop, double PlaybackSpeed);
    void Anim_Highlight2(double PlaybackSpeed);
    void Anim_Highlight1(double PlaybackSpeed);
    void GetEffectColor(FLinearColor& Effect Color);
    void GetEffectScale(FVector2D& EffectScale);
    void Anim_Fade(bool FadeIn, double PlaybackSpeed);
    bool IsTriggeredAlways();
    void GetEffectTexture(class UObject*& Custom Effect Texture);
    void GetEffectType(TEnumAsByte<EEffectType::Type>& Effect Texture Type);
    void GetEffectTranslation(FVector2D& Transition);
    void GetSpecificPercentValue(double& Specific Percent Value);
    void GetTriggerMethod(TEnumAsByte<ETriggerMethod::Type>& Trigger);
    void SetSize(FVector2D Size);
    void AddEffect(FS_Effects Effect);
    void SetFillType(TEnumAsByte<EProgressBarFillType::Type> FillType);
    void StartEffect();
    void StopEffect();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void TriggerEffect();
    void AddEffectTextureType();
    void EventPreConstruct(bool IsDesignTime);
    void Construct();
    void CreateParticle(int32 NumParticles);
    void ClearParticles();
    void OnAnimationFinished_Event_0();
    void GetProgressBar(class UProgressBarLinear_C* ProgressBar);
    void UpdatePercent(double Percent);
    void OnAudioFinished_Event_0();
    void ExecuteUbergraph_WB_Effect(int32 EntryPoint);
    void StopProgressChangeColor__DelegateSignature();
    void StartProgressChangeColor__DelegateSignature(FLinearColor Color);
}; // Size: 0x4D0

#endif
