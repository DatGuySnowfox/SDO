#ifndef UE4SS_SDK_InterchangePipelines_HPP
#define UE4SS_SDK_InterchangePipelines_HPP

#include "InterchangePipelines_enums.hpp"

struct FInterchangeLodSceneNodeContainer
{
    TArray<class UInterchangeSceneNode*> SceneNodes;                                  // 0x0000 (size: 0x10)

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

}; // Size: 0x5

class UGLTFPipelineSettings : public UDeveloperSettings
{
    TMap<class FString, class FSoftObjectPath> MaterialParents;                       // 0x0038 (size: 0x50)

}; // Size: 0x90

class UInterchangeGLTFPipeline : public UInterchangePipelineBase
{
    bool bUseGLTFMaterialInstanceLibrary;                                             // 0x00F0 (size: 0x1)

}; // Size: 0xF8

class UInterchangeGenericAnimationPipeline : public UInterchangePipelineBase
{
    TWeakObjectPtr<class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties> CommonSkeletalMeshesAndAnimationsProperties; // 0x00E8 (size: 0x8)
    TWeakObjectPtr<class UInterchangeGenericCommonMeshesProperties> CommonMeshesProperties; // 0x00F0 (size: 0x8)
    bool bImportAnimations;                                                           // 0x00F8 (size: 0x1)
    bool bImportBoneTracks;                                                           // 0x00F9 (size: 0x1)
    EInterchangeAnimationRange AnimationRange;                                        // 0x00FA (size: 0x1)
    FInt32Interval FrameImportRange;                                                  // 0x00FC (size: 0x8)
    bool bUse30HzToBakeBoneAnimation;                                                 // 0x0104 (size: 0x1)
    int32 CustomBoneAnimationSampleRate;                                              // 0x0108 (size: 0x4)
    bool bSnapToClosestFrameBoundary;                                                 // 0x010C (size: 0x1)
    bool bImportCustomAttribute;                                                      // 0x010D (size: 0x1)
    bool bAddCurveMetadataToSkeleton;                                                 // 0x010E (size: 0x1)
    bool bSetMaterialDriveParameterOnCustomAttribute;                                 // 0x010F (size: 0x1)
    TArray<FString> MaterialCurveSuffixes;                                            // 0x0110 (size: 0x10)
    bool bRemoveCurveRedundantKeys;                                                   // 0x0120 (size: 0x1)
    bool bDoNotImportCurveWithZero;                                                   // 0x0121 (size: 0x1)
    bool bDeleteExistingNonCurveCustomAttributes;                                     // 0x0122 (size: 0x1)
    bool bDeleteExistingCustomAttributeCurves;                                        // 0x0123 (size: 0x1)
    bool bDeleteExistingMorphTargetCurves;                                            // 0x0124 (size: 0x1)
    FString SourceAnimationName;                                                      // 0x0128 (size: 0x10)
    bool bSceneImport;                                                                // 0x0138 (size: 0x1)

}; // Size: 0x158

class UInterchangeGenericAssetsPipeline : public UInterchangePipelineBase
{
    EReimportStrategyFlags ReimportStrategy;                                          // 0x00E8 (size: 0x1)
    bool bUseSourceNameForAsset;                                                      // 0x00E9 (size: 0x1)
    FString AssetName;                                                                // 0x00F0 (size: 0x10)
    FVector ImportOffsetTranslation;                                                  // 0x0100 (size: 0x18)
    FRotator ImportOffsetRotation;                                                    // 0x0118 (size: 0x18)
    float ImportOffsetUniformScale;                                                   // 0x0130 (size: 0x4)
    class UInterchangeGenericCommonMeshesProperties* CommonMeshesProperties;          // 0x0138 (size: 0x8)
    class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties* CommonSkeletalMeshesAndAnimationsProperties; // 0x0140 (size: 0x8)
    class UInterchangeGenericMeshPipeline* MeshPipeline;                              // 0x0148 (size: 0x8)
    class UInterchangeGenericAnimationPipeline* AnimationPipeline;                    // 0x0150 (size: 0x8)
    class UInterchangeGenericMaterialPipeline* MaterialPipeline;                      // 0x0158 (size: 0x8)

}; // Size: 0x178

