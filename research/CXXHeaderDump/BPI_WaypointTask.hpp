#ifndef UE4SS_SDK_BPI_WaypointTask_HPP
#define UE4SS_SDK_BPI_WaypointTask_HPP

class IBPI_WaypointTask_C : public IInterface
{

    void GetTaskLocationName(FText& Name);
    void GetTaskLocation(FVector& Location);
}; // Size: 0x28

#endif
