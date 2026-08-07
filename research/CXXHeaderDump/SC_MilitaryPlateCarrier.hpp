#ifndef UE4SS_SDK_SC_MilitaryPlateCarrier_HPP
#define UE4SS_SDK_SC_MilitaryPlateCarrier_HPP

class USC_MilitaryPlateCarrier_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* JSIContainer2;                                             // 0x0308 (size: 0x8)
    class UJSIContainer_C* JSIContainer3;                                             // 0x0310 (size: 0x8)
    class UJSIContainer_C* JSIContainer7;                                             // 0x0318 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_MilitaryPlateCarrier(int32 EntryPoint);
}; // Size: 0x320

#endif
