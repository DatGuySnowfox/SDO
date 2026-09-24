#ifndef UE4SS_SDK_BP_BountyQuestMaster_HPP
#define UE4SS_SDK_BP_BountyQuestMaster_HPP

class ABP_BountyQuestMaster_C : public ABP_MasterQuestObject_C
{
    FBP_BountyQuestMaster_CSpawnEvent SpawnEvent;                                     // 0x02D0 (size: 0x10)
    void SpawnEvent();

    void SpawnEvent__DelegateSignature();
}; // Size: 0x2E0

#endif
