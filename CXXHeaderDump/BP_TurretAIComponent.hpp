#ifndef UE4SS_SDK_BP_TurretAIComponent_HPP
#define UE4SS_SDK_BP_TurretAIComponent_HPP

class UBP_TurretAIComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    bool Reloading;                                                                   // 0x00A8 (size: 0x1)
    bool Attacking;                                                                   // 0x00A9 (size: 0x1)
    class ABP_MasterTurret_C* AI Turret;                                              // 0x00B0 (size: 0x8)
    class AActor* Attack Target;                                                      // 0x00B8 (size: 0x8)
    TArray<class AActor*> Attack Targets;                                             // 0x00C0 (size: 0x10)
    int32 Current Ammo;                                                               // 0x00D0 (size: 0x4)
    int32 Current Health;                                                             // 0x00D4 (size: 0x4)
    bool Use Ammo;                                                                    // 0x00D8 (size: 0x1)
    bool Can Reload;                                                                  // 0x00D9 (size: 0x1)
    int32 Starting Ammo;                                                              // 0x00DC (size: 0x4)
    int32 Magazine Size;                                                              // 0x00E0 (size: 0x4)
    double Reload Speed;                                                              // 0x00E8 (size: 0x8)
    bool Dead;                                                                        // 0x00F0 (size: 0x1)
    bool Can Attack;                                                                  // 0x00F1 (size: 0x1)
    TArray<FName> Reaction Tags;                                                      // 0x00F8 (size: 0x10)
    TMap<TEnumAsByte<E_HitboxTypeAI::Type>, double> Damage;                           // 0x0108 (size: 0x50)
    double Detection Range;                                                           // 0x0158 (size: 0x8)
    bool Debug Mode;                                                                  // 0x0160 (size: 0x1)
    double Turret Rotation Speed;                                                     // 0x0168 (size: 0x8)
    double Weapon Range;                                                              // 0x0170 (size: 0x8)
    double Spread;                                                                    // 0x0178 (size: 0x8)
    bool Allow Up/Down Rotation;                                                      // 0x0180 (size: 0x1)
    class UParticleSystem* Range Attack Particle Effect;                              // 0x0188 (size: 0x8)
    TSubclassOf<class AActor> Projectile;                                             // 0x0190 (size: 0x8)
    int32 Starting Health;                                                            // 0x0198 (size: 0x4)
    int32 Max Health;                                                                 // 0x019C (size: 0x4)
    class UParticleSystemComponent* Muzzle Effect;                                    // 0x01A0 (size: 0x8)
    FHitResult Effect Hit;                                                            // 0x01A8 (size: 0xE8)
    TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> Surface Emitters (Trace);   // 0x0290 (size: 0x50)
    FVector Surface Decal Size (Trace);                                               // 0x02E0 (size: 0x18)
    double Decal Life Span (Trace);                                                   // 0x02F8 (size: 0x8)
    double Decal Fade Distance (Trace);                                               // 0x0300 (size: 0x8)
    TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Surface Decal (Trace);   // 0x0308 (size: 0x50)
    bool Alert AI;                                                                    // 0x0358 (size: 0x1)
    double Alert Radius;                                                              // 0x0360 (size: 0x8)
    TArray<class AActor*> Alert Actors;                                               // 0x0368 (size: 0x10)
    TArray<FName> Alert Tags;                                                         // 0x0378 (size: 0x10)
    TArray<class AActor*> Reacted Actors;                                             // 0x0388 (size: 0x10)
    bool Track Target;                                                                // 0x0398 (size: 0x1)
    TArray<class AActor*> Activate Actors;                                            // 0x03A0 (size: 0x10)
    class USoundBase* FireSound;                                                      // 0x03B0 (size: 0x8)
    double AttackSpeed;                                                               // 0x03B8 (size: 0x8)
    TEnumAsByte<ETraceTypeQuery> AttackTraceChannel;                                  // 0x03C0 (size: 0x1)

    void Activate Target Actors(bool Activate);
    void Check Tags(TArray<FName>& Tags 1, TArray<FName>& Tags 2, bool& Return value, FName& Found Tag);
    void Deduct Health(double Damage);
    void Deduct Ammo();
    void Turret Fire();
    void Debug Text(FString In String, FLinearColor Text Color);
    void Perception Check();
    void Reload();
    void Multicast Projectile(FVector Velocity);
    void Server Turret Damaged(double Damage);
    void Multicast Muzzle Effect(FHitResult Effect Hit);
    void ReceiveTick(float DeltaSeconds);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_TurretAIComponent(int32 EntryPoint);
}; // Size: 0x3C1

#endif
