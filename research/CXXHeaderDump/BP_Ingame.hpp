#ifndef UE4SS_SDK_BP_Ingame_HPP
#define UE4SS_SDK_BP_Ingame_HPP

class UBP_Ingame_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UWidgetAnimation* ReloadingAnim;                                            // 0x02C8 (size: 0x8)
    class UWidgetAnimation* PowerAttackAnim;                                          // 0x02D0 (size: 0x8)
    class UWidgetAnimation* ShowNightVisionDur;                                       // 0x02D8 (size: 0x8)
    class UWidgetAnimation* ShowFlashlightDur;                                        // 0x02E0 (size: 0x8)
    class UWidgetAnimation* ShowRespiratorDur;                                        // 0x02E8 (size: 0x8)
    class UWidgetAnimation* OxygenAnim;                                               // 0x02F0 (size: 0x8)
    class UWidgetAnimation* ActionBarAnim;                                            // 0x02F8 (size: 0x8)
    class UWidgetAnimation* StatsAnim;                                                // 0x0300 (size: 0x8)
    class UWidgetAnimation* HitmarkerEffect;                                          // 0x0308 (size: 0x8)
    class UWidgetAnimation* HitEffect;                                                // 0x0310 (size: 0x8)
    class UWidgetAnimation* FadeOut;                                                  // 0x0318 (size: 0x8)
    class UWidgetAnimation* FadeIn;                                                   // 0x0320 (size: 0x8)
    class UWidgetAnimation* Shoot;                                                    // 0x0328 (size: 0x8)
    class UWidgetAnimation* CombatCrosshairFadeOut;                                   // 0x0330 (size: 0x8)
    class UWidgetAnimation* InteractCrosshairFadeOut;                                 // 0x0338 (size: 0x8)
    class UWidgetAnimation* CombatCrosshairFadeIn;                                    // 0x0340 (size: 0x8)
    class UWidgetAnimation* InteractCrosshairFadeIn;                                  // 0x0348 (size: 0x8)
    class UBorder* ActionBar;                                                         // 0x0350 (size: 0x8)
    class UBorder* AmmoBox;                                                           // 0x0358 (size: 0x8)
    class UProgressBar* BufferBar;                                                    // 0x0360 (size: 0x8)
    class UBorder* BuildBox;                                                          // 0x0368 (size: 0x8)
    class UTextBlock* BuildingDegrees;                                                // 0x0370 (size: 0x8)
    class UImage* CombatCrosshair;                                                    // 0x0378 (size: 0x8)
    class UCompass_C* Compass;                                                        // 0x0380 (size: 0x8)
    class UCanvasPanel* CrosshairBox;                                                 // 0x0388 (size: 0x8)
    class UTextBlock* CurrentAmmoTxt;                                                 // 0x0390 (size: 0x8)
    class UOverlay* CurrentEffectsOverlay;                                            // 0x0398 (size: 0x8)
    class UVerticalBox* CurrentEffectsVB;                                             // 0x03A0 (size: 0x8)
    class UTextBlock* CurrentFirearmName;                                             // 0x03A8 (size: 0x8)
    class UTextBlock* CurrentXPText;                                                  // 0x03B0 (size: 0x8)
    class UImage* DamageEffect;                                                       // 0x03B8 (size: 0x8)
    class UTextBlock* EnableSnappingText;                                             // 0x03C0 (size: 0x8)
    class UImage* FireMode;                                                           // 0x03C8 (size: 0x8)
    class UTextBlock* FishingRodText;                                                 // 0x03D0 (size: 0x8)
    class UBorder* FlashlightDur;                                                     // 0x03D8 (size: 0x8)
    class UTextBlock* FlashlightText;                                                 // 0x03E0 (size: 0x8)
    class UProgressBarLinear_C* HealthBar;                                            // 0x03E8 (size: 0x8)
    class UTextBlock* HealthText;                                                     // 0x03F0 (size: 0x8)
    class UImage* HitmarkerCrosshair;                                                 // 0x03F8 (size: 0x8)
    class UTextBlock* Hotbar6Text;                                                    // 0x0400 (size: 0x8)
    class UTextBlock* Hotbar7Text;                                                    // 0x0408 (size: 0x8)
    class UTextBlock* Hotbar8Text;                                                    // 0x0410 (size: 0x8)
    class UProgressBarCircular_C* HungerBar;                                          // 0x0418 (size: 0x8)
    class UImage* HungerIcon;                                                         // 0x0420 (size: 0x8)
    class UTextBlock* HungerText;                                                     // 0x0428 (size: 0x8)
    class UImage* Image;                                                              // 0x0430 (size: 0x8)
    class UImage* Image_1;                                                            // 0x0438 (size: 0x8)
    class UImage* Image_78;                                                           // 0x0440 (size: 0x8)
    class UImage* InteractCrosshair;                                                  // 0x0448 (size: 0x8)
    class UJournalUI_C* JournalUI;                                                    // 0x0450 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Consumable1;                                  // 0x0458 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Consumable2;                                  // 0x0460 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Consumable3;                                  // 0x0468 (size: 0x8)
    class UJSIContainer_C* JSIContainer_FishingRod;                                   // 0x0470 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Grenade;                                      // 0x0478 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Melee;                                        // 0x0480 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Primary;                                      // 0x0488 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Secondary;                                    // 0x0490 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Sidearm;                                      // 0x0498 (size: 0x8)
    class UTextBlock* LevelText;                                                      // 0x04A0 (size: 0x8)
    class UMapWidget_C* MapWidget;                                                    // 0x04A8 (size: 0x8)
    class UTextBlock* MaxAmmoTxt;                                                     // 0x04B0 (size: 0x8)
    class UTextBlock* MaxXP;                                                          // 0x04B8 (size: 0x8)
    class UTextBlock* MeleeText;                                                      // 0x04C0 (size: 0x8)
    class UBorder* NightVisionDur;                                                    // 0x04C8 (size: 0x8)
    class UTextBlock* NightVisionText;                                                // 0x04D0 (size: 0x8)
    class UOverlay* Overlay_Fishing;                                                  // 0x04D8 (size: 0x8)
    class UProgressBarLinear_C* OxygenBar;                                            // 0x04E0 (size: 0x8)
    class UOverlay* OxygenStat;                                                       // 0x04E8 (size: 0x8)
    class UTextBlock* OxygenText;                                                     // 0x04F0 (size: 0x8)
    class UTextBlock* PlaceText;                                                      // 0x04F8 (size: 0x8)
    class UTextBlock* PrimaryText;                                                    // 0x0500 (size: 0x8)
    class UTextBlock* RadText;                                                        // 0x0508 (size: 0x8)
    class UTextBlock* RangeFinderDistance;                                            // 0x0510 (size: 0x8)
    class UImage* ReloadingImg;                                                       // 0x0518 (size: 0x8)
    class UBorder* RespiratorDur;                                                     // 0x0520 (size: 0x8)
    class UTextBlock* RespiratorText;                                                 // 0x0528 (size: 0x8)
    class UTextBlock* SecondaryText;                                                  // 0x0530 (size: 0x8)
    class UTextBlock* SidearmText;                                                    // 0x0538 (size: 0x8)
    class UProgressBarLinear_C* StaminaBar;                                           // 0x0540 (size: 0x8)
    class UTextBlock* StaminaText;                                                    // 0x0548 (size: 0x8)
    class UBorder* Stats;                                                             // 0x0550 (size: 0x8)
    class UImage* StopFiringCrosshair;                                                // 0x0558 (size: 0x8)
    class UTextBlock* StopText;                                                       // 0x0560 (size: 0x8)
    class UProgressBarCircular_C* ThirstBar;                                          // 0x0568 (size: 0x8)
    class UImage* ThirstIcon;                                                         // 0x0570 (size: 0x8)
    class UTextBlock* ThirstText;                                                     // 0x0578 (size: 0x8)
    class UTextBlock* ThrowableText;                                                  // 0x0580 (size: 0x8)
    class UProgressBarCircular_C* ToxinBar;                                           // 0x0588 (size: 0x8)
    class UImage* ToxinIcon;                                                          // 0x0590 (size: 0x8)
    class UUI_RadialMenu_C* UI_RadialMenu;                                            // 0x0598 (size: 0x8)
    class UVerticalBox* VerticalBox_223;                                              // 0x05A0 (size: 0x8)
    class UProgressBar* XPBar;                                                        // 0x05A8 (size: 0x8)
    TMap<class UJSI_Slot_C*, class UJSI_Slot_C*> MemicJSI;                            // 0x05B0 (size: 0x50)
    class UCameraComponent* Rotation;                                                 // 0x0600 (size: 0x8)
    int32 FPS;                                                                        // 0x0608 (size: 0x4)
    bool CombatCrosshairEnabled;                                                      // 0x060C (size: 0x1)
    bool InteractCrosshairEnabled;                                                    // 0x060D (size: 0x1)
    bool StopFiringCrosshairEnabled;                                                  // 0x060E (size: 0x1)
    class ULevellingComponent_C* LevelComponent;                                      // 0x0610 (size: 0x8)
    bool FPSVisible;                                                                  // 0x0618 (size: 0x1)
    double TimeHeld;                                                                  // 0x0620 (size: 0x8)
    double MaxTimeHeld;                                                               // 0x0628 (size: 0x8)
    bool Opened?;                                                                     // 0x0630 (size: 0x1)
    class UMedicalUI_C* MedUI;                                                        // 0x0638 (size: 0x8)
    class UMedicalUI_Bleed_C* BleedUI;                                                // 0x0640 (size: 0x8)
    class UMedicalUI_HeavyBleed_C* HeavyBleedUI;                                      // 0x0648 (size: 0x8)
    class UMedicalUI_BrokenBone_C* BrokenBoneUI;                                      // 0x0650 (size: 0x8)
    class UMedicalUI_Radiation_C* RadiationUI;                                        // 0x0658 (size: 0x8)
    class UFilterDurabilityUI_C* FilterUI;                                            // 0x0660 (size: 0x8)

    void ReloadingUI(bool Show?);
    void UnequipExisting(class UJSI_Slot_C* Item);
    void GetXPUI(double CurrentXP, double MaxXP);
    void GetLevelUI(int32 Level);
    void BindEvents();
    void SetHotbarText();
    void MedicalUIAnimation(FName MedicalEffect);
    void CreateMedicalUI(FName MedicalEffect, bool Adding?);
    void Hitmarker(FLinearColor InColorAndOpacity);
    void GetActionbarUIDs(TArray<FGuid>& UIDs);
    float GetBufferBarPercent();
    float GetCurrentPercent();
    class UW_PlayerMarker_C* AddScreenMarker(FS_MarkerData Marker Data, class AActor* Target Actor, int32 Index);
    void FindAndUnbindCapacity(class UJSI_Slot_C* ItemRef);
    void GetHotbarByIndex(int32 Index, class UJSI_Slot_C*& ItemRef, bool& Found?, class UJSIContainer_C*& ContainerRef, class UJSI_Slot_C*& MainItem);
    void Construct();
    void Crosshairs_Scale(float NewValue);
    void Event_VisibilityChanged(ESlateVisibility InVisibility);
    void GetRespiratorDurability(double Dur);
    void ShowRespiratorDurability(bool Show?);
    void ShowFlashlightDurability(bool Show?);
    void GetFlashlightDurability(double Dur);
    void ShowNightVisionDurability(bool Show?);
    void GetNightVisionDurability(double Dur);
    void ActionBar_Keys(bool NewValue);
    void UpdateRangeFinderDistance(int32 Degree);
    void UpdateBuildingDegree(double Degree);
    void ChangeSnappingModeText(bool Enable?);
    void AllCrosshairsVisibility(ESlateVisibility InVisibility);
    void OxygenVisibility(bool Show?);
    void UpdateOxygen(double Oxygen);
    void StatsVisibilty(bool Hide?);
    void ChangeFiringMode(bool Auto?);
    void UpdateFirearmName(FText Text);
    void SetCrosshair();
    void UpdateRadiation(double Radiation);
    void UpdateThirst(double thirst);
    void UpdateHunger(double hunger);
    void UpdateHealth(double Health);
    void UpdateStamina(double Stamina);
    void SetInteractCrosshairType();
    void SetCombatCrosshairType();
    void OnCapacityUpdated_Event_0(int32 Current, int32 Max, class UJSIContainer_C* Container, class UJSI_Slot_C* SlotMother);
    void ClearContainers();
    void EventOnMouseBtnDown(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void OnItemRemoved_Event_0(class UJSI_Slot_C* ItemRef, class UJSIContainer_C* FromContainer);
    void EventOnSomethingDropped(class UJSIContainer_C* FromContainer, class UJSIContainer_C* ToContainer, class UJSI_Slot_C* DroppedSlotRef, class UJSI_Slot_C* SlotReceiverRef, int32 ToSlotIndex, bool Rotated?);
    void OnInitialized();
    void HideAmmo/Name();
    void UpdateAmmo(int32 Current, int32 Max);
    void CrosshairShoot();
    void Event_HideStopFiringCrosshair();
    void Event_StopFiringCrosshair();
    void ExecuteUbergraph_BP_Ingame(int32 EntryPoint);
}; // Size: 0x668

#endif
