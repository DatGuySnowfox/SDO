#ifndef UE4SS_SDK_SleepingUI_HPP
#define UE4SS_SDK_SleepingUI_HPP

class USleepingUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x0348 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x0350 (size: 0x8)
    class UTextBlock* SleepingText;                                                   // 0x0358 (size: 0x8)
    class UButton* SleepButton;                                                       // 0x0360 (size: 0x8)
    class UButton* DontSleepButton;                                                   // 0x0368 (size: 0x8)
    class UVerticalBox* Choices;                                                      // 0x0370 (size: 0x8)
    class UWidgetAnimation* BlurAnim;                                                 // 0x0378 (size: 0x8)

    void ClearUI();
    void BndEvt__Button_60_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnButton_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void Exit();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_SleepingUI(int32 EntryPoint);
}; // Size: 0x380

#endif
