#ifndef UE4SS_SDK_BP_LaptopEventMaster_HPP
#define UE4SS_SDK_BP_LaptopEventMaster_HPP

class ABP_LaptopEventMaster_C : public ABP_MasterQuestObject_C
{
    FBP_LaptopEventMaster_CSpawnEvent SpawnEvent;                                     // 0x02C0 (size: 0x10)
    void SpawnEvent();

    void SpawnEvent__DelegateSignature();
}; // Size: 0x2D0

#endif
