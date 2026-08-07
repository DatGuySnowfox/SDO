#ifndef UE4SS_SDK_Container_RandomLoot_LetterBox_HPP
#define UE4SS_SDK_Container_RandomLoot_LetterBox_HPP

class AContainer_RandomLoot_LetterBox_C : public AContainer_RandomLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_RandomLoot_LetterBox(int32 EntryPoint);
}; // Size: 0x448

#endif
