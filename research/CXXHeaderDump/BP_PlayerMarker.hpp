#ifndef UE4SS_SDK_BP_PlayerMarker_HPP
#define UE4SS_SDK_BP_PlayerMarker_HPP

class ABP_PlayerMarker_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A8 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02B0 (size: 0x8)
    FS_MarkerData Marker Data;                                                        // 0x02B8 (size: 0x68)
    int32 Index;                                                                      // 0x0320 (size: 0x4)
    class ABP_CompassMarker_C* CompassMarker;                                         // 0x0328 (size: 0x8)
    class UW_PlayerMarker_C* Marker;                                                  // 0x0330 (size: 0x8)

    void ComponentsToSave(TArray<UActorComponent*>& Components);
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
