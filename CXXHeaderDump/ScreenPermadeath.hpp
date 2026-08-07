#ifndef UE4SS_SDK_ScreenPermadeath_HPP
#define UE4SS_SDK_ScreenPermadeath_HPP

class UScreenPermadeath_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02C8 (size: 0x8)
    class UTextBlock* DaysSurvived;                                                   // 0x02D0 (size: 0x8)
    class UTextBlock* DistanceTravelled;                                              // 0x02D8 (size: 0x8)
    class UTextBlock* HumansKilled;                                                   // 0x02E0 (size: 0x8)
    class UButton* QuitButton;                                                        // 0x02E8 (size: 0x8)
    class UButton* QuitToMainMenuButton;                                              // 0x02F0 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x02F8 (size: 0x8)
    class UTextBlock* TextBlock_2;                                                    // 0x0300 (size: 0x8)
    class UTextBlock* ZombiesKilled;                                                  // 0x0308 (size: 0x8)
    bool DeleteSave?;                                                                 // 0x0310 (size: 0x1)

    FText Get_DistanceTravelled();
    FText Get_HumansKilled();
    FText Get_ZombiesKilled();
    FText Get_DaysSurvived();
    void ClearUI();
    void Construct();
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_7_OnButtonHoverEvent__DelegateSignature();
    void ExecuteUbergraph_ScreenPermadeath(int32 EntryPoint);
}; // Size: 0x311

#endif
