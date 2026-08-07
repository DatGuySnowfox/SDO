#ifndef UE4SS_SDK_Container_HiddenCache_HPP
#define UE4SS_SDK_Container_HiddenCache_HPP

class AContainer_HiddenCache_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_HiddenCache(int32 EntryPoint);
}; // Size: 0x448

#endif
