#ifndef UE4SS_SDK_GeometryCollectionNodes_HPP
#define UE4SS_SDK_GeometryCollectionNodes_HPP

#include "GeometryCollectionNodes_enums.hpp"

struct FAbsDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FAddCustomCollectionAttributeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    EStandardGroupNameEnum GroupName;                                                 // 0x0330 (size: 0x1)
    FString CustomGroupName;                                                          // 0x0338 (size: 0x10)
    FString AttrName;                                                                 // 0x0348 (size: 0x10)
    ECustomAttributeTypeEnum CustomAttributeType;                                     // 0x0358 (size: 0x1)
    int32 NumElements;                                                                // 0x035C (size: 0x4)

}; // Size: 0x360

struct FAddDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FAddMaterialToCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowFaceSelection FaceSelection;                                             // 0x0330 (size: 0x28)
    TArray<UMaterial*> Materials;                                                     // 0x0358 (size: 0x10)
    class UMaterial* OutsideMaterial;                                                 // 0x0368 (size: 0x8)
    class UMaterial* InsideMaterial;                                                  // 0x0370 (size: 0x8)
    bool bAssignOutsideMaterial;                                                      // 0x0378 (size: 0x1)
    bool bAssignInsideMaterial;                                                       // 0x0379 (size: 0x1)

}; // Size: 0x380

struct FAddRootProxyMeshToArrayDataflowNode : public FDataflowNode
{
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0280 (size: 0x10)
    FDataflowRootProxyMesh RootProxyMesh;                                             // 0x0290 (size: 0x70)

}; // Size: 0x300

struct FAddToMaterialInterfaceArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0280 (size: 0x10)
    TArray<UMaterialInterface*> MaterialsToAdd;                                       // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FAddUVChannelDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    int32 UVChannel;                                                                  // 0x0330 (size: 0x4)
    FVector2f DefaultValue;                                                           // 0x0334 (size: 0x8)

}; // Size: 0x340

struct FAppendCollectionAssetsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection1;                                              // 0x0280 (size: 0xB0)
    FManagedArrayCollection Collection2;                                              // 0x0330 (size: 0xB0)
    TArray<FString> GeometryGroupGuidsOut1;                                           // 0x03E0 (size: 0x10)
    TArray<FString> GeometryGroupGuidsOut2;                                           // 0x03F0 (size: 0x10)

}; // Size: 0x400

struct FAppendMeshesToCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection AddedSelection;                                       // 0x0330 (size: 0x28)
    TArray<UDynamicMesh*> Meshes;                                                     // 0x0358 (size: 0x10)
    int32 ParentIndex;                                                                // 0x0368 (size: 0x4)

}; // Size: 0x370

struct FAppendPointsDataflowNode : public FDataflowNode
{
    TArray<FVector> PointsA;                                                          // 0x0280 (size: 0x10)
    TArray<FVector> PointsB;                                                          // 0x0290 (size: 0x10)
    TArray<FVector> Points;                                                           // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

struct FApplyMeshProcessorToGeometryCollectionDataflowNode : public FMeshProcessorDataflowNodeBase
{
    FManagedArrayCollection Collection;                                               // 0x02A0 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0350 (size: 0x28)
    bool bWeldVertices;                                                               // 0x0378 (size: 0x1)
    bool bPreserveIsolatedVertices;                                                   // 0x0379 (size: 0x1)

}; // Size: 0x380

struct FApplyMeshProcessorToMeshDataflowNode : public FMeshProcessorDataflowNodeBase
{
    class UDynamicMesh* Mesh;                                                         // 0x02A0 (size: 0x8)

}; // Size: 0x2A8

struct FArcCosDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FArcSinDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FArcTan2DataflowNode : public FDataflowNode
{
    float Y;                                                                          // 0x0280 (size: 0x4)
    float X;                                                                          // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FArcTanDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FAssignMaterialInterfaceToCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowFaceSelection FaceSelection;                                             // 0x0330 (size: 0x28)
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0358 (size: 0x10)
    class UMaterialInterface* Material;                                               // 0x0368 (size: 0x8)
    int32 MaterialIndex;                                                              // 0x0370 (size: 0x4)
    bool bMergeDuplicateMaterials;                                                    // 0x0374 (size: 0x1)

}; // Size: 0x378

struct FAutoClusterDataflowNode : public FDataflowNode
{
    EClusterSizeMethodEnum ClusterSizeMethod;                                         // 0x0280 (size: 0x1)
    int32 ClusterSites;                                                               // 0x0284 (size: 0x4)
    float ClusterFraction;                                                            // 0x0288 (size: 0x4)
    float SiteSize;                                                                   // 0x028C (size: 0x4)
    int32 ClusterGridWidth;                                                           // 0x0290 (size: 0x4)
    int32 ClusterGridDepth;                                                           // 0x0294 (size: 0x4)
    int32 ClusterGridHeight;                                                          // 0x0298 (size: 0x4)
    int32 DriftIterations;                                                            // 0x029C (size: 0x4)
    float MinimumSize;                                                                // 0x02A0 (size: 0x4)
    bool bPreferConvexity;                                                            // 0x02A4 (size: 0x1)
    float ConcavityTolerance;                                                         // 0x02A8 (size: 0x4)
    bool AutoCluster;                                                                 // 0x02AC (size: 0x1)
    bool EnforceSiteParameters;                                                       // 0x02AD (size: 0x1)
    bool AvoidIsolated;                                                               // 0x02AE (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x02B0 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0360 (size: 0x28)
    FLinearColor Color;                                                               // 0x0388 (size: 0x10)
    float LineWidthMultiplier;                                                        // 0x0398 (size: 0x4)
    FLinearColor CenterColor;                                                         // 0x039C (size: 0x10)
    float CenterSize;                                                                 // 0x03AC (size: 0x4)
    bool bRandomizeColor;                                                             // 0x03B0 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x03B4 (size: 0x4)

}; // Size: 0x3B8

struct FAutoUnwrapUVDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowFaceSelection FaceSelection;                                             // 0x0330 (size: 0x28)
    int32 UVChannel;                                                                  // 0x0358 (size: 0x4)
    int32 GutterSize;                                                                 // 0x035C (size: 0x4)

}; // Size: 0x360

struct FBakeTextureFromCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowImage Image;                                                             // 0x0330 (size: 0x28)
    FDataflowFaceSelection FaceSelection;                                             // 0x0358 (size: 0x28)
    EDataflowImageResolution Resolution;                                              // 0x0380 (size: 0x4)
    int32 GutterSize;                                                                 // 0x0384 (size: 0x4)
    int32 UVChannel;                                                                  // 0x0388 (size: 0x4)
    ECollectionBakeTextureAttribute RedChannel;                                       // 0x038C (size: 0x4)
    ECollectionBakeTextureAttribute GreenChannel;                                     // 0x0390 (size: 0x4)
    ECollectionBakeTextureAttribute BlueChannel;                                      // 0x0394 (size: 0x4)
    ECollectionBakeTextureAttribute AlphaChannel;                                     // 0x0398 (size: 0x4)
    float MaxDistance;                                                                // 0x039C (size: 0x4)
    int32 OcclusionRays;                                                              // 0x03A0 (size: 0x4)
    float OcclusionBlurRadius;                                                        // 0x03A4 (size: 0x4)
    float CurvatureBlurRadius;                                                        // 0x03A8 (size: 0x4)
    int32 VoxelResolution;                                                            // 0x03AC (size: 0x4)
    int32 SmoothingIterations;                                                        // 0x03B0 (size: 0x4)
    float ThicknessFactor;                                                            // 0x03B4 (size: 0x4)
    float MaxCurvature;                                                               // 0x03B8 (size: 0x4)

}; // Size: 0x3C0

struct FBakeTransformsInCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)

}; // Size: 0x330

struct FBlueprintToCollectionDataflowNode : public FDataflowNode
{
    class UBlueprint* Blueprint;                                                      // 0x0280 (size: 0x8)
    bool bSplitComponents;                                                            // 0x0288 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    TArray<UMaterial*> Materials;                                                     // 0x0340 (size: 0x10)
    TArray<UMaterialInterface*> MaterialInstances;                                    // 0x0350 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0360 (size: 0x10)

}; // Size: 0x370

struct FBlueprintToCollectionDataflowNode_v2 : public FDataflowNode
{
    class UBlueprint* Blueprint;                                                      // 0x0280 (size: 0x8)
    bool bSplitComponents;                                                            // 0x0288 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    TArray<UMaterialInterface*> Materials;                                            // 0x0340 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0350 (size: 0x10)
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0360 (size: 0x10)

}; // Size: 0x370

struct FBoolArrayToFaceSelectionDataflowNode : public FDataflowNode
{
    TArray<bool> BoolAttributeData;                                                   // 0x0280 (size: 0x10)
    FDataflowFaceSelection FaceSelection;                                             // 0x0290 (size: 0x28)

}; // Size: 0x2B8

struct FBoolToIntDataflowNode : public FDataflowNode
{
    bool bool;                                                                        // 0x0280 (size: 0x1)
    int32 int;                                                                        // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FBoolToStringDataflowNode : public FDataflowNode
{
    bool bool;                                                                        // 0x0280 (size: 0x1)
    FString String;                                                                   // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FBooleanOperationDataflowNode : public FDataflowNode
{
    EBooleanOperationEnum Operation;                                                  // 0x0280 (size: 0x1)
    bool bBoolA;                                                                      // 0x0281 (size: 0x1)
    bool bBoolB;                                                                      // 0x0282 (size: 0x1)
    bool bResult;                                                                     // 0x0283 (size: 0x1)

}; // Size: 0x288

struct FBoundingBoxDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)

}; // Size: 0x368

struct FBoundingSphereDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FSphere BoundingSphere;                                                           // 0x0330 (size: 0x20)

}; // Size: 0x350

struct FBoxFalloffFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    FBox Box;                                                                         // 0x02B8 (size: 0x38)
    FTransform Transform;                                                             // 0x02F0 (size: 0x60)
    float Magnitude;                                                                  // 0x0350 (size: 0x4)
    float MinRange;                                                                   // 0x0354 (size: 0x4)
    float MaxRange;                                                                   // 0x0358 (size: 0x4)
    float Default;                                                                    // 0x035C (size: 0x4)
    EDataflowFieldFalloffType FalloffType;                                            // 0x0360 (size: 0x1)
    TArray<float> FieldFloatResult;                                                   // 0x0368 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0378 (size: 0x10)
    FDataflowVertexSelection FieldSelectionMask;                                      // 0x0388 (size: 0x28)
    int32 NumSamplePositions;                                                         // 0x03B0 (size: 0x4)

}; // Size: 0x3C0

struct FBoxProjectUVDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    int32 UVChannel;                                                                  // 0x0330 (size: 0x4)
    int32 GutterSize;                                                                 // 0x0334 (size: 0x4)
    FVector ProjectionScale;                                                          // 0x0338 (size: 0x18)
    FVector2f UVOffset;                                                               // 0x0350 (size: 0x8)
    bool bAutoFitToBounds;                                                            // 0x0358 (size: 0x1)
    bool bCenterBoxAtPivot;                                                           // 0x0359 (size: 0x1)
    bool bUniformProjectionScale;                                                     // 0x035A (size: 0x1)

}; // Size: 0x360

struct FBoxToMeshDataflowNode : public FDataflowNode
{
    FBox Box;                                                                         // 0x0280 (size: 0x38)
    class UDynamicMesh* Mesh;                                                         // 0x02B8 (size: 0x8)
    int32 TriangleCount;                                                              // 0x02C0 (size: 0x4)

}; // Size: 0x2C8

struct FBranchCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection TrueCollection;                                           // 0x0280 (size: 0xB0)
    FManagedArrayCollection FalseCollection;                                          // 0x0330 (size: 0xB0)
    bool bCondition;                                                                  // 0x03E0 (size: 0x1)
    FManagedArrayCollection ChosenCollection;                                         // 0x03E8 (size: 0xB0)

}; // Size: 0x498

struct FBranchFloatDataflowNode : public FDataflowNode
{
    float A;                                                                          // 0x0280 (size: 0x4)
    float B;                                                                          // 0x0284 (size: 0x4)
    bool bCondition;                                                                  // 0x0288 (size: 0x1)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FBranchIntDataflowNode : public FDataflowNode
{
    int32 A;                                                                          // 0x0280 (size: 0x4)
    int32 B;                                                                          // 0x0284 (size: 0x4)
    bool bCondition;                                                                  // 0x0288 (size: 0x1)
    int32 ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FBranchMeshDataflowNode : public FDataflowNode
{
    class UDynamicMesh* MeshA;                                                        // 0x0280 (size: 0x8)
    class UDynamicMesh* MeshB;                                                        // 0x0288 (size: 0x8)
    bool bCondition;                                                                  // 0x0290 (size: 0x1)
    class UDynamicMesh* Mesh;                                                         // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FBreakTransformDataflowNode : public FDataflowNode
{
    FTransform Transform;                                                             // 0x0280 (size: 0x60)
    FDataflowVectorTypes Translation;                                                 // 0x02E0 (size: 0x20)
    FVector Rotation;                                                                 // 0x0300 (size: 0x18)
    FRotator Rotator;                                                                 // 0x0318 (size: 0x18)
    FQuat Quat;                                                                       // 0x0330 (size: 0x20)
    FDataflowVectorTypes Scale;                                                       // 0x0350 (size: 0x20)

}; // Size: 0x370

struct FBrickCutterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)
    FDataflowTransformSelection TransformSelection;                                   // 0x0368 (size: 0x28)
    FTransform Transform;                                                             // 0x0390 (size: 0x60)
    EFractureBrickBondEnum Bond;                                                      // 0x03F0 (size: 0x1)
    float BrickLength;                                                                // 0x03F4 (size: 0x4)
    float BrickHeight;                                                                // 0x03F8 (size: 0x4)
    float BrickDepth;                                                                 // 0x03FC (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0400 (size: 0x4)
    float ChanceToFracture;                                                           // 0x0404 (size: 0x4)
    bool SplitIslands;                                                                // 0x0408 (size: 0x1)
    float Grout;                                                                      // 0x040C (size: 0x4)
    float Amplitude;                                                                  // 0x0410 (size: 0x4)
    float Frequency;                                                                  // 0x0414 (size: 0x4)
    float Persistence;                                                                // 0x0418 (size: 0x4)
    float Lacunarity;                                                                 // 0x041C (size: 0x4)
    int32 OctaveNumber;                                                               // 0x0420 (size: 0x4)
    float PointSpacing;                                                               // 0x0424 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x0428 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x042C (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x0430 (size: 0x28)

}; // Size: 0x460

struct FCeilDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FClampDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float min;                                                                        // 0x0284 (size: 0x4)
    float max;                                                                        // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FClearConvexHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCloseGeometryOnCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)

}; // Size: 0x330

struct FClusterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FClusterFlattenDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection OptionalTransformSelection;                           // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FClusterIsolatedRootsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)

}; // Size: 0x330

struct FClusterMagnetDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    int32 Iterations;                                                                 // 0x0358 (size: 0x4)

}; // Size: 0x360

