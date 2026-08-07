#ifndef UE4SS_SDK_Container_GunCaseLoot_WeaponCase_HPP
#define UE4SS_SDK_Container_GunCaseLoot_WeaponCase_HPP

class AContainer_GunCaseLoot_WeaponCase_C : public AContainer_GunCaseLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event2();
    void Event1();
    void ExecuteUbergraph_Container_GunCaseLoot_WeaponCase(int32 EntryPoint);
}; // Size: 0x448

#endif
