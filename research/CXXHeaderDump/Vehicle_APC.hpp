#ifndef UE4SS_SDK_Vehicle_APC_HPP
#define UE4SS_SDK_Vehicle_APC_HPP

class AVehicle_APC_C : public ABP_VehicleMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0538 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Vehicle_APC(int32 EntryPoint);
}; // Size: 0x540

#endif