struct FClusterMergeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FClusterMergeToNeighborsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    EClusterNeighborSelectionMethodEnum NeighborSelectionMethod;                      // 0x0358 (size: 0x1)
    float MinVolumeCubeRoot;                                                          // 0x035C (size: 0x4)
    bool bOnlyToConnected;                                                            // 0x0360 (size: 0x1)
    bool bOnlySameParent;                                                             // 0x0361 (size: 0x1)

}; // Size: 0x368

struct FClusterScatterPointsDataflowNode : public FDataflowNode
{
    int32 NumberClustersMin;                                                          // 0x0280 (size: 0x4)
    int32 NumberClustersMax;                                                          // 0x0284 (size: 0x4)
    int32 PointsPerClusterMin;                                                        // 0x0288 (size: 0x4)
    int32 PointsPerClusterMax;                                                        // 0x028C (size: 0x4)
    float ClusterRadiusFractionMin;                                                   // 0x0290 (size: 0x4)
    float ClusterRadiusFractionMax;                                                   // 0x0294 (size: 0x4)
    float ClusterRadiusOffset;                                                        // 0x0298 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x029C (size: 0x4)
    FBox BoundingBox;                                                                 // 0x02A0 (size: 0x38)
    TArray<FVector> Points;                                                           // 0x02D8 (size: 0x10)

}; // Size: 0x2E8

struct FClusterUnclusterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionFaceSelectionCustomDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString FaceIndicies;                                                             // 0x0330 (size: 0x10)
    FDataflowFaceSelection FaceSelection;                                             // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionFaceSelectionInvertDataflowNode : public FDataflowNode
{
    FDataflowFaceSelection FaceSelection;                                             // 0x0280 (size: 0x28)

}; // Size: 0x2A8

struct FCollectionSelectInternalFacesDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    FDataflowFaceSelection FaceSelection;                                             // 0x0358 (size: 0x28)

}; // Size: 0x380

struct FCollectionSelectionByAttrDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x0330 (size: 0x20)
    ESelectionByAttrGroup Group;                                                      // 0x0350 (size: 0x1)
    FString Attribute;                                                                // 0x0358 (size: 0x10)
    ESelectionByAttrOperation Operation;                                              // 0x0368 (size: 0x1)
    FString Value;                                                                    // 0x0370 (size: 0x10)
    FDataflowVertexSelection VertexSelection;                                         // 0x0380 (size: 0x28)
    FDataflowFaceSelection FaceSelection;                                             // 0x03A8 (size: 0x28)
    FDataflowTransformSelection TransformSelection;                                   // 0x03D0 (size: 0x28)
    FDataflowGeometrySelection GeometrySelection;                                     // 0x03F8 (size: 0x28)
    FDataflowMaterialSelection MaterialSelection;                                     // 0x0420 (size: 0x28)

}; // Size: 0x448

struct FCollectionSelectionConvertDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    FDataflowFaceSelection FaceSelection;                                             // 0x0358 (size: 0x28)
    FDataflowVertexSelection VertexSelection;                                         // 0x0380 (size: 0x28)
    bool bAllElementsMustBeSelected;                                                  // 0x03A8 (size: 0x1)

}; // Size: 0x3B0

struct FCollectionSelectionInvertDataflowNode : public FDataflowNode
{
    FDataflowSelectionTypes Selection;                                                // 0x0280 (size: 0x28)

}; // Size: 0x2A8

struct FCollectionSelectionSetOperationDataflowNode : public FDataflowNode
{
    ESetOperationEnum Operation;                                                      // 0x0280 (size: 0x1)
    FDataflowSelectionTypes SelectionA;                                               // 0x0288 (size: 0x28)
    FDataflowSelectionTypes SelectionB;                                               // 0x02B0 (size: 0x28)
    FDataflowSelectionTypes Selection;                                                // 0x02D8 (size: 0x28)

}; // Size: 0x300

struct FCollectionSelectionToMeshesDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    bool bConvertSelectionToLeaves;                                                   // 0x0358 (size: 0x1)
    bool bWeldVertices;                                                               // 0x0359 (size: 0x1)
    bool bPreserveIsolatedVertices;                                                   // 0x035A (size: 0x1)
    TArray<UDynamicMesh*> Meshes;                                                     // 0x0360 (size: 0x10)

}; // Size: 0x370

struct FCollectionSetPivotDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FTransform Transform;                                                             // 0x0330 (size: 0x60)

}; // Size: 0x390

struct FCollectionToMeshDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    bool bCenterPivot;                                                                // 0x0330 (size: 0x1)
    class UDynamicMesh* Mesh;                                                         // 0x0338 (size: 0x8)

}; // Size: 0x340

struct FCollectionToPointsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<FVector> Points;                                                           // 0x0330 (size: 0x10)

}; // Size: 0x340

struct FCollectionTransformSelectionAllDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionByFloatAttrDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString GroupName;                                                                // 0x0330 (size: 0x10)
    FString AttrName;                                                                 // 0x0340 (size: 0x10)
    float min;                                                                        // 0x0350 (size: 0x4)
    float max;                                                                        // 0x0354 (size: 0x4)
    ERangeSettingEnum RangeSetting;                                                   // 0x0358 (size: 0x1)
    bool bInclusive;                                                                  // 0x0359 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x0360 (size: 0x28)

}; // Size: 0x388

struct FCollectionTransformSelectionByIntAttrDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString GroupName;                                                                // 0x0330 (size: 0x10)
    FString AttrName;                                                                 // 0x0340 (size: 0x10)
    int32 min;                                                                        // 0x0350 (size: 0x4)
    int32 max;                                                                        // 0x0354 (size: 0x4)
    ERangeSettingEnum RangeSetting;                                                   // 0x0358 (size: 0x1)
    bool bInclusive;                                                                  // 0x0359 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x0360 (size: 0x28)

}; // Size: 0x388

struct FCollectionTransformSelectionByPercentageDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    int32 Percentage;                                                                 // 0x02A8 (size: 0x4)
    bool bDeterministic;                                                              // 0x02AC (size: 0x1)
    float RandomSeed;                                                                 // 0x02B0 (size: 0x4)

}; // Size: 0x2B8

struct FCollectionTransformSelectionBySizeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    float SizeMin;                                                                    // 0x0330 (size: 0x4)
    float SizeMax;                                                                    // 0x0334 (size: 0x4)
    ERangeSettingEnum RangeSetting;                                                   // 0x0338 (size: 0x1)
    bool bInclusive;                                                                  // 0x0339 (size: 0x1)
    bool bUseRelativeSize;                                                            // 0x033A (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionByVolumeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    float VolumeMin;                                                                  // 0x0330 (size: 0x4)
    float VolumeMax;                                                                  // 0x0334 (size: 0x4)
    ERangeSettingEnum RangeSetting;                                                   // 0x0338 (size: 0x1)
    bool bInclusive;                                                                  // 0x0339 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionChildrenDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)

}; // Size: 0x358

struct FCollectionTransformSelectionClusterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionClusterDataflowNode_v2 : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionContactDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)
    bool bAllowContactInParentLevels;                                                 // 0x0358 (size: 0x1)

}; // Size: 0x360

struct FCollectionTransformSelectionCustomDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString BoneIndicies;                                                             // 0x0330 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionCustomDataflowNode_v2 : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString BoneIndices;                                                              // 0x0330 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionFromIndexArrayDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<int32> BoneIndices;                                                        // 0x0330 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionInBoxDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox Box;                                                                         // 0x0330 (size: 0x38)
    FTransform Transform;                                                             // 0x0370 (size: 0x60)
    ESelectSubjectTypeEnum Type;                                                      // 0x03D0 (size: 0x1)
    bool bAllVerticesMustContainedInBox;                                              // 0x03D1 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x03D8 (size: 0x28)

}; // Size: 0x400

struct FCollectionTransformSelectionInSphereDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FSphere Sphere;                                                                   // 0x0330 (size: 0x20)
    FTransform Transform;                                                             // 0x0350 (size: 0x60)
    ESelectSubjectTypeEnum Type;                                                      // 0x03B0 (size: 0x1)
    bool bAllVerticesMustContainedInSphere;                                           // 0x03B1 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x03B8 (size: 0x28)

}; // Size: 0x3E0

struct FCollectionTransformSelectionInfoDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)
    FString String;                                                                   // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FCollectionTransformSelectionInvertDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)

}; // Size: 0x2A8

struct FCollectionTransformSelectionLeafDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionLevelDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)

}; // Size: 0x358

struct FCollectionTransformSelectionNoneDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionParentDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)

}; // Size: 0x358

struct FCollectionTransformSelectionRandomDataflowNode : public FDataflowNode
{
    bool bDeterministic;                                                              // 0x0280 (size: 0x1)
    float RandomSeed;                                                                 // 0x0284 (size: 0x4)
    float RandomThreshold;                                                            // 0x0288 (size: 0x4)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionTransformSelectionRootDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FCollectionTransformSelectionSetOperationDataflowNode : public FDataflowNode
{
    ESetOperationEnum Operation;                                                      // 0x0280 (size: 0x1)
    FDataflowTransformSelection TransformSelectionA;                                  // 0x0288 (size: 0x28)
    FDataflowTransformSelection TransformSelectionB;                                  // 0x02B0 (size: 0x28)
    FDataflowTransformSelection TransformSelection;                                   // 0x02D8 (size: 0x28)

}; // Size: 0x300

struct FCollectionTransformSelectionSiblingsDataflowNode : public FDataflowNode
{
    FDataflowTransformSelection TransformSelection;                                   // 0x0280 (size: 0x28)
    FManagedArrayCollection Collection;                                               // 0x02A8 (size: 0xB0)

}; // Size: 0x358

struct FCollectionTransformSelectionTargetLevelDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    int32 TargetLevel;                                                                // 0x0330 (size: 0x4)
    bool bSkipEmbedded;                                                               // 0x0334 (size: 0x1)
    FDataflowTransformSelection TransformSelection;                                   // 0x0338 (size: 0x28)

}; // Size: 0x360

struct FCollectionVertexSelectionByPercentageDataflowNode : public FDataflowNode
{
    FDataflowVertexSelection VertexSelection;                                         // 0x0280 (size: 0x28)
    int32 Percentage;                                                                 // 0x02A8 (size: 0x4)
    bool bDeterministic;                                                              // 0x02AC (size: 0x1)
    float RandomSeed;                                                                 // 0x02B0 (size: 0x4)

}; // Size: 0x2B8

struct FCollectionVertexSelectionCustomDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString VertexIndicies;                                                           // 0x0330 (size: 0x10)
    FDataflowVertexSelection VertexSelection;                                         // 0x0340 (size: 0x28)

}; // Size: 0x368

struct FCollectionVertexSelectionSetOperationDataflowNode : public FDataflowNode
{
    ESetOperationEnum Operation;                                                      // 0x0280 (size: 0x1)
    FDataflowVertexSelection VertexSelectionA;                                        // 0x0288 (size: 0x28)
    FDataflowVertexSelection VertexSelectionB;                                        // 0x02B0 (size: 0x28)
    FDataflowVertexSelection VertexSelection;                                         // 0x02D8 (size: 0x28)

}; // Size: 0x300

struct FCompareFloatDataflowNode : public FDataflowNode
{
    ECompareOperationEnum Operation;                                                  // 0x0280 (size: 0x1)
    float FloatA;                                                                     // 0x0284 (size: 0x4)
    float FloatB;                                                                     // 0x0288 (size: 0x4)
    bool Result;                                                                      // 0x028C (size: 0x1)

}; // Size: 0x290

struct FCompareIntDataflowNode : public FDataflowNode
{
    ECompareOperationEnum Operation;                                                  // 0x0280 (size: 0x1)
    int32 IntA;                                                                       // 0x0284 (size: 0x4)
    int32 IntB;                                                                       // 0x0288 (size: 0x4)
    bool Result;                                                                      // 0x028C (size: 0x1)

}; // Size: 0x290

struct FConvexHullToMeshDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0330 (size: 0x28)
    bool bUseRobustHulls;                                                             // 0x0358 (size: 0x1)
    class UDynamicMesh* Mesh;                                                         // 0x0360 (size: 0x8)
    TArray<UDynamicMesh*> Meshes;                                                     // 0x0368 (size: 0x10)

}; // Size: 0x378

struct FCopyConvexHullsFromRootDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FManagedArrayCollection FromCollection;                                           // 0x0330 (size: 0xB0)
    bool bSkipIfEmpty;                                                                // 0x03E0 (size: 0x1)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x03E4 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x03FC (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x0400 (size: 0x4)

}; // Size: 0x408

struct FCosDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FCreateColorArrayFromFloatArrayDataflowNode : public FDataflowNode
{
    TArray<float> FloatArray;                                                         // 0x0280 (size: 0x10)
    TArray<FLinearColor> ColorArray;                                                  // 0x0290 (size: 0x10)
    bool bNormalizeInput;                                                             // 0x02A0 (size: 0x1)
    FLinearColor Color;                                                               // 0x02A4 (size: 0x10)

}; // Size: 0x2B8

struct FCreateGeometryCollectionFromSourcesDataflowNode : public FDataflowNode
{
    TArray<FGeometryCollectionSource> Sources;                                        // 0x0280 (size: 0x10)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    TArray<UMaterial*> Materials;                                                     // 0x0340 (size: 0x10)
    TArray<UMaterialInterface*> MaterialInstances;                                    // 0x0350 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0360 (size: 0x10)

}; // Size: 0x370

struct FCreateGeometryCollectionFromSourcesDataflowNode_v2 : public FDataflowNode
{
    TArray<FGeometryCollectionSource> Sources;                                        // 0x0280 (size: 0x10)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    TArray<UMaterialInterface*> Materials;                                            // 0x0340 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0350 (size: 0x10)
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0360 (size: 0x10)

}; // Size: 0x370

struct FCreateLeafConvexHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowSphereCovering SphereCovering;                                           // 0x0330 (size: 0x20)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0350 (size: 0x28)
    EGenerateConvexMethod GenerateMethod;                                             // 0x0378 (size: 0x1)
    float IntersectIfComputedIsSmallerByFactor;                                       // 0x037C (size: 0x4)
    float MinExternalVolumeToIntersect;                                               // 0x0380 (size: 0x4)
    bool bComputeIntersectionsBeforeHull;                                             // 0x0384 (size: 0x1)
    float SimplificationDistanceThreshold;                                            // 0x0388 (size: 0x4)
    FDataflowConvexDecompositionSettings ConvexDecompositionSettings;                 // 0x038C (size: 0x24)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x03B0 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x03C8 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x03CC (size: 0x4)
    FDataflowNodeSphereCoveringDebugDrawSettings SphereCoveringDebugDrawRenderSettings; // 0x03D0 (size: 0x40)

}; // Size: 0x410

struct FCreateNonOverlappingConvexHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    float CanExceedFraction;                                                          // 0x0330 (size: 0x4)
    float SimplificationDistanceThreshold;                                            // 0x0334 (size: 0x4)
    EConvexOverlapRemovalMethodEnum OverlapRemovalMethod;                             // 0x0338 (size: 0x1)
    float OverlapRemovalShrinkPercent;                                                // 0x033C (size: 0x4)
    float CanRemoveFraction;                                                          // 0x0340 (size: 0x4)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x0344 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x035C (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x0360 (size: 0x4)

}; // Size: 0x368

