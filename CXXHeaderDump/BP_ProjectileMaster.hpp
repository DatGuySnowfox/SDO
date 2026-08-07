#ifndef UE4SS_SDK_BP_ProjectileMaster_HPP
#define UE4SS_SDK_BP_ProjectileMaster_HPP

class ABP_ProjectileMaster_C : public AActor
{
    class UStaticMeshComponent* StaticMesh;                                           // 0x0298 (size: 0x8)
    class UProjectileMovementComponent* ProjectileMovement;                           // 0x02A0 (size: 0x8)
    class ABP_FirearmPickup_C* WeaponOwner;                                           // 0x02A8 (size: 0x8)
    double MinDmg;                                                                    // 0x02B0 (size: 0x8)
    double MaxDmg;                                                                    // 0x02B8 (size: 0x8)

    void SpawnCombatText(FVector Location, double Damage);
}; // Size: 0x2C0

#endif
