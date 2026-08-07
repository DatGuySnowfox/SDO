#ifndef UE4SS_SDK_W_QuestMarker_HPP
#define UE4SS_SDK_W_QuestMarker_HPP

class UW_QuestMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Hover;                                                    // 0x02C8 (size: 0x8)
    class UImage* Fade;                                                               // 0x02D0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D8 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x02E0 (size: 0x8)
    class UImage* Underground;                                                        // 0x02E8 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x02F0 (size: 0x1)
    class ABP_QuestMarker_C* Target Actor;                                            // 0x02F8 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0300 (size: 0x70)
    class UW_WorldMarkerTooltip_C* MarkerTooltip;                                     // 0x0370 (size: 0x8)

    class UWidget* Get_Marker_Box_ToolTipWidget();
    void Construct Marker();
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_QuestMarker(int32 EntryPoint);
}; // Size: 0x378

#endif
