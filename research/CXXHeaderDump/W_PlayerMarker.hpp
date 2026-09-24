#ifndef UE4SS_SDK_W_PlayerMarker_HPP
#define UE4SS_SDK_W_PlayerMarker_HPP

class UW_PlayerMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UVerticalBox* VerticalBox_2;                                                // 0x0348 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x0350 (size: 0x8)
    class UImage* Icon;                                                               // 0x0358 (size: 0x8)
    class UImage* Fade;                                                               // 0x0360 (size: 0x8)
    class UWidgetAnimation* Hover;                                                    // 0x0368 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x0370 (size: 0x1)
    class ABP_PlayerMarker_C* Target Actor;                                           // 0x0378 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0380 (size: 0x68)

    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    void Construct Marker();
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_PlayerMarker(int32 EntryPoint);
}; // Size: 0x3E8

#endif
