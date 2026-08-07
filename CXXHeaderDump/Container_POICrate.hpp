#ifndef UE4SS_SDK_Container_POICrate_HPP
#define UE4SS_SDK_Container_POICrate_HPP

class AContainer_POICrate_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_POICrate(int32 EntryPoint);
}; // Size: 0x448

#endif
