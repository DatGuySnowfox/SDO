#ifndef UE4SS_SDK_W_WorldMarkerTooltip_HPP
#define UE4SS_SDK_W_WorldMarkerTooltip_HPP

class UW_WorldMarkerTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UBorder* DescriptionBorder;                                                 // 0x02C8 (size: 0x8)
    class UTextBlock* POIDescription;                                                 // 0x02D0 (size: 0x8)
    class UImage* POIImage;                                                           // 0x02D8 (size: 0x8)
    class UTextBlock* POIName;                                                        // 0x02E0 (size: 0x8)
    class UBorder* TitleBorder;                                                       // 0x02E8 (size: 0x8)
    FText Name;                                                                       // 0x02F0 (size: 0x18)
    FText Description;                                                                // 0x0308 (size: 0x18)
    class UObject* Image;                                                             // 0x0320 (size: 0x8)
    FLinearColor Color;                                                               // 0x0328 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_W_WorldMarkerTooltip(int32 EntryPoint);
}; // Size: 0x338

#endif
