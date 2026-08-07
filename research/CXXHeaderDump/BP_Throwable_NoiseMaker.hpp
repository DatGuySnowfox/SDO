#ifndef UE4SS_SDK_BP_Throwable_NoiseMaker_HPP
#define UE4SS_SDK_BP_Throwable_NoiseMaker_HPP

class ABP_Throwable_NoiseMaker_C : public ABP_MasterThrowable_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)

    void ReceiveBeginPlay();
    void NoiseMaker();
    void ExecuteUbergraph_BP_Throwable_NoiseMaker(int32 EntryPoint);
}; // Size: 0x2D8

#endif
