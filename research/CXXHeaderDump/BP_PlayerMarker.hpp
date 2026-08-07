#ifndef UE4SS_SDK_BP_PlayerMarker_HPP
#define UE4SS_SDK_BP_PlayerMarker_HPP

class ABP_PlayerMarker_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A0 (size: 0x8)
    TArray<class UW_PlayerMarker_C*> Markers;                                         // 0x02A8 (size: 0x10)
    FS_MarkerData Marker Data;                                                        // 0x02B8 (size: 0x70)
    int32 Index;                                                                      // 0x0328 (size: 0x4)
    class ABP_CompassMarker_C* CompassMarker;                                         // 0x0330 (size: 0x8)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void ActorPreLoad();
    void ActorPreSave();
    void ActorSaved();
    void Event_AddMarker();
    void ActorLoaded();
    void ReceiveDestroyed();
    void RespawnMarker();
    void ExecuteUbergraph_BP_PlayerMarker(int32 EntryPoint);
}; // Size: 0x338

#endif
