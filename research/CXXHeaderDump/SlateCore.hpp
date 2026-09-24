#ifndef UE4SS_SDK_SlateCore_HPP
#define UE4SS_SDK_SlateCore_HPP

#include "SlateCore_enums.hpp"

struct FAnalogInputEvent : public FKeyEvent
{
}; // Size: 0x50

struct FButtonStyle : public FSlateWidgetStyle
{
    FSlateBrush Normal;                                                               // 0x0010 (size: 0xB0)
    FSlateBrush Hovered;                                                              // 0x00C0 (size: 0xB0)
    FSlateBrush Pressed;                                                              // 0x0170 (size: 0xB0)
    FSlateBrush Disabled;                                                             // 0x0220 (size: 0xB0)
    FSlateColor NormalForeground;                                                     // 0x02D0 (size: 0x14)
    FSlateColor HoveredForeground;                                                    // 0x02E4 (size: 0x14)
    FSlateColor PressedForeground;                                                    // 0x02F8 (size: 0x14)
    FSlateColor DisabledForeground;                                                   // 0x030C (size: 0x14)
    FMargin NormalPadding;                                                            // 0x0320 (size: 0x10)
    FMargin PressedPadding;                                                           // 0x0330 (size: 0x10)
    FSlateSound PressedSlateSound;                                                    // 0x0340 (size: 0x18)
    FSlateSound ClickedSlateSound;                                                    // 0x0358 (size: 0x18)
    FSlateSound HoveredSlateSound;                                                    // 0x0370 (size: 0x18)

}; // Size: 0x390

struct FCaptureLostEvent
{
}; // Size: 0x8

struct FCharacterEvent : public FInputEvent
{
}; // Size: 0x30

struct FCheckBoxStyle : public FSlateWidgetStyle
{
    TEnumAsByte<ESlateCheckBoxType::Type> CheckBoxType;                               // 0x0008 (size: 0x1)
    FSlateBrush UncheckedImage;                                                       // 0x0010 (size: 0xB0)
    FSlateBrush UncheckedHoveredImage;                                                // 0x00C0 (size: 0xB0)
    FSlateBrush UncheckedPressedImage;                                                // 0x0170 (size: 0xB0)
    FSlateBrush CheckedImage;                                                         // 0x0220 (size: 0xB0)
    FSlateBrush CheckedHoveredImage;                                                  // 0x02D0 (size: 0xB0)
    FSlateBrush CheckedPressedImage;                                                  // 0x0380 (size: 0xB0)
    FSlateBrush UndeterminedImage;                                                    // 0x0430 (size: 0xB0)
    FSlateBrush UndeterminedHoveredImage;                                             // 0x04E0 (size: 0xB0)
    FSlateBrush UndeterminedPressedImage;                                             // 0x0590 (size: 0xB0)
    FMargin Padding;                                                                  // 0x0640 (size: 0x10)
    FSlateBrush BackgroundImage;                                                      // 0x0650 (size: 0xB0)
    FSlateBrush BackgroundHoveredImage;                                               // 0x0700 (size: 0xB0)
    FSlateBrush BackgroundPressedImage;                                               // 0x07B0 (size: 0xB0)
    FSlateColor ForegroundColor;                                                      // 0x0860 (size: 0x14)
    FSlateColor HoveredForeground;                                                    // 0x0874 (size: 0x14)
    FSlateColor PressedForeground;                                                    // 0x0888 (size: 0x14)
    FSlateColor CheckedForeground;                                                    // 0x089C (size: 0x14)
    FSlateColor CheckedHoveredForeground;                                             // 0x08B0 (size: 0x14)
    FSlateColor CheckedPressedForeground;                                             // 0x08C4 (size: 0x14)
    FSlateColor UndeterminedForeground;                                               // 0x08D8 (size: 0x14)
    FSlateColor BorderBackgroundColor;                                                // 0x08EC (size: 0x14)
    FSlateSound CheckedSlateSound;                                                    // 0x0900 (size: 0x18)
    FSlateSound UncheckedSlateSound;                                                  // 0x0918 (size: 0x18)
    FSlateSound HoveredSlateSound;                                                    // 0x0930 (size: 0x18)

}; // Size: 0x950

struct FComboBoxStyle : public FSlateWidgetStyle
{
    FComboButtonStyle ComboButtonStyle;                                               // 0x0010 (size: 0x560)
    FSlateSound PressedSlateSound;                                                    // 0x0570 (size: 0x18)
    FSlateSound SelectionChangeSlateSound;                                            // 0x0588 (size: 0x18)
    FMargin ContentPadding;                                                           // 0x05A0 (size: 0x10)
    FMargin MenuRowPadding;                                                           // 0x05B0 (size: 0x10)

}; // Size: 0x5C0

