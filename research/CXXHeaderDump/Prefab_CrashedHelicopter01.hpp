#ifndef UE4SS_SDK_Prefab_CrashedHelicopter01_HPP
#define UE4SS_SDK_Prefab_CrashedHelicopter01_HPP

class APrefab_CrashedHelicopter01_C : public APrefabMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UParticleSystemComponent* Fire;                                             // 0x02D8 (size: 0x8)
    class UParticleSystemComponent* Smoke;                                            // 0x02E0 (size: 0x8)
    class UChildActorComponent* ChildActor3;                                          // 0x02E8 (size: 0x8)
    class UChildActorComponent* ChildActor2;                                          // 0x02F0 (size: 0x8)
    class UChildActorComponent* ChildActor1;                                          // 0x02F8 (size: 0x8)
    class UChildActorComponent* ChildActor;                                           // 0x0300 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x0308 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_Prefab_CrashedHelicopter01(int32 EntryPoint);
}; // Size: 0x310

#endif
