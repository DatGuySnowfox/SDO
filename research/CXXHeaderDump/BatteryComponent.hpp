#ifndef UE4SS_SDK_BatteryComponent_HPP
#define UE4SS_SDK_BatteryComponent_HPP

class UBatteryComponent_C : public UBaseComponent_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00D0 (size: 0x8)

    void CheckBatteryLevel(class UBP_JigComponent_C* JigComp, FGuid UID, bool& Battery?, double& Value);
    void UseBattery(class UBP_JigComponent_C* JigComp, FGuid UID, double MinusDur, double& NewDur, bool& Successful?);
    void Svr_Initialise();
    void ExecuteUbergraph_BatteryComponent(int32 EntryPoint);
}; // Size: 0xD8

#endif
