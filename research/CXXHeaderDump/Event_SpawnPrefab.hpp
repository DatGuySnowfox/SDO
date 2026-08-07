#ifndef UE4SS_SDK_Event_SpawnPrefab_HPP
#define UE4SS_SDK_Event_SpawnPrefab_HPP

class UEvent_SpawnPrefab_C : public UNarrativeEvent
{
    TSubclassOf<class APrefabMaster_C> Prefab;                                        // 0x0030 (size: 0x8)
    FTransform Transform;                                                             // 0x0040 (size: 0x60)
    FName Tag;                                                                        // 0x00A0 (size: 0x8)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0xA8

#endif
