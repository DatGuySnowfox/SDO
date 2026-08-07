#ifndef UE4SS_SDK_CharacterBarberMenu_HPP
#define UE4SS_SDK_CharacterBarberMenu_HPP

class UCharacterBarberMenu_C : public UUserWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C0 (size: 0x8)
    class UTextBlock* BackText;                                                       // 0x02C8 (size: 0x8)
    class UComboBoxString* DropDownAccessory1;                                        // 0x02D0 (size: 0x8)
    class UComboBoxString* DropDownAccessory2;                                        // 0x02D8 (size: 0x8)
    class UComboBoxString* DropDownAccessory3;                                        // 0x02E0 (size: 0x8)
    class UComboBoxString* DropDownBeard;                                             // 0x02E8 (size: 0x8)
    class UComboBoxString* DropDownBeardColor;                                        // 0x02F0 (size: 0x8)
    class UComboBoxString* DropDownEyebrows;                                          // 0x02F8 (size: 0x8)
    class UComboBoxString* DropDownHairColor;                                         // 0x0300 (size: 0x8)
    class UComboBoxString* DropDownHairType;                                          // 0x0308 (size: 0x8)
    class UComboBoxString* DropDownMouth;                                             // 0x0310 (size: 0x8)
    class UComboBoxString* DropDownOccupation;                                        // 0x0318 (size: 0x8)
    class UComboBoxString* DropDownSex;                                               // 0x0320 (size: 0x8)
    class UComboBoxString* DropDownSkinColor_2;                                       // 0x0328 (size: 0x8)
    class UEditableTextBox* EditableTextBox_Age;                                      // 0x0330 (size: 0x8)
    class UEditableTextBox* EditableTextBox_Forename;                                 // 0x0338 (size: 0x8)
    class UEditableTextBox* EditableTextBox_Surname;                                  // 0x0340 (size: 0x8)
    class UButton* ExitButton;                                                        // 0x0348 (size: 0x8)
    class ABP_PlayerCharacter_C* Chr;                                                 // 0x0350 (size: 0x8)
    TArray<TEnumAsByte<Enum_Occupation::Type>> OccupationTypes;                       // 0x0358 (size: 0x10)
    bool SkipAnim?;                                                                   // 0x0368 (size: 0x1)
    bool IsMale?;                                                                     // 0x0369 (size: 0x1)
    FCharacterBarberMenu_CExitMenu ExitMenu;                                          // 0x0370 (size: 0x10)
    void ExitMenu();
    class ABP_Barber_C* Barber;                                                       // 0x0380 (size: 0x8)
    class ABP_PlayerController_C* Controller;                                         // 0x0388 (size: 0x8)

    void UpdateClothing(bool Male?);
    void Sex(FString String);
    void AccessoryType3(FString String);
    void AccessoryType2(FString String);
    void AccessoryType1(FString String);
    void MouthType(FString String);
    void EyebrowsType(FString String);
    void BeardColor(FString String);
    void BeardType(FString String);
    void HairColor(FString String);
    void HairType(FString String);
    void SkinColor(FString String);
    void BndEvt__BackButton_K2Node_ComponentBoundEvent_13_OnButtonClickedEvent__DelegateSignature();
    void Construct();
    void Exit();
    void BndEvt__CharacterCreatorMenu_ComboBoxString_60_K2Node_ComponentBoundEvent_3_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownSkinColor_2_K2Node_ComponentBoundEvent_0_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownBeardColor_K2Node_ComponentBoundEvent_1_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownBeard_K2Node_ComponentBoundEvent_2_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownHairType_K2Node_ComponentBoundEvent_4_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_EditableTextBox_Forename_K2Node_ComponentBoundEvent_5_OnEditableTextBoxCommittedEvent__DelegateSignature(const FText& Text, TEnumAsByte<ETextCommit::Type> CommitMethod);
    void BndEvt__CharacterCreatorMenu_EditableTextBox_Surname_K2Node_ComponentBoundEvent_6_OnEditableTextBoxCommittedEvent__DelegateSignature(const FText& Text, TEnumAsByte<ETextCommit::Type> CommitMethod);
    void BndEvt__CharacterCreatorMenu_EditableTextBox_Age_K2Node_ComponentBoundEvent_7_OnEditableTextBoxCommittedEvent__DelegateSignature(const FText& Text, TEnumAsByte<ETextCommit::Type> CommitMethod);
    void BndEvt__CharacterCreatorMenu_DropDownEyebrows_K2Node_ComponentBoundEvent_8_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownOccupation_K2Node_ComponentBoundEvent_9_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_BackButton_K2Node_ComponentBoundEvent_12_OnButtonHoverEvent__DelegateSignature();
    void BndEvt__CharacterCreatorMenu_DropDownBeard_1_K2Node_ComponentBoundEvent_15_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownAccessory1_K2Node_ComponentBoundEvent_16_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownAccessory2_K2Node_ComponentBoundEvent_17_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownAccessory3_K2Node_ComponentBoundEvent_18_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void BndEvt__CharacterCreatorMenu_DropDownOccupation_1_K2Node_ComponentBoundEvent_10_OnSelectionChangedEvent__DelegateSignature(FString SelectedItem, TEnumAsByte<ESelectInfo::Type> SelectionType);
    void ExecuteUbergraph_CharacterBarberMenu(int32 EntryPoint);
    void ExitMenu__DelegateSignature();
}; // Size: 0x390

#endif
