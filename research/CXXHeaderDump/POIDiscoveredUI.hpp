#ifndef UE4SS_SDK_POIDiscoveredUI_HPP
#define UE4SS_SDK_POIDiscoveredUI_HPP

class UPOIDiscoveredUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Title;                                                          // 0x0348 (size: 0x8)
    class UImage* Image_182;                                                          // 0x0350 (size: 0x8)
    class UTextBlock* Area;                                                           // 0x0358 (size: 0x8)
    class UWidgetAnimation* FadeStart;                                                // 0x0360 (size: 0x8)
    class UWidgetAnimation* FadeStop;                                                 // 0x0368 (size: 0x8)

    void SetName(FText Name, class UTexture* Texture, FLinearColor Color);
    void Construct();
    void FadeStartFinished();
    void ExecuteUbergraph_POIDiscoveredUI(int32 EntryPoint);
}; // Size: 0x370

#endif
