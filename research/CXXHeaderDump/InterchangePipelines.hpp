#ifndef UE4SS_SDK_InterchangePipelines_HPP
#define UE4SS_SDK_InterchangePipelines_HPP

#include "InterchangePipelines_enums.hpp"

struct FInterchangeLodSceneNodeContainer
{
    TArray<UInterchangeSceneNode*> SceneNodes;                                        // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FInterchangeMeshGeometry
{
    FString MeshUid;                                                                  // 0x0000 (size: 0x10)
    class UInterchangeMeshNode* MeshNode;                                             // 0x0010 (size: 0x8)
    TArray<FString> ReferencingMeshInstanceUids;                                      // 0x0018 (size: 0x10)
    TArray<FString> AttachedSocketUids;                                               // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FInterchangeMeshInstance
{
    FString MeshInstanceUid;                                                          // 0x0000 (size: 0x10)
    class UInterchangeSceneNode* LodGroupNode;                                        // 0x0010 (size: 0x8)
    bool bReferenceSkinnedMesh;                                                       // 0x0018 (size: 0x1)
    bool bReferenceMorphTarget;                                                       // 0x0019 (size: 0x1)
    bool bHasMorphTargets;                                                            // 0x001A (size: 0x1)
    bool bIsAnimated;                                                                 // 0x001B (size: 0x1)
    TMap<int32, FInterchangeLodSceneNodeContainer> SceneNodePerLodIndex;              // 0x0020 (size: 0x50)
    TArray<FString> ReferencingMeshGeometryUids;                                      // 0x0070 (size: 0x10)

}; // Size: 0x80

struct FInterchangePipelineMeshesUtilitiesContext
{
    bool bConvertStaticMeshToSkeletalMesh;                                            // 0x0000 (size: 0x1)
    bool bConvertSkeletalMeshToStaticMesh;                                            // 0x0001 (size: 0x1)
    bool bConvertStaticsWithMorphTargetsToSkeletals;                                  // 0x0002 (size: 0x1)
    bool bImportMeshesInBoneHierarchy;                                                // 0x0003 (size: 0x1)
    bool bQueryGeometryOnlyIfNoInstance;                                              // 0x0004 (size: 0x1)
    bool bIgnoreStaticMeshes;                                                         // 0x0005 (size: 0x1)
    bool bIgnoreGeometryCaches;                                                       // 0x0006 (size: 0x1)

}; // Size: 0x7

class UGLTFPipelineSettings : public UDeveloperSettings
{
    TMap<FString, FSoftObjectPath> MaterialParents;                                   // 0x0038 (size: 0x50)

}; // Size: 0x90

class UInterchangeGLTFPipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)

}; // Size: 0x140

class UInterchangeGenericAnimationPipeline : public UInterchangePipelineBase
{
    TWeakObjectPtr<class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties> CommonSkeletalMeshesAndAnimationsProperties; // 0x0128 (size: 0x8)
    TWeakObjectPtr<class UInterchangeGenericCommonMeshesProperties> CommonMeshesProperties; // 0x0130 (size: 0x8)
    bool bImportAnimations;                                                           // 0x0138 (size: 0x1)
    bool bImportBoneTracks;                                                           // 0x0139 (size: 0x1)
    EInterchangeAnimationRange AnimationRange;                                        // 0x013A (size: 0x1)
    FInt32Interval FrameImportRange;                                                  // 0x013C (size: 0x8)
    bool bUse30HzToBakeBoneAnimation;                                                 // 0x0144 (size: 0x1)
    int32 CustomBoneAnimationSampleRate;                                              // 0x0148 (size: 0x4)
    bool bSnapToClosestFrameBoundary;                                                 // 0x014C (size: 0x1)
    bool bImportCustomAttribute;                                                      // 0x014D (size: 0x1)
    bool bAddCurveMetadataToSkeleton;                                                 // 0x014E (size: 0x1)
    bool bSetMaterialDriveParameterOnCustomAttribute;                                 // 0x014F (size: 0x1)
    TArray<FString> MaterialCurveSuffixes;                                            // 0x0150 (size: 0x10)
    bool bRemoveCurveRedundantKeys;                                                   // 0x0160 (size: 0x1)
    bool bDoNotImportCurveWithZero;                                                   // 0x0161 (size: 0x1)
    bool bDeleteExistingNonCurveCustomAttributes;                                     // 0x0162 (size: 0x1)
    bool bDeleteExistingCustomAttributeCurves;                                        // 0x0163 (size: 0x1)
    bool bDeleteExistingMorphTargetCurves;                                            // 0x0164 (size: 0x1)
    FString SourceAnimationName;                                                      // 0x0168 (size: 0x10)
    bool bSceneImport;                                                                // 0x0178 (size: 0x1)

}; // Size: 0x198

class UInterchangeGenericAssetsPipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)
    EReimportStrategyFlags ReimportStrategy;                                          // 0x0138 (size: 0x1)
    bool bUseSourceNameForAsset;                                                      // 0x0139 (size: 0x1)
    bool bSceneNameSubFolder;                                                         // 0x013A (size: 0x1)
    bool bAssetTypeSubFolders;                                                        // 0x013B (size: 0x1)
    FString AssetName;                                                                // 0x0140 (size: 0x10)
    FVector ImportOffsetTranslation;                                                  // 0x0150 (size: 0x18)
    FRotator ImportOffsetRotation;                                                    // 0x0168 (size: 0x18)
    float ImportOffsetUniformScale;                                                   // 0x0180 (size: 0x4)
    class UInterchangeGenericCommonMeshesProperties* CommonMeshesProperties;          // 0x0188 (size: 0x8)
    class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties* CommonSkeletalMeshesAndAnimationsProperties; // 0x0190 (size: 0x8)
    class UInterchangeGenericMeshPipeline* MeshPipeline;                              // 0x0198 (size: 0x8)
    class UInterchangeGenericAnimationPipeline* AnimationPipeline;                    // 0x01A0 (size: 0x8)
    class UInterchangeGenericMaterialPipeline* MaterialPipeline;                      // 0x01A8 (size: 0x8)
    FSoftObjectPath ContentPathExistingSkeleton;                                      // 0x01B0 (size: 0x20)
    bool bImportOnlyAnimationAdjusted;                                                // 0x01D0 (size: 0x1)

}; // Size: 0x2B0

class UInterchangeGenericCommonMeshesProperties : public UInterchangePipelineBase
{
    EInterchangeForceMeshType ForceAllMeshAsType;                                     // 0x0128 (size: 0x1)
    bool bAutoDetectMeshType;                                                         // 0x0129 (size: 0x1)
    bool bImportLods;                                                                 // 0x012A (size: 0x1)
    bool bBakeMeshes;                                                                 // 0x012B (size: 0x1)
    bool bBakePivotMeshes;                                                            // 0x012C (size: 0x1)
    bool bKeepSectionsSeparate;                                                       // 0x012D (size: 0x1)
    EInterchangeVertexColorImportOption VertexColorImportOption;                      // 0x012E (size: 0x1)
    FColor VertexOverrideColor;                                                       // 0x0130 (size: 0x4)
    bool bImportSockets;                                                              // 0x0134 (size: 0x1)
    bool bRecomputeNormals;                                                           // 0x0135 (size: 0x1)
    bool bRecomputeTangents;                                                          // 0x0136 (size: 0x1)
    bool bUseMikkTSpace;                                                              // 0x0137 (size: 0x1)
    bool bComputeWeightedNormals;                                                     // 0x0138 (size: 0x1)
    bool bUseHighPrecisionTangentBasis;                                               // 0x0139 (size: 0x1)
    bool bUseFullPrecisionUVs;                                                        // 0x013A (size: 0x1)
    bool bUseBackwardsCompatibleF16TruncUVs;                                          // 0x013B (size: 0x1)
    bool bRemoveDegenerates;                                                          // 0x013C (size: 0x1)

}; // Size: 0x140

class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties : public UInterchangePipelineBase
{
    bool bImportOnlyAnimations;                                                       // 0x0128 (size: 0x1)
    TWeakObjectPtr<class USkeleton> Skeleton;                                         // 0x012C (size: 0x8)
    bool bImportMeshesInBoneHierarchy;                                                // 0x0134 (size: 0x1)
    bool bUseT0AsRefPose;                                                             // 0x0135 (size: 0x1)
    bool bAddCurveMetadataToSkeleton;                                                 // 0x0136 (size: 0x1)
    bool bConvertStaticsWithMorphTargetsToSkeletals;                                  // 0x0137 (size: 0x1)

}; // Size: 0x138

class UInterchangeGenericLevelPipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)
    EReimportStrategyFlags ReimportPropertyStrategy;                                  // 0x0138 (size: 0x1)
    EInterchangeSceneHierarchyType SceneHierarchyType;                                // 0x0139 (size: 0x1)
    bool bDeleteMissingActors;                                                        // 0x013A (size: 0x1)
    bool bForceReimportDeletedActors;                                                 // 0x013B (size: 0x1)
    bool bForceReimportDeletedAssets;                                                 // 0x013C (size: 0x1)
    bool bDeleteMissingAssets;                                                        // 0x013D (size: 0x1)
    bool bUsePhysicalInsteadOfStandardPerspectiveCamera;                              // 0x013E (size: 0x1)

}; // Size: 0x160

class UInterchangeGenericMaterialPipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)
    bool bImportMaterials;                                                            // 0x0138 (size: 0x1)
    EInterchangeMaterialSearchLocation SearchLocation;                                // 0x0139 (size: 0x1)
    FString AssetName;                                                                // 0x0140 (size: 0x10)
    EInterchangeMaterialImportOption MaterialImport;                                  // 0x0150 (size: 0x1)
    bool bIdentifyDuplicateMaterials;                                                 // 0x0151 (size: 0x1)
    bool bCreateMaterialInstanceForParent;                                            // 0x0152 (size: 0x1)
    FSoftObjectPath ParentMaterial;                                                   // 0x0158 (size: 0x20)
    class UInterchangeGenericTexturePipeline* TexturePipeline;                        // 0x0178 (size: 0x8)
    class UInterchangeSparseVolumeTexturePipeline* SparseVolumeTexturePipeline;       // 0x0180 (size: 0x8)
    bool bOverrideDisplacement;                                                       // 0x0188 (size: 0x1)
    float OverrideDisplacementCenter;                                                 // 0x018C (size: 0x4)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0190 (size: 0x8)

}; // Size: 0x1C8

class UInterchangeGenericMeshPipeline : public UInterchangePipelineBase
{
    TWeakObjectPtr<class UInterchangeGenericCommonMeshesProperties> CommonMeshesProperties; // 0x0128 (size: 0x8)
    TWeakObjectPtr<class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties> CommonSkeletalMeshesAndAnimationsProperties; // 0x0130 (size: 0x8)
    bool bImportStaticMeshes;                                                         // 0x0138 (size: 0x1)
    bool bCombineStaticMeshes;                                                        // 0x0139 (size: 0x1)
    FName LODGroup;                                                                   // 0x013C (size: 0x8)
    bool bAutoComputeLODScreenSizes;                                                  // 0x0144 (size: 0x1)
    TArray<float> LODScreenSizes;                                                     // 0x0148 (size: 0x10)
    bool bImportCollision;                                                            // 0x0158 (size: 0x1)
    bool bCollision;                                                                  // 0x0159 (size: 0x1)
    bool bImportCollisionAccordingToMeshName;                                         // 0x015A (size: 0x1)
    bool bOneConvexHullPerUCX;                                                        // 0x015B (size: 0x1)
    EInterchangeMeshCollision Collision;                                              // 0x015C (size: 0x1)
    bool bForceCollisionPrimitiveGeneration;                                          // 0x015D (size: 0x1)
    bool bBuildNanite;                                                                // 0x015E (size: 0x1)
    bool bBuildReversedIndexBuffer;                                                   // 0x015F (size: 0x1)
    bool bGenerateLightmapUVs;                                                        // 0x0160 (size: 0x1)
    bool bGenerateDistanceFieldAsIfTwoSided;                                          // 0x0161 (size: 0x1)
    bool bSupportFaceRemap;                                                           // 0x0162 (size: 0x1)
    int32 MinLightmapResolution;                                                      // 0x0164 (size: 0x4)
    int32 SrcLightmapIndex;                                                           // 0x0168 (size: 0x4)
    int32 DstLightmapIndex;                                                           // 0x016C (size: 0x4)
    FVector BuildScale3D;                                                             // 0x0170 (size: 0x18)
    float DistanceFieldResolutionScale;                                               // 0x0188 (size: 0x4)
    TWeakObjectPtr<class UStaticMesh> DistanceFieldReplacementMesh;                   // 0x018C (size: 0x8)
    int32 MaxLumenMeshCards;                                                          // 0x0194 (size: 0x4)
    bool bImportSkeletalMeshes;                                                       // 0x0198 (size: 0x1)
    EInterchangeSkeletalMeshContentType SkeletalMeshImportContentType;                // 0x0199 (size: 0x1)
    EInterchangeSkeletalMeshContentType LastSkeletalMeshImportContentType;            // 0x019A (size: 0x1)
    bool bCombineSkeletalMeshes;                                                      // 0x019B (size: 0x1)
    bool bImportMorphTargets;                                                         // 0x019C (size: 0x1)
    bool bMergeMorphTargetsWithSameName;                                              // 0x019D (size: 0x1)
    bool bImportVertexAttributes;                                                     // 0x019E (size: 0x1)
    bool bUpdateSkeletonReferencePose;                                                // 0x019F (size: 0x1)
    bool bCreatePhysicsAsset;                                                         // 0x01A0 (size: 0x1)
    TWeakObjectPtr<class UPhysicsAsset> PhysicsAsset;                                 // 0x01A4 (size: 0x8)
    bool bUseHighPrecisionSkinWeights;                                                // 0x01AC (size: 0x1)
    float ThresholdPosition;                                                          // 0x01B0 (size: 0x4)
    float ThresholdTangentNormal;                                                     // 0x01B4 (size: 0x4)
    float ThresholdUV;                                                                // 0x01B8 (size: 0x4)
    float MorphThresholdPosition;                                                     // 0x01BC (size: 0x4)
    int32 BoneInfluenceLimit;                                                         // 0x01C0 (size: 0x4)
    bool bImportGeometryCaches;                                                       // 0x01C4 (size: 0x1)
    bool bFlattenTracks;                                                              // 0x01C5 (size: 0x1)
    float CompressedPositionPrecision;                                                // 0x01C8 (size: 0x4)
    int32 CompressedTextureCoordinatesNumberOfBits;                                   // 0x01CC (size: 0x4)
    bool bOverrideTimeRange;                                                          // 0x01D0 (size: 0x1)
    int32 FrameStart;                                                                 // 0x01D4 (size: 0x4)
    int32 FrameEnd;                                                                   // 0x01D8 (size: 0x4)
    EInterchangeMotionVectorsHandling MotionVectors;                                  // 0x01DC (size: 0x1)
    bool bApplyConstantTopologyOptimizations;                                         // 0x01DD (size: 0x1)
    bool bStoreImportedVertexNumbers;                                                 // 0x01DE (size: 0x1)
    bool bOptimizeIndexBuffers;                                                       // 0x01DF (size: 0x1)

