#ifndef UE4SS_SDK_WBP_NotificationBox_HPP
#define UE4SS_SDK_WBP_NotificationBox_HPP

class UWBP_NotificationBox_C : public UCommonUserWidget
{
    class UVerticalBox* VerticalBox_Notifications;                                    // 0x0368 (size: 0x8)
    FText Text;                                                                       // 0x0370 (size: 0x10)

    void NotificationExpired(class UWBP_NarrativeHUDNotification_C* Notification);
    void ShowNotification(FText Text, double Duration);
}; // Size: 0x380

#endif
