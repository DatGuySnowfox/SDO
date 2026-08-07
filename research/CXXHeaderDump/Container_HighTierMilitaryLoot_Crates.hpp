#ifndef UE4SS_SDK_Container_HighTierMilitaryLoot_Crates_HPP
#define UE4SS_SDK_Container_HighTierMilitaryLoot_Crates_HPP

class AContainer_HighTierMilitaryLoot_Crates_C : public AContainer_HighTierMilitaryLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_HighTierMilitaryLoot_Crates(int32 EntryPoint);
}; // Size: 0x448

#endif
