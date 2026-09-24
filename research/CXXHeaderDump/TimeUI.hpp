#ifndef UE4SS_SDK_TimeUI_HPP
#define UE4SS_SDK_TimeUI_HPP

class UTimeUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* YearText;                                                       // 0x0348 (size: 0x8)
    class UTextBlock* TimeText;                                                       // 0x0350 (size: 0x8)
    class UTextBlock* TempText;                                                       // 0x0358 (size: 0x8)
    class UTextBlock* MonthText;                                                      // 0x0360 (size: 0x8)
    class UImage* Image_174;                                                          // 0x0368 (size: 0x8)
    class UTextBlock* DayText;                                                        // 0x0370 (size: 0x8)
    class UTextBlock* DateSlashText;                                                  // 0x0378 (size: 0x8)
    class UWidgetAnimation* Anim;                                                     // 0x0380 (size: 0x8)
    class UWidgetAnimation* Anim_Reverse;                                             // 0x0388 (size: 0x8)
    int32 Zoom;                                                                       // 0x0390 (size: 0x4)
    class AUltra_Dynamic_Sky_C* UDS;                                                  // 0x0398 (size: 0x8)

    FText Get_ZoomText_Text_0();
    void Construct();
    void Event_Destruct();
    void CustomEvent();
    void Event_Clock();
    void ExecuteUbergraph_TimeUI(int32 EntryPoint);
}; // Size: 0x3A0

#endif
