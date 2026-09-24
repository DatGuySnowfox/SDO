#ifndef UE4SS_SDK_W_POIMarker_HPP
#define UE4SS_SDK_W_POIMarker_HPP

class UW_POIMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x0348 (size: 0x8)
    class UImage* Icon;                                                               // 0x0350 (size: 0x8)
    class UImage* Fade;                                                               // 0x0358 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x0360 (size: 0x8)
    class AActor* Target Actor;                                                       // 0x0368 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0370 (size: 0x68)
    class UW_POIMarkerTooltip_C* MarkerTooltip;                                       // 0x03D8 (size: 0x8)
    bool Explored?;                                                                   // 0x03E0 (size: 0x1)
    FStruct_POI POI Data;                                                             // 0x03E8 (size: 0x70)
    FW_POIMarker_CPOIExplored POIExplored;                                            // 0x0458 (size: 0x10)
    void POIExplored();

    class UWidget* Get_ToolTipWidget();
    void Construct Marker();
    void Construct();
    void EventExplored();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_POIMarker(int32 EntryPoint);
    void POIExplored__DelegateSignature();
}; // Size: 0x468

#endif
