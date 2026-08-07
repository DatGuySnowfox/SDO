#ifndef UE4SS_SDK_ProgressBarLinear_HPP
#define UE4SS_SDK_ProgressBarLinear_HPP

class UProgressBarLinear_C : public UWB_Base_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C8 (size: 0x8)
    class UBorder* B_Content;                                                         // 0x02D0 (size: 0x8)
    class UBorder* BackgroundBrush;                                                   // 0x02D8 (size: 0x8)
    class UBorder* BG_Brush_Left;                                                     // 0x02E0 (size: 0x8)
    class UBorder* BG_Brush_Right;                                                    // 0x02E8 (size: 0x8)
    class UBackgroundBlur* BlurLeft;                                                  // 0x02F0 (size: 0x8)
    class UBackgroundBlur* BlurRight;                                                 // 0x02F8 (size: 0x8)
    class UWB_Container_Linear_C* Container;                                          // 0x0300 (size: 0x8)
    class UBackgroundBlur* DefaultBlur;                                               // 0x0308 (size: 0x8)
    class UHorizontalBox* HB_BG_FillFromCenter;                                       // 0x0310 (size: 0x8)
    class UHorizontalBox* HB_Effects_Background;                                      // 0x0318 (size: 0x8)
    class UHorizontalBox* HB_Effects_Foreground;                                      // 0x0320 (size: 0x8)
    class UHorizontalBox* HB_PB_FillFromCenter;                                       // 0x0328 (size: 0x8)
    class UWB_Container_Linear_C* Left;                                               // 0x0330 (size: 0x8)
    class UOverlay* OV_Effect_BG_Left;                                                // 0x0338 (size: 0x8)
    class UOverlay* OV_Effect_BG_Right;                                               // 0x0340 (size: 0x8)
    class UOverlay* OV_Effect_FG_Left;                                                // 0x0348 (size: 0x8)
    class UOverlay* OV_Effect_FG_Right;                                               // 0x0350 (size: 0x8)
    class UOverlay* OV_Effects_Background;                                            // 0x0358 (size: 0x8)
    class UOverlay* OV_Effects_Foreground;                                            // 0x0360 (size: 0x8)
    class UOverlay* OV_ProgressBar;                                                   // 0x0368 (size: 0x8)
    class URetainerBox* RB_BlendMask;                                                 // 0x0370 (size: 0x8)
    class UWB_Container_Linear_C* Right;                                              // 0x0378 (size: 0x8)
    class USizeBox* SB_Background;                                                    // 0x0380 (size: 0x8)
    class USizeBox* SB_BG_Brush_Left;                                                 // 0x0388 (size: 0x8)
    class USizeBox* SB_BG_Brush_Right;                                                // 0x0390 (size: 0x8)
    class USizeBox* SB_Container;                                                     // 0x0398 (size: 0x8)
    class USizeBox* SB_Content;                                                       // 0x03A0 (size: 0x8)
    class UWidgetSwitcher* WS_Effects_Background_FillType;                            // 0x03A8 (size: 0x8)
    class UWidgetSwitcher* WS_Effects_Foreground_FillType;                            // 0x03B0 (size: 0x8)
    class UWidgetSwitcher* WS_FillType_BG;                                            // 0x03B8 (size: 0x8)
    class UWidgetSwitcher* WS_FillType_PB;                                            // 0x03C0 (size: 0x8)
    FVector2D Size;                                                                   // 0x03C8 (size: 0x10)
    FVector2D Size_Local;                                                             // 0x03D8 (size: 0x10)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod;                                // 0x03E8 (size: 0x1)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod_Local;                          // 0x03E9 (size: 0x1)
    double QueueDelay;                                                                // 0x03F0 (size: 0x8)
    double QueueDelay_Local;                                                          // 0x03F8 (size: 0x8)
    TEnumAsByte<EProgressBarFillType::Type> FillType;                                 // 0x0400 (size: 0x1)
    TEnumAsByte<EProgressBarFillType::Type> FillType_Local;                           // 0x0401 (size: 0x1)
    double FillFromCenterSpacing;                                                     // 0x0408 (size: 0x8)
    double CurrentPercent;                                                            // 0x0410 (size: 0x8)
    double CurrentPercent_Local;                                                      // 0x0418 (size: 0x8)
    double InterpTimeCurrent;                                                         // 0x0420 (size: 0x8)
    double InterpTimeCurrent_Local;                                                   // 0x0428 (size: 0x8)
    FLinearColor FillColorCurrent;                                                    // 0x0430 (size: 0x10)
    FLinearColor FillColorCurrent_Local;                                              // 0x0440 (size: 0x10)
    class UObject* FillColorMask;                                                     // 0x0450 (size: 0x8)
    class UObject* FillColorMask_Local;                                               // 0x0458 (size: 0x8)
    TEnumAsByte<ESlateBrushTileType::Type> FillColorBrushTiling;                      // 0x0460 (size: 0x1)
    TEnumAsByte<ESlateBrushTileType::Type> FillColorBrushTiling_Local;                // 0x0461 (size: 0x1)
    bool FillColorUseGradient;                                                        // 0x0462 (size: 0x1)
    bool UseTargetPercent;                                                            // 0x0463 (size: 0x1)
    bool UseTargetPercent_Local;                                                      // 0x0464 (size: 0x1)
    double TargetPercent;                                                             // 0x0468 (size: 0x8)
    double TargetPercent_Local;                                                       // 0x0470 (size: 0x8)
    double InterpTimeTarget;                                                          // 0x0478 (size: 0x8)
    double InterpTimeTarget_local;                                                    // 0x0480 (size: 0x8)
    FLinearColor TargetFillColor_Positive;                                            // 0x0488 (size: 0x10)
    FLinearColor TargetFillColor_Positive_Local;                                      // 0x0498 (size: 0x10)
    FLinearColor TargetFillColor_Negative;                                            // 0x04A8 (size: 0x10)
    FLinearColor BackgroundTint;                                                      // 0x04B8 (size: 0x10)
    FTimerHandle Timer;                                                               // 0x04C8 (size: 0x8)
    double DeltaTime;                                                                 // 0x04D0 (size: 0x8)
    double InterpValue;                                                               // 0x04D8 (size: 0x8)
    double NewPercent;                                                                // 0x04E0 (size: 0x8)
    double OldPercent;                                                                // 0x04E8 (size: 0x8)
    double InterpValueTarget;                                                         // 0x04F0 (size: 0x8)
    double NewTargetPercent;                                                          // 0x04F8 (size: 0x8)
    double OldTargetPercent;                                                          // 0x0500 (size: 0x8)
    bool CanSetTargetPercent;                                                         // 0x0508 (size: 0x1)
    bool CanSetPercent;                                                               // 0x0509 (size: 0x1)
    bool bIsDesignTime;                                                               // 0x050A (size: 0x1)
    bool FillColorUseGradient_Local;                                                  // 0x050B (size: 0x1)
    TEnumAsByte<EGradientTypes::Type> FillColorGradientType;                          // 0x050C (size: 0x1)
    TEnumAsByte<EGradientTypes::Type> FillColorGradientType_Local;                    // 0x050D (size: 0x1)
    double FillColorGradientPower;                                                    // 0x0510 (size: 0x8)
    FLinearColor BackgroundTint_Local;                                                // 0x0518 (size: 0x10)
    class UTexture2D* BackgroundMask;                                                 // 0x0528 (size: 0x8)
    class UTexture2D* BackgroundMask_Local;                                           // 0x0530 (size: 0x8)
    TEnumAsByte<ESlateBrushTileType::Type> BackgroundBrushTiling;                     // 0x0538 (size: 0x1)
    TEnumAsByte<ESlateBrushTileType::Type> BackgroundBrushTiling_Local;               // 0x0539 (size: 0x1)
    double BackgroundThickness;                                                       // 0x0540 (size: 0x8)
    double FillFromCenterSpacing_Local;                                               // 0x0548 (size: 0x8)
    class UTexture2D* BlendMask;                                                      // 0x0550 (size: 0x8)
    bool bUseCustomMarquee;                                                           // 0x0558 (size: 0x1)
    bool bUseCustomMarquee_Local;                                                     // 0x0559 (size: 0x1)
    TEnumAsByte<EMarqueeMethod::Type> CustomMarqueeMethod;                            // 0x055A (size: 0x1)
    TEnumAsByte<EMarqueeMethod::Type> CustomMarqueeMethod_Local;                      // 0x055B (size: 0x1)
    double CustomMarqueeSpeed;                                                        // 0x0560 (size: 0x8)
    double CustomMarqueeSpeed_Local;                                                  // 0x0568 (size: 0x8)
    FLinearColor CustomMarqueeTint;                                                   // 0x0570 (size: 0x10)
    FLinearColor CustomMarqueeTint_Local;                                             // 0x0580 (size: 0x10)
    TEnumAsByte<EMarqueeMask::Type> CustomMarqueeMaskType;                            // 0x0590 (size: 0x1)
    TEnumAsByte<EMarqueeMask::Type> CustomMarqueeMaskType_Local;                      // 0x0591 (size: 0x1)
    class UTexture2D* CustomMarqueeMask;                                              // 0x0598 (size: 0x8)
    class UTexture2D* CustomMarqueeMask_Local;                                        // 0x05A0 (size: 0x8)
    TEnumAsByte<ESlateBrushTileType::Type> CustomMarqueeMaskTiling;                   // 0x05A8 (size: 0x1)
    bool bUseDefaultMarquee;                                                          // 0x05A9 (size: 0x1)
    bool bUseDefaultMarquee_Local;                                                    // 0x05AA (size: 0x1)
    class UTexture2D* DefaultMarquee_Image;                                           // 0x05B0 (size: 0x8)
    class UTexture2D* DefaultMarquee_Image_Local;                                     // 0x05B8 (size: 0x8)
    FVector2D DefaultMarquee_ImageSize;                                               // 0x05C0 (size: 0x10)
    FVector2D DefaultMarquee_ImageSize_Local;                                         // 0x05D0 (size: 0x10)
    FLinearColor DefaultMarquee_Tint;                                                 // 0x05E0 (size: 0x10)
    FLinearColor DefaultMarquee_Tint_Local;                                           // 0x05F0 (size: 0x10)
    TEnumAsByte<ESlateBrushDrawType::Type> DefaultMarquee_DrawAs;                     // 0x0600 (size: 0x1)
    TEnumAsByte<ESlateBrushDrawType::Type> DefaultMarquee_DrawAs_Local;               // 0x0601 (size: 0x1)
    TEnumAsByte<ESlateBrushTileType::Type> DefaultMarquee_Tiling;                     // 0x0602 (size: 0x1)
    int32 Separation_Steps;                                                           // 0x0604 (size: 0x4)
    int32 Separation_Steps_Local;                                                     // 0x0608 (size: 0x4)
    double Separation_Steps_Spacing;                                                  // 0x0610 (size: 0x8)
    bool bIsSeparated;                                                                // 0x0618 (size: 0x1)
    double Separation_Steps_Spacing_Local;                                            // 0x0620 (size: 0x8)
    bool Separation_AbsoluteFillMethod;                                               // 0x0628 (size: 0x1)
    FVector2D LocalSize;                                                              // 0x0630 (size: 0x10)
    TArray<FS_Effects> Effects;                                                       // 0x0640 (size: 0x10)
    double BackgroundThickness_Local;                                                 // 0x0650 (size: 0x8)
    double BackgroundBlurStrength;                                                    // 0x0658 (size: 0x8)
    class UWB_Effect_C* CurrentEffectWidget;                                          // 0x0660 (size: 0x8)
    FProgressBarLinear_COnPercentChanged OnPercentChanged;                            // 0x0668 (size: 0x10)
    void OnPercentChanged(double Percent);
    class UTexture2D* BlendMask_Local;                                                // 0x0678 (size: 0x8)
    double FillColorGradientPower_Local;                                              // 0x0680 (size: 0x8)
    FLinearColor TargetFillColor_Negative_Local;                                      // 0x0688 (size: 0x10)
    double BackgroundBlurStrength_Local;                                              // 0x0698 (size: 0x8)
    TEnumAsByte<ESlateBrushTileType::Type> CustomMarqueeMaskTiling_Local;             // 0x06A0 (size: 0x1)
    TEnumAsByte<ESlateBrushTileType::Type> DefaultMarquee_Tiling_Local;               // 0x06A1 (size: 0x1)
    bool Separation_AbsoluteFillMethod_Local;                                         // 0x06A2 (size: 0x1)
    TArray<FS_Effects> Effects_Local;                                                 // 0x06A8 (size: 0x10)
    TArray<double> Entries;                                                           // 0x06B8 (size: 0x10)
    bool bIsProgressRunning;                                                          // 0x06C8 (size: 0x1)
    bool PauseWithGamePause;                                                          // 0x06C9 (size: 0x1)
    bool PauseWithGamePause_Local;                                                    // 0x06CA (size: 0x1)
    bool UseShaderProgressBar;                                                        // 0x06CB (size: 0x1)
    bool UseShaderProgressBar_Local;                                                  // 0x06CC (size: 0x1)

    void Count_TargetPercent(double Time, double& Value);
    void RemoveEntryIndex(int32 IndexToRemove);
    void GetEntryIndex(int32 Index, double& Value);
    bool IsValidEntryIndex(int32 IndexToTest);
    void SetLocalVariables();
    bool HasEffectChanged();
    bool NotEqualColor(const FLinearColor A, const FLinearColor B);
    bool HasAnyValueChanged();
    bool IsEffectProgressChangeColor(const FS_Effects& Effect);
    void PB_GetSeparationSteps(int32& Steps);
    void PB_AddEffect(FS_Effects Effect, int32& Index);
    void PB_GetEffects(TArray<FS_Effects>& Effects);
    void PB_GetIsCustomMarquee(bool& IsMarquee);
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
    void GetEffectOverlays(TArray<class UOverlay*>& Array);
    void FindEffectOverlaysCentered(TEnumAsByte<EEffectLayer::Type> EffectLayer, TArray<class UOverlay*>& Overlays);
    void SetCenterSpacing_Effects(double PaddingValue);
    void ClearEffectLayers(TEnumAsByte<EEffectLayer::Type> EffectLayer);
    void SetBlurEnabledDefault(bool bInIsEnabled);
    void SetBlurEnabledCentered(bool bInIsEnabled);
    void SetBlurStrengthDefault(double InStrength);
    void SetBlurStrengthCentered(double InStrength);
    void SendPercentToEffects(class UOverlay* EffectsOverlay, double Percent);
    void FindEffectOverlay(TEnumAsByte<EEffectLayer::Type> EffectLayer, class UOverlay*& Overlay);
    void Count_Percent(double Time, double& Value);
    void Transfer_CurrentPercent(double Percent);
    void Transfer_UseTargetPercent(bool UseTargetPercent);
    void Transfer_TargetPercent(double TargetPercent);
    void SetBlendMask(class UTexture* Value);
    bool IsFillTypeTopOrBottom();
    double GetFillFromCenterSpacing();
    void SetCenterSpacing_BG(double PaddingValue);
    void SetSize_BGBrushLeftRight(FVector2D Size);
    FVector2D FindThicknessSizeCentered(double Thickness);
    double GetCenterSpacingSize();
    void SetCenterSpacing(double PaddingValue);
    void GetChildren(TArray<class UWB_Container_Linear_C*>& Array);
    bool IsFillTypeFillFromCenter();
    double GetInterpolationTime();
    FVector2D FindThicknessSize(double Thickness);
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
    void PB_SetFillColorBrushTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void PB_SetFillColorMask(class UObject* Mask);
    void PB_SetBackgroundColorMask(class UTexture2D* Mask);
    void PB_SetBackgroundBrushTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void PB_SetBackgroundBlurStrength(double BlurStrength);
    void PB_SetFillColorGradientType(TEnumAsByte<EGradientTypes::Type> GradientType);
    void PB_SetBlendMask(class UTexture2D* BlendMask);
    void PB_SetFillType(TEnumAsByte<EProgressBarFillType::Type> FillType);
    void PB_SetFillFromCenterSpacing(double Spacing);
    void PB_SetIsCustomMarquee(bool IsMarquee);
    void PB_SetIsMarquee(bool IsMarquee);
    void PB_SetCustomMarqueeImage(class UTexture2D* Image);
    void PB_SetCustomMarqueeMaskType(TEnumAsByte<EMarqueeMask::Type> MaskType);
    void PB_SetMarqueeImage(class UTexture2D* Image);
    void PB_SetSeparationSteps(int32 Steps);
    void PB_SetSeparationStepsSpacing(double Spacing);
    void PB_SetSeparationAbsoluteFill(bool AbsoluteFillMethod);
    void PB_SetEffects(const TArray<FS_Effects>& Effects);
    void PB_SetEffectEnabled(int32 Index, bool IsEnabled);
    void PB_SetAllEffectsEnabled(bool IsEnabled);
    void PB_RemoveEffect(int32 Index);
    void UpdatePercent(double Percent);
    void UpdateInterpolation();
    void SetFillColor(FLinearColor Color, double GradientPower, TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetSize(FVector2D Size);
    void SetUseGradient(bool UseGradient);
    void SetTargetPercent(bool UseTargetPercent, double TargetPercent);
    void SetTargetFillColorPositive(FLinearColor Color);
    void SetTargetFillColorNegative(FLinearColor Color);
    void SetBackgroundColor(class UTexture2D* BackgroundColorMask, TEnumAsByte<ESlateBrushTileType::Type> In Brush Tiling, FLinearColor Color);
    void SetProgressMethod(TEnumAsByte<EProgressMethod::Type> ProgressMethod);
    void SetPercentInterpolated(double NewPercent);
    void SetFillColorMask(class UObject* Value);
    void SetGradientMask(TEnumAsByte<EGradientTypes::Type> GradientType);
    void SetBackgroundColorMask(class UTexture2D* Brush, TEnumAsByte<ESlateBrushTileType::Type> In Brush Tiling);
    void SetOverallBlendMask(class UTexture2D* BlendMask);
    void UpdateCustomMarquee(bool bIsMarquee, TEnumAsByte<EMarqueeMethod::Type> MarqueeMethod);
    void UpdateCustomMarqueeSpeed(double Speed);
    void UpdateCustomMarqueeMask(TEnumAsByte<EMarqueeMask::Type> MaskType, class UTexture2D* Mask, TEnumAsByte<ESlateBrushTileType::Type> MaskTiling);
    void SetDefaultMarquee_IsMarquee(bool IsMarquee);
    void SetDefaultMarquee_Image(class UTexture2D* Image);
    void SetDefaultMarquee_ImageSize(FVector2D Image Size);
    void SetDefaultMarquee_Tint(FLinearColor Tint);
    void SetDefaultMarquee_DrawAs(TEnumAsByte<ESlateBrushDrawType::Type> Draw As);
    void SetDefaultMarquee_Tiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void UpdateCustomMarqueeColor(FLinearColor InColorAndOpacity);
    void Separation_AddSegments(int32 NumSegments, double Spacing, FVector2D Size, FLinearColor FillColor, TEnumAsByte<EProgressBarFillType::Type> FillType);
    void Separation_AddBackground(FLinearColor Color);
    void Separation_SetUseAbsoluteFillValue(bool bAbsoluteFill);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> FillType, bool bUseShader);
    void SetCurrentCustomMarqueePercent();
    void CreateEffects();
    void AddEffect(FS_Effects Effect);
    void Reset();
    void SetBackgroundBlur(bool UseBackgroundBlur, double BackgroundBlurStrength);
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
    void ExecuteUbergraph_ProgressBarLinear(int32 EntryPoint);
    void OnPercentChanged__DelegateSignature(double Percent);
}; // Size: 0x6CD

#endif
