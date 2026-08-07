#ifndef UE4SS_SDK_AudioWidgets_HPP
#define UE4SS_SDK_AudioWidgets_HPP

#include "AudioWidgets_enums.hpp"

struct FAudioMeterStyle : public FSlateWidgetStyle
{
    FSlateBrush MeterValueImage;                                                      // 0x0010 (size: 0xD0)
    FSlateBrush BackgroundImage;                                                      // 0x00E0 (size: 0xD0)
    FSlateBrush MeterBackgroundImage;                                                 // 0x01B0 (size: 0xD0)
    FSlateBrush MeterValueBackgroundImage;                                            // 0x0280 (size: 0xD0)
    FSlateBrush MeterPeakImage;                                                       // 0x0350 (size: 0xD0)
    FVector2D MeterSize;                                                              // 0x0420 (size: 0x10)
    FVector2D MeterPadding;                                                           // 0x0430 (size: 0x10)
    float MeterValuePadding;                                                          // 0x0440 (size: 0x4)
    float PeakValueWidth;                                                             // 0x0444 (size: 0x4)
    FVector2D ValueRangeDb;                                                           // 0x0448 (size: 0x10)
    bool bShowScale;                                                                  // 0x0458 (size: 0x1)
    bool bScaleSide;                                                                  // 0x0459 (size: 0x1)
    float ScaleHashOffset;                                                            // 0x045C (size: 0x4)
    float ScaleHashWidth;                                                             // 0x0460 (size: 0x4)
    float ScaleHashHeight;                                                            // 0x0464 (size: 0x4)
    int32 DecibelsPerHash;                                                            // 0x0468 (size: 0x4)
    FSlateFontInfo Font;                                                              // 0x0470 (size: 0x58)

}; // Size: 0x4D0

struct FAudioRadialSliderStyle : public FSlateWidgetStyle
{
    FAudioTextBoxStyle TextBoxStyle;                                                  // 0x0010 (size: 0x100)
    FSlateColor CenterBackgroundColor;                                                // 0x0110 (size: 0x14)
    FSlateColor SliderBarColor;                                                       // 0x0124 (size: 0x14)
    FSlateColor SliderProgressColor;                                                  // 0x0138 (size: 0x14)
    float LabelPadding;                                                               // 0x014C (size: 0x4)
    float DefaultSliderRadius;                                                        // 0x0150 (size: 0x4)

}; // Size: 0x160

struct FAudioSliderStyle : public FSlateWidgetStyle
{
    FSliderStyle SliderStyle;                                                         // 0x0010 (size: 0x500)
    FAudioTextBoxStyle TextBoxStyle;                                                  // 0x0510 (size: 0x100)
    FSlateBrush WidgetBackgroundImage;                                                // 0x0610 (size: 0xD0)
    FSlateColor SliderBackgroundColor;                                                // 0x06E0 (size: 0x14)
    FVector2D SliderBackgroundSize;                                                   // 0x06F8 (size: 0x10)
    float LabelPadding;                                                               // 0x0708 (size: 0x4)
    FSlateColor SliderBarColor;                                                       // 0x070C (size: 0x14)
    FSlateColor SliderThumbColor;                                                     // 0x0720 (size: 0x14)
    FSlateColor WidgetBackgroundColor;                                                // 0x0734 (size: 0x14)

}; // Size: 0x750

struct FAudioTextBoxStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundImage;                                                      // 0x0010 (size: 0xD0)
    FSlateColor BackgroundColor;                                                      // 0x00E0 (size: 0x14)

}; // Size: 0x100

struct FFixedSampleSequenceRulerStyle : public FSlateWidgetStyle
{
    float HandleWidth;                                                                // 0x0008 (size: 0x4)
    FSlateColor HandleColor;                                                          // 0x000C (size: 0x14)
    FSlateBrush HandleBrush;                                                          // 0x0020 (size: 0xD0)
    FSlateColor TicksColor;                                                           // 0x00F0 (size: 0x14)
    FSlateColor TicksTextColor;                                                       // 0x0104 (size: 0x14)
    FSlateFontInfo TicksTextFont;                                                     // 0x0118 (size: 0x58)
    float TicksTextOffset;                                                            // 0x0170 (size: 0x4)
    FSlateColor BackgroundColor;                                                      // 0x0174 (size: 0x14)
    FSlateBrush BackgroundBrush;                                                      // 0x0190 (size: 0xD0)
    float DesiredWidth;                                                               // 0x0260 (size: 0x4)
    float DesiredHeight;                                                              // 0x0264 (size: 0x4)

}; // Size: 0x270

