#ifndef UE4SS_SDK_SC_MilitaryBackpack_HPP
#define UE4SS_SDK_SC_MilitaryBackpack_HPP

class USC_MilitaryBackpack_C : public UBP_MainSpecialContainer_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Main;                                         // 0x0308 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Main2;                                        // 0x0310 (size: 0x8)

    void Event_Callback(bool NewValue);
    void PreInitSpecialContainer();
    void ExecuteUbergraph_SC_MilitaryBackpack(int32 EntryPoint);
}; // Size: 0x318

#endif
