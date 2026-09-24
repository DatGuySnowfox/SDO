#ifndef UE4SS_SDK_Container_PoliceLoot_PoliceCar_HPP
#define UE4SS_SDK_Container_PoliceLoot_PoliceCar_HPP

class AContainer_PoliceLoot_PoliceCar_C : public AContainer_MilitaryLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_PoliceLoot_PoliceCar(int32 EntryPoint);
}; // Size: 0x448

#endif
