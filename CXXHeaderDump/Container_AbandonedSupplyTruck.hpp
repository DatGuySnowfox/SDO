#ifndef UE4SS_SDK_Container_AbandonedSupplyTruck_HPP
#define UE4SS_SDK_Container_AbandonedSupplyTruck_HPP

class AContainer_AbandonedSupplyTruck_C : public ABP_LootContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_AbandonedSupplyTruck(int32 EntryPoint);
}; // Size: 0x448

#endif
