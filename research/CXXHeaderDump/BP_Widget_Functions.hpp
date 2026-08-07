#ifndef UE4SS_SDK_BP_Widget_Functions_HPP
#define UE4SS_SDK_BP_Widget_Functions_HPP

class UBP_Widget_Functions_C : public UBlueprintFunctionLibrary
{

    double RandomizePositiveNegativeFloat(double Value, class UObject* __WorldContext);
    void SetBorderBrushTiling(class UBorder* Target, TEnumAsByte<ESlateBrushTileType::Type> Tiling, class UObject* __WorldContext);
    void SetBorderBrushSize(class UBorder* Target, FVector2D Size, class UObject* __WorldContext);
    void SetBorderBrush(class UBorder* Target, class UObject* Brush, class UObject* __WorldContext);
    void SetImageBrushSize(class UImage* Target, FVector2D Size, class UObject* __WorldContext);
    void SetImageBrushTiling(class UImage* Target, TEnumAsByte<ESlateBrushTileType::Type> Tiling, class UObject* __WorldContext);
    void SetImageBrush(class UImage* Target, class UObject* Brush, class UObject* __WorldContext);
    void SetFillImageDrawAs(class UProgressBar* Target, TEnumAsByte<ESlateBrushDrawType::Type> DrawAs, class UObject* __WorldContext);
    void SetFillImageMargin(class UProgressBar* Target, double Margin, class UObject* __WorldContext);
    void SetBackgroundTint(class UProgressBar* Target, FSlateColor Tint, class UObject* __WorldContext);
    void SetMarqueeMirroring(class UProgressBar* Target, TEnumAsByte<ESlateBrushMirrorType::Type> Mirroring, class UObject* __WorldContext);
    void SetMarqueeTiling(class UProgressBar* Target, TEnumAsByte<ESlateBrushTileType::Type> Tiling, class UObject* __WorldContext);
    void SetMarqueeDrawAs(class UProgressBar* Target, TEnumAsByte<ESlateBrushDrawType::Type> Draw As, class UObject* __WorldContext);
    void SetMarqueeTint(class UProgressBar* Target, FLinearColor Tint, class UObject* __WorldContext);
    void SetMarqueeMargin(class UProgressBar* Target, FMargin Margin, class UObject* __WorldContext);
    void SetMarqueeImageSize(class UProgressBar* Target, FVector2D Image Size, class UObject* __WorldContext);
    void SetMarqueeImage(class UProgressBar* Target, class UObject* Image, class UObject* __WorldContext);
    void GetFillImage(class UProgressBar* Target, class UObject* __WorldContext, class UObject*& FillImage);
    void SetFillImageTiling(class UProgressBar* Target, TEnumAsByte<ESlateBrushTileType::Type> Tiling, class UObject* __WorldContext, FProgressBarStyle& Output_Get);
    void SetFillImageSize(class UProgressBar* Target, FVector2D FillImageSize, class UObject* __WorldContext);
    void SetFillImage(class UProgressBar* Target, class UObject* FillImage, class UObject* __WorldContext);
    FLinearColor SubtractBrightnessValue(const FLinearColor InColor, double Value, class UObject* __WorldContext);
    FLinearColor SubtractFromColor(const FLinearColor InColor, double Value, class UObject* __WorldContext);
    void GetSize(class USizeBox* SizeBox, class UObject* __WorldContext, double& Width, double& Height);
    void SetSize(class USizeBox* SizeBox, double Width, double Height, class UObject* __WorldContext);
    FVector2D GetTexture2DSizeClamped(class UTexture2D* Texture2D, FVector2D Clamp, class UObject* __WorldContext);
    FVector2D GetTexture2DSize(class UTexture2D* Texture2D, class UObject* __WorldContext);
}; // Size: 0x28

#endif
