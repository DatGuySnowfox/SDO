#ifndef UE4SS_SDK_ItemButton_HPP
#define UE4SS_SDK_ItemButton_HPP

class UItemButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_41;                                                         // 0x02C8 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x02D0 (size: 0x8)
    class UJigsawItem_DataAsset_C* ItemId;                                            // 0x02D8 (size: 0x8)
    class UDebugMenu_C* DebugMenu;                                                    // 0x02E0 (size: 0x8)

    FLinearColor GetRarity(FGameplayTag Rarity);
    void BndEvt__ItemButton_Button_41_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_ItemButton(int32 EntryPoint);
}; // Size: 0x2E8

#endif
