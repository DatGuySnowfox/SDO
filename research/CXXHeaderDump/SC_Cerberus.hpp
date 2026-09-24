#ifndef UE4SS_SDK_SC_Cerberus_HPP
#define UE4SS_SDK_SC_Cerberus_HPP

class USC_Cerberus_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* TacticalContainer;                                         // 0x0388 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0390 (size: 0x8)
    class UJSIContainer_C* ChamberContainer;                                          // 0x0398 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Cerberus(int32 EntryPoint);
}; // Size: 0x3A0

#endif
