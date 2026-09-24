#ifndef UE4SS_SDK_W_WorldMarker_HPP
#define UE4SS_SDK_W_WorldMarker_HPP

class UW_WorldMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x0348 (size: 0x8)
    class UImage* Icon;                                                               // 0x0350 (size: 0x8)
    class UImage* Fade;                                                               // 0x0358 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x0360 (size: 0x8)
    class AActor* Target Actor;                                                       // 0x0368 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0370 (size: 0x68)
    class UW_WorldMarkerTooltip_C* MarkerTooltip;                                     // 0x03D8 (size: 0x8)
    FText Description;                                                                // 0x03E0 (size: 0x10)
    FW_WorldMarker_CPOIExplored POIExplored;                                          // 0x03F0 (size: 0x10)
    void POIExplored();

    class UWidget* Get_ToolTipWidget();
    void Construct Marker();
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_WorldMarker(int32 EntryPoint);
    void POIExplored__DelegateSignature();
}; // Size: 0x400

#endif
