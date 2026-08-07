#ifndef UE4SS_SDK_UDS_Cloud_Paint_Cell_HPP
#define UE4SS_SDK_UDS_Cloud_Paint_Cell_HPP

class UUDS_Cloud_Paint_Cell_C : public UPrimaryDataAsset
{
    TArray<FLinearColor> Painting Array;                                              // 0x0030 (size: 0x10)
    TSoftObjectPtr<UTexture2D> Texture;                                               // 0x0040 (size: 0x28)
    bool Zero Coverage Present;                                                       // 0x0068 (size: 0x1)
    bool Mid Coverage Present;                                                        // 0x0069 (size: 0x1)
    bool Full Coverage Present;                                                       // 0x006A (size: 0x1)

}; // Size: 0x6B

#endif
