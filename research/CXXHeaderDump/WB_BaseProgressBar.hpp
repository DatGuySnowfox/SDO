#ifndef UE4SS_SDK_WB_BaseProgressBar_HPP
#define UE4SS_SDK_WB_BaseProgressBar_HPP

class UWB_BaseProgressBar_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UWidgetSwitcher* WS_Marquee;                                                // 0x0348 (size: 0x8)
    class UProgressBar* TopToBottom;                                                  // 0x0350 (size: 0x8)
    class UProgressBar* RightToLeft;                                                  // 0x0358 (size: 0x8)
    class UProgressBar* LeftToRight;                                                  // 0x0360 (size: 0x8)
    class UImage* Img_TopToBottom;                                                    // 0x0368 (size: 0x8)
    class UImage* Img_RightToLeft;                                                    // 0x0370 (size: 0x8)
    class UImage* Img_LeftToRight;                                                    // 0x0378 (size: 0x8)
    class UWidgetSwitcher* Img_FillTypes;                                             // 0x0380 (size: 0x8)
    class UHorizontalBox* Img_FillFromCenter;                                         // 0x0388 (size: 0x8)
    class UImage* Img_BottomToTop;                                                    // 0x0390 (size: 0x8)
    class UWidgetSwitcher* FillTypes;                                                 // 0x0398 (size: 0x8)
    class UProgressBar* FillFromCenter;                                               // 0x03A0 (size: 0x8)
    class UImage* Center_Right;                                                       // 0x03A8 (size: 0x8)
    class UImage* Center_Left;                                                        // 0x03B0 (size: 0x8)
    class UProgressBar* BottomToTop;                                                  // 0x03B8 (size: 0x8)
    class UBorder* B_FillTypes;                                                       // 0x03C0 (size: 0x8)
    TEnumAsByte<EProgressBarFillType::Type> FillType;                                 // 0x03C8 (size: 0x1)
    double Percent;                                                                   // 0x03D0 (size: 0x8)
    bool bUseShader;                                                                  // 0x03D8 (size: 0x1)

    void SetPBsEnabled(bool bInIsEnabled);
    void SetImgsEnabled(bool bInIsEnabled);
    void SetInstanceBlendMask(class UImage* Img, class UTexture* BlendMask);
    void SetImageBlendMask(class UObject* BlendMask);
    void SetTextureParameter(class UImage* Img, FName ParameterName, class UTexture* Value);
    void SetInstancePercent(class UImage* Img, double Percent);
    void SetScalarParameter(class UImage* Img, FName ParameterName, double Value);
    void SetImagePercent(double Percent);
    void GetPercent(double& Percent);
    void GetCurrentProgressBar(class UProgressBar*& AsProgress Bar);
    void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> BarFillType, bool bUseShader);
    void SetPercent(double InPercent);
    void SetFillColorAndOpacity(FLinearColor InColor);
    void SetFillImageTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetFillImageSize(FVector2D FillImageSize);
    void SetFillImage(class UObject* FillImage);
    void SetIsMarquee(bool InbIsMarquee);
    void SetMarqueeImage(class UObject* Image);
    void SetMarqueeImageSize(FVector2D Image Size);
    void SetMarqueeTint(FLinearColor Color);
    void SetMarqueeDrawAs(TEnumAsByte<ESlateBrushDrawType::Type> Draw As);
    void SetMarqueeTiling(TEnumAsByte<ESlateBrushTileType::Type> Tiling);
    void SetBackgroundTint(FLinearColor Tint);
    void SetFillImageMargin(double Margin);
    void SetFillImageDrawAs(TEnumAsByte<ESlateBrushDrawType::Type> Draw As);
    void ExecuteUbergraph_WB_BaseProgressBar(int32 EntryPoint);
}; // Size: 0x3D9

#endif
