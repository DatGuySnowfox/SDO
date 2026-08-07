#ifndef UE4SS_SDK_Container_ShopLoot_ShopShelves_HPP
#define UE4SS_SDK_Container_ShopLoot_ShopShelves_HPP

class AContainer_ShopLoot_ShopShelves_C : public AContainer_ShopLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ShopLoot_ShopShelves(int32 EntryPoint);
}; // Size: 0x448

#endif
