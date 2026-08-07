#ifndef UE4SS_SDK_BP_RespawnHUD_HPP
#define UE4SS_SDK_BP_RespawnHUD_HPP

class UBP_RespawnHUD_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* RespawnButton;                                                     // 0x02C8 (size: 0x8)
    class ABP_ExampleCharacter_C* Character;                                          // 0x02D0 (size: 0x8)

    void BndEvt__RespawnButton_K2Node_ComponentBoundEvent_130_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_BP_RespawnHUD(int32 EntryPoint);
}; // Size: 0x2D8

#endif
