#ifndef UE4SS_SDK_BP_MasterCamera_HPP
#define UE4SS_SDK_BP_MasterCamera_HPP

class ABP_MasterCamera_C : public ABP_MasterTurret_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0318 (size: 0x8)
    float Rotate_Camera_Rotate_Alpha_1FCEDB2D4D69EF795AB31D9899FEACD6;                // 0x0320 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> Rotate_Camera__Direction_1FCEDB2D4D69EF795AB31D9899FEACD6; // 0x0324 (size: 0x1)
    class UTimelineComponent* Rotate Camera;                                          // 0x0328 (size: 0x8)

    void AI Is Dead?(bool& Dead);
    void AI Can Interact?(bool& Can Interact);
    void Actor Attack Target(class AActor*& Attack Target);
    void UserConstructionScript();
    void Rotate Camera__FinishedFunc();
    void Rotate Camera__UpdateFunc();
    void ReceiveBeginPlay();
    void Multicast Turret Idle Stop();
    void Multicast Turret Idle Start();
    void Turret Idle Stop();
    void Turret Idle Start();
    void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);
    void Turret Destroyed Effect();
    void ExecuteUbergraph_BP_MasterCamera(int32 EntryPoint);
}; // Size: 0x330

#endif
