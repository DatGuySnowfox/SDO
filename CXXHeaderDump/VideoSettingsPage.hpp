#ifndef UE4SS_SDK_VideoSettingsPage_HPP
#define UE4SS_SDK_VideoSettingsPage_HPP

class UVideoSettingsPage_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* ApplyButton;                                                       // 0x02C8 (size: 0x8)
    class UCheckBox* AutoApplyCheckbox;                                               // 0x02D0 (size: 0x8)
    class UButton* AutoDetectButton;                                                  // 0x02D8 (size: 0x8)
    class UCheckBox* AutoSaveCheckbox;                                                // 0x02E0 (size: 0x8)
    class UButton* CancelButton;                                                      // 0x02E8 (size: 0x8)
    class UButton* SaveButton;                                                        // 0x02F0 (size: 0x8)
    class USettingRow_C* SettingRow;                                                  // 0x02F8 (size: 0x8)
    class USettingRow_C* SettingRow_1;                                                // 0x0300 (size: 0x8)
    class USettingRow_C* SettingRow_2;                                                // 0x0308 (size: 0x8)
    class USettingRow_C* SettingRow_3;                                                // 0x0310 (size: 0x8)
    class USettingRow_C* SettingRow_4;                                                // 0x0318 (size: 0x8)
    class USettingRow_C* SettingRow_5;                                                // 0x0320 (size: 0x8)
    class USettingRow_C* SettingRow_6;                                                // 0x0328 (size: 0x8)
    class USettingRow_C* SettingRow_7;                                                // 0x0330 (size: 0x8)
    class USettingRow_C* SettingRow_8;                                                // 0x0338 (size: 0x8)
    class USettingRow_C* SettingRow_9;                                                // 0x0340 (size: 0x8)
    class USettingRow_C* SettingRow_10;                                               // 0x0348 (size: 0x8)
    class USettingRow_C* SettingRow_11;                                               // 0x0350 (size: 0x8)
    class USettingRow_C* SettingRow_12;                                               // 0x0358 (size: 0x8)
    class USettingRow_C* SettingRow_13;                                               // 0x0360 (size: 0x8)
    class USettingRow_C* SettingRow_14;                                               // 0x0368 (size: 0x8)
    class USettingRow_C* SettingRow_17;                                               // 0x0370 (size: 0x8)
    class USettingRow_C* SettingRow_18;                                               // 0x0378 (size: 0x8)
    class USettingRow_C* SettingRow_88;                                               // 0x0380 (size: 0x8)
    class USettingRow_C* SettingRow_149;                                              // 0x0388 (size: 0x8)
    class USettingRow_C* SettingRow_194;                                              // 0x0390 (size: 0x8)
    class USettingRow_C* SettingRow_234;                                              // 0x0398 (size: 0x8)
    class USettingRow_C* SettingRow_302;                                              // 0x03A0 (size: 0x8)
    class USettingRow_C* SettingRow_366;                                              // 0x03A8 (size: 0x8)
    class USettingRow_C* SettingRow_489;                                              // 0x03B0 (size: 0x8)
    class USettingRow_C* SettingRow_715;                                              // 0x03B8 (size: 0x8)
    class USettingRow_C* SettingRow_791;                                              // 0x03C0 (size: 0x8)
    class USettingRow_C* SettingRow_880;                                              // 0x03C8 (size: 0x8)
    class USettingRow_C* SettingRow_1080;                                             // 0x03D0 (size: 0x8)
    class USettingRow_C* SettingRow_1137;                                             // 0x03D8 (size: 0x8)
    class USettingRow_C* SettingRow_1197;                                             // 0x03E0 (size: 0x8)
    class USettingRow_C* SettingRow_AMDFG;                                            // 0x03E8 (size: 0x8)
    class USettingRow_C* SettingRow_ResolutionScale;                                  // 0x03F0 (size: 0x8)
    class USettingRow_C* SettingRow_UpscalingQuality;                                 // 0x03F8 (size: 0x8)
    bool Autosave;                                                                    // 0x0400 (size: 0x1)
    bool AutoApply;                                                                   // 0x0401 (size: 0x1)

    bool Get_AutoApplyCheckbox_bIsEnabled_0();
    bool Get_ApplyButton_bIsEnabled_0();
    bool Get_SaveButton_bIsEnabled_0();
    void BndEvt__AutoDetectButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__AutoSaveCheckbox_K2Node_ComponentBoundEvent_40_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void BndEvt__AutoApplyCheckbox_K2Node_ComponentBoundEvent_336_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void BndEvt__SaveButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__ApplyButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__CancelButton_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void CallbackUpscalingMethod(int32 NewValue);
    void ExecuteUbergraph_VideoSettingsPage(int32 EntryPoint);
}; // Size: 0x402

#endif
