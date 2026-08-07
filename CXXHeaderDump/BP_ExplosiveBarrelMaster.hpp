#ifndef UE4SS_SDK_BP_ExplosiveBarrelMaster_HPP
#define UE4SS_SDK_BP_ExplosiveBarrelMaster_HPP

class ABP_ExplosiveBarrelMaster_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* CameraShakeSphere;                                        // 0x02A8 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02B0 (size: 0x8)
    class URadialForceComponent* RadialForce;                                         // 0x02B8 (size: 0x8)
    class USphereComponent* ExplosionSphere;                                          // 0x02C0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02C8 (size: 0x8)
    double Health;                                                                    // 0x02D0 (size: 0x8)

    void IsObjectDamageable?(bool& Damageable?);
    void UserConstructionScript();
    void Damage_Shoved(bool Anim, double Force);
    void ReceiveBeginPlay();
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void Event_Explode(class AController* EventInstigator);
    void BndEvt__BP_CookingStation_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_ExplosiveBarrelMaster(int32 EntryPoint);
}; // Size: 0x2D8

#endif
