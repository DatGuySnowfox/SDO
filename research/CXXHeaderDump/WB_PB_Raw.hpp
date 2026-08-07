#ifndef UE4SS_SDK_WB_PB_Raw_HPP
#define UE4SS_SDK_WB_PB_Raw_HPP

class UWB_PB_Raw_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWB_BaseProgressBar_C* WB_BaseProgressBar;                                  // 0x02C8 (size: 0x8)

    void SetMarqueeTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetMarqueeDrawAs(TEnumAsByte<ESlateBrushDrawType::Type> DrawAs);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> BarFillType, bool bUseShader);
    void SetMarqueeTint(FLinearColor Tint);
    void SetFillColorAndOpacity(FLinearColor InColor);
    void SetMarqueeImageSize(FVector2D ImageSize);
    void SetBackgroundTint(FLinearColor InColor);
    void SetMarqueeImage(class UObject* Image);
    void SetFillImage(class UObject* FillImage);
    void SetMarquee(bool IsMarquee);
    void SetFillImageSize(FVector2D FillImageSize);
    void SetColor(FLinearColor InColor);
    void SetFillImageTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetPercent(double InPercent);
    void SetFillImageMargin(double Margin);
    void SetFillImageDrawAs(TEnumAsByte<ESlateBrushDrawType::Type> Draw As);
    void ExecuteUbergraph_WB_PB_Raw(int32 EntryPoint);
}; // Size: 0x2D0

#endif
