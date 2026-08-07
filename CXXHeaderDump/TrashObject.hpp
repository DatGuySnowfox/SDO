#ifndef UE4SS_SDK_TrashObject_HPP
#define UE4SS_SDK_TrashObject_HPP

class ATrashObject_C : public AStaticMeshActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class UWorldObjectComponent_C* WorldObjectComponent;                              // 0x02B0 (size: 0x8)
    bool IsDestroyed;                                                                 // 0x02B8 (size: 0x1)
    double Health;                                                                    // 0x02C0 (size: 0x8)

    void IsObjectDamageable?(bool& Damageable?);
    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void Damage_Shoved(bool Anim, double Force);
    void Damage_Object(double Damage, class AActor* Damage Causer, class AController* Event Instigator);
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void ActorLoaded();
    void BndEvt__TrashObject_WorldObjectComponent_K2Node_ComponentBoundEvent_0_ObjectDamaged__DelegateSignature(double Dmg);
    void BndEvt__TrashObject_WorldObjectComponent_K2Node_ComponentBoundEvent_1_ObjectDestroyed__DelegateSignature();
    void ExecuteUbergraph_TrashObject(int32 EntryPoint);
}; // Size: 0x2C8

#endif
