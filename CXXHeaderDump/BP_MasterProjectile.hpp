#ifndef UE4SS_SDK_BP_MasterProjectile_HPP
#define UE4SS_SDK_BP_MasterProjectile_HPP

class ABP_MasterProjectile_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USphereComponent* Projectile Collision;                                     // 0x02A0 (size: 0x8)
    class UProjectileMovementComponent* ProjectileMovement;                           // 0x02A8 (size: 0x8)
    TArray<class AActor*> Hit Actors;                                                 // 0x02B0 (size: 0x10)
    class AActor* Other Actor;                                                        // 0x02C0 (size: 0x8)
    class UPrimitiveComponent* Other Comp;                                            // 0x02C8 (size: 0x8)
    TMap<TEnumAsByte<E_HitboxTypeAI::Type>, double> Damage;                           // 0x02D0 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Surface Emitters;           // 0x0320 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Surface Decals;          // 0x0370 (size: 0x50)
    double Decal LifeSpan;                                                            // 0x03C0 (size: 0x8)
    double Decal Fade Distance;                                                       // 0x03C8 (size: 0x8)
    FVector Surface Decal Size;                                                       // 0x03D0 (size: 0x18)
    FVector Impact Point;                                                             // 0x03E8 (size: 0x18)
    FVector Impact Normal;                                                            // 0x0400 (size: 0x18)
    class UPhysicalMaterial* Phys Material;                                           // 0x0418 (size: 0x8)
    class UPrimitiveComponent* Hit Component;                                         // 0x0420 (size: 0x8)
    bool Allow Damage;                                                                // 0x0428 (size: 0x1)
    double DamageMulti;                                                               // 0x0430 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Surface Sound;                   // 0x0438 (size: 0x50)

    void ReceiveHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
    void ExecuteUbergraph_BP_MasterProjectile(int32 EntryPoint);
}; // Size: 0x488

#endif
