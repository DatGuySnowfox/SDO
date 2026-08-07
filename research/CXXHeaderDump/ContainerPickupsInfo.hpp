#ifndef UE4SS_SDK_ContainerPickupsInfo_HPP
#define UE4SS_SDK_ContainerPickupsInfo_HPP

struct FContainerPickupsInfo
{
    FGuid UniqueServerID_37_C49AE0DA449ACAE3BE4633A263303B68;                         // 0x0000 (size: 0x10)
    bool IsContainer_14_0198F62E4F1B93DD8B4D6E92B7E3EA45;                             // 0x0010 (size: 0x1)
    FVector2D ContainerDimension_28_232D13A9468B596AE3CE01B69CB32CD6;                 // 0x0018 (size: 0x10)
    FRepItemInfo ItemInfo_25_937A083B4BD3D9B590E0A69C76A4F6F7;                        // 0x0028 (size: 0x78)
    FGuid ContainerMotherID_38_33D59C5047C31FF0367835AAF5ABE05D;                      // 0x00A0 (size: 0x10)
    int32 SlotIndex_6_8ED1F34A4AF1D565616883B80192E3DD;                               // 0x00B0 (size: 0x4)
    bool Rotated_9_3CFE5D9342A81FB1FF17DEB7B491CE56;                                  // 0x00B4 (size: 0x1)
    int32 InContainerIndex_31_A61F5C3A47C8E761BA5CF0ADA1DF9FDE;                       // 0x00B8 (size: 0x4)
    class AActor* PickupRef_20_A3E97763428AC84AF4DBB8A56CFAC657;                      // 0x00C0 (size: 0x8)
    TArray<FS_SubContainerInfo> SubContainers_35_9B8F8C174A191BE2B5DAE19C1EB3FAAE;    // 0x00C8 (size: 0x10)

}; // Size: 0xD8

#endif