    void SetCombineSkeletalMeshes(bool InbCombineSkeletalMeshes);
    bool GetCombineSkeletalMeshes();
}; // Size: 0x240

class UInterchangeGenericTexturePipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)
    bool bImportTextures;                                                             // 0x0138 (size: 0x1)
    FString AssetName;                                                                // 0x0140 (size: 0x10)
    bool bAllowNonPowerOfTwo;                                                         // 0x0150 (size: 0x1)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0158 (size: 0x8)

}; // Size: 0x190

class UInterchangeMaterialXPipeline : public UInterchangePipelineBase
{
}; // Size: 0x130

class UInterchangePipelineMeshesUtilities : public UObject
{

    void SetContext(const FInterchangePipelineMeshesUtilitiesContext& Context);
    bool IsValidMeshInstanceUid(FString MeshInstanceUid);
    bool IsValidMeshGeometryUid(FString MeshGeometryUid);
    FString GetMeshInstanceSkeletonRootUid(FString MeshInstanceUid);
    FInterchangeMeshInstance GetMeshInstanceByUid(FString MeshInstanceUid);
    FString GetMeshGeometrySkeletonRootUid(FString MeshGeometryUid);
    FInterchangeMeshGeometry GetMeshGeometryByUid(FString MeshGeometryUid);
    void GetAllStaticMeshInstance(TArray<FString>& MeshInstanceUids);
    void GetAllStaticMeshGeometry(TArray<FString>& MeshGeometryUids);
    void GetAllSkinnedMeshInstance(TArray<FString>& MeshInstanceUids);
    void GetAllSkinnedMeshGeometry(TArray<FString>& MeshGeometryUids);
    void GetAllMeshInstanceUidsUsingMeshGeometryUid(FString MeshGeometryUid, TArray<FString>& MeshInstanceUids);
    void GetAllMeshInstanceUids(TArray<FString>& MeshInstanceUids);
    void GetAllMeshGeometryNotInstanced(TArray<FString>& MeshGeometryUids);
    void GetAllMeshGeometry(TArray<FString>& MeshGeometryUids);
    void GetAllGeometryCacheInstance(TArray<FString>& MeshInstanceUids);
    void GetAllGeometryCacheGeometry(TArray<FString>& MeshGeometryUids);
    class UInterchangePipelineMeshesUtilities* CreateInterchangePipelineMeshesUtilities(class UInterchangeBaseNodeContainer* BaseNodeContainer);
}; // Size: 0x128

class UInterchangeSparseVolumeTexturePipeline : public UInterchangePipelineBase
{
    FString PipelineDisplayName;                                                      // 0x0128 (size: 0x10)
    bool bImportSparseVolumeTextures;                                                 // 0x0138 (size: 0x1)
    bool bImportAnimatedSparseVolumeTextures;                                         // 0x0139 (size: 0x1)
    FString AssetName;                                                                // 0x0140 (size: 0x10)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0150 (size: 0x8)

}; // Size: 0x158

class UMaterialXPipelineSettings : public UDeveloperSettings
{
    TMap<EInterchangeMaterialXShaders, FSoftObjectPath> PredefinedSurfaceShaders;     // 0x0038 (size: 0x50)
    TMap<EInterchangeMaterialXBSDF, FSoftObjectPath> PredefinedBSDF;                  // 0x0088 (size: 0x50)
    TMap<EInterchangeMaterialXEDF, FSoftObjectPath> PredefinedEDF;                    // 0x00D8 (size: 0x50)
    TMap<EInterchangeMaterialXVDF, FSoftObjectPath> PredefinedVDF;                    // 0x0128 (size: 0x50)

}; // Size: 0x178

#endif
