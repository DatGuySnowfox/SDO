#ifndef UE4SS_SDK_Dialogue_WanderingTrader_HPP
#define UE4SS_SDK_Dialogue_WanderingTrader_HPP

class UDialogue_WanderingTrader_C : public UDialogue
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03B0 (size: 0x8)

    void ExecuteUbergraph_Dialogue_WanderingTrader(int32 EntryPoint);
}; // Size: 0x3B8

#endif
