#ifndef UE4SS_SDK_SD_AIComponent_HPP
#define UE4SS_SDK_SD_AIComponent_HPP

class USD_AIComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    bool CanSystemDespawnAI?;                                                         // 0x00C0 (size: 0x1)
    TEnumAsByte<Enum_AIBehviour::Type> Behaviour;                                     // 0x00C1 (size: 0x1)
    double MeleeAttackDistance;                                                       // 0x00C8 (size: 0x8)
    class ABP_AIWaypoint_C* Waypoint;                                                 // 0x00D0 (size: 0x8)
    class UBlendSpace1D* MovementBlendspace;                                          // 0x00D8 (size: 0x8)

    void SetBB_ForceAttack(class AActor* Attacker);
    void CheckBehaviour();
    void SetBB_LockOnTarget(FVector VectorValue);
    void SetBB_Behaviour(TEnumAsByte<Enum_AIBehviour::Type> EnumValue);
    void SetBB_Location(FVector VectorValue);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_SD_AIComponent(int32 EntryPoint);
}; // Size: 0xE0

#endif
