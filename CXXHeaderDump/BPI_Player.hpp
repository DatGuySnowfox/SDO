#ifndef UE4SS_SDK_BPI_Player_HPP
#define UE4SS_SDK_BPI_Player_HPP

class IBPI_Player_C : public IInterface
{

    void IsPlayerInVehicle?(bool& InVehicle);
    void VehicleInteraction(bool Enter?, const class ABP_VehicleMaster_C*& VehicleRef);
    void IsGPSEquipped?(bool& GPS?);
    void SetTraceToWorld(bool Set?);
    void IsBurning?(bool& Burning?);
    void SetBurning(bool Burning, double Damage);
    void GetInGameUI(class UBP_Ingame_C*& UI);
    void SendOxygenToClient(double NewOxygen);
    void CreateNotificationUI(FText Text, class UTexture2D* Image, FLinearColor Color, double UI Delay);
    void StopMontage(class UAnimMontage* Montage);
    void PlayMontage(class UAnimMontage* Montage, double Play Rate);
    void PlayerDeath();
    void SendRadiationToClient(double NewRadiation);
    void SendThirstToClient(double NewThirst);
    void SendHungerToClient(double NewHunger);
    void SendHealthToClient(double NewHealth);
    void UpdatePlayerSpeed(double NewSpeed);
    void GetPlayerRef(class ABP_PlayerCharacter_C*& Player);
    void SendStaminaToClient(double NewStamina);
}; // Size: 0x28

#endif
