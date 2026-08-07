#ifndef UE4SS_SDK_Event_SpawnActor_HPP
#define UE4SS_SDK_Event_SpawnActor_HPP

class UEvent_SpawnActor_C : public UNarrativeEvent
{
    TSubclassOf<class AActor> Actor;                                                  // 0x0030 (size: 0x8)
    FTransform Spawn Transform;                                                       // 0x0040 (size: 0x60)
    FName Tag;                                                                        // 0x00A0 (size: 0x8)
    FString QuestArgument;                                                            // 0x00A8 (size: 0x10)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0xB8

#endif
