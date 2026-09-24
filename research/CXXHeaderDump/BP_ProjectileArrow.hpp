#ifndef UE4SS_SDK_BP_ProjectileArrow_HPP
#define UE4SS_SDK_BP_ProjectileArrow_HPP

class ABP_ProjectileArrow_C : public ABP_ProjectileMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void BndEvt__BP_ProjectileArrow_StaticMesh_K2Node_ComponentBoundEvent_2_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void ExecuteUbergraph_BP_ProjectileArrow(int32 EntryPoint);
}; // Size: 0x2D8

#endif
