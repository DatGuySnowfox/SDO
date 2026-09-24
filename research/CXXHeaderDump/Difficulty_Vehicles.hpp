#ifndef UE4SS_SDK_Difficulty_Vehicles_HPP
#define UE4SS_SDK_Difficulty_Vehicles_HPP

class UDifficulty_Vehicles_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class USettingRow_C* SettingRow_9;                                                // 0x0348 (size: 0x8)
    class USettingRow_C* SettingRow_8;                                                // 0x0350 (size: 0x8)
    class USettingRow_C* SettingRow_1;                                                // 0x0358 (size: 0x8)
    class USettingRow_C* SettingRow;                                                  // 0x0360 (size: 0x8)
    class UOverlay* ResetVehicleOverlay;                                              // 0x0368 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_YesReset;                                     // 0x0370 (size: 0x8)
    class UButtonWidget_C* ButtonWidget_NoReset;                                      // 0x0378 (size: 0x8)
    class UButtonWidget_C* ButtonWidget;                                              // 0x0380 (size: 0x8)
    FSlateColor HoveredTextColor;                                                     // 0x0388 (size: 0x14)

    void BndEvt__Difficulty_Vehicles_ButtonWidget_K2Node_ComponentBoundEvent_0_ButtonPressed__DelegateSignature();
    void BndEvt__Difficulty_Vehicles_ButtonWidget_YesReset_K2Node_ComponentBoundEvent_1_ButtonPressed__DelegateSignature();
    void BndEvt__Difficulty_Vehicles_ButtonWidget_NoReset_K2Node_ComponentBoundEvent_2_ButtonPressed__DelegateSignature();
    void ExecuteUbergraph_Difficulty_Vehicles(int32 EntryPoint);
}; // Size: 0x39C

#endif
