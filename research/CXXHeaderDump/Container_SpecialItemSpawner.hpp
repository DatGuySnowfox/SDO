#ifndef UE4SS_SDK_Container_SpecialItemSpawner_HPP
#define UE4SS_SDK_Container_SpecialItemSpawner_HPP

class AContainer_SpecialItemSpawner_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)
    TArray<FName> Unique Items Set;                                                   // 0x0448 (size: 0x10)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_SpecialItemSpawner(int32 EntryPoint);
}; // Size: 0x458

#endif
