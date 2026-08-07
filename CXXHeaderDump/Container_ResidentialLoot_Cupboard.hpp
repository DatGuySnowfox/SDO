#ifndef UE4SS_SDK_Container_ResidentialLoot_Cupboard_HPP
#define UE4SS_SDK_Container_ResidentialLoot_Cupboard_HPP

class AContainer_ResidentialLoot_Cupboard_C : public AContainer_ResidentialLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_ResidentialLoot_Cupboard(int32 EntryPoint);
}; // Size: 0x448

#endif
