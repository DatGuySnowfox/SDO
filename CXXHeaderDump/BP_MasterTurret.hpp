#ifndef UE4SS_SDK_BP_MasterTurret_HPP
#define UE4SS_SDK_BP_MasterTurret_HPP

class ABP_MasterTurret_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UArrowComponent* FOV Arrow Down;                                            // 0x02A0 (size: 0x8)
    class UArrowComponent* FOV Arrow Up;                                              // 0x02A8 (size: 0x8)
    class UArrowComponent* FOV Arrow Right;                                           // 0x02B0 (size: 0x8)
    class UArrowComponent* FOV Arrow Left;                                            // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* Turret Base;                                          // 0x02C0 (size: 0x8)
    class USceneComponent* Scene;                                                     // 0x02C8 (size: 0x8)
    class USphereComponent* Weapon Muzzle;                                            // 0x02D0 (size: 0x8)
    class UBP_TurretAIComponent_C* BP_TurretAIComponent;                              // 0x02D8 (size: 0x8)
    class UArrowComponent* Forward Direction;                                         // 0x02E0 (size: 0x8)
    class UStaticMeshComponent* Turret;                                               // 0x02E8 (size: 0x8)
    double FOV Down;                                                                  // 0x02F0 (size: 0x8)
    double FOV Up;                                                                    // 0x02F8 (size: 0x8)
    double FOV Right;                                                                 // 0x0300 (size: 0x8)
    double FOV Left;                                                                  // 0x0308 (size: 0x8)
    bool Debug Mode;                                                                  // 0x0310 (size: 0x1)

    void IsObjectDamageable?(bool& Damageable?);
    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void UserConstructionScript();
    void Damage_Shoved(bool Anim, double Force);
    void AI Dead();
    void Actor Aim Focus(class AActor* Instigator);
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Turret Destroyed Effect();
    void Turret Idle Start();
    void Turret Idle Stop();
    void Turret Start Reload();
    void Turret End Reload();
    void AI Alert(class AActor* Alert Actor);
    void AI End Alert();
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI Camera Activate(bool Activate);
    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void ExecuteUbergraph_BP_MasterTurret(int32 EntryPoint);
}; // Size: 0x311

#endif
