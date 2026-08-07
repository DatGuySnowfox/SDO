#ifndef UE4SS_SDK_Binoculars_DUPL_1_HPP
#define UE4SS_SDK_Binoculars_DUPL_1_HPP

class UBinoculars_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* BinocularsOverlay;                                                  // 0x02C8 (size: 0x8)
    class UTextBlock* DistanceText;                                                   // 0x02D0 (size: 0x8)
    class UTextBlock* MarkText;                                                       // 0x02D8 (size: 0x8)
    class UTextBlock* ZoomText;                                                       // 0x02E0 (size: 0x8)
    int32 Zoom;                                                                       // 0x02E8 (size: 0x4)
    int32 Distance;                                                                   // 0x02EC (size: 0x4)

    FText GetDistanceTextText();
    FText GetZoomText();
    void Construct();
    void Exit();
    void Destruct();
    void ExecuteUbergraph_Binoculars(int32 EntryPoint);
}; // Size: 0x2F0

#endif
