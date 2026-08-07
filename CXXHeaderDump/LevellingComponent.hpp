#ifndef UE4SS_SDK_LevellingComponent_HPP
#define UE4SS_SDK_LevellingComponent_HPP

class ULevellingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    int32 CurrentLevel;                                                               // 0x00C0 (size: 0x4)
    int32 LevelCap;                                                                   // 0x00C4 (size: 0x4)
    double CurrentXP;                                                                 // 0x00C8 (size: 0x8)
    double CurrentMaxXP;                                                              // 0x00D0 (size: 0x8)
    double CurrentPercentage;                                                         // 0x00D8 (size: 0x8)
    double BufferXP;                                                                  // 0x00E0 (size: 0x8)
    double RemainingXP;                                                               // 0x00E8 (size: 0x8)
    double MultiplyValue;                                                             // 0x00F0 (size: 0x8)
    double Difficulty_MultiplierValue;                                                // 0x00F8 (size: 0x8)
    FLevellingComponent_CUpdateLevelUI UpdateLevelUI;                                 // 0x0100 (size: 0x10)
    void UpdateLevelUI(int32 Level);
    FLevellingComponent_CUpdateXPUI UpdateXPUI;                                       // 0x0110 (size: 0x10)
    void UpdateXPUI(double CurrentXP, double MaxXP);

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void Reset();
    void XPDeath();
    void LevelUp();
    void AddXP(double XP, bool LevelUp?, double& XPOutput);
    void CalculateMaxXP();
    void CalculatePercentage();
    void ActorLoaded();
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void LoadComponent();
    void Event_levelBar();
    void Event_LevelBarTimer();
    void Event_XPDeath();
    void Event_Multiplier(float NewValue);
    void ExecuteUbergraph_LevellingComponent(int32 EntryPoint);
    void UpdateXPUI__DelegateSignature(double CurrentXP, double MaxXP);
    void UpdateLevelUI__DelegateSignature(int32 Level);
}; // Size: 0x120

#endif
