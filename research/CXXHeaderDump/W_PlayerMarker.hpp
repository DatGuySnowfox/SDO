#ifndef UE4SS_SDK_W_PlayerMarker_HPP
#define UE4SS_SDK_W_PlayerMarker_HPP

class UW_PlayerMarker_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Hover;                                                    // 0x02C8 (size: 0x8)
    class UImage* Fade;                                                               // 0x02D0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D8 (size: 0x8)
    class USizeBox* Marker_Box;                                                       // 0x02E0 (size: 0x8)
    class UVerticalBox* VerticalBox_2;                                                // 0x02E8 (size: 0x8)
    bool Is Hidden?;                                                                  // 0x02F0 (size: 0x1)
    class ABP_PlayerMarker_C* Target Actor;                                           // 0x02F8 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x0300 (size: 0x70)
    int32 Index;                                                                      // 0x0370 (size: 0x4)

    FEventReply OnMouseButtonDoubleClick(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);
    void Construct Marker();
    void Construct();
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void OnMouseLeave(const FPointerEvent& MouseEvent);
    void ExecuteUbergraph_W_PlayerMarker(int32 EntryPoint);
}; // Size: 0x374

#endif
