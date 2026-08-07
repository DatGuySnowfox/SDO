#ifndef UE4SS_SDK_BP_LaptopEventManager_HPP
#define UE4SS_SDK_BP_LaptopEventManager_HPP

class ABP_LaptopEventManager_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    FBP_LaptopEventManager_CRandomEvent RandomEvent;                                  // 0x02A8 (size: 0x10)
    void RandomEvent();
    TArray<class TSubclassOf<UQuest>> Quests;                                         // 0x02B8 (size: 0x10)
    FBP_LaptopEventManager_CSelectCertainLocation SelectCertainLocation;              // 0x02C8 (size: 0x10)
    void SelectCertainLocation(class AActor* Actor, class ABP_LaptopEventMaster_C* Event);

    void CallRandomEvent();
    void ReceiveBeginPlay();
    void Event_Random();
    void BeginQuest(const class UQuest* Quest);
    void ExecuteUbergraph_BP_LaptopEventManager(int32 EntryPoint);
    void SelectCertainLocation__DelegateSignature(class AActor* Actor, class ABP_LaptopEventMaster_C* Event);
    void RandomEvent__DelegateSignature();
}; // Size: 0x2D8

#endif
