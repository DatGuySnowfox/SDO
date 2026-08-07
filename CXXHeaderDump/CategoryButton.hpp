#ifndef UE4SS_SDK_CategoryButton_HPP
#define UE4SS_SDK_CategoryButton_HPP

class UCategoryButton_C : public URadioButton
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0300 (size: 0x8)
    class UImage* Image_0;                                                            // 0x0308 (size: 0x8)
    class UButton* MainButton;                                                        // 0x0310 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0318 (size: 0x8)

    FSlateColor Get_Text_ColorAndOpacity_0();
    ESlateVisibility GetIndicatorVisibility();
    void BndEvt__MainButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void UpdateSelected(bool InSelected);
    void BndEvt__CategoryButton_MainButton_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature();
    void ExecuteUbergraph_CategoryButton(int32 EntryPoint);
}; // Size: 0x320

#endif
