#ifndef UE4SS_SDK_RadiationComponent_HPP
#define UE4SS_SDK_RadiationComponent_HPP

class URadiationComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    double CurrentRadiation;                                                          // 0x00D8 (size: 0x8)
    double MaxRadiation;                                                              // 0x00E0 (size: 0x8)
    class UAudioComponent* Geiger;                                                    // 0x00E8 (size: 0x8)
    FTimerHandle RadiationHandle;                                                     // 0x00F0 (size: 0x8)
    FTimerHandle RadiationDamageHandle;                                               // 0x00F8 (size: 0x8)
    bool InRadArea;                                                                   // 0x0100 (size: 0x1)
    FTimerHandle FilterHandle;                                                        // 0x0108 (size: 0x8)

    void ClearTimers();
    void GetRespiratorFilter(bool& Found?, FContainerPickupsInfo& ItemInfo);
    void GetRespiratorStat(FGameplayTag Stat, double& MinValue, double& MaxValue);
    void GetRespirator(bool& Found?, FContainerPickupsInfo& ItemInfo);
    void UseFilter(double MinusDur, double& NewDur, bool& Successful?);
    void DecreaseRadiation(double ReduceAmount);
    void IncreaseRadiation(double Amount);
    void ApplyRadiationDamage();
    void RadiationTimer();
    void ComponentLoaded();
    void ComponentPreLoad();
    void ComponentPreSave();
    void ComponentSaved();
    void UpdateRadiation(float Time);
    void CheckStats();
    void StopRadiation();
    void StopGeiger();
    void SetFilterUI(bool Show?);
    void Mask_Filter();
    void UpdateFilter();
    void UpdateGeiger();
    void ExecuteUbergraph_RadiationComponent(int32 EntryPoint);
}; // Size: 0x110

#endif