class UInterchangeGenericCommonMeshesProperties : public UInterchangePipelineBase
{
    EInterchangeForceMeshType ForceAllMeshAsType;                                     // 0x00E8 (size: 0x1)
    bool bImportLods;                                                                 // 0x00E9 (size: 0x1)
    bool bBakeMeshes;                                                                 // 0x00EA (size: 0x1)
    EInterchangeVertexColorImportOption VertexColorImportOption;                      // 0x00EB (size: 0x1)
    FColor VertexOverrideColor;                                                       // 0x00EC (size: 0x4)
    bool bRecomputeNormals;                                                           // 0x00F0 (size: 0x1)
    bool bRecomputeTangents;                                                          // 0x00F1 (size: 0x1)
    bool bUseMikkTSpace;                                                              // 0x00F2 (size: 0x1)
    bool bComputeWeightedNormals;                                                     // 0x00F3 (size: 0x1)
    bool bUseHighPrecisionTangentBasis;                                               // 0x00F4 (size: 0x1)
    bool bUseFullPrecisionUVs;                                                        // 0x00F5 (size: 0x1)
    bool bUseBackwardsCompatibleF16TruncUVs;                                          // 0x00F6 (size: 0x1)
    bool bRemoveDegenerates;                                                          // 0x00F7 (size: 0x1)

}; // Size: 0xF8

class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties : public UInterchangePipelineBase
{
    bool bImportOnlyAnimations;                                                       // 0x00E8 (size: 0x1)
    TWeakObjectPtr<class USkeleton> Skeleton;                                         // 0x00EC (size: 0x8)
    bool bImportMeshesInBoneHierarchy;                                                // 0x00F4 (size: 0x1)
    bool bUseT0AsRefPose;                                                             // 0x00F5 (size: 0x1)
    bool bConvertStaticsWithMorphTargetsToSkeletals;                                  // 0x00F6 (size: 0x1)

}; // Size: 0xF8

class UInterchangeGenericLevelPipeline : public UInterchangePipelineBase
{
    EReimportStrategyFlags ReimportPropertyStrategy;                                  // 0x00E8 (size: 0x1)
    bool bDeleteMissingActors;                                                        // 0x00E9 (size: 0x1)
    bool bForceReimportDeletedActors;                                                 // 0x00EA (size: 0x1)
    bool bForceReimportDeletedAssets;                                                 // 0x00EB (size: 0x1)
    bool bDeleteMissingAssets;                                                        // 0x00EC (size: 0x1)
    bool bUsePhysicalInsteadOfStandardPerspectiveCamera;                              // 0x00ED (size: 0x1)

}; // Size: 0xF8

class UInterchangeGenericMaterialPipeline : public UInterchangePipelineBase
{
    bool bImportMaterials;                                                            // 0x00E8 (size: 0x1)
    FString AssetName;                                                                // 0x00F0 (size: 0x10)
    EInterchangeMaterialImportOption MaterialImport;                                  // 0x0100 (size: 0x1)
    FSoftObjectPath ParentMaterial;                                                   // 0x0108 (size: 0x20)
    class UInterchangeGenericTexturePipeline* TexturePipeline;                        // 0x0128 (size: 0x8)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0130 (size: 0x8)

}; // Size: 0x180

