#ifndef UE4SS_SDK_SC_AliceBackpack_HPP
#define UE4SS_SDK_SC_AliceBackpack_HPP

class USC_AliceBackpack_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* JSIContainer_3;                                            // 0x0388 (size: 0x8)
    class UJSIContainer_C* JSIContainer_2;                                            // 0x0390 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x0398 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x03A0 (size: 0x8)

    void Event_Callback(bool NewValue);
    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_AliceBackpack(int32 EntryPoint);
}; // Size: 0x3A8

#endif
