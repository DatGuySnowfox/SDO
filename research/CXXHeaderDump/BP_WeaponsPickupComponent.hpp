#ifndef UE4SS_SDK_BP_WeaponsPickupComponent_HPP
#define UE4SS_SDK_BP_WeaponsPickupComponent_HPP

class UBP_WeaponsPickupComponent_C : public UActorComponent
{
    TArray<UJigsawItem_DataAsset_C*> Sights;                                          // 0x00B8 (size: 0x10)
    TArray<UJigsawItem_DataAsset_C*> Scopes;                                          // 0x00C8 (size: 0x10)
    class UJigsawItem_DataAsset_C* CurrentSight;                                      // 0x00D8 (size: 0x8)
    bool SightAttached?;                                                              // 0x00E0 (size: 0x1)
    class UJigsawItem_DataAsset_C* CurrentSuppressor;                                 // 0x00E8 (size: 0x8)
    bool SuppressorAttached?;                                                         // 0x00F0 (size: 0x1)
    bool MuzzleBrakeAttached?;                                                        // 0x00F1 (size: 0x1)
    class UJigsawItem_DataAsset_C* CurrentMuzzleBrake;                                // 0x00F8 (size: 0x8)
    TArray<UJigsawItem_DataAsset_C*> Suppressors;                                     // 0x0100 (size: 0x10)
    TArray<UJigsawItem_DataAsset_C*> MuzzleBrakes;                                    // 0x0110 (size: 0x10)
    class USoundBase* SwingSound;                                                     // 0x0120 (size: 0x8)
    bool MeleeHasPushback?;                                                           // 0x0128 (size: 0x1)
    double DelayBetweenSwings;                                                        // 0x0130 (size: 0x8)
    bool CanDamageTree?;                                                              // 0x0138 (size: 0x1)
    bool CanDamageRock?;                                                              // 0x0139 (size: 0x1)
    bool CanDamageTrashObjects?;                                                      // 0x013A (size: 0x1)
    TArray<AActor*> HitActors;                                                        // 0x0140 (size: 0x10)
    double SwingSpeed;                                                                // 0x0150 (size: 0x8)
    TArray<UAnimMontage*> NormalMeleeAttackMontages;                                  // 0x0158 (size: 0x10)
    TArray<UAnimMontage*> PowerMeleeAttackMontages;                                   // 0x0168 (size: 0x10)
    class UAnimMontage* CrouchedMeleeAttackMontage;                                   // 0x0178 (size: 0x8)
    TSubclassOf<class ULegacyCameraShake> CameraShake;                                // 0x0180 (size: 0x8)
    class UAnimMontage* RecoilMontage;                                                // 0x0188 (size: 0x8)
    class UAnimMontage* ReloadMontage;                                                // 0x0190 (size: 0x8)
    class UAnimMontage* LoopReloadMontage;                                            // 0x0198 (size: 0x8)
    double ReloadBaseSpeed;                                                           // 0x01A0 (size: 0x8)
    class UAnimMontage* ChamberFirearmMontage;                                        // 0x01A8 (size: 0x8)
    class UAnimMontage* ChamberFirearmAfterReloadMontage;                             // 0x01B0 (size: 0x8)
    double ChamberBaseSpeed;                                                          // 0x01B8 (size: 0x8)
    FRecoil_Struct RecoilData;                                                        // 0x01C0 (size: 0x20)
    double ResetRecoilDivider;                                                        // 0x01E0 (size: 0x8)
    double RecoilRate;                                                                // 0x01E8 (size: 0x8)
    bool UseShotgunSpread?;                                                           // 0x01F0 (size: 0x1)
    double ShotgunMinSpread;                                                          // 0x01F8 (size: 0x8)
    double ShotgunMaxSpread;                                                          // 0x0200 (size: 0x8)
    bool IsWeaponAutomatic?;                                                          // 0x0208 (size: 0x1)
    TArray<UJigsawItem_DataAsset_C*> SupportedMags/Ammo;                              // 0x0210 (size: 0x10)
    bool HasInternalMag?;                                                             // 0x0220 (size: 0x1)
    bool DoesWeaponUseAProjectile?;                                                   // 0x0221 (size: 0x1)
    TSubclassOf<class ABP_ProjectileMaster_C> ProjectileClass;                        // 0x0228 (size: 0x8)
    double DefaultSoundRadius;                                                        // 0x0230 (size: 0x8)
    class USoundBase* FirearmSound;                                                   // 0x0238 (size: 0x8)
    class USoundBase* FirearmSuppressedSound;                                         // 0x0240 (size: 0x8)
    class USoundBase* FirearmChamberSound;                                            // 0x0248 (size: 0x8)
    class USoundBase* MagOutSound;                                                    // 0x0250 (size: 0x8)
    class USoundBase* MagInSound;                                                     // 0x0258 (size: 0x8)
    class UParticleSystem* MuzzleFlash;                                               // 0x0260 (size: 0x8)
    class UParticleSystem* SuppressedMuzzleFlash;                                     // 0x0268 (size: 0x8)
    class UParticleSystem* BrassType;                                                 // 0x0270 (size: 0x8)
    double ShootingSpread;                                                            // 0x0278 (size: 0x8)
    double HipfireSpread;                                                             // 0x0280 (size: 0x8)
    double FirearmMalfunctionChance;                                                  // 0x0288 (size: 0x8)
    double MuzzleDurabilityMultiplier;                                                // 0x0290 (size: 0x8)
    double FireRateIfNoRPM;                                                           // 0x0298 (size: 0x8)
    int32 ShotgunPellets;                                                             // 0x02A0 (size: 0x4)
    bool IsFirearm?;                                                                  // 0x02A4 (size: 0x1)
    TEnumAsByte<Enum_Firearms::Type> FirearmType;                                     // 0x02A5 (size: 0x1)
    int32 MaxActorsToHitInSwing?;                                                     // 0x02A8 (size: 0x4)
    bool SpawnBrassRightAfterShot?;                                                   // 0x02AC (size: 0x1)

    void ScopeFunction();
    void SetCurrentMuzzleBrake(class UJigsawItem_DataAsset_C* CurrentCompensator, bool Compensator?);
    void SetCurrentSuppressor(class UJigsawItem_DataAsset_C* CurrentSuppressor, bool Suppressor?);
    void SetCurrentSight(class UJigsawItem_DataAsset_C* CurrentSight, bool Sight?);
    void GetSight(bool& Sight, bool& LongRange?, class UJigsawItem_DataAsset_C*& ID);
    void GetMuzzleBrake(bool& MuzzleBrakeAttached?);
    void GetSuppressor(bool& SuppressorAttached?);
}; // Size: 0x2AD

#endif
