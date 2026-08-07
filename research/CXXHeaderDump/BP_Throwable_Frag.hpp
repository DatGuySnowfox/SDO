#ifndef UE4SS_SDK_BP_Throwable_Frag_HPP
#define UE4SS_SDK_BP_Throwable_Frag_HPP

class ABP_Throwable_Frag_C : public ABP_MasterThrowable_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    double MinDamage;                                                                 // 0x02D8 (size: 0x8)
    double MaxDamage;                                                                 // 0x02E0 (size: 0x8)

    void ReceiveBeginPlay();
    void Frag();
    void ExecuteUbergraph_BP_Throwable_Frag(int32 EntryPoint);
}; // Size: 0x2E8

#endif
