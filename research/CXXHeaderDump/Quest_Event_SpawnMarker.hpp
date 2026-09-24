#ifndef UE4SS_SDK_Quest_Event_SpawnMarker_HPP
#define UE4SS_SDK_Quest_Event_SpawnMarker_HPP

class UQuest_Event_SpawnMarker_C : public UNarrativeEvent
{
    FVector Location;                                                                 // 0x0040 (size: 0x18)
    class UTexture2D* Marker Texture;                                                 // 0x0058 (size: 0x8)
    FString Text;                                                                     // 0x0060 (size: 0x10)
    FName ActorTag;                                                                   // 0x0070 (size: 0x8)
    FLinearColor Color;                                                               // 0x0078 (size: 0x10)
    bool Underground?;                                                                // 0x0088 (size: 0x1)

    void ExecuteEvent(class APawn* Target, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x89

#endif
