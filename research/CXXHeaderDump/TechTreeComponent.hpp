#ifndef UE4SS_SDK_TechTreeComponent_HPP
#define UE4SS_SDK_TechTreeComponent_HPP

class UTechTreeComponent_C : public UTechTreeManager
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0140 (size: 0x8)
    int32 SkillPoints;                                                                // 0x0148 (size: 0x4)
    int32 CurrentPoints;                                                              // 0x014C (size: 0x4)
    FTechTreeComponent_CUpdatePointsUI UpdatePointsUI;                                // 0x0150 (size: 0x10)
    void UpdatePointsUI(int32 Points);

    void ResetTreeComplete();
    void ResetTree();
    void Add Skill Points();
    void PayCost(int32 Cost);
    void HasEnoughResources(int32 TechnologyCost, bool& EnoughResources);
    void TryPayTechnologyCost(int32 Cost, bool& Success);
    void CheckUnlockCost(class UTechnologyAsset* Technology, bool& CanUnlock);
    void TryToUnlockTechnology(class UTechnologyAsset* Technology);
    void ComponentPreSave();
    void ComponentLoaded();
    void ExecuteUbergraph_TechTreeComponent(int32 EntryPoint);
    void UpdatePointsUI__DelegateSignature(int32 Points);
}; // Size: 0x160

#endif
