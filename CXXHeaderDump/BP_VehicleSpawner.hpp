#ifndef UE4SS_SDK_BP_VehicleSpawner_HPP
#define UE4SS_SDK_BP_VehicleSpawner_HPP

class ABP_VehicleSpawner_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A0 (size: 0x8)
    class USceneComponent* DefaultSceneRoot;                                          // 0x02A8 (size: 0x8)
    bool SpawnerUsed?;                                                                // 0x02B0 (size: 0x1)
    TEnumAsByte<Enum_VehicleSpawner::Type> VehicleSelection;                          // 0x02B1 (size: 0x1)

    void ComponentsToSave(TArray<class UActorComponent*>& Components);
    bool RandomChance();
    void SpawnSelectVehicles(TSubclassOf<class ABP_VehicleMaster_C>& Vehicle);
    void OnCompleted_943CBE6A4C95C1F090F71487FED92EDC();
    void ActorLoaded();
    void ActorPreSave();
    void ActorSaved();
    void ReceiveBeginPlay();
    void Svr_Spawn();
    void ActorPreLoad();
    void ExecuteUbergraph_BP_VehicleSpawner(int32 EntryPoint);
}; // Size: 0x2B2

#endif
