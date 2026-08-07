#ifndef UE4SS_SDK_Struct_POI_HPP
#define UE4SS_SDK_Struct_POI_HPP

struct FStruct_POI
{
    bool IsAMajorPOI?_11_A87DDB26439CC030B3170C9A49D4FDEB;                            // 0x0000 (size: 0x1)
    FString Name_2_5D639B8747C652C2C1E4E2A0F1D71410;                                  // 0x0008 (size: 0x10)
    FString Description_4_3F2251FE44FE0E96F2B4A69E5DFE074C;                           // 0x0018 (size: 0x10)
    int32 ContaminationLevel_35_CC8B8826490E5D703FB398B2BA0588C2;                     // 0x0028 (size: 0x4)
    TArray<FRandomContainerItem> ItemsToSpawn_13_3D0E9296494719799691DA960524296E;    // 0x0030 (size: 0x10)
    FString ItemsToSpawnDescription_26_A8822EE742C4F5B6785B8388EC878B6E;              // 0x0040 (size: 0x10)
    class UTexture2D* MarkerImage_24_1FA3233641EF2AE38BCE6F8B3382A03C;                // 0x0050 (size: 0x8)
    FLinearColor MarkerColour_23_F4B0472F460D4AC7319A7B963C0F9284;                    // 0x0058 (size: 0x10)
    bool ForceLargeIcon_31_61D6CE59432B0ECEB011B6ABD456E711;                          // 0x0068 (size: 0x1)

}; // Size: 0x69

#endif
