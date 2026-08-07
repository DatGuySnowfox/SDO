#ifndef UE4SS_SDK_BP_Inventory_HPP
#define UE4SS_SDK_BP_Inventory_HPP

class UBP_Inventory_C : public UBP_MainJigInventory_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0380 (size: 0x8)
    class UBorder* B_Backpack;                                                        // 0x0388 (size: 0x8)
    class UBorder* B_Bag;                                                             // 0x0390 (size: 0x8)
    class UBorder* B_BodyArmor;                                                       // 0x0398 (size: 0x8)
    class UBorder* B_Legs;                                                            // 0x03A0 (size: 0x8)
    class UBorder* B_Torso;                                                           // 0x03A8 (size: 0x8)
    class UW_PlayerCraftingUI_C* BP_CraftingWidget;                                   // 0x03B0 (size: 0x8)
    class UBP_EquipToContent_C* BP_EquipToContent_Inventory;                          // 0x03B8 (size: 0x8)
    class UBP_EquipToContent_C* BP_EquipToContent_Loot;                               // 0x03C0 (size: 0x8)
    class UButton* Button_Bags;                                                       // 0x03C8 (size: 0x8)
    class UJSIContainer_C* CAccessories;                                              // 0x03D0 (size: 0x8)
    class UJSIContainer_C* CArmor;                                                    // 0x03D8 (size: 0x8)
    class UJSIContainer_C* CBackpack;                                                 // 0x03E0 (size: 0x8)
    class UJSIContainer_C* CBinoculars;                                               // 0x03E8 (size: 0x8)
    class UJSIContainer_C* CCompass;                                                  // 0x03F0 (size: 0x8)
    class UJSIContainer_C* CContainer;                                                // 0x03F8 (size: 0x8)
    class UJSIContainer_C* CEyewear;                                                  // 0x0400 (size: 0x8)
    class UJSIContainer_C* CFaceWear;                                                 // 0x0408 (size: 0x8)
    class UJSIContainer_C* CFeet;                                                     // 0x0410 (size: 0x8)
    class UJSIContainer_C* CFishingRod;                                               // 0x0418 (size: 0x8)
    class UJSIContainer_C* CFlashlight;                                               // 0x0420 (size: 0x8)
    class UJSIContainer_C* CGloves;                                                   // 0x0428 (size: 0x8)
    class UJSIContainer_C* CGPS;                                                      // 0x0430 (size: 0x8)
    class UJSIContainer_C* CHeadWear;                                                 // 0x0438 (size: 0x8)
    class UJSIContainer_C* CLegs;                                                     // 0x0440 (size: 0x8)
    class UJSIContainer_C* CMelee;                                                    // 0x0448 (size: 0x8)
    class UOverlay* ContainerBoxOverlay;                                              // 0x0450 (size: 0x8)
    class UJSIContainer_C* CPistol;                                                   // 0x0458 (size: 0x8)
    class UJSIContainer_C* CPrimary;                                                  // 0x0460 (size: 0x8)
    class UJSIContainer_C* CSecondary;                                                // 0x0468 (size: 0x8)
    class UJSIContainer_C* CThrowable;                                                // 0x0470 (size: 0x8)
    class UJSIContainer_C* CTorso;                                                    // 0x0478 (size: 0x8)
    class UDropItemBackGwidget_C* DropItemBackGwidget;                                // 0x0480 (size: 0x8)
    class USizeBox* EquipmentSizeBox;                                                 // 0x0488 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_Crafting;                         // 0x0490 (size: 0x8)
    class UEquipmentSlotTitle_C* EquipmentSlotTitle_Equipment;                        // 0x0498 (size: 0x8)
    class UBorder* FacewearAttachments;                                               // 0x04A0 (size: 0x8)
    class UBorder* FlashlightAttachment;                                              // 0x04A8 (size: 0x8)
    class UBorder* HeadwearAttachments;                                               // 0x04B0 (size: 0x8)
    class UEquipmentSlotTitle_C* InventoryTitle;                                      // 0x04B8 (size: 0x8)
    class UVerticalBox* InventoryVB;                                                  // 0x04C0 (size: 0x8)
    class UJSIContainer_C* JSIContainer_Pockets;                                      // 0x04C8 (size: 0x8)
    class UBorder* LootContent;                                                       // 0x04D0 (size: 0x8)
    class USizeBox* LootSizeBox;                                                      // 0x04D8 (size: 0x8)
    class UEquipmentSlotTitle_C* LootTitle;                                           // 0x04E0 (size: 0x8)
    class UVerticalBox* LootVB;                                                       // 0x04E8 (size: 0x8)
    class UVerticalBox* OtherContent;                                                 // 0x04F0 (size: 0x8)
    class UOverlay* PrimaryAttachments;                                               // 0x04F8 (size: 0x8)
    class UOverlay* SecondaryAttachments;                                             // 0x0500 (size: 0x8)
    class UOverlay* SidearmAttachments;                                               // 0x0508 (size: 0x8)
    class UBorder* VicinityContent;                                                   // 0x0510 (size: 0x8)
    class UEquipmentSlotTitle_C* VicinityTitle;                                       // 0x0518 (size: 0x8)
    TArray<class UObject*> ContentAdded;                                              // 0x0520 (size: 0x10)
    TArray<class UJSIContainer_C*> AllContainers;                                     // 0x0530 (size: 0x10)
    class UW_VicinityLootUI_C* VicinityWidget;                                        // 0x0540 (size: 0x8)
    class U1_ContainersListUI_C* ContainerListUI;                                     // 0x0548 (size: 0x8)

    void GetContentPanelBySlot(FGameplayTag Tag, class UPanelWidget*& Result);
    void GetInventoryData(TArray<class UJSIContainer_C*>& Containers, TArray<class UJSIContainer_C*>& ExcludedContainers, class UBP_EquipToContent_C*& LootContentTarget, bool& OnCloseClearLootWidget?, class UPanelWidget*& EquipmentContentPanel, TMap<class UJSIContainer_C*, class UJSIContainer_C*>& SwapContainers, class UBP_CharPreview_C*& CharacterPreviewWidget, TSubclassOf<class AActor>& PreviewCharClass);
    void GetValidReloadContainers(TArray<class UJSIContainer_C*>& Containers);
    void GetDropWidget(class UDropItemBackGwidget_C*& DropWRef);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void GetRespiratorDurability(double Dur);
    void ShowRespiratorDurability(bool Show?);
    void GetFlashlightDurability(double Dur);
    void ShowFlashlightDurability(bool Show?);
    void GetNightVisionDurability(double Dur);
    void ShowNightVisionDurability(bool Show?);
    void Destruct();
    void OnInitialized();
    void HandleCraftingInventoryButtonDown();
    void OnWidgetMouseButtonDown_Event_0(FKey Button);
    void OnWidgetMouseButtonDown_Event_1(FKey Button);
    void Construct();
    void Exit();
    void HandleLoot&VicinityButtons();
    void Loot_OnMouseButtonDown(FKey Button);
    void Vicinity_OnMouseButtonDown(FKey Button);
    void LootContentVisible();
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void OnLootAdded/Removed(bool Added?);
    void OnCreatedFromUtility();
    void BndEvt__BP_Inventory_Button_0_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void ContainerListClicked();
    void ExecuteUbergraph_BP_Inventory(int32 EntryPoint);
}; // Size: 0x550

#endif
