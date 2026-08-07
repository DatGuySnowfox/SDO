#ifndef UE4SS_SDK_SC_PumpActionShotgun_HPP
#define UE4SS_SDK_SC_PumpActionShotgun_HPP

class USC_PumpActionShotgun_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0308 (size: 0x8)
    class UJSIContainer_C* MuzzleContainer;                                           // 0x0310 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_PumpActionShotgun(int32 EntryPoint);
}; // Size: 0x318

#endif