struct FCrossProductDataflowNode : public FDataflowNode
{
    FVector VectorA;                                                                  // 0x0280 (size: 0x18)
    FVector VectorB;                                                                  // 0x0298 (size: 0x18)
    FVector ReturnValue;                                                              // 0x02B0 (size: 0x18)

}; // Size: 0x2C8

struct FCubeDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FDataflowConvexDecompositionSettings
{
    float MinSizeToDecompose;                                                         // 0x0000 (size: 0x4)
    float MaxGeoToHullVolumeRatioToDecompose;                                         // 0x0004 (size: 0x4)
    float ErrorTolerance;                                                             // 0x0008 (size: 0x4)
    int32 MaxHullsPerGeometry;                                                        // 0x000C (size: 0x4)
    float MinThicknessTolerance;                                                      // 0x0010 (size: 0x4)
    int32 NumAdditionalSplits;                                                        // 0x0014 (size: 0x4)
    bool bProtectNegativeSpace;                                                       // 0x0018 (size: 0x1)
    bool bOnlyConnectedToHull;                                                        // 0x0019 (size: 0x1)
    float NegativeSpaceTolerance;                                                     // 0x001C (size: 0x4)
    float NegativeSpaceMinRadius;                                                     // 0x0020 (size: 0x4)

}; // Size: 0x24

struct FDataflowGetArrayElementNode : public FDataflowNode
{
    FDataflowArrayTypes Array;                                                        // 0x0280 (size: 0x1)
    int32 Index;                                                                      // 0x0284 (size: 0x4)
    FDataflowAllTypes Element;                                                        // 0x0288 (size: 0x1)

}; // Size: 0x290

struct FDataflowGetArraySizeNode : public FDataflowNode
{
    FDataflowArrayTypes Array;                                                        // 0x0280 (size: 0x1)
    int32 Size;                                                                       // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FDataflowMakeManagedArrayCollectionArrayNode : public FDataflowNode
{
    TArray<FManagedArrayCollection> Array;                                            // 0x0280 (size: 0x10)
    FManagedArrayCollection Element;                                                  // 0x0290 (size: 0xB0)

}; // Size: 0x340

struct FDataflowMeshAppendDataflowNode : public FDataflowNode
{
    class UDataflowMesh* Mesh;                                                        // 0x0280 (size: 0x8)
    class UDataflowMesh* AppendMesh;                                                  // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FDataflowRootProxyMesh
{
    class UStaticMesh* Mesh;                                                          // 0x0000 (size: 0x8)
    FTransform Transform;                                                             // 0x0010 (size: 0x60)

}; // Size: 0x70

struct FDataflowSphereCovering
{
}; // Size: 0x20

struct FDegreesToRadiansDataflowNode : public FDataflowNode
{
    float Degrees;                                                                    // 0x0280 (size: 0x4)
    float Radians;                                                                    // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FDistanceDataflowNode : public FDataflowNode
{
    FVector PointA;                                                                   // 0x0280 (size: 0x18)
    FVector PointB;                                                                   // 0x0298 (size: 0x18)
    float ReturnValue;                                                                // 0x02B0 (size: 0x4)

}; // Size: 0x2B8

struct FDivideDataflowNode : public FSafeDivideDataflowNode
{
}; // Size: 0x290

struct FDivisionDataflowNode : public FDataflowNode
{
    float Dividend;                                                                   // 0x0280 (size: 0x4)
    float Divisor;                                                                    // 0x0284 (size: 0x4)
    float Remainder;                                                                  // 0x0288 (size: 0x4)
    int32 ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FDotProductDataflowNode : public FDataflowNode
{
    FVector VectorA;                                                                  // 0x0280 (size: 0x18)
    FVector VectorB;                                                                  // 0x0298 (size: 0x18)
    float ReturnValue;                                                                // 0x02B0 (size: 0x4)

}; // Size: 0x2B8

struct FDuplicateMeshUVChannelNode : public FDataflowNode
{
    class UDataflowMesh* Mesh;                                                        // 0x0280 (size: 0x8)
    int32 SourceUVChannel;                                                            // 0x0288 (size: 0x4)
    int32 NewUVChannel;                                                               // 0x028C (size: 0x4)

}; // Size: 0x290

struct FEFitDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float OldMin;                                                                     // 0x0284 (size: 0x4)
    float OldMax;                                                                     // 0x0288 (size: 0x4)
    float NewMin;                                                                     // 0x028C (size: 0x4)
    float NewMax;                                                                     // 0x0290 (size: 0x4)
    float ReturnValue;                                                                // 0x0294 (size: 0x4)

}; // Size: 0x298

struct FExpDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FExpandBoundingBoxDataflowNode : public FDataflowNode
{
    FBox BoundingBox;                                                                 // 0x0280 (size: 0x38)
    FVector min;                                                                      // 0x02B8 (size: 0x18)
    FVector max;                                                                      // 0x02D0 (size: 0x18)
    FVector Center;                                                                   // 0x02E8 (size: 0x18)
    FVector HalfExtents;                                                              // 0x0300 (size: 0x18)
    float Volume;                                                                     // 0x0318 (size: 0x4)

}; // Size: 0x320

struct FExpandBoundingSphereDataflowNode : public FDataflowNode
{
    FSphere BoundingSphere;                                                           // 0x0280 (size: 0x20)
    FVector Center;                                                                   // 0x02A0 (size: 0x18)
    float Radius;                                                                     // 0x02B8 (size: 0x4)
    float Volume;                                                                     // 0x02BC (size: 0x4)

}; // Size: 0x2C0

struct FExpandVectorDataflowNode : public FDataflowNode
{
    FVector Vector;                                                                   // 0x0280 (size: 0x18)
    float X;                                                                          // 0x0298 (size: 0x4)
    float Y;                                                                          // 0x029C (size: 0x4)
    float Z;                                                                          // 0x02A0 (size: 0x4)

}; // Size: 0x2A8

struct FExplodedViewDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    float UniformScale;                                                               // 0x0330 (size: 0x4)
    FVector Scale;                                                                    // 0x0338 (size: 0x18)
    FVector Offset;                                                                   // 0x0350 (size: 0x18)

}; // Size: 0x368

struct FFieldMakeDenseFloatArrayDataflowNode : public FDataflowNode
{
    TArray<float> FieldFloatInput;                                                    // 0x0280 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0290 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02A0 (size: 0x4)
    float Default;                                                                    // 0x02A4 (size: 0x4)
    TArray<float> FieldFloatResult;                                                   // 0x02A8 (size: 0x10)

}; // Size: 0x2B8

struct FFilterPointSetWithMeshDataflowNode : public FDataflowNode
{
    class UDynamicMesh* TargetMesh;                                                   // 0x0280 (size: 0x8)
    bool bKeepInside;                                                                 // 0x0288 (size: 0x1)
    float WindingThreshold;                                                           // 0x028C (size: 0x4)
    TArray<FVector> SamplePoints;                                                     // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FFitDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float OldMin;                                                                     // 0x0284 (size: 0x4)
    float OldMax;                                                                     // 0x0288 (size: 0x4)
    float NewMin;                                                                     // 0x028C (size: 0x4)
    float NewMax;                                                                     // 0x0290 (size: 0x4)
    float ReturnValue;                                                                // 0x0294 (size: 0x4)

}; // Size: 0x298

struct FFixTinyGeoDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    EFixTinyGeoMergeType MergeType;                                                   // 0x0358 (size: 0x1)
    bool bOnFractureLevel;                                                            // 0x0359 (size: 0x1)
    bool bOnlyClusters;                                                               // 0x035A (size: 0x1)
    bool bOnlySameParent;                                                             // 0x035B (size: 0x1)
    bool bFractureLevelIsAll;                                                         // 0x035C (size: 0x1)
    EFixTinyGeoNeighborSelectionMethod NeighborSelection;                             // 0x035D (size: 0x1)
    bool bOnlyToConnected;                                                            // 0x035E (size: 0x1)
    bool bUseCollectionProximityForConnections;                                       // 0x035F (size: 0x1)
    EFixTinyGeoUseBoneSelection UseBoneSelection;                                     // 0x0360 (size: 0x1)
    EFixTinyGeoGeometrySelectionMethod SelectionMethod;                               // 0x0361 (size: 0x1)
    float MinVolumeCubeRoot;                                                          // 0x0364 (size: 0x4)
    float RelativeVolume;                                                             // 0x0368 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x036C (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x0370 (size: 0x4)

}; // Size: 0x378

struct FFloatArrayComputeStatisticsDataflowNode : public FDataflowNode
{
    TArray<float> FloatArray;                                                         // 0x0280 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0290 (size: 0x28)
    EStatisticsOperationEnum OperationName;                                           // 0x02B8 (size: 0x1)
    float Value;                                                                      // 0x02BC (size: 0x4)
    TArray<int32> Indices;                                                            // 0x02C0 (size: 0x10)

}; // Size: 0x2D0

struct FFloatArrayNormalizeDataflowNode : public FDataflowNode
{
    TArray<float> InFloatArray;                                                       // 0x0280 (size: 0x10)
    FDataflowVertexSelection Selection;                                               // 0x0290 (size: 0x28)
    float MinRange;                                                                   // 0x02B8 (size: 0x4)
    float MaxRange;                                                                   // 0x02BC (size: 0x4)
    TArray<float> OutFloatArray;                                                      // 0x02C0 (size: 0x10)

}; // Size: 0x2D0

struct FFloatArrayToIntArrayDataflowNode : public FDataflowNode
{
    EFloatArrayToIntArrayFunctionEnum Function;                                       // 0x0280 (size: 0x1)
    TArray<float> FloatArray;                                                         // 0x0288 (size: 0x10)
    TArray<int32> IntArray;                                                           // 0x0298 (size: 0x10)

}; // Size: 0x2A8

struct FFloatArrayToVertexSelectionDataflowNode : public FDataflowNode
{
    TArray<float> FloatArray;                                                         // 0x0280 (size: 0x10)
    ECompareOperation1Enum Operation;                                                 // 0x0290 (size: 0x1)
    float Threshold;                                                                  // 0x0294 (size: 0x4)
    FDataflowVertexSelection VertexSelection;                                         // 0x0298 (size: 0x28)

}; // Size: 0x2C0

struct FFloatMathExpressionDataflowNode : public FDataflowNode
{
    float A;                                                                          // 0x0280 (size: 0x4)
    float B;                                                                          // 0x0284 (size: 0x4)
    float C;                                                                          // 0x0288 (size: 0x4)
    float D;                                                                          // 0x028C (size: 0x4)
    FString Expression;                                                               // 0x0290 (size: 0x10)
    float ReturnValue;                                                                // 0x02A0 (size: 0x4)

}; // Size: 0x2A8

struct FFloatToDoubleDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    double Double;                                                                    // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FFloatToIntDataflowNode : public FDataflowNode
{
    EFloatToIntFunctionEnum Function;                                                 // 0x0280 (size: 0x1)
    float float;                                                                      // 0x0284 (size: 0x4)
    int32 int;                                                                        // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FFloatToStringDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    FString String;                                                                   // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FFloorDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FFracDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FGenerateClusterConvexHullsFromChildrenHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowSphereCovering SphereCovering;                                           // 0x0330 (size: 0x20)
    int32 ConvexCount;                                                                // 0x0350 (size: 0x4)
    double ErrorTolerance;                                                            // 0x0358 (size: 0x8)
    bool bPreferExternalCollisionShapes;                                              // 0x0360 (size: 0x1)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0368 (size: 0x28)
    EConvexHullProximityFilter MergeProximityFilter;                                  // 0x0390 (size: 0x1)
    float MergeProximityDistanceThreshold;                                            // 0x0394 (size: 0x4)
    bool bAllowMergingLeafHulls;                                                      // 0x0398 (size: 0x1)
    bool bProtectNegativeSpace;                                                       // 0x0399 (size: 0x1)
    ENegativeSpaceSampleMethodDataflowEnum SampleMethod;                              // 0x039A (size: 0x1)
    bool bRequireSearchSampleCoverage;                                                // 0x039B (size: 0x1)
    bool bOnlyConnectedToHull;                                                        // 0x039C (size: 0x1)
    int32 TargetNumSamples;                                                           // 0x03A0 (size: 0x4)
    double MinSampleSpacing;                                                          // 0x03A8 (size: 0x8)
    double NegativeSpaceTolerance;                                                    // 0x03B0 (size: 0x8)
    double MinRadius;                                                                 // 0x03B8 (size: 0x8)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x03C0 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x03D8 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x03DC (size: 0x4)
    FDataflowNodeSphereCoveringDebugDrawSettings SphereCoveringDebugDrawRenderSettings; // 0x03E0 (size: 0x40)

}; // Size: 0x420

struct FGenerateClusterConvexHullsFromLeafHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowSphereCovering SphereCovering;                                           // 0x0330 (size: 0x20)
    int32 ConvexCount;                                                                // 0x0350 (size: 0x4)
    double ErrorTolerance;                                                            // 0x0358 (size: 0x8)
    bool bPreferExternalCollisionShapes;                                              // 0x0360 (size: 0x1)
    EAllowConvexMergeMethod AllowMerges;                                              // 0x0361 (size: 0x1)
    EConvexHullProximityFilter MergeProximityFilter;                                  // 0x0362 (size: 0x1)
    float MergeProximityDistanceThreshold;                                            // 0x0364 (size: 0x4)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0368 (size: 0x28)
    bool bAllowMergingLeafHulls;                                                      // 0x0390 (size: 0x1)
    bool bProtectNegativeSpace;                                                       // 0x0391 (size: 0x1)
    ENegativeSpaceSampleMethodDataflowEnum SampleMethod;                              // 0x0392 (size: 0x1)
    bool bRequireSearchSampleCoverage;                                                // 0x0393 (size: 0x1)
    bool bOnlyConnectedToHull;                                                        // 0x0394 (size: 0x1)
    int32 TargetNumSamples;                                                           // 0x0398 (size: 0x4)
    double MinSampleSpacing;                                                          // 0x03A0 (size: 0x8)
    double NegativeSpaceTolerance;                                                    // 0x03A8 (size: 0x8)
    double MinRadius;                                                                 // 0x03B0 (size: 0x8)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x03B8 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x03D0 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x03D4 (size: 0x4)
    FDataflowNodeSphereCoveringDebugDrawSettings SphereCoveringDebugDrawRenderSettings; // 0x03D8 (size: 0x40)

}; // Size: 0x418

struct FGeometryCollectionSetKinematicVertexSelectionNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowVertexSelection VertexSelection;                                         // 0x0330 (size: 0x28)
    ESetKinematicVertexSelectionKinematicValue KinematicValue;                        // 0x0358 (size: 0x1)

}; // Size: 0x360

struct FGeometryCollectionTerminalDataflowNode : public FDataflowTerminalNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<UMaterial*> Materials;                                                     // 0x0330 (size: 0x10)
    TArray<UMaterialInterface*> MaterialInstances;                                    // 0x0340 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0350 (size: 0x10)

}; // Size: 0x360

struct FGeometryCollectionTerminalDataflowNode_v2 : public FDataflowTerminalNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<UMaterialInterface*> Materials;                                            // 0x0330 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0340 (size: 0x10)
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0350 (size: 0x10)

}; // Size: 0x360