struct FComboButtonStyle : public FSlateWidgetStyle
{
    FButtonStyle ButtonStyle;                                                         // 0x0010 (size: 0x390)
    FSlateBrush DownArrowImage;                                                       // 0x03A0 (size: 0xB0)
    FDeprecateSlateVector2D ShadowOffset;                                             // 0x0450 (size: 0x8)
    FLinearColor ShadowColorAndOpacity;                                               // 0x0458 (size: 0x10)
    FSlateBrush MenuBorderBrush;                                                      // 0x0470 (size: 0xB0)
    FMargin MenuBorderPadding;                                                        // 0x0520 (size: 0x10)
    FMargin ContentPadding;                                                           // 0x0530 (size: 0x10)
    FMargin DownArrowPadding;                                                         // 0x0540 (size: 0x10)
    TEnumAsByte<EVerticalAlignment> DownArrowAlign;                                   // 0x0550 (size: 0x1)

}; // Size: 0x560

struct FCompositeFallbackFont
{
    FTypeface Typeface;                                                               // 0x0000 (size: 0x10)
    float ScalingFactor;                                                              // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FCompositeFont
{
    FTypeface DefaultTypeface;                                                        // 0x0000 (size: 0x10)
    FCompositeFallbackFont FallbackTypeface;                                          // 0x0010 (size: 0x18)
    TArray<FCompositeSubFont> SubTypefaces;                                           // 0x0028 (size: 0x10)
    bool bEnableAscentDescentOverride;                                                // 0x0038 (size: 0x1)

}; // Size: 0x40

struct FCompositeSubFont : public FCompositeFallbackFont
{
    TArray<FInt32Range> CharacterRanges;                                              // 0x0018 (size: 0x10)
    FString Cultures;                                                                 // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FDeprecateSlateVector2D : public FVector2f
{
}; // Size: 0x8

struct FDockTabStyle : public FSlateWidgetStyle
{
    FButtonStyle CloseButtonStyle;                                                    // 0x0010 (size: 0x390)
    FSlateBrush NormalBrush;                                                          // 0x03A0 (size: 0xB0)
    FSlateBrush ColorOverlayTabBrush;                                                 // 0x0450 (size: 0xB0)
    FSlateBrush ColorOverlayIconBrush;                                                // 0x0500 (size: 0xB0)
    FSlateBrush ForegroundBrush;                                                      // 0x05B0 (size: 0xB0)
    FSlateBrush HoveredBrush;                                                         // 0x0660 (size: 0xB0)
    FSlateBrush ContentAreaBrush;                                                     // 0x0710 (size: 0xB0)
    FSlateBrush TabWellBrush;                                                         // 0x07C0 (size: 0xB0)
    FTextBlockStyle TabTextStyle;                                                     // 0x0870 (size: 0x2E0)
    FMargin TabPadding;                                                               // 0x0B50 (size: 0x10)
    FDeprecateSlateVector2D IconSize;                                                 // 0x0B60 (size: 0x8)
    float OverlapWidth;                                                               // 0x0B68 (size: 0x4)
    FSlateColor FlashColor;                                                           // 0x0B6C (size: 0x14)
    FSlateColor NormalForegroundColor;                                                // 0x0B80 (size: 0x14)
    FSlateColor HoveredForegroundColor;                                               // 0x0B94 (size: 0x14)
    FSlateColor ActiveForegroundColor;                                                // 0x0BA8 (size: 0x14)
    FSlateColor ForegroundForegroundColor;                                            // 0x0BBC (size: 0x14)
    float IconBorderPadding;                                                          // 0x0BD0 (size: 0x4)

}; // Size: 0xBE0

struct FEditableTextBoxStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundImageNormal;                                                // 0x0010 (size: 0xB0)
    FSlateBrush BackgroundImageHovered;                                               // 0x00C0 (size: 0xB0)
    FSlateBrush BackgroundImageFocused;                                               // 0x0170 (size: 0xB0)
    FSlateBrush BackgroundImageReadOnly;                                              // 0x0220 (size: 0xB0)
    FMargin Padding;                                                                  // 0x02D0 (size: 0x10)
    FTextBlockStyle TextStyle;                                                        // 0x02E0 (size: 0x2E0)
    FSlateColor ForegroundColor;                                                      // 0x05C0 (size: 0x14)
    FSlateColor BackgroundColor;                                                      // 0x05D4 (size: 0x14)
    FSlateColor ReadOnlyForegroundColor;                                              // 0x05E8 (size: 0x14)
    FSlateColor FocusedForegroundColor;                                               // 0x05FC (size: 0x14)
    FMargin HScrollBarPadding;                                                        // 0x0610 (size: 0x10)
    FMargin VScrollBarPadding;                                                        // 0x0620 (size: 0x10)
    FScrollBarStyle ScrollBarStyle;                                                   // 0x0630 (size: 0x650)

}; // Size: 0xC80

struct FEditableTextStyle : public FSlateWidgetStyle
{
    FSlateFontInfo Font;                                                              // 0x0008 (size: 0x60)
    FSlateColor ColorAndOpacity;                                                      // 0x0068 (size: 0x14)
    FSlateBrush BackgroundImageSelected;                                              // 0x0080 (size: 0xB0)
    FSlateBrush BackgroundImageComposing;                                             // 0x0130 (size: 0xB0)
    FSlateBrush CaretImage;                                                           // 0x01E0 (size: 0xB0)

}; // Size: 0x290

struct FExpandableAreaStyle : public FSlateWidgetStyle
{
    FSlateBrush CollapsedImage;                                                       // 0x0010 (size: 0xB0)
    FSlateBrush ExpandedImage;                                                        // 0x00C0 (size: 0xB0)
    float RolloutAnimationSeconds;                                                    // 0x0170 (size: 0x4)

}; // Size: 0x180

struct FFocusEvent
{
}; // Size: 0x8

struct FFontData
{
    FString FontFilename;                                                             // 0x0000 (size: 0x10)
    EFontHinting Hinting;                                                             // 0x0014 (size: 0x1)
    EFontLoadingPolicy LoadingPolicy;                                                 // 0x0015 (size: 0x1)
    int32 SubFaceIndex;                                                               // 0x0018 (size: 0x4)
    class UObject* FontFaceAsset;                                                     // 0x0020 (size: 0x8)

}; // Size: 0x28

struct FFontOutlineSettings
{
    int32 OutlineSize;                                                                // 0x0000 (size: 0x4)
    bool bMiteredCorners;                                                             // 0x0004 (size: 0x1)
    bool bSeparateFillAlpha;                                                          // 0x0005 (size: 0x1)
    bool bApplyOutlineToDropShadows;                                                  // 0x0006 (size: 0x1)
    class UObject* OutlineMaterial;                                                   // 0x0008 (size: 0x8)
    FLinearColor OutlineColor;                                                        // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FFontSdfSettings
{
    int32 BasePpem;                                                                   // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FGeometry
{
}; // Size: 0x38

struct FHeaderRowStyle : public FSlateWidgetStyle
{
    FTableColumnHeaderStyle ColumnStyle;                                              // 0x0010 (size: 0x640)
    FTableColumnHeaderStyle LastColumnStyle;                                          // 0x0650 (size: 0x640)
    FSplitterStyle ColumnSplitterStyle;                                               // 0x0C90 (size: 0x170)
    float SplitterHandleSize;                                                         // 0x0E00 (size: 0x4)
    FSlateBrush BackgroundBrush;                                                      // 0x0E10 (size: 0xB0)
    FSlateColor ForegroundColor;                                                      // 0x0EC0 (size: 0x14)
    FSlateBrush HorizontalSeparatorBrush;                                             // 0x0EE0 (size: 0xB0)
    float HorizontalSeparatorThickness;                                               // 0x0F90 (size: 0x4)

}; // Size: 0xFA0

struct FHyperlinkStyle : public FSlateWidgetStyle
{
    FButtonStyle UnderlineStyle;                                                      // 0x0010 (size: 0x390)
    FTextBlockStyle TextStyle;                                                        // 0x03A0 (size: 0x2E0)
    FMargin Padding;                                                                  // 0x0680 (size: 0x10)

}; // Size: 0x690

struct FInlineEditableTextBlockStyle : public FSlateWidgetStyle
{
    FEditableTextBoxStyle EditableTextBoxStyle;                                       // 0x0010 (size: 0xC80)
    FTextBlockStyle TextStyle;                                                        // 0x0C90 (size: 0x2E0)

}; // Size: 0xF70

struct FInlineTextImageStyle : public FSlateWidgetStyle
{
    FSlateBrush Image;                                                                // 0x0010 (size: 0xB0)
    int16 Baseline;                                                                   // 0x00C0 (size: 0x2)

}; // Size: 0xD0

struct FInputEvent
{
}; // Size: 0x28

struct FKeyEvent : public FInputEvent
{
}; // Size: 0x48

struct FMargin
{
    float Left;                                                                       // 0x0000 (size: 0x4)
    float Top;                                                                        // 0x0004 (size: 0x4)
    float Right;                                                                      // 0x0008 (size: 0x4)
    float Bottom;                                                                     // 0x000C (size: 0x4)

}; // Size: 0x10

struct FMotionEvent : public FInputEvent
{
}; // Size: 0x88

struct FNavigationEvent : public FInputEvent
{
}; // Size: 0x30

struct FPointerEvent : public FInputEvent
{
}; // Size: 0x80

struct FProgressBarStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundImage;                                                      // 0x0010 (size: 0xB0)
    FSlateBrush FillImage;                                                            // 0x00C0 (size: 0xB0)
    FSlateBrush MarqueeImage;                                                         // 0x0170 (size: 0xB0)
    bool EnableFillAnimation;                                                         // 0x0220 (size: 0x1)

}; // Size: 0x230

struct FScrollBarStyle : public FSlateWidgetStyle
{
    FSlateBrush HorizontalBackgroundImage;                                            // 0x0010 (size: 0xB0)
    FSlateBrush VerticalBackgroundImage;                                              // 0x00C0 (size: 0xB0)
    FSlateBrush VerticalTopSlotImage;                                                 // 0x0170 (size: 0xB0)
    FSlateBrush HorizontalTopSlotImage;                                               // 0x0220 (size: 0xB0)
    FSlateBrush VerticalBottomSlotImage;                                              // 0x02D0 (size: 0xB0)
    FSlateBrush HorizontalBottomSlotImage;                                            // 0x0380 (size: 0xB0)
    FSlateBrush NormalThumbImage;                                                     // 0x0430 (size: 0xB0)
    FSlateBrush HoveredThumbImage;                                                    // 0x04E0 (size: 0xB0)
    FSlateBrush DraggedThumbImage;                                                    // 0x0590 (size: 0xB0)
    float Thickness;                                                                  // 0x0640 (size: 0x4)

}; // Size: 0x650

struct FScrollBorderStyle : public FSlateWidgetStyle
{
    FSlateBrush TopShadowBrush;                                                       // 0x0010 (size: 0xB0)
    FSlateBrush BottomShadowBrush;                                                    // 0x00C0 (size: 0xB0)

}; // Size: 0x170

struct FScrollBoxStyle : public FSlateWidgetStyle
{
    float BarThickness;                                                               // 0x0008 (size: 0x4)
    FSlateBrush TopShadowBrush;                                                       // 0x0010 (size: 0xB0)
    FSlateBrush BottomShadowBrush;                                                    // 0x00C0 (size: 0xB0)
    FSlateBrush LeftShadowBrush;                                                      // 0x0170 (size: 0xB0)
    FSlateBrush RightShadowBrush;                                                     // 0x0220 (size: 0xB0)
    FMargin HorizontalScrolledContentPadding;                                         // 0x02D0 (size: 0x10)
    FMargin VerticalScrolledContentPadding;                                           // 0x02E0 (size: 0x10)

}; // Size: 0x2F0

struct FSearchBoxStyle : public FSlateWidgetStyle
{
    FEditableTextBoxStyle TextBoxStyle;                                               // 0x0010 (size: 0xC80)
    FSlateFontInfo ActiveFontInfo;                                                    // 0x0C90 (size: 0x60)
    FSlateBrush UpArrowImage;                                                         // 0x0CF0 (size: 0xB0)
    FSlateBrush DownArrowImage;                                                       // 0x0DA0 (size: 0xB0)
    FSlateBrush GlassImage;                                                           // 0x0E50 (size: 0xB0)
    FSlateBrush ClearImage;                                                           // 0x0F00 (size: 0xB0)
    FMargin ImagePadding;                                                             // 0x0FB0 (size: 0x10)
    TOptional<FVector2D> ImageSizeOverride;                                           // 0x0FC0 (size: 0x18)
    bool bLeftAlignButtons;                                                           // 0x0FD8 (size: 0x1)
    bool bLeftAlignSearchResultButtons;                                               // 0x0FD9 (size: 0x1)
    bool bLeftAlignGlassImageAndClearButton;                                          // 0x0FDA (size: 0x1)

}; // Size: 0xFE0

struct FSegmentedControlStyle : public FSlateWidgetStyle
{
    FCheckBoxStyle ControlStyle;                                                      // 0x0010 (size: 0x950)
    FCheckBoxStyle FirstControlStyle;                                                 // 0x0960 (size: 0x950)
    FCheckBoxStyle LastControlStyle;                                                  // 0x12B0 (size: 0x950)
    FSlateBrush BackgroundBrush;                                                      // 0x1C00 (size: 0xB0)
    FMargin UniformPadding;                                                           // 0x1CB0 (size: 0x10)

}; // Size: 0x1CC0

struct FSlateBrush
{
    FSlateColor TintColor;                                                            // 0x0008 (size: 0x14)
    TEnumAsByte<ESlateBrushDrawType::Type> DrawAs;                                    // 0x001C (size: 0x1)
    TEnumAsByte<ESlateBrushTileType::Type> Tiling;                                    // 0x001D (size: 0x1)
    TEnumAsByte<ESlateBrushMirrorType::Type> Mirroring;                               // 0x001E (size: 0x1)
    TEnumAsByte<ESlateBrushImageType::Type> ImageType;                                // 0x001F (size: 0x1)
    FDeprecateSlateVector2D ImageSize;                                                // 0x0020 (size: 0x8)
    FMargin Margin;                                                                   // 0x0028 (size: 0x10)
    class UObject* ResourceObject;                                                    // 0x0038 (size: 0x8)
    FSlateBrushOutlineSettings OutlineSettings;                                       // 0x0040 (size: 0x40)
    FBox2f UVRegion;                                                                  // 0x0080 (size: 0x14)
    uint8 bIsDynamicallyLoaded;                                                       // 0x0094 (size: 0x1)
    uint8 bHasUObject;                                                                // 0x0094 (size: 0x1)
    FName ResourceName;                                                               // 0x00A8 (size: 0x8)

}; // Size: 0xB0

struct FSlateBrushOutlineSettings
{
    FVector4 CornerRadii;                                                             // 0x0000 (size: 0x20)
    FSlateColor Color;                                                                // 0x0020 (size: 0x14)
    float Width;                                                                      // 0x0034 (size: 0x4)
    TEnumAsByte<ESlateBrushRoundingType::Type> RoundingType;                          // 0x0038 (size: 0x1)
    bool bUseBrushTransparency;                                                       // 0x0039 (size: 0x1)

}; // Size: 0x40

struct FSlateColor
{
    FLinearColor SpecifiedColor;                                                      // 0x0000 (size: 0x10)
    ESlateColorStylingMode ColorUseRule;                                              // 0x0010 (size: 0x1)

}; // Size: 0x14

struct FSlateFontInfo
{
    class UObject* FontObject;                                                        // 0x0000 (size: 0x8)
    class UObject* FontMaterial;                                                      // 0x0008 (size: 0x8)
    FFontOutlineSettings OutlineSettings;                                             // 0x0010 (size: 0x20)
    FName TypefaceFontName;                                                           // 0x0040 (size: 0x8)
    float Size;                                                                       // 0x0048 (size: 0x4)
    int32 LetterSpacing;                                                              // 0x004C (size: 0x4)
    float SkewAmount;                                                                 // 0x0050 (size: 0x4)
    bool bForceMonospaced;                                                            // 0x0055 (size: 0x1)
    bool bMaterialIsStencil;                                                          // 0x0056 (size: 0x1)
    float MonospacedWidth;                                                            // 0x0058 (size: 0x4)

}; // Size: 0x60

struct FSlateSound
{
    class UObject* ResourceObject;                                                    // 0x0000 (size: 0x8)

}; // Size: 0x18

struct FSlateWidgetStyle
{
}; // Size: 0x8

struct FSliderStyle : public FSlateWidgetStyle
{
    FSlateBrush NormalBarImage;                                                       // 0x0010 (size: 0xB0)
    FSlateBrush HoveredBarImage;                                                      // 0x00C0 (size: 0xB0)
    FSlateBrush DisabledBarImage;                                                     // 0x0170 (size: 0xB0)
    FSlateBrush NormalThumbImage;                                                     // 0x0220 (size: 0xB0)
    FSlateBrush HoveredThumbImage;                                                    // 0x02D0 (size: 0xB0)
    FSlateBrush DisabledThumbImage;                                                   // 0x0380 (size: 0xB0)
    float BarThickness;                                                               // 0x0430 (size: 0x4)

}; // Size: 0x440

struct FSpinBoxStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundBrush;                                                      // 0x0010 (size: 0xB0)
    FSlateBrush ActiveBackgroundBrush;                                                // 0x00C0 (size: 0xB0)
    FSlateBrush HoveredBackgroundBrush;                                               // 0x0170 (size: 0xB0)
    FSlateBrush ActiveFillBrush;                                                      // 0x0220 (size: 0xB0)
    FSlateBrush HoveredFillBrush;                                                     // 0x02D0 (size: 0xB0)
    FSlateBrush InactiveFillBrush;                                                    // 0x0380 (size: 0xB0)
    FSlateBrush ArrowsImage;                                                          // 0x0430 (size: 0xB0)
    FSlateColor ForegroundColor;                                                      // 0x04E0 (size: 0x14)
    FMargin TextPadding;                                                              // 0x04F4 (size: 0x10)
    FMargin InsetPadding;                                                             // 0x0504 (size: 0x10)

}; // Size: 0x520

struct FSplitterStyle : public FSlateWidgetStyle
{
    FSlateBrush HandleNormalBrush;                                                    // 0x0010 (size: 0xB0)
    FSlateBrush HandleHighlightBrush;                                                 // 0x00C0 (size: 0xB0)

}; // Size: 0x170

struct FStyleColorList
{
    FLinearColor StyleColors;                                                         // 0x0000 (size: 0x3D0)

}; // Size: 0x7A0

struct FStyleTheme
{
}; // Size: 0x40

struct FTableColumnHeaderStyle : public FSlateWidgetStyle
{
    FSlateBrush SortPrimaryAscendingImage;                                            // 0x0010 (size: 0xB0)
    FSlateBrush SortPrimaryDescendingImage;                                           // 0x00C0 (size: 0xB0)
    FSlateBrush SortSecondaryAscendingImage;                                          // 0x0170 (size: 0xB0)
    FSlateBrush SortSecondaryDescendingImage;                                         // 0x0220 (size: 0xB0)
    FSlateBrush NormalBrush;                                                          // 0x02D0 (size: 0xB0)
    FSlateBrush HoveredBrush;                                                         // 0x0380 (size: 0xB0)
    FSlateBrush MenuDropdownImage;                                                    // 0x0430 (size: 0xB0)
    FSlateBrush MenuDropdownNormalBorderBrush;                                        // 0x04E0 (size: 0xB0)
    FSlateBrush MenuDropdownHoveredBorderBrush;                                       // 0x0590 (size: 0xB0)

}; // Size: 0x640

struct FTableRowStyle : public FSlateWidgetStyle
{
    FSlateBrush SelectorFocusedBrush;                                                 // 0x0010 (size: 0xB0)
    FSlateBrush ActiveHoveredBrush;                                                   // 0x00C0 (size: 0xB0)
    FSlateBrush ActiveBrush;                                                          // 0x0170 (size: 0xB0)
    FSlateBrush InactiveHoveredBrush;                                                 // 0x0220 (size: 0xB0)
    FSlateBrush InactiveBrush;                                                        // 0x02D0 (size: 0xB0)
    bool bUseParentRowBrush;                                                          // 0x0380 (size: 0x1)
    FSlateBrush ParentRowBackgroundBrush;                                             // 0x0390 (size: 0xB0)
    FSlateBrush ParentRowBackgroundHoveredBrush;                                      // 0x0440 (size: 0xB0)
    FSlateBrush EvenRowBackgroundHoveredBrush;                                        // 0x04F0 (size: 0xB0)
    FSlateBrush EvenRowBackgroundBrush;                                               // 0x05A0 (size: 0xB0)
    FSlateBrush OddRowBackgroundHoveredBrush;                                         // 0x0650 (size: 0xB0)
    FSlateBrush OddRowBackgroundBrush;                                                // 0x0700 (size: 0xB0)
    FSlateColor TextColor;                                                            // 0x07B0 (size: 0x14)
    FSlateColor SelectedTextColor;                                                    // 0x07C4 (size: 0x14)
    FSlateBrush DropIndicator_Above;                                                  // 0x07E0 (size: 0xB0)
    FSlateBrush DropIndicator_Onto;                                                   // 0x0890 (size: 0xB0)
    FSlateBrush DropIndicator_Below;                                                  // 0x0940 (size: 0xB0)
    FSlateBrush ActiveHighlightedBrush;                                               // 0x09F0 (size: 0xB0)
    FSlateBrush InactiveHighlightedBrush;                                             // 0x0AA0 (size: 0xB0)

}; // Size: 0xB50

struct FTableViewStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundBrush;                                                      // 0x0010 (size: 0xB0)

}; // Size: 0xC0

