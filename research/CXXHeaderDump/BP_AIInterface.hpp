#ifndef UE4SS_SDK_BP_AIInterface_HPP
#define UE4SS_SDK_BP_AIInterface_HPP

class IBP_AIInterface_C : public IInterface
{

    void AI Block(TEnumAsByte<E_BlockType::Type> Block Type, class AActor* Block Attacker);
    void AI Camera Activate(bool Activate);
    void AI Animation Switch(TEnumAsByte<E_AIBehaviour::Type> Behaviour);
    void AI End Alert();
    void AI Alert(class AActor* Alert Actor);
    void Turret End Reload();
    void Turret Start Reload();
    void Turret Idle Stop();
    void Turret Idle Start();
    void Turret Destroyed Effect();
    void Alert Actor Defenders(class AActor* Attacked, class AActor* Attacker);
    void Actor Attack Target(class AActor*& Attack Target);
    void Actor Aim Focus(class AActor* Instigator);
    void AI Dead();
    void AI Can Interact?(bool& Can Interact);
    void AI Is Dead?(bool& Dead);
}; // Size: 0x28

#endif
