#ifndef UE4SS_SDK_SC_Battery_HPP
#define UE4SS_SDK_SC_Battery_HPP

class USC_Battery_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UJSIContainer_C* BatteryContainer;                                          // 0x0388 (size: 0x8)

    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_Battery(int32 EntryPoint);
}; // Size: 0x390

#endif
