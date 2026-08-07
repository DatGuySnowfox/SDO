#ifndef UE4SS_SDK_BP_ShakesConesSpawner_HPP
#define UE4SS_SDK_BP_ShakesConesSpawner_HPP

class ABP_ShakesConesSpawner_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh4;                                      // 0x02A8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh3;                                      // 0x02B0 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh2;                                      // 0x02B8 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh1;                                      // 0x02C0 (size: 0x8)
    class USkeletalMeshComponent* SkeletalMesh;                                       // 0x02C8 (size: 0x8)
    class UBillboardComponent* Billboard4;                                            // 0x02D0 (size: 0x8)
    class UBillboardComponent* Billboard3;                                            // 0x02D8 (size: 0x8)
    class UBillboardComponent* Billboard2;                                            // 0x02E0 (size: 0x8)
    class UBillboardComponent* Billboard1;                                            // 0x02E8 (size: 0x8)
    class UBillboardComponent* Billboard0;                                            // 0x02F0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02F8 (size: 0x8)
    class USceneComponent* Spawn4;                                                    // 0x0300 (size: 0x8)
    class USceneComponent* Spawn3;                                                    // 0x0308 (size: 0x8)
    class USceneComponent* Spawn2;                                                    // 0x0310 (size: 0x8)
    class USceneComponent* Spawn1;                                                    // 0x0318 (size: 0x8)
    class USceneComponent* Spawn0;                                                    // 0x0320 (size: 0x8)
    TArray<class USceneComponent*> Spawns;                                            // 0x0328 (size: 0x10)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_ShakesConesSpawner(int32 EntryPoint);
}; // Size: 0x338

#endif
