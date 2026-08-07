#ifndef UE4SS_SDK_RadiationComponent_HPP
#define UE4SS_SDK_RadiationComponent_HPP

class URadiationComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double MaxRadiation;                                                              // 0x00C0 (size: 0x8)
    double CurrentRadiation;                                                          // 0x00C8 (size: 0x8)
    FTimerHandle RadiationHandle;                                                     // 0x00D0 (size: 0x8)
    double ReduceRadiationAmount;                                                     // 0x00D8 (size: 0x8)
    FTimerHandle RadiationDamageHandle;                                               // 0x00E0 (size: 0x8)
    double RadDeduction;                                                              // 0x00E8 (size: 0x8)
    bool InRadArea;                                                                   // 0x00F0 (size: 0x1)
    class UAudioComponent* Geiger;                                                    // 0x00F8 (size: 0x8)
    FTimerHandle FilterHandle;                                                        // 0x0100 (size: 0x8)

    void GetRespiratorFilter(bool& Found?, FContainerPickupsInfo& ItemInfo);
    void GetRespiratorStat(FGameplayTag Stat, double& MinValue, double& MaxValue);
    void GetRespirator(bool& Found?, FContainerPickupsInfo& ItemInfo);
    void UseFilter(double MinusDur, double& NewDur, bool& Successful?);
    void DecreaseRadiation(double ReduceAmount);
    void IncreaseRadiation(double Amount);
    void ApplyRadiationDamage();
    void RadiationTimer();
    void ComponentLoaded();
    void ComponentPreSave();
    void UpdateRadiation(float Time);
    void CheckStats();
    void UpdateGeiger();
    void UpdateFilter();
    void Mask_Filter();
    void SetFilterUI(bool Show?);
    void StopRadiation();
    void FilterStoppedWorking();
    void ExecuteUbergraph_RadiationComponent(int32 EntryPoint);
}; // Size: 0x108

#endif
