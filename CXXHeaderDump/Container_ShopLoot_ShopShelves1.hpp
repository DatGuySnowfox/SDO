#ifndef UE4SS_SDK_Container_ShopLoot_ShopShelves1_HPP
#define UE4SS_SDK_Container_ShopLoot_ShopShelves1_HPP

class AContainer_ShopLoot_ShopShelves1_C : public AContainer_ShopLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ShopLoot_ShopShelves1(int32 EntryPoint);
}; // Size: 0x448

#endif
