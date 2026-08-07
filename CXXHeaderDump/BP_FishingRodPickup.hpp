#ifndef UE4SS_SDK_BP_FishingRodPickup_HPP
#define UE4SS_SDK_BP_FishingRodPickup_HPP

class ABP_FishingRodPickup_C : public ABP_StaticMeshPickup_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0310 (size: 0x8)
    class UFishingRodPickupComponent_C* FishingRodPickupComponent;                    // 0x0318 (size: 0x8)
    double ChanceToCatch;                                                             // 0x0320 (size: 0x8)
    class UCableComponent* CableVar;                                                  // 0x0328 (size: 0x8)
    FTimerHandle FishingTimer;                                                        // 0x0330 (size: 0x8)
    class ABP_FishingBuoy_C* FishingLocation;                                         // 0x0338 (size: 0x8)

    void RandomChance(double Chance, bool& Fish?);
    void CastRod();
    void Event_FishingBehaviour();
    void Event_Fish?();
    void StartFishing();
    void CatchFish();
    void StopFishing(bool Caught A Fish?);
    void ExecuteUbergraph_BP_FishingRodPickup(int32 EntryPoint);
}; // Size: 0x340

#endif
