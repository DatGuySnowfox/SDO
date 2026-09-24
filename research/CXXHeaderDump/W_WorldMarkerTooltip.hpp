#ifndef UE4SS_SDK_W_WorldMarkerTooltip_HPP
#define UE4SS_SDK_W_WorldMarkerTooltip_HPP

class UW_WorldMarkerTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UBorder* TitleBorder;                                                       // 0x0348 (size: 0x8)
    class UTextBlock* POIName;                                                        // 0x0350 (size: 0x8)
    class UImage* POIImage;                                                           // 0x0358 (size: 0x8)
    class UTextBlock* POIDescription;                                                 // 0x0360 (size: 0x8)
    class UBorder* DescriptionBorder;                                                 // 0x0368 (size: 0x8)
    FText Name;                                                                       // 0x0370 (size: 0x10)
    FText Description;                                                                // 0x0380 (size: 0x10)
    class UObject* Image;                                                             // 0x0390 (size: 0x8)
    FLinearColor Color;                                                               // 0x0398 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_W_WorldMarkerTooltip(int32 EntryPoint);
}; // Size: 0x3A8

#endif
