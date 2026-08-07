#ifndef UE4SS_SDK_RespawnScreen_HPP
#define UE4SS_SDK_RespawnScreen_HPP

class URespawnScreen_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x02C8 (size: 0x8)
    class UTextBlock* DaysSurvived;                                                   // 0x02D0 (size: 0x8)
    class UOverlay* DeadOverlay;                                                      // 0x02D8 (size: 0x8)
    class UTextBlock* DistanceTravelled;                                              // 0x02E0 (size: 0x8)
    class UTextBlock* HumansKilled;                                                   // 0x02E8 (size: 0x8)
    class UButton* LoadSaveButton;                                                    // 0x02F0 (size: 0x8)
    class UButton* QuitButton;                                                        // 0x02F8 (size: 0x8)
    class UButton* QuitToMainMenuButton;                                              // 0x0300 (size: 0x8)
    class UButton* RespawnRandomButton;                                               // 0x0308 (size: 0x8)
    class UButton* RespawnSpawnPointButton;                                           // 0x0310 (size: 0x8)
    class USaveMenu_C* SaveMenu;                                                      // 0x0318 (size: 0x8)
    class UTextBlock* TextBlock;                                                      // 0x0320 (size: 0x8)
    class UTextBlock* TextBlock_1;                                                    // 0x0328 (size: 0x8)
    class UTextBlock* TextBlock_2;                                                    // 0x0330 (size: 0x8)
    class UTextBlock* TextBlock_3;                                                    // 0x0338 (size: 0x8)
    class UTextBlock* TextBlock_6;                                                    // 0x0340 (size: 0x8)
    class UTextBlock* ZombiesKilled;                                                  // 0x0348 (size: 0x8)

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
}; // Size: 0x350

#endif
