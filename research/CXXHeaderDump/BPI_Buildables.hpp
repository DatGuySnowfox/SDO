#ifndef UE4SS_SDK_BPI_Buildables_HPP
#define UE4SS_SDK_BPI_Buildables_HPP

class IBPI_Buildables_C : public IInterface
{

    void OverrideBuildName(FText Name);
    void GetBuildType(TEnumAsByte<Enum_BuildingSnapTypes::Type>& Type);
    void RemoveModularBuilds();
    void Requires Power?(bool& Required?);
    void OnDestroy(class AActor* Actor);
    void OnInteractBuildable(class AActor* Actor);
    void Power Off(class AActor* Actor);
    void Power On(class AActor* Actor);
}; // Size: 0x28

#endif
