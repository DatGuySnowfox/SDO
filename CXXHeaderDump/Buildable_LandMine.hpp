#ifndef UE4SS_SDK_Buildable_LandMine_HPP
#define UE4SS_SDK_Buildable_LandMine_HPP

class ABuildable_LandMine_C : public ABuildable_MASTER_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0430 (size: 0x8)
    class USphereComponent* ExplosionSphere;                                          // 0x0438 (size: 0x8)
    class USphereComponent* CameraShakeSphere;                                        // 0x0440 (size: 0x8)
    class URadialForceComponent* RadialForce;                                         // 0x0448 (size: 0x8)
    class UBoxComponent* Box_0;                                                       // 0x0450 (size: 0x8)

    void IsObjectDamageable?(bool& Damageable?);
    void SpawnCombatText(FVector Location, double Damage);
    void Damage_Shoved(bool Anim, double Force);
    void BndEvt__Buildable_BearTrap_Box_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void Event_Explode();
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_Buildable_LandMine(int32 EntryPoint);
}; // Size: 0x458

#endif
