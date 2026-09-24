#ifndef UE4SS_SDK_MapMarker_ToolTip_HPP
#define UE4SS_SDK_MapMarker_ToolTip_HPP

class UMapMarker_ToolTip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0348 (size: 0x8)
    FString Name;                                                                     // 0x0350 (size: 0x10)
    FLinearColor Colour;                                                              // 0x0360 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_MapMarker_ToolTip(int32 EntryPoint);
}; // Size: 0x370

#endif
