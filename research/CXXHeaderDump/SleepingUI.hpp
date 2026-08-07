#ifndef UE4SS_SDK_SleepingUI_HPP
#define UE4SS_SDK_SleepingUI_HPP

class USleepingUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* BlurAnim;                                                 // 0x02C8 (size: 0x8)
    class UVerticalBox* Choices;                                                      // 0x02D0 (size: 0x8)
    class UButton* DontSleepButton;                                                   // 0x02D8 (size: 0x8)
    class UButton* SleepButton;                                                       // 0x02E0 (size: 0x8)
    class UTextBlock* SleepingText;                                                   // 0x02E8 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x02F0 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x02F8 (size: 0x8)

    void ClearUI();
    void BndEvt__Button_60_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnButton_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void Exit();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_SleepingUI(int32 EntryPoint);
}; // Size: 0x300

#endif
