#ifndef UE4SS_SDK_KeypadUI_HPP
#define UE4SS_SDK_KeypadUI_HPP

class UKeypadUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Code;                                                           // 0x0348 (size: 0x8)
    class UButton* Button_Cancel;                                                     // 0x0350 (size: 0x8)
    class UButton* Button_9;                                                          // 0x0358 (size: 0x8)
    class UButton* Button_8;                                                          // 0x0360 (size: 0x8)
    class UButton* Button_7;                                                          // 0x0368 (size: 0x8)
    class UButton* Button_6;                                                          // 0x0370 (size: 0x8)
    class UButton* Button_5;                                                          // 0x0378 (size: 0x8)
    class UButton* Button_4;                                                          // 0x0380 (size: 0x8)
    class UButton* Button_3;                                                          // 0x0388 (size: 0x8)
    class UButton* Button_2;                                                          // 0x0390 (size: 0x8)
    class UButton* Button_1;                                                          // 0x0398 (size: 0x8)
    class UButton* Button_0;                                                          // 0x03A0 (size: 0x8)
    FString CurrentCode;                                                              // 0x03A8 (size: 0x10)
    int32 AcceptedCode;                                                               // 0x03B8 (size: 0x4)
    FKeypadUI_CCorrectCodeEntered CorrectCodeEntered;                                 // 0x03C0 (size: 0x10)
    void CorrectCodeEntered();
    FKeypadUI_CIncorrectCodeEntered IncorrectCodeEntered;                             // 0x03D0 (size: 0x10)
    void IncorrectCodeEntered();
    class ABP_KeypadDoor_C* DoorRef;                                                  // 0x03E0 (size: 0x8)

    void BndEvt__KeypadUI_Button_1_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_2_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_3_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_4_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_5_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_6_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_7_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_8_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_9_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__KeypadUI_Button_0_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void SetCurrentCode(FString Num);
    void PausePressed();
    void BndEvt__KeypadUI_Button_Cancel_K2Node_ComponentBoundEvent_10_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void ExecuteUbergraph_KeypadUI(int32 EntryPoint);
    void IncorrectCodeEntered__DelegateSignature();
    void CorrectCodeEntered__DelegateSignature();
}; // Size: 0x3E8

#endif
