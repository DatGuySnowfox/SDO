#ifndef UE4SS_SDK_WBP_NarrativeHUDNotification_HPP
#define UE4SS_SDK_WBP_NarrativeHUDNotification_HPP

class UWBP_NarrativeHUDNotification_C : public UCommonUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0368 (size: 0x8)
    class UImage* Image_124;                                                          // 0x0370 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_NotificationText;                         // 0x0378 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x0380 (size: 0x8)
    FText Text;                                                                       // 0x0388 (size: 0x10)
    double Duration;                                                                  // 0x0398 (size: 0x8)
    FWBP_NarrativeHUDNotification_CNotificationExpired NotificationExpired;           // 0x03A0 (size: 0x10)
    void NotificationExpired(class UWBP_NarrativeHUDNotification_C* Notification);
    double AnimLength;                                                                // 0x03B0 (size: 0x8)

    void Construct();
    void Expire();
    void Begin Expire();
    void ExecuteUbergraph_WBP_NarrativeHUDNotification(int32 EntryPoint);
    void NotificationExpired__DelegateSignature(class UWBP_NarrativeHUDNotification_C* Notification);
}; // Size: 0x3B8

#endif
