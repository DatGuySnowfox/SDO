#ifndef UE4SS_SDK_SC_SkirtPockets_HPP
#define UE4SS_SDK_SC_SkirtPockets_HPP

class USC_SkirtPockets_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* JSIContainer7;                                             // 0x0308 (size: 0x8)
    class UJSIContainer_C* JSIContainer7_1;                                           // 0x0310 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_SkirtPockets(int32 EntryPoint);
}; // Size: 0x318

#endif
