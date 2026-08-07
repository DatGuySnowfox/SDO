#ifndef UE4SS_SDK_HUD_Game_HPP
#define UE4SS_SDK_HUD_Game_HPP

class AHUD_Game_C : public AHUD
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0388 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x0390 (size: 0x8)
    class UDaysSurvivedWidget_C* DaysSurvivedUI;                                      // 0x0398 (size: 0x8)
    class UNotificationParentWidget_C* NotificationUI;                                // 0x03A0 (size: 0x8)
    class UZoneAreaWidget_C* AreaUI;                                                  // 0x03A8 (size: 0x8)
    class UTimeUI_C* TimeUI;                                                          // 0x03B0 (size: 0x8)
    bool TimeIsOpen?;                                                                 // 0x03B8 (size: 0x1)
    class UAIHealthBarUI_C* AiHealthBar;                                              // 0x03C0 (size: 0x8)
    class UPauseScreen_C* PauseMenu;                                                  // 0x03C8 (size: 0x8)
    class UInteractionTimerUI_C* InteractTimerWidget;                                 // 0x03D0 (size: 0x8)
    class USafeZoneUI_C* SafeZoneUI;                                                  // 0x03D8 (size: 0x8)
    class UBP_InteractWidget_C* InteractWidget;                                       // 0x03E0 (size: 0x8)
    class UBP_LootContainerWidget_C* LootContainerWidget;                             // 0x03E8 (size: 0x8)
    class URadioTextUI_C* RadioTextUI;                                                // 0x03F0 (size: 0x8)
    class UBuildableRenameObject_C* ChangeNameWidget;                                 // 0x03F8 (size: 0x8)
    class USleepingUI_C* SleepUI;                                                     // 0x0400 (size: 0x8)
    class UPOIDiscoveredUI_C* POIDiscover;                                            // 0x0408 (size: 0x8)
    class UModularYesNoUI_C* YesNoUI;                                                 // 0x0410 (size: 0x8)

    void UpdateInteractStats(const FName StatToUpdate, FText Value, bool ClearOptions?);
    void UpdateLootOptions(const FGameplayTag OptionToUpdate, FText Text, bool ClearOptions?);
    void YesAndNoUI(bool Show?, FText Text, class UModularYesNoUI_C*& UI);
    void RemoveInteractAndLootWidgets();
    void DiscoverPOI(FStruct_POI POI);
    void ShowSleepWidget(bool Show?);
    void ShowChangeObjectNameWidget(bool Show?, class AActor* ObjectRef);
    void CreateRadioTextWidget(bool Visible?, FStruct_Radio RadioStruct, class ABP_Radio_Text_C* RadioRef);
    void ShowLootContainerWidget(bool Show?, class AActor* Actor, FText Container, FText LootType, bool Marked?, bool Looted?, const TMap<class FGameplayTag, class FText> Options);
    void UpdateInteractOptions(const FGameplayTag OptionToUpdate, FText Text, bool ClearOptions?);
    void ShowInteractWidget(bool Visible?, FText Name, FText AdditionalText, const TMap<class FGameplayTag, class FText> Options, TMap<class FName, class FText> Stats, class UWidget*& Widget);
    void SafeZoneAreaUI(bool Show?);
    void InteractTimerUI(bool Open?, FText InteractionText);
    void UpdateHealth(double Health, double MaxHealth);
    void ToggleAIHealthBar(bool Toggle, FString Name, bool Boss?);
    void Time();
    void BindAreaUIFinished();
    void Notification(FS_NotificationDetails NotificationDetails);
    void DaysSurvivedHUD();
    void ReceiveBeginPlay();
    void Client_PauseGame(bool Toggle);
    void Client_ForceUnpause(class UWidget* Target);
    void Re-Initialise();
    void BindEnteringFinished();
    void BindLeavingFinished();
    void ZoneAreaUI(bool Entering?, FText Area Name, class UTexture* Texture, FLinearColor Color);
    void ExecuteUbergraph_HUD_Game(int32 EntryPoint);
}; // Size: 0x418

#endif