struct FMeterChannelInfo
{
    float MeterValue;                                                                 // 0x0000 (size: 0x4)
    float PeakValue;                                                                  // 0x0004 (size: 0x4)
    float ClippingValue;                                                              // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FPlayheadOverlayStyle : public FSlateWidgetStyle
{
    FSlateColor PlayheadColor;                                                        // 0x0008 (size: 0x14)
    float PlayheadWidth;                                                              // 0x001C (size: 0x4)
    float DesiredWidth;                                                               // 0x0020 (size: 0x4)
    float DesiredHeight;                                                              // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FSampledSequenceValueGridOverlayStyle : public FSlateWidgetStyle
{
    FSlateColor GridColor;                                                            // 0x0008 (size: 0x14)
    float GridThickness;                                                              // 0x001C (size: 0x4)
    FSlateColor LabelTextColor;                                                       // 0x0020 (size: 0x14)
    FSlateFontInfo LabelTextFont;                                                     // 0x0038 (size: 0x58)
    float DesiredWidth;                                                               // 0x0090 (size: 0x4)
    float DesiredHeight;                                                              // 0x0094 (size: 0x4)

}; // Size: 0x98

struct FSampledSequenceViewerStyle : public FSlateWidgetStyle
{
    FSlateColor SequenceColor;                                                        // 0x0008 (size: 0x14)
    float SequenceLineThickness;                                                      // 0x001C (size: 0x4)
    FSlateColor MajorGridLineColor;                                                   // 0x0020 (size: 0x14)
    FSlateColor MinorGridLineColor;                                                   // 0x0034 (size: 0x14)
    FSlateColor ZeroCrossingLineColor;                                                // 0x0048 (size: 0x14)
    float ZeroCrossingLineThickness;                                                  // 0x005C (size: 0x4)
    float SampleMarkersSize;                                                          // 0x0060 (size: 0x4)
    FSlateColor SequenceBackgroundColor;                                              // 0x0064 (size: 0x14)
    FSlateBrush BackgroundBrush;                                                      // 0x0080 (size: 0xD0)
    float DesiredWidth;                                                               // 0x0150 (size: 0x4)
    float DesiredHeight;                                                              // 0x0154 (size: 0x4)

}; // Size: 0x160

class UAudioFrequencyRadialSlider : public UAudioRadialSlider
{
}; // Size: 0x3A0

class UAudioFrequencySlider : public UAudioSliderBase
{
    FVector2D OutputRange;                                                            // 0x09D0 (size: 0x10)

}; // Size: 0x9E0

class UAudioMeter : public UWidget
{
    TArray<FMeterChannelInfo> MeterChannelInfo;                                       // 0x0180 (size: 0x10)
    FAudioMeterMeterChannelInfoDelegate MeterChannelInfoDelegate;                     // 0x0190 (size: 0x10)
    TArray<FMeterChannelInfo> GetMeterChannelInfo();
    FAudioMeterStyle WidgetStyle;                                                     // 0x01A0 (size: 0x4D0)
    TEnumAsByte<EOrientation> Orientation;                                            // 0x0670 (size: 0x1)
    FLinearColor BackgroundColor;                                                     // 0x0674 (size: 0x10)
    FLinearColor MeterBackgroundColor;                                                // 0x0684 (size: 0x10)
    FLinearColor MeterValueColor;                                                     // 0x0694 (size: 0x10)
    FLinearColor MeterPeakColor;                                                      // 0x06A4 (size: 0x10)
    FLinearColor MeterClippingColor;                                                  // 0x06B4 (size: 0x10)
    FLinearColor MeterScaleColor;                                                     // 0x06C4 (size: 0x10)
    FLinearColor MeterScaleLabelColor;                                                // 0x06D4 (size: 0x10)

    void SetMeterValueColor(FLinearColor InValue);
    void SetMeterScaleLabelColor(FLinearColor InValue);
    void SetMeterScaleColor(FLinearColor InValue);
    void SetMeterPeakColor(FLinearColor InValue);
    void SetMeterClippingColor(FLinearColor InValue);
    void SetMeterChannelInfo(const TArray<FMeterChannelInfo>& InMeterChannelInfo);
    void SetMeterBackgroundColor(FLinearColor InValue);
    void SetBackgroundColor(FLinearColor InValue);
    TArray<FMeterChannelInfo> GetMeterChannelInfo__DelegateSignature();
    TArray<FMeterChannelInfo> GetMeterChannelInfo();
}; // Size: 0x700

class UAudioRadialSlider : public UWidget
{
    float Value;                                                                      // 0x0180 (size: 0x4)
    FAudioRadialSliderValueDelegate ValueDelegate;                                    // 0x0184 (size: 0x10)
    float GetFloat();
    TEnumAsByte<EAudioRadialSliderLayout> WidgetLayout;                               // 0x0194 (size: 0x1)
    FLinearColor CenterBackgroundColor;                                               // 0x0198 (size: 0x10)
    FLinearColor SliderProgressColor;                                                 // 0x01A8 (size: 0x10)
    FLinearColor SliderBarColor;                                                      // 0x01B8 (size: 0x10)
    FVector2D HandStartEndRatio;                                                      // 0x01C8 (size: 0x10)
    FText UnitsText;                                                                  // 0x01D8 (size: 0x18)
    FLinearColor TextLabelBackgroundColor;                                            // 0x01F0 (size: 0x10)
    bool ShowLabelOnlyOnHover;                                                        // 0x0200 (size: 0x1)
    bool ShowUnitsText;                                                               // 0x0201 (size: 0x1)
    bool IsUnitsTextReadOnly;                                                         // 0x0202 (size: 0x1)
    bool IsValueTextReadOnly;                                                         // 0x0203 (size: 0x1)
    float SliderThickness;                                                            // 0x0204 (size: 0x4)
    FVector2D OutputRange;                                                            // 0x0208 (size: 0x10)
    FAudioRadialSliderOnValueChanged OnValueChanged;                                  // 0x0218 (size: 0x10)
    void OnAudioRadialSliderValueChangedEvent(float Value);

    void SetWidgetLayout(TEnumAsByte<EAudioRadialSliderLayout> InLayout);
    void SetValueTextReadOnly(const bool bIsReadOnly);
    void SetUnitsTextReadOnly(const bool bIsReadOnly);
    void SetUnitsText(const FText Units);
    void SetTextLabelBackgroundColor(FSlateColor InColor);
    void SetSliderThickness(const float InThickness);
    void SetSliderProgressColor(FLinearColor InValue);
    void SetSliderBarColor(FLinearColor InValue);
    void SetShowUnitsText(const bool bShowUnitsText);
    void SetShowLabelOnlyOnHover(const bool bShowLabelOnlyOnHover);
    void SetOutputRange(const FVector2D InOutputRange);
    void SetHandStartEndRatio(const FVector2D InHandStartEndRatio);
    void SetCenterBackgroundColor(FLinearColor InValue);
    float GetSliderValue(const float OutputValue);
    float GetOutputValue(const float InSliderValue);
}; // Size: 0x3A0

class UAudioSlider : public UAudioSliderBase
{
    TWeakObjectPtr<class UCurveFloat> LinToOutputCurve;                               // 0x09D0 (size: 0x8)
    TWeakObjectPtr<class UCurveFloat> OutputToLinCurve;                               // 0x09D8 (size: 0x8)

}; // Size: 0x9E0

class UAudioSliderBase : public UWidget
{
    float Value;                                                                      // 0x0180 (size: 0x4)
    FText UnitsText;                                                                  // 0x0188 (size: 0x18)
    FLinearColor TextLabelBackgroundColor;                                            // 0x01A0 (size: 0x10)
    FAudioSliderBaseTextLabelBackgroundColorDelegate TextLabelBackgroundColorDelegate; // 0x01B0 (size: 0x10)
    FLinearColor GetLinearColor();
    bool ShowLabelOnlyOnHover;                                                        // 0x01C0 (size: 0x1)
    bool ShowUnitsText;                                                               // 0x01C1 (size: 0x1)
    bool IsUnitsTextReadOnly;                                                         // 0x01C2 (size: 0x1)
    bool IsValueTextReadOnly;                                                         // 0x01C3 (size: 0x1)
    FAudioSliderBaseValueDelegate ValueDelegate;                                      // 0x01C4 (size: 0x10)
    float GetFloat();
    FLinearColor SliderBackgroundColor;                                               // 0x01D4 (size: 0x10)
    FAudioSliderBaseSliderBackgroundColorDelegate SliderBackgroundColorDelegate;      // 0x01E4 (size: 0x10)
    FLinearColor GetLinearColor();
    FLinearColor SliderBarColor;                                                      // 0x01F4 (size: 0x10)
    FAudioSliderBaseSliderBarColorDelegate SliderBarColorDelegate;                    // 0x0204 (size: 0x10)
    FLinearColor GetLinearColor();
    FLinearColor SliderThumbColor;                                                    // 0x0214 (size: 0x10)
    FAudioSliderBaseSliderThumbColorDelegate SliderThumbColorDelegate;                // 0x0224 (size: 0x10)
    FLinearColor GetLinearColor();
    FLinearColor WidgetBackgroundColor;                                               // 0x0234 (size: 0x10)
    FAudioSliderBaseWidgetBackgroundColorDelegate WidgetBackgroundColorDelegate;      // 0x0244 (size: 0x10)
    FLinearColor GetLinearColor();
    TEnumAsByte<EOrientation> Orientation;                                            // 0x0254 (size: 0x1)
    FAudioSliderBaseOnValueChanged OnValueChanged;                                    // 0x0258 (size: 0x10)
    void OnFloatValueChangedEvent(float Value);

    void SetWidgetBackgroundColor(FLinearColor InValue);
    void SetValueTextReadOnly(const bool bIsReadOnly);
    void SetUnitsTextReadOnly(const bool bIsReadOnly);
    void SetUnitsText(const FText Units);
    void SetTextLabelBackgroundColor(FSlateColor InColor);
    void SetSliderThumbColor(FLinearColor InValue);
    void SetSliderBarColor(FLinearColor InValue);
    void SetSliderBackgroundColor(FLinearColor InValue);
    void SetShowUnitsText(const bool bShowUnitsText);
    void SetShowLabelOnlyOnHover(const bool bShowLabelOnlyOnHover);
    float GetSliderValue(const float OutputValue);
    float GetOutputValue(const float InSliderValue);
    float GetLinValue(const float OutputValue);
}; // Size: 0x9D0

class UAudioVolumeRadialSlider : public UAudioRadialSlider
{
}; // Size: 0x3A0

class UAudioVolumeSlider : public UAudioSlider
{
}; // Size: 0x9E0

#endif
