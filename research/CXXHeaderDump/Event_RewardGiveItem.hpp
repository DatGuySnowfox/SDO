#ifndef UE4SS_SDK_Event_RewardGiveItem_HPP
#define UE4SS_SDK_Event_RewardGiveItem_HPP

class UEvent_RewardGiveItem_C : public UNarrativeEvent
{
    bool RandomSetOfItems?;                                                           // 0x0030 (size: 0x1)
    TArray<FS_RewardGiveItem> ItemSet1;                                               // 0x0038 (size: 0x10)
    TArray<FS_RewardGiveItem> ItemSet2;                                               // 0x0048 (size: 0x10)
    TArray<FS_RewardGiveItem> ItemSet3;                                               // 0x0058 (size: 0x10)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x68

#endif
