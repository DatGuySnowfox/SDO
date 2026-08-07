#ifndef UE4SS_SDK_Container_ClothingLoot_ResortWardrobe_HPP
#define UE4SS_SDK_Container_ClothingLoot_ResortWardrobe_HPP

class AContainer_ClothingLoot_ResortWardrobe_C : public AContainer_ClothingLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ClothingLoot_ResortWardrobe(int32 EntryPoint);
}; // Size: 0x448

#endif
