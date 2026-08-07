#ifndef UE4SS_SDK_Event_RemoveMarker_HPP
#define UE4SS_SDK_Event_RemoveMarker_HPP

class UEvent_RemoveMarker_C : public UNarrativeEvent
{
    FName ActorTag;                                                                   // 0x0030 (size: 0x8)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x38

#endif
