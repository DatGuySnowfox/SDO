#ifndef UE4SS_SDK_EnhancedMappingOption_HPP
#define UE4SS_SDK_EnhancedMappingOption_HPP

class UEnhancedMappingOption_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UInputKeySelector* InputKeySelector;                                        // 0x02C8 (size: 0x8)
    FName InputName;                                                                  // 0x02D0 (size: 0x8)
    EPlayerMappableKeySlot SlotPriority;                                              // 0x02D8 (size: 0x1)
    bool SelectingKey;                                                                // 0x02D9 (size: 0x1)

    void Update(FKey Key);
    void BndEvt__EnhancedMappingOption_InputKeySelector_220_K2Node_ComponentBoundEvent_0_OnKeySelected__DelegateSignature(FInputChord SelectedKey);
    void BndEvt__EnhancedMappingOption_InputKeySelector_220_K2Node_ComponentBoundEvent_1_OnIsSelectingKeyChanged__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_EnhancedMappingOption(int32 EntryPoint);
}; // Size: 0x2DA

#endif
