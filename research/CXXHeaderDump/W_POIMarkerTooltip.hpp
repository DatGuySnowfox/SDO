#ifndef UE4SS_SDK_W_POIMarkerTooltip_HPP
#define UE4SS_SDK_W_POIMarkerTooltip_HPP

class UW_POIMarkerTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UHorizontalBox* ContaminationBox;                                           // 0x02C8 (size: 0x8)
    class UImage* ContaminationImg;                                                   // 0x02D0 (size: 0x8)
    class UTextBlock* ContaminationLevelText;                                         // 0x02D8 (size: 0x8)
    class UImage* Image_92;                                                           // 0x02E0 (size: 0x8)
    class UVerticalBox* LargePOIInfoBox;                                              // 0x02E8 (size: 0x8)
    class UTextBlock* POIBossAmount;                                                  // 0x02F0 (size: 0x8)
    class UTextBlock* POIContainer;                                                   // 0x02F8 (size: 0x8)
    class UTextBlock* POIDescription;                                                 // 0x0300 (size: 0x8)
    class UImage* POIImage;                                                           // 0x0308 (size: 0x8)
    class UTextBlock* POILoot;                                                        // 0x0310 (size: 0x8)
    class UTextBlock* POIName;                                                        // 0x0318 (size: 0x8)
    class UBorder* POITitleBorder;                                                    // 0x0320 (size: 0x8)
    FString Name;                                                                     // 0x0328 (size: 0x10)
    FString Description;                                                              // 0x0338 (size: 0x10)
    FString ItemsToSpawn;                                                             // 0x0348 (size: 0x10)
    FStruct_POI POI;                                                                  // 0x0358 (size: 0x70)
    FLinearColor TitleBorderColor;                                                    // 0x03C8 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_W_POIMarkerTooltip(int32 EntryPoint);
}; // Size: 0x3D8

#endif
