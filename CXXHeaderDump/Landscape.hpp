#ifndef UE4SS_SDK_Landscape_HPP
#define UE4SS_SDK_Landscape_HPP

#include "Landscape_enums.hpp"

struct FForeignControlPointData
{
}; // Size: 0x1

struct FForeignSplineSegmentData
{
}; // Size: 0x1

struct FForeignWorldSplineData
{
}; // Size: 0x1

struct FGizmoSelectData
{
}; // Size: 0x50

struct FGrassInput
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    class ULandscapeGrassType* GrassType;                                             // 0x0008 (size: 0x8)
    FExpressionInput Input;                                                           // 0x0010 (size: 0x28)

}; // Size: 0x38

struct FGrassVariety
{
    class UStaticMesh* GrassMesh;                                                     // 0x0000 (size: 0x8)
    TArray<class UMaterialInterface*> OverrideMaterials;                              // 0x0008 (size: 0x10)
    FPerPlatformFloat GrassDensity;                                                   // 0x0018 (size: 0x4)
    FPerQualityLevelFloat GrassDensityQuality;                                        // 0x0020 (size: 0x68)
    bool bUseGrid;                                                                    // 0x0088 (size: 0x1)
    float PlacementJitter;                                                            // 0x008C (size: 0x4)
    FPerPlatformInt StartCullDistance;                                                // 0x0090 (size: 0x4)
    FPerQualityLevelInt StartCullDistanceQuality;                                     // 0x0098 (size: 0x68)
    FPerPlatformInt EndCullDistance;                                                  // 0x0100 (size: 0x4)
    FPerQualityLevelInt EndCullDistanceQuality;                                       // 0x0108 (size: 0x68)
    int32 MinLOD;                                                                     // 0x0170 (size: 0x4)
    EGrassScaling Scaling;                                                            // 0x0174 (size: 0x1)
    FFloatInterval ScaleX;                                                            // 0x0178 (size: 0x8)
    FFloatInterval ScaleY;                                                            // 0x0180 (size: 0x8)
    FFloatInterval ScaleZ;                                                            // 0x0188 (size: 0x8)
    bool RandomRotation;                                                              // 0x0190 (size: 0x1)
    bool AlignToSurface;                                                              // 0x0191 (size: 0x1)
    bool bUseLandscapeLightmap;                                                       // 0x0192 (size: 0x1)
    FLightingChannels LightingChannels;                                               // 0x0193 (size: 0x1)
    bool bReceivesDecals;                                                             // 0x0194 (size: 0x1)
    bool bAffectDistanceFieldLighting;                                                // 0x0195 (size: 0x1)
    bool bCastDynamicShadow;                                                          // 0x0196 (size: 0x1)
    bool bCastContactShadow;                                                          // 0x0197 (size: 0x1)
    bool bKeepInstanceBufferCPUCopy;                                                  // 0x0198 (size: 0x1)
    uint32 InstanceWorldPositionOffsetDisableDistance;                                // 0x019C (size: 0x4)
    EShadowCacheInvalidationBehavior ShadowCacheInvalidationBehavior;                 // 0x01A0 (size: 0x1)

}; // Size: 0x1A8

