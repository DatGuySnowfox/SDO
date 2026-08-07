#ifndef UE4SS_SDK_BPI_RandomEvent_HPP
#define UE4SS_SDK_BPI_RandomEvent_HPP

class IBPI_RandomEvent_C : public IInterface
{

    void IsDynamicEvent?(bool& Dynamic?);
    void IsEventSpawned?(bool& Spawned?);
    void SpawnRandomEvent();
}; // Size: 0x28

#endif
