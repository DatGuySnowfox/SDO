#ifndef UE4SS_SDK_DataflowNodes_HPP
#define UE4SS_SDK_DataflowNodes_HPP

#include "DataflowNodes_enums.hpp"

struct FBreakAttributeKeyDataflowNode : public FDataflowNode
{
    FCollectionAttributeKey AttributeKeyIn;                                           // 0x0280 (size: 0x20)
    FString AttributeOut;                                                             // 0x02A0 (size: 0x10)
    FString GroupOut;                                                                 // 0x02B0 (size: 0x10)

}; // Size: 0x2C0

struct FDataflowCollectionAddScalarVertexPropertyNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString Name;                                                                     // 0x0330 (size: 0x10)
    FCollectionAttributeKey AttributeKey;                                             // 0x0340 (size: 0x20)
    TArray<float> VertexWeights;                                                      // 0x0360 (size: 0x10)
    FScalarVertexPropertyGroup TargetGroup;                                           // 0x0370 (size: 0x8)
    EDataflowWeightMapOverrideType OverrideType;                                      // 0x0378 (size: 0x1)

}; // Size: 0x380

struct FDataflowCollectionEditSkinWeightsNode : public FDataflowPrimitiveNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString BoneIndicesName;                                                          // 0x0330 (size: 0x10)
    FString BoneWeightsName;                                                          // 0x0340 (size: 0x10)
    FScalarVertexPropertyGroup VertexGroup;                                           // 0x0350 (size: 0x8)
    FCollectionAttributeKey BoneIndicesKey;                                           // 0x0358 (size: 0x20)
    FCollectionAttributeKey BoneWeightsKey;                                           // 0x0378 (size: 0x20)
    class USkeleton* ObjectSkeleton;                                                  // 0x0398 (size: 0x8)
    bool bCompressSkinWeights;                                                        // 0x03A0 (size: 0x1)
    TArray<FDataflowSkinWeightData> SkinWeights;                                      // 0x03A8 (size: 0x10)
    TArray<USkeletalMesh*> SkeletalMeshes;                                            // 0x03D0 (size: 0x10)

}; // Size: 0x3E8

struct FDataflowFunctionProperty
{
}; // Size: 0x10

struct FDataflowPrimitiveNode : public FDataflowNode
{
}; // Size: 0x280

struct FDataflowSkinWeightData
{
    TArray<float> BoneWeights;                                                        // 0x0000 (size: 0x10)
    TArray<int32> BoneIndices;                                                        // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FFloatOverrideDataflowNode : public FDataflowNode
{
    FName PropertyName;                                                               // 0x0280 (size: 0x8)
    FName KeyName;                                                                    // 0x0288 (size: 0x8)
    float ValueOut;                                                                   // 0x0290 (size: 0x4)

}; // Size: 0x298

struct FGetPhysicsAssetFromSkeletalMeshDataflowNode : public FDataflowNode
{
    class USkeletalMesh* SkeletalMesh;                                                // 0x0280 (size: 0x8)
    class UPhysicsAsset* PhysicsAsset;                                                // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FGetSkeletalMeshDataflowNode : public FDataflowNode
{
    class USkeletalMesh* SkeletalMesh;                                                // 0x0280 (size: 0x8)
    FName PropertyName;                                                               // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FGetSkeletonDataflowNode : public FDataflowNode
{
    class USkeleton* Skeleton;                                                        // 0x0280 (size: 0x8)
    FName PropertyName;                                                               // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FGetStaticMeshDataflowNode : public FDataflowNode
{
    class UStaticMesh* StaticMesh;                                                    // 0x0280 (size: 0x8)
    FName PropertyName;                                                               // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FMakeAttributeKeyDataflowNode : public FDataflowNode
{
    FString GroupIn;                                                                  // 0x0280 (size: 0x10)
    FString AttributeIn;                                                              // 0x0290 (size: 0x10)
    FCollectionAttributeKey AttributeKeyOut;                                          // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FScalarVertexPropertyGroup
{
    FName Name;                                                                       // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FSelectionSetDataflowNode : public FDataflowNode
{
    FString Indices;                                                                  // 0x0280 (size: 0x10)
    TArray<int32> IndicesOut;                                                         // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FSkeletalMeshBoneDataflowNode : public FDataflowNode
{
    FName BoneName;                                                                   // 0x0280 (size: 0x8)
    class USkeletalMesh* SkeletalMesh;                                                // 0x0288 (size: 0x8)
    int32 BoneIndexOut;                                                               // 0x0290 (size: 0x4)
    FName PropertyName;                                                               // 0x0294 (size: 0x8)

}; // Size: 0x2A0

struct FSkeletalMeshReferenceTransformDataflowNode : public FDataflowNode
{
    class USkeletalMesh* SkeletalMeshIn;                                              // 0x0280 (size: 0x8)
    int32 BoneIndexIn;                                                                // 0x0288 (size: 0x4)
    FTransform TransformOut;                                                          // 0x0290 (size: 0x60)

}; // Size: 0x2F0

#endif
