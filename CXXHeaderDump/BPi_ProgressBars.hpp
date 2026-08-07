#ifndef UE4SS_SDK_BPi_ProgressBars_HPP
#define UE4SS_SDK_BPi_ProgressBars_HPP

class IBPi_ProgressBars_C : public IInterface
{

    void PB_GetSeparationSteps(int32& Steps);
    void PB_RemoveEffect(int32 Index);
    void PB_AddEffect(FS_Effects Effect, int32& Index);
    void PB_SetAllEffectsEnabled(bool IsEnabled);
    void PB_SetEffectEnabled(int32 Index, bool IsEnabled);
    void PB_GetEffects(TArray<FS_Effects>& Effects);
    void PB_SetEffects(const TArray<FS_Effects>& Effects);
    void PB_SetSeparationAbsoluteFill(bool AbsoluteFillMethod);
    void PB_SetSeparationStepsSpacing(double Spacing);
    void PB_SetSeparationSteps(int32 Steps);
    void PB_SetCustomMarqueeMaskType(TEnumAsByte<EMarqueeMask::Type> MaskType);
    void PB_SetMarqueeImage(class UTexture2D* Image);
    void PB_SetCustomMarqueeImage(class UTexture2D* Image);
    void PB_GetIsMarquee(bool& IsMarquee);
    void PB_SetIsMarquee(bool IsMarquee);
    void PB_GetIsCustomMarquee(bool& IsMarquee);
    void PB_SetIsCustomMarquee(bool IsMarquee);
    void PB_SetFillFromCenterSpacing(double Spacing);
    void PB_SetFillType(TEnumAsByte<EProgressBarFillType::Type> FillType);
    void PB_SetBlendMask(class UTexture2D* BlendMask);
    void PB_SetFillColorGradientType(TEnumAsByte<EGradientTypes::Type> GradientType);
    void PB_SetBackgroundBlurStrength(double BlurStrength);
    void PB_SetBackgroundBrushTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void PB_SetBackgroundColorMask(class UTexture2D* Mask);
    void PB_SetFillColorMask(class UObject* Mask);
    void PB_SetFillColorBrushTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void PB_SetFillColorGradientPower(double GradientPower);
    void PB_GetBackgroundColor(FLinearColor& Color);
    void PB_SetBackgroundColor(FLinearColor Color);
    void PB_GetUseGradientFillColor(bool& UseGradientFillColor);
    void PB_SetUseGradientFillColor(bool UseGradientFillColor);
    void PB_GetTargetFillColor_Negative(FLinearColor& Color);
    void PB_GetTargetFillColor_Positive(FLinearColor& Color);
    void PB_SetTargetFillColor_Negative(FLinearColor Color);
    void PB_SetTargetFillColor_Positive(FLinearColor Color);
    void PB_GetInterpTimeTarget(double& TargetInterpTime);
    void PB_SetInterpTimeTarget(double TargetInterpTime);
    void PB_GetInterpTimeCurrent(double& CurrentInterpTime);
    void PB_SetInterpTimeCurrent(double CurrentInterpTime);
    void PB_GetProgressMethod(TEnumAsByte<EProgressMethod::Type>& EProgressMethod);
    void PB_SetProgressMethod(TEnumAsByte<EProgressMethod::Type> EProgressMethod);
    void PB_GetUseTargetPercent(bool& UseTargetPercent);
    void PB_SetUseTargetPercent(bool UseTargetPercent);
    void PB_GetThickness(double& Thickness);
    void PB_SetThickness(double Thickness);
    void PB_GetTargetPercent(double& TargetPercent);
    void PB_SetTargetPercent(double TargetPercent);
    void PB_GetFillColor(FLinearColor& Color);
    void PB_SetFillColor(FLinearColor Color);
    void PB_GetPercent(double& Percent);
    void PB_SetPercent(double Value);
    void PB_GetSize(FVector2D& Size);
    void PB_SetSize(FVector2D Size);
}; // Size: 0x28

#endif
