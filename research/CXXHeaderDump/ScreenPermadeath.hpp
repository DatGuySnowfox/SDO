#ifndef UE4SS_SDK_ScreenPermadeath_HPP
#define UE4SS_SDK_ScreenPermadeath_HPP

class UScreenPermadeath_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ZombiesKilled;                                                  // 0x0348 (size: 0x8)
    class UTextBlock* TextBlock_2;                                                    // 0x0350 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x0358 (size: 0x8)
    class UButton* QuitToMainMenuButton;                                              // 0x0360 (size: 0x8)
    class UButton* QuitButton;                                                        // 0x0368 (size: 0x8)
    class UTextBlock* HumansKilled;                                                   // 0x0370 (size: 0x8)
    class UTextBlock* DistanceTravelled;                                              // 0x0378 (size: 0x8)
    class UTextBlock* DaysSurvived;                                                   // 0x0380 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0388 (size: 0x8)
    bool DeleteSave?;                                                                 // 0x0390 (size: 0x1)

    FText Get_DistanceTravelled();
    FText Get_HumansKilled();
    FText Get_ZombiesKilled();
    FText Get_DaysSurvived();
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__BP_RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_7_OnButtonHoverEvent__DelegateSignature();
    void ExecuteUbergraph_ScreenPermadeath(int32 EntryPoint);
}; // Size: 0x391

#endif
