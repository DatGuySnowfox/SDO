#ifndef UE4SS_SDK_WidgetCarousel_HPP
#define UE4SS_SDK_WidgetCarousel_HPP

struct FWidgetCarouselNavigationBarStyle : public FSlateWidgetStyle
{
    FSlateBrush HighlightBrush;                                                       // 0x0010 (size: 0xD0)
    FButtonStyle LeftButtonStyle;                                                     // 0x00E0 (size: 0x3F0)
    FButtonStyle CenterButtonStyle;                                                   // 0x04D0 (size: 0x3F0)
    FButtonStyle RightButtonStyle;                                                    // 0x08C0 (size: 0x3F0)

}; // Size: 0xCB0

struct FWidgetCarouselNavigationButtonStyle : public FSlateWidgetStyle
{
    FButtonStyle InnerButtonStyle;                                                    // 0x0010 (size: 0x3F0)
    FSlateBrush NavigationButtonLeftImage;                                            // 0x0400 (size: 0xD0)
    FSlateBrush NavigationButtonRightImage;                                           // 0x04D0 (size: 0xD0)

}; // Size: 0x5A0

#endif
