#ifndef UE4SS_SDK_WB_Container_Circular_HPP
#define UE4SS_SDK_WB_Container_Circular_HPP

class UWB_Container_Circular_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Circle;                                                             // 0x02C8 (size: 0x8)
    class UImage* MarqueeBackground;                                                  // 0x02D0 (size: 0x8)
    class UImage* MarqueeCircle;                                                      // 0x02D8 (size: 0x8)
    class UOverlay* OV_TargetCircle;                                                  // 0x02E0 (size: 0x8)
    class USizeBox* SB_Content;                                                       // 0x02E8 (size: 0x8)
    class UImage* TargetCircle;                                                       // 0x02F0 (size: 0x8)
    class UWidgetSwitcher* WS_Marquee;                                                // 0x02F8 (size: 0x8)
    class UMaterialInstanceDynamic* CircleMaterial;                                   // 0x0300 (size: 0x8)
    double Thickness;                                                                 // 0x0308 (size: 0x8)
    double Density;                                                                   // 0x0310 (size: 0x8)
    int32 Steps;                                                                      // 0x0318 (size: 0x4)
    double Spacing;                                                                   // 0x0320 (size: 0x8)
    double StepDensity;                                                               // 0x0328 (size: 0x8)
    FLinearColor BackgroundColor;                                                     // 0x0330 (size: 0x10)
    FLinearColor FillColor;                                                           // 0x0340 (size: 0x10)
    double TargetPercent;                                                             // 0x0350 (size: 0x8)
    double CurrentPercent;                                                            // 0x0358 (size: 0x8)
    bool bAbsoluteFillMethod;                                                         // 0x0360 (size: 0x1)
    bool bIsNegativeFillValue;                                                        // 0x0361 (size: 0x1)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod;                                // 0x0362 (size: 0x1)
    bool bIsDesignTime;                                                               // 0x0363 (size: 0x1)
    double DeltaTime;                                                                 // 0x0368 (size: 0x8)
    FTimerHandle Timer;                                                               // 0x0370 (size: 0x8)
    bool bUseTargetPercent;                                                           // 0x0378 (size: 0x1)
    FLinearColor TargetFillColor_Positive;                                            // 0x037C (size: 0x10)
    FLinearColor TargetFillColor_Negative;                                            // 0x038C (size: 0x10)
    FLinearColor GradientColor;                                                       // 0x039C (size: 0x10)
    double GradientOpacity;                                                           // 0x03B0 (size: 0x8)
    bool bUseGradient;                                                                // 0x03B8 (size: 0x1)
    class UTexture2D* FillColorMask;                                                  // 0x03C0 (size: 0x8)
    class UTexture2D* BackgroundMask;                                                 // 0x03C8 (size: 0x8)
    double BackgroundThickness;                                                       // 0x03D0 (size: 0x8)
    bool bUseMarquee;                                                                 // 0x03D8 (size: 0x1)
    FLinearColor MarqueeColor;                                                        // 0x03DC (size: 0x10)
    FLinearColor MarqueeColorBG;                                                      // 0x03EC (size: 0x10)
    FLinearColor ProgressChangeColor;                                                 // 0x03FC (size: 0x10)
    bool bIsProgressChanging;                                                         // 0x040C (size: 0x1)
    bool bUseProgressChangeColor;                                                     // 0x040D (size: 0x1)
    bool bIsChanging;                                                                 // 0x040E (size: 0x1)
    FLinearColor LocalColor;                                                          // 0x0410 (size: 0x10)

    FLinearColor GetFillColor();
    double GetGradientOpacity();
    void SetMarqueeMaskParam(class UTexture* Value);
    void SetTimeParamMarquee(double Value);
    void SetMarqueeBGColorParam(FLinearColor Value);
    void SetMarqueeColorParam(FLinearColor Value);
    void SetStepDensityParamMarquee(double Value);
    void SetDensityParamMarquee(double Value);
    void SetSpacingParamMarquee(double Value);
    void SetStepsParamMarquee(int32 Value);
    void SetThicknessParamMarquee(double Value);
    class UMaterialInstanceDynamic* GetCircleMaterialMarqueeBG();
    class UMaterialInstanceDynamic* GetCircleMaterialMarquee();
    void SetBackgroundColorMaskParam(class UTexture* Value);
    void SetFillColorMaskParam(class UTexture* Value);
    void SetGradientColorParam(FLinearColor Value);
    void SetGradientMaskParam(class UTexture* Value);
    void SetGradientOpacityParam(double Value);
    void SetUseGradientParam(bool UseGradient);
    void SetDensityParamTarget(double Value);
    void SetThicknessParamTarget(double Value);
    void SetPercentParamTarget(double Value);
    double GetAbsoluteTargetPercent();
    void SetFillColorParamTarget(FLinearColor Value);
    class UMaterialInstanceDynamic* GetCircleMaterialTarget();
    FLinearColor FindTargetFillColor();
    void FindTargetProgressBarPosition(double Progress);
    bool IsProgressMethodStatic();
    void UpdateStaticPercent();
    bool IsProgressMethodInterpolate();
    bool IsNegativeFillValue();
    void SetAbsoluteFillMethod(bool bAbsoluteFillMethod);
    void GetTargetPercent(double& TargetPercent);
    void GetPercent(double& Percent);
    class UMaterialInstanceDynamic* GetCircleMaterial();
    void SetFillColorParam(FLinearColor Value);
    void SetEmptyColorParam(FLinearColor Value);
    void SetStepDensityParam(double Value);
    void SetSpacingParam(double Value);
    void SetStepsParam(int32 Value);
    void SetDensityParam(double Value);
    void SetThicknessParam(double Value);
    void SetPercentParam(double Value);
    void SetThickness(double Thickness);
    void SetTargetPercent(double TargetPercent);
    void SetSteps(int32 Steps);
    void SetPercent(double Percent);
    void SetSpacing(double Spacing);
    void SetSize(double Size);
    void SetDensity(double Density);
    void SetStepDensity(double HardStepBorder);
    void SetUseTargetPercent(bool bIsTargetPercent);
    void SetBackgroundColor(class UTexture2D* BackgroundMask, FLinearColor Color);
    void SetFillColor(FLinearColor FillColor, double GradientPower);
    void SetUseAbsoluteFillMethod(bool bAbsoluteFillMethod);
    void UpdateTargetPercent();
    void SetProgressMethod(TEnumAsByte<EProgressMethod::Type> ProgressMethod);
    void UpdatePercent();
    void SetTargetFillColor_Positive(FLinearColor FillColor);
    void SetTargetFillColor_Negative(FLinearColor FillColor);
    void SetGradientOpacity(double GradientOpacity);
    void SetGradientType(TEnumAsByte<EGradientTypes::Type> GradientType);
    void SetUseGradient(bool bUseGradient);
    void SetFillColorMask(class UTexture2D* Mask);
    void SetupMarquee(bool bUseMarquee, FLinearColor MarqueeColor, FLinearColor MarqueeBackgroundColor);
    void SetMarqueeTime(double Value);
    void SetMarqueeMask(TEnumAsByte<EMarqueeMask::Type> MarqueeMask, class UTexture2D* CustomMask);
    void StartTriggerProgressChangeColor(FLinearColor ProgressChangeColor);
    void StopTriggerProgressChangeColor();
    void UpdateProgressChangeColor(FLinearColor NewColor, double InterpSpeed, bool IsChanging);
    void PreConstruct(bool IsDesignTime);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_WB_Container_Circular(int32 EntryPoint);
}; // Size: 0x420

#endif
