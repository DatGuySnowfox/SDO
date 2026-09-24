#ifndef UE4SS_SDK_W_QuestMarker_HPP
#define UE4SS_SDK_W_QuestMarker_HPP

class UW_QuestMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UImage* Underground;                                                        // 0x0348 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x0350 (size: 0x8)
    class UImage* Icon;                                                               // 0x0358 (size: 0x8)
    class UImage* Fade;                                                               // 0x0360 (size: 0x8)
    class UWidgetAnimation* Hover;                                                    // 0x0368 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x0370 (size: 0x1)
    class ABP_QuestMarker_C* Target Actor;                                            // 0x0378 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0380 (size: 0x68)
    class UW_WorldMarkerTooltip_C* MarkerTooltip;                                     // 0x03E8 (size: 0x8)

    class UWidget* Get_Marker_Box_ToolTipWidget();
    void Construct Marker();
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_QuestMarker(int32 EntryPoint);
}; // Size: 0x3F0

#endif
