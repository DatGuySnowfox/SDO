#ifndef UE4SS_SDK_PrefabMaster_HPP
#define UE4SS_SDK_PrefabMaster_HPP

class APrefabMaster_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class USphereComponent* FoliageRemover;                                           // 0x02A8 (size: 0x8)
    class USceneComponent* AI;                                                        // 0x02B0 (size: 0x8)
    class USceneComponent* Actors;                                                    // 0x02B8 (size: 0x8)
    class USceneComponent* StaticMeshes;                                              // 0x02C0 (size: 0x8)
    float FoliageRemoverRadius;                                                       // 0x02C8 (size: 0x4)

    void RemoveFoliageComp();
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_PrefabMaster(int32 EntryPoint);
}; // Size: 0x2CC

#endif
