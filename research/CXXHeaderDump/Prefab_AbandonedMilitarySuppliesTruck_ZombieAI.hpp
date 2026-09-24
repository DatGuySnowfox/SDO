#ifndef UE4SS_SDK_Prefab_AbandonedMilitarySuppliesTruck_ZombieAI_HPP
#define UE4SS_SDK_Prefab_AbandonedMilitarySuppliesTruck_ZombieAI_HPP

class APrefab_AbandonedMilitarySuppliesTruck_ZombieAI_C : public APrefabMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02E0 (size: 0x8)
    class UChildActorComponent* ChildActor3;                                          // 0x02E8 (size: 0x8)
    class UChildActorComponent* ChildActor;                                           // 0x02F0 (size: 0x8)
    class UParticleSystemComponent* Smoke;                                            // 0x02F8 (size: 0x8)

    void ReceiveBeginPlay();
    void ExecuteUbergraph_Prefab_AbandonedMilitarySuppliesTruck_ZombieAI(int32 EntryPoint);
}; // Size: 0x300

#endif
