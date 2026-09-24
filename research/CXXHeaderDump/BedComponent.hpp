#ifndef UE4SS_SDK_BedComponent_HPP
#define UE4SS_SDK_BedComponent_HPP

class UBedComponent_C : public UBaseComponent_C
{

    void GoToBed(double SleepTime, double WakeupTime);
    void SetSpawnPoint(FTransform RespawnLoc);
}; // Size: 0xD0

#endif
