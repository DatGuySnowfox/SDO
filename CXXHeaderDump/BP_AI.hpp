#ifndef UE4SS_SDK_BP_AI_HPP
#define UE4SS_SDK_BP_AI_HPP

class ABP_AI_C : public ACharacter
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0680 (size: 0x8)
    bool Dead;                                                                        // 0x0688 (size: 0x1)
    bool BloodSplatterEnabled?;                                                       // 0x0689 (size: 0x1)

    void Event_BloodSplatter();
    void ExecuteUbergraph_BP_AI(int32 EntryPoint);
}; // Size: 0x68A

#endif
