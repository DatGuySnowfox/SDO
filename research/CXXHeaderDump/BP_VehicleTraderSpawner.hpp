#ifndef UE4SS_SDK_BP_VehicleTraderSpawner_HPP
#define UE4SS_SDK_BP_VehicleTraderSpawner_HPP

class ABP_VehicleTraderSpawner_C : public ABP_MasterObject_C
{
    class UArrowComponent* Arrow;                                                     // 0x02A0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A8 (size: 0x8)

    void SpawnVehicle(TSubclassOf<class AActor> Class);
}; // Size: 0x2B0

#endif