struct FTextBlockStyle : public FSlateWidgetStyle
{
    FSlateFontInfo Font;                                                              // 0x0008 (size: 0x60)
    FSlateColor ColorAndOpacity;                                                      // 0x0068 (size: 0x14)
    FDeprecateSlateVector2D ShadowOffset;                                             // 0x007C (size: 0x8)
    FLinearColor ShadowColorAndOpacity;                                               // 0x0084 (size: 0x10)
    FSlateColor SelectedBackgroundColor;                                              // 0x0094 (size: 0x14)
    FSlateColor HighlightColor;                                                       // 0x00A8 (size: 0x14)
    FSlateBrush HighlightShape;                                                       // 0x00C0 (size: 0xB0)
    FSlateBrush StrikeBrush;                                                          // 0x0170 (size: 0xB0)
    FSlateBrush UnderlineBrush;                                                       // 0x0220 (size: 0xB0)
    ETextTransformPolicy TransformPolicy;                                             // 0x02D0 (size: 0x1)
    ETextOverflowPolicy OverflowPolicy;                                               // 0x02D1 (size: 0x1)

}; // Size: 0x2E0

struct FToolBarStyle : public FSlateWidgetStyle
{
    FSlateBrush BackgroundBrush;                                                      // 0x0010 (size: 0xB0)
    FSlateBrush ExpandBrush;                                                          // 0x00C0 (size: 0xB0)
    FSlateBrush SeparatorBrush;                                                       // 0x0170 (size: 0xB0)
    FTextBlockStyle LabelStyle;                                                       // 0x0220 (size: 0x2E0)
    FEditableTextBoxStyle EditableTextStyle;                                          // 0x0500 (size: 0xC80)
    FCheckBoxStyle ToggleButton;                                                      // 0x1180 (size: 0x950)
    FComboButtonStyle ComboButtonStyle;                                               // 0x1AD0 (size: 0x560)
    FButtonStyle SettingsButtonStyle;                                                 // 0x2030 (size: 0x390)
    FComboButtonStyle SettingsComboButton;                                            // 0x23C0 (size: 0x560)
    FCheckBoxStyle SettingsToggleButton;                                              // 0x2920 (size: 0x950)
    FButtonStyle ButtonStyle;                                                         // 0x3270 (size: 0x390)
    FMargin LabelPadding;                                                             // 0x3600 (size: 0x10)
    float UniformBlockWidth;                                                          // 0x3610 (size: 0x4)
    float UniformBlockHeight;                                                         // 0x3614 (size: 0x4)
    int32 NumColumns;                                                                 // 0x3618 (size: 0x4)
    FMargin IconPadding;                                                              // 0x361C (size: 0x10)
    FMargin SeparatorPadding;                                                         // 0x362C (size: 0x10)
    float SeparatorThickness;                                                         // 0x363C (size: 0x4)
    FMargin ComboButtonPadding;                                                       // 0x3640 (size: 0x10)
    FMargin ButtonPadding;                                                            // 0x3650 (size: 0x10)
    FMargin CheckBoxPadding;                                                          // 0x3660 (size: 0x10)
    FMargin BlockPadding;                                                             // 0x3670 (size: 0x10)
    FMargin IndentedBlockPadding;                                                     // 0x3680 (size: 0x10)
    FSlateBrush BlockHovered;                                                         // 0x3690 (size: 0xB0)
    FMargin BackgroundPadding;                                                        // 0x3740 (size: 0x10)
    FWrapButtonStyle WrapButtonStyle;                                                 // 0x3750 (size: 0x740)
    FMargin WrapButtonPadding;                                                        // 0x3E90 (size: 0x10)
    int32 WrapButtonIndex;                                                            // 0x3EA0 (size: 0x4)
    bool bAllowWrapButton;                                                            // 0x3EA4 (size: 0x1)
    bool bAllowWrappingDefault;                                                       // 0x3EA5 (size: 0x1)
    FDeprecateSlateVector2D IconSize;                                                 // 0x3EA8 (size: 0x8)
    bool bShowLabels;                                                                 // 0x3EB0 (size: 0x1)
    float ButtonContentMaxWidth;                                                      // 0x3EB4 (size: 0x4)
    float ButtonContentFillWidth;                                                     // 0x3EB8 (size: 0x4)
    float ComboContentMinWidth;                                                       // 0x3EBC (size: 0x4)
    float ComboContentMaxWidth;                                                       // 0x3EC0 (size: 0x4)
    TEnumAsByte<EHorizontalAlignment> ComboContentHorizontalAlignment;                // 0x3EC4 (size: 0x1)
    FMargin IconPaddingWithVisibleLabel;                                              // 0x3EC8 (size: 0x10)
    FMargin IconPaddingWithCollapsedLabel;                                            // 0x3ED8 (size: 0x10)
    TOptional<TEnumAsByte<EVerticalAlignment>> VerticalAlignmentOverride;             // 0x3EE8 (size: 0x2)
    float RaisedChildrenRightPadding;                                                 // 0x3EEC (size: 0x4)

}; // Size: 0x3EF0

