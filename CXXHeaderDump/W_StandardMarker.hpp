#ifndef UE4SS_SDK_W_StandardMarker_HPP
#define UE4SS_SDK_W_StandardMarker_HPP

class UW_StandardMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Hide;                                                     // 0x02C8 (size: 0x8)
    class UImage* Fade;                                                               // 0x02D0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D8 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x02E0 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02E8 (size: 0x8)
    class UVerticalBox* VerticalBox_2;                                                // 0x02F0 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x02F8 (size: 0x1)
    class AActor* Target Actor;                                                       // 0x0300 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0308 (size: 0x70)
    class UMapMarker_ToolTip_C* MarkerTooltip;                                        // 0x0378 (size: 0x8)

    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    void Construct Marker();
    void Construct();
    void Play Hide Animation(TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode, bool Destroy On Hide);
    void ExecuteUbergraph_W_StandardMarker(int32 EntryPoint);
}; // Size: 0x380

#endif
