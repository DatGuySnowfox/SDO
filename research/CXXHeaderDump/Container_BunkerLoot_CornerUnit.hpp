#ifndef UE4SS_SDK_Container_BunkerLoot_CornerUnit_HPP
#define UE4SS_SDK_Container_BunkerLoot_CornerUnit_HPP

class AContainer_BunkerLoot_CornerUnit_C : public AContainer_BunkerLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_BunkerLoot_CornerUnit(int32 EntryPoint);
}; // Size: 0x448

#endif
