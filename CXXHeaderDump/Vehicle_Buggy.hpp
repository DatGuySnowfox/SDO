#ifndef UE4SS_SDK_Vehicle_Buggy_HPP
#define UE4SS_SDK_Vehicle_Buggy_HPP

class AVehicle_Buggy_C : public ABP_VehicleMaster_C
{
    class USpotLightComponent* LightCentre1;                                          // 0x0530 (size: 0x8)
    class USpotLightComponent* LightCentre;                                           // 0x0538 (size: 0x8)
    class USpotLightComponent* LightLeft1;                                            // 0x0540 (size: 0x8)
    class USpotLightComponent* LightRight1;                                           // 0x0548 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x550

#endif
