#ifndef UE4SS_SDK_SC_BattleReadyGlock_HPP
#define UE4SS_SDK_SC_BattleReadyGlock_HPP

class USC_BattleReadyGlock_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* TacticalContainer;                                         // 0x0388 (size: 0x8)
    class UJSIContainer_C* SightContainer;                                            // 0x0390 (size: 0x8)
    class UJSIContainer_C* MuzzleContainer;                                           // 0x0398 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x03A0 (size: 0x8)
    class UJSIContainer_C* ChamberContainer;                                          // 0x03A8 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_BattleReadyGlock(int32 EntryPoint);
}; // Size: 0x3B0

#endif
