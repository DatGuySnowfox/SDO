#ifndef UE4SS_SDK_BP_ProjectileRocket_HPP
#define UE4SS_SDK_BP_ProjectileRocket_HPP

class ABP_ProjectileRocket_C : public ABP_ProjectileMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class URadialForceComponent* RadialForce;                                         // 0x02C8 (size: 0x8)
    class USphereComponent* CameraShakeSphere;                                        // 0x02D0 (size: 0x8)
    double MinDamage;                                                                 // 0x02D8 (size: 0x8)
    double MaxDamage;                                                                 // 0x02E0 (size: 0x8)
    class UTechTreeComponent_C* Tech Tree;                                            // 0x02E8 (size: 0x8)

    void SkillCheck(double Damage, double& Multiplier);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void BndEvt__BP_ProjectileRocket_StaticMesh_K2Node_ComponentBoundEvent_0_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void ExecuteUbergraph_BP_ProjectileRocket(int32 EntryPoint);
}; // Size: 0x2F0

#endif
