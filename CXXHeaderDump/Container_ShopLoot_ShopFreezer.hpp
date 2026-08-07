#ifndef UE4SS_SDK_Container_ShopLoot_ShopFreezer_HPP
#define UE4SS_SDK_Container_ShopLoot_ShopFreezer_HPP

class AContainer_ShopLoot_ShopFreezer_C : public AContainer_ShopLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ShopLoot_ShopFreezer(int32 EntryPoint);
}; // Size: 0x448

#endif
