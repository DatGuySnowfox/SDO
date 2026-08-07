#ifndef UE4SS_SDK_NotficationUI_HPP
#define UE4SS_SDK_NotficationUI_HPP

class UNotficationUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Movement;                                                 // 0x02C8 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02D0 (size: 0x8)
    class UImage* Icon;                                                               // 0x02D8 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x02E0 (size: 0x8)
    double NotificationTime;                                                          // 0x02E8 (size: 0x8)
    double FadeOutSpeed;                                                              // 0x02F0 (size: 0x8)
    int32 TextSize;                                                                   // 0x02F8 (size: 0x4)
    FS_NotificationDetails NotificationDetailsStruct;                                 // 0x0300 (size: 0x40)

    void SetupNotification();
    void Construct();
    void Finished();
    void ExecuteUbergraph_NotficationUI(int32 EntryPoint);
}; // Size: 0x340

#endif
