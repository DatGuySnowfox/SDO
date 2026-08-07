#ifndef UE4SS_SDK_BPC_MinimapSystem_HPP
#define UE4SS_SDK_BPC_MinimapSystem_HPP

class UBPC_MinimapSystem_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class ABP_PlayerMarker_C* PlayerMarker;                                           // 0x00A8 (size: 0x8)
    bool MapToggled;                                                                  // 0x00B0 (size: 0x1)
    TArray<FS_MarkerData> Markers;                                                    // 0x00B8 (size: 0x10)
    FS_MarkerData MarkerData;                                                         // 0x00C8 (size: 0x70)
    class UMapWidget_C* MapWidget;                                                    // 0x0138 (size: 0x8)

    void Remove World Marker(class UW_WorldMarker_C* Marker, class ABP_Marker_C* Actor);
    void Add World Marker(FS_MarkerData Marker Data, class AActor* Target Actor, FText Description, class UW_WorldMarker_C*& Markers);
    void Add POI Marker(FS_MarkerData Marker Data, class AActor* Target Actor, FStruct_POI POI Data, bool Explored?, class UW_POIMarker_C*& Markers);
    void Remove Quest Marker(class UW_QuestMarker_C* Marker, class ABP_QuestMarker_C* Actor);
    void Add Saved Player Marker(FS_MarkerData Marker Data, class ABP_PlayerMarker_C* Target Actor, int32 Index, TArray<class UW_PlayerMarker_C*>& Markers);
    void Remove Player Marker(class UW_PlayerMarker_C* Marker, int32 Index, class ABP_PlayerMarker_C* Actor);
    void Add Player Marker(FS_MarkerData Marker Data, class ABP_PlayerMarker_C* Target Actor, TArray<class UW_PlayerMarker_C*>& Markers);
    void Add Quest Marker(FS_MarkerData Marker Data, class ABP_QuestMarker_C* Target Actor, TArray<class UW_QuestMarker_C*>& Markers);
    void Remove Standard Marker(class UW_StandardMarker_C* Marker);
    void Marker Location(FVector2D Map Location, FVector& ImpactPoint, bool& Success);
    void Add Standard Marker(FS_MarkerData Marker Data, class AActor* Target Actor, TArray<class UW_StandardMarker_C*>& Markers);
    void Toggle Bigmap();
    void SERVER Spawn Player Marker(FVector Hit Location, bool Success);
    void Event_ConstructMap();
    void Create Checkpoint Marker(FVector2D Map Location);
    void ExecuteUbergraph_BPC_MinimapSystem(int32 EntryPoint);
}; // Size: 0x140

#endif
