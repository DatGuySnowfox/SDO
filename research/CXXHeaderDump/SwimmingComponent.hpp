#ifndef UE4SS_SDK_SwimmingComponent_HPP
#define UE4SS_SDK_SwimmingComponent_HPP

class USwimmingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    double CurrentOxygen;                                                             // 0x00C0 (size: 0x8)
    double MaxOxygen;                                                                 // 0x00C8 (size: 0x8)
    bool IsSwimming?;                                                                 // 0x00D0 (size: 0x1)
    bool InWater?;                                                                    // 0x00D1 (size: 0x1)
    bool Fast?;                                                                       // 0x00D2 (size: 0x1)
    double Deepness;                                                                  // 0x00D8 (size: 0x8)
    bool UnderWater?;                                                                 // 0x00E0 (size: 0x1)
    class ABP_PlayerCharacter_C* PlayerRef;                                           // 0x00E8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x00F0 (size: 0x8)
    class UPassiveSkillsComponent_C* PassiveSkillsComponent;                          // 0x00F8 (size: 0x8)

    void OnRep_UnderWater?();
    void OnRep_IsSwimming?();
    void DealDamage();
    void OxygenTimer();
    void WaterDetection();
    void WaterTrace(double& Deepness, FVector& ImpactPoint);
    void ComponentLoaded();
    void ComponentPreSave();
    void LoadComponent();
    void ExecuteUbergraph_SwimmingComponent(int32 EntryPoint);
}; // Size: 0x100

#endif