struct FTypeface
{
    TArray<FTypefaceEntry> Fonts;                                                     // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FTypefaceEntry
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    FFontData Font;                                                                   // 0x0008 (size: 0x28)

}; // Size: 0x30

struct FVolumeControlStyle : public FSlateWidgetStyle
{
    FSliderStyle SliderStyle;                                                         // 0x0010 (size: 0x440)
    FSlateBrush HighVolumeImage;                                                      // 0x0450 (size: 0xB0)
    FSlateBrush MidVolumeImage;                                                       // 0x0500 (size: 0xB0)
    FSlateBrush LowVolumeImage;                                                       // 0x05B0 (size: 0xB0)
    FSlateBrush NoVolumeImage;                                                        // 0x0660 (size: 0xB0)
    FSlateBrush MutedImage;                                                           // 0x0710 (size: 0xB0)

}; // Size: 0x7C0

struct FWindowStyle : public FSlateWidgetStyle
{
    FButtonStyle MinimizeButtonStyle;                                                 // 0x0010 (size: 0x390)
    FButtonStyle MaximizeButtonStyle;                                                 // 0x03A0 (size: 0x390)
    FButtonStyle RestoreButtonStyle;                                                  // 0x0730 (size: 0x390)
    FButtonStyle EnterFullscreenButtonStyle;                                          // 0x0AC0 (size: 0x390)
    FButtonStyle ExitFullscreenButtonStyle;                                           // 0x0E50 (size: 0x390)
    FButtonStyle CloseButtonStyle;                                                    // 0x11E0 (size: 0x390)
    FTextBlockStyle TitleTextStyle;                                                   // 0x1570 (size: 0x2E0)
    FSlateBrush ActiveTitleBrush;                                                     // 0x1850 (size: 0xB0)
    FSlateBrush InactiveTitleBrush;                                                   // 0x1900 (size: 0xB0)
    FSlateBrush FlashTitleBrush;                                                      // 0x19B0 (size: 0xB0)
    FSlateColor BackgroundColor;                                                      // 0x1A60 (size: 0x14)
    FSlateBrush OutlineBrush;                                                         // 0x1A80 (size: 0xB0)
    FSlateColor OutlineColor;                                                         // 0x1B30 (size: 0x14)
    FSlateBrush BorderBrush;                                                          // 0x1B50 (size: 0xB0)
    FSlateColor BorderColor;                                                          // 0x1C00 (size: 0x14)
    FSlateBrush BackgroundBrush;                                                      // 0x1C20 (size: 0xB0)
    FSlateBrush ChildBackgroundBrush;                                                 // 0x1CD0 (size: 0xB0)
    int32 WindowCornerRadius;                                                         // 0x1D80 (size: 0x4)
    FMargin BorderPadding;                                                            // 0x1D84 (size: 0x10)

}; // Size: 0x1DA0

