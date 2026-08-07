#ifndef UE4SS_SDK_S_ReplicatedContainerInfo_HPP
#define UE4SS_SDK_S_ReplicatedContainerInfo_HPP

struct FS_ReplicatedContainerInfo
{
    FGuid ReplicationUID_23_EB27D09649B6E02591DCA98708B06332;                         // 0x0000 (size: 0x10)
    FGuid InContainerUID_24_D1504ED7438699C73EA046AD734E79BF;                         // 0x0010 (size: 0x10)
    int32 Columns_4_1D84448048078D988999ABA138CE1809;                                 // 0x0020 (size: 0x4)
    int32 Rows_6_78B4BC5B449017D63758E79844375AE9;                                    // 0x0024 (size: 0x4)
    int32 ContainerIndex_22_9A211B86466A9D1C57E1FC839A5B0896;                         // 0x0028 (size: 0x4)
    TArray<FS_ContainerSlots> ContainerSlots_13_97B5C4324E23EC5C1D899EA59CCDD3AC;     // 0x0030 (size: 0x10)
    TArray<FContainerPickupsInfo> ContainerItems_16_16E3D87B4C0AC5D6C9C861B96B5ABA8B; // 0x0040 (size: 0x10)

}; // Size: 0x50

#endif
