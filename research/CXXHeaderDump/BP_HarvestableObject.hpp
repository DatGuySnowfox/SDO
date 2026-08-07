#ifndef UE4SS_SDK_BP_HarvestableObject_HPP
#define UE4SS_SDK_BP_HarvestableObject_HPP

class ABP_HarvestableObject_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class UAIOSubjectComponent* AIOSubject;                                           // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* StaticMesh;                                           // 0x02B0 (size: 0x8)
    int32 MaxHits;                                                                    // 0x02B8 (size: 0x4)
    class USoundBase* Sound;                                                          // 0x02C0 (size: 0x8)
    class UStaticMesh* Mesh;                                                          // 0x02C8 (size: 0x8)
    class UMaterialInterface* Material;                                               // 0x02D0 (size: 0x8)

    void HitHarvestableObject(FVector HitLocation, int32 Hits);
    void BndEvt__BP_MPLootContainer_AIOSubject_K2Node_ComponentBoundEvent_0_OnOptimizationUpdate__DelegateSignature(bool bIsBeyondLastLayer, int32 LayerIndex, bool bIsSeen);
    void ExecuteUbergraph_BP_HarvestableObject(int32 EntryPoint);
}; // Size: 0x2D8

#endif
