#ifndef UE4SS_SDK_EnhancedMappingOption_HPP
#define UE4SS_SDK_EnhancedMappingOption_HPP

class UEnhancedMappingOption_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UInputKeySelector* InputKeySelector;                                        // 0x0348 (size: 0x8)
    FName InputName;                                                                  // 0x0350 (size: 0x8)
    EPlayerMappableKeySlot SlotPriority;                                              // 0x0358 (size: 0x1)
    bool SelectingKey;                                                                // 0x0359 (size: 0x1)

    void Update(FKey Key);
    void BndEvt__EnhancedMappingOption_InputKeySelector_220_K2Node_ComponentBoundEvent_0_OnKeySelected__DelegateSignature(FInputChord SelectedKey);
    void BndEvt__EnhancedMappingOption_InputKeySelector_220_K2Node_ComponentBoundEvent_1_OnIsSelectingKeyChanged__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_EnhancedMappingOption(int32 EntryPoint);
}; // Size: 0x35A

#endif
