#ifndef UE4SS_SDK_WidgetCarousel_HPP
#define UE4SS_SDK_WidgetCarousel_HPP

struct FWidgetCarouselNavigationBarStyle : public FSlateWidgetStyle
{
    FSlateBrush HighlightBrush;                                                       // 0x0010 (size: 0xB0)
    FButtonStyle LeftButtonStyle;                                                     // 0x00C0 (size: 0x390)
    FButtonStyle CenterButtonStyle;                                                   // 0x0450 (size: 0x390)
    FButtonStyle RightButtonStyle;                                                    // 0x07E0 (size: 0x390)

}; // Size: 0xB70

struct FWidgetCarouselNavigationButtonStyle : public FSlateWidgetStyle
{
    FButtonStyle InnerButtonStyle;                                                    // 0x0010 (size: 0x390)
    FSlateBrush NavigationButtonLeftImage;                                            // 0x03A0 (size: 0xB0)
    FSlateBrush NavigationButtonRightImage;                                           // 0x0450 (size: 0xB0)

}; // Size: 0x500

#endif
