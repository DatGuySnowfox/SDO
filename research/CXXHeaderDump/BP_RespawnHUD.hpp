#ifndef UE4SS_SDK_BP_RespawnHUD_HPP
#define UE4SS_SDK_BP_RespawnHUD_HPP

class UBP_RespawnHUD_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UButton* RespawnButton;                                                     // 0x0348 (size: 0x8)
    class ABP_ExampleCharacter_C* Character;                                          // 0x0350 (size: 0x8)

    void BndEvt__RespawnButton_K2Node_ComponentBoundEvent_130_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_BP_RespawnHUD(int32 EntryPoint);
}; // Size: 0x358

#endif
