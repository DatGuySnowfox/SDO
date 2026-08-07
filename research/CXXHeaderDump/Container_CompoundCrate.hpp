#ifndef UE4SS_SDK_Container_CompoundCrate_HPP
#define UE4SS_SDK_Container_CompoundCrate_HPP

class AContainer_CompoundCrate_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_CompoundCrate(int32 EntryPoint);
}; // Size: 0x448

#endif
