#ifndef UE4SS_SDK_BP_ZombieBossRock_HPP
#define UE4SS_SDK_BP_ZombieBossRock_HPP

class ABP_ZombieBossRock_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UProjectileMovementComponent* ProjectileMovement;                           // 0x02A0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02A8 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02B0 (size: 0x8)

    void Launch(FVector Origin, FVector Target);
    void ExecuteUbergraph_BP_ZombieBossRock(int32 EntryPoint);
}; // Size: 0x2B8

#endif
