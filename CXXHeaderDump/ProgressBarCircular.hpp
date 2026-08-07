#ifndef UE4SS_SDK_ProgressBarCircular_HPP
#define UE4SS_SDK_ProgressBarCircular_HPP

class UProgressBarCircular_C : public UWB_Base_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C8 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02D0 (size: 0x8)
    class UBorder* B_Content;                                                         // 0x02D8 (size: 0x8)
    class UOverlay* OV_Effects_Background;                                            // 0x02E0 (size: 0x8)
    class UOverlay* OV_Effects_Foreground;                                            // 0x02E8 (size: 0x8)
    class URetainerBox* RB_BlendMask;                                                 // 0x02F0 (size: 0x8)
    class USizeBox* SB_Content;                                                       // 0x02F8 (size: 0x8)
    class UWB_Container_Circular_C* WB_Container_Circular;                            // 0x0300 (size: 0x8)
    double Size;                                                                      // 0x0308 (size: 0x8)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod;                                // 0x0310 (size: 0x1)
    double QueueDelay;                                                                // 0x0318 (size: 0x8)
    double CurrentPercent;                                                            // 0x0320 (size: 0x8)
    double InterpTimeCurrent;                                                         // 0x0328 (size: 0x8)
    FLinearColor FillColorCurrent;                                                    // 0x0330 (size: 0x10)
    class UTexture2D* FillColorMask;                                                  // 0x0340 (size: 0x8)
    bool UseGradient;                                                                 // 0x0348 (size: 0x1)
    bool UseTargetPercent;                                                            // 0x0349 (size: 0x1)
    double TargetPercent;                                                             // 0x0350 (size: 0x8)
    double InterpTimeTarget;                                                          // 0x0358 (size: 0x8)
    FLinearColor TargetFillColor_Positive;                                            // 0x0360 (size: 0x10)
    FLinearColor TargetFillColor_Negative;                                            // 0x0370 (size: 0x10)
    FLinearColor BackgroundColor;                                                     // 0x0380 (size: 0x10)
    class UTexture2D* BackgroundMask;                                                 // 0x0390 (size: 0x8)
    double DeltaTime;                                                                 // 0x0398 (size: 0x8)
    double InterpValue;                                                               // 0x03A0 (size: 0x8)
    double NewPercent;                                                                // 0x03A8 (size: 0x8)
    double OldPercent;                                                                // 0x03B0 (size: 0x8)
    double InterpValueTarget;                                                         // 0x03B8 (size: 0x8)
    double NewTargetPercent;                                                          // 0x03C0 (size: 0x8)
    double OldTargetPercent;                                                          // 0x03C8 (size: 0x8)
    bool CanSetTargetPercent;                                                         // 0x03D0 (size: 0x1)
    bool CanSetPercent;                                                               // 0x03D1 (size: 0x1)
    bool bIsDesignTime;                                                               // 0x03D2 (size: 0x1)
    TEnumAsByte<EGradientTypes::Type> GradientType;                                   // 0x03D3 (size: 0x1)
    double GradientColorVariation;                                                    // 0x03D8 (size: 0x8)
    class UTexture2D* BlendMask;                                                      // 0x03E0 (size: 0x8)
    double Thickness;                                                                 // 0x03E8 (size: 0x8)
    double Density;                                                                   // 0x03F0 (size: 0x8)
    double StepDensity;                                                               // 0x03F8 (size: 0x8)
    bool bUseMarquee;                                                                 // 0x0400 (size: 0x1)
    FLinearColor MarqueeColor;                                                        // 0x0404 (size: 0x10)
    int32 Separation_Steps;                                                           // 0x0414 (size: 0x4)
    double Separation_Steps_Spacing;                                                  // 0x0418 (size: 0x8)
    bool bIsSeparated;                                                                // 0x0420 (size: 0x1)
    bool Separation_AbsoluteFillMethod;                                               // 0x0421 (size: 0x1)
    FVector2D LocalSize;                                                              // 0x0428 (size: 0x10)
    TArray<FS_Effects> Effects;                                                       // 0x0438 (size: 0x10)
    class UWB_Effect_C* CurrentEffectWidget;                                          // 0x0448 (size: 0x8)
    FProgressBarCircular_COnPercentChanged OnPercentChanged;                          // 0x0450 (size: 0x10)
    void OnPercentChanged(double Percent);
    double GradientOpacity;                                                           // 0x0460 (size: 0x8)
    FLinearColor MarqueeColorBackground;                                              // 0x0468 (size: 0x10)
    double MarqueeTime;                                                               // 0x0478 (size: 0x8)
    TEnumAsByte<EMarqueeMask::Type> MarqueeMask;                                      // 0x0480 (size: 0x1)
    class UTexture2D* CustomMarqueeMask;                                              // 0x0488 (size: 0x8)
    FTimerHandle Timer;                                                               // 0x0490 (size: 0x8)
    TArray<double> Entries;                                                           // 0x0498 (size: 0x10)
    bool bIsProgressRunning;                                                          // 0x04A8 (size: 0x1)
    bool PauseWithGamePause;                                                          // 0x04A9 (size: 0x1)

    void Count_TargetPercent(double Time, double& Value);
    void RemoveEntryIndex(int32 IndexToRemove);
    bool IsValidEntryIndex(int32 IndexToTest);
    void GetEntryIndex(int32 Index, double& Value);
    bool IsEffectProgressChangeColor(const FS_Effects& Effect);
    void PB_GetSeparationSteps(int32& Steps);
    void PB_AddEffect(FS_Effects Effect, int32& Index);
    void PB_GetEffects(TArray<FS_Effects>& Effects);
    void PB_GetIsMarquee(bool& IsMarquee);
    void PB_GetUseGradientFillColor(bool& UseGradientFillColor);
    void PB_GetFillColor(FLinearColor& Color);
    void PB_GetTargetFillColor_Negative(FLinearColor& Color);
    void PB_GetTargetFillColor_Positive(FLinearColor& Color);
    void PB_GetInterpTimeTarget(double& TargetInterpTime);
    void PB_GetInterpTimeCurrent(double& CurrentInterpTime);
    void PB_GetProgressMethod(TEnumAsByte<EProgressMethod::Type>& EProgressMethod);
    void PB_GetUseTargetPercent(bool& UseTargetPercent);
    void PB_GetTargetPercent(double& TargetPercent);
    void PB_GetPercent(double& Percent);
    void PB_GetSize(FVector2D& Size);
    void ClearEffectLayers(TEnumAsByte<EEffectLayer::Type> EffectLayer);
    void FindEffectOverlay(TEnumAsByte<EEffectLayer::Type> EffectLayer, class UOverlay*& Overlay);
    void Count_Percent(double Time, double& Value);
    void Transfer_CurrentPercent(double Percent);
    void Transfer_UseTargetPercent(bool UseTargetPercent);
    void Transfer_TargetPercent(double TargetPercent);
    void SetBlendMask(class UTexture* Value);
    double GetInterpolationTime();
    bool IsProgressMethodInterpolated();
    bool IsProgressMethodStatic();
    void PB_SetSize(FVector2D Size);
    void PB_SetPercent(double Value);
    void PB_SetFillColor(FLinearColor Color);
    void PB_SetTargetPercent(double TargetPercent);
    void PB_SetThickness(double Thickness);
    void PB_SetUseTargetPercent(bool UseTargetPercent);
    void PB_SetProgressMethod(TEnumAsByte<EProgressMethod::Type> EProgressMethod);
    void PB_SetInterpTimeCurrent(double CurrentInterpTime);
    void PB_SetInterpTimeTarget(double TargetInterpTime);
    void PB_SetTargetFillColor_Positive(FLinearColor Color);
    void PB_SetTargetFillColor_Negative(FLinearColor Color);
    void PB_SetUseGradientFillColor(bool UseGradientFillColor);
    void PB_SetBackgroundColor(FLinearColor Color);
    void PB_SetFillColorGradientPower(double GradientPower);
    void PB_SetFillColorMask(class UObject* Mask);
    void PB_SetBackgroundColorMask(class UTexture2D* Mask);
    void PB_SetFillColorGradientType(TEnumAsByte<EGradientTypes::Type> GradientType);
    void PB_SetBlendMask(class UTexture2D* BlendMask);
    void PB_SetIsMarquee(bool IsMarquee);
    void PB_SetCustomMarqueeImage(class UTexture2D* Image);
    void PB_SetCustomMarqueeMaskType(TEnumAsByte<EMarqueeMask::Type> MaskType);
    void PB_SetSeparationSteps(int32 Steps);
    void PB_SetSeparationStepsSpacing(double Spacing);
    void PB_SetSeparationAbsoluteFill(bool AbsoluteFillMethod);
    void PB_SetEffects(const TArray<FS_Effects>& Effects);
    void PB_SetEffectEnabled(int32 Index, bool IsEnabled);
    void PB_SetAllEffectsEnabled(bool IsEnabled);
    void PB_RemoveEffect(int32 Index);
    void UpdatePercent(double Percent);
    void UpdateInterpolation();
    void SetSize(double Size);
    void SetUseGradient(bool UseGradient, double GradientOpacity, TEnumAsByte<EGradientTypes::Type> GradientType);
    void SetTargetPercent(bool UseTargetPercent, double TargetPercent);
    void SetTargetFillColorPositive(FLinearColor Color);
    void SetTargetFillColorNegative(FLinearColor Color);
    void SetProgressMethod(TEnumAsByte<EProgressMethod::Type> ProgressMethod);
    void SetPercentInterpolated(double NewPercent);
    void SetFillColorMask(class UTexture2D* Value);
    void SetOverallBlendMask(class UTexture2D* BlendMask);
    void CreateEffects();
    void AddEffect(FS_Effects Effect);
    void Reset();
    void SetThickness(double Thickness);
    void SetSteps(int32 Separation_Steps);
    void SetSpacing(double Spacing);
    void SetDensity(double Density);
    void SetStepDensity(double HardStepBorder);
    void SetBackgroundColor(FLinearColor BackgroundColor, class UTexture2D* BackgroundMask);
    void SetFillColor(FLinearColor FillColorCurrent, double GradientPower);
    void UseAbsoluteFillMethod(bool AbsoluteFillMethod);
    void SetupMarquee(bool UseMarquee, FLinearColor MarqueeColor, FLinearColor MarqueeBackgroundColor);
    void SetMarqueeTime(double Value);
    void SetMarqueeMask(TEnumAsByte<EMarqueeMask::Type> MarqueeMask, class UTexture2D* CustomMarqueeMask);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void AddEntry(double NewPercent);
    void InitNextEntry();
    void ExecuteEntry(double CurrentEntry);
    void Reconstruct();
    void Construct();
    void PreConstruct(bool IsDesignTime);
    void StopTriggerProgressChangeColor();
    void StartTriggerProgressChangeColor(FLinearColor Color);
    void HandlePausedQueue();
    void ExecuteUbergraph_ProgressBarCircular(int32 EntryPoint);
    void OnPercentChanged__DelegateSignature(double Percent);
}; // Size: 0x4AA

#endif
