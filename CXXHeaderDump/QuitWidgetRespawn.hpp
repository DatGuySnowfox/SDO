#ifndef UE4SS_SDK_QuitWidgetRespawn_HPP
#define UE4SS_SDK_QuitWidgetRespawn_HPP

class UQuitWidgetRespawn_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* No;                                                                // 0x02C8 (size: 0x8)
    class UTextBlock* NoText;                                                         // 0x02D0 (size: 0x8)
    class UButton* Yes;                                                               // 0x02D8 (size: 0x8)
    class UTextBlock* YesText;                                                        // 0x02E0 (size: 0x8)
    bool Permadeath?;                                                                 // 0x02E8 (size: 0x1)

    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    void ClearUI();
    void BndEvt__YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__NO_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__YES_K2Node_ComponentBoundEvent_4_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__NO_K2Node_ComponentBoundEvent_6_OnButtonHoverEvent__DelegateSignature();
    void CloseMenu();
    void Construct();
    void ExecuteUbergraph_QuitWidgetRespawn(int32 EntryPoint);
}; // Size: 0x2E9

#endif
