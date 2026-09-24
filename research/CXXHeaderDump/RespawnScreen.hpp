#ifndef UE4SS_SDK_RespawnScreen_HPP
#define UE4SS_SDK_RespawnScreen_HPP

class URespawnScreen_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* ZombiesKilled;                                                  // 0x0348 (size: 0x8)
    class UTextBlock* TextBlock_6;                                                    // 0x0350 (size: 0x8)
    class UTextBlock* TextBlock_3;                                                    // 0x0358 (size: 0x8)
    class UTextBlock* TextBlock_2;                                                    // 0x0360 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x0368 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x0370 (size: 0x8)
    class USaveMenu_C* SaveMenu;                                                      // 0x0378 (size: 0x8)
    class UButton* RespawnSpawnPointButton;                                           // 0x0380 (size: 0x8)
    class UButton* RespawnRandomButton;                                               // 0x0388 (size: 0x8)
    class UButton* QuitToMainMenuButton;                                              // 0x0390 (size: 0x8)
    class UButton* QuitButton;                                                        // 0x0398 (size: 0x8)
    class UButton* LoadSaveButton;                                                    // 0x03A0 (size: 0x8)
    class UTextBlock* HumansKilled;                                                   // 0x03A8 (size: 0x8)
    class UTextBlock* DistanceTravelled;                                              // 0x03B0 (size: 0x8)
    class UOverlay* DeadOverlay;                                                      // 0x03B8 (size: 0x8)
    class UTextBlock* DaysSurvived;                                                   // 0x03C0 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x03C8 (size: 0x8)

    FText Get_DistanceTravelled();
    FText Get_HumansKilled();
    FText Get_ZombiesKilled();
    FText Get_DaysSurvived();
    void ClearUI();
    void InpActEvt_Escape_K2Node_InputKeyEvent_0(FKey Key);
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__BP_RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitButton_K2Node_ComponentBoundEvent_7_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnButton_K2Node_ComponentBoundEvent_9_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnButton_K2Node_ComponentBoundEvent_11_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnSpawnPointButton_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_RespawnSpawnPointButton_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_LoadSaveButton_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__RespawnScreen_LoadSaveButton_K2Node_ComponentBoundEvent_6_OnButtonHoverEvent__DelegateSignature();
    void EventReturnToMenu();
    void ExecuteUbergraph_RespawnScreen(int32 EntryPoint);
}; // Size: 0x3D0

#endif
