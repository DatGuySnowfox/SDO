#ifndef UE4SS_SDK_W_StandardMarker_HPP
#define UE4SS_SDK_W_StandardMarker_HPP

class UW_StandardMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVerticalBox* VerticalBox_2;                                                // 0x0348 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0350 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x0358 (size: 0x8)
    class UImage* Icon;                                                               // 0x0360 (size: 0x8)
    class UImage* Fade;                                                               // 0x0368 (size: 0x8)
    class UWidgetAnimation* Hide;                                                     // 0x0370 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x0378 (size: 0x1)
    class AActor* Target Actor;                                                       // 0x0380 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0388 (size: 0x68)
    class UMapMarker_ToolTip_C* MarkerTooltip;                                        // 0x03F0 (size: 0x8)

    void Construct Marker();
    void Construct();
    void Play Hide Animation(TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode, bool Destroy On Hide);
    void ExecuteUbergraph_W_StandardMarker(int32 EntryPoint);
}; // Size: 0x3F8

#endif
