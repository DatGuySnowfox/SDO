#ifndef UE4SS_SDK_BP_EventManager_HPP
#define UE4SS_SDK_BP_EventManager_HPP

class ABP_EventManager_C : public ABP_EventMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    int32 MaxNumberOfJetsToSpawn;                                                     // 0x02A8 (size: 0x4)
    double JetTimeToSpawn;                                                            // 0x02B0 (size: 0x8)
    double DistanceToSpawnAirdrop;                                                    // 0x02B8 (size: 0x8)
    double AirdropTimeToSpawn;                                                        // 0x02C0 (size: 0x8)
    FTimerHandle AirdropTimer;                                                        // 0x02C8 (size: 0x8)
    double ExplosionDelay;                                                            // 0x02D0 (size: 0x8)
    FTimerHandle ExplosionTimer;                                                      // 0x02D8 (size: 0x8)
    FTimerHandle RandomEventTimer;                                                    // 0x02E0 (size: 0x8)
    int32 RandomEventSpawnRetries;                                                    // 0x02E8 (size: 0x4)
    double BloodMoonChance;                                                           // 0x02F0 (size: 0x8)
    FBP_EventManager_CBloodMoon BloodMoon;                                            // 0x02F8 (size: 0x10)
    void BloodMoon(bool Start?);
    TArray<class ABP_PrefabSpawner_Dynamic_C*> DynamicEvents;                         // 0x0308 (size: 0x10)
    int32 CurrentEventSpawned;                                                        // 0x0318 (size: 0x4)
    int32 ClampEventSpawnTries;                                                       // 0x031C (size: 0x4)
    double TimeUntilDynamicEvent;                                                     // 0x0320 (size: 0x8)

    void GetDaysSurvived(int32& DaysSurvived);
    void SetBloodMoon(bool BloodMoon?);
    void ReceiveBeginPlay();
    void SpawnJet();
    void Jet_Spawn();
    void SpawnAirdrop();
    void AirdropSpawn();
    void SpawnExplosion();
    void Explosion_Spawn();
    void SpawnRandomEvent();
    void DynamicRandomEventSpawn();
    void BloodMoonEvent();
    void Event_StopBloodMoonSunrise();
    void Event_StartBloodMoon();
    void ExecuteUbergraph_BP_EventManager(int32 EntryPoint);
    void BloodMoon__DelegateSignature(bool Start?);
}; // Size: 0x328

#endif
