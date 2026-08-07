#ifndef UE4SS_SDK_WBP_NotificationBox_HPP
#define UE4SS_SDK_WBP_NotificationBox_HPP

class UWBP_NotificationBox_C : public UCommonUserWidget
{
    class UVerticalBox* VerticalBox_Notifications;                                    // 0x02E8 (size: 0x8)
    FText Text;                                                                       // 0x02F0 (size: 0x18)

    void NotificationExpired(class UWBP_NarrativeHUDNotification_C* Notification);
    void ShowNotification(FText Text, double Duration);
}; // Size: 0x308

#endif
