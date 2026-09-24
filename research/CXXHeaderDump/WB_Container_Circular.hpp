#ifndef UE4SS_SDK_WB_Container_Circular_HPP
#define UE4SS_SDK_WB_Container_Circular_HPP

class UWB_Container_Circular_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UWidgetSwitcher* WS_Marquee;                                                // 0x0348 (size: 0x8)
    class UImage* TargetCircle;                                                       // 0x0350 (size: 0x8)
    class USizeBox* SB_Content;                                                       // 0x0358 (size: 0x8)
    class UOverlay* OV_TargetCircle;                                                  // 0x0360 (size: 0x8)
    class UImage* MarqueeCircle;                                                      // 0x0368 (size: 0x8)
    class UImage* MarqueeBackground;                                                  // 0x0370 (size: 0x8)
    class UImage* Circle;                                                             // 0x0378 (size: 0x8)
    class UMaterialInstanceDynamic* CircleMaterial;                                   // 0x0380 (size: 0x8)
    double Thickness;                                                                 // 0x0388 (size: 0x8)
    double Density;                                                                   // 0x0390 (size: 0x8)
    int32 Steps;                                                                      // 0x0398 (size: 0x4)
    double Spacing;                                                                   // 0x03A0 (size: 0x8)
    double StepDensity;                                                               // 0x03A8 (size: 0x8)
    FLinearColor BackgroundColor;                                                     // 0x03B0 (size: 0x10)
    FLinearColor FillColor;                                                           // 0x03C0 (size: 0x10)
    double TargetPercent;                                                             // 0x03D0 (size: 0x8)
    double CurrentPercent;                                                            // 0x03D8 (size: 0x8)
    bool bAbsoluteFillMethod;                                                         // 0x03E0 (size: 0x1)
    bool bIsNegativeFillValue;                                                        // 0x03E1 (size: 0x1)
    TEnumAsByte<EProgressMethod::Type> ProgressMethod;                                // 0x03E2 (size: 0x1)
    bool bIsDesignTime;                                                               // 0x03E3 (size: 0x1)
    double DeltaTime;                                                                 // 0x03E8 (size: 0x8)
    FTimerHandle Timer;                                                               // 0x03F0 (size: 0x8)
    bool bUseTargetPercent;                                                           // 0x03F8 (size: 0x1)
    FLinearColor TargetFillColor_Positive;                                            // 0x03FC (size: 0x10)
    FLinearColor TargetFillColor_Negative;                                            // 0x040C (size: 0x10)
    FLinearColor GradientColor;                                                       // 0x041C (size: 0x10)
    double GradientOpacity;                                                           // 0x0430 (size: 0x8)
    bool bUseGradient;                                                                // 0x0438 (size: 0x1)
    class UTexture2D* FillColorMask;                                                  // 0x0440 (size: 0x8)
    class UTexture2D* BackgroundMask;                                                 // 0x0448 (size: 0x8)
    double BackgroundThickness;                                                       // 0x0450 (size: 0x8)
    bool bUseMarquee;                                                                 // 0x0458 (size: 0x1)
    FLinearColor MarqueeColor;                                                        // 0x045C (size: 0x10)
    FLinearColor MarqueeColorBG;                                                      // 0x046C (size: 0x10)
    FLinearColor ProgressChangeColor;                                                 // 0x047C (size: 0x10)
    bool bIsProgressChanging;                                                         // 0x048C (size: 0x1)
    bool bUseProgressChangeColor;                                                     // 0x048D (size: 0x1)
    bool bIsChanging;                                                                 // 0x048E (size: 0x1)
    FLinearColor LocalColor;                                                          // 0x0490 (size: 0x10)

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
}; // Size: 0x4A0

#endif
