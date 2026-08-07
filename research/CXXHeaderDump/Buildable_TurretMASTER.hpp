#ifndef UE4SS_SDK_Buildable_TurretMASTER_HPP
#define UE4SS_SDK_Buildable_TurretMASTER_HPP

class ABuildable_TurretMASTER_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class UWidgetComponent* Widget;                                                   // 0x0438 (size: 0x8)
    class UBP_BuildableTurretAIComponent_C* BP_BuildableTurretAIComponent;            // 0x0440 (size: 0x8)
    class USphereComponent* Weapon Muzzle;                                            // 0x0448 (size: 0x8)
    class UArrowComponent* Forward Direction;                                         // 0x0450 (size: 0x8)
    class UArrowComponent* FOV Arrow Right;                                           // 0x0458 (size: 0x8)
    class UArrowComponent* FOV Arrow Up;                                              // 0x0460 (size: 0x8)
    class UArrowComponent* FOV Arrow Down;                                            // 0x0468 (size: 0x8)
    class UArrowComponent* FOV Arrow Left;                                            // 0x0470 (size: 0x8)
    class UStaticMeshComponent* Turret;                                               // 0x0478 (size: 0x8)

    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void UpdateHealth(int32 Health);
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
    void ExecuteUbergraph_Buildable_TurretMASTER(int32 EntryPoint);
}; // Size: 0x480

#endif
