#ifndef UE4SS_SDK_BP_FlareRoundProjectile_HPP
#define UE4SS_SDK_BP_FlareRoundProjectile_HPP

class ABP_FlareRoundProjectile_C : public ABP_ProjectileMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UPointLightComponent* PointLight;                                           // 0x02D8 (size: 0x8)

    void ReceiveBeginPlay();
    void BndEvt__BP_ProjectileRocket_StaticMesh_K2Node_ComponentBoundEvent_0_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void ExecuteUbergraph_BP_FlareRoundProjectile(int32 EntryPoint);
}; // Size: 0x2E0

#endif
