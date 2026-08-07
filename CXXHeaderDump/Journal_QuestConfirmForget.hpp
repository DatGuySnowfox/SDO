#ifndef UE4SS_SDK_Journal_QuestConfirmForget_HPP
#define UE4SS_SDK_Journal_QuestConfirmForget_HPP

class UJournal_QuestConfirmForget_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UImage* Image_27;                                                           // 0x02C8 (size: 0x8)
    class UButton* No;                                                                // 0x02D0 (size: 0x8)
    class UTextBlock* NoText;                                                         // 0x02D8 (size: 0x8)
    class UButton* Yes;                                                               // 0x02E0 (size: 0x8)
    class UTextBlock* YesText;                                                        // 0x02E8 (size: 0x8)
    class ULoadingScreenWidget_C* LoadingScreen;                                      // 0x02F0 (size: 0x8)
    class UJournal_QuestsUI_C* Journal;                                               // 0x02F8 (size: 0x8)

    void ClearUI();
    void BndEvt__YES_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__NO_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void CloseMenu();
    void ExecuteUbergraph_Journal_QuestConfirmForget(int32 EntryPoint);
}; // Size: 0x300

#endif