struct FWrapButtonStyle
{
    FMargin Padding;                                                                  // 0x0000 (size: 0x10)
    int32 WrapButtonIndex;                                                            // 0x0010 (size: 0x4)
    FSlateBrush ExpandBrush;                                                          // 0x0020 (size: 0xB0)
    bool bHasDownArrow;                                                               // 0x00D0 (size: 0x1)
    TOptional<FComboButtonStyle> ComboButtonStyle;                                    // 0x00E0 (size: 0x570)
    bool bIncludeSeparator;                                                           // 0x0650 (size: 0x1)
    TOptional<FSlateBrush> SeparatorBrush;                                            // 0x0660 (size: 0xC0)
    TOptional<float> SeparatorThickness;                                              // 0x0720 (size: 0x8)
    TOptional<FMargin> SeparatorPadding;                                              // 0x0728 (size: 0x14)

}; // Size: 0x740

class IFontFaceInterface : public IInterface
{
}; // Size: 0x28

class IFontProviderInterface : public IInterface
{
}; // Size: 0x28

class ISlateWidgetStyleContainerInterface : public IInterface
{
}; // Size: 0x28

class UFontBulkData : public UObject
{
}; // Size: 0x78

class USlateThemeManager : public UObject
{
    FGuid CurrentThemeId;                                                             // 0x0028 (size: 0x10)
    FStyleColorList ActiveColors;                                                     // 0x0038 (size: 0x7A0)

}; // Size: 0x7D8

class USlateTypes : public UObject
{
}; // Size: 0x28

class USlateWidgetStyleAsset : public UObject
{
    class USlateWidgetStyleContainerBase* CustomStyle;                                // 0x0028 (size: 0x8)

}; // Size: 0x30

class USlateWidgetStyleContainerBase : public UObject
{
}; // Size: 0x30

#endif
