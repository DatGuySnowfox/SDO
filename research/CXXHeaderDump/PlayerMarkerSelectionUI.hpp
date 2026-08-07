#ifndef UE4SS_SDK_PlayerMarkerSelectionUI_HPP
#define UE4SS_SDK_PlayerMarkerSelectionUI_HPP

class UPlayerMarkerSelectionUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_Animal;                                                     // 0x02C8 (size: 0x8)
    class UButton* Button_Black;                                                      // 0x02D0 (size: 0x8)
    class UButton* Button_Blue;                                                       // 0x02D8 (size: 0x8)
    class UButton* Button_Cross;                                                      // 0x02E0 (size: 0x8)
    class UButton* Button_Down;                                                       // 0x02E8 (size: 0x8)
    class UButton* Button_Home;                                                       // 0x02F0 (size: 0x8)
    class UButton* Button_Left;                                                       // 0x02F8 (size: 0x8)
    class UButton* Button_Marker1;                                                    // 0x0300 (size: 0x8)
    class UButton* Button_Marker2;                                                    // 0x0308 (size: 0x8)
    class UButton* Button_Red;                                                        // 0x0310 (size: 0x8)
    class UButton* Button_Right;                                                      // 0x0318 (size: 0x8)
    class UButton* Button_Skull;                                                      // 0x0320 (size: 0x8)
    class UButton* Button_Tent;                                                       // 0x0328 (size: 0x8)
    class UButton* Button_Up;                                                         // 0x0330 (size: 0x8)
    class UButton* Button_Vehicle;                                                    // 0x0338 (size: 0x8)
    class UButton* Button_White;                                                      // 0x0340 (size: 0x8)
    class USettingRowSmall_C* SettingRowSmall;                                        // 0x0348 (size: 0x8)
    class UTextBlock* SizeText;                                                       // 0x0350 (size: 0x8)
    class USlider* Slider_168;                                                        // 0x0358 (size: 0x8)
    class UBPC_MinimapSystem_C* Map;                                                  // 0x0360 (size: 0x8)

    void BndEvt__PlayerMarkerSelectionUI_Button_Skull_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Animal_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Cross_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Right_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Up_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Down_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Left_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Marker1_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Marker2_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__PlayerMarkerSelectionUI_Button_Black_K2Node_ComponentBoundEvent_10_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Tent_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_White_K2Node_ComponentBoundEvent_11_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Red_K2Node_ComponentBoundEvent_12_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Blue_K2Node_ComponentBoundEvent_13_OnButtonClickedEvent__DelegateSignature();
    void OnVisibilityChanged_Event(ESlateVisibility InVisibility);
    void BndEvt__PlayerMarkerSelectionUI_Button_Vehicle_K2Node_ComponentBoundEvent_14_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Button_Home_K2Node_ComponentBoundEvent_15_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__PlayerMarkerSelectionUI_Slider_168_K2Node_ComponentBoundEvent_16_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void Event_Marker(bool NewValue);
    void ExecuteUbergraph_PlayerMarkerSelectionUI(int32 EntryPoint);
}; // Size: 0x368

#endif
