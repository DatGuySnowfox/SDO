#ifndef UE4SS_SDK_Container_AmmoLoot_AmmoCrate_HPP
#define UE4SS_SDK_Container_AmmoLoot_AmmoCrate_HPP

class AContainer_AmmoLoot_AmmoCrate_C : public AContainer_AmmoLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_AmmoLoot_AmmoCrate(int32 EntryPoint);
}; // Size: 0x448

#endif
