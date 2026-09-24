#ifndef UE4SS_SDK_BP_ProjectileRocket_HPP
#define UE4SS_SDK_BP_ProjectileRocket_HPP

class ABP_ProjectileRocket_C : public ABP_ProjectileMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    double DmgToDo;                                                                   // 0x02D8 (size: 0x8)

    void CalculateDmg(double& Dmg);
    void SkillCheck(double Damage, double& Multiplier);
    void UserConstructionScript();
    void BndEvt__BP_ProjectileRocket_StaticMesh_K2Node_ComponentBoundEvent_0_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void ExecuteUbergraph_BP_ProjectileRocket(int32 EntryPoint);
}; // Size: 0x2E0

#endif
