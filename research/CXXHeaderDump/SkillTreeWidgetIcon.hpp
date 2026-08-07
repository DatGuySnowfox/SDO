#ifndef UE4SS_SDK_SkillTreeWidgetIcon_HPP
#define UE4SS_SDK_SkillTreeWidgetIcon_HPP

class USkillTreeWidgetIcon_C : public UTechNodeWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02C8 (size: 0x8)
    class UImage* SlotImage;                                                          // 0x02D0 (size: 0x8)
    class UButton* UnlockButton;                                                      // 0x02D8 (size: 0x8)
    class UJournal_SkillTreeTooltip_C* Tooltip;                                       // 0x02E0 (size: 0x8)
    class UBP_SkillTreeAsset_C* SkillTreeAsset;                                       // 0x02E8 (size: 0x8)

    class UWidget* SkillTreeTooltip();
    void UpdateUnlocked(bool Unlocked);
    void OnTechNodeInitialized();
    void OnTechnologyUnlockStateChanged(class UTTTechNode* TechNode, bool IsUnlocked);
    void BndEvt__SkillTreeWidget_UnlockButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_SkillTreeWidgetIcon(int32 EntryPoint);
}; // Size: 0x2F0

#endif