struct FHeightmapData
{
    class UTexture2D* Texture;                                                        // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FLandscapeBrushParameters
{
    ELandscapeToolTargetType LayerType;                                               // 0x0000 (size: 0x1)
    class UTextureRenderTarget2D* CombinedResult;                                     // 0x0008 (size: 0x8)
    FName WeightmapLayerName;                                                         // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FLandscapeComponentMaterialOverride
{
    FPerPlatformInt LODIndex;                                                         // 0x0000 (size: 0x4)
    class UMaterialInterface* Material;                                               // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FLandscapeEditToolRenderData
{
    class UMaterialInterface* ToolMaterial;                                           // 0x0000 (size: 0x8)
    class UMaterialInterface* GizmoMaterial;                                          // 0x0008 (size: 0x8)
    int32 SelectedType;                                                               // 0x0010 (size: 0x4)
    int32 DebugChannelR;                                                              // 0x0014 (size: 0x4)
    int32 DebugChannelG;                                                              // 0x0018 (size: 0x4)
    int32 DebugChannelB;                                                              // 0x001C (size: 0x4)
    class UTexture2D* DataTexture;                                                    // 0x0020 (size: 0x8)
    class UTexture2D* LayerContributionTexture;                                       // 0x0028 (size: 0x8)
    class UTexture2D* DirtyTexture;                                                   // 0x0030 (size: 0x8)

}; // Size: 0x38

struct FLandscapeEditorLayerSettings
{
}; // Size: 0x1

struct FLandscapeImportLayerInfo
{
}; // Size: 0x1

struct FLandscapeInfoLayerSettings
{
    class ULandscapeLayerInfoObject* LayerInfoObj;                                    // 0x0000 (size: 0x8)
    FName LayerName;                                                                  // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FLandscapeLayer
{
    FGuid Guid;                                                                       // 0x0000 (size: 0x10)
    FName Name;                                                                       // 0x0010 (size: 0x8)
    bool bVisible;                                                                    // 0x0018 (size: 0x1)
    bool bLocked;                                                                     // 0x0019 (size: 0x1)
    float HeightmapAlpha;                                                             // 0x001C (size: 0x4)
    float WeightmapAlpha;                                                             // 0x0020 (size: 0x4)
    TEnumAsByte<ELandscapeBlendMode> BlendMode;                                       // 0x0024 (size: 0x1)
    TArray<FLandscapeLayerBrush> Brushes;                                             // 0x0028 (size: 0x10)
    TMap<class ULandscapeLayerInfoObject*, class bool> WeightmapLayerAllocationBlend; // 0x0038 (size: 0x50)

}; // Size: 0x88

struct FLandscapeLayerBrush
{
}; // Size: 0x1

struct FLandscapeLayerComponentData
{
    FHeightmapData HeightmapData;                                                     // 0x0000 (size: 0x8)
    FWeightmapData WeightmapData;                                                     // 0x0008 (size: 0x30)

}; // Size: 0x38

struct FLandscapeLayerStruct
{
    class ULandscapeLayerInfoObject* LayerInfoObj;                                    // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FLandscapeMaterialTextureStreamingInfo
{
    FName TextureName;                                                                // 0x0000 (size: 0x8)
    float TexelFactor;                                                                // 0x0008 (size: 0x4)

}; // Size: 0xC

struct FLandscapePerLODMaterialOverride
{
    int32 LODIndex;                                                                   // 0x0000 (size: 0x4)
    class UMaterialInterface* Material;                                               // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FLandscapeProxyMaterialOverride
{
    FPerPlatformInt LODIndex;                                                         // 0x0000 (size: 0x4)
    class UMaterialInterface* Material;                                               // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FLandscapeSplineConnection
{
    class ULandscapeSplineSegment* Segment;                                           // 0x0000 (size: 0x8)
    uint8 End;                                                                        // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FLandscapeSplineInterpPoint
{
    FVector Center;                                                                   // 0x0000 (size: 0x18)
    FVector Left;                                                                     // 0x0018 (size: 0x18)
    FVector Right;                                                                    // 0x0030 (size: 0x18)
    FVector FalloffLeft;                                                              // 0x0048 (size: 0x18)
    FVector FalloffRight;                                                             // 0x0060 (size: 0x18)
    FVector LayerLeft;                                                                // 0x0078 (size: 0x18)
    FVector LayerRight;                                                               // 0x0090 (size: 0x18)
    FVector LayerFalloffLeft;                                                         // 0x00A8 (size: 0x18)
    FVector LayerFalloffRight;                                                        // 0x00C0 (size: 0x18)
    float StartEndFalloff;                                                            // 0x00D8 (size: 0x4)

}; // Size: 0xE0

struct FLandscapeSplineMeshEntry
{
    class UStaticMesh* Mesh;                                                          // 0x0000 (size: 0x8)
    TArray<class UMaterialInterface*> MaterialOverrides;                              // 0x0008 (size: 0x10)
    uint8 bCenterH;                                                                   // 0x0018 (size: 0x1)
    FVector2D CenterAdjust;                                                           // 0x0020 (size: 0x10)
    uint8 bScaleToWidth;                                                              // 0x0030 (size: 0x1)
    FVector Scale;                                                                    // 0x0038 (size: 0x18)
    TEnumAsByte<LandscapeSplineMeshOrientation> Orientation;                          // 0x0050 (size: 0x1)
    TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;                                   // 0x0051 (size: 0x1)
    TEnumAsByte<ESplineMeshAxis::Type> UpAxis;                                        // 0x0052 (size: 0x1)

}; // Size: 0x58

struct FLandscapeSplineSegmentConnection
{
    class ULandscapeSplineControlPoint* ControlPoint;                                 // 0x0000 (size: 0x8)
    float TangentLen;                                                                 // 0x0008 (size: 0x4)
    FName SocketName;                                                                 // 0x000C (size: 0x8)

}; // Size: 0x18

struct FLandscapeTexture2DMipMap
{
    int32 SizeX;                                                                      // 0x0000 (size: 0x4)
    int32 SizeY;                                                                      // 0x0004 (size: 0x4)
    bool bCompressed;                                                                 // 0x0008 (size: 0x1)

}; // Size: 0x38

struct FLayerBlendInput
{
    FName LayerName;                                                                  // 0x0000 (size: 0x8)
    TEnumAsByte<ELandscapeLayerBlendType> BlendType;                                  // 0x0008 (size: 0x1)
    FExpressionInput LayerInput;                                                      // 0x0010 (size: 0x28)
    FExpressionInput HeightInput;                                                     // 0x0038 (size: 0x28)
    float PreviewWeight;                                                              // 0x0060 (size: 0x4)
    FVector ConstLayerInput;                                                          // 0x0068 (size: 0x18)
    float ConstHeightInput;                                                           // 0x0080 (size: 0x4)

}; // Size: 0x88

struct FPhysicalMaterialInput
{
    class UPhysicalMaterial* PhysicalMaterial;                                        // 0x0000 (size: 0x8)
    FExpressionInput Input;                                                           // 0x0008 (size: 0x28)

}; // Size: 0x30

struct FWeightmapData
{
    TArray<class UTexture2D*> Textures;                                               // 0x0000 (size: 0x10)
    TArray<FWeightmapLayerAllocationInfo> LayerAllocations;                           // 0x0010 (size: 0x10)
    TArray<class ULandscapeWeightmapUsage*> TextureUsages;                            // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FWeightmapLayerAllocationInfo
{
    class ULandscapeLayerInfoObject* LayerInfo;                                       // 0x0000 (size: 0x8)
    uint8 WeightmapTextureIndex;                                                      // 0x0008 (size: 0x1)
    uint8 WeightmapTextureChannel;                                                    // 0x0009 (size: 0x1)

}; // Size: 0x10

class AControlPointMeshActor : public AActor
{
    class UControlPointMeshComponent* ControlPointMeshComponent;                      // 0x0298 (size: 0x8)

}; // Size: 0x2A0

class ALandscape : public ALandscapeProxy
{

    void RenderHeightmap(const FTransform& InWorldTransform, const FBox2D& InExtents, class UTextureRenderTarget2D* OutRenderTarget);
}; // Size: 0x620

class ALandscapeBlueprintBrushBase : public AActor
{

    void RequestLandscapeUpdate(bool bInUserTriggered);
    class UTextureRenderTarget2D* RenderLayer(const FLandscapeBrushParameters& InParameters);
    class UTextureRenderTarget2D* Render(bool InIsHeightmap, class UTextureRenderTarget2D* InCombinedResult, const FName& InWeightmapLayerName);
    void Initialize(const FTransform& InLandscapeTransform, const FIntPoint& InLandscapeSize, const FIntPoint& InLandscapeRenderTargetSize);
    void GetBlueprintRenderDependencies(TArray<class UObject*>& OutStreamableAssets);
}; // Size: 0x298

class ALandscapeGizmoActiveActor : public ALandscapeGizmoActor
{
}; // Size: 0x2E8

class ALandscapeGizmoActor : public AActor
{
}; // Size: 0x298

class ALandscapeMeshProxyActor : public AActor
{
    class ULandscapeMeshProxyComponent* LandscapeMeshProxyComponent;                  // 0x0298 (size: 0x8)

}; // Size: 0x2A0

class ALandscapeProxy : public APartitionActor
{
    class ULandscapeSplinesComponent* SplineComponent;                                // 0x02A0 (size: 0x8)
    FGuid LandscapeGuid;                                                              // 0x02A8 (size: 0x10)
    bool bEnableNanite;                                                               // 0x02C8 (size: 0x1)
    TArray<FLandscapePerLODMaterialOverride> PerLODOverrideMaterials;                 // 0x02D0 (size: 0x10)
    FIntPoint LandscapeSectionOffset;                                                 // 0x02E0 (size: 0x8)
    int32 MaxLODLevel;                                                                // 0x02E8 (size: 0x4)
    float ComponentScreenSizeToUseSubSections;                                        // 0x02EC (size: 0x4)
    float LOD0ScreenSize;                                                             // 0x02F0 (size: 0x4)
    uint32 LODGroupKey;                                                               // 0x02F4 (size: 0x4)
    float LOD0DistributionSetting;                                                    // 0x02F8 (size: 0x4)
    float LODDistributionSetting;                                                     // 0x02FC (size: 0x4)
    int32 StaticLightingLOD;                                                          // 0x0300 (size: 0x4)
    class UPhysicalMaterial* DefaultPhysMaterial;                                     // 0x0308 (size: 0x8)
    float StreamingDistanceMultiplier;                                                // 0x0310 (size: 0x4)
    class UMaterialInterface* LandscapeMaterial;                                      // 0x0318 (size: 0x8)
    class UMaterialInterface* LandscapeHoleMaterial;                                  // 0x0340 (size: 0x8)
    TArray<class URuntimeVirtualTexture*> RuntimeVirtualTextures;                     // 0x0348 (size: 0x10)
    bool bSetCreateRuntimeVirtualTextureVolumes;                                      // 0x0358 (size: 0x1)
    bool bVirtualTextureRenderWithQuad;                                               // 0x0359 (size: 0x1)
    bool bVirtualTextureRenderWithQuadHQ;                                             // 0x035A (size: 0x1)
    int32 VirtualTextureNumLods;                                                      // 0x035C (size: 0x4)
    int32 VirtualTextureLodBias;                                                      // 0x0360 (size: 0x4)
    ERuntimeVirtualTextureMainPassType VirtualTextureRenderPassType;                  // 0x0364 (size: 0x1)
    float NegativeZBoundsExtension;                                                   // 0x0368 (size: 0x4)
    float PositiveZBoundsExtension;                                                   // 0x036C (size: 0x4)
    TArray<class ULandscapeComponent*> LandscapeComponents;                           // 0x0370 (size: 0x10)
    TArray<class ULandscapeHeightfieldCollisionComponent*> CollisionComponents;       // 0x0380 (size: 0x10)
    TArray<class UHierarchicalInstancedStaticMeshComponent*> FoliageComponents;       // 0x0390 (size: 0x10)
    class ULandscapeNaniteComponent* NaniteComponent;                                 // 0x03A0 (size: 0x8)
    TArray<class ULandscapeNaniteComponent*> NaniteComponents;                        // 0x03A8 (size: 0x10)
    bool bHasLandscapeGrass;                                                          // 0x041C (size: 0x1)
    float StaticLightingResolution;                                                   // 0x0420 (size: 0x4)
    uint8 CastShadow;                                                                 // 0x0424 (size: 0x1)
    uint8 bCastDynamicShadow;                                                         // 0x0424 (size: 0x1)
    uint8 bCastStaticShadow;                                                          // 0x0424 (size: 0x1)
    EShadowCacheInvalidationBehavior ShadowCacheInvalidationBehavior;                 // 0x0425 (size: 0x1)
    uint8 bCastContactShadow;                                                         // 0x0426 (size: 0x1)
    uint8 bCastFarShadow;                                                             // 0x0428 (size: 0x1)
    uint8 bCastHiddenShadow;                                                          // 0x042C (size: 0x1)
    uint8 bCastShadowAsTwoSided;                                                      // 0x0430 (size: 0x1)
    uint8 bAffectDistanceFieldLighting;                                               // 0x0434 (size: 0x1)
    FLightingChannels LightingChannels;                                               // 0x0435 (size: 0x1)
    uint8 bUseMaterialPositionOffsetInStaticLighting;                                 // 0x0438 (size: 0x1)
    uint8 bRenderCustomDepth;                                                         // 0x0438 (size: 0x1)
    ERendererStencilMask CustomDepthStencilWriteMask;                                 // 0x043C (size: 0x1)
    int32 CustomDepthStencilValue;                                                    // 0x0440 (size: 0x4)
    float LDMaxDrawDistance;                                                          // 0x0444 (size: 0x4)
    FLightmassPrimitiveSettings LightmassSettings;                                    // 0x0448 (size: 0x18)
    int32 CollisionMipLevel;                                                          // 0x0460 (size: 0x4)
    int32 SimpleCollisionMipLevel;                                                    // 0x0464 (size: 0x4)
    FBodyInstance BodyInstance;                                                       // 0x0470 (size: 0x190)
    uint8 bGenerateOverlapEvents;                                                     // 0x0600 (size: 0x1)
    uint8 bBakeMaterialPositionOffsetIntoCollision;                                   // 0x0600 (size: 0x1)
    int32 ComponentSizeQuads;                                                         // 0x0604 (size: 0x4)
    int32 SubsectionSizeQuads;                                                        // 0x0608 (size: 0x4)
    int32 NumSubsections;                                                             // 0x060C (size: 0x4)
    uint8 bUsedForNavigation;                                                         // 0x0610 (size: 0x1)
    uint8 bFillCollisionUnderLandscapeForNavmesh;                                     // 0x0610 (size: 0x1)
    ENavDataGatheringMode NavigationGeometryGatheringMode;                            // 0x0614 (size: 0x1)
    bool bUseDynamicMaterialInstance;                                                 // 0x0615 (size: 0x1)
    bool bUseLandscapeForCullingInvisibleHLODVertices;                                // 0x0616 (size: 0x1)
    bool bHasLayersContent;                                                           // 0x0617 (size: 0x1)
    bool bUseCompressedHeightmapStorage;                                              // 0x0618 (size: 0x1)

    void SetVirtualTextureRenderPassType(ERuntimeVirtualTextureMainPassType InType);
    void SetLandscapeMaterialVectorParameterValue(FName ParameterName, FLinearColor Value);
    void SetLandscapeMaterialTextureParameterValue(FName ParameterName, class UTexture* Value);
    void SetLandscapeMaterialScalarParameterValue(FName ParameterName, float Value);
    bool LandscapeExportHeightmapToRenderTarget(class UTextureRenderTarget2D* InRenderTarget, bool InExportHeightIntoRGChannel, bool InExportLandscapeProxies);
    class ALandscape* GetLandscapeActor();
    void EditorSetLandscapeMaterial(class UMaterialInterface* NewLandscapeMaterial);
    void EditorApplySpline(class USplineComponent* InSplineComponent, float StartWidth, float EndWidth, float StartSideFalloff, float EndSideFalloff, float StartRoll, float EndRoll, int32 NumSubdivisions, bool bRaiseHeights, bool bLowerHeights, class ULandscapeLayerInfoObject* PaintLayer, FName EditLayerName);
    void ChangeLODDistanceFactor(float InLODDistanceFactor);
    void ChangeComponentScreenSizeToUseSubSections(float InComponentScreenSizeToUseSubSections);
}; // Size: 0x620

class ALandscapeSplineActor : public AActor
{
    FGuid LandscapeGuid;                                                              // 0x02A0 (size: 0x10)

}; // Size: 0x2B0

class ALandscapeSplineMeshesActor : public APartitionActor
{
    TArray<class UStaticMeshComponent*> StaticMeshComponents;                         // 0x0298 (size: 0x10)

}; // Size: 0x2A8

class ALandscapeStreamingProxy : public ALandscapeProxy
{
    TSoftObjectPtr<ALandscape> LandscapeActorRef;                                     // 0x0620 (size: 0x28)
    TSet<FString> OverriddenSharedProperties;                                         // 0x0648 (size: 0x50)

}; // Size: 0x698

class ILandscapeSplineInterface : public IInterface
{
}; // Size: 0x28

class UControlPointMeshComponent : public UStaticMeshComponent
{
    float VirtualTextureMainPassMaxDrawDistance;                                      // 0x0628 (size: 0x4)

}; // Size: 0x630

class ULandscapeComponent : public UPrimitiveComponent
{
    int32 SectionBaseX;                                                               // 0x0570 (size: 0x4)
    int32 SectionBaseY;                                                               // 0x0574 (size: 0x4)
    int32 ComponentSizeQuads;                                                         // 0x0578 (size: 0x4)
    int32 SubsectionSizeQuads;                                                        // 0x057C (size: 0x4)
    int32 NumSubsections;                                                             // 0x0580 (size: 0x4)
    class UMaterialInterface* OverrideMaterial;                                       // 0x0588 (size: 0x8)
    class UMaterialInterface* OverrideHoleMaterial;                                   // 0x0590 (size: 0x8)
    TArray<class UMaterialInstanceConstant*> MaterialInstances;                       // 0x0598 (size: 0x10)
    TArray<class UMaterialInstanceDynamic*> MaterialInstancesDynamic;                 // 0x05A8 (size: 0x10)
    TArray<int8> LODIndexToMaterialIndex;                                             // 0x05B8 (size: 0x10)
    class UTexture2D* XYOffsetmapTexture;                                             // 0x05C8 (size: 0x8)
    FVector4 WeightmapScaleBias;                                                      // 0x05D0 (size: 0x20)
    float WeightmapSubsectionOffset;                                                  // 0x05F0 (size: 0x4)
    FVector4 HeightmapScaleBias;                                                      // 0x0600 (size: 0x20)
    FBox CachedLocalBox;                                                              // 0x0620 (size: 0x38)
    class ULandscapeHeightfieldCollisionComponent* CollisionComponentRef;             // 0x0658 (size: 0x8)
    bool bUserTriggeredChangeRequested;                                               // 0x0660 (size: 0x1)
    bool bNaniteActive;                                                               // 0x0661 (size: 0x1)
    class UTexture2D* HeightmapTexture;                                               // 0x0668 (size: 0x8)
    TArray<FWeightmapLayerAllocationInfo> WeightmapLayerAllocations;                  // 0x0670 (size: 0x10)
    TArray<class UTexture2D*> WeightmapTextures;                                      // 0x0680 (size: 0x10)
    TArray<FLandscapePerLODMaterialOverride> PerLODOverrideMaterials;                 // 0x0690 (size: 0x10)
    FGuid MapBuildDataId;                                                             // 0x06A0 (size: 0x10)
    int32 CollisionMipLevel;                                                          // 0x06B0 (size: 0x4)
    int32 SimpleCollisionMipLevel;                                                    // 0x06B4 (size: 0x4)
    float NegativeZBoundsExtension;                                                   // 0x06B8 (size: 0x4)
    float PositiveZBoundsExtension;                                                   // 0x06BC (size: 0x4)
    float StaticLightingResolution;                                                   // 0x06C0 (size: 0x4)
    int32 ForcedLOD;                                                                  // 0x06C4 (size: 0x4)
    int32 LODBias;                                                                    // 0x06C8 (size: 0x4)
    FGuid StateId;                                                                    // 0x06CC (size: 0x10)
    class UMaterialInterface* MobileMaterialInterface;                                // 0x0700 (size: 0x8)
    TArray<class UMaterialInterface*> MobileMaterialInterfaces;                       // 0x0708 (size: 0x10)
    TArray<class UTexture2D*> MobileWeightmapTextures;                                // 0x0718 (size: 0x10)
    TArray<FWeightmapLayerAllocationInfo> MobileWeightmapLayerAllocations;            // 0x0728 (size: 0x10)

    void SetLODBias(int32 InLODBias);
    void SetForcedLOD(int32 InForcedLOD);
    class UMaterialInstanceDynamic* GetMaterialInstanceDynamic(int32 InIndex);
    float EditorGetPaintLayerWeightByNameAtLocation(const FVector& InLocation, const FName InPaintLayerName);
    float EditorGetPaintLayerWeightAtLocation(const FVector& InLocation, class ULandscapeLayerInfoObject* PaintLayer);
}; // Size: 0x760

class ULandscapeGizmoRenderComponent : public UPrimitiveComponent
{
}; // Size: 0x570

class ULandscapeGrassType : public UObject
{
    TArray<FGrassVariety> GrassVarieties;                                             // 0x0028 (size: 0x10)
    uint8 bEnableDensityScaling;                                                      // 0x0038 (size: 0x1)
    class UStaticMesh* GrassMesh;                                                     // 0x0040 (size: 0x8)
    float GrassDensity;                                                               // 0x0048 (size: 0x4)
    float PlacementJitter;                                                            // 0x004C (size: 0x4)
    int32 StartCullDistance;                                                          // 0x0050 (size: 0x4)
    int32 EndCullDistance;                                                            // 0x0054 (size: 0x4)
    bool RandomRotation;                                                              // 0x0058 (size: 0x1)
    bool AlignToSurface;                                                              // 0x0059 (size: 0x1)

}; // Size: 0x60

class ULandscapeHLODBuilder : public UHLODBuilder
{
}; // Size: 0x28

class ULandscapeHeightfieldCollisionComponent : public UPrimitiveComponent
{
    TArray<class ULandscapeLayerInfoObject*> ComponentLayerInfos;                     // 0x0570 (size: 0x10)
    int32 SectionBaseX;                                                               // 0x0580 (size: 0x4)
    int32 SectionBaseY;                                                               // 0x0584 (size: 0x4)
    int32 CollisionSizeQuads;                                                         // 0x0588 (size: 0x4)
    float CollisionScale;                                                             // 0x058C (size: 0x4)
    int32 SimpleCollisionSizeQuads;                                                   // 0x0590 (size: 0x4)
    TArray<uint8> CollisionQuadFlags;                                                 // 0x0598 (size: 0x10)
    FGuid HeightfieldGuid;                                                            // 0x05A8 (size: 0x10)
    FBox CachedLocalBox;                                                              // 0x05B8 (size: 0x38)
    class ULandscapeComponent* RenderComponentRef;                                    // 0x05F0 (size: 0x8)
    TArray<class UPhysicalMaterial*> CookedPhysicalMaterials;                         // 0x0608 (size: 0x10)

    class ULandscapeComponent* GetRenderComponent();
}; // Size: 0x660

class ULandscapeInfo : public UObject
{
    TWeakObjectPtr<class ALandscape> LandscapeActor;                                  // 0x0028 (size: 0x8)
    FGuid LandscapeGuid;                                                              // 0x0030 (size: 0x10)
    int32 ComponentSizeQuads;                                                         // 0x0040 (size: 0x4)
    int32 SubsectionSizeQuads;                                                        // 0x0044 (size: 0x4)
    int32 ComponentNumSubsections;                                                    // 0x0048 (size: 0x4)
    FVector DrawScale;                                                                // 0x0050 (size: 0x18)
    TArray<TWeakObjectPtr<ALandscapeStreamingProxy>> StreamingProxies;                // 0x0118 (size: 0x10)

}; // Size: 0x228

class ULandscapeInfoMap : public UObject
{
}; // Size: 0x80

class ULandscapeLODStreamingProxy_DEPRECATED : public UStreamableRenderAsset
{
}; // Size: 0xC8

class ULandscapeLayerInfoObject : public UObject
{
    FName LayerName;                                                                  // 0x0028 (size: 0x8)
    class UPhysicalMaterial* PhysMaterial;                                            // 0x0030 (size: 0x8)
    float Hardness;                                                                   // 0x0038 (size: 0x4)
    FLinearColor LayerUsageDebugColor;                                                // 0x003C (size: 0x10)

}; // Size: 0x50

class ULandscapeMaterialInstanceConstant : public UMaterialInstanceConstant
{
    TArray<FLandscapeMaterialTextureStreamingInfo> TextureStreamingInfo;              // 0x0250 (size: 0x10)
    uint8 bIsLayerThumbnail;                                                          // 0x0260 (size: 0x1)
    uint8 bDisableTessellation;                                                       // 0x0260 (size: 0x1)
    uint8 bMobile;                                                                    // 0x0260 (size: 0x1)
    uint8 bEditorToolUsage;                                                           // 0x0260 (size: 0x1)

}; // Size: 0x268

class ULandscapeMeshCollisionComponent : public ULandscapeHeightfieldCollisionComponent
{
    FGuid MeshGuid;                                                                   // 0x0658 (size: 0x10)

}; // Size: 0x670

class ULandscapeMeshProxyComponent : public UStaticMeshComponent
{
    FGuid LandscapeGuid;                                                              // 0x0628 (size: 0x10)
    TArray<FIntPoint> ProxyComponentBases;                                            // 0x0638 (size: 0x10)
    int8 ProxyLOD;                                                                    // 0x0648 (size: 0x1)
    uint32 LODGroupKey;                                                               // 0x064C (size: 0x4)

}; // Size: 0x650

class ULandscapeNaniteComponent : public UStaticMeshComponent
{
    FGuid ProxyContentId;                                                             // 0x0628 (size: 0x10)
    bool bEnabled;                                                                    // 0x0638 (size: 0x1)

}; // Size: 0x640

class ULandscapeSettings : public UDeveloperSettings
{
    int32 MaxNumberOfLayers;                                                          // 0x0038 (size: 0x4)
    int32 MaxComponents;                                                              // 0x003C (size: 0x4)
    uint32 MaxImageImportCacheSizeMegaBytes;                                          // 0x0040 (size: 0x4)
    float PaintStrengthGamma;                                                         // 0x0044 (size: 0x4)
    bool bDisablePaintingStartupSlowdown;                                             // 0x0048 (size: 0x1)
    ELandscapeDirtyingMode LandscapeDirtyingMode;                                     // 0x0049 (size: 0x1)
    int32 SideResolutionLimit;                                                        // 0x004C (size: 0x4)
    TSoftObjectPtr<UMaterialInterface> DefaultLandscapeMaterial;                      // 0x0050 (size: 0x28)
    TSoftObjectPtr<ULandscapeLayerInfoObject> DefaultLayerInfoObject;                 // 0x0078 (size: 0x28)
    bool bRestrictiveMode;                                                            // 0x00A0 (size: 0x1)

}; // Size: 0xA8

class ULandscapeSplineControlPoint : public UObject
{
    FVector Location;                                                                 // 0x0028 (size: 0x18)
    FRotator Rotation;                                                                // 0x0040 (size: 0x18)
    float Width;                                                                      // 0x0058 (size: 0x4)
    float LayerWidthRatio;                                                            // 0x005C (size: 0x4)
    float SideFalloff;                                                                // 0x0060 (size: 0x4)
    float LeftSideFalloffFactor;                                                      // 0x0064 (size: 0x4)
    float RightSideFalloffFactor;                                                     // 0x0068 (size: 0x4)
    float LeftSideLayerFalloffFactor;                                                 // 0x006C (size: 0x4)
    float RightSideLayerFalloffFactor;                                                // 0x0070 (size: 0x4)
    float EndFalloff;                                                                 // 0x0074 (size: 0x4)
    TArray<FLandscapeSplineConnection> ConnectedSegments;                             // 0x0078 (size: 0x10)
    TArray<FLandscapeSplineInterpPoint> Points;                                       // 0x0088 (size: 0x10)
    FBox Bounds;                                                                      // 0x0098 (size: 0x38)
    class UControlPointMeshComponent* LocalMeshComponent;                             // 0x00D0 (size: 0x8)

}; // Size: 0xD8

class ULandscapeSplineSegment : public UObject
{
    FLandscapeSplineSegmentConnection Connections;                                    // 0x0028 (size: 0x30)
    FInterpCurveVector SplineInfo;                                                    // 0x0058 (size: 0x18)
    TArray<FLandscapeSplineInterpPoint> Points;                                       // 0x0070 (size: 0x10)
    FBox Bounds;                                                                      // 0x0080 (size: 0x38)
    TArray<class USplineMeshComponent*> LocalMeshComponents;                          // 0x00B8 (size: 0x10)

}; // Size: 0xC8

class ULandscapeSplinesComponent : public UPrimitiveComponent
{
    TArray<class ULandscapeSplineControlPoint*> ControlPoints;                        // 0x0570 (size: 0x10)
    TArray<class ULandscapeSplineSegment*> Segments;                                  // 0x0580 (size: 0x10)
    TArray<class UMeshComponent*> CookedForeignMeshComponents;                        // 0x0590 (size: 0x10)

    TArray<class USplineMeshComponent*> GetSplineMeshComponents();
}; // Size: 0x5A0

class ULandscapeSubsystem : public UTickableWorldSubsystem
{
}; // Size: 0x60

class ULandscapeTextureStorageProviderFactory : public UTextureAllMipDataProviderFactory
{
}; // Size: 0x60

class ULandscapeWeightmapUsage : public UObject
{
    class ULandscapeComponent* ChannelUsage;                                          // 0x0028 (size: 0x20)
    FGuid LayerGuid;                                                                  // 0x0048 (size: 0x10)

}; // Size: 0x58

class UMaterialExpressionLandscapeGrassOutput : public UMaterialExpressionCustomOutput
{
    TArray<FGrassInput> GrassTypes;                                                   // 0x00B0 (size: 0x10)

}; // Size: 0xC0

class UMaterialExpressionLandscapeLayerBlend : public UMaterialExpression
{
    TArray<FLayerBlendInput> Layers;                                                  // 0x00B0 (size: 0x10)

}; // Size: 0xC0

class UMaterialExpressionLandscapeLayerCoords : public UMaterialExpression
{
    TEnumAsByte<ETerrainCoordMappingType> MappingType;                                // 0x00B0 (size: 0x1)
    TEnumAsByte<ELandscapeCustomizedCoordType> CustomUVType;                          // 0x00B1 (size: 0x1)
    float MappingScale;                                                               // 0x00B4 (size: 0x4)
    float MappingRotation;                                                            // 0x00B8 (size: 0x4)
    float MappingPanU;                                                                // 0x00BC (size: 0x4)
    float MappingPanV;                                                                // 0x00C0 (size: 0x4)

}; // Size: 0xC8

class UMaterialExpressionLandscapeLayerSample : public UMaterialExpression
{
    FName ParameterName;                                                              // 0x00B0 (size: 0x8)
    float PreviewWeight;                                                              // 0x00B8 (size: 0x4)

}; // Size: 0xC0

class UMaterialExpressionLandscapeLayerSwitch : public UMaterialExpression
{
    FExpressionInput LayerUsed;                                                       // 0x00B0 (size: 0x28)
    FExpressionInput LayerNotUsed;                                                    // 0x00D8 (size: 0x28)
    FName ParameterName;                                                              // 0x0100 (size: 0x8)
    uint8 PreviewUsed;                                                                // 0x0108 (size: 0x1)

}; // Size: 0x110

class UMaterialExpressionLandscapeLayerWeight : public UMaterialExpression
{
    FExpressionInput base;                                                            // 0x00B0 (size: 0x28)
    FExpressionInput Layer;                                                           // 0x00D8 (size: 0x28)
    FName ParameterName;                                                              // 0x0100 (size: 0x8)
    float PreviewWeight;                                                              // 0x0108 (size: 0x4)
    FVector ConstBase;                                                                // 0x0110 (size: 0x18)

}; // Size: 0x128

class UMaterialExpressionLandscapePhysicalMaterialOutput : public UMaterialExpressionCustomOutput
{
    TArray<FPhysicalMaterialInput> Inputs;                                            // 0x00B0 (size: 0x10)

}; // Size: 0xC0

class UMaterialExpressionLandscapeVisibilityMask : public UMaterialExpression
{
}; // Size: 0xB0

#endif
