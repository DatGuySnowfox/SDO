#ifndef UE4SS_SDK_Prefab_AbandonedMilitarySuppliesTruck_HPP
#define UE4SS_SDK_Prefab_AbandonedMilitarySuppliesTruck_HPP

class APrefab_AbandonedMilitarySuppliesTruck_C : public APrefabMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02D0 (size: 0x8)
    class UChildActorComponent* ChildActor;                                           // 0x02D8 (size: 0x8)
    class UChildActorComponent* ChildActor3;                                          // 0x02E0 (size: 0x8)
    class UParticleSystemComponent* Smoke;                                            // 0x02E8 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_Prefab_AbandonedMilitarySuppliesTruck(int32 EntryPoint);
}; // Size: 0x2F0

#endif
