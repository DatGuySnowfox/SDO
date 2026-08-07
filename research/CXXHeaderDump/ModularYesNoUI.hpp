#ifndef UE4SS_SDK_ModularYesNoUI_HPP
#define UE4SS_SDK_ModularYesNoUI_HPP

class UModularYesNoUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* No;                                                                // 0x02C8 (size: 0x8)
    class UTextBlock* NoText;                                                         // 0x02D0 (size: 0x8)
    class UTextBlock* TextBlock_289;                                                  // 0x02D8 (size: 0x8)
    class UButton* Yes;                                                               // 0x02E0 (size: 0x8)
    class UTextBlock* YesText;                                                        // 0x02E8 (size: 0x8)
    FText Text;                                                                       // 0x02F0 (size: 0x18)
    FModularYesNoUI_CYesPressed YesPressed;                                           // 0x0308 (size: 0x10)
    void YesPressed();

    FText GetText();
    void BndEvt__ModularYesNoUI_YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__ModularYesNoUI_NO_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_ModularYesNoUI(int32 EntryPoint);
    void YesPressed__DelegateSignature();
}; // Size: 0x318

#endif
