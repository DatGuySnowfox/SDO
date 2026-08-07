#ifndef UE4SS_SDK_BPI_HarvestableObject_HPP
#define UE4SS_SDK_BPI_HarvestableObject_HPP

class IBPI_HarvestableObject_C : public IInterface
{

    void HitHarvestableObject(FVector HitLocation, int32 Hits);
}; // Size: 0x28

#endif
