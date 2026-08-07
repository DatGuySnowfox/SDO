#ifndef UE4SS_SDK_InterchangeMessages_HPP
#define UE4SS_SDK_InterchangeMessages_HPP

class UInterchangeResultMeshError : public UInterchangeResultError
{
    FString MeshName;                                                                 // 0x0060 (size: 0x10)

}; // Size: 0x70

class UInterchangeResultMeshError_Generic : public UInterchangeResultMeshError
{
    FText Text;                                                                       // 0x0070 (size: 0x18)

}; // Size: 0x88

class UInterchangeResultMeshWarning : public UInterchangeResultWarning
{
    FString MeshName;                                                                 // 0x0060 (size: 0x10)

}; // Size: 0x70

class UInterchangeResultMeshWarning_Generic : public UInterchangeResultMeshWarning
{
    FText Text;                                                                       // 0x0070 (size: 0x18)

}; // Size: 0x88

class UInterchangeResultMeshWarning_TooManyUVs : public UInterchangeResultMeshWarning
{
    int32 ExcessUVs;                                                                  // 0x0070 (size: 0x4)

}; // Size: 0x78

class UInterchangeResultTextureWarning : public UInterchangeResultWarning
{
    FString TextureName;                                                              // 0x0060 (size: 0x10)

}; // Size: 0x70

class UInterchangeResultTextureWarning_TextureFileDoNotExist : public UInterchangeResultTextureWarning
{
    FText Text;                                                                       // 0x0070 (size: 0x18)
    FString MaterialName;                                                             // 0x0088 (size: 0x10)

}; // Size: 0x98

#endif
