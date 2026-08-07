#ifndef UE4SS_SDK_Vehicle_SUV_HPP
#define UE4SS_SDK_Vehicle_SUV_HPP

class AVehicle_SUV_C : public ABP_VehicleMaster_C
{
    class USpotLightComponent* LightRight1;                                           // 0x0530 (size: 0x8)
    class USpotLightComponent* LightLeft1;                                            // 0x0538 (size: 0x8)

    void UserConstructionScript();
}; // Size: 0x540

#endif
