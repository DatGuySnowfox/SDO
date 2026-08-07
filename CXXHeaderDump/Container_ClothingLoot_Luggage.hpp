#ifndef UE4SS_SDK_Container_ClothingLoot_Luggage_HPP
#define UE4SS_SDK_Container_ClothingLoot_Luggage_HPP

class AContainer_ClothingLoot_Luggage_C : public AContainer_ClothingLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ClothingLoot_Luggage(int32 EntryPoint);
}; // Size: 0x448

#endif
