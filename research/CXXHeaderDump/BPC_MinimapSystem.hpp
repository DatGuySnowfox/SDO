#ifndef UE4SS_SDK_BPC_MinimapSystem_HPP
#define UE4SS_SDK_BPC_MinimapSystem_HPP

class UBPC_MinimapSystem_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00B8 (size: 0x8)
    bool MapToggled;                                                                  // 0x00C0 (size: 0x1)
    class UMapWidget_C* MapWidget;                                                    // 0x00C8 (size: 0x8)
    FS_MarkerData DefaultPlayerMarkerData;                                            // 0x00D0 (size: 0x68)

    void Remove World Marker(class UW_WorldMarker_C* Marker, class ABP_Marker_C* Actor);
    void Add World Marker(FS_MarkerData Marker Data, class AActor* Target Actor, FText Description, class UW_WorldMarker_C*& Markers);
    void Add POI Marker(FS_MarkerData Marker Data, class AActor* Target Actor, FStruct_POI POI Data, bool Explored?, class UW_POIMarker_C*& Markers);
    void Remove Quest Marker(class UW_QuestMarker_C* Marker, class ABP_QuestMarker_C* Actor);
    void Add Saved Player Marker(FS_MarkerData Marker Data, class ABP_PlayerMarker_C* Target Actor, class UW_PlayerMarker_C*& Marker);
    void Remove Player Marker(class ABP_PlayerMarker_C* Actor);
    void Add Player Marker(FS_MarkerData Marker Data, class ABP_PlayerMarker_C* Target Actor, class UW_PlayerMarker_C*& Marker);
    void Add Quest Marker(FS_MarkerData Marker Data, class ABP_QuestMarker_C* Target Actor, class UW_QuestMarker_C*& Marker);
    void Remove Standard Marker(class UW_StandardMarker_C* Marker);
    void Marker Location(FVector2D Map Location, FVector& ImpactPoint, bool& Success);
    void Add Standard Marker(FS_MarkerData Marker Data, class AActor* Target Actor, class UW_StandardMarker_C*& Markers);
    void Toggle Bigmap();
    void Svr_SpawnPlayerMarker(FVector Hit Location, bool Success);
    void Event_ConstructMap();
    void CreatePlayerMarker(FVector2D Map Location);
    void ExecuteUbergraph_BPC_MinimapSystem(int32 EntryPoint);
}; // Size: 0x138

#endif
