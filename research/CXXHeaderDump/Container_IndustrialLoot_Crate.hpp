#ifndef UE4SS_SDK_Container_IndustrialLoot_Crate_HPP
#define UE4SS_SDK_Container_IndustrialLoot_Crate_HPP

class AContainer_IndustrialLoot_Crate_C : public AContainer_IndustrialLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_IndustrialLoot_Crate(int32 EntryPoint);
}; // Size: 0x448

#endif
