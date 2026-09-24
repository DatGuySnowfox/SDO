#ifndef UE4SS_SDK_Binoculars_HPP
#define UE4SS_SDK_Binoculars_HPP

class UBinoculars_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ZoomText;                                                       // 0x0348 (size: 0x8)
    class UTextBlock* MarkText;                                                       // 0x0350 (size: 0x8)
    class UTextBlock* DistanceText;                                                   // 0x0358 (size: 0x8)
    class UImage* BinocularsOverlay;                                                  // 0x0360 (size: 0x8)
    int32 Zoom;                                                                       // 0x0368 (size: 0x4)
    int32 Distance;                                                                   // 0x036C (size: 0x4)

    FText GetDistanceTextText();
    FText GetZoomText();
    void Construct();
    void Exit();
    void Destruct();
    void ExecuteUbergraph_Binoculars(int32 EntryPoint);
}; // Size: 0x370

#endif
