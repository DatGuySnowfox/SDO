#ifndef UE4SS_SDK_BP_Throwable_PipeBomb_HPP
#define UE4SS_SDK_BP_Throwable_PipeBomb_HPP

class ABP_Throwable_PipeBomb_C : public ABP_MasterThrowable_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    double MinDamage;                                                                 // 0x02D8 (size: 0x8)
    double MaxDamage;                                                                 // 0x02E0 (size: 0x8)

    void ReceiveBeginPlay();
    void PipeBomb();
    void ExecuteUbergraph_BP_Throwable_PipeBomb(int32 EntryPoint);
}; // Size: 0x2E8

#endif
