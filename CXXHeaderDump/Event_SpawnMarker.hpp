#ifndef UE4SS_SDK_Event_SpawnMarker_HPP
#define UE4SS_SDK_Event_SpawnMarker_HPP

class UEvent_SpawnMarker_C : public UNarrativeEvent
{
    FVector Location;                                                                 // 0x0030 (size: 0x18)
    class UTexture2D* Marker Texture;                                                 // 0x0048 (size: 0x8)
    FString Text;                                                                     // 0x0050 (size: 0x10)
    FName ActorTag;                                                                   // 0x0060 (size: 0x8)
    FLinearColor Color;                                                               // 0x0068 (size: 0x10)
    bool Underground?;                                                                // 0x0078 (size: 0x1)

    void ExecuteEvent(class APawn* Pawn, class APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
}; // Size: 0x79

#endif
