#ifndef UE4SS_SDK_LongdownValley_HPP
#define UE4SS_SDK_LongdownValley_HPP

class ALongdownValley_C : public ALevelScriptActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_LongdownValley(int32 EntryPoint);
}; // Size: 0x2A8

#endif
