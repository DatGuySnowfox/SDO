#ifndef UE4SS_SDK_BP_BountyQuestManager_HPP
#define UE4SS_SDK_BP_BountyQuestManager_HPP

class ABP_BountyQuestManager_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    FBP_BountyQuestManager_CRandomEvent RandomEvent;                                  // 0x02B8 (size: 0x10)
    void RandomEvent();
    TArray<TSubclassOf<class UQuest_Bounty_Master_C>> Quests;                         // 0x02C8 (size: 0x10)
    FBP_BountyQuestManager_CSelectCertainLocation SelectCertainLocation;              // 0x02D8 (size: 0x10)
    void SelectCertainLocation(class AActor* Actor, class USkeletalBodySetup_20* Event);

    void CallRandomEvent();
    void ReceiveBeginPlay();
    void Event_Random();
    void BeginQuest(const class UQuest* Quest);
    void ExecuteUbergraph_BP_BountyQuestManager(int32 EntryPoint);
    void SelectCertainLocation__DelegateSignature(class AActor* Actor, class USkeletalBodySetup_20* Event);
    void RandomEvent__DelegateSignature();
}; // Size: 0x2E8

#endif
