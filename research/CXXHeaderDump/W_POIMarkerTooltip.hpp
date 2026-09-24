#ifndef UE4SS_SDK_W_POIMarkerTooltip_HPP
#define UE4SS_SDK_W_POIMarkerTooltip_HPP

class UW_POIMarkerTooltip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UBorder* POITitleBorder;                                                    // 0x0348 (size: 0x8)
    class UTextBlock* POIName;                                                        // 0x0350 (size: 0x8)
    class UTextBlock* POILoot;                                                        // 0x0358 (size: 0x8)
    class UImage* POIImage;                                                           // 0x0360 (size: 0x8)
    class UTextBlock* POIDescription;                                                 // 0x0368 (size: 0x8)
    class UTextBlock* POIContainer;                                                   // 0x0370 (size: 0x8)
    class UTextBlock* POIBossAmount;                                                  // 0x0378 (size: 0x8)
    class UVerticalBox* LargePOIInfoBox;                                              // 0x0380 (size: 0x8)
    class UImage* Image_92;                                                           // 0x0388 (size: 0x8)
    class UTextBlock* ContaminationLevelText;                                         // 0x0390 (size: 0x8)
    class UImage* ContaminationImg;                                                   // 0x0398 (size: 0x8)
    class UHorizontalBox* ContaminationBox;                                           // 0x03A0 (size: 0x8)
    FString Name;                                                                     // 0x03A8 (size: 0x10)
    FString Description;                                                              // 0x03B8 (size: 0x10)
    FString ItemsToSpawn;                                                             // 0x03C8 (size: 0x10)
    FStruct_POI POI;                                                                  // 0x03D8 (size: 0x70)
    FLinearColor TitleBorderColor;                                                    // 0x0448 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_W_POIMarkerTooltip(int32 EntryPoint);
}; // Size: 0x458

#endif
