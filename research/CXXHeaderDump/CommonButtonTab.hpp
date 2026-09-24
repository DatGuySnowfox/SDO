#ifndef UE4SS_SDK_CommonButtonTab_HPP
#define UE4SS_SDK_CommonButtonTab_HPP

class UCommonButtonTab_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UButton* Button;                                                            // 0x0348 (size: 0x8)
    int32 TabToOpen;                                                                  // 0x0350 (size: 0x4)
    class UPhotoModeWidget_C* PhotoModeWidgetRef;                                     // 0x0358 (size: 0x8)
    class UObject* TabIcon;                                                           // 0x0360 (size: 0x8)
    FVector2D ButtonSize;                                                             // 0x0368 (size: 0x10)
    FVector2D ButtonSizeWhenSelected;                                                 // 0x0378 (size: 0x10)
    FSlateColor SelectedTabColor;                                                     // 0x0388 (size: 0x14)
    FSlateColor UnselectedTabColor;                                                   // 0x039C (size: 0x14)

    void Initialize(class UPhotoModeWidget_C* PhotoModeWidgetRef);
    void SetSelectedTabStyle();
    void SetUnselectedTabStyle();
    void BndEvt__Button_99_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_CommonButtonTab(int32 EntryPoint);
}; // Size: 0x3B0

#endif
