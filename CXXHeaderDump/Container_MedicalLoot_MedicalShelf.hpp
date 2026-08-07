#ifndef UE4SS_SDK_Container_MedicalLoot_MedicalShelf_HPP
#define UE4SS_SDK_Container_MedicalLoot_MedicalShelf_HPP

class AContainer_MedicalLoot_MedicalShelf_C : public AContainer_MedicalLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_MedicalLoot_MedicalShelf(int32 EntryPoint);
}; // Size: 0x448

#endif
