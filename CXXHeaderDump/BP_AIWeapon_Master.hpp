#ifndef UE4SS_SDK_BP_AIWeapon_Master_HPP
#define UE4SS_SDK_BP_AIWeapon_Master_HPP

class ABP_AIWeapon_Master_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02A0 (size: 0x8)
    class USceneComponent* Particle Socket;                                           // 0x02A8 (size: 0x8)
    class UBillboardComponent* End Trace;                                             // 0x02B0 (size: 0x8)
    class UBillboardComponent* Start Trace;                                           // 0x02B8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02C0 (size: 0x8)
    class USceneComponent* Attach Point;                                              // 0x02C8 (size: 0x8)
    class USceneComponent* RangeStartPoint;                                           // 0x02D0 (size: 0x8)
    class UArrowComponent* Arrow;                                                     // 0x02D8 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02E0 (size: 0x8)
    bool Use Equip Animation;                                                         // 0x02E8 (size: 0x1)
    class UAnimMontage* Equip Animation;                                              // 0x02F0 (size: 0x8)
    FName Equip Attach Socket;                                                        // 0x02F8 (size: 0x8)
    bool Use Unequip Animation;                                                       // 0x0300 (size: 0x1)
    class UAnimMontage* Unequip Animation;                                            // 0x0308 (size: 0x8)
    FName Unequip Attach Socket;                                                      // 0x0310 (size: 0x8)
    TMap<TEnumAsByte<E_HitboxTypeAI::Type>, double> Damage;                           // 0x0318 (size: 0x50)
    class UBP_SmartAIComponent_C* AI Component;                                       // 0x0368 (size: 0x8)
    int32 Max Number Of Hits;                                                         // 0x0370 (size: 0x4)
    TEnumAsByte<E_OrderOptions::Type> Melee Attack Anims Order;                       // 0x0374 (size: 0x1)
    TArray<class UAnimMontage*> Melee Animations;                                     // 0x0378 (size: 0x10)
    TArray<class UAnimMontage*> Lower Melee Animations;                               // 0x0388 (size: 0x10)
    bool Can Melee Attack While Moving;                                               // 0x0398 (size: 0x1)
    double Melee Attack Frequency Min;                                                // 0x03A0 (size: 0x8)
    double Melee Attack Frequency Max;                                                // 0x03A8 (size: 0x8)
    bool Use Melee Anim Time Frequancy;                                               // 0x03B0 (size: 0x1)
    bool Can Melee Attack;                                                            // 0x03B1 (size: 0x1)
    bool Reloading;                                                                   // 0x03B2 (size: 0x1)
    int32 Current Hits;                                                               // 0x03B4 (size: 0x4)
    TArray<class AActor*> Hit Actors;                                                 // 0x03B8 (size: 0x10)
    int32 Current Ammo;                                                               // 0x03C8 (size: 0x4)
    class USoundCue* RangeAttackSound;                                                // 0x03D0 (size: 0x8)
    bool Can Range Attack;                                                            // 0x03D8 (size: 0x1)
    float Weapon Noise;                                                               // 0x03DC (size: 0x4)
    double Range Attack Distance;                                                     // 0x03E0 (size: 0x8)
    double Range (Trace);                                                             // 0x03E8 (size: 0x8)
    double Spread (Trace);                                                            // 0x03F0 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Surface Emitters (Trace);   // 0x03F8 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Surface Decal (Trace);   // 0x0448 (size: 0x50)
    FVector Surface Decal Size (Trace);                                               // 0x0498 (size: 0x18)
    double Decal LifeSpan (Trace);                                                    // 0x04B0 (size: 0x8)
    double Decal Fade Distance (Trace);                                               // 0x04B8 (size: 0x8)
    double Radius (Trace);                                                            // 0x04C0 (size: 0x8)
    double Max Hit Deviation (Projectile);                                            // 0x04C8 (size: 0x8)
    bool Use Projectile Actor Settings;                                               // 0x04D0 (size: 0x1)
    double Projectile Homing Acceleration Magnitude;                                  // 0x04D8 (size: 0x8)
    TSubclassOf<class AActor> Range Projectile;                                       // 0x04E0 (size: 0x8)
    double Accuracy (Projectile);                                                     // 0x04E8 (size: 0x8)
    double Max Miss Deviation (Projectile);                                           // 0x04F0 (size: 0x8)
    bool Homing Projectile;                                                           // 0x04F8 (size: 0x1)
    class UAnimMontage* Character Reload Montage;                                     // 0x0500 (size: 0x8)
    bool Can Reload;                                                                  // 0x0508 (size: 0x1)
    bool Use Ammo;                                                                    // 0x0509 (size: 0x1)
    int32 Max Total Ammo;                                                             // 0x050C (size: 0x4)
    int32 Max Magazine Ammo;                                                          // 0x0510 (size: 0x4)
    TEnumAsByte<E_NoAmmo::Type> No Ammo Behaviour;                                    // 0x0514 (size: 0x1)
    bool Use Holdable Anims;                                                          // 0x0515 (size: 0x1)
    FS_AllAIAnims Weapon Animations;                                                  // 0x0518 (size: 0x4C8)
    class UAnimMontage* Holdable Mesh Reload Anim;                                    // 0x09E0 (size: 0x8)
    bool Allow Emote Anims;                                                           // 0x09E8 (size: 0x1)
    TEnumAsByte<E_OrderOptions::Type> Range Attack Anims Order;                       // 0x09E9 (size: 0x1)
    TArray<class UAnimMontage*> Range Attack Anims;                                   // 0x09F0 (size: 0x10)
    double Range Attack Frequency Min;                                                // 0x0A00 (size: 0x8)
    double Range Attack Frequency Max;                                                // 0x0A08 (size: 0x8)
    bool Use Range Anim Time Frequancy;                                               // 0x0A10 (size: 0x1)
    bool Can Range Attack While Moving;                                               // 0x0A11 (size: 0x1)
    bool Start With Full Magazine;                                                    // 0x0A12 (size: 0x1)
    class UAnimMontage* Holdable Mesh Shoot Anim;                                     // 0x0A18 (size: 0x8)
    class UParticleSystemComponent* Muzzle Effect;                                    // 0x0A20 (size: 0x8)
    class UParticleSystem* Range Attack Particle Effect;                              // 0x0A28 (size: 0x8)
    FHitResult Effect Hit Result;                                                     // 0x0A30 (size: 0xE8)
    bool Delay Range Attack;                                                          // 0x0B18 (size: 0x1)
    double Range Delay Time;                                                          // 0x0B20 (size: 0x8)
    double Melee Trace Radius;                                                        // 0x0B28 (size: 0x8)
    FTimerHandle Melee Timer;                                                         // 0x0B30 (size: 0x8)
    bool Can Block;                                                                   // 0x0B38 (size: 0x1)
    TMap<TEnumAsByte<E_BlockType::Type>, int32> Block Chances;                        // 0x0B40 (size: 0x50)
    double Block Cooldown;                                                            // 0x0B90 (size: 0x8)
    TArray<class UAnimMontage*> Melee Block Anims;                                    // 0x0B98 (size: 0x10)
    TArray<class UAnimMontage*> Blocked Attack Montage;                               // 0x0BA8 (size: 0x10)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Melee Emitters;             // 0x0BB8 (size: 0x50)
    double Projectile Speed;                                                          // 0x0C08 (size: 0x8)
    bool Allow Melee Rotation;                                                        // 0x0C10 (size: 0x1)
    bool ProjectileUseWeaponDamage;                                                   // 0x0C11 (size: 0x1)
    TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Surface Sound (Trace);           // 0x0C18 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Melee Surface Sounds;            // 0x0C68 (size: 0x50)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> MeleeDecals;             // 0x0CB8 (size: 0x50)

    void SpawnTrail(FVector MuzzleLocation, FVector EndPointLocation, FVector TraceEndLocation, FRotator Rotation);
    void Update Melee Hit();
    void Add Current Ammo(int32 Add Amount);
    void Remove Total Ammo(int32 Remove Ammo, bool& Removed All, int32& Remaining Ammo);
    void Total Ammo Check(bool& Has Ammo, int32& Ammo);
    void Remove Current Ammo(int32 Remove Amount);
    void Current Ammo Check(bool& Has Ammo, int32& Ammo);
    void Melee Hit(class AActor* Hit Actor, class UPrimitiveComponent* Hit Component, FName Bone Name, FHitResult Hit);
    void OnNotifyEnd_4840180348DE7FAC42113FADB8E440A3(FName NotifyName);
    void OnNotifyBegin_4840180348DE7FAC42113FADB8E440A3(FName NotifyName);
    void OnInterrupted_4840180348DE7FAC42113FADB8E440A3(FName NotifyName);
    void OnBlendOut_4840180348DE7FAC42113FADB8E440A3(FName NotifyName);
    void OnCompleted_4840180348DE7FAC42113FADB8E440A3(FName NotifyName);
    void Server Melee Attack();
    void Server End Melee Attack();
    void Server Play Skeletal Mesh Montage(class UAnimMontage* Montage);
    void ReceiveBeginPlay();
    void Multicast Play Skeletal Mesh Montage(class UAnimMontage* Montage);
    void Server Play Shoot Montage();
    void Server Range Attack Muzzle Effect(FHitResult Hit);
    void Multicast Range Attack Effects(FHitResult Hit);
    void Multicast Melee Emitter(FHitResult Hit);
    void ExecuteUbergraph_BP_AIWeapon_Master(int32 EntryPoint);
}; // Size: 0xD08

#endif
