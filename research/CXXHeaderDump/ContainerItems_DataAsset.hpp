#ifndef UE4SS_SDK_ContainerItems_DataAsset_HPP
#define UE4SS_SDK_ContainerItems_DataAsset_HPP

class UContainerItems_DataAsset_C : public UPrimaryDataAsset
{
    FName ID;                                                                         // 0x0030 (size: 0x8)
    FStruct_ContainerLootTable Loot Table;                                            // 0x0038 (size: 0x10)

}; // Size: 0x48

#endif
