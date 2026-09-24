#ifndef UE4SS_SDK_TravelToLocUI_HPP
#define UE4SS_SDK_TravelToLocUI_HPP

class UTravelToLocUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UButton* YesButton;                                                         // 0x0348 (size: 0x8)
    class UTextBlock* Text;                                                           // 0x0350 (size: 0x8)
    class UButton* NoButton;                                                          // 0x0358 (size: 0x8)
    class UWidgetAnimation* Fade;                                                     // 0x0360 (size: 0x8)
    class UWidgetAnimation* MoveLoc;                                                  // 0x0368 (size: 0x8)
    class ABP_SubwayTravelDoor_C* DoorRef;                                            // 0x0370 (size: 0x8)
    class AActor* ActorRef;                                                           // 0x0378 (size: 0x8)

    void ClearUI();
    void BndEvt__Button_60_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__RespawnScreen_RespawnButton_K2Node_ComponentBoundEvent_3_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__RespawnScreen_QuitToMainMenuButton_K2Node_ComponentBoundEvent_5_OnButtonHoverEvent__DelegateSignature();
    void Exit();
    void ExecuteUbergraph_TravelToLocUI(int32 EntryPoint);
}; // Size: 0x380

#endif
