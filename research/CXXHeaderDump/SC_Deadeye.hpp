#ifndef UE4SS_SDK_SC_Deadeye_HPP
#define UE4SS_SDK_SC_Deadeye_HPP

class USC_Deadeye_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* SightContainer;                                            // 0x0388 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0390 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Deadeye(int32 EntryPoint);
}; // Size: 0x398

#endif
