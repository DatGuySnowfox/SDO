#ifndef UE4SS_SDK_BP_Landmine_HPP
#define UE4SS_SDK_BP_Landmine_HPP

class ABP_Landmine_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class URadialForceComponent* RadialForce;                                         // 0x02A8 (size: 0x8)
    class USphereComponent* ExplosionSphere;                                          // 0x02B0 (size: 0x8)
    class USphereComponent* CameraShakeSphere;                                        // 0x02B8 (size: 0x8)
    class UBoxComponent* Box;                                                         // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02D0 (size: 0x8)
    FTimerHandle DamageTimer;                                                         // 0x02D8 (size: 0x8)
    bool IgnoreSoldier?;                                                              // 0x02E0 (size: 0x1)

    void IsObjectDamageable?(bool& Damageable?);
    void SpawnCombatText(FVector Location, double Damage);
    void Damage_Shoved(bool Anim, double Force);
    void BndEvt__Buildable_BearTrap_Box_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void Event_Explode();
    void ReceiveBeginPlay();
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_Landmine(int32 EntryPoint);
}; // Size: 0x2E1

#endif
