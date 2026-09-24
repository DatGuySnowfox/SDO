#ifndef UE4SS_SDK_GeometryFlowCore_HPP
#define UE4SS_SDK_GeometryFlowCore_HPP

struct FDoubleSetting
{
    double Value;                                                                     // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FFloatSetting
{
    float Value;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FInt32Setting
{
    int32 Value;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FNameSetting
{
    FName Value;                                                                      // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FVector3dSetting
{
    FVector3d Value;                                                                  // 0x0000 (size: 0x18)

}; // Size: 0x18

struct FVector3fSetting
{
    FVector3f Value;                                                                  // 0x0000 (size: 0xC)

}; // Size: 0xC

#endif
