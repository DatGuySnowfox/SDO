#ifndef UE4SS_SDK_GeometryFlowMeshProcessing_HPP
#define UE4SS_SDK_GeometryFlowMeshProcessing_HPP

#include "GeometryFlowMeshProcessing_enums.hpp"

struct FBakeMeshMultiTextureSettings : public FBakeMeshTextureImageSettings
{
}; // Size: 0x10

struct FBakeMeshNormalMapSettings
{
    double MaxDistance;                                                               // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FBakeMeshTextureImageSettings
{
    int32 DetailUVLayer;                                                              // 0x0000 (size: 0x4)
    double MaxDistance;                                                               // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGenerateConvexHullMeshSettings
{
    bool bPrefilterVertices;                                                          // 0x0000 (size: 0x1)
    int32 PrefilterGridResolution;                                                    // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FGenerateConvexHullSettings
{
    int32 SimplifyToTriangleCount;                                                    // 0x0000 (size: 0x4)
    bool bPrefilterVertices;                                                          // 0x0004 (size: 0x1)
    int32 PrefilterGridResolution;                                                    // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FGenerateSimpleCollisionSettings
{
    FGenerateConvexHullSettings ConvexHullSettings;                                   // 0x0004 (size: 0xC)
    FGenerateSweptHullSettings SweptHullSettings;                                     // 0x0010 (size: 0xC)

}; // Size: 0x1C

struct FGenerateSweptHullSettings
{
    bool bSimplifyPolygons;                                                           // 0x0000 (size: 0x1)
    EGeometryFlow_ProjectedHullAxisMode SweepAxis;                                    // 0x0004 (size: 0x4)
    float HullTolerance;                                                              // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FMeshMakeBakingCacheSettings
{
    FIntPoint Dimensions;                                                             // 0x0000 (size: 0x8)
    int32 UVLayer;                                                                    // 0x0008 (size: 0x4)
    float Thickness;                                                                  // 0x000C (size: 0x4)

}; // Size: 0x10

struct FMeshMakeCleanGeometrySettings
{
    int32 FillHolesEdgeCountThresh;                                                   // 0x0000 (size: 0x4)
    double FillHolesEstimatedAreaFraction;                                            // 0x0008 (size: 0x8)
    bool bDiscardAllAttributes;                                                       // 0x0010 (size: 0x1)
    bool bClearUVs;                                                                   // 0x0011 (size: 0x1)
    bool bClearNormals;                                                               // 0x0012 (size: 0x1)
    bool bClearTangents;                                                              // 0x0013 (size: 0x1)
    bool bClearVertexColors;                                                          // 0x0014 (size: 0x1)
    bool bClearMaterialIDs;                                                           // 0x0015 (size: 0x1)
    bool bOutputMeshVertexNormals;                                                    // 0x0016 (size: 0x1)
    bool bOutputOverlayVertexNormals;                                                 // 0x0017 (size: 0x1)

}; // Size: 0x18

struct FMeshNormalFlowSettings : public FMeshSimplifySettings
{
    int32 MaxRemeshIterations;                                                        // 0x0028 (size: 0x4)
    int32 NumExtraProjectionIterations;                                               // 0x002C (size: 0x4)
    bool bFlips;                                                                      // 0x0030 (size: 0x1)
    bool bSplits;                                                                     // 0x0031 (size: 0x1)
    bool bCollapses;                                                                  // 0x0032 (size: 0x1)
    EGeometryFlow_SmoothTypes SmoothingType;                                          // 0x0034 (size: 0x4)
    float SmoothingStrength;                                                          // 0x0038 (size: 0x4)

}; // Size: 0x3C

struct FMeshNormalsSettings
{
    EGeometryFlow_ComputeNormalsType NormalsType;                                     // 0x0000 (size: 0x4)
    bool bInvert;                                                                     // 0x0004 (size: 0x1)
    bool bAreaWeighted;                                                               // 0x0005 (size: 0x1)
    bool bAngleWeighted;                                                              // 0x0006 (size: 0x1)
    double AngleThresholdDeg;                                                         // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FMeshRecalculateUVsSettings
{
    EGeometryFlow_RecalculateUVsUnwrapType UnwrapType;                                // 0x0000 (size: 0x1)
    int32 UVLayer;                                                                    // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FMeshRepackUVsSettings
{
    int32 UVLayer;                                                                    // 0x0000 (size: 0x4)
    int32 TextureResolution;                                                          // 0x0004 (size: 0x4)
    int32 GutterSize;                                                                 // 0x0008 (size: 0x4)
    bool bAllowFlips;                                                                 // 0x000C (size: 0x1)
    FVector2f UVScale;                                                                // 0x0010 (size: 0x8)
    FVector2f UVTranslation;                                                          // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FMeshSimplifySettings
{
    EGeometryFlow_MeshSimplifyType SimplifyType;                                      // 0x0000 (size: 0x4)
    EGeomtryFlow_MeshSimplifyTargetType TargetType;                                   // 0x0004 (size: 0x4)
    int32 TargetCount;                                                                // 0x0008 (size: 0x4)
    float TargetFraction;                                                             // 0x000C (size: 0x4)
    float GeometricTolerance;                                                         // 0x0010 (size: 0x4)
    bool bDiscardAttributes;                                                          // 0x0014 (size: 0x1)
    bool bPreventNormalFlips;                                                         // 0x0015 (size: 0x1)
    bool bPreserveSharpEdges;                                                         // 0x0016 (size: 0x1)
    bool bAllowSeamCollapse;                                                          // 0x0017 (size: 0x1)
    bool bAllowSeamSplits;                                                            // 0x0018 (size: 0x1)
    EGeometryFlow_EdgeRefineFlags MeshBoundaryConstraints;                            // 0x001C (size: 0x4)
    EGeometryFlow_EdgeRefineFlags GroupBorderConstraints;                             // 0x0020 (size: 0x4)
    EGeometryFlow_EdgeRefineFlags MaterialBorderConstraints;                          // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FMeshSolidifySettings
{
    int32 VoxelResolution;                                                            // 0x0000 (size: 0x4)
    float WindingThreshold;                                                           // 0x0004 (size: 0x4)
    int32 SurfaceConvergeSteps;                                                       // 0x0008 (size: 0x4)
    float ExtendBounds;                                                               // 0x000C (size: 0x4)

}; // Size: 0x10

struct FMeshTangentsSettings
{
    EGeometryFlow_ComputeTangentsType TangentsType;                                   // 0x0000 (size: 0x4)
    int32 UVLayer;                                                                    // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FMeshThickenSettings
{
}; // Size: 0x4

struct FVoxMorphologyOpSettings
{
    int32 VoxelResolution;                                                            // 0x0000 (size: 0x4)
    double Distance;                                                                  // 0x0008 (size: 0x8)

}; // Size: 0x10

#endif
