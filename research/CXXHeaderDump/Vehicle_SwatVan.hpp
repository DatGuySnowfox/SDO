#ifndef UE4SS_SDK_Vehicle_SwatVan_HPP
#define UE4SS_SDK_Vehicle_SwatVan_HPP

class AVehicle_SwatVan_C : public ABP_VehicleMaster_C
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0530 (size: 0x8)

    void UserConstructionScript();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Vehicle_SwatVan(int32 EntryPoint);
}; // Size: 0x538

#endif
