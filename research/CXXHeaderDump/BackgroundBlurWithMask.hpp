#ifndef UE4SS_SDK_BackgroundBlurWithMask_HPP
#define UE4SS_SDK_BackgroundBlurWithMask_HPP

#include "BackgroundBlurWithMask_enums.hpp"

struct FMaskMaterialSetting
{
    class UMaterialInterface* MaskMaterial;                                           // 0x0000 (size: 0x8)
    FIntPoint ToTextureSize;                                                          // 0x0008 (size: 0x8)
    TEnumAsByte<EMaskMaterialRedrawMethod> RedrawMethod;                              // 0x0010 (size: 0x1)

}; // Size: 0x18

class UBackgroundBlurWithMask : public UContentWidget
{
    FMargin Padding;                                                                  // 0x0188 (size: 0x10)
    TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;                            // 0x0198 (size: 0x1)
    TEnumAsByte<EVerticalAlignment> VerticalAlignment;                                // 0x0199 (size: 0x1)
    bool bApplyAlphaToBlur;                                                           // 0x019A (size: 0x1)
    float BlurStrength;                                                               // 0x019C (size: 0x4)
    bool bUseMaskMaterial;                                                            // 0x01A0 (size: 0x1)
    class UTexture2D* MaskTexture;                                                    // 0x01A8 (size: 0x8)
    FMaskMaterialSetting MaskMaterialSetting;                                         // 0x01B0 (size: 0x18)
    TEnumAsByte<EMaskTextureChannel> MaskTextureChannel;                              // 0x01C8 (size: 0x1)
    bool bOverrideAutoRadiusCalculation;                                              // 0x01C9 (size: 0x1)
    int32 BlurRadius;                                                                 // 0x01CC (size: 0x4)
    FSlateBrush LowQualityFallbackBrush;                                              // 0x01D0 (size: 0xB0)
    class UTextureRenderTarget2D* MaskMaterialRenderTarget;                           // 0x0290 (size: 0x8)

    void SetVerticalAlignment(TEnumAsByte<EVerticalAlignment> InVerticalAlignment);
    void SetUseMaskMaterial(bool bInUseMaskMaterial);
    void SetPadding(FMargin InPadding);
    void SetMaskTextureChannel(TEnumAsByte<EMaskTextureChannel> InMaskTextureChannel);
    void SetMaskTexture(class UTexture2D* InMaskTexture);
    void SetMaskMaterialSetting(const FMaskMaterialSetting& InMaskMaterialSetting);
    void SetLowQualityFallbackBrush(const FSlateBrush& InBrush);
    void SetHorizontalAlignment(TEnumAsByte<EHorizontalAlignment> InHorizontalAlignment);
    void SetBlurStrength(float InStrength);
    void SetBlurRadius(int32 InBlurRadius);
    void SetApplyAlphaToBlur(bool bInApplyAlphaToBlur);
    void RedrawMaskMaterial();
    class UTexture* GetMaskTextureInUse();
}; // Size: 0x2A0

class UBackgroundBlurWithMaskSlot : public UPanelSlot
{
    FMargin Padding;                                                                  // 0x0038 (size: 0x10)
    TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;                            // 0x0048 (size: 0x1)
    TEnumAsByte<EVerticalAlignment> VerticalAlignment;                                // 0x0049 (size: 0x1)

    void SetVerticalAlignment(TEnumAsByte<EVerticalAlignment> InVerticalAlignment);
    void SetPadding(FMargin InPadding);
    void SetHorizontalAlignment(TEnumAsByte<EHorizontalAlignment> InHorizontalAlignment);
}; // Size: 0x60

#endif
