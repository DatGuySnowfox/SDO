#ifndef UE4SS_SDK_SC_Briefcase_HPP
#define UE4SS_SDK_SC_Briefcase_HPP

class USC_Briefcase_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x0308 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Briefcase(int32 EntryPoint);
}; // Size: 0x310

#endif
