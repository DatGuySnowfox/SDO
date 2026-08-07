#ifndef UE4SS_SDK_WB_Container_Linear_HPP
#define UE4SS_SDK_WB_Container_Linear_HPP

class UWB_Container_Linear_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWB_PB_Linear_Base_C* BaseProgressBar;                                      // 0x02C8 (size: 0x8)
    class UWB_Marquee_Linear_C* BaseProgressBar_Marquee;                              // 0x02D0 (size: 0x8)
    class UWB_Marquee_Linear_C* OnTop_Marquee;                                        // 0x02D8 (size: 0x8)
    class UOverlay* OV_BaseProgressBar_Marquee;                                       // 0x02E0 (size: 0x8)
    class UOverlay* OV_OnTop_Marquee;                                                 // 0x02E8 (size: 0x8)
    class UOverlay* OV_TargetProgressBar;                                             // 0x02F0 (size: 0x8)
    class UOverlay* OV_TargetProgressBar_Marquee;                                     // 0x02F8 (size: 0x8)
    class UProgressBar* PB_DefaultMarquee;                                            // 0x0300 (size: 0x8)
    class UWidgetSwitcher* Switcher;                                                  // 0x0308 (size: 0x8)
    class UWB_PB_Linear_Target_C* TargetProgressBar;                                  // 0x0310 (size: 0x8)
    class UWB_Marquee_Linear_C* TargetProgressBar_Marquee;                            // 0x0318 (size: 0x8)
    class UWB_LinearProgress_Separated_C* WB_LinearProgress_Separated;                // 0x0320 (size: 0x8)
    FVector2D Size;                                                                   // 0x0328 (size: 0x10)
    double CurrentPercent;                                                            // 0x0338 (size: 0x8)
    FLinearColor TargetFillColor_Positive;                                            // 0x0340 (size: 0x10)
    FLinearColor TargetFillColor_Negative;                                            // 0x0350 (size: 0x10)
    double TargetPercent;                                                             // 0x0360 (size: 0x8)
    bool bIsNegativeFillValue;                                                        // 0x0368 (size: 0x1)
    double DeltaTime;                                                                 // 0x0370 (size: 0x8)
    FTimerHandle Timer;                                                               // 0x0378 (size: 0x8)
    bool bUseTargetPercent;                                                           // 0x0380 (size: 0x1)
    bool bIsDesignTime;                                                               // 0x0381 (size: 0x1)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod;                                // 0x0382 (size: 0x1)
    bool bUseMarquee;                                                                 // 0x0383 (size: 0x1)
    TEnumAsByte<EMarqueeMethod::Type> MarqueeMethod;                                  // 0x0384 (size: 0x1)
    bool bUseSeparation;                                                              // 0x0385 (size: 0x1)
    FLinearColor FillColor;                                                           // 0x0388 (size: 0x10)
    FLinearColor ProgressChangeColor;                                                 // 0x0398 (size: 0x10)
    bool bUseProgressChangeColor;                                                     // 0x03A8 (size: 0x1)
    bool bIsProgressChanging;                                                         // 0x03A9 (size: 0x1)
    TEnumAsByte<EProgressBarFillType::Type> BarFillType;                              // 0x03AA (size: 0x1)

    void Set_Mirror_OV_OnTopMarquee(bool Mirror);
    void Set_Mirror_OV_TargetPB_Marquee(bool Mirror);
    void Set_Mirror_OV_BasePB_Marquee(bool Mirror);
    void Handle_SetUseSeparation(bool bUseSeparation);
    void Set_DefaultMarquee_Visibility(bool bVisible);
    void Set_MarqueeMask(TEnumAsByte<EMarqueeMask::Type> MaskType, class UTexture2D* CustomMask);
    void Hide_AllMarquees();
    void Interp_BasePB_Color(FLinearColor Target, double InterpSpeed, bool bIsChanging);
    void SetMirrorY_OV_Target(bool MirrorY);
    void SetMirrorX_OV_Target(bool MirrorX);
    void GetSizeY(double& Current Size Y);
    void GetPercent_Separated(double& Percent);
    double GetAbsoluteTargetPercent();
    double FindHighestPercentValue();
    bool IsMarqueeMethod(TEnumAsByte<EMarqueeMethod::Type> Method);
    class UWB_Marquee_Linear_C* FindMarquee();
    void UpdateStaticPercent();
    bool IsProgressMethodStatic();
    bool IsProgressMethodInterpolate();
    void GetTargetPercent(double& Percent);
    double FindTargetPercentValue();
    FLinearColor FindTargetFillColor();
    bool IsNegativeFillValue();
    void GetSizeX(double& Current Size X);
    void FindTargetProgressBarPosition(double Percent);
    void GetPercent(double& Percent);
    void SetPercent(double InPercent);
    void SetFillColor(FLinearColor InColor, double GradientPower, TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetSize(FVector2D Size);
    void SetUseGradient(bool UseGradient);
    void SetTargetPercent(double TargetPercent);
    void SetTargetFillColorPositive(FLinearColor Color);
    void SetTargetFillColorNegative(FLinearColor Color);
    void SetUseTargetPercent(bool UseTargetPercent);
    void UpdateTargetPercent();
    void UpdatePercent();
    void SetProgressMethod(TEnumAsByte<EProgressMethod::Type> ProgressMethod);
    void SetFillColorMask(class UObject* Value);
    void SetGradientMask(class UTexture2D* GradientTexture);
    void SetCustomMarqueeAppearance(bool bIsMarquee, TEnumAsByte<EMarqueeMethod::Type> MarqueeMethod);
    void SetCustomMarqueeSpeed(double Speed);
    void SetCustomMarqueeMask(TEnumAsByte<EMarqueeMask::Type> MaskType, class UTexture2D* CustomMask, TEnumAsByte<ESlateBrushTileType::Type> MaskTiling);
    void SetCustomMarqueePercent_Current();
    void SetCustomMarqueePercent_Target(double Percent);
    void SetCustomMarqueePercent_CurrentAndTarget();
    void SetDefaultMarquee_IsMarquee(bool IsMarquee);
    void SetDefaultMarquee_Image(class UTexture2D* Image);
    void SetDefaultMarquee_ImageSize(FVector2D Image Size);
    void SetDefaultMarquee_Tint(FLinearColor Color);
    void SetDefaultMarquee_DrawAs(TEnumAsByte<ESlateBrushDrawType::Type> Draw As);
    void SetDefaultMarquee_Tiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetCustomMarqueeColor(FLinearColor InColorAndOpacity);
    void SetUseSeparation(bool bUseSeparation);
    void AddSegments(int32 NumSegments, double Spacing, FVector2D Size, FLinearColor FillColor, TEnumAsByte<EProgressBarFillType::Type> FillType, bool bIsFillFromCenter, bool bUseShader);
    void AddBackground(FLinearColor Color, bool bUseBackgroundBlur, double BlurStrength);
    void UpdateSeparationPercent(double Percent);
    void SetUseAbsoluteFillMethod(bool bAbsoluteFill);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> FillType, bool bUseShader);
    void UpdateMarqueeFillType();
    void StartTriggerProgressChangeColor(FLinearColor ProgressChangeColor);
    void StopTriggerProgressChangeColor();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_WB_Container_Linear(int32 EntryPoint);
}; // Size: 0x3AB

#endif