struct FGeometryCollectionToCollectionDataflowNode : public FDataflowNode
{
    class UGeometryCollection* GeometryCollection;                                    // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    TArray<UMaterial*> Materials;                                                     // 0x0338 (size: 0x10)
    TArray<UMaterialInterface*> MaterialInstances;                                    // 0x0348 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FGeometryCollectionToCollectionDataflowNode_v2 : public FDataflowNode
{
    class UGeometryCollection* GeometryCollection;                                    // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    TArray<UMaterialInterface*> Materials;                                            // 0x0338 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x0348 (size: 0x10)
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FGeometryCollectionTransferVertexAttributeNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FManagedArrayCollection FromCollection;                                           // 0x0330 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x03E0 (size: 0x20)
    EDataflowTransferVertexAttributeNodeTransferMethod TransferMethod;                // 0x0400 (size: 0x1)
    EDataflowTransferVertexAttributeNodeBoundingVolume BoundingVolumeType;            // 0x0401 (size: 0x1)
    EDataflowTransferVertexAttributeNodeSourceScale SourceScale;                      // 0x0402 (size: 0x1)
    EDataflowTransferVertexAttributeNodeFalloff Falloff;                              // 0x0403 (size: 0x1)
    float FalloffThreshold;                                                           // 0x0404 (size: 0x4)
    float EdgeMultiplier;                                                             // 0x0408 (size: 0x4)
    float BoundMultiplier;                                                            // 0x040C (size: 0x4)
    FString TransformNameSuffix;                                                      // 0x0410 (size: 0x10)

}; // Size: 0x420

struct FGeometryCollectionTransferVertexSkinWeightsNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FManagedArrayCollection FromCollection;                                           // 0x0330 (size: 0xB0)
    EDataflowTransferVertexAttributeNodeTransferMethod TransferMethod;                // 0x03E0 (size: 0x1)
    EDataflowTransferVertexAttributeNodeBoundingVolume BoundingVolumeType;            // 0x03E1 (size: 0x1)
    EDataflowTransferVertexAttributeNodeSourceScale SourceScale;                      // 0x03E2 (size: 0x1)
    EDataflowTransferVertexAttributeNodeFalloff Falloff;                              // 0x03E3 (size: 0x1)
    float FalloffThreshold;                                                           // 0x03E4 (size: 0x4)
    float EdgeMultiplier;                                                             // 0x03E8 (size: 0x4)
    float BoundMultiplier;                                                            // 0x03EC (size: 0x4)
    FString TransformNameSuffix;                                                      // 0x03F0 (size: 0x10)

}; // Size: 0x400

struct FGeometryCollectionVertexScalarToVertexIndicesNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x0330 (size: 0x20)
    float SelectionThreshold;                                                         // 0x0350 (size: 0x4)
    TArray<int32> VertexIndices;                                                      // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FGeometrySelectionToVertexSelectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString GeometryIndices;                                                          // 0x0330 (size: 0x10)
    FDataflowGeometrySelection GeometrySelection;                                     // 0x0340 (size: 0x28)
    FDataflowVertexSelection VertexSelection;                                         // 0x0368 (size: 0x28)

}; // Size: 0x390

struct FGetArrayElementDataflowNode : public FDataflowNode
{
    int32 Index;                                                                      // 0x0280 (size: 0x4)
    TArray<FVector> Points;                                                           // 0x0288 (size: 0x10)
    FVector Point;                                                                    // 0x0298 (size: 0x18)

}; // Size: 0x2B0

struct FGetBoolOverrideFromAssetDataflowNode : public FDataflowOverrideNode
{
    bool bool;                                                                        // 0x02A0 (size: 0x1)
    bool BoolDefault;                                                                 // 0x02A1 (size: 0x1)

}; // Size: 0x2A8

struct FGetBoundingBoxesFromCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    TArray<FBox> BoundingBoxes;                                                       // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FGetBoxLengthsDataflowNode : public FDataflowNode
{
    TArray<FBox> Boxes;                                                               // 0x0280 (size: 0x10)
    TArray<float> Lengths;                                                            // 0x0290 (size: 0x10)
    EBoxLengthMeasurementMethod MeasurementMethod;                                    // 0x02A0 (size: 0x1)

}; // Size: 0x2A8

struct FGetCentroidsFromCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    TArray<FVector> Centroids;                                                        // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FGetCollectionAttributeDataTypedDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x0330 (size: 0x20)
    EStandardGroupNameEnum GroupName;                                                 // 0x0350 (size: 0x1)
    FString CustomGroupName;                                                          // 0x0358 (size: 0x10)
    FString AttrName;                                                                 // 0x0368 (size: 0x10)
    TArray<bool> BoolAttributeData;                                                   // 0x0378 (size: 0x10)
    TArray<float> FloatAttributeData;                                                 // 0x0388 (size: 0x10)
    TArray<double> DoubleAttributeData;                                               // 0x0398 (size: 0x10)
    TArray<int32> Int32AttributeData;                                                 // 0x03A8 (size: 0x10)
    TArray<FString> StringAttributeData;                                              // 0x03B8 (size: 0x10)
    TArray<FVector3f> Vector3fAttributeData;                                          // 0x03C8 (size: 0x10)
    TArray<FVector3d> Vector3dAttributeData;                                          // 0x03D8 (size: 0x10)
    TArray<FLinearColor> LinearColorAttributeData;                                    // 0x03E8 (size: 0x10)

}; // Size: 0x3F8

struct FGetCollectionAttributeDataTypedDataflowNode_v2 : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x0330 (size: 0x20)
    EStandardGroupNameEnum GroupName;                                                 // 0x0350 (size: 0x1)
    FString CustomGroupName;                                                          // 0x0358 (size: 0x10)
    FString AttrName;                                                                 // 0x0368 (size: 0x10)
    TArray<bool> BoolAttributeData;                                                   // 0x0378 (size: 0x10)
    FDataflowNumericArrayTypes NumericArray;                                          // 0x0388 (size: 0x10)
    FDataflowVectorArrayTypes VectorArray;                                            // 0x0398 (size: 0x10)
    FDataflowStringArrayTypes StringArray;                                            // 0x03A8 (size: 0x10)

}; // Size: 0x3B8

struct FGetCollectionFromAssetDataflowNode : public FDataflowNode
{
    class UGeometryCollection* CollectionAsset;                                       // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)

}; // Size: 0x338

struct FGetConvexHullVolumeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    float Volume;                                                                     // 0x0358 (size: 0x4)
    bool bSumChildrenForClustersWithoutHulls;                                         // 0x035C (size: 0x1)
    bool bVolumeOfUnion;                                                              // 0x035D (size: 0x1)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x0360 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x0378 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x037C (size: 0x4)

}; // Size: 0x380

struct FGetFloatArrayElementDataflowNode : public FDataflowNode
{
    int32 Index;                                                                      // 0x0280 (size: 0x4)
    TArray<float> FloatArray;                                                         // 0x0288 (size: 0x10)
    float FloatValue;                                                                 // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FGetFloatOverrideFromAssetDataflowNode : public FDataflowOverrideNode
{
    float float;                                                                      // 0x02A0 (size: 0x4)
    float FloatDefault;                                                               // 0x02A4 (size: 0x4)

}; // Size: 0x2A8

struct FGetFromMaterialInterfaceArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0280 (size: 0x10)
    class UMaterialInterface* Material;                                               // 0x0290 (size: 0x8)
    int32 Index;                                                                      // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FGetGeometryCollectionAssetDataflowNode : public FDataflowNode
{
    class UGeometryCollection* Asset;                                                 // 0x0280 (size: 0x8)

}; // Size: 0x288

struct FGetGeometryCollectionSourcesDataflowNode : public FDataflowNode
{
    class UGeometryCollection* Asset;                                                 // 0x0280 (size: 0x8)
    TArray<FGeometryCollectionSource> Sources;                                        // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FGetIntOverrideFromAssetDataflowNode : public FDataflowOverrideNode
{
    int32 int;                                                                        // 0x02A0 (size: 0x4)
    int32 IntDefault;                                                                 // 0x02A4 (size: 0x4)

}; // Size: 0x2A8

struct FGetMaterialFromMaterialsArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterial*> Materials;                                                     // 0x0280 (size: 0x10)
    class UMaterial* Material;                                                        // 0x0290 (size: 0x8)
    int32 MaterialIdx;                                                                // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FGetMaterialInterfaceArraySizeDataflowNode : public FDataflowNode
{
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0280 (size: 0x10)
    int32 Size;                                                                       // 0x0290 (size: 0x4)

}; // Size: 0x298

struct FGetMaterialInterfaceAssetDataflowNode : public FDataflowNode
{
    class UMaterialInterface* Material;                                               // 0x0280 (size: 0x8)

}; // Size: 0x288

struct FGetMeshDataDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    int32 VertexCount;                                                                // 0x0288 (size: 0x4)
    int32 EdgeCount;                                                                  // 0x028C (size: 0x4)
    int32 TriangleCount;                                                              // 0x0290 (size: 0x4)

}; // Size: 0x298

struct FGetNumArrayElementsDataflowNode : public FDataflowNode
{
    TArray<float> FloatArray;                                                         // 0x0280 (size: 0x10)
    TArray<int32> IntArray;                                                           // 0x0290 (size: 0x10)
    TArray<FVector> Points;                                                           // 0x02A0 (size: 0x10)
    TArray<FVector3f> Vector3fArray;                                                  // 0x02B0 (size: 0x10)
    int32 NumElements;                                                                // 0x02C0 (size: 0x4)

}; // Size: 0x2C8

struct FGetNumElementsInCollectionGroupDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    EStandardGroupNameEnum GroupName;                                                 // 0x0330 (size: 0x1)
    FString CustomGroupName;                                                          // 0x0338 (size: 0x10)
    int32 NumElements;                                                                // 0x0348 (size: 0x4)

}; // Size: 0x350

struct FGetRootIndexFromCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    int32 RootIndex;                                                                  // 0x0330 (size: 0x4)

}; // Size: 0x338

struct FGetSchemaDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString String;                                                                   // 0x0330 (size: 0x10)

}; // Size: 0x340

struct FGetStringOverrideFromAssetDataflowNode : public FDataflowOverrideNode
{
    FString String;                                                                   // 0x02A0 (size: 0x10)
    FString StringDefault;                                                            // 0x02B0 (size: 0x10)

}; // Size: 0x2C0

struct FGridScatterPointsDataflowNode : public FDataflowNode
{
    int32 NumberOfPointsInX;                                                          // 0x0280 (size: 0x4)
    int32 NumberOfPointsInY;                                                          // 0x0284 (size: 0x4)
    int32 NumberOfPointsInZ;                                                          // 0x0288 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x028C (size: 0x4)
    float MaxRandomDisplacementX;                                                     // 0x0290 (size: 0x4)
    float MaxRandomDisplacementY;                                                     // 0x0294 (size: 0x4)
    float MaxRandomDisplacementZ;                                                     // 0x0298 (size: 0x4)
    FBox BoundingBox;                                                                 // 0x02A0 (size: 0x38)
    TArray<FVector> Points;                                                           // 0x02D8 (size: 0x10)

}; // Size: 0x2E8

struct FHashStringDataflowNode : public FDataflowNode
{
    FString String;                                                                   // 0x0280 (size: 0x10)
    int32 Hash;                                                                       // 0x0290 (size: 0x4)

}; // Size: 0x298

