#ifndef UE4SS_SDK_BP_Throwable_Frag_HPP
#define UE4SS_SDK_BP_Throwable_Frag_HPP

class ABP_Throwable_Frag_C : public ABP_MasterThrowable_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E8 (size: 0x8)
    double MinDamage;                                                                 // 0x02F0 (size: 0x8)
    double MaxDamage;                                                                 // 0x02F8 (size: 0x8)
    double DmgToDo;                                                                   // 0x0300 (size: 0x8)

    void CalculateDmg(double& Dmg);
    void ReceiveBeginPlay();
    void Frag();
    void ExecuteUbergraph_BP_Throwable_Frag(int32 EntryPoint);
}; // Size: 0x308

#endif
