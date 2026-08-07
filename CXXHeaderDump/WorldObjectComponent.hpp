#ifndef UE4SS_SDK_WorldObjectComponent_HPP
#define UE4SS_SDK_WorldObjectComponent_HPP

class UWorldObjectComponent_C : public UActorComponent
{
    FWorldObjectComponent_CObjectDestroyed ObjectDestroyed;                           // 0x00A0 (size: 0x10)
    void ObjectDestroyed();
    FWorldObjectComponent_CObjectDamaged ObjectDamaged;                               // 0x00B0 (size: 0x10)
    void ObjectDamaged(double Dmg);
    FWorldObjectComponent_CObjectInteracted ObjectInteracted;                         // 0x00C0 (size: 0x10)
    void ObjectInteracted();

    void ObjectInteracted__DelegateSignature();
    void ObjectDamaged__DelegateSignature(double Dmg);
    void ObjectDestroyed__DelegateSignature();
}; // Size: 0xD0

#endif
