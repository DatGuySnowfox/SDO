#ifndef UE4SS_SDK_UDW_Lightning_Spawn_Manager_HPP
#define UE4SS_SDK_UDW_Lightning_Spawn_Manager_HPP

class UUDW_Lightning_Spawn_Manager_C : public UActorComponent
{
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x00A0 (size: 0x8)
    FTimerHandle Lightning Timer;                                                     // 0x00A8 (size: 0x8)
    FTimerHandle Check Timer;                                                         // 0x00B0 (size: 0x8)

    void Disable Spawning();
    void Timed Lightning Spawn();
    void Initialize(class AUltra_Dynamic_Weather_C* UDW);
}; // Size: 0xB8

#endif
