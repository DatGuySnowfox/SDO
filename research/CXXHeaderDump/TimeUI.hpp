#ifndef UE4SS_SDK_TimeUI_HPP
#define UE4SS_SDK_TimeUI_HPP

class UTimeUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Anim_Reverse;                                             // 0x02C8 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x02D0 (size: 0x8)
    class UTextBlock* DateSlashText;                                                  // 0x02D8 (size: 0x8)
    class UTextBlock* DayText;                                                        // 0x02E0 (size: 0x8)
    class UImage* Image_174;                                                          // 0x02E8 (size: 0x8)
    class UTextBlock* MonthText;                                                      // 0x02F0 (size: 0x8)
    class UTextBlock* SeasonText;                                                     // 0x02F8 (size: 0x8)
    class UTextBlock* TimeText;                                                       // 0x0300 (size: 0x8)
    class UTextBlock* YearText;                                                       // 0x0308 (size: 0x8)
    int32 Zoom;                                                                       // 0x0310 (size: 0x4)
    class AUltra_Dynamic_Sky_C* UDS;                                                  // 0x0318 (size: 0x8)

    FText Get_ZoomText_Text_0();
    void Construct();
    void Event_Destruct();
    void CustomEvent();
    void Event_Clock();
    void ExecuteUbergraph_TimeUI(int32 EntryPoint);
}; // Size: 0x320

#endif