class UInterchangeGenericMeshPipeline : public UInterchangePipelineBase
{
    TWeakObjectPtr<class UInterchangeGenericCommonMeshesProperties> CommonMeshesProperties; // 0x00E8 (size: 0x8)
    TWeakObjectPtr<class UInterchangeGenericCommonSkeletalMeshesAndAnimationsProperties> CommonSkeletalMeshesAndAnimationsProperties; // 0x00F0 (size: 0x8)
    bool bImportStaticMeshes;                                                         // 0x00F8 (size: 0x1)
    bool bCombineStaticMeshes;                                                        // 0x00F9 (size: 0x1)
    FName LODGroup;                                                                   // 0x00FC (size: 0x8)
    bool bImportCollision;                                                            // 0x0104 (size: 0x1)
    bool bImportCollisionAccordingToMeshName;                                         // 0x0105 (size: 0x1)
    bool bOneConvexHullPerUCX;                                                        // 0x0106 (size: 0x1)
    bool bBuildNanite;                                                                // 0x0107 (size: 0x1)
    bool bBuildReversedIndexBuffer;                                                   // 0x0108 (size: 0x1)
    bool bGenerateLightmapUVs;                                                        // 0x0109 (size: 0x1)
    bool bGenerateDistanceFieldAsIfTwoSided;                                          // 0x010A (size: 0x1)
    bool bSupportFaceRemap;                                                           // 0x010B (size: 0x1)
    int32 MinLightmapResolution;                                                      // 0x010C (size: 0x4)
    int32 SrcLightmapIndex;                                                           // 0x0110 (size: 0x4)
    int32 DstLightmapIndex;                                                           // 0x0114 (size: 0x4)
    FVector BuildScale3D;                                                             // 0x0118 (size: 0x18)
    float DistanceFieldResolutionScale;                                               // 0x0130 (size: 0x4)
    TWeakObjectPtr<class UStaticMesh> DistanceFieldReplacementMesh;                   // 0x0134 (size: 0x8)
    int32 MaxLumenMeshCards;                                                          // 0x013C (size: 0x4)
    bool bImportSkeletalMeshes;                                                       // 0x0140 (size: 0x1)
    EInterchangeSkeletalMeshContentType SkeletalMeshImportContentType;                // 0x0141 (size: 0x1)
    EInterchangeSkeletalMeshContentType LastSkeletalMeshImportContentType;            // 0x0142 (size: 0x1)
    bool bCombineSkeletalMeshes;                                                      // 0x0143 (size: 0x1)
    bool bImportMorphTargets;                                                         // 0x0144 (size: 0x1)
    bool bUpdateSkeletonReferencePose;                                                // 0x0145 (size: 0x1)
    bool bCreatePhysicsAsset;                                                         // 0x0146 (size: 0x1)
    TWeakObjectPtr<class UPhysicsAsset> PhysicsAsset;                                 // 0x0148 (size: 0x8)
    bool bUseHighPrecisionSkinWeights;                                                // 0x0150 (size: 0x1)
    float ThresholdPosition;                                                          // 0x0154 (size: 0x4)
    float ThresholdTangentNormal;                                                     // 0x0158 (size: 0x4)
    float ThresholdUV;                                                                // 0x015C (size: 0x4)
    float MorphThresholdPosition;                                                     // 0x0160 (size: 0x4)
    int32 BoneInfluenceLimit;                                                         // 0x0164 (size: 0x4)

}; // Size: 0x1B8

class UInterchangeGenericTexturePipeline : public UInterchangePipelineBase
{
    bool bImportTextures;                                                             // 0x00E8 (size: 0x1)
    FString AssetName;                                                                // 0x00F0 (size: 0x10)
    bool bAllowNonPowerOfTwo;                                                         // 0x0100 (size: 0x1)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0108 (size: 0x8)

}; // Size: 0x140

class UInterchangeMaterialXPipeline : public UInterchangePipelineBase
{
}; // Size: 0xF0

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
    class UInterchangePipelineMeshesUtilities* CreateInterchangePipelineMeshesUtilities(class UInterchangeBaseNodeContainer* BaseNodeContainer);
}; // Size: 0x128

class UMaterialXPipelineSettings : public UDeveloperSettings
{
    TMap<class EInterchangeMaterialXShaders, class FSoftObjectPath> PredefinedSurfaceShaders; // 0x0038 (size: 0x50)

}; // Size: 0x88

#endif
