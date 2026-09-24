#ifndef UE4SS_SDK_BP_AI_HPP
#define UE4SS_SDK_BP_AI_HPP

class ABP_AI_C : public ACharacter
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0650 (size: 0x8)
    bool Dead;                                                                        // 0x0658 (size: 0x1)
    bool BloodSplatterEnabled?;                                                       // 0x0659 (size: 0x1)

    void Event_BloodSplatter();
    void ExecuteUbergraph_BP_AI(int32 EntryPoint);
}; // Size: 0x65A

#endif
