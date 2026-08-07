#ifndef UE4SS_SDK_PassiveSkillsComponent_HPP
#define UE4SS_SDK_PassiveSkillsComponent_HPP

class UPassiveSkillsComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double CurrentFitnessXP;                                                          // 0x00C0 (size: 0x8)
    double MaxFitnessXP;                                                              // 0x00C8 (size: 0x8)
    double CurrentFitnessLevel;                                                       // 0x00D0 (size: 0x8)
    double MaxFitnessLevel;                                                           // 0x00D8 (size: 0x8)
    double CurrentStrengthXP;                                                         // 0x00E0 (size: 0x8)
    double MaxStrengthXP;                                                             // 0x00E8 (size: 0x8)
    double CurrentStrengthLevel;                                                      // 0x00F0 (size: 0x8)
    double MaxStrengthLevel;                                                          // 0x00F8 (size: 0x8)
    double CurrentToughessXP;                                                         // 0x0100 (size: 0x8)
    double MaxToughnessXP;                                                            // 0x0108 (size: 0x8)
    double CurrentToughessLevel;                                                      // 0x0110 (size: 0x8)
    double MaxToughessLevel;                                                          // 0x0118 (size: 0x8)
    double CurrentSneakingXP;                                                         // 0x0120 (size: 0x8)
    double MaxSneakingXP;                                                             // 0x0128 (size: 0x8)
    double CurrentSneakingLevel;                                                      // 0x0130 (size: 0x8)
    double MaxSneakingLevel;                                                          // 0x0138 (size: 0x8)
    double CurrentFirstAidXP;                                                         // 0x0140 (size: 0x8)
    double MaxFirstAidXP;                                                             // 0x0148 (size: 0x8)
    double CurrentFirstAidLevel;                                                      // 0x0150 (size: 0x8)
    double MaxFirstAidLevel;                                                          // 0x0158 (size: 0x8)
    double CurrentMarksmanshipXP;                                                     // 0x0160 (size: 0x8)
    double MaxMarksmanshipXP;                                                         // 0x0168 (size: 0x8)
    double CurrentMarksmanshipLevel;                                                  // 0x0170 (size: 0x8)
    double MaxMarksmanshipLevel;                                                      // 0x0178 (size: 0x8)
    double CurrentReloadingXP;                                                        // 0x0180 (size: 0x8)
    double MaxReloadingXP;                                                            // 0x0188 (size: 0x8)
    double CurrentReloadingLevel;                                                     // 0x0190 (size: 0x8)
    double MaxReloadingLevel;                                                         // 0x0198 (size: 0x8)
    double FitnessPercentage;                                                         // 0x01A0 (size: 0x8)
    double StrengthPercentage;                                                        // 0x01A8 (size: 0x8)
    double ToughnessPercentage;                                                       // 0x01B0 (size: 0x8)
    double SneakingPercentage;                                                        // 0x01B8 (size: 0x8)
    double FirstAidPercentage;                                                        // 0x01C0 (size: 0x8)
    double MarksmanshipPercentage;                                                    // 0x01C8 (size: 0x8)
    double ReloadingPercentage;                                                       // 0x01D0 (size: 0x8)
    double FitnessMultiplier;                                                         // 0x01D8 (size: 0x8)
    double StrengthMultiplier;                                                        // 0x01E0 (size: 0x8)
    double ToughnessMultiplier;                                                       // 0x01E8 (size: 0x8)
    double SneakingMultiplier;                                                        // 0x01F0 (size: 0x8)
    double FirstAidMultiplier;                                                        // 0x01F8 (size: 0x8)
    double ReloadingMultiplier;                                                       // 0x0200 (size: 0x8)
    double MarksmanshipMultiplier;                                                    // 0x0208 (size: 0x8)
    double StrengthSpeedMultiplier;                                                   // 0x0210 (size: 0x8)
    double FitnessStaminaMultiplier;                                                  // 0x0218 (size: 0x8)
    double CurrentThiefXP;                                                            // 0x0220 (size: 0x8)
    double MaxThiefXP;                                                                // 0x0228 (size: 0x8)
    double CurrentThiefLevel;                                                         // 0x0230 (size: 0x8)
    double MaxThiefLevel;                                                             // 0x0238 (size: 0x8)
    double ThiefPercentage;                                                           // 0x0240 (size: 0x8)
    double ThiefMultiplier;                                                           // 0x0248 (size: 0x8)
    double CurrentFishingXP;                                                          // 0x0250 (size: 0x8)
    double MaxFishingXP;                                                              // 0x0258 (size: 0x8)
    double CurrentFishingLevel;                                                       // 0x0260 (size: 0x8)
    double MaxFishingLevel;                                                           // 0x0268 (size: 0x8)
    double FishingPercentage;                                                         // 0x0270 (size: 0x8)
    double FishingMultiplier;                                                         // 0x0278 (size: 0x8)
    double FitnessStaminaRegainMultiplier;                                            // 0x0280 (size: 0x8)
    double FitnessSwimmingMultiplier;                                                 // 0x0288 (size: 0x8)
    bool FitnessMultiplierUsed?;                                                      // 0x0290 (size: 0x1)
    bool StrengthMultiplierUsed?;                                                     // 0x0291 (size: 0x1)
    bool ToughnessMultiplierUsed?;                                                    // 0x0292 (size: 0x1)
    bool SneakingMultiplierUsed?;                                                     // 0x0293 (size: 0x1)
    bool FirstAidMultiplierUsed?;                                                     // 0x0294 (size: 0x1)
    bool MarksmanshipMultiplierUsed?;                                                 // 0x0295 (size: 0x1)
    bool ReloadingMultiplierUsed?;                                                    // 0x0296 (size: 0x1)
    bool ThiefMultiplierUsed?;                                                        // 0x0297 (size: 0x1)
    bool FishingMultiplierUsed?;                                                      // 0x0298 (size: 0x1)
    double CurrentScavengingXP;                                                       // 0x02A0 (size: 0x8)
    double MaxScavengingXP;                                                           // 0x02A8 (size: 0x8)
    double CurrentScavengingLevel;                                                    // 0x02B0 (size: 0x8)
    double MaxScavengingLevel;                                                        // 0x02B8 (size: 0x8)
    double ScavengingPercentage;                                                      // 0x02C0 (size: 0x8)
    double ScavengingMultiplier;                                                      // 0x02C8 (size: 0x8)
    bool ScavengingMultiplierUsed?;                                                   // 0x02D0 (size: 0x1)
    FPassiveSkillsComponent_CUpdateFitnessUI UpdateFitnessUI;                         // 0x02D8 (size: 0x10)
    void UpdateFitnessUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateStrengthUI UpdateStrengthUI;                       // 0x02E8 (size: 0x10)
    void UpdateStrengthUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateToughnessUI UpdateToughnessUI;                     // 0x02F8 (size: 0x10)
    void UpdateToughnessUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateSneakingUI UpdateSneakingUI;                       // 0x0308 (size: 0x10)
    void UpdateSneakingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateFirstAidUI UpdateFirstAidUI;                       // 0x0318 (size: 0x10)
    void UpdateFirstAidUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateReloadingUI UpdateReloadingUI;                     // 0x0328 (size: 0x10)
    void UpdateReloadingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateMarksmanshipUI UpdateMarksmanshipUI;               // 0x0338 (size: 0x10)
    void UpdateMarksmanshipUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateThiefUI UpdateThiefUI;                             // 0x0348 (size: 0x10)
    void UpdateThiefUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateFishingUI UpdateFishingUI;                         // 0x0358 (size: 0x10)
    void UpdateFishingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CUpdateScavengingUI UpdateScavengingUI;                   // 0x0368 (size: 0x10)
    void UpdateScavengingUI(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    FPassiveSkillsComponent_CComponentLoad ComponentLoad;                             // 0x0378 (size: 0x10)
    void ComponentLoad();

    void XPMultiplier_Scavenging(bool& Used?);
    void XPMultiplier_Thief(bool& Used?);
    void XPMultiplier_Reloading(bool& Used?);
    void XPMultiplier_Marksmanship(bool& Used?);
    void XPMultiplier_FirstAid(bool& Used?);
    void XPMultiplier_Sneaking(bool& Used?);
    void XPMultiplier_Toughness(bool& Used?);
    void XPMultiplier_Strength(bool& Used?);
    void XPMultiplier_Fitness(bool& Used?);
    void XPMultiplier_Fishing(bool& Used?);
    void LevelUpNotification(FText Skill, double Level);
    void Scavenging_LevelUp();
    void ResetPassiveSkills_NewSave();
    void Fishing_LevelUp();
    void Thief_LevelUp();
    void ResetPassiveSkills();
    void Reloading_LevelUp();
    void Marksmanship_LevelUp();
    void FirstAid_LevelUp();
    void Sneaking_LevelUp();
    void Toughness_LevelUp();
    void Strength_LevelUp();
    void Fitness_LevelUp();
    void CalculateMaxXP(double MaxXP, float& MaxXPLevel);
    void CalculatePercentage(double CurrentXP, double MaxXP, double& CurrentPercentage);
    void AddXP(double AddXP, double Skill, double& NewXP);
    void ComponentPreSave();
    void LoadComponent();
    void ComponentLoaded();
    void Event_ComponentLoad();
    void AddXP_Fitness(double float);
    void AddXP_Strength(double float);
    void AddXP_Toughness(double float);
    void AddXP_Sneaking(double float);
    void AddXP_FirstAid(double float);
    void AddXP_Marksmanship(double float);
    void AddXP_Reloading(double float);
    void AddXP_Thief(double float);
    void AddXP_Fishing(double float);
    void AddXP_Scavenging(double float);
    void ExecuteUbergraph_PassiveSkillsComponent(int32 EntryPoint);
    void ComponentLoad__DelegateSignature();
    void UpdateScavengingUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateFishingUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateThiefUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateMarksmanshipUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateReloadingUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateFirstAidUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateSneakingUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateToughnessUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateStrengthUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
    void UpdateFitnessUI__DelegateSignature(double CurrentXP, double MaxXP, double CurrentLevel, double NextLevel, double Percentage);
}; // Size: 0x388

#endif
