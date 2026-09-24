#ifndef UE4SS_SDK_SC_Pockets3x2_HPP
#define UE4SS_SDK_SC_Pockets3x2_HPP

class USC_Pockets3x2_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* JSIContainer7_1;                                           // 0x0388 (size: 0x8)
    class UJSIContainer_C* JSIContainer7;                                             // 0x0390 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Pockets3x2(int32 EntryPoint);
}; // Size: 0x398

#endif
