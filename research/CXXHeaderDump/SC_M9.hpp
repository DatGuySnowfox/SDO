#ifndef UE4SS_SDK_SC_M9_HPP
#define UE4SS_SDK_SC_M9_HPP

class USC_M9_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* ChamberContainer;                                          // 0x0308 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0310 (size: 0x8)
    class UJSIContainer_C* MuzzleContainer;                                           // 0x0318 (size: 0x8)
    class UJSIContainer_C* SightContainer;                                            // 0x0320 (size: 0x8)
    class UJSIContainer_C* TacticalContainer;                                         // 0x0328 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_M9(int32 EntryPoint);
}; // Size: 0x330

#endif
