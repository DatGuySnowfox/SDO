#ifndef UE4SS_SDK_BP_VehicleUniqueSpawner_HPP
#define UE4SS_SDK_BP_VehicleUniqueSpawner_HPP

class ABP_VehicleUniqueSpawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A8 (size: 0x8)
    bool SpawnerUsed?;                                                                // 0x02B0 (size: 0x1)
    TSubclassOf<class ABP_VehicleMaster_C> Vehicle;                                   // 0x02B8 (size: 0x8)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    void OnCompleted_63EA519847229E38F2FAA4856B866960();
    void ActorLoaded();
    void ActorPreSave();
    void ActorSaved();
    void ReceiveBeginPlay();
    void Svr_Spawn();
    void ActorPreLoad();
    void ExecuteUbergraph_BP_VehicleUniqueSpawner(int32 EntryPoint);
}; // Size: 0x2C0

#endif
