#ifndef UE4SS_SDK_W_POIMarker_HPP
#define UE4SS_SDK_W_POIMarker_HPP

class UW_POIMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x02C8 (size: 0x8)
    class UImage* Fade;                                                               // 0x02D0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D8 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x02E0 (size: 0x8)
    class AActor* Target Actor;                                                       // 0x02E8 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x02F0 (size: 0x70)
    class UW_POIMarkerTooltip_C* MarkerTooltip;                                       // 0x0360 (size: 0x8)
    bool Explored?;                                                                   // 0x0368 (size: 0x1)
    FStruct_POI POI Data;                                                             // 0x0370 (size: 0x70)
    FW_POIMarker_CPOIExplored POIExplored;                                            // 0x03E0 (size: 0x10)
    void POIExplored();

    class UWidget* Get_ToolTipWidget();
    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    void Construct Marker();
    void Construct();
    void EventExplored();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_POIMarker(int32 EntryPoint);
    void POIExplored__DelegateSignature();
}; // Size: 0x3F0

#endif
