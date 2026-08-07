#ifndef UE4SS_SDK_WBP_NarrativeHUD_HPP
#define UE4SS_SDK_WBP_NarrativeHUD_HPP

class UWBP_NarrativeHUD_C : public UNarrativeCommonHUD
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E8 (size: 0x8)
    class UCommonBoundActionBar* CommonBoundActionBar;                                // 0x02F0 (size: 0x8)
    class UWBP_NarrativeWidgetStack_C* MenuStack;                                     // 0x02F8 (size: 0x8)
    class UCommonActivatableWidgetStack* PromptStack;                                 // 0x0300 (size: 0x8)
    class UWBP_NotificationBox_C* WBP_NotificationBox;                                // 0x0308 (size: 0x8)
    FWBP_NarrativeHUD_COnMenuAdded OnMenuAdded;                                       // 0x0310 (size: 0x10)
    void OnMenuAdded();
    int32 MaxAllowedNotifications;                                                    // 0x0320 (size: 0x4)

    void Open Quantity Selector(int32 MinAmount, int32 MaxAmount, FText InstructionText, class UWBP_QuantitySelector_C*& Quantity Selector);
    class UWBP_NarrativeMenu_C* OpenMenu(TSubclassOf<class UWBP_NarrativeMenu_C> ActivatableWidgetClass);
    void ShowNotification(const FText& NotificationText, const float Duration);
    void ExecuteUbergraph_WBP_NarrativeHUD(int32 EntryPoint);
    void OnMenuAdded__DelegateSignature();
}; // Size: 0x324

#endif
