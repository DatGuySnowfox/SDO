#ifndef UE4SS_SDK_SC_SmallSurvivalBackpack_HPP
#define UE4SS_SDK_SC_SmallSurvivalBackpack_HPP

class USC_SmallSurvivalBackpack_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Main2;                                        // 0x0388 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Main;                                         // 0x0390 (size: 0x8)
    class UJSIContainer_C* JSIContainer_2;                                            // 0x0398 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x03A0 (size: 0x8)

    void Event_Callback(bool NewValue);
    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_SmallSurvivalBackpack(int32 EntryPoint);
}; // Size: 0x3A8

#endif
