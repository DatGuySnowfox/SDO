#ifndef UE4SS_SDK_FishingComponent_HPP
#define UE4SS_SDK_FishingComponent_HPP

class UFishingComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)
    double ChanceToCatchFish;                                                         // 0x00D8 (size: 0x8)
    bool CurrentlyFishing?;                                                           // 0x00E0 (size: 0x1)
    FTimerHandle FishingTimer;                                                        // 0x00E8 (size: 0x8)
    bool AbleToCatchFish?;                                                            // 0x00F0 (size: 0x1)
    class UCableComponent* CableComp;                                                 // 0x00F8 (size: 0x8)
    class AActor* FishingRodActor;                                                    // 0x0100 (size: 0x8)
    class ABP_FishingBuoy_C* FishingLocation;                                         // 0x0108 (size: 0x8)
    FFishingComponent_CDestroyCable DestroyCable;                                     // 0x0110 (size: 0x10)
    void DestroyCable();
    class AActor* WaterActor;                                                         // 0x0120 (size: 0x8)
    double MinTimeForFish;                                                            // 0x0128 (size: 0x8)
    double MaxTimeForFish;                                                            // 0x0130 (size: 0x8)

    void CheckDurability(bool& bool);
    void ReduceDurability();
    void RandomChance(double Chance, bool& Fish?);
    void Svr_Initialise();
    void Event_FishingBehaviour();
    void Event_Fish?();
    void MC_CatchFish();
    void Svr_CatchFish();
    void MC_CastRod();
    void Svr_CastRod();
    void MC_StopFishing(bool PlayAnim?, bool Catch?);
    void Svr_StopFishing(bool PlayAnim?, bool Catch?);
    void ExecuteUbergraph_FishingComponent(int32 EntryPoint);
    void DestroyCable__DelegateSignature();
}; // Size: 0x138

#endif
