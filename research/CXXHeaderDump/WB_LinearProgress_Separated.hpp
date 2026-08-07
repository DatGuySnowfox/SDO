#ifndef UE4SS_SDK_WB_LinearProgress_Separated_HPP
#define UE4SS_SDK_WB_LinearProgress_Separated_HPP

class UWB_LinearProgress_Separated_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UHorizontalBox* HB_Backgrounds;                                             // 0x02C8 (size: 0x8)
    class UHorizontalBox* HB_Marquees;                                                // 0x02D0 (size: 0x8)
    class UHorizontalBox* HB_ProgressBars;                                            // 0x02D8 (size: 0x8)
    class UVerticalBox* VB_Backgrounds;                                               // 0x02E0 (size: 0x8)
    class UVerticalBox* VB_Marquees;                                                  // 0x02E8 (size: 0x8)
    class UVerticalBox* VB_ProgressBars;                                              // 0x02F0 (size: 0x8)
    TArray<class UWB_Image_Raw_C*> Segments_BG;                                       // 0x02F8 (size: 0x10)
    TArray<class UWB_PB_Raw_C*> Segments_PB;                                          // 0x0308 (size: 0x10)
    int32 NumSegments;                                                                // 0x0318 (size: 0x4)
    double Spacing;                                                                   // 0x0320 (size: 0x8)
    FVector2D Size;                                                                   // 0x0328 (size: 0x10)
    FLinearColor BackgroundColor;                                                     // 0x0338 (size: 0x10)
    double Percent;                                                                   // 0x0348 (size: 0x8)
    FLinearColor FillColor;                                                           // 0x0350 (size: 0x10)
    bool bAbsoluteFill;                                                               // 0x0360 (size: 0x1)
    TArray<class UWB_PB_Raw_C*> Segments_Marquees;                                    // 0x0368 (size: 0x10)
    TEnumAsByte<EProgressBarFillType::Type> FillType;                                 // 0x0378 (size: 0x1)
    bool bIsFillFromCenter;                                                           // 0x0379 (size: 0x1)
    bool bUseBackgroundBlur;                                                          // 0x037A (size: 0x1)
    double BlurStrength;                                                              // 0x0380 (size: 0x8)
    bool bUseShader;                                                                  // 0x0388 (size: 0x1)

    double FindMaxPaddingValue_Vertical(double Tolerance);
    double FindCurrentSegmentPercentValue();
    int32 FindPercentCurrentSegment();
    double GetSegmentSteps();
    double FindMaxPaddingValue_Horizontal(double Tolerance);
    void AddSegmentsProgressBar(int32 NumSegments, double Spacing, FVector2D Size, FLinearColor FillColor, TEnumAsByte<EProgressBarFillType::Type> FillType, bool bIsFillFromCenter, bool bUseShader);
    void ClearSegments_ProgressBar();
    void AddSegmentsBackground(FLinearColor Color, bool bUseBackgroundBlur, double BlurStrength);
    void ClearSegments_Background();
    void SetPercent(double Percent);
    void SetAbsoluteFillMethod(bool bAbsoluteFill);
    void SetUseMarquee(bool bSetUseMarquee);
    void ClearSegments_Marquee();
    void SetMarqueeImage(class UObject* Image);
    void SetMarqueeImageSize(FVector2D ImageSize);
    void SetMarqueeTint(FLinearColor Tint);
    void SetMarqueeDrawAs(TEnumAsByte<ESlateBrushDrawType::Type> DrawAs);
    void SetMarqueeTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void UpdateProgressChangeColor(FLinearColor NewColor, double InterpSpeed);
    void AddSegmentsMarquee();
    void ExecuteUbergraph_WB_LinearProgress_Separated(int32 EntryPoint);
}; // Size: 0x389

#endif
