#ifndef UE4SS_SDK_Vehicle_Humvee_HPP
#define UE4SS_SDK_Vehicle_Humvee_HPP

class AVehicle_Humvee_C : public ABP_VehicleMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0530 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Vehicle_Humvee(int32 EntryPoint);
}; // Size: 0x538

#endif
