#ifndef UE4SS_SDK_BP_MasterWaterActor_HPP
#define UE4SS_SDK_BP_MasterWaterActor_HPP

class ABP_MasterWaterActor_C : public ABP_MasterObject_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02B0 (size: 0x8)
    TArray<FStruct_FishingLoot> FishingTable;                                         // 0x02B8 (size: 0x10)
    FName FishingDataTableName;                                                       // 0x02C8 (size: 0x8)
    class UMaterialInterface* Material;                                               // 0x02D0 (size: 0x8)
    bool ShallowWater?;                                                               // 0x02D8 (size: 0x1)
    FVector2D Start Scale;                                                            // 0x02E0 (size: 0x10)
    FVector2D End Scale;                                                              // 0x02F0 (size: 0x10)
    FVector Scale;                                                                    // 0x0300 (size: 0x18)
    bool Radiated?;                                                                   // 0x0318 (size: 0x1)
    double TotalWeight;                                                               // 0x0320 (size: 0x8)
    class UStaticMesh* PlaneMesh;                                                     // 0x0328 (size: 0x8)

    void CanFishHere?(bool& Can?);
    void GetFishDataAssetToSpawn(FStruct_FishingLoot& FishToSpawn);
    void SetCollisionType();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_BP_MasterWaterActor(int32 EntryPoint);
}; // Size: 0x330

#endif