struct FHashVectorDataflowNode : public FDataflowNode
{
    FVector Vector;                                                                   // 0x0280 (size: 0x18)
    int32 Hash;                                                                       // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FIntToBoolDataflowNode : public FDataflowNode
{
    int32 int;                                                                        // 0x0280 (size: 0x4)
    bool bool;                                                                        // 0x0284 (size: 0x1)

}; // Size: 0x288

struct FIntToDoubleDataflowNode : public FDataflowNode
{
    int32 int;                                                                        // 0x0280 (size: 0x4)
    double Double;                                                                    // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FIntToFloatDataflowNode : public FDataflowNode
{
    int32 int;                                                                        // 0x0280 (size: 0x4)
    float float;                                                                      // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FIntToStringDataflowNode : public FDataflowNode
{
    int32 int;                                                                        // 0x0280 (size: 0x4)
    FString String;                                                                   // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FInverseSqrtDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FInvertTransformDataflowNode : public FDataflowNode
{
    FTransform InTransform;                                                           // 0x0280 (size: 0x60)
    FTransform OutTransform;                                                          // 0x02E0 (size: 0x60)

}; // Size: 0x340

struct FIsNearlyZeroDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    bool ReturnValue;                                                                 // 0x0284 (size: 0x1)

}; // Size: 0x288

struct FLengthDataflowNode : public FDataflowNode
{
    FVector Vector;                                                                   // 0x0280 (size: 0x18)
    float ReturnValue;                                                                // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FLerpDataflowNode : public FDataflowNode
{
    float A;                                                                          // 0x0280 (size: 0x4)
    float B;                                                                          // 0x0284 (size: 0x4)
    float Alpha;                                                                      // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FLogDataflowNode : public FDataflowNode
{
    float base;                                                                       // 0x0280 (size: 0x4)
    float A;                                                                          // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FLogStringDataflowNode : public FDataflowNode
{
    bool bPrintToLog;                                                                 // 0x0280 (size: 0x1)
    FString String;                                                                   // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FLogeDataflowNode : public FDataflowNode
{
    float A;                                                                          // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FMakeBoxDataflowNode : public FDataflowNode
{
    EMakeBoxDataTypeEnum DataType;                                                    // 0x0280 (size: 0x1)
    FVector min;                                                                      // 0x0288 (size: 0x18)
    FVector max;                                                                      // 0x02A0 (size: 0x18)
    FVector Center;                                                                   // 0x02B8 (size: 0x18)
    FVector Size;                                                                     // 0x02D0 (size: 0x18)
    FBox Box;                                                                         // 0x02E8 (size: 0x38)

}; // Size: 0x320

struct FMakeBoxMeshDataflowNode : public FDataflowNode
{
    FVector Center;                                                                   // 0x0280 (size: 0x18)
    FVector Size;                                                                     // 0x0298 (size: 0x18)
    int32 SubdivisionsX;                                                              // 0x02B0 (size: 0x4)
    int32 SubdivisionsY;                                                              // 0x02B4 (size: 0x4)
    int32 SubdivisionsZ;                                                              // 0x02B8 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02C0 (size: 0x8)

}; // Size: 0x2C8

struct FMakeCapsuleMeshDataflowNode : public FDataflowNode
{
    float Radius;                                                                     // 0x0280 (size: 0x4)
    float SegmentLength;                                                              // 0x0284 (size: 0x4)
    int32 NumHemisphereArcSteps;                                                      // 0x0288 (size: 0x4)
    int32 NumCircleSteps;                                                             // 0x028C (size: 0x4)
    int32 NumSegmentSteps;                                                            // 0x0290 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FMakeCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)

}; // Size: 0x330

struct FMakeCylinderMeshDataflowNode : public FDataflowNode
{
    float Radius1;                                                                    // 0x0280 (size: 0x4)
    float Radius2;                                                                    // 0x0284 (size: 0x4)
    float Height;                                                                     // 0x0288 (size: 0x4)
    int32 LengthSamples;                                                              // 0x028C (size: 0x4)
    int32 AngleSamples;                                                               // 0x0290 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FMakeDataflowConvexDecompositionSettingsNode : public FDataflowNode
{
    float MinSizeToDecompose;                                                         // 0x0280 (size: 0x4)
    float MaxGeoToHullVolumeRatioToDecompose;                                         // 0x0284 (size: 0x4)
    float ErrorTolerance;                                                             // 0x0288 (size: 0x4)
    int32 MaxHullsPerGeometry;                                                        // 0x028C (size: 0x4)
    float MinThicknessTolerance;                                                      // 0x0290 (size: 0x4)
    int32 NumAdditionalSplits;                                                        // 0x0294 (size: 0x4)
    bool bProtectNegativeSpace;                                                       // 0x0298 (size: 0x1)
    bool bOnlyConnectedToHull;                                                        // 0x0299 (size: 0x1)
    float NegativeSpaceTolerance;                                                     // 0x029C (size: 0x4)
    float NegativeSpaceMinRadius;                                                     // 0x02A0 (size: 0x4)
    FDataflowConvexDecompositionSettings DecompositionSettings;                       // 0x02A4 (size: 0x24)

}; // Size: 0x2C8

struct FMakeDataflowMeshDataflowNode : public FDataflowNode
{
    class UDynamicMesh* InMesh;                                                       // 0x0280 (size: 0x8)
    TArray<UMaterialInterface*> InMaterials;                                          // 0x0288 (size: 0x10)
    class UDataflowMesh* Mesh;                                                        // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FMakeDiscMeshDataflowNode : public FDataflowNode
{
    float Radius;                                                                     // 0x0280 (size: 0x4)
    FVector Normal;                                                                   // 0x0288 (size: 0x18)
    int32 AngleSamples;                                                               // 0x02A0 (size: 0x4)
    int32 RadialSamples;                                                              // 0x02A4 (size: 0x4)
    float StartAngle;                                                                 // 0x02A8 (size: 0x4)
    float EndAngle;                                                                   // 0x02AC (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02B0 (size: 0x8)

}; // Size: 0x2B8

struct FMakeFloatArrayDataflowNode : public FDataflowNode
{
    int32 NumElements;                                                                // 0x0280 (size: 0x4)
    float Value;                                                                      // 0x0284 (size: 0x4)
    TArray<float> FloatArray;                                                         // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FMakeLiteralBoolDataflowNode : public FDataflowNode
{
    bool Value;                                                                       // 0x0280 (size: 0x1)
    bool bool;                                                                        // 0x0281 (size: 0x1)

}; // Size: 0x288

struct FMakeLiteralBoolDataflowNode_v2 : public FDataflowNode
{
    bool bool;                                                                        // 0x0280 (size: 0x1)

}; // Size: 0x288

struct FMakeLiteralDoubleDataflowNode : public FDataflowNode
{
    double Double;                                                                    // 0x0280 (size: 0x8)

}; // Size: 0x288

struct FMakeLiteralFloatDataflowNode : public FDataflowNode
{
    float Value;                                                                      // 0x0280 (size: 0x4)
    float float;                                                                      // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FMakeLiteralFloatDataflowNode_v2 : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)

}; // Size: 0x288

struct FMakeLiteralIntDataflowNode : public FDataflowNode
{
    int32 Value;                                                                      // 0x0280 (size: 0x4)
    int32 int;                                                                        // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FMakeLiteralIntDataflowNode_v2 : public FDataflowNode
{
    int32 int;                                                                        // 0x0280 (size: 0x4)

}; // Size: 0x288

struct FMakeLiteralStringDataflowNode : public FDataflowNode
{
    FString Value;                                                                    // 0x0280 (size: 0x10)
    FString String;                                                                   // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FMakeLiteralStringDataflowNode_v2 : public FDataflowNode
{
    FString String;                                                                   // 0x0280 (size: 0x10)

}; // Size: 0x290

struct FMakeLiteralVectorDataflowNode : public FDataflowNode
{
    float X;                                                                          // 0x0280 (size: 0x4)
    float Y;                                                                          // 0x0284 (size: 0x4)
    float Z;                                                                          // 0x0288 (size: 0x4)
    FVector Vector;                                                                   // 0x0290 (size: 0x18)

}; // Size: 0x2A8

struct FMakeMaterialDataflowNode : public FDataflowNode
{
    class UMaterial* InMaterial;                                                      // 0x0280 (size: 0x8)
    class UMaterial* Material;                                                        // 0x0288 (size: 0x8)

}; // Size: 0x290

struct FMakeMaterialInterfaceArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0280 (size: 0x10)

}; // Size: 0x290

struct FMakeMaterialsArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterial*> Materials;                                                     // 0x0280 (size: 0x10)

}; // Size: 0x290

struct FMakePlaneDataflowNode : public FDataflowNode
{
    FVector BasePoint;                                                                // 0x0280 (size: 0x18)
    FVector Normal;                                                                   // 0x0298 (size: 0x18)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x02B0 (size: 0x18)
    float PlaneSizeMultiplier;                                                        // 0x02C8 (size: 0x4)
    FPlane Plane;                                                                     // 0x02D0 (size: 0x20)

}; // Size: 0x2F0

struct FMakePointsDataflowNode : public FDataflowNode
{
    TArray<FVector> Point;                                                            // 0x0280 (size: 0x10)
    TArray<FVector> Points;                                                           // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FMakeQuaternionDataflowNode : public FDataflowNode
{
    float X;                                                                          // 0x0280 (size: 0x4)
    float Y;                                                                          // 0x0284 (size: 0x4)
    float Z;                                                                          // 0x0288 (size: 0x4)
    float W;                                                                          // 0x028C (size: 0x4)
    FQuat Quaternion;                                                                 // 0x0290 (size: 0x20)

}; // Size: 0x2B0

struct FMakeRectangleMeshDataflowNode : public FDataflowNode
{
    FVector Origin;                                                                   // 0x0280 (size: 0x18)
    FVector Normal;                                                                   // 0x0298 (size: 0x18)
    float Width;                                                                      // 0x02B0 (size: 0x4)
    float Height;                                                                     // 0x02B4 (size: 0x4)
    int32 WidthVertexCount;                                                           // 0x02B8 (size: 0x4)
    int32 HeightVertexCount;                                                          // 0x02BC (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02C0 (size: 0x8)

}; // Size: 0x2C8

struct FMakeRootProxyMeshArrayDataflowNode : public FDataflowNode
{
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x0280 (size: 0x10)

}; // Size: 0x290

struct FMakeRootProxyMeshDataflowNode : public FDataflowNode
{
    class UStaticMesh* Mesh;                                                          // 0x0280 (size: 0x8)
    FTransform Transform;                                                             // 0x0290 (size: 0x60)
    FDataflowRootProxyMesh RootProxyMesh;                                             // 0x02F0 (size: 0x70)

}; // Size: 0x360

struct FMakeRotatorDataflowNode : public FDataflowNode
{
    float Pitch;                                                                      // 0x0280 (size: 0x4)
    float Yaw;                                                                        // 0x0284 (size: 0x4)
    float Roll;                                                                       // 0x0288 (size: 0x4)
    FRotator Rotator;                                                                 // 0x0290 (size: 0x18)

}; // Size: 0x2A8

struct FMakeSphereDataflowNode : public FDataflowNode
{
    FVector Center;                                                                   // 0x0280 (size: 0x18)
    float Radius;                                                                     // 0x0298 (size: 0x4)
    FSphere Sphere;                                                                   // 0x02A0 (size: 0x20)

}; // Size: 0x2C0

struct FMakeSphereMeshDataflowNode : public FDataflowNode
{
    float Radius;                                                                     // 0x0280 (size: 0x4)
    int32 NumPhi;                                                                     // 0x0284 (size: 0x4)
    int32 NumTheta;                                                                   // 0x0288 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FMakeStairMeshDataflowNode : public FDataflowNode
{
    EDataflowStairTypeEnum StairType;                                                 // 0x0280 (size: 0x1)
    int32 NumSteps;                                                                   // 0x0284 (size: 0x4)
    float StepWidth;                                                                  // 0x0288 (size: 0x4)
    float StepHeight;                                                                 // 0x028C (size: 0x4)
    float StepDepth;                                                                  // 0x0290 (size: 0x4)
    float CurveAngle;                                                                 // 0x0294 (size: 0x4)
    float InnerRadius;                                                                // 0x0298 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02A0 (size: 0x8)

}; // Size: 0x2A8

struct FMakeTorusMeshDataflowNode : public FDataflowNode
{
    FVector Origin;                                                                   // 0x0280 (size: 0x18)
    float Radius1;                                                                    // 0x0298 (size: 0x4)
    int32 ProfileVertexCount;                                                         // 0x029C (size: 0x4)
    float Radius2;                                                                    // 0x02A0 (size: 0x4)
    int32 SweepVertexCount;                                                           // 0x02A4 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02A8 (size: 0x8)

}; // Size: 0x2B0

struct FMakeTransformDataflowNode : public FDataflowNode
{
    FVector InTranslation;                                                            // 0x0280 (size: 0x18)
    FVector InRotation;                                                               // 0x0298 (size: 0x18)
    FVector InScale;                                                                  // 0x02B0 (size: 0x18)
    FTransform OutTransform;                                                          // 0x02D0 (size: 0x60)

}; // Size: 0x330

struct FMakeTransformDataflowNode_v2 : public FDataflowNode
{
    FVector Translation;                                                              // 0x0280 (size: 0x18)
    FVector Rotation;                                                                 // 0x0298 (size: 0x18)
    FRotator Rotator;                                                                 // 0x02B0 (size: 0x18)
    FQuat Quat;                                                                       // 0x02D0 (size: 0x20)
    FVector Scale;                                                                    // 0x02F0 (size: 0x18)
    FTransform Transform;                                                             // 0x0310 (size: 0x60)

}; // Size: 0x370

struct FMaterialInterfaceTextureOverrideDataflowNode : public FDataflowNode
{
    class UMaterialInterface* Material;                                               // 0x0280 (size: 0x8)
    class UTexture2D* TargetTexture;                                                  // 0x0288 (size: 0x8)
    class UTexture2D* OverrideTexture;                                                // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FMaterialsInfoDataflowNode : public FDataflowNode
{
    TArray<UMaterial*> Materials;                                                     // 0x0280 (size: 0x10)
    FString String;                                                                   // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FMathConstantsDataflowNode : public FDataflowNode
{
    EMathConstantsEnum Constant;                                                      // 0x0280 (size: 0x1)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FMathExpressionDataflowNode : public FDataflowNode
{
    FDataflowNumericTypes A;                                                          // 0x0280 (size: 0x8)
    FDataflowNumericTypes B;                                                          // 0x0288 (size: 0x8)
    FDataflowNumericTypes C;                                                          // 0x0290 (size: 0x8)
    FDataflowNumericTypes D;                                                          // 0x0298 (size: 0x8)
    FString Expression;                                                               // 0x02A0 (size: 0x10)
    FDataflowNumericTypes ReturnValue;                                                // 0x02B0 (size: 0x8)

}; // Size: 0x2B8

struct FMax3DataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float FloatC;                                                                     // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FMaxDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FMaxSettings
{
    float MaxAttrValue;                                                               // 0x0000 (size: 0x4)
    FLinearColor MaxColor;                                                            // 0x0004 (size: 0x10)

}; // Size: 0x14

struct FMergeConvexHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowSphereCovering SphereCovering;                                           // 0x0330 (size: 0x20)
    int32 MaxConvexCount;                                                             // 0x0350 (size: 0x4)
    double ErrorTolerance;                                                            // 0x0358 (size: 0x8)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0360 (size: 0x28)
    EConvexHullProximityFilter MergeProximityFilter;                                  // 0x0388 (size: 0x1)
    float MergeProximityDistanceThreshold;                                            // 0x038C (size: 0x4)
    bool bProtectNegativeSpace;                                                       // 0x0390 (size: 0x1)
    bool bComputeNegativeSpacePerBone;                                                // 0x0391 (size: 0x1)
    ENegativeSpaceSampleMethodDataflowEnum SampleMethod;                              // 0x0392 (size: 0x1)
    bool bRequireSearchSampleCoverage;                                                // 0x0393 (size: 0x1)
    bool bOnlyConnectedToHull;                                                        // 0x0394 (size: 0x1)
    int32 TargetNumSamples;                                                           // 0x0398 (size: 0x4)
    double MinSampleSpacing;                                                          // 0x03A0 (size: 0x8)
    double NegativeSpaceTolerance;                                                    // 0x03A8 (size: 0x8)
    double MinRadius;                                                                 // 0x03B0 (size: 0x8)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x03B8 (size: 0x18)
    bool bRandomizeColor;                                                             // 0x03D0 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x03D4 (size: 0x4)
    FDataflowNodeSphereCoveringDebugDrawSettings SphereCoveringDebugDrawRenderSettings; // 0x03D8 (size: 0x40)

}; // Size: 0x418

struct FMergeInCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FMergeUVIslandsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowFaceSelection FaceSelection;                                             // 0x0330 (size: 0x28)
    int32 UVChannel;                                                                  // 0x0358 (size: 0x4)
    double AreaDistortionThreshold;                                                   // 0x0360 (size: 0x8)
    double MaxNormalDeviationDeg;                                                     // 0x0368 (size: 0x8)
    int32 NormalSmoothingRounds;                                                      // 0x0370 (size: 0x4)
    double NormalSmoothingAlpha;                                                      // 0x0378 (size: 0x8)

}; // Size: 0x380

struct FMeshAppendDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh1;                                                        // 0x0280 (size: 0x8)
    class UDynamicMesh* Mesh2;                                                        // 0x0288 (size: 0x8)
    class UDynamicMesh* Mesh;                                                         // 0x0290 (size: 0x8)

}; // Size: 0x298

struct FMeshCopyToPointsDataflowNode : public FDataflowNode
{
    TArray<FVector> Points;                                                           // 0x0280 (size: 0x10)
    class UDynamicMesh* MeshToCopy;                                                   // 0x0290 (size: 0x8)
    float Scale;                                                                      // 0x0298 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02A0 (size: 0x8)
    TArray<UDynamicMesh*> Meshes;                                                     // 0x02A8 (size: 0x10)

}; // Size: 0x2B8

struct FMeshCutterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)
    FDataflowTransformSelection TransformSelection;                                   // 0x0368 (size: 0x28)
    FTransform Transform;                                                             // 0x0390 (size: 0x60)
    TArray<UDynamicMesh*> CuttingDynamicMeshes;                                       // 0x03F0 (size: 0x10)
    class UStaticMesh* CuttingStaticMesh;                                             // 0x0400 (size: 0x8)
    bool bUseHiRes;                                                                   // 0x0408 (size: 0x1)
    int32 LODLevel;                                                                   // 0x040C (size: 0x4)
    EMeshCutterCutDistribution CutDistribution;                                       // 0x0410 (size: 0x1)
    EMeshCutterPerCutMeshSelection PerCutMeshSelection;                               // 0x0411 (size: 0x1)
    int32 NumberToScatter;                                                            // 0x0414 (size: 0x4)
    int32 GridX;                                                                      // 0x0418 (size: 0x4)
    int32 GridY;                                                                      // 0x041C (size: 0x4)
    int32 GridZ;                                                                      // 0x0420 (size: 0x4)
    float Variability;                                                                // 0x0424 (size: 0x4)
    float MinScaleFactor;                                                             // 0x0428 (size: 0x4)
    float MaxScaleFactor;                                                             // 0x042C (size: 0x4)
    bool bRandomOrientation;                                                          // 0x0430 (size: 0x1)
    float RollRange;                                                                  // 0x0434 (size: 0x4)
    float PitchRange;                                                                 // 0x0438 (size: 0x4)
    float YawRange;                                                                   // 0x043C (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0440 (size: 0x4)
    float ChanceToFracture;                                                           // 0x0444 (size: 0x4)
    bool SplitIslands;                                                                // 0x0448 (size: 0x1)
    bool AddSamplesForCollision;                                                      // 0x0449 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x044C (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x0450 (size: 0x28)

}; // Size: 0x480

struct FMeshInfoDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    FString InfoString;                                                               // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FMeshProcessorDataflowNodeBase : public FDataflowNode
{
    TSubclassOf<class UDynamicMeshProcessorBlueprint> MeshProcessor;                  // 0x0280 (size: 0x8)
    class UDynamicMeshProcessorBlueprint* MeshProcessorInstance;                      // 0x0288 (size: 0x8)
    class UObject* OwningObject;                                                      // 0x0298 (size: 0x8)

}; // Size: 0x2A0

struct FMeshToCollectionDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    bool bSplitIslands;                                                               // 0x0338 (size: 0x1)
    bool bConnectIslandsByVertexOverlap;                                              // 0x0339 (size: 0x1)
    float ConnectVerticesThreshold;                                                   // 0x033C (size: 0x4)
    bool bAddClusterRootForSingleMesh;                                                // 0x0340 (size: 0x1)

}; // Size: 0x348

struct FMeshToOBJStringDebugDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    bool bInvertFaces;                                                                // 0x0288 (size: 0x1)
    FString StringOBJ;                                                                // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FMin3DataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float FloatC;                                                                     // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FMinDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FMinSettings
{
    float MinAttrValue;                                                               // 0x0000 (size: 0x4)
    FLinearColor MinColor;                                                            // 0x0004 (size: 0x10)

}; // Size: 0x14

struct FMultiplyDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FMultiplyTransformDataflowNode : public FDataflowNode
{
    FTransform InLeftTransform;                                                       // 0x0280 (size: 0x60)
    FTransform InRightTransform;                                                      // 0x02E0 (size: 0x60)
    FTransform OutTransform;                                                          // 0x0340 (size: 0x60)

}; // Size: 0x3A0

struct FNegateDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FNoiseFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float MinRange;                                                                   // 0x02B8 (size: 0x4)
    float MaxRange;                                                                   // 0x02BC (size: 0x4)
    FTransform Transform;                                                             // 0x02C0 (size: 0x60)
    TArray<float> FieldFloatResult;                                                   // 0x0320 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0330 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x0340 (size: 0x4)

}; // Size: 0x350

struct FNonUniformPointSamplingDataflowNode : public FDataflowNode
{
    class UDynamicMesh* TargetMesh;                                                   // 0x0280 (size: 0x8)
    float SamplingRadius;                                                             // 0x0288 (size: 0x4)
    int32 MaxNumSamples;                                                              // 0x028C (size: 0x4)
    float SubSampleDensity;                                                           // 0x0290 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0294 (size: 0x4)
    float MaxSamplingRadius;                                                          // 0x0298 (size: 0x4)
    ENonUniformSamplingDistributionMode SizeDistribution;                             // 0x029C (size: 0x1)
    float SizeDistributionPower;                                                      // 0x02A0 (size: 0x4)
    TArray<FVector> SamplePoints;                                                     // 0x02A8 (size: 0x10)
    TArray<float> SampleRadii;                                                        // 0x02B8 (size: 0x10)
    TArray<int32> SampleTriangleIDs;                                                  // 0x02C8 (size: 0x10)
    TArray<FVector> SampleBarycentricCoords;                                          // 0x02D8 (size: 0x10)
    int32 NumSamplePoints;                                                            // 0x02E8 (size: 0x4)

}; // Size: 0x2F0

struct FNormalizeDataflowNode : public FDataflowNode
{
    FVector VectorA;                                                                  // 0x0280 (size: 0x18)
    float Tolerance;                                                                  // 0x0298 (size: 0x4)
    FVector ReturnValue;                                                              // 0x02A0 (size: 0x18)

}; // Size: 0x2B8

struct FNormalizeToRangeDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float RangeMin;                                                                   // 0x0284 (size: 0x4)
    float RangeMax;                                                                   // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FOneMinusDataflowNode : public FDataflowNode
{
    float A;                                                                          // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FPlaneCutterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)
    FDataflowTransformSelection TransformSelection;                                   // 0x0368 (size: 0x28)
    int32 NumPlanes;                                                                  // 0x0390 (size: 0x4)
    float RandomSeed;                                                                 // 0x0394 (size: 0x4)
    float Grout;                                                                      // 0x0398 (size: 0x4)
    float Amplitude;                                                                  // 0x039C (size: 0x4)
    float Frequency;                                                                  // 0x03A0 (size: 0x4)
    float Persistence;                                                                // 0x03A4 (size: 0x4)
    float Lacunarity;                                                                 // 0x03A8 (size: 0x4)
    int32 OctaveNumber;                                                               // 0x03AC (size: 0x4)
    float PointSpacing;                                                               // 0x03B0 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x03B4 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x03B8 (size: 0x4)

}; // Size: 0x3C0

struct FPlaneCutterDataflowNode_v2 : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)
    FDataflowTransformSelection TransformSelection;                                   // 0x0368 (size: 0x28)
    FTransform Transform;                                                             // 0x0390 (size: 0x60)
    int32 NumPlanes;                                                                  // 0x03F0 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x03F4 (size: 0x4)
    float ChanceToFracture;                                                           // 0x03F8 (size: 0x4)
    bool SplitIslands;                                                                // 0x03FC (size: 0x1)
    float Grout;                                                                      // 0x0400 (size: 0x4)
    float Amplitude;                                                                  // 0x0404 (size: 0x4)
    float Frequency;                                                                  // 0x0408 (size: 0x4)
    float Persistence;                                                                // 0x040C (size: 0x4)
    float Lacunarity;                                                                 // 0x0410 (size: 0x4)
    int32 OctaveNumber;                                                               // 0x0414 (size: 0x4)
    float PointSpacing;                                                               // 0x0418 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x041C (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x0420 (size: 0x4)
    EDataflowDebugDrawRenderType RenderType;                                          // 0x0424 (size: 0x1)
    float PlaneSizeMultiplier;                                                        // 0x0428 (size: 0x4)
    bool bTranslucent;                                                                // 0x042C (size: 0x1)
    bool bRandomizeColors;                                                            // 0x042D (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x0430 (size: 0x4)
    float LineWidthMultiplier;                                                        // 0x0434 (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x0438 (size: 0x28)

}; // Size: 0x460

struct FPlaneFalloffFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    FVector Position;                                                                 // 0x02B8 (size: 0x18)
    FVector Normal;                                                                   // 0x02D0 (size: 0x18)
    float Distance;                                                                   // 0x02E8 (size: 0x4)
    FVector Translation;                                                              // 0x02F0 (size: 0x18)
    float Magnitude;                                                                  // 0x0308 (size: 0x4)
    float MinRange;                                                                   // 0x030C (size: 0x4)
    float MaxRange;                                                                   // 0x0310 (size: 0x4)
    float Default;                                                                    // 0x0314 (size: 0x4)
    EDataflowFieldFalloffType FalloffType;                                            // 0x0318 (size: 0x1)
    TArray<float> FieldFloatResult;                                                   // 0x0320 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0330 (size: 0x10)
    FDataflowVertexSelection FieldSelectionMask;                                      // 0x0340 (size: 0x28)
    int32 NumSamplePositions;                                                         // 0x0368 (size: 0x4)

}; // Size: 0x370

struct FPointsToCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<FVector> Points;                                                           // 0x0330 (size: 0x10)

}; // Size: 0x340

struct FPointsToMeshDataflowNode : public FDataflowNode
{
    TArray<FVector> Points;                                                           // 0x0280 (size: 0x10)
    class UDynamicMesh* Mesh;                                                         // 0x0290 (size: 0x8)
    int32 TriangleCount;                                                              // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FPowDataflowNode : public FDataflowNode
{
    float base;                                                                       // 0x0280 (size: 0x4)
    float exp;                                                                        // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FPrintStringDataflowNode : public FDataflowNode
{
    bool bPrintToScreen;                                                              // 0x0280 (size: 0x1)
    bool bPrintToLog;                                                                 // 0x0281 (size: 0x1)
    FColor Color;                                                                     // 0x0284 (size: 0x4)
    float Duration;                                                                   // 0x0288 (size: 0x4)
    FString String;                                                                   // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FProximityDataflowNode : public FDataflowNode
{
    EProximityMethodEnum ProximityMethod;                                             // 0x0280 (size: 0x1)
    float DistanceThreshold;                                                          // 0x0284 (size: 0x4)
    float ContactThreshold;                                                           // 0x0288 (size: 0x4)
    EProximityContactFilteringMethodEnum FilterContactMethod;                         // 0x028C (size: 0x1)
    bool bUseAsConnectionGraph;                                                       // 0x028D (size: 0x1)
    EConnectionContactAreaMethodEnum ContactAreaMethod;                               // 0x028E (size: 0x1)
    bool bRecomputeConvexHulls;                                                       // 0x028F (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0290 (size: 0xB0)
    FLinearColor Color;                                                               // 0x0340 (size: 0x10)
    float LineWidthMultiplier;                                                        // 0x0350 (size: 0x4)
    FLinearColor CenterColor;                                                         // 0x0354 (size: 0x10)
    float CenterSize;                                                                 // 0x0364 (size: 0x4)
    bool bRandomizeColor;                                                             // 0x0368 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x036C (size: 0x4)

}; // Size: 0x370

struct FPruneInCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)

}; // Size: 0x358

struct FRadialFalloffFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    FSphere Sphere;                                                                   // 0x02B8 (size: 0x20)
    FVector Translation;                                                              // 0x02D8 (size: 0x18)
    float Magnitude;                                                                  // 0x02F0 (size: 0x4)
    float MinRange;                                                                   // 0x02F4 (size: 0x4)
    float MaxRange;                                                                   // 0x02F8 (size: 0x4)
    float Default;                                                                    // 0x02FC (size: 0x4)
    EDataflowFieldFalloffType FalloffType;                                            // 0x0300 (size: 0x1)
    TArray<float> FieldFloatResult;                                                   // 0x0308 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0318 (size: 0x10)
    FDataflowVertexSelection FieldSelectionMask;                                      // 0x0328 (size: 0x28)
    int32 NumSamplePositions;                                                         // 0x0350 (size: 0x4)

}; // Size: 0x358

struct FRadialIntMaskFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    FSphere Sphere;                                                                   // 0x02B8 (size: 0x20)
    FVector Translation;                                                              // 0x02D8 (size: 0x18)
    int32 InteriorValue;                                                              // 0x02F0 (size: 0x4)
    int32 ExteriorValue;                                                              // 0x02F4 (size: 0x4)
    EDataflowSetMaskConditionType SetMaskConditionType;                               // 0x02F8 (size: 0x1)
    TArray<int32> FieldIntResult;                                                     // 0x0300 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0310 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x0320 (size: 0x4)

}; // Size: 0x328

struct FRadialScatterPointsDataflowNode : public FDataflowNode
{
    FVector Center;                                                                   // 0x0280 (size: 0x18)
    FVector Normal;                                                                   // 0x0298 (size: 0x18)
    float Radius;                                                                     // 0x02B0 (size: 0x4)
    int32 AngularSteps;                                                               // 0x02B4 (size: 0x4)
    int32 RadialSteps;                                                                // 0x02B8 (size: 0x4)
    float AngleOffset;                                                                // 0x02BC (size: 0x4)
    float Variability;                                                                // 0x02C0 (size: 0x4)
    float RandomSeed;                                                                 // 0x02C4 (size: 0x4)
    TArray<FVector> Points;                                                           // 0x02C8 (size: 0x10)

}; // Size: 0x2D8

struct FRadialScatterPointsDataflowNode_v2 : public FDataflowNode
{
    FBox BoundingBox;                                                                 // 0x0280 (size: 0x38)
    FVector Center;                                                                   // 0x02B8 (size: 0x18)
    FVector Normal;                                                                   // 0x02D0 (size: 0x18)
    int32 RandomSeed;                                                                 // 0x02E8 (size: 0x4)
    int32 AngularSteps;                                                               // 0x02EC (size: 0x4)
    float AngleOffset;                                                                // 0x02F0 (size: 0x4)
    float AngularNoise;                                                               // 0x02F4 (size: 0x4)
    float Radius;                                                                     // 0x02F8 (size: 0x4)
    int32 RadialSteps;                                                                // 0x02FC (size: 0x4)
    float RadialStepExponent;                                                         // 0x0300 (size: 0x4)
    float RadialMinStep;                                                              // 0x0304 (size: 0x4)
    float RadialNoise;                                                                // 0x0308 (size: 0x4)
    float RadialVariability;                                                          // 0x030C (size: 0x4)
    float AngularVariability;                                                         // 0x0310 (size: 0x4)
    float AxialVariability;                                                           // 0x0314 (size: 0x4)
    TArray<FVector> Points;                                                           // 0x0318 (size: 0x10)

}; // Size: 0x328

struct FRadialVectorFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    FVector Position;                                                                 // 0x02C0 (size: 0x18)
    TArray<FVector> FieldVectorResult;                                                // 0x02D8 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02E8 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02F8 (size: 0x4)

}; // Size: 0x300

struct FRadiansToDegreesDataflowNode : public FDataflowNode
{
    float Radians;                                                                    // 0x0280 (size: 0x4)
    float Degrees;                                                                    // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FRandomFloatDataflowNode : public FDataflowNode
{
    bool bDeterministic;                                                              // 0x0280 (size: 0x1)
    float RandomSeed;                                                                 // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FRandomFloatInRangeDataflowNode : public FDataflowNode
{
    bool bDeterministic;                                                              // 0x0280 (size: 0x1)
    float RandomSeed;                                                                 // 0x0284 (size: 0x4)
    float min;                                                                        // 0x0288 (size: 0x4)
    float max;                                                                        // 0x028C (size: 0x4)
    float ReturnValue;                                                                // 0x0290 (size: 0x4)

}; // Size: 0x298

struct FRandomUnitVectorDataflowNode : public FDataflowNode
{
    bool bDeterministic;                                                              // 0x0280 (size: 0x1)
    float RandomSeed;                                                                 // 0x0284 (size: 0x4)
    FVector ReturnValue;                                                              // 0x0288 (size: 0x18)

}; // Size: 0x2A0

struct FRandomUnitVectorInConeDataflowNode : public FDataflowNode
{
    bool bDeterministic;                                                              // 0x0280 (size: 0x1)
    float RandomSeed;                                                                 // 0x0284 (size: 0x4)
    FVector ConeDirection;                                                            // 0x0288 (size: 0x18)
    float ConeHalfAngle;                                                              // 0x02A0 (size: 0x4)
    FVector ReturnValue;                                                              // 0x02A8 (size: 0x18)

}; // Size: 0x2C0

struct FRandomVectorFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    TArray<FVector> FieldVectorResult;                                                // 0x02C0 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02D0 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02E0 (size: 0x4)

}; // Size: 0x2E8

struct FRandomizeFloatArrayDataflowNode : public FDataflowNode
{
    TArray<float> FloatArray;                                                         // 0x0280 (size: 0x10)
    float RandomRangeMin;                                                             // 0x0290 (size: 0x4)
    float RandomRangeMax;                                                             // 0x0294 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FReAssignMaterialInCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowFaceSelection FaceSelection;                                             // 0x0330 (size: 0x28)
    TArray<UMaterial*> Materials;                                                     // 0x0358 (size: 0x10)
    int32 OutsideMaterialIdx;                                                         // 0x0368 (size: 0x4)
    int32 InsideMaterialIdx;                                                          // 0x036C (size: 0x4)
    bool bAssignOutsideMaterial;                                                      // 0x0370 (size: 0x1)
    bool bAssignInsideMaterial;                                                       // 0x0371 (size: 0x1)

}; // Size: 0x378

struct FRecomputeNormalsInGeometryCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    bool bOnlyTangents;                                                               // 0x0358 (size: 0x1)
    bool bRecomputeSharpEdges;                                                        // 0x0359 (size: 0x1)
    float SharpEdgeAngleThreshold;                                                    // 0x035C (size: 0x4)
    bool bOnlyInternalSurfaces;                                                       // 0x0360 (size: 0x1)

}; // Size: 0x368

struct FRemoveFloatArrayElementDataflowNode : public FDataflowNode
{
    int32 Index;                                                                      // 0x0280 (size: 0x4)
    bool bPreserveOrder;                                                              // 0x0284 (size: 0x1)
    TArray<float> FloatArray;                                                         // 0x0288 (size: 0x10)

}; // Size: 0x298

struct FRemoveOnBreakDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    bool bEnabledRemoval;                                                             // 0x0358 (size: 0x1)
    FVector2f PostBreakTimer;                                                         // 0x035C (size: 0x8)
    FVector2f RemovalTimer;                                                           // 0x0364 (size: 0x8)
    bool bClusterCrumbling;                                                           // 0x036C (size: 0x1)

}; // Size: 0x370

struct FResampleGeometryCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    bool AddSamplesForCollision;                                                      // 0x0358 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x035C (size: 0x4)

}; // Size: 0x360

struct FRoundDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FSafeDivideDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FSafeReciprocalDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FScaleVectorDataflowNode : public FDataflowNode
{
    FVector Vector;                                                                   // 0x0280 (size: 0x18)
    float Scale;                                                                      // 0x0298 (size: 0x4)
    FVector ScaledVector;                                                             // 0x02A0 (size: 0x18)

}; // Size: 0x2B8

struct FSelectFloatArrayIndicesInRangeDataflowNode : public FDataflowNode
{
    TArray<float> Values;                                                             // 0x0280 (size: 0x10)
    float min;                                                                        // 0x0290 (size: 0x4)
    float max;                                                                        // 0x0294 (size: 0x4)
    ERangeSettingEnum RangeSetting;                                                   // 0x0298 (size: 0x1)
    bool bInclusive;                                                                  // 0x0299 (size: 0x1)
    TArray<int32> Indices;                                                            // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

struct FSelectionToVertexListDataflowNode : public FDataflowNode
{
    FDataflowVertexSelection VertexSelection;                                         // 0x0280 (size: 0x28)
    TArray<int32> VertexList;                                                         // 0x02A8 (size: 0x10)

}; // Size: 0x2B8

struct FSetAnchorStateDataflowNode : public FDataflowNode
{
    EAnchorStateEnum AnchorState;                                                     // 0x0280 (size: 0x1)
    bool bSetNotSelectedBonesToOppositeState;                                         // 0x0281 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0338 (size: 0x28)

}; // Size: 0x360

struct FSetCollectionAttributeDataTypedDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FCollectionAttributeKey AttributeKey;                                             // 0x0330 (size: 0x20)
    EStandardGroupNameEnum GroupName;                                                 // 0x0350 (size: 0x1)
    FString CustomGroupName;                                                          // 0x0358 (size: 0x10)
    FString AttrName;                                                                 // 0x0368 (size: 0x10)
    TArray<bool> BoolAttributeData;                                                   // 0x0378 (size: 0x10)
    TArray<float> FloatAttributeData;                                                 // 0x0388 (size: 0x10)
    TArray<double> DoubleAttributeData;                                               // 0x0398 (size: 0x10)
    TArray<int32> Int32AttributeData;                                                 // 0x03A8 (size: 0x10)
    TArray<FString> StringAttributeData;                                              // 0x03B8 (size: 0x10)
    TArray<FVector3f> Vector3fAttributeData;                                          // 0x03C8 (size: 0x10)
    TArray<FVector3d> Vector3dAttributeData;                                          // 0x03D8 (size: 0x10)
    TArray<FLinearColor> LinearColorAttributeData;                                    // 0x03E8 (size: 0x10)

}; // Size: 0x3F8

struct FSetDynamicStateDataflowNode : public FDataflowNode
{
    EDataflowGeometryCollectionDynamicState DynamicState;                             // 0x0280 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0338 (size: 0x28)

}; // Size: 0x360

struct FSetFloatAttributeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FString Attribute;                                                                // 0x0330 (size: 0x10)
    EDataflowSetFloatArrayMethod Method;                                              // 0x0340 (size: 0x1)
    int32 RandomSeed;                                                                 // 0x0344 (size: 0x4)
    float NoiseScale;                                                                 // 0x0348 (size: 0x4)
    TArray<float> FloatArray;                                                         // 0x0350 (size: 0x10)

}; // Size: 0x360

struct FSetIntoMaterialInterfaceArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0280 (size: 0x10)
    class UMaterialInterface* Material;                                               // 0x0290 (size: 0x8)
    int32 Index;                                                                      // 0x0298 (size: 0x4)

}; // Size: 0x2A0

struct FSetMaterialInMaterialsArrayDataflowNode : public FDataflowNode
{
    TArray<UMaterial*> Materials;                                                     // 0x0280 (size: 0x10)
    class UMaterial* Material;                                                        // 0x0290 (size: 0x8)
    ESetMaterialOperationTypeEnum Operation;                                          // 0x0298 (size: 0x1)
    int32 MaterialIdx;                                                                // 0x029C (size: 0x4)

}; // Size: 0x2A0

struct FSetVertexColorFromFloatArrayDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<float> FloatArray;                                                         // 0x0330 (size: 0x10)
    bool bNormalizeInput;                                                             // 0x0340 (size: 0x1)
    FLinearColor Color;                                                               // 0x0344 (size: 0x10)

}; // Size: 0x358

struct FSetVertexColorFromVertexIndicesDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<int32> VertexIndicesIn;                                                    // 0x0330 (size: 0x10)
    FLinearColor SelectedColor;                                                       // 0x0340 (size: 0x10)

}; // Size: 0x350

struct FSetVertexColorFromVertexSelectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowVertexSelection VertexSelection;                                         // 0x0330 (size: 0x28)
    FLinearColor SelectedColor;                                                       // 0x0358 (size: 0x10)

}; // Size: 0x368

struct FSetVisibilityInCollectionDataflowNode : public FDataflowNode
{
    EVisibiltyOptionsEnum Visibility;                                                 // 0x0280 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0338 (size: 0x28)
    FDataflowFaceSelection FaceSelection;                                             // 0x0360 (size: 0x28)

}; // Size: 0x388

struct FSignDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FSimplifyConvexHullsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection OptionalSelectionFilter;                              // 0x0330 (size: 0x28)
    EConvexHullSimplifyMethod SimplifyMethod;                                         // 0x0358 (size: 0x4)
    float SimplificationAngleThreshold;                                               // 0x035C (size: 0x4)
    float SimplificationDistanceThreshold;                                            // 0x0360 (size: 0x4)
    int32 MinTargetTriangleCount;                                                     // 0x0364 (size: 0x4)
    bool bUseExistingVertices;                                                        // 0x0368 (size: 0x1)
    FDataflowNodeDebugDrawSettings DebugDrawRenderSettings;                           // 0x036C (size: 0x18)
    bool bRandomizeColor;                                                             // 0x0384 (size: 0x1)
    int32 ColorRandomSeed;                                                            // 0x0388 (size: 0x4)

}; // Size: 0x390

struct FSinDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FSkeletalMeshToCollectionDataflowNode : public FDataflowNode
{
    class USkeletalMesh* SkeletalMesh;                                                // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)
    bool bImportTransformOnly;                                                        // 0x0338 (size: 0x1)

}; // Size: 0x340

struct FSkeletonToCollectionDataflowNode : public FDataflowNode
{
    class USkeleton* Skeleton;                                                        // 0x0280 (size: 0x8)
    FManagedArrayCollection Collection;                                               // 0x0288 (size: 0xB0)

}; // Size: 0x338

struct FSliceCutterDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FBox BoundingBox;                                                                 // 0x0330 (size: 0x38)
    FDataflowTransformSelection TransformSelection;                                   // 0x0368 (size: 0x28)
    int32 SlicesX;                                                                    // 0x0390 (size: 0x4)
    int32 SlicesY;                                                                    // 0x0394 (size: 0x4)
    int32 SlicesZ;                                                                    // 0x0398 (size: 0x4)
    float SliceAngleVariation;                                                        // 0x039C (size: 0x4)
    float SliceOffsetVariation;                                                       // 0x03A0 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x03A4 (size: 0x4)
    float ChanceToFracture;                                                           // 0x03A8 (size: 0x4)
    bool SplitIslands;                                                                // 0x03AC (size: 0x1)
    float Grout;                                                                      // 0x03B0 (size: 0x4)
    float Amplitude;                                                                  // 0x03B4 (size: 0x4)
    float Frequency;                                                                  // 0x03B8 (size: 0x4)
    float Persistence;                                                                // 0x03BC (size: 0x4)
    float Lacunarity;                                                                 // 0x03C0 (size: 0x4)
    int32 OctaveNumber;                                                               // 0x03C4 (size: 0x4)
    float PointSpacing;                                                               // 0x03C8 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x03CC (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x03D0 (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x03D8 (size: 0x28)

}; // Size: 0x400

struct FSphereCoveringCountSpheresNode : public FDataflowNode
{
    FDataflowSphereCovering SphereCovering;                                           // 0x0280 (size: 0x20)
    int32 NumSpheres;                                                                 // 0x02A0 (size: 0x4)

}; // Size: 0x2A8

struct FSphereCoveringToMeshDataflowNode : public FDataflowNode
{
    FDataflowSphereCovering SphereCovering;                                           // 0x0280 (size: 0x20)
    int32 VerticesAlongEachSide;                                                      // 0x02A0 (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x02A8 (size: 0x8)

}; // Size: 0x2B0

struct FSpheresToPointsDataflowNode : public FDataflowNode
{
    TArray<FSphere> Spheres;                                                          // 0x0280 (size: 0x10)
    TArray<FVector> Points;                                                           // 0x0290 (size: 0x10)
    TArray<float> Radii;                                                              // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

struct FSplitDataflowMeshDataflowNode : public FDataflowNode
{
    class UDataflowMesh* InMesh;                                                      // 0x0280 (size: 0x8)
    class UDynamicMesh* Mesh;                                                         // 0x0288 (size: 0x8)
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FSplitMeshIslandsDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    TArray<UDynamicMesh*> Meshes;                                                     // 0x0288 (size: 0x10)
    EDataflowMeshSplitIslandsMethod SplitMethod;                                      // 0x0298 (size: 0x1)
    float ConnectVerticesThreshold;                                                   // 0x029C (size: 0x4)

}; // Size: 0x2A0

struct FSquareDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FSquareRootDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FStaticMeshToCollectionDataflowNode : public FDataflowNode
{
    class UStaticMesh* StaticMesh;                                                    // 0x0280 (size: 0x8)
    FTransform MeshTransform;                                                         // 0x0290 (size: 0x60)
    bool bSetInternalFromMaterialIndex;                                               // 0x02F0 (size: 0x1)
    bool bSplitComponents;                                                            // 0x02F1 (size: 0x1)
    FManagedArrayCollection Collection;                                               // 0x02F8 (size: 0xB0)
    TArray<UMaterial*> Materials;                                                     // 0x03A8 (size: 0x10)
    TArray<UMaterialInterface*> MaterialInstances;                                    // 0x03B8 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x03C8 (size: 0x10)

}; // Size: 0x3E0

struct FStaticMeshToCollectionDataflowNode_v2 : public FDataflowNode
{
    class UStaticMesh* StaticMesh;                                                    // 0x0280 (size: 0x8)
    FTransform MeshTransform;                                                         // 0x0290 (size: 0x60)
    FManagedArrayCollection Collection;                                               // 0x02F0 (size: 0xB0)
    TArray<UMaterialInterface*> Materials;                                            // 0x03A0 (size: 0x10)
    TArray<FGeometryCollectionAutoInstanceMesh> InstancedMeshes;                      // 0x03B0 (size: 0x10)
    TArray<FDataflowRootProxyMesh> RootProxyMeshes;                                   // 0x03C0 (size: 0x10)
    bool bSetInternalFromMaterialIndex;                                               // 0x03D0 (size: 0x1)
    bool bSplitComponents;                                                            // 0x03D1 (size: 0x1)

}; // Size: 0x3E0

struct FStaticMeshToMeshDataflowNode : public FDataflowNode
{
    class UStaticMesh* StaticMesh;                                                    // 0x0280 (size: 0x8)
    bool bUseHiRes;                                                                   // 0x0288 (size: 0x1)
    int32 LODLevel;                                                                   // 0x028C (size: 0x4)
    class UDynamicMesh* Mesh;                                                         // 0x0290 (size: 0x8)
    TArray<UMaterialInterface*> MaterialArray;                                        // 0x0298 (size: 0x10)

}; // Size: 0x2A8

struct FStringAppendDataflowNode : public FDataflowNode
{
    FString String1;                                                                  // 0x0280 (size: 0x10)
    FString String2;                                                                  // 0x0290 (size: 0x10)
    FString String;                                                                   // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

struct FStringAppendDataflowNode_v2 : public FDataflowNode
{
    TArray<FDataflowStringConvertibleTypes> Inputs;                                   // 0x0280 (size: 0x10)
    FString String;                                                                   // 0x0290 (size: 0x10)

}; // Size: 0x2A0

struct FSubtractDataflowNode : public FDataflowNode
{
    float FloatA;                                                                     // 0x0280 (size: 0x4)
    float FloatB;                                                                     // 0x0284 (size: 0x4)
    float ReturnValue;                                                                // 0x0288 (size: 0x4)

}; // Size: 0x290

struct FSumScalarFieldDataflowNode : public FDataflowNode
{
    TArray<float> FieldFloatLeft;                                                     // 0x0280 (size: 0x10)
    TArray<int32> FieldRemapLeft;                                                     // 0x0290 (size: 0x10)
    TArray<float> FieldFloatRight;                                                    // 0x02A0 (size: 0x10)
    TArray<int32> FieldRemapRight;                                                    // 0x02B0 (size: 0x10)
    float Magnitude;                                                                  // 0x02C0 (size: 0x4)
    EDataflowFloatFieldOperationType Operation;                                       // 0x02C4 (size: 0x1)
    bool bSwapInputs;                                                                 // 0x02C5 (size: 0x1)
    TArray<float> FieldFloatResult;                                                   // 0x02C8 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02D8 (size: 0x10)

}; // Size: 0x2E8

struct FSumVectorFieldDataflowNode : public FDataflowNode
{
    TArray<float> FieldFloat;                                                         // 0x0280 (size: 0x10)
    TArray<int32> FieldFloatRemap;                                                    // 0x0290 (size: 0x10)
    TArray<FVector> FieldVectorLeft;                                                  // 0x02A0 (size: 0x10)
    TArray<int32> FieldRemapLeft;                                                     // 0x02B0 (size: 0x10)
    TArray<FVector> FieldVectorRight;                                                 // 0x02C0 (size: 0x10)
    TArray<int32> FieldRemapRight;                                                    // 0x02D0 (size: 0x10)
    float Magnitude;                                                                  // 0x02E0 (size: 0x4)
    EDataflowVectorFieldOperationType Operation;                                      // 0x02E4 (size: 0x1)
    bool bSwapVectorInputs;                                                           // 0x02E5 (size: 0x1)
    TArray<FVector> FieldVectorResult;                                                // 0x02E8 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02F8 (size: 0x10)

}; // Size: 0x308

struct FTanDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FTransformCollectionAttributeDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FTransform TransformIn;                                                           // 0x0330 (size: 0x60)
    FTransform LocalTransform;                                                        // 0x0390 (size: 0x60)
    FString GroupName;                                                                // 0x03F0 (size: 0x10)
    FString AttributeName;                                                            // 0x0400 (size: 0x10)

}; // Size: 0x410

struct FTransformCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    FVector Translate;                                                                // 0x0358 (size: 0x18)
    ERotationOrderEnum RotationOrder;                                                 // 0x0370 (size: 0x1)
    FVector Rotate;                                                                   // 0x0378 (size: 0x18)
    FVector Scale;                                                                    // 0x0390 (size: 0x18)
    float UniformScale;                                                               // 0x03A8 (size: 0x4)
    FVector RotatePivot;                                                              // 0x03B0 (size: 0x18)
    FVector ScalePivot;                                                               // 0x03C8 (size: 0x18)
    bool bInvertTransformation;                                                       // 0x03E0 (size: 0x1)

}; // Size: 0x3E8

