#ifndef UE4SS_SDK_MapMarker_ToolTip_HPP
#define UE4SS_SDK_MapMarker_ToolTip_HPP

class UMapMarker_ToolTip_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02C8 (size: 0x8)
    FString Name;                                                                     // 0x02D0 (size: 0x10)
    FLinearColor Colour;                                                              // 0x02E0 (size: 0x10)

    void Construct();
    void ExecuteUbergraph_MapMarker_ToolTip(int32 EntryPoint);
}; // Size: 0x2F0

#endif
