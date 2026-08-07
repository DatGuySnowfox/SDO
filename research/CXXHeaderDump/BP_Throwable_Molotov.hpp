#ifndef UE4SS_SDK_BP_Throwable_Molotov_HPP
#define UE4SS_SDK_BP_Throwable_Molotov_HPP

class ABP_Throwable_Molotov_C : public ABP_MasterThrowable_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UParticleSystemComponent* ParticleSystem;                                   // 0x02D8 (size: 0x8)
    double MinDamage;                                                                 // 0x02E0 (size: 0x8)
    double MaxDamage;                                                                 // 0x02E8 (size: 0x8)
    class UAudioComponent* FireSound;                                                 // 0x02F0 (size: 0x8)

    void BndEvt__BP_Throwable_Molotov_Capsule_K2Node_ComponentBoundEvent_0_ComponentHitSignature__DelegateSignature(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void ReceiveBeginPlay();
    void Fire_Damage();
    void ExecuteUbergraph_BP_Throwable_Molotov(int32 EntryPoint);
}; // Size: 0x2F8

#endif
