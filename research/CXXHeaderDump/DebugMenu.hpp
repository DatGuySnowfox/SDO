#ifndef UE4SS_SDK_DebugMenu_HPP
#define UE4SS_SDK_DebugMenu_HPP

class UDebugMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UButton* Button_LeftItems;                                                  // 0x02C8 (size: 0x8)
    class UButton* Button_RightItems;                                                 // 0x02D0 (size: 0x8)
    class UButton* DamagePlayerButton;                                                // 0x02D8 (size: 0x8)
    class UComboBoxString* DropDownWeather;                                           // 0x02E0 (size: 0x8)
    class UEditableText* EditableDilationTxt;                                         // 0x02E8 (size: 0x8)
    class UEditableText* EditableText_357;                                            // 0x02F0 (size: 0x8)
    class UEditableText* EditableText_All;                                            // 0x02F8 (size: 0x8)
    class UEditableText* EditableText_Attachments;                                    // 0x0300 (size: 0x8)
    class UEditableText* EditableText_Buildables;                                     // 0x0308 (size: 0x8)
    class UEditableText* EditableText_Consumables;                                    // 0x0310 (size: 0x8)
    class UEditableText* EditableText_CraftingMaterials;                              // 0x0318 (size: 0x8)
    class UEditableText* EditableText_Equipment;                                      // 0x0320 (size: 0x8)
    class UEditableText* EditableText_Other;                                          // 0x0328 (size: 0x8)
    class UEditableText* EditableText_Weapons;                                        // 0x0330 (size: 0x8)
    class UButton* FirstAidButton;                                                    // 0x0338 (size: 0x8)
    class UButton* FishingButton;                                                     // 0x0340 (size: 0x8)
    class UButton* FitnessButton;                                                     // 0x0348 (size: 0x8)
    class UButton* GiveXPButton;                                                      // 0x0350 (size: 0x8)
    class UButton* MarksmanshipButton;                                                // 0x0358 (size: 0x8)
    class UButton* ReduceHungerButton;                                                // 0x0360 (size: 0x8)
    class UButton* ReduceThirstButton;                                                // 0x0368 (size: 0x8)
    class UButton* ReloadingButton;                                                   // 0x0370 (size: 0x8)
    class UButton* ResetLevelButton;                                                  // 0x0378 (size: 0x8)
    class UButton* ResetPassiveSkillsButton;                                          // 0x0380 (size: 0x8)
    class UButton* ResetSkillTreeButton;                                              // 0x0388 (size: 0x8)
    class UButton* ScavengingButton;                                                  // 0x0390 (size: 0x8)
    class UScrollBox* ScrollBox_All;                                                  // 0x0398 (size: 0x8)
    class UScrollBox* ScrollBox_Attachments;                                          // 0x03A0 (size: 0x8)
    class UScrollBox* ScrollBox_Buildables;                                           // 0x03A8 (size: 0x8)
    class UScrollBox* ScrollBox_Consumable;                                           // 0x03B0 (size: 0x8)
    class UScrollBox* ScrollBox_CraftingMaterials;                                    // 0x03B8 (size: 0x8)
    class UScrollBox* ScrollBox_Equipment;                                            // 0x03C0 (size: 0x8)
    class UScrollBox* ScrollBox_Other;                                                // 0x03C8 (size: 0x8)
    class UScrollBox* ScrollBox_Vehicle;                                              // 0x03D0 (size: 0x8)
    class UScrollBox* ScrollBox_Weapons;                                              // 0x03D8 (size: 0x8)
    class USettingRow_C* SettingRow;                                                  // 0x03E0 (size: 0x8)
    class USettingRow_C* SettingRow_1;                                                // 0x03E8 (size: 0x8)
    class USettingRow_C* SettingRow_2;                                                // 0x03F0 (size: 0x8)
    class USettingRow_C* SettingRow_3;                                                // 0x03F8 (size: 0x8)
    class USettingRow_C* SettingRow_4;                                                // 0x0400 (size: 0x8)
    class USettingRow_C* SettingRow_5;                                                // 0x0408 (size: 0x8)
    class USettingRow_C* SettingRow_6;                                                // 0x0410 (size: 0x8)
    class USettingRow_C* SettingRow_7;                                                // 0x0418 (size: 0x8)
    class USettingRow_C* SettingRow_8;                                                // 0x0420 (size: 0x8)
    class USettingRow_C* SettingRow_12;                                               // 0x0428 (size: 0x8)
    class USettingRow_C* SettingRow_13;                                               // 0x0430 (size: 0x8)
    class USettingRow_C* SettingRow_14;                                               // 0x0438 (size: 0x8)
    class USettingRow_C* SettingRow_15;                                               // 0x0440 (size: 0x8)
    class USettingRow_C* SettingRow_16;                                               // 0x0448 (size: 0x8)
    class USettingRow_C* SettingRow_17;                                               // 0x0450 (size: 0x8)
    class USettingRow_C* SettingRow_18;                                               // 0x0458 (size: 0x8)
    class USettingRow_C* SettingRow_19;                                               // 0x0460 (size: 0x8)
    class USettingRow_C* SettingRow_20;                                               // 0x0468 (size: 0x8)
    class USettingRow_C* SettingRow_21;                                               // 0x0470 (size: 0x8)
    class USettingRow_C* SettingRow_22;                                               // 0x0478 (size: 0x8)
    class USettingRow_C* SettingRow_23;                                               // 0x0480 (size: 0x8)
    class USettingRow_C* SettingRow_24;                                               // 0x0488 (size: 0x8)
    class USettingRow_C* SettingRow_25;                                               // 0x0490 (size: 0x8)
    class USettingRow_C* SettingRow_26;                                               // 0x0498 (size: 0x8)
    class USettingRow_C* SettingRow_27;                                               // 0x04A0 (size: 0x8)
    class USettingRow_C* SettingRow_28;                                               // 0x04A8 (size: 0x8)
    class USlider* Slider_126;                                                        // 0x04B0 (size: 0x8)
    class UButton* SneakingButton;                                                    // 0x04B8 (size: 0x8)
    class UButton* SpawnCameraButton;                                                 // 0x04C0 (size: 0x8)
    class USpinBox* SpinBox_719;                                                      // 0x04C8 (size: 0x8)
    class UButton* StrengthButton;                                                    // 0x04D0 (size: 0x8)
    class UButton* ThiefButton;                                                       // 0x04D8 (size: 0x8)
    class UButton* ToughnessButton;                                                   // 0x04E0 (size: 0x8)
    class UWidgetSwitcher* WidgetSwitcher;                                            // 0x04E8 (size: 0x8)
    float Amount;                                                                     // 0x04F0 (size: 0x4)
    int32 ActiveTabIndex;                                                             // 0x04F4 (size: 0x4)
    TArray<class UJigsawItem_DataAsset_C*> AllItems;                                  // 0x04F8 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Weapons;                                   // 0x0508 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Attachments;                               // 0x0518 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Equipment;                                 // 0x0528 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Consumables;                               // 0x0538 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> CraftingMats;                              // 0x0548 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Other;                                     // 0x0558 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> Buildables;                                // 0x0568 (size: 0x10)
    TArray<class UJigsawItem_DataAsset_C*> HideTheseItems;                            // 0x0578 (size: 0x10)

    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void SetActiveItemsList(int32 Index);
    void SearchForOther(FText Text);
    void SearchForBuildables(FText Text);
    void SearchForCraftingMats(FText Text);
    void SearchForConsumable(FText Text);
    void SearchForEquipment(FText Text);
    void SearchForAttachments(FText Text);
    void SearchForWeapons(FText Text);
    void GetDataTableItems_All();
    void GetDataTableItems_Other();
    void GetDataTableItems_Buildables();
    void GetDataTableItems_CraftingMaterials();
    void GetDataTableItems_Consumable();
    void GetDataTableItems_Equipment();
    void GetDataTableItems_Attachments();
    void GetDataTableItems_Weapons();
    void GetAllDataTableRefs();
    void ChangeWeather(class UUDS_Weather_Settings_C* New Weather Type);
    void SearchForAll(FText Text);
    void GetDataTable_Vehicles();
    void Construct();
    void BndEvt__DifficultySettingsPage_ResetLevelButton_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Difficulty_Other_ResetSkillTreeButton_K2Node_ComponentBoundEvent_12_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__Difficulty_Other_ResetPassiveSkillsButton_K2Node_ComponentBoundEvent_15_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_SpinBox_719_K2Node_ComponentBoundEvent_3_OnSpinBoxValueChangedEvent__DelegateSignature(float InValue);
    void BndEvt__DebugMenu_GiveXPButton_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_FitnessButton_K2Node_ComponentBoundEvent_8_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_StrengthButton_K2Node_ComponentBoundEvent_9_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ToughnessButton_K2Node_ComponentBoundEvent_10_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_SneakingButton_K2Node_ComponentBoundEvent_11_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_FirstAidButton_K2Node_ComponentBoundEvent_13_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ReloadingButton_K2Node_ComponentBoundEvent_14_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_MarksmanshipButton_K2Node_ComponentBoundEvent_16_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ThiefButton_K2Node_ComponentBoundEvent_17_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_FishingButton_K2Node_ComponentBoundEvent_18_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ScavengingButton_K2Node_ComponentBoundEvent_29_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_DamagePlayerButton_K2Node_ComponentBoundEvent_30_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ReduceHungerButton_K2Node_ComponentBoundEvent_32_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_ReduceThirstButton_K2Node_ComponentBoundEvent_33_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_DropDownWeather_K2Node_ComponentBoundEvent_31_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__DebugMenu_SpawnCameraButton_K2Node_ComponentBoundEvent_34_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_EditableText_All_K2Node_ComponentBoundEvent_1_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_Button_LeftItems_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_Button_RightItems_K2Node_ComponentBoundEvent_19_OnButtonClickedEvent__DelegateSignature();
    void BndEvt__DebugMenu_EditableText_Attachments_K2Node_ComponentBoundEvent_20_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_Equipment_K2Node_ComponentBoundEvent_21_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_Consumables_K2Node_ComponentBoundEvent_22_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_CraftingMaterials_K2Node_ComponentBoundEvent_23_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_Buildables_K2Node_ComponentBoundEvent_24_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_Other_K2Node_ComponentBoundEvent_25_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void BndEvt__DebugMenu_EditableText_All_K2Node_ComponentBoundEvent_26_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void Tick(FGeometry MyGeometry, float InDeltaTime);
    void BndEvt__DebugMenu_Slider_126_K2Node_ComponentBoundEvent_27_OnFloatValueChangedEvent__DelegateSignature(float Value);
    void BndEvt__DebugMenu_EditableDilationTxt_K2Node_ComponentBoundEvent_5_OnEditableTextChangedEvent__DelegateSignature(const FText& Text);
    void ExecuteUbergraph_DebugMenu(int32 EntryPoint);
}; // Size: 0x588

#endif
