#ifndef UE4SS_SDK_BPI_Fishing_HPP
#define UE4SS_SDK_BPI_Fishing_HPP

class IBPI_Fishing_C : public IInterface
{

    void CanFishHere?(bool& Can?);
    void GetFishDataAssetToSpawn(FStruct_FishingLoot& FishToSpawn);
}; // Size: 0x28

#endif
