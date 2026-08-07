#ifndef UE4SS_SDK_POIDiscoveredUI_HPP
#define UE4SS_SDK_POIDiscoveredUI_HPP

class UPOIDiscoveredUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* FadeStop;                                                 // 0x02C8 (size: 0x8)
    class UWidgetAnimation* FadeStart;                                                // 0x02D0 (size: 0x8)
    class UTextBlock* Area;                                                           // 0x02D8 (size: 0x8)
    class UImage* Image_182;                                                          // 0x02E0 (size: 0x8)
    class UTextBlock* Title;                                                          // 0x02E8 (size: 0x8)

    void SetName(FText Name, class UTexture* Texture, FLinearColor Color);
    void Construct();
    void FadeStartFinished();
    void ExecuteUbergraph_POIDiscoveredUI(int32 EntryPoint);
}; // Size: 0x2F0

#endif
