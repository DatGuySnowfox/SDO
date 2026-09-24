#ifndef UE4SS_SDK_NotficationUI_HPP
#define UE4SS_SDK_NotficationUI_HPP

class UNotficationUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0348 (size: 0x8)
    class UImage* Icon;                                                               // 0x0350 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0358 (size: 0x8)
    class UWidgetAnimation* Movement;                                                 // 0x0360 (size: 0x8)
    double NotificationTime;                                                          // 0x0368 (size: 0x8)
    double FadeOutSpeed;                                                              // 0x0370 (size: 0x8)
    int32 TextSize;                                                                   // 0x0378 (size: 0x4)
    FS_NotificationDetails NotificationDetailsStruct;                                 // 0x0380 (size: 0x38)

    void SetupNotification();
    void Construct();
    void Finished();
    void ExecuteUbergraph_NotficationUI(int32 EntryPoint);
}; // Size: 0x3B8

#endif
