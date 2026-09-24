#ifndef UE4SS_SDK_UMG_BasicRadialIcon_HPP
#define UE4SS_SDK_UMG_BasicRadialIcon_HPP

class UUMG_BasicRadialIcon_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USizeBox* Sizer;                                                            // 0x0348 (size: 0x8)
    class UImage* Icon;                                                               // 0x0350 (size: 0x8)
    class UWidgetAnimation* Grow;                                                     // 0x0358 (size: 0x8)
    class UWidgetAnimation* Shake;                                                    // 0x0360 (size: 0x8)
    class UTexture2D* IconImage;                                                      // 0x0368 (size: 0x8)
    double IconSize;                                                                  // 0x0370 (size: 0x8)
    FFBasicIconSettings Settings;                                                     // 0x0378 (size: 0x28)
    FLinearColor CurrentColor;                                                        // 0x03A0 (size: 0x10)
    bool highlighted;                                                                 // 0x03B0 (size: 0x1)
    double Alpha;                                                                     // 0x03B8 (size: 0x8)

    FLinearColor Get_Icon_ColorAndOpacity_0();
    void PreConstruct(bool IsDesignTime);
    void OnHighlight();
    void OnUnhighlight();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_UMG_BasicRadialIcon(int32 EntryPoint);
}; // Size: 0x3C0

#endif
