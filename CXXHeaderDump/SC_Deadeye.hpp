#ifndef UE4SS_SDK_SC_Deadeye_HPP
#define UE4SS_SDK_SC_Deadeye_HPP

class USC_Deadeye_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* MagContainer;                                              // 0x0308 (size: 0x8)
    class UJSIContainer_C* SightContainer;                                            // 0x0310 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Deadeye(int32 EntryPoint);
}; // Size: 0x318

#endif
