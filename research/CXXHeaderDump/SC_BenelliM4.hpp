#ifndef UE4SS_SDK_SC_BenelliM4_HPP
#define UE4SS_SDK_SC_BenelliM4_HPP

class USC_BenelliM4_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0308 (size: 0x8)
    class UJSIContainer_C* MuzzleContainer;                                           // 0x0310 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_BenelliM4(int32 EntryPoint);
}; // Size: 0x318

#endif
