#ifndef UE4SS_SDK_Container_ResidentialFoodLoot_Oven_HPP
#define UE4SS_SDK_Container_ResidentialFoodLoot_Oven_HPP

class AContainer_ResidentialFoodLoot_Oven_C : public AContainer_ResidentialFoodLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ResidentialFoodLoot_Oven(int32 EntryPoint);
}; // Size: 0x448

#endif
