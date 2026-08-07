#ifndef UE4SS_SDK_CommonButtonTab_HPP
#define UE4SS_SDK_CommonButtonTab_HPP

class UCommonButtonTab_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button;                                                            // 0x02C8 (size: 0x8)
    int32 TabToOpen;                                                                  // 0x02D0 (size: 0x4)
    class UPhotoModeWidget_C* PhotoModeWidgetRef;                                     // 0x02D8 (size: 0x8)
    class UObject* TabIcon;                                                           // 0x02E0 (size: 0x8)
    FVector2D ButtonSize;                                                             // 0x02E8 (size: 0x10)
    FVector2D ButtonSizeWhenSelected;                                                 // 0x02F8 (size: 0x10)
    FSlateColor SelectedTabColor;                                                     // 0x0308 (size: 0x14)
    FSlateColor UnselectedTabColor;                                                   // 0x031C (size: 0x14)

    void Initialize(class UPhotoModeWidget_C* PhotoModeWidgetRef);
    void SetSelectedTabStyle();
    void SetUnselectedTabStyle();
    void BndEvt__Button_99_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void PreConstruct(bool IsDesignTime);
    void ExecuteUbergraph_CommonButtonTab(int32 EntryPoint);
}; // Size: 0x330

#endif
