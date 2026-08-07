#ifndef UE4SS_SDK_Difficulty_Vehicles_HPP
#define UE4SS_SDK_Difficulty_Vehicles_HPP

class UDifficulty_Vehicles_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButtonWidget_C* ButtonWidget;                                              // 0x02C8 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_NoReset;                                      // 0x02D0 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_YesReset;                                     // 0x02D8 (size: 0x8)
    class UOverlay* ResetVehicleOverlay;                                              // 0x02E0 (size: 0x8)
    class USettingRow_C* SettingRow;                                                  // 0x02E8 (size: 0x8)
    class USettingRow_C* SettingRow_1;                                                // 0x02F0 (size: 0x8)
    class USettingRow_C* SettingRow_8;                                                // 0x02F8 (size: 0x8)
    class USettingRow_C* SettingRow_9;                                                // 0x0300 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x0308 (size: 0x14)

    void BndEvt__Difficulty_Vehicles_ButtonWidget_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void BndEvt__Difficulty_Vehicles_ButtonWidget_YesReset_K2Node_ComponentBoundEvent_1_ButtonPressed__DelegateSignature();
    void BndEvt__Difficulty_Vehicles_ButtonWidget_NoReset_K2Node_ComponentBoundEvent_2_ButtonPressed__DelegateSignature();
    void ExecuteUbergraph_Difficulty_Vehicles(int32 EntryPoint);
}; // Size: 0x31C

#endif
