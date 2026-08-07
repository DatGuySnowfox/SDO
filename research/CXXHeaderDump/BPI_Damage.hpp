#ifndef UE4SS_SDK_BPI_Damage_HPP
#define UE4SS_SDK_BPI_Damage_HPP

class IBPI_Damage_C : public IInterface
{

    void IsObjectDamageable?(bool& Damageable?);
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void Damage_Shoved(bool Anim, double Force);
}; // Size: 0x28

#endif
