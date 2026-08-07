#ifndef UE4SS_SDK_Container_IndustrialLoot_SupplyPile_HPP
#define UE4SS_SDK_Container_IndustrialLoot_SupplyPile_HPP

class AContainer_IndustrialLoot_SupplyPile_C : public AContainer_IndustrialLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_IndustrialLoot_SupplyPile(int32 EntryPoint);
}; // Size: 0x448

#endif