struct FTransformMeshDataflowNode : public FDataflowNode
{
    class UDynamicMesh* Mesh;                                                         // 0x0280 (size: 0x8)
    FVector Translate;                                                                // 0x0288 (size: 0x18)
    ERotationOrderEnum RotationOrder;                                                 // 0x02A0 (size: 0x1)
    FVector Rotate;                                                                   // 0x02A8 (size: 0x18)
    FVector Scale;                                                                    // 0x02C0 (size: 0x18)
    float UniformScale;                                                               // 0x02D8 (size: 0x4)
    FVector RotatePivot;                                                              // 0x02E0 (size: 0x18)
    FVector ScalePivot;                                                               // 0x02F8 (size: 0x18)
    bool bInvertTransformation;                                                       // 0x0310 (size: 0x1)

}; // Size: 0x318

struct FTransformPointsDataflowNode : public FDataflowNode
{
    TArray<FVector> Points;                                                           // 0x0280 (size: 0x10)
    FTransform Transform;                                                             // 0x0290 (size: 0x60)

}; // Size: 0x2F0

struct FTriangleBoundaryIndicesNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<int32> BoundaryIndicesOut;                                                 // 0x0330 (size: 0x10)

}; // Size: 0x340

struct FTruncDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float ReturnValue;                                                                // 0x0284 (size: 0x4)

}; // Size: 0x288

struct FUniformFractureDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    FDataflowTransformSelection TransformSelection;                                   // 0x0330 (size: 0x28)
    FTransform Transform;                                                             // 0x0360 (size: 0x60)
    int32 MinVoronoiSites;                                                            // 0x03C0 (size: 0x4)
    int32 MaxVoronoiSites;                                                            // 0x03C4 (size: 0x4)
    int32 InternalMaterialID;                                                         // 0x03C8 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x03CC (size: 0x4)
    float ChanceToFracture;                                                           // 0x03D0 (size: 0x4)
    bool GroupFracture;                                                               // 0x03D4 (size: 0x1)
    bool SplitIslands;                                                                // 0x03D5 (size: 0x1)
    float Grout;                                                                      // 0x03D8 (size: 0x4)
    float Amplitude;                                                                  // 0x03DC (size: 0x4)
    float Frequency;                                                                  // 0x03E0 (size: 0x4)
    float Persistence;                                                                // 0x03E4 (size: 0x4)
    float Lacunarity;                                                                 // 0x03E8 (size: 0x4)
    int32 OctaveNumber;                                                               // 0x03EC (size: 0x4)
    float PointSpacing;                                                               // 0x03F0 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x03F4 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x03F8 (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x0400 (size: 0x28)

}; // Size: 0x430

struct FUniformIntegerFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    int32 Magnitude;                                                                  // 0x02B8 (size: 0x4)
    TArray<int32> FieldIntResult;                                                     // 0x02C0 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02D0 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02E0 (size: 0x4)

}; // Size: 0x2E8

struct FUniformPointSamplingDataflowNode : public FDataflowNode
{
    class UDynamicMesh* TargetMesh;                                                   // 0x0280 (size: 0x8)
    float SamplingRadius;                                                             // 0x0288 (size: 0x4)
    int32 MaxNumSamples;                                                              // 0x028C (size: 0x4)
    float SubSampleDensity;                                                           // 0x0290 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0294 (size: 0x4)
    TArray<FVector> SamplePoints;                                                     // 0x0298 (size: 0x10)
    TArray<int32> SampleTriangleIDs;                                                  // 0x02A8 (size: 0x10)
    TArray<FVector> SampleBarycentricCoords;                                          // 0x02B8 (size: 0x10)
    int32 NumSamplePoints;                                                            // 0x02C8 (size: 0x4)

}; // Size: 0x2D0

struct FUniformScalarFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    TArray<float> FieldFloatResult;                                                   // 0x02C0 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02D0 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02E0 (size: 0x4)

}; // Size: 0x2E8

struct FUniformScatterPointsDataflowNode : public FDataflowNode
{
    int32 MinNumberOfPoints;                                                          // 0x0280 (size: 0x4)
    int32 MaxNumberOfPoints;                                                          // 0x0284 (size: 0x4)
    float RandomSeed;                                                                 // 0x0288 (size: 0x4)
    FBox BoundingBox;                                                                 // 0x0290 (size: 0x38)
    TArray<FVector> Points;                                                           // 0x02C8 (size: 0x10)

}; // Size: 0x2D8

struct FUniformScatterPointsDataflowNode_v2 : public FDataflowNode
{
    int32 MinNumberOfPoints;                                                          // 0x0280 (size: 0x4)
    int32 MaxNumberOfPoints;                                                          // 0x0284 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0288 (size: 0x4)
    FBox BoundingBox;                                                                 // 0x0290 (size: 0x38)
    TArray<FVector> Points;                                                           // 0x02C8 (size: 0x10)

}; // Size: 0x2D8

struct FUniformVectorFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    FVector Direction;                                                                // 0x02C0 (size: 0x18)
    TArray<FVector> FieldVectorResult;                                                // 0x02D8 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x02E8 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x02F8 (size: 0x4)

}; // Size: 0x300

struct FUnionIntArraysDataflowNode : public FDataflowNode
{
    TArray<int32> InArray1;                                                           // 0x0280 (size: 0x10)
    TArray<int32> InArray2;                                                           // 0x0290 (size: 0x10)
    TArray<int32> OutArray;                                                           // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

struct FUpdateVolumeAttributesDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)

}; // Size: 0x330

struct FValidateGeometryCollectionDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    bool bRemoveUnreferencedGeometry;                                                 // 0x0330 (size: 0x1)
    bool bRemoveClustersOfOne;                                                        // 0x0331 (size: 0x1)
    bool bRemoveDanglingClusters;                                                     // 0x0332 (size: 0x1)

}; // Size: 0x338

struct FVectorArrayNormalizeDataflowNode : public FDataflowNode
{
    TArray<FVector> InVectorArray;                                                    // 0x0280 (size: 0x10)
    FDataflowVertexSelection Selection;                                               // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    TArray<FVector> OutVectorArray;                                                   // 0x02C0 (size: 0x10)

}; // Size: 0x2D0

struct FVectorToStringDataflowNode : public FDataflowNode
{
    FVector Vector;                                                                   // 0x0280 (size: 0x18)
    FString String;                                                                   // 0x0298 (size: 0x10)

}; // Size: 0x2A8

struct FVertexWeightedPointSamplingDataflowNode : public FDataflowNode
{
    class UDynamicMesh* TargetMesh;                                                   // 0x0280 (size: 0x8)
    TArray<float> VertexWeights;                                                      // 0x0288 (size: 0x10)
    float SamplingRadius;                                                             // 0x0298 (size: 0x4)
    int32 MaxNumSamples;                                                              // 0x029C (size: 0x4)
    float SubSampleDensity;                                                           // 0x02A0 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x02A4 (size: 0x4)
    float MaxSamplingRadius;                                                          // 0x02A8 (size: 0x4)
    ENonUniformSamplingDistributionMode SizeDistribution;                             // 0x02AC (size: 0x1)
    float SizeDistributionPower;                                                      // 0x02B0 (size: 0x4)
    ENonUniformSamplingWeightMode WeightMode;                                         // 0x02B4 (size: 0x1)
    bool bInvertWeights;                                                              // 0x02B5 (size: 0x1)
    TArray<FVector> SamplePoints;                                                     // 0x02B8 (size: 0x10)
    TArray<float> SampleRadii;                                                        // 0x02C8 (size: 0x10)
    TArray<int32> SampleTriangleIDs;                                                  // 0x02D8 (size: 0x10)
    TArray<FVector> SampleBarycentricCoords;                                          // 0x02E8 (size: 0x10)
    int32 NumSamplePoints;                                                            // 0x02F8 (size: 0x4)

}; // Size: 0x300

struct FVisualizeFractureDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    int32 Level;                                                                      // 0x0330 (size: 0x4)
    int32 RandomSeed;                                                                 // 0x0334 (size: 0x4)
    bool bApplyExplodedView;                                                          // 0x0338 (size: 0x1)
    float ExplodeAmount;                                                              // 0x033C (size: 0x4)
    FVector Scale;                                                                    // 0x0340 (size: 0x18)
    bool bApplyColor;                                                                 // 0x0358 (size: 0x1)
    EDataflowVisualizeFractureColoringType ColoringType;                              // 0x0359 (size: 0x1)
    int32 RandomColorRangeMin;                                                        // 0x035C (size: 0x4)
    int32 RandomColorRangeMax;                                                        // 0x0360 (size: 0x4)
    FString Attribute;                                                                // 0x0368 (size: 0x10)
    FMinSettings min;                                                                 // 0x0378 (size: 0x14)
    FMaxSettings max;                                                                 // 0x038C (size: 0x14)
    FVector Offset;                                                                   // 0x03A0 (size: 0x18)

}; // Size: 0x3B8

struct FVisualizeTetrahedronsDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<FVector> Vertices;                                                         // 0x0330 (size: 0x10)

}; // Size: 0x340

struct FVoronoiFractureDataflowNode : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<FVector> Points;                                                           // 0x0330 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)
    float RandomSeed;                                                                 // 0x0368 (size: 0x4)
    float ChanceToFracture;                                                           // 0x036C (size: 0x4)
    bool GroupFracture;                                                               // 0x0370 (size: 0x1)
    float Grout;                                                                      // 0x0374 (size: 0x4)
    float Amplitude;                                                                  // 0x0378 (size: 0x4)
    float Frequency;                                                                  // 0x037C (size: 0x4)
    float Persistence;                                                                // 0x0380 (size: 0x4)
    float Lacunarity;                                                                 // 0x0384 (size: 0x4)
    int32 OctaveNumber;                                                               // 0x0388 (size: 0x4)
    float PointSpacing;                                                               // 0x038C (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x0390 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x0394 (size: 0x4)

}; // Size: 0x398

struct FVoronoiFractureDataflowNode_v2 : public FDataflowNode
{
    FManagedArrayCollection Collection;                                               // 0x0280 (size: 0xB0)
    TArray<FVector> Points;                                                           // 0x0330 (size: 0x10)
    FDataflowTransformSelection TransformSelection;                                   // 0x0340 (size: 0x28)
    FTransform Transform;                                                             // 0x0370 (size: 0x60)
    int32 RandomSeed;                                                                 // 0x03D0 (size: 0x4)
    float ChanceToFracture;                                                           // 0x03D4 (size: 0x4)
    bool SplitIslands;                                                                // 0x03D8 (size: 0x1)
    float Grout;                                                                      // 0x03DC (size: 0x4)
    float Amplitude;                                                                  // 0x03E0 (size: 0x4)
    float Frequency;                                                                  // 0x03E4 (size: 0x4)
    float Persistence;                                                                // 0x03E8 (size: 0x4)
    float Lacunarity;                                                                 // 0x03EC (size: 0x4)
    int32 OctaveNumber;                                                               // 0x03F0 (size: 0x4)
    float PointSpacing;                                                               // 0x03F4 (size: 0x4)
    bool AddSamplesForCollision;                                                      // 0x03F8 (size: 0x1)
    float CollisionSampleSpacing;                                                     // 0x03FC (size: 0x4)
    FDataflowTransformSelection NewGeometryTransformSelection;                        // 0x0400 (size: 0x28)

}; // Size: 0x430

struct FWaveScalarFieldDataflowNode : public FDataflowNode
{
    TArray<FVector3f> SamplePositions;                                                // 0x0280 (size: 0x10)
    FDataflowVertexSelection SampleIndices;                                           // 0x0290 (size: 0x28)
    float Magnitude;                                                                  // 0x02B8 (size: 0x4)
    FVector Position;                                                                 // 0x02C0 (size: 0x18)
    FVector Translation;                                                              // 0x02D8 (size: 0x18)
    float Wavelength;                                                                 // 0x02F0 (size: 0x4)
    float Period;                                                                     // 0x02F4 (size: 0x4)
    EDataflowWaveFunctionType FunctionType;                                           // 0x02F8 (size: 0x1)
    EDataflowFieldFalloffType FalloffType;                                            // 0x02F9 (size: 0x1)
    TArray<float> FieldFloatResult;                                                   // 0x0300 (size: 0x10)
    TArray<int32> FieldRemap;                                                         // 0x0310 (size: 0x10)
    int32 NumSamplePositions;                                                         // 0x0320 (size: 0x4)

}; // Size: 0x328

struct FWrapDataflowNode : public FDataflowNode
{
    float float;                                                                      // 0x0280 (size: 0x4)
    float min;                                                                        // 0x0284 (size: 0x4)
    float max;                                                                        // 0x0288 (size: 0x4)
    float ReturnValue;                                                                // 0x028C (size: 0x4)

}; // Size: 0x290

struct FWriteStringToFile : public FDataflowNode
{
    FString FilePath;                                                                 // 0x0280 (size: 0x10)
    FString FileContents;                                                             // 0x0290 (size: 0x10)

}; // Size: 0x2A0

#endif
