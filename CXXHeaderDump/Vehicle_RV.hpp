#ifndef UE4SS_SDK_Vehicle_RV_HPP
#define UE4SS_SDK_Vehicle_RV_HPP

class AVehicle_RV_C : public ABP_VehicleMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0530 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void SleepInteractionOption(class AActor* InteractingActor);
    void ExecuteUbergraph_Vehicle_RV(int32 EntryPoint);
}; // Size: 0x538

#endif
