#ifndef UE4SS_SDK_ModularYesNoUI_HPP
#define UE4SS_SDK_ModularYesNoUI_HPP

class UModularYesNoUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* YesText;                                                        // 0x0348 (size: 0x8)
    class UButton* Yes;                                                               // 0x0350 (size: 0x8)
    class UTextBlock* TextBlock_289;                                                  // 0x0358 (size: 0x8)
    class UTextBlock* NoText;                                                         // 0x0360 (size: 0x8)
    class UButton* No;                                                                // 0x0368 (size: 0x8)
    FText Text;                                                                       // 0x0370 (size: 0x10)
    FModularYesNoUI_CYesPressed YesPressed;                                           // 0x0380 (size: 0x10)
    void YesPressed();

    FText GetText();
    void BndEvt__ModularYesNoUI_YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__ModularYesNoUI_NO_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_ModularYesNoUI(int32 EntryPoint);
    void YesPressed__DelegateSignature();
}; // Size: 0x390

#endif
