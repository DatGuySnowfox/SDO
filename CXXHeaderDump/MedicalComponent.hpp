#ifndef UE4SS_SDK_MedicalComponent_HPP
#define UE4SS_SDK_MedicalComponent_HPP

class UMedicalComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    bool Bleed?;                                                                      // 0x00C0 (size: 0x1)
    bool HeavyBleed?;                                                                 // 0x00C1 (size: 0x1)
    bool BrokenBone?;                                                                 // 0x00C2 (size: 0x1)
    class ABP_PlayerCharacter_C* Character;                                           // 0x00C8 (size: 0x8)
    double Health;                                                                    // 0x00D0 (size: 0x8)
    double MaxHealth;                                                                 // 0x00D8 (size: 0x8)
    FTimerHandle BleedTimer;                                                          // 0x00E0 (size: 0x8)
    FTimerHandle HeavyBleedTimer;                                                     // 0x00E8 (size: 0x8)
    FTimerHandle BleedStopTimer;                                                      // 0x00F0 (size: 0x8)
    FTimerHandle HeavyBleedStopTimer;                                                 // 0x00F8 (size: 0x8)
    FTimerHandle BrokenBoneStopTimer;                                                 // 0x0100 (size: 0x8)
    bool RadiationSickness?;                                                          // 0x0108 (size: 0x1)

    void IsPlayerInVehicle?(bool& InVehicle);
    void IsGPSEquipped?(bool& GPS?);
    void IsBurning?(bool& Burning?);
    void GetInGameUI(class UBP_Ingame_C*& UI);
    void GetPlayerRef(class ABP_PlayerCharacter_C*& Player);
    void OnRep_RadiationSickness?();
    void HeavyBleed();
    void Bleed();
    void SetPostProcessHealth(bool Damage);
    void IncreaseHealth(double Amount);
    void OnRep_BrokenBone?();
    void OnRep_HeavyBleed?();
    void OnRep_Bleed?();
    void ComponentPreSave();
    void SendStaminaToClient(double NewStamina);
    void UpdatePlayerSpeed(double NewSpeed);
    void SendHealthToClient(double NewHealth);
    void SendHungerToClient(double NewHunger);
    void SendThirstToClient(double NewThirst);
    void SendRadiationToClient(double NewRadiation);
    void PlayerDeath();
    void PlayMontage(class UAnimMontage* Montage, double Play Rate);
    void StopMontage(class UAnimMontage* Montage);
    void CreateNotificationUI(FText Text, class UTexture2D* Image, FLinearColor Color, double UI Delay);
    void SendOxygenToClient(double NewOxygen);
    void SetBurning(bool Burning, double Damage);
    void SetTraceToWorld(bool Set?);
    void VehicleInteraction(bool Enter?, const class ABP_VehicleMaster_C*& VehicleRef);
    void LoadComponent();
    void Event_TimeToStopBleed();
    void Event_TimeToStopHeavyBleed();
    void Event_TimeToStopBrokenBone();
    void Time_Bleed();
    void Time_HeavyBleed();
    void Time_BrokenBone();
    void ComponentLoaded();
    void Client_CreateMedUI(FName MedicalEffect, bool Adding?);
    void Svr_Damage(class AActor* DamagedActor, float BaseDamage);
    void Client_RemoveEffect(TArray<FName>& Array);
    void ExecuteUbergraph_MedicalComponent(int32 EntryPoint);
}; // Size: 0x109

#endif
