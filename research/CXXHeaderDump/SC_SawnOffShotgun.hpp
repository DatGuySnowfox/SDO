#ifndef UE4SS_SDK_SC_SawnOffShotgun_HPP
#define UE4SS_SDK_SC_SawnOffShotgun_HPP

class USC_SawnOffShotgun_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0388 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_SawnOffShotgun(int32 EntryPoint);
}; // Size: 0x390

#endif
