#ifndef UE4SS_SDK_BPI_LootContainers_HPP
#define UE4SS_SDK_BPI_LootContainers_HPP

class IBPI_LootContainers_C : public IInterface
{

    void IsHeld?(bool& Held?);
    void GetSettings(double& ChanceToProduceNoise, double& Default_ChanceToProduceNoise, double& DurationToHold);
}; // Size: 0x28

#endif
