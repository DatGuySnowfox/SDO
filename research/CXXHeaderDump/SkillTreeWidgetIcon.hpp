#ifndef UE4SS_SDK_SkillTreeWidgetIcon_HPP
#define UE4SS_SDK_SkillTreeWidgetIcon_HPP

class USkillTreeWidgetIcon_C : public UTechNodeWidget
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0348 (size: 0x8)
    class UButton* UnlockButton;                                                      // 0x0350 (size: 0x8)
    class UImage* SlotImage;                                                          // 0x0358 (size: 0x8)
    class UJournal_SkillTreeTooltip_C* Tooltip;                                       // 0x0360 (size: 0x8)
    class UBP_SkillTreeAsset_C* SkillTreeAsset;                                       // 0x0368 (size: 0x8)

    class UWidget* SkillTreeTooltip();
    void UpdateUnlocked(bool Unlocked);
    void OnTechNodeInitialized();
    void OnTechnologyUnlockStateChanged(class UTTTechNode* TechNode, bool IsUnlocked);
    void BndEvt__SkillTreeWidget_UnlockButton_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature();
    void ExecuteUbergraph_SkillTreeWidgetIcon(int32 EntryPoint);
}; // Size: 0x370

#endif
