#ifndef UE4SS_SDK_BP_Keycard_Spawner_HPP
#define UE4SS_SDK_BP_Keycard_Spawner_HPP

class ABP_Keycard_Spawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh1;                                          // 0x02C0 (size: 0x8)
    class UBillboardComponent* Billboard1;                                            // 0x02C8 (size: 0x8)
    class UBillboardComponent* Billboard0;                                            // 0x02D0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02D8 (size: 0x8)
    class USceneComponent* Spawn1;                                                    // 0x02E0 (size: 0x8)
    class USceneComponent* Spawn0;                                                    // 0x02E8 (size: 0x8)
    TArray<USceneComponent*> Spawns;                                                  // 0x02F0 (size: 0x10)
    TArray<FTransform> SpawnTransforms;                                               // 0x0300 (size: 0x10)
    TArray<UStaticMeshComponent*> Tests;                                              // 0x0310 (size: 0x10)

    void TestSpawns();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_Keycard_Spawner(int32 EntryPoint);
}; // Size: 0x320

#endif
