#ifndef UE4SS_SDK_W_PlayerCraftingUI_HPP
#define UE4SS_SDK_W_PlayerCraftingUI_HPP

class UW_PlayerCraftingUI_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_81;                                                         // 0x02C8 (size: 0x8)
    class UButton* Button_Decrease;                                                   // 0x02D0 (size: 0x8)
    class UButton* Button_Increase;                                                   // 0x02D8 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle;                                  // 0x02E0 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_1;                                // 0x02E8 (size: 0x8)
    class UJSIContainer_C* JSIContainer;                                              // 0x02F0 (size: 0x8)
    class UJSIContainer_C* JSIContainer_1;                                            // 0x02F8 (size: 0x8)
    class UBorder* LoadingT;                                                          // 0x0300 (size: 0x8)
    class UTextBlock* TextBlock_75;                                                   // 0x0308 (size: 0x8)
    class UThrobber* Throbber_567;                                                    // 0x0310 (size: 0x8)
    TMap<UJSI_Slot_C*, int32> AddedItems;                                             // 0x0318 (size: 0x50)
    class UJSI_Slot_C* CurrentFaded;                                                  // 0x0368 (size: 0x8)
    class UBP_JigMultiplayer_C* MPComponent;                                          // 0x0370 (size: 0x8)
    TArray<FGuid> RequestConsume;                                                     // 0x0378 (size: 0x10)
    int32 CraftIndex;                                                                 // 0x0388 (size: 0x4)
    int32 CountIndex;                                                                 // 0x038C (size: 0x4)

    void SetCraftableItems();
    FText GetText();
    void CheckIngredientsAvailability(bool& Proceed);
    void AddRequiredItems();
    void InitializeInventory();
    void BindEvents();
    void EventOnMouseButtonDown(class UJSIContainer_C* Container, class UJSI_Slot_C* SlotRef, FKey Button);
    void BndEvt__Button_81_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void BndEvt__BP_CraftingWidget_Button_Decrease_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__BP_CraftingWidget_Button_Increase_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_W_PlayerCraftingUI(int32 EntryPoint);
}; // Size: 0x390

#endif
