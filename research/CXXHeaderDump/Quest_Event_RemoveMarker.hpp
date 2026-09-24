#ifndef UE4SS_SDK_Quest_Event_RemoveMarker_HPP
#define UE4SS_SDK_Quest_Event_RemoveMarker_HPP

class UQuest_Event_RemoveMarker_C : public UNarrativeEvent
{
    FName ActorTag;                                                                   // 0x0040 (size: 0x8)

    void ExecuteEvent(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x48

#endif
