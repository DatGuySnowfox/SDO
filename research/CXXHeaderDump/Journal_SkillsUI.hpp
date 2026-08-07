#ifndef UE4SS_SDK_Journal_SkillsUI_HPP
#define UE4SS_SDK_Journal_SkillsUI_HPP

class UJournal_SkillsUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* Current_FirstAidLevel;                                          // 0x02C8 (size: 0x8)
    class UTextBlock* Current_FirstAidXP;                                             // 0x02D0 (size: 0x8)
    class UTextBlock* Current_FishingLevel;                                           // 0x02D8 (size: 0x8)
    class UTextBlock* Current_FishingXP;                                              // 0x02E0 (size: 0x8)
    class UTextBlock* Current_FitnessLevel;                                           // 0x02E8 (size: 0x8)
    class UTextBlock* Current_FitnessXP;                                              // 0x02F0 (size: 0x8)
    class UTextBlock* Current_MarksmanshipLevel;                                      // 0x02F8 (size: 0x8)
    class UTextBlock* Current_MarksmanshipXP;                                         // 0x0300 (size: 0x8)
    class UTextBlock* Current_ReloadingLevel;                                         // 0x0308 (size: 0x8)
    class UTextBlock* Current_ReloadingXP;                                            // 0x0310 (size: 0x8)
    class UTextBlock* Current_ScavengingLevel;                                        // 0x0318 (size: 0x8)
    class UTextBlock* Current_ScavengingXP;                                           // 0x0320 (size: 0x8)
    class UTextBlock* Current_SneakingLevel;                                          // 0x0328 (size: 0x8)
    class UTextBlock* Current_SneakingXP;                                             // 0x0330 (size: 0x8)
    class UTextBlock* Current_StrengthLevel;                                          // 0x0338 (size: 0x8)
    class UTextBlock* Current_StrengthXP;                                             // 0x0340 (size: 0x8)
    class UTextBlock* Current_ThiefLevel;                                             // 0x0348 (size: 0x8)
    class UTextBlock* Current_ThiefXP;                                                // 0x0350 (size: 0x8)
    class UTextBlock* Current_ToughnessLevel;                                         // 0x0358 (size: 0x8)
    class UTextBlock* Current_ToughnessXP;                                            // 0x0360 (size: 0x8)
    class UProgressBar* FirstAid_XPBar;                                               // 0x0368 (size: 0x8)
    class USizeBox* FirstAidBox;                                                      // 0x0370 (size: 0x8)
    class UProgressBar* Fishing_XPBar;                                                // 0x0378 (size: 0x8)
    class USizeBox* FishingBox;                                                       // 0x0380 (size: 0x8)
    class UProgressBar* Fitness_XPBar;                                                // 0x0388 (size: 0x8)
    class USizeBox* FitnessBox;                                                       // 0x0390 (size: 0x8)
    class UImage* InfoMarker;                                                         // 0x0398 (size: 0x8)
    class UProgressBar* Marksmanship_XPBar;                                           // 0x03A0 (size: 0x8)
    class USizeBox* MarksmanshipBox;                                                  // 0x03A8 (size: 0x8)
    class UTextBlock* Max_FirstAidMaxXP;                                              // 0x03B0 (size: 0x8)
    class UTextBlock* Max_FishingMaxXP;                                               // 0x03B8 (size: 0x8)
    class UTextBlock* Max_FitnessMaxXP;                                               // 0x03C0 (size: 0x8)
    class UTextBlock* Max_MarksmanshipMaxXP;                                          // 0x03C8 (size: 0x8)
    class UTextBlock* Max_ReloadingMaxXP;                                             // 0x03D0 (size: 0x8)
    class UTextBlock* Max_ScavengingMaxXP;                                            // 0x03D8 (size: 0x8)
    class UTextBlock* Max_SneakingMaxXP;                                              // 0x03E0 (size: 0x8)
    class UTextBlock* Max_StrengthMaxXP;                                              // 0x03E8 (size: 0x8)
    class UTextBlock* Max_ThiefMaxXP;                                                 // 0x03F0 (size: 0x8)
    class UTextBlock* Max_ToughnessMaxXP;                                             // 0x03F8 (size: 0x8)
    class UTextBlock* Next_FirstAidLevel;                                             // 0x0400 (size: 0x8)
    class UTextBlock* Next_FishingLevel;                                              // 0x0408 (size: 0x8)
    class UTextBlock* Next_FitnessLevel;                                              // 0x0410 (size: 0x8)
    class UTextBlock* Next_MarksmanshipLevel;                                         // 0x0418 (size: 0x8)
    class UTextBlock* Next_ReloadingLevel;                                            // 0x0420 (size: 0x8)
    class UTextBlock* Next_ScavengingLevel;                                           // 0x0428 (size: 0x8)
    class UTextBlock* Next_SneakingLevel;                                             // 0x0430 (size: 0x8)
    class UTextBlock* Next_StrengthLevel;                                             // 0x0438 (size: 0x8)
    class UTextBlock* Next_ThiefLevel;                                                // 0x0440 (size: 0x8)
    class UTextBlock* Next_ToughnessLevel;                                            // 0x0448 (size: 0x8)
    class UImage* Page;                                                               // 0x0450 (size: 0x8)
    class UImage* Points;                                                             // 0x0458 (size: 0x8)
    class UTextBlock* PointsText;                                                     // 0x0460 (size: 0x8)
    class UImage* QuestMarker;                                                        // 0x0468 (size: 0x8)
    class UProgressBar* Reloading_XPBar;                                              // 0x0470 (size: 0x8)
    class USizeBox* ReloadingBox;                                                     // 0x0478 (size: 0x8)
    class UButtonWidget_C* ResetButton;                                               // 0x0480 (size: 0x8)
    class UProgressBar* Scavenging_XPBar;                                             // 0x0488 (size: 0x8)
    class USizeBox* ScavengingBox;                                                    // 0x0490 (size: 0x8)
    class UImage* SkillsMarker;                                                       // 0x0498 (size: 0x8)
    class UTechTreeWidget* SkillTreeUI;                                               // 0x04A0 (size: 0x8)
    class UProgressBar* Sneaking_XPBar;                                               // 0x04A8 (size: 0x8)
    class USizeBox* SneakingBox;                                                      // 0x04B0 (size: 0x8)
    class UProgressBar* Strength_XPBar;                                               // 0x04B8 (size: 0x8)
    class USizeBox* StrengthBox;                                                      // 0x04C0 (size: 0x8)
    class UTextBlock* TextBlock_8;                                                    // 0x04C8 (size: 0x8)
    class UTextBlock* TextBlock_9;                                                    // 0x04D0 (size: 0x8)
    class UTextBlock* TextBlock_10;                                                   // 0x04D8 (size: 0x8)
    class UTextBlock* TextBlock_11;                                                   // 0x04E0 (size: 0x8)
    class UTextBlock* TextBlock_12;                                                   // 0x04E8 (size: 0x8)
    class UTextBlock* TextBlock_13;                                                   // 0x04F0 (size: 0x8)
    class UTextBlock* TextBlock_14;                                                   // 0x04F8 (size: 0x8)
    class UTextBlock* TextBlock_15;                                                   // 0x0500 (size: 0x8)
    class UTextBlock* TextBlock_16;                                                   // 0x0508 (size: 0x8)
    class UTextBlock* TextBlock_18;                                                   // 0x0510 (size: 0x8)
    class UProgressBar* Thief_XPBar;                                                  // 0x0518 (size: 0x8)
    class USizeBox* ThiefBox;                                                         // 0x0520 (size: 0x8)
    class UProgressBar* Toughness_XPBar;                                              // 0x0528 (size: 0x8)
    class USizeBox* ToughnessBox;                                                     // 0x0530 (size: 0x8)
    class UPassiveSkillsComponent_C* Skills Component;                                // 0x0538 (size: 0x8)
    class UTooltip_PassiveStats_C* Tooltip;                                           // 0x0540 (size: 0x8)

    void Update_SkillPoints(int32 Points);
    void Update_ScavengingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_FishingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_ThiefUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_MarksmanshipUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_ReloadingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_FirstAidUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_SneakingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_ToughnessUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_StrengthUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void Update_FitnessUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void BindEvents();
    class UWidget* Scavenging_ToolTipWidget();
    FText ScavengingXPMultiplier();
    FText FishingXPMultiplier();
    FText ThiefXPMultiplier();
    FText ReloadingXPMultiplier();
    FText MarksmanshipXPMultiplier();
    FText FirstAidXPMultiplier();
    FText SneakingXPMultiplier();
    FText ToughnessXPMultiplier();
    FText StrengthXPMultiplier();
    FText FitnessXPMultiplier();
    class UWidget* Fishing_ToolTipWidget();
    class UWidget* Thief_ToolTipWidget();
    class UWidget* Toughness_ToolTipWidget();
    class UWidget* Strength_ToolTipWidget();
    class UWidget* Sneaking_ToolTipWidget();
    class UWidget* Reloading_ToolTipWidget();
    class UWidget* FirstAid_ToolTipWidget();
    class UWidget* Marksmanship_ToolTipWidget();
    class UWidget* Fitness_ToolTipWidget();
    void Construct();
    void BndEvt__Journal_SkillsUI_ResetWidget_K2Node_ComponentBoundEvent_4_ButtonPressed__DelegateSignature();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_Journal_SkillsUI(int32 EntryPoint);
}; // Size: 0x548

#endif
