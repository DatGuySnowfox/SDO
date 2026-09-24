#ifndef UE4SS_SDK_StateStream_HPP
#define UE4SS_SDK_StateStream_HPP

struct FStateStreamHandle
{
}; // Size: 0x1

struct FTransformDynamicState
{
    FTransform LocalTransform;                                                        // 0x0010 (size: 0x60)
    TArray<FTransform> BoneTransforms;                                                // 0x0070 (size: 0x10)
    FTransformHandle Parent;                                                          // 0x0080 (size: 0x10)
    bool bVisible;                                                                    // 0x0090 (size: 0x1)

}; // Size: 0xA0

struct FTransformHandle : public FStateStreamHandle
{
}; // Size: 0x10

struct FTransformStaticState
{
}; // Size: 0x4

#endif
