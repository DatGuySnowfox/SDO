#ifndef UE4SS_SDK_ItemButton_HPP
#define UE4SS_SDK_ItemButton_HPP

class UItemButton_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0340 (size: 0x8)
    class UTextBlock* Name;                                                           // 0x0348 (size: 0x8)
    class UButton* Button_41;                                                         // 0x0350 (size: 0x8)
    class UJigsawItem_DataAsset_C* ItemId;                                            // 0x0358 (size: 0x8)
    class UDebugMenu_C* DebugMenu;                                                    // 0x0360 (size: 0x8)

    FLinearColor GetRarity(FGameplayTag Rarity);
    void BndEvt__ItemButton_Button_41_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void ExecuteUbergraph_ItemButton(int32 EntryPoint);
}; // Size: 0x368

#endif
