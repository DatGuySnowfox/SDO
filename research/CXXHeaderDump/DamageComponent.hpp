#ifndef UE4SS_SDK_DamageComponent_HPP
#define UE4SS_SDK_DamageComponent_HPP

class UDamageComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    bool CanDamage?;                                                                  // 0x00A8 (size: 0x1)
    double StandardHealthValue;                                                       // 0x00B0 (size: 0x8)
    double CurrentHealth;                                                             // 0x00B8 (size: 0x8)
    double MaxHealth;                                                                 // 0x00C0 (size: 0x8)
    FDamageComponent_CStandardDamage StandardDamage;                                  // 0x00C8 (size: 0x10)
    void StandardDamage(double Dmg, class AActor* DmgCauser);
    FDamageComponent_CDeath Death;                                                    // 0x00D8 (size: 0x10)
    void Death(class AActor* DmgCauser, bool Headshot);
    bool SaveValues?;                                                                 // 0x00E8 (size: 0x1)
    FDamageComponent_CSetBurning SetBurning;                                          // 0x00F0 (size: 0x10)
    void SetBurning(bool Burning?);
    class UParticleSystemComponent* FireFX;                                           // 0x0100 (size: 0x8)
    FVector2D TimeOfBurning;                                                          // 0x0108 (size: 0x10)
    FTimerHandle TakingDamageTimer;                                                   // 0x0118 (size: 0x8)
    FTimerHandle TimeBurningTimer;                                                    // 0x0120 (size: 0x8)
    FDamageComponent_CSetCurrent&MaxHealth SetCurrent&MaxHealth;                      // 0x0128 (size: 0x10)
    void SetCurrent&MaxHealth(double CurrentHealth, double MaxHealth);
    double HealthRandomDeviation;                                                     // 0x0138 (size: 0x8)
    class USoundBase* DamageSound;                                                    // 0x0140 (size: 0x8)
    class USoundBase* DeathSound;                                                     // 0x0148 (size: 0x8)
    bool CanScaleHealth?;                                                             // 0x0150 (size: 0x1)
    class AActor* CurrentFireDamageGiver;                                             // 0x0158 (size: 0x8)
    bool CanHeadshot1Shot?;                                                           // 0x0160 (size: 0x1)

    void SetHealth(double Health, bool ScaleHealth?);
    void GetValues(double& StandardHealth, double& CurrentHealth, double& MaxHealth);
    void BurningFinished(bool Death?);
    void FireDamageTaken(double DmgAmount, class AActor* DmgCauser);
    void IsAlive?(bool& Alive?);
    void DamageTaken(double DmgAmount, const class UDamageType* DmgType, class AActor* DmgCauser);
    void ComponentPreSave();
    void ComponentLoaded();
    void ReceiveBeginPlay();
    void SetCurrent&MaxHealth_Event(double CurrentHealth, double MaxHealth);
    void Event_BloodSplatterFX();
    void ExecuteUbergraph_DamageComponent(int32 EntryPoint);
    void SetCurrent&MaxHealth__DelegateSignature(double CurrentHealth, double MaxHealth);
    void SetBurning__DelegateSignature(bool Burning?);
    void Death__DelegateSignature(class AActor* DmgCauser, bool Headshot);
    void StandardDamage__DelegateSignature(double Dmg, class AActor* DmgCauser);
}; // Size: 0x161

#endif
