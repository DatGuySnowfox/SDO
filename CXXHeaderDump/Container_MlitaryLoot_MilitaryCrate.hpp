#ifndef UE4SS_SDK_Container_MlitaryLoot_MilitaryCrate_HPP
#define UE4SS_SDK_Container_MlitaryLoot_MilitaryCrate_HPP

class AContainer_MlitaryLoot_MilitaryCrate_C : public AContainer_MilitaryLoot_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0440 (size: 0x8)

    void ReceiveBeginPlay();
    void Event1();
    void Event2();
    void ExecuteUbergraph_Container_MlitaryLoot_MilitaryCrate(int32 EntryPoint);
}; // Size: 0x448

#endif
