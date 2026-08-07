#ifndef UE4SS_SDK_BPI_FishingRod_HPP
#define UE4SS_SDK_BPI_FishingRod_HPP

class IBPI_FishingRod_C : public IInterface
{

    void StartFishing();
    void StopFishing(bool Caught A Fish?);
    void CastRod();
    void CatchFish();
}; // Size: 0x28

#endif
