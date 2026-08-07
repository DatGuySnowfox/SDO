#ifndef UE4SS_SDK_BP_RandomInfestationSpawnPoint_HPP
#define UE4SS_SDK_BP_RandomInfestationSpawnPoint_HPP

class ABP_RandomInfestationSpawnPoint_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02A8 (size: 0x8)
    double RandomChance;                                                              // 0x02B0 (size: 0x8)
    class UStaticMesh* NormalMesh;                                                    // 0x02B8 (size: 0x8)
    class UStaticMesh* DestroyedMesh;                                                 // 0x02C0 (size: 0x8)
    FBP_RandomInfestationSpawnPoint_CSpawn Spawn;                                     // 0x02C8 (size: 0x10)
    void Spawn();
    TArray<FTransform> AISpawningTransforms;                                          // 0x02D8 (size: 0x10)

    void TraceForActor(bool& Ouput);
    bool RandomSpawn();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void SpawnInfestation();
    void ExecuteUbergraph_BP_RandomInfestationSpawnPoint(int32 EntryPoint);
    void Spawn__DelegateSignature();
}; // Size: 0x2E8

#endif
