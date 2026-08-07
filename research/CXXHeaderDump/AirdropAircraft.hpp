#ifndef UE4SS_SDK_AirdropAircraft_HPP
#define UE4SS_SDK_AirdropAircraft_HPP

class AAirdropAircraft_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UPointLightComponent* LightBackWhite;                                       // 0x02A8 (size: 0x8)
    class UPointLightComponent* LightRightGreen;                                      // 0x02B0 (size: 0x8)
    class UPointLightComponent* LightLeftRed;                                         // 0x02B8 (size: 0x8)
    class UAudioComponent* Audio;                                                     // 0x02C0 (size: 0x8)
    class UArrowComponent* DirectionOfLineTracer;                                     // 0x02C8 (size: 0x8)
    class UArrowComponent* DirectionOfFlight;                                         // 0x02D0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02D8 (size: 0x8)
    TArray<class TSubclassOf<AActor>> Airdrops;                                       // 0x02E0 (size: 0x10)
    double Speed;                                                                     // 0x02F0 (size: 0x8)
    double RegionRadius;                                                              // 0x02F8 (size: 0x8)
    double FlightDuration;                                                            // 0x0300 (size: 0x8)
    int32 MaxAirdropsToSpawn;                                                         // 0x0308 (size: 0x4)
    bool DisplayDrawDebug?;                                                           // 0x030C (size: 0x1)
    FTimerHandle Timer_CheckForRegionCrossing;                                        // 0x0310 (size: 0x8)

    void ReceiveBeginPlay();
    void ReceiveTick(float DeltaSeconds);
    void CheckForRegionCrossing();
    void MC_Lights();
    void SpawnAirdrop();
    void ExecuteUbergraph_AirdropAircraft(int32 EntryPoint);
}; // Size: 0x318

#endif
