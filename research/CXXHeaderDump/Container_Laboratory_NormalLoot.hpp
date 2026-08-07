#ifndef UE4SS_SDK_Container_Laboratory_NormalLoot_HPP
#define UE4SS_SDK_Container_Laboratory_NormalLoot_HPP

class AContainer_Laboratory_NormalLoot_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void Event1();
    void Event2();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Container_Laboratory_NormalLoot(int32 EntryPoint);
}; // Size: 0x448

#endif
