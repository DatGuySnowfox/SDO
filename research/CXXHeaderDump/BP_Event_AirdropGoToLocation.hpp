#ifndef UE4SS_SDK_BP_Event_AirdropGoToLocation_HPP
#define UE4SS_SDK_BP_Event_AirdropGoToLocation_HPP

class ABP_Event_AirdropGoToLocation_C : public ABP_EventMaster_C
{
    class USphereComponent* Sphere Region;                                            // 0x02B0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B8 (size: 0x8)
    double RegionRadius;                                                              // 0x02C0 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x2C8

#endif
