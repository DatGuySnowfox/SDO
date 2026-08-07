#ifndef UE4SS_SDK_WBP_NarrativeHUDNotification_HPP
#define UE4SS_SDK_WBP_NarrativeHUDNotification_HPP

class UWBP_NarrativeHUDNotification_C : public UCommonUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E8 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x02F0 (size: 0x8)
    class UCommonTextBlock* CommonTextBlock_NotificationText;                         // 0x02F8 (size: 0x8)
    class UImage* Image_124;                                                          // 0x0300 (size: 0x8)
    FText Text;                                                                       // 0x0308 (size: 0x18)
    double Duration;                                                                  // 0x0320 (size: 0x8)
    FWBP_NarrativeHUDNotification_CNotificationExpired NotificationExpired;           // 0x0328 (size: 0x10)
    void NotificationExpired(class UWBP_NarrativeHUDNotification_C* Notification);
    double AnimLength;                                                                // 0x0338 (size: 0x8)

    void Construct();
    void Expire();
    void Begin Expire();
    void ExecuteUbergraph_WBP_NarrativeHUDNotification(int32 EntryPoint);
    void NotificationExpired__DelegateSignature(class UWBP_NarrativeHUDNotification_C* Notification);
}; // Size: 0x340

#endif
