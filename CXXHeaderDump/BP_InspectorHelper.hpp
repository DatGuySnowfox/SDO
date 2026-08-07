#ifndef UE4SS_SDK_BP_InspectorHelper_HPP
#define UE4SS_SDK_BP_InspectorHelper_HPP

class UBP_InspectorHelper_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_128;                                                        // 0x02C8 (size: 0x8)
    class UCheckBox* CheckBox_147;                                                    // 0x02D0 (size: 0x8)
    class UEditableTextBox* FOVTxt;                                                   // 0x02D8 (size: 0x8)
    class UHorizontalBox* HorizontalBox_0;                                            // 0x02E0 (size: 0x8)
    class UHorizontalBox* HorizontalBox_150;                                          // 0x02E8 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x02F0 (size: 0x8)
    class UEditableTextBox* LocX;                                                     // 0x02F8 (size: 0x8)
    class UEditableTextBox* LocY;                                                     // 0x0300 (size: 0x8)
    class UEditableTextBox* LocZ;                                                     // 0x0308 (size: 0x8)
    class UVerticalBox* MainVB;                                                       // 0x0310 (size: 0x8)
    class UEditableTextBox* RotX;                                                     // 0x0318 (size: 0x8)
    class UEditableTextBox* RotY;                                                     // 0x0320 (size: 0x8)
    class UEditableTextBox* RotZ;                                                     // 0x0328 (size: 0x8)
    class UEditableTextBox* ScaleX;                                                   // 0x0330 (size: 0x8)
    class UEditableTextBox* ScaleY;                                                   // 0x0338 (size: 0x8)
    class UEditableTextBox* ScaleZ;                                                   // 0x0340 (size: 0x8)
    class ABP_ItemInspector_C* InspectorBP;                                           // 0x0348 (size: 0x8)
    bool ProcessTextInputs;                                                           // 0x0350 (size: 0x1)
    class UJSI_Slot_C* JigItem;                                                       // 0x0358 (size: 0x8)
    class UJSI_Slot_C* JigDebug;                                                      // 0x0360 (size: 0x8)

    void UpdateTransform();
    void BndEvt__LocX_K2Node_ComponentBoundEvent_0_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__FOVTxt_K2Node_ComponentBoundEvent_1_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__LocY_K2Node_ComponentBoundEvent_2_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__LocZ_K2Node_ComponentBoundEvent_3_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__RotX_K2Node_ComponentBoundEvent_4_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__RotY_K2Node_ComponentBoundEvent_5_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__RotZ_K2Node_ComponentBoundEvent_6_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__Button_128_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature();
    void SetInitialValues(class ABP_ItemInspector_C* InspectorBP);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void SetJigRef(class UJSI_Slot_C* JigItem);
    void BndEvt__BP_InspectorHelper_CheckBox_147_K2Node_ComponentBoundEvent_9_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked);
    void BndEvt__BP_InspectorHelper_ScaleX_K2Node_ComponentBoundEvent_8_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__BP_InspectorHelper_ScaleY_K2Node_ComponentBoundEvent_10_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__BP_InspectorHelper_ScaleZ_K2Node_ComponentBoundEvent_11_OnEditableTextBoxChangedEvent__DelegateSignature(const FText& Text);
    void ExecuteUbergraph_BP_InspectorHelper(int32 EntryPoint);
}; // Size: 0x368

#endif
