#ifndef UE4SS_SDK_Container_HighTierMilitaryLoot_FirearmsLocker_HPP
#define UE4SS_SDK_Container_HighTierMilitaryLoot_FirearmsLocker_HPP

class AContainer_HighTierMilitaryLoot_FirearmsLocker_C : public AContainer_HighTierMilitaryLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_HighTierMilitaryLoot_FirearmsLocker(int32 EntryPoint);
}; // Size: 0x448

#endif
