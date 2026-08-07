#ifndef UE4SS_SDK_Container_HighTierMilitaryLoot_Lockers_HPP
#define UE4SS_SDK_Container_HighTierMilitaryLoot_Lockers_HPP

class AContainer_HighTierMilitaryLoot_Lockers_C : public AContainer_HighTierMilitaryLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_HighTierMilitaryLoot_Lockers(int32 EntryPoint);
}; // Size: 0x448

#endif
