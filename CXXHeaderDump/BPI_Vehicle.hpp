#ifndef UE4SS_SDK_BPI_Vehicle_HPP
#define UE4SS_SDK_BPI_Vehicle_HPP

class IBPI_Vehicle_C : public IInterface
{

    void GetVehicleMesh(class USkeletalMeshComponent*& SkelMesh);
    void OnEndStorageInteract(class AActor* Actor);
    void OnBeginStorageInteract(class AActor* Actor);
    void OnEndFuelInteract(class AActor* Actor);
    void OnBeginFuelInteract(class AActor* Actor);
    void OnEndRepairInteract(class AActor* Actor);
    void OnBeginRepairInteract(class AActor* Actor);
}; // Size: 0x28

#endif
