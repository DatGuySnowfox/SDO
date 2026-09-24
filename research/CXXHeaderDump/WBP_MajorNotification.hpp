#ifndef UE4SS_SDK_WBP_MajorNotification_HPP
#define UE4SS_SDK_WBP_MajorNotification_HPP

class UWBP_MajorNotification_C : public UCommonUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0368 (size: 0x8)
    class UVerticalBox* VerticalBox_Main;                                             // 0x0370 (size: 0x8)
    class UWBP_NarrativeCommonTextBlock_C* CommonTextBlock_Title;                     // 0x0378 (size: 0x8)
    class UWBP_NarrativeCommonTextBlock_C* CommonTextBlock_Subtitle;                  // 0x0380 (size: 0x8)
    class UWidgetAnimation* PopIn;                                                    // 0x0388 (size: 0x8)
    FText Text;                                                                       // 0x0390 (size: 0x10)
    class USoundBase* NotificationSound;                                              // 0x03A0 (size: 0x8)

    void Show Notification(FText Title, FText Subtitle, double Duration, bool Override Existing);
    void Clear Notification();
    void Hide();
    void ExecuteUbergraph_WBP_MajorNotification(int32 EntryPoint);
}; // Size: 0x3A8

#endif
