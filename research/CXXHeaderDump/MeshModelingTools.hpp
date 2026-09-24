#ifndef UE4SS_SDK_MeshModelingTools_HPP
#define UE4SS_SDK_MeshModelingTools_HPP

#include "MeshModelingTools_enums.hpp"

struct FBrushToolRadius
{
    EBrushToolSizeType SizeType;                                                      // 0x0000 (size: 0x1)
    float AdaptiveSize;                                                               // 0x0004 (size: 0x4)
    float WorldRadius;                                                                // 0x0008 (size: 0x4)
    bool bToolSupportsPressureSensitivity;                                            // 0x000C (size: 0x1)
    bool bEnablePressureSensitivity;                                                  // 0x000D (size: 0x1)

}; // Size: 0x18

struct FPerlinLayerProperties
{
    float Frequency;                                                                  // 0x0000 (size: 0x4)
    float Intensity;                                                                  // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAddArrowPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddBoxPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddCapsulePrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddConePrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddCylinderPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddDiscPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddPrimitiveTool : public USingleClickTool
{
    class UCreateMeshObjectTypeProperties* OutputTypeProperties;                      // 0x00B8 (size: 0x8)
    class UProceduralShapeToolProperties* ShapeSettings;                              // 0x00C0 (size: 0x8)
    class UNewMeshMaterialProperties* MaterialProperties;                             // 0x00C8 (size: 0x8)
    class UPreviewMesh* PreviewMesh;                                                  // 0x00D0 (size: 0x8)
    class UCombinedTransformGizmo* Gizmo;                                             // 0x00D8 (size: 0x8)
    class UDragAlignmentMechanic* DragAlignmentMechanic;                              // 0x00E0 (size: 0x8)
    FString AssetName;                                                                // 0x00E8 (size: 0x10)

}; // Size: 0x138

class UAddPrimitiveToolBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x30

class UAddRectanglePrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddSpherePrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddStairsPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UAddTorusPrimitiveTool : public UAddPrimitiveTool
{
}; // Size: 0x138

class UBaseKelvinletBrushOpProps : public UMeshSculptBrushOpProps
{
    float Stiffness;                                                                  // 0x00B0 (size: 0x4)
    float Incompressiblity;                                                           // 0x00B4 (size: 0x4)
    int32 BrushSteps;                                                                 // 0x00B8 (size: 0x4)

}; // Size: 0xC0

class UBasePlaneBrushOpProps : public UMeshSculptBrushOpProps
{
}; // Size: 0xB0

class UBaseSmoothBrushOpProps : public UMeshSculptBrushOpProps
{
}; // Size: 0xB0

class UBrushRemeshProperties : public URemeshProperties
{
    bool bEnableRemeshing;                                                            // 0x00B8 (size: 0x1)
    int32 TriangleSize;                                                               // 0x00BC (size: 0x4)
    int32 PreserveDetail;                                                             // 0x00C0 (size: 0x4)
    int32 Iterations;                                                                 // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class UCSGMeshesTool : public UBaseCreateFromSelectedTool
{
    class UCSGMeshesToolProperties* CSGProperties;                                    // 0x0108 (size: 0x8)
    class UTrimMeshesToolProperties* TrimProperties;                                  // 0x0110 (size: 0x8)
    TArray<UPreviewMesh*> OriginalMeshPreviews;                                       // 0x0128 (size: 0x10)
    class UMaterialInstanceDynamic* PreviewsGhostMaterial;                            // 0x0138 (size: 0x8)
    class ULineSetComponent* DrawnLineSet;                                            // 0x0140 (size: 0x8)

}; // Size: 0x160

class UCSGMeshesToolBuilder : public UBaseCreateFromSelectedToolBuilder
{
}; // Size: 0x30

class UCSGMeshesToolProperties : public UInteractiveToolPropertySet
{
    ECSGOperation Operation;                                                          // 0x00A8 (size: 0x1)
    bool bTryFixHoles;                                                                // 0x00A9 (size: 0x1)
    bool bTryCollapseEdges;                                                           // 0x00AA (size: 0x1)
    float WindingThreshold;                                                           // 0x00AC (size: 0x4)
    bool bShowNewBoundaries;                                                          // 0x00B0 (size: 0x1)
    bool bShowSubtractedMesh;                                                         // 0x00B1 (size: 0x1)
    float SubtractedMeshOpacity;                                                      // 0x00B4 (size: 0x4)
    FLinearColor SubtractedMeshColor;                                                 // 0x00B8 (size: 0x10)
    bool bUseFirstMeshMaterials;                                                      // 0x00C8 (size: 0x1)

}; // Size: 0xD0

class UCombineMeshesTool : public UMultiSelectionMeshEditingTool
{
    class UCombineMeshesToolProperties* BasicProperties;                              // 0x00C0 (size: 0x8)
    class UCreateMeshObjectTypeProperties* OutputTypeProperties;                      // 0x00C8 (size: 0x8)
    class UOnAcceptHandleSourcesPropertiesBase* HandleSourceProperties;               // 0x00D0 (size: 0x8)

}; // Size: 0xE0

class UCombineMeshesToolBuilder : public UMultiSelectionMeshEditingToolBuilder
{
}; // Size: 0x30

class UCombineMeshesToolProperties : public UInteractiveToolPropertySet
{
    bool bIsDuplicateMode;                                                            // 0x00A8 (size: 0x1)
    EBaseCreateFromSelectedTargetType OutputWriteTo;                                  // 0x00AC (size: 0x4)
    FString OutputNewName;                                                            // 0x00B0 (size: 0x10)
    FString OutputExistingName;                                                       // 0x00C0 (size: 0x10)

}; // Size: 0xD0

class UConvertToPolygonsOperatorFactory : public UObject
{
    class UConvertToPolygonsTool* ConvertToPolygonsTool;                              // 0x0030 (size: 0x8)

}; // Size: 0x38

class UConvertToPolygonsTool : public USingleTargetWithSelectionTool
{
    class UConvertToPolygonsToolProperties* Settings;                                 // 0x0120 (size: 0x8)
    class UPolygroupLayersProperties* CopyFromLayerProperties;                        // 0x0128 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* PreviewCompute;                        // 0x0130 (size: 0x8)
    class UPreviewGeometry* PreviewGeometry;                                          // 0x0138 (size: 0x8)
    class UPreviewMesh* UnmodifiedAreaPreviewMesh;                                    // 0x0140 (size: 0x8)

}; // Size: 0x1B0

class UConvertToPolygonsToolBuilder : public USingleTargetWithSelectionToolBuilder
{
}; // Size: 0x28

class UConvertToPolygonsToolProperties : public UInteractiveToolPropertySet
{
    EConvertToPolygonsMode ConversionMode;                                            // 0x00A8 (size: 0x4)
    float AngleTolerance;                                                             // 0x00AC (size: 0x4)
    bool bUseAverageGroupNormal;                                                      // 0x00B0 (size: 0x1)
    int32 NumPoints;                                                                  // 0x00B4 (size: 0x4)
    bool bSplitExisting;                                                              // 0x00B8 (size: 0x1)
    bool bNormalWeighted;                                                             // 0x00B9 (size: 0x1)
    float NormalWeighting;                                                            // 0x00BC (size: 0x4)
    float QuadAdjacencyWeight;                                                        // 0x00C0 (size: 0x4)
    float QuadMetricClamp;                                                            // 0x00C4 (size: 0x4)
    int32 QuadSearchRounds;                                                           // 0x00C8 (size: 0x4)
    bool bRespectUVSeams;                                                             // 0x00CC (size: 0x1)
    bool bRespectHardNormals;                                                         // 0x00CD (size: 0x1)
    int32 MinGroupSize;                                                               // 0x00D0 (size: 0x4)
    bool bShowGroupColors;                                                            // 0x00D4 (size: 0x1)
    bool bCalculateNormals;                                                           // 0x00D5 (size: 0x1)
    FName GroupLayer;                                                                 // 0x00D8 (size: 0x8)
    TArray<FString> OptionsList;                                                      // 0x00E0 (size: 0x10)
    bool bShowNewLayerName;                                                           // 0x00F0 (size: 0x1)
    FString NewLayerName;                                                             // 0x00F8 (size: 0x10)

    TArray<FString> GetGroupOptionsList();
}; // Size: 0x108

class UCutMeshWithMeshTool : public UBaseCreateFromSelectedTool
{
    class UCutMeshWithMeshToolProperties* CutProperties;                              // 0x0108 (size: 0x8)
    class UPreviewMesh* IntersectPreviewMesh;                                         // 0x0110 (size: 0x8)
    class ULineSetComponent* DrawnLineSet;                                            // 0x0138 (size: 0x8)

}; // Size: 0x380

class UCutMeshWithMeshToolBuilder : public UBaseCreateFromSelectedToolBuilder
{
}; // Size: 0x28

class UCutMeshWithMeshToolProperties : public UInteractiveToolPropertySet
{
    bool bTryFixHoles;                                                                // 0x00A8 (size: 0x1)
    bool bTryCollapseEdges;                                                           // 0x00A9 (size: 0x1)
    float WindingThreshold;                                                           // 0x00AC (size: 0x4)
    bool bShowNewBoundaries;                                                          // 0x00B0 (size: 0x1)
    bool bUseFirstMeshMaterials;                                                      // 0x00B1 (size: 0x1)

}; // Size: 0xB8

class UDeformMeshPolygonsTool : public UMeshSurfacePointTool
{
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x0110 (size: 0x8)
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x0118 (size: 0x8)
    class UDeformMeshPolygonsTransformProperties* TransformProps;                     // 0x0120 (size: 0x8)

}; // Size: 0x1780

class UDeformMeshPolygonsToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x30

class UDeformMeshPolygonsTransformProperties : public UInteractiveToolPropertySet
{
    EGroupTopologyDeformationStrategy DeformationStrategy;                            // 0x00A8 (size: 0x1)
    EQuickTransformerMode TransformMode;                                              // 0x00A9 (size: 0x1)
    bool bSelectFaces;                                                                // 0x00AA (size: 0x1)
    bool bSelectEdges;                                                                // 0x00AB (size: 0x1)
    bool bSelectVertices;                                                             // 0x00AC (size: 0x1)
    bool bShowWireframe;                                                              // 0x00AD (size: 0x1)
    EWeightScheme SelectedWeightScheme;                                               // 0x00B0 (size: 0x4)
    double HandleWeight;                                                              // 0x00B8 (size: 0x8)
    bool bPostFixHandles;                                                             // 0x00C0 (size: 0x1)

}; // Size: 0xC8

class UDeleteGeometrySelectionCommand : public UGeometrySelectionEditCommand
{
}; // Size: 0x28

class UDiffusionSmoothProperties : public UInteractiveToolPropertySet
{
    float SmoothingPerStep;                                                           // 0x00A8 (size: 0x4)
    int32 Steps;                                                                      // 0x00AC (size: 0x4)
    bool bPreserveUVs;                                                                // 0x00B0 (size: 0x1)

}; // Size: 0xB8

class UDisconnectGeometrySelectionCommand : public UGeometrySelectionEditCommand
{
}; // Size: 0x28

class UDisplaceMeshCommonProperties : public UInteractiveToolPropertySet
{
    EDisplaceMeshToolDisplaceType DisplacementType;                                   // 0x00A8 (size: 0x1)
    float DisplaceIntensity;                                                          // 0x00AC (size: 0x4)
    int32 RandomSeed;                                                                 // 0x00B0 (size: 0x4)
    EDisplaceMeshToolSubdivisionType SubdivisionType;                                 // 0x00B4 (size: 0x1)
    int32 Subdivisions;                                                               // 0x00B8 (size: 0x4)
    FName WeightMap;                                                                  // 0x00BC (size: 0x8)
    TArray<FString> WeightMapsList;                                                   // 0x00C8 (size: 0x10)
    bool bInvertWeightMap;                                                            // 0x00D8 (size: 0x1)
    bool bShowWireframe;                                                              // 0x00D9 (size: 0x1)
    bool bDisableSizeWarning;                                                         // 0x00DA (size: 0x1)

    TArray<FString> GetWeightMapsFunc();
}; // Size: 0xE0

class UDisplaceMeshDirectionalFilterProperties : public UInteractiveToolPropertySet
{
    bool bEnableFilter;                                                               // 0x00A8 (size: 0x1)
    FVector FilterDirection;                                                          // 0x00B0 (size: 0x18)
    float FilterWidth;                                                                // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UDisplaceMeshPerlinNoiseProperties : public UInteractiveToolPropertySet
{
    TArray<FPerlinLayerProperties> PerlinLayerProperties;                             // 0x00A8 (size: 0x10)

}; // Size: 0xB8

class UDisplaceMeshSineWaveProperties : public UInteractiveToolPropertySet
{
    float SineWaveFrequency;                                                          // 0x00A8 (size: 0x4)
    float SineWavePhaseShift;                                                         // 0x00AC (size: 0x4)
    FVector SineWaveDirection;                                                        // 0x00B0 (size: 0x18)

}; // Size: 0xC8

class UDisplaceMeshTextureMapProperties : public UInteractiveToolPropertySet
{
    class UTexture2D* DisplacementMap;                                                // 0x00A8 (size: 0x8)
    EDisplaceMeshToolChannelType Channel;                                             // 0x00B0 (size: 0x1)
    float DisplacementMapBaseValue;                                                   // 0x00B4 (size: 0x4)
    FVector2D UVScale;                                                                // 0x00B8 (size: 0x10)
    FVector2D UVOffset;                                                               // 0x00C8 (size: 0x10)
    bool bApplyAdjustmentCurve;                                                       // 0x00D8 (size: 0x1)
    class UCurveFloat* AdjustmentCurve;                                               // 0x00E0 (size: 0x8)
    bool bRecalcNormals;                                                              // 0x00E8 (size: 0x1)

}; // Size: 0xF0

class UDisplaceMeshTool : public USingleTargetWithSelectionTool
{
    class UDisplaceMeshCommonProperties* CommonProperties;                            // 0x0120 (size: 0x8)
    class UDisplaceMeshDirectionalFilterProperties* DirectionalFilterProperties;      // 0x0128 (size: 0x8)
    class UDisplaceMeshTextureMapProperties* TextureMapProperties;                    // 0x0130 (size: 0x8)
    class UDisplaceMeshPerlinNoiseProperties* NoiseProperties;                        // 0x0138 (size: 0x8)
    class UDisplaceMeshSineWaveProperties* SineWaveProperties;                        // 0x0140 (size: 0x8)
    class USelectiveTessellationProperties* SelectiveTessellationProperties;          // 0x0148 (size: 0x8)
    class UCurveFloat* ActiveContrastCurveTarget;                                     // 0x0150 (size: 0x8)
    class UMeshStatisticsProperties* MeshStatistics;                                  // 0x0158 (size: 0x8)
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x0498 (size: 0x8)
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x04A0 (size: 0x8)

}; // Size: 0x4C0

class UDisplaceMeshToolBuilder : public USingleTargetWithSelectionToolBuilder
{
}; // Size: 0x28

class UDrawAndRevolveTool : public UInteractiveTool
{
    class UCurveControlPointsMechanic* ControlPointsMechanic;                         // 0x0160 (size: 0x8)
    class UConstructionPlaneMechanic* PlaneMechanic;                                  // 0x0168 (size: 0x8)
    class UCreateMeshObjectTypeProperties* OutputTypeProperties;                      // 0x0170 (size: 0x8)
    class URevolveToolProperties* Settings;                                           // 0x0178 (size: 0x8)
    class UNewMeshMaterialProperties* MaterialProperties;                             // 0x0180 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x0188 (size: 0x8)

}; // Size: 0x190

class UDrawAndRevolveToolBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x28

class UDrawPolygonTool : public UInteractiveTool
{
    class UCreateMeshObjectTypeProperties* OutputTypeProperties;                      // 0x00A8 (size: 0x8)
    class UDrawPolygonToolStandardProperties* PolygonProperties;                      // 0x00B0 (size: 0x8)
    class UDrawPolygonToolSnapProperties* SnapProperties;                             // 0x00B8 (size: 0x8)
    class UNewMeshMaterialProperties* MaterialProperties;                             // 0x00C0 (size: 0x8)
    class UPreviewMesh* PreviewMesh;                                                  // 0x0160 (size: 0x8)
    class UPlaneDistanceFromHitMechanic* HeightMechanic;                              // 0x0568 (size: 0x8)
    class UDragAlignmentMechanic* DragAlignmentMechanic;                              // 0x0570 (size: 0x8)
    class UConstructionPlaneMechanic* PlaneMechanic;                                  // 0x0578 (size: 0x8)

}; // Size: 0x590

class UDrawPolygonToolBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x28

class UDrawPolygonToolSnapProperties : public UInteractiveToolPropertySet
{
    bool bEnableSnapping;                                                             // 0x00A8 (size: 0x1)
    bool bSnapToWorldGrid;                                                            // 0x00A9 (size: 0x1)
    bool bSnapToVertices;                                                             // 0x00AA (size: 0x1)
    bool bSnapToEdges;                                                                // 0x00AB (size: 0x1)
    bool bSnapToAxes;                                                                 // 0x00AC (size: 0x1)
    bool bSnapToLengths;                                                              // 0x00AD (size: 0x1)
    bool bSnapToSurfaces;                                                             // 0x00AE (size: 0x1)
    float SnapToSurfacesOffset;                                                       // 0x00B0 (size: 0x4)

}; // Size: 0xB8

class UDrawPolygonToolStandardProperties : public UInteractiveToolPropertySet
{
    EDrawPolygonDrawMode PolygonDrawMode;                                             // 0x00A8 (size: 0x1)
    bool bAllowSelfIntersections;                                                     // 0x00A9 (size: 0x1)
    float FeatureSizeRatio;                                                           // 0x00AC (size: 0x4)
    int32 RadialSlices;                                                               // 0x00B0 (size: 0x4)
    float Distance;                                                                   // 0x00B4 (size: 0x4)
    bool bShowGridGizmo;                                                              // 0x00B8 (size: 0x1)
    EDrawPolygonExtrudeMode ExtrudeMode;                                              // 0x00B9 (size: 0x1)
    float ExtrudeHeight;                                                              // 0x00BC (size: 0x4)

}; // Size: 0xC0

class UDynamicMeshBrushProperties : public UInteractiveToolPropertySet
{
    FBrushToolRadius BrushSize;                                                       // 0x00A8 (size: 0x18)
    float BrushFalloffAmount;                                                         // 0x00C0 (size: 0x4)
    float Depth;                                                                      // 0x00C4 (size: 0x4)
    bool bHitBackFaces;                                                               // 0x00C8 (size: 0x1)

}; // Size: 0xD0

class UDynamicMeshBrushSculptProperties : public UInteractiveToolPropertySet
{
    bool bIsRemeshingEnabled;                                                         // 0x00A8 (size: 0x1)
    EDynamicMeshSculptBrushType PrimaryBrushType;                                     // 0x00A9 (size: 0x1)
    float PrimaryBrushSpeed;                                                          // 0x00AC (size: 0x4)
    bool bPreserveUVFlow;                                                             // 0x00B0 (size: 0x1)
    bool bFreezeTarget;                                                               // 0x00B1 (size: 0x1)
    float SmoothBrushSpeed;                                                           // 0x00B4 (size: 0x4)
    bool bDetailPreservingSmooth;                                                     // 0x00B8 (size: 0x1)

}; // Size: 0xC0

class UDynamicMeshBrushTool : public UBaseBrushTool
{
    class UPreviewMesh* PreviewMesh;                                                  // 0x02A8 (size: 0x8)

}; // Size: 0x2F0

class UDynamicMeshSculptTool : public UMeshSurfacePointTool
{
    class UDynamicMeshBrushProperties* BrushProperties;                               // 0x0100 (size: 0x8)
    class UDynamicMeshBrushSculptProperties* SculptProperties;                        // 0x0108 (size: 0x8)
    class USculptMaxBrushProperties* SculptMaxBrushProperties;                        // 0x0110 (size: 0x8)
    class UKelvinBrushProperties* KelvinBrushProperties;                              // 0x0118 (size: 0x8)
    class UBrushRemeshProperties* RemeshProperties;                                   // 0x0120 (size: 0x8)
    class UFixedPlaneBrushProperties* GizmoProperties;                                // 0x0128 (size: 0x8)
    class UMeshEditingViewProperties* ViewProperties;                                 // 0x0130 (size: 0x8)
    class UDynamicSculptToolActions* SculptToolActions;                               // 0x0138 (size: 0x8)
    class UBrushStampIndicator* BrushIndicator;                                       // 0x01A8 (size: 0x8)
    class UMaterialInstanceDynamic* BrushIndicatorMaterial;                           // 0x01B0 (size: 0x8)
    class UPreviewMesh* BrushIndicatorMesh;                                           // 0x01B8 (size: 0x8)
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x01C0 (size: 0x8)
    class UOctreeDynamicMeshComponent* DynamicMeshComponent;                          // 0x01C8 (size: 0x8)
    class UMaterialInstanceDynamic* ActiveOverrideMaterial;                           // 0x01D0 (size: 0x8)
    class UCombinedTransformGizmo* PlaneTransformGizmo;                               // 0x0E78 (size: 0x8)
    class UTransformProxy* PlaneTransformProxy;                                       // 0x0E80 (size: 0x8)

}; // Size: 0xE90

class UDynamicMeshSculptToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x38

class UDynamicSculptToolActions : public UInteractiveToolPropertySet
{

    void DiscardAttributes();
}; // Size: 0xB0

class UEdgeLoopInsertionProperties : public UInteractiveToolPropertySet
{
    EEdgeLoopPositioningMode PositionMode;                                            // 0x00A8 (size: 0x4)
    EEdgeLoopInsertionMode InsertionMode;                                             // 0x00AC (size: 0x4)
    int32 NumLoops;                                                                   // 0x00B0 (size: 0x4)
    double ProportionOffset;                                                          // 0x00B8 (size: 0x8)
    double DistanceOffset;                                                            // 0x00C0 (size: 0x8)
    bool bInteractive;                                                                // 0x00C8 (size: 0x1)
    bool bFlipOffsetDirection;                                                        // 0x00C9 (size: 0x1)
    bool bHighlightProblemGroups;                                                     // 0x00CA (size: 0x1)
    double VertexTolerance;                                                           // 0x00D0 (size: 0x8)

}; // Size: 0xD8

class UEditMeshPolygonsActionModeToolBuilder : public UEditMeshPolygonsToolBuilder
{
}; // Size: 0x38

class UEditMeshPolygonsSelectionModeToolBuilder : public UEditMeshPolygonsToolBuilder
{
}; // Size: 0x38

class UEditMeshPolygonsTool : public USingleTargetWithSelectionTool
{
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x0140 (size: 0x8)
    class UPolyEditCommonProperties* CommonProps;                                     // 0x0148 (size: 0x8)
    class UEditMeshPolygonsToolActions* EditActions;                                  // 0x0150 (size: 0x8)
    class UEditMeshPolygonsToolActions_Triangles* EditActions_Triangles;              // 0x0158 (size: 0x8)
    class UEditMeshPolygonsToolEdgeActions* EditEdgeActions;                          // 0x0160 (size: 0x8)
    class UEditMeshPolygonsToolEdgeActions_Triangles* EditEdgeActions_Triangles;      // 0x0168 (size: 0x8)
    class UEditMeshPolygonsToolUVActions* EditUVActions;                              // 0x0170 (size: 0x8)
    class UPolyEditTopologyProperties* TopologyProperties;                            // 0x0178 (size: 0x8)
    class UPolyEditExtrudeActivity* ExtrudeActivity;                                  // 0x0180 (size: 0x8)
    class UPolyEditInsetOutsetActivity* InsetOutsetActivity;                          // 0x0188 (size: 0x8)
    class UPolyEditCutFacesActivity* CutFacesActivity;                                // 0x0190 (size: 0x8)
    class UPolyEditPlanarProjectionUVActivity* PlanarProjectionUVActivity;            // 0x0198 (size: 0x8)
    class UPolyEditInsertEdgeActivity* InsertEdgeActivity;                            // 0x01A0 (size: 0x8)
    class UPolyEditInsertEdgeLoopActivity* InsertEdgeLoopActivity;                    // 0x01A8 (size: 0x8)
    class UPolyEditBevelEdgeActivity* BevelEdgeActivity;                              // 0x01B0 (size: 0x8)
    class UPolyEditExtrudeEdgeActivity* ExtrudeEdgeActivity;                          // 0x01B8 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0298 (size: 0x8)
    class UPolygonSelectionMechanic* SelectionMechanic;                               // 0x02A0 (size: 0x8)
    class UDragAlignmentMechanic* DragAlignmentMechanic;                              // 0x02A8 (size: 0x8)
    class UCombinedTransformGizmo* TransformGizmo;                                    // 0x02B0 (size: 0x8)
    class UTransformProxy* TransformProxy;                                            // 0x02B8 (size: 0x8)

}; // Size: 0x8F0

class UEditMeshPolygonsToolActionPropertySet : public UInteractiveToolPropertySet
{
}; // Size: 0xB0

class UEditMeshPolygonsToolActions : public UEditMeshPolygonsToolActionPropertySet
{

    void SimplifyByGroups();
    void Retriangulate();
    void RecalcNormals();
    void PushPull();
    void Outset();
    void Offset();
    void Merge();
    void Inset();
    void InsertEdgeLoop();
    void InsertEdge();
    void Flip();
    void Extrude();
    void Duplicate();
    void Disconnect();
    void Delete();
    void Decompose();
    void CutFaces();
    void Bevel();
}; // Size: 0xB0

class UEditMeshPolygonsToolActions_Triangles : public UEditMeshPolygonsToolActionPropertySet
{

    void RecalcNormals();
    void PushPull();
    void Poke();
    void Outset();
    void Offset();
    void Inset();
    void Flip();
    void Extrude();
    void Duplicate();
    void Disconnect();
    void Delete();
    void CutFaces();
}; // Size: 0xB0

class UEditMeshPolygonsToolBuilder : public USingleTargetWithSelectionToolBuilder
{
}; // Size: 0x30

class UEditMeshPolygonsToolEdgeActions : public UEditMeshPolygonsToolActionPropertySet
{

    void WeldCentered();
    void Weld();
    void Straighten();
    void Simplify();
    void FillHole();
    void Extrude();
    void DeleteEdge();
    void Collapse();
    void Bridge();
    void Bevel();
}; // Size: 0xB0

class UEditMeshPolygonsToolEdgeActions_Triangles : public UEditMeshPolygonsToolActionPropertySet
{

    void WeldCentered();
    void Weld();
    void Split();
    void Flip();
    void FillHole();
    void Extrude();
    void Collapse();
    void Bridge();
}; // Size: 0xB0

class UEditMeshPolygonsToolUVActions : public UEditMeshPolygonsToolActionPropertySet
{

    void PlanarProjection();
}; // Size: 0xB0

class UEraseBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UEraseSculptLayerBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UExistingMeshMaterialProperties : public UInteractiveToolPropertySet
{
    ESetMeshMaterialMode MaterialMode;                                                // 0x00A8 (size: 0x1)
    float CheckerDensity;                                                             // 0x00AC (size: 0x4)
    class UMaterialInterface* OverrideMaterial;                                       // 0x00B0 (size: 0x8)
    FString UVChannel;                                                                // 0x00B8 (size: 0x10)
    TArray<FString> UVChannelNamesList;                                               // 0x00C8 (size: 0x10)
    class UMaterialInstanceDynamic* CheckerMaterial;                                  // 0x00D8 (size: 0x8)

    TArray<FString> GetUVChannelNamesFunc();
}; // Size: 0xE0

class UFixedPlaneBrushOpProps : public UBasePlaneBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    EPlaneBrushSideMode WhichSide;                                                    // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UFixedPlaneBrushProperties : public UInteractiveToolPropertySet
{
    bool bPropertySetEnabled;                                                         // 0x00A8 (size: 0x1)
    bool bShowGizmo;                                                                  // 0x00A9 (size: 0x1)
    FVector Position;                                                                 // 0x00B0 (size: 0x18)
    FQuat Rotation;                                                                   // 0x00D0 (size: 0x20)

}; // Size: 0xF0

class UFlattenBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    EPlaneBrushSideMode WhichSide;                                                    // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UGroupEdgeInsertionProperties : public UInteractiveToolPropertySet
{
    EGroupEdgeInsertionMode InsertionMode;                                            // 0x00A8 (size: 0x4)
    bool bContinuousInsertion;                                                        // 0x00AC (size: 0x1)
    double VertexTolerance;                                                           // 0x00B0 (size: 0x8)

}; // Size: 0xB8

class UGroupEraseBrushOpProps : public UMeshSculptBrushOpProps
{
    int32 Group;                                                                      // 0x00B0 (size: 0x4)
    bool bOnlyEraseCurrent;                                                           // 0x00B4 (size: 0x1)

}; // Size: 0xF0

class UGroupPaintBrushFilterProperties : public UInteractiveToolPropertySet
{
    EMeshGroupPaintBrushType PrimaryBrushType;                                        // 0x00A8 (size: 0x1)
    EMeshGroupPaintInteractionType SubToolType;                                       // 0x00A9 (size: 0x1)
    float BrushSize;                                                                  // 0x00AC (size: 0x4)
    EMeshGroupPaintBrushAreaType BrushAreaMode;                                       // 0x00B0 (size: 0x1)
    bool bHitBackFaces;                                                               // 0x00B1 (size: 0x1)
    int32 SetGroup;                                                                   // 0x00B4 (size: 0x4)
    bool bOnlySetUngrouped;                                                           // 0x00B8 (size: 0x1)
    int32 EraseGroup;                                                                 // 0x00BC (size: 0x4)
    bool bOnlyEraseCurrent;                                                           // 0x00C0 (size: 0x1)
    float AngleThreshold;                                                             // 0x00C4 (size: 0x4)
    bool bUVSeams;                                                                    // 0x00C8 (size: 0x1)
    bool bNormalSeams;                                                                // 0x00C9 (size: 0x1)
    EMeshGroupPaintVisibilityType VisibilityFilter;                                   // 0x00CA (size: 0x1)
    int32 MinTriVertCount;                                                            // 0x00CC (size: 0x4)
    bool bShowHitGroup;                                                               // 0x00D0 (size: 0x1)
    bool bShowAllGroups;                                                              // 0x00D1 (size: 0x1)

}; // Size: 0xD8

class UGroupPaintBrushOpProps : public UMeshSculptBrushOpProps
{
    int32 Group;                                                                      // 0x00B0 (size: 0x4)
    bool bOnlyPaintUngrouped;                                                         // 0x00B4 (size: 0x1)

}; // Size: 0xB8

class UHoleFillOperatorFactory : public UObject
{
    class UHoleFillTool* FillTool;                                                    // 0x0030 (size: 0x8)

}; // Size: 0x38

class UHoleFillStatisticsProperties : public UInteractiveToolPropertySet
{
    FString InitialHoles;                                                             // 0x00A8 (size: 0x10)
    FString SelectedHoles;                                                            // 0x00B8 (size: 0x10)
    FString SuccessfulFills;                                                          // 0x00C8 (size: 0x10)
    FString FailedFills;                                                              // 0x00D8 (size: 0x10)
    FString RemainingHoles;                                                           // 0x00E8 (size: 0x10)

}; // Size: 0xF8

class UHoleFillTool : public USingleSelectionMeshEditingTool
{
    class USmoothHoleFillProperties* SmoothHoleFillProperties;                        // 0x00B0 (size: 0x8)
    class UHoleFillToolProperties* Properties;                                        // 0x00B8 (size: 0x8)
    class UHoleFillToolActions* Actions;                                              // 0x00C0 (size: 0x8)
    class UHoleFillStatisticsProperties* Statistics;                                  // 0x00C8 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x00D0 (size: 0x8)
    class UBoundarySelectionMechanic* SelectionMechanic;                              // 0x00D8 (size: 0x8)

}; // Size: 0x230

class UHoleFillToolActions : public UInteractiveToolPropertySet
{

    void SelectAll();
    void Clear();
}; // Size: 0xB0

class UHoleFillToolBuilder : public USingleSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class UHoleFillToolProperties : public UInteractiveToolPropertySet
{
    EHoleFillOpFillType FillType;                                                     // 0x00A8 (size: 0x1)
    bool bRemoveIsolatedTriangles;                                                    // 0x00A9 (size: 0x1)
    bool bQuickFillSmallHoles;                                                        // 0x00AA (size: 0x1)

}; // Size: 0xB0

class UImplicitOffsetProperties : public UInteractiveToolPropertySet
{
    float Smoothness;                                                                 // 0x00A8 (size: 0x4)
    bool bPreserveUVs;                                                                // 0x00AC (size: 0x1)

}; // Size: 0xB0

class UImplicitSmoothProperties : public UInteractiveToolPropertySet
{
    float SmoothSpeed;                                                                // 0x00A8 (size: 0x4)
    float Smoothness;                                                                 // 0x00AC (size: 0x4)
    bool bPreserveUVs;                                                                // 0x00B0 (size: 0x1)
    float VolumeCorrection;                                                           // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UInflateBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UIterativeOffsetProperties : public UInteractiveToolPropertySet
{
    int32 Steps;                                                                      // 0x00A8 (size: 0x4)
    bool bOffsetBoundaries;                                                           // 0x00AC (size: 0x1)
    float SmoothingPerStep;                                                           // 0x00B0 (size: 0x4)
    bool bReprojectSmooth;                                                            // 0x00B4 (size: 0x1)

}; // Size: 0xB8

class UIterativeSmoothProperties : public UInteractiveToolPropertySet
{
    float SmoothingPerStep;                                                           // 0x00A8 (size: 0x4)
    int32 Steps;                                                                      // 0x00AC (size: 0x4)
    bool bSmoothBoundary;                                                             // 0x00B0 (size: 0x1)

}; // Size: 0xB8

class UKelvinBrushProperties : public UInteractiveToolPropertySet
{
    float FalloffDistance;                                                            // 0x00A8 (size: 0x4)
    float Stiffness;                                                                  // 0x00AC (size: 0x4)
    float Incompressiblity;                                                           // 0x00B0 (size: 0x4)
    int32 BrushSteps;                                                                 // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class ULatticeDeformerOperatorFactory : public UObject
{
    class ULatticeDeformerTool* LatticeDeformerTool;                                  // 0x0030 (size: 0x8)

}; // Size: 0x38

class ULatticeDeformerTool : public UMultiTargetWithSelectionTool
{
    class ULatticeControlPointsMechanic* ControlPointsMechanic;                       // 0x0190 (size: 0x8)
    class ULatticeDeformerToolProperties* Settings;                                   // 0x0198 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x01A0 (size: 0x8)
    class UMeshSculptLayerProperties* SculptLayerProperties;                          // 0x01A8 (size: 0x8)
    bool bLatticeDeformed;                                                            // 0x01B0 (size: 0x1)

}; // Size: 0x220

class ULatticeDeformerToolBuilder : public UMultiTargetWithSelectionToolBuilder
{
}; // Size: 0x28

class ULatticeDeformerToolProperties : public UInteractiveToolPropertySet
{
    int32 XAxisResolution;                                                            // 0x00B0 (size: 0x4)
    int32 YAxisResolution;                                                            // 0x00B4 (size: 0x4)
    int32 ZAxisResolution;                                                            // 0x00B8 (size: 0x4)
    float Padding;                                                                    // 0x00BC (size: 0x4)
    ELatticeInterpolationType InterpolationType;                                      // 0x00C0 (size: 0x1)
    bool bDeformNormals;                                                              // 0x00C1 (size: 0x1)
    bool bCanChangeResolution;                                                        // 0x00C2 (size: 0x1)
    EToolContextCoordinateSystem GizmoCoordinateSystem;                               // 0x00C3 (size: 0x1)
    bool bSetPivotMode;                                                               // 0x00C4 (size: 0x1)
    bool bSoftDeformation;                                                            // 0x00C5 (size: 0x1)

    void Constrain();
    void ClearConstraints();
}; // Size: 0xC8

class UMeshAttributePaintBrushOperationProperties : public UInteractiveToolPropertySet
{
    bool bIsolateGeometrySelection;                                                   // 0x00A8 (size: 0x1)
    bool bToolHasSelection;                                                           // 0x00A9 (size: 0x1)
    EBrushActionMode BrushAction;                                                     // 0x00AC (size: 0x4)

}; // Size: 0xB0

class UMeshAttributePaintEditActions : public UInteractiveToolPropertySet
{
}; // Size: 0xB0

class UMeshAttributePaintTool : public UDynamicMeshBrushTool
{
    class UMeshAttributePaintBrushOperationProperties* BrushActionProps;              // 0x02F8 (size: 0x8)
    class UMeshAttributePaintToolProperties* AttribProps;                             // 0x0300 (size: 0x8)

}; // Size: 0x900

class UMeshAttributePaintToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x60

class UMeshAttributePaintToolProperties : public UInteractiveToolPropertySet
{
    FString Attribute;                                                                // 0x00A8 (size: 0x10)

    TArray<FString> GetAttributeNames();
}; // Size: 0xC8

class UMeshConstraintProperties : public UInteractiveToolPropertySet
{
    bool bPreserveSharpEdges;                                                         // 0x00A8 (size: 0x1)
    EMeshBoundaryConstraint MeshBoundaryConstraint;                                   // 0x00A9 (size: 0x1)
    EGroupBoundaryConstraint GroupBoundaryConstraint;                                 // 0x00AA (size: 0x1)
    EMaterialBoundaryConstraint MaterialBoundaryConstraint;                           // 0x00AB (size: 0x1)
    bool bPreventNormalFlips;                                                         // 0x00AC (size: 0x1)
    bool bPreventTinyTriangles;                                                       // 0x00AD (size: 0x1)

    bool IsPreventTinyTrianglesEnabled();
    bool IsPreventNormalFlipsEnabled();
}; // Size: 0xB0

class UMeshEditingViewProperties : public UInteractiveToolPropertySet
{
    bool bShowWireframe;                                                              // 0x00A8 (size: 0x1)
    EMeshEditingMaterialModes MaterialMode;                                           // 0x00AC (size: 0x4)
    bool bFlatShading;                                                                // 0x00B0 (size: 0x1)
    FLinearColor Color;                                                               // 0x00B4 (size: 0x10)
    class UTexture2D* Image;                                                          // 0x00C8 (size: 0x8)
    double Opacity;                                                                   // 0x00D0 (size: 0x8)
    FLinearColor TransparentMaterialColor;                                            // 0x00D8 (size: 0x10)
    bool bTwoSided;                                                                   // 0x00E8 (size: 0x1)
    TWeakObjectPtr<class UMaterialInterface> CustomMaterial;                          // 0x00EC (size: 0x8)

}; // Size: 0xF8

class UMeshGroupPaintTool : public UMeshSculptToolBase
{
    class UPolygroupLayersProperties* PolygroupLayerProperties;                       // 0x0BC0 (size: 0x8)
    class UGroupPaintBrushFilterProperties* FilterProperties;                         // 0x0BC8 (size: 0x8)
    class UGroupPaintBrushOpProps* PaintBrushOpProperties;                            // 0x0BD0 (size: 0x8)
    class UGroupEraseBrushOpProps* EraseBrushOpProperties;                            // 0x0BD8 (size: 0x8)
    class UMeshGroupPaintToolFreezeActions* FreezeActions;                            // 0x0BE0 (size: 0x8)
    class UPolyLassoMarqueeMechanic* PolyLassoMechanic;                               // 0x0BF0 (size: 0x8)
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x0BF8 (size: 0x8)
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x0C00 (size: 0x8)
    class UMeshElementsVisualizer* MeshElementsDisplay;                               // 0x0C08 (size: 0x8)

}; // Size: 0x1000

class UMeshGroupPaintToolActionPropertySet : public UInteractiveToolPropertySet
{
}; // Size: 0xB0

class UMeshGroupPaintToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x30

class UMeshGroupPaintToolFreezeActions : public UMeshGroupPaintToolActionPropertySet
{

    void UnfreezeAll();
    void ShrinkCurrent();
    void GrowCurrent();
    void FreezeOthers();
    void FreezeCurrent();
    void FloodFillCurrent();
    void ClearCurrent();
    void ClearAll();
}; // Size: 0xB0

class UMeshSculptBrushOpProps : public UInteractiveToolPropertySet
{
    bool bIsStrengthPressureEnabled;                                                  // 0x00A8 (size: 0x1)

}; // Size: 0xB0

class UMeshSculptLayerProperties : public UObject
{
    int32 ActiveLayer;                                                                // 0x0028 (size: 0x4)
    TArray<double> LayerWeights;                                                      // 0x0030 (size: 0x10)
    bool bCanEditLayers;                                                              // 0x0040 (size: 0x1)

    void RemoveLayer();
    void AddLayer();
}; // Size: 0x68

class UMeshSculptToolBase : public UMeshSurfacePointTool
{
    class USculptBrushProperties* BrushProperties;                                    // 0x0100 (size: 0x8)
    class UWorkPlaneProperties* GizmoProperties;                                      // 0x0108 (size: 0x8)
    TMap<int32, UMeshSculptBrushOpProps*> BrushOpPropSets;                            // 0x02B0 (size: 0x50)
    TMap<int32, UMeshSculptBrushOpProps*> SecondaryBrushOpPropSets;                   // 0x0350 (size: 0x50)
    class UMeshEditingViewProperties* ViewProperties;                                 // 0x0A60 (size: 0x8)
    class UMaterialInstanceDynamic* ActiveOverrideMaterial;                           // 0x0A68 (size: 0x8)
    class UBrushStampIndicator* BrushIndicator;                                       // 0x0A70 (size: 0x8)
    bool bIsVolumetricIndicator;                                                      // 0x0A78 (size: 0x1)
    class UMaterialInstanceDynamic* BrushIndicatorMaterial;                           // 0x0A80 (size: 0x8)
    class UPreviewMesh* BrushIndicatorMesh;                                           // 0x0A88 (size: 0x8)
    class UCombinedTransformGizmo* PlaneTransformGizmo;                               // 0x0A98 (size: 0x8)
    class UTransformProxy* PlaneTransformProxy;                                       // 0x0AA0 (size: 0x8)

}; // Size: 0xBC0

class UMeshSpaceDeformerTool : public USingleSelectionMeshEditingTool
{
    class UMeshSpaceDeformerToolProperties* Settings;                                 // 0x00B8 (size: 0x8)
    class UMeshSpaceDeformerToolActionPropertySet* ToolActions;                       // 0x00C0 (size: 0x8)
    class UGizmoTransformChangeStateTarget* StateTarget;                              // 0x00C8 (size: 0x8)
    class UDragAlignmentMechanic* DragAlignmentMechanic;                              // 0x00D0 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x00D8 (size: 0x8)
    class UPreviewMesh* OriginalMeshPreview;                                          // 0x00F0 (size: 0x8)
    class UIntervalGizmo* IntervalGizmo;                                              // 0x00F8 (size: 0x8)
    class UCombinedTransformGizmo* TransformGizmo;                                    // 0x0100 (size: 0x8)
    class UTransformProxy* TransformProxy;                                            // 0x0108 (size: 0x8)
    class UGizmoLocalFloatParameterSource* UpIntervalSource;                          // 0x0110 (size: 0x8)
    class UGizmoLocalFloatParameterSource* DownIntervalSource;                        // 0x0118 (size: 0x8)
    class UGizmoLocalFloatParameterSource* ForwardIntervalSource;                     // 0x0120 (size: 0x8)

}; // Size: 0x2C0

class UMeshSpaceDeformerToolActionPropertySet : public UInteractiveToolPropertySet
{

    void ShiftToCenter();
}; // Size: 0xB0

class UMeshSpaceDeformerToolBuilder : public USingleSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class UMeshSpaceDeformerToolProperties : public UInteractiveToolPropertySet
{
    ENonlinearOperationType SelectedOperationType;                                    // 0x00A8 (size: 0x1)
    float UpperBoundsInterval;                                                        // 0x00AC (size: 0x4)
    float LowerBoundsInterval;                                                        // 0x00B0 (size: 0x4)
    float BendDegrees;                                                                // 0x00B4 (size: 0x4)
    float TwistDegrees;                                                               // 0x00B8 (size: 0x4)
    EFlareProfileType FlareProfileType;                                               // 0x00BC (size: 0x1)
    float FlarePercentY;                                                              // 0x00C0 (size: 0x4)
    bool bLockXAndYFlaring;                                                           // 0x00C4 (size: 0x1)
    float FlarePercentX;                                                              // 0x00C8 (size: 0x4)
    bool bLockBottom;                                                                 // 0x00CC (size: 0x1)
    bool bShowOriginalMesh;                                                           // 0x00CD (size: 0x1)
    bool bDrawVisualization;                                                          // 0x00CE (size: 0x1)
    bool bAlignToNormalOnCtrlClick;                                                   // 0x00CF (size: 0x1)

}; // Size: 0xD0

class UMeshStatisticsProperties : public UInteractiveToolPropertySet
{
    FString Mesh;                                                                     // 0x00A8 (size: 0x10)
    FString UV;                                                                       // 0x00B8 (size: 0x10)
    FString Attributes;                                                               // 0x00C8 (size: 0x10)

}; // Size: 0xD8

class UMeshSymmetryProperties : public UInteractiveToolPropertySet
{
    bool bEnableSymmetry;                                                             // 0x00A8 (size: 0x1)
    bool bSymmetryCanBeEnabled;                                                       // 0x00A9 (size: 0x1)

}; // Size: 0xB0

class UMeshUVChannelProperties : public UInteractiveToolPropertySet
{
    FString UVChannel;                                                                // 0x00A8 (size: 0x10)
    TArray<FString> UVChannelNamesList;                                               // 0x00B8 (size: 0x10)

    TArray<FString> GetUVChannelNamesFunc();
}; // Size: 0xC8

class UMeshVertexPaintTool : public UMeshSculptToolBase
{
    class UPolygroupLayersProperties* PolygroupLayerProperties;                       // 0x0BC8 (size: 0x8)
    class UVertexPaintBasicProperties* BasicProperties;                               // 0x0BD0 (size: 0x8)
    class UVertexPaintBrushFilterProperties* FilterProperties;                        // 0x0BD8 (size: 0x8)
    class UVertexColorPaintBrushOpProps* PaintBrushOpProperties;                      // 0x0BE0 (size: 0x8)
    class UVertexColorPaintBrushOpProps* EraseBrushOpProperties;                      // 0x0BE8 (size: 0x8)
    class UMeshVertexPaintToolQuickActions* QuickActions;                             // 0x0BF0 (size: 0x8)
    class UMeshVertexPaintToolUtilityActions* UtilityActions;                         // 0x0BF8 (size: 0x8)
    class UPolyLassoMarqueeMechanic* PolyLassoMechanic;                               // 0x0C08 (size: 0x8)
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x0C10 (size: 0x8)
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x0C18 (size: 0x8)
    class UMeshElementsVisualizer* MeshElementsDisplay;                               // 0x0C20 (size: 0x8)

}; // Size: 0x1200

class UMeshVertexPaintToolActionPropertySet : public UInteractiveToolPropertySet
{
}; // Size: 0xB0

class UMeshVertexPaintToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x30

class UMeshVertexPaintToolQuickActions : public UMeshVertexPaintToolActionPropertySet
{

    void PaintAll();
    void FillWhite();
    void FillBlack();
    void EraseAll();
}; // Size: 0xB0

class UMeshVertexPaintToolUtilityActions : public UMeshVertexPaintToolActionPropertySet
{
    EMeshVertexPaintToolUtilityOperations Operation;                                  // 0x00B0 (size: 0x4)
    EMeshVertexPaintColorChannel SourceChannel;                                       // 0x00B4 (size: 0x1)
    float SourceValue;                                                                // 0x00B8 (size: 0x4)
    FName WeightMap;                                                                  // 0x00BC (size: 0x8)
    TArray<FString> WeightMapsList;                                                   // 0x00C8 (size: 0x10)
    FModelingToolsColorChannelFilter TargetChannels;                                  // 0x00D8 (size: 0x4)
    EMeshVertexPaintColorChannel TargetChannel;                                       // 0x00DC (size: 0x1)
    bool bCopyToHiRes;                                                                // 0x00DD (size: 0x1)
    FString CopyToLODName;                                                            // 0x00E0 (size: 0x10)
    TArray<FString> LODNamesList;                                                     // 0x00F0 (size: 0x10)

    TArray<FString> GetWeightMapsFunc();
    TArray<FString> GetLODNamesFunc();
    void ApplySelectedOperation();
}; // Size: 0x100

class UMeshVertexSculptTool : public UMeshSculptToolBase
{
    class UVertexBrushSculptProperties* SculptProperties;                             // 0x0BD0 (size: 0x8)
    class UVertexBrushAlphaProperties* AlphaProperties;                               // 0x0BD8 (size: 0x8)
    class UTexture2D* BrushAlpha;                                                     // 0x0BE0 (size: 0x8)
    class UMeshSymmetryProperties* SymmetryProperties;                                // 0x0BE8 (size: 0x8)
    class UMeshSculptLayerProperties* SculptLayerProperties;                          // 0x0BF0 (size: 0x8)
    class AInternalToolFrameworkActor* PreviewMeshActor;                              // 0x0C10 (size: 0x8)
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x0C18 (size: 0x8)

}; // Size: 0x16E0

class UMeshVertexSculptToolBuilder : public UMeshSurfacePointMeshEditingToolBuilder
{
}; // Size: 0x38

class UModifyGeometrySelectionCommand : public UGeometrySelectionEditCommand
{
}; // Size: 0x28

class UModifyGeometrySelectionCommand_Contract : public UModifyGeometrySelectionCommand
{
}; // Size: 0x28

class UModifyGeometrySelectionCommand_Expand : public UModifyGeometrySelectionCommand
{
}; // Size: 0x28

class UModifyGeometrySelectionCommand_ExpandToConnected : public UModifyGeometrySelectionCommand
{
}; // Size: 0x28

class UModifyGeometrySelectionCommand_Invert : public UModifyGeometrySelectionCommand
{
}; // Size: 0x28

class UModifyGeometrySelectionCommand_InvertConnected : public UModifyGeometrySelectionCommand
{
}; // Size: 0x28

class UMoveBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    FModelingToolsAxisFilter AxisFilters;                                             // 0x00BC (size: 0x3)

}; // Size: 0xC0

class UNewMeshMaterialProperties : public UInteractiveToolPropertySet
{
    TWeakObjectPtr<class UMaterialInterface> Material;                                // 0x00A8 (size: 0x8)
    float UVScale;                                                                    // 0x00B0 (size: 0x4)
    bool bWorldSpaceUVScale;                                                          // 0x00B4 (size: 0x1)
    bool bShowWireframe;                                                              // 0x00B5 (size: 0x1)
    bool bShowExtendedOptions;                                                        // 0x00B6 (size: 0x1)

}; // Size: 0xB8

class UOffsetMeshTool : public UBaseMeshProcessingTool
{
    class UOffsetMeshToolProperties* OffsetProperties;                                // 0x0460 (size: 0x8)
    class UIterativeOffsetProperties* IterativeProperties;                            // 0x0468 (size: 0x8)
    class UImplicitOffsetProperties* ImplicitProperties;                              // 0x0470 (size: 0x8)
    class UOffsetWeightMapSetProperties* WeightMapProperties;                         // 0x0478 (size: 0x8)

}; // Size: 0x480

class UOffsetMeshToolBuilder : public UBaseMeshProcessingToolBuilder
{
}; // Size: 0x28

class UOffsetMeshToolProperties : public UInteractiveToolPropertySet
{
    EOffsetMeshToolOffsetType OffsetType;                                             // 0x00A8 (size: 0x1)
    float Distance;                                                                   // 0x00AC (size: 0x4)
    bool bCreateShell;                                                                // 0x00B0 (size: 0x1)

}; // Size: 0xB8

class UOffsetWeightMapSetProperties : public UWeightMapSetProperties
{
    float MinDistance;                                                                // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UPinchBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    bool bPerpDamping;                                                                // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UPlaneBrushOpProps : public UBasePlaneBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    EPlaneBrushSideMode WhichSide;                                                    // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UPolyEditActivityContext : public UObject
{
    class UPolyEditCommonProperties* CommonProperties;                                // 0x0028 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x0040 (size: 0x8)
    class UPolygonSelectionMechanic* SelectionMechanic;                               // 0x0070 (size: 0x8)

}; // Size: 0x100

class UPolyEditBevelEdgeActivity : public UInteractiveToolActivity
{
    class UPolyEditBevelEdgeProperties* BevelProperties;                              // 0x0038 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0040 (size: 0x8)

}; // Size: 0x140

class UPolyEditBevelEdgeProperties : public UInteractiveToolPropertySet
{
    double BevelDistance;                                                             // 0x00A8 (size: 0x8)
    int32 Subdivisions;                                                               // 0x00B0 (size: 0x4)
    float RoundWeight;                                                                // 0x00B4 (size: 0x4)
    bool bInferMaterialID;                                                            // 0x00B8 (size: 0x1)
    int32 SetMaterialID;                                                              // 0x00BC (size: 0x4)

}; // Size: 0xC0

class UPolyEditCommonProperties : public UInteractiveToolPropertySet
{
    bool bShowWireframe;                                                              // 0x00A8 (size: 0x1)
    bool bShowSelectableCorners;                                                      // 0x00A9 (size: 0x1)
    bool bGizmoVisible;                                                               // 0x00AA (size: 0x1)
    ELocalFrameMode LocalFrameMode;                                                   // 0x00AC (size: 0x4)
    bool bLockRotation;                                                               // 0x00B0 (size: 0x1)
    bool bLocalCoordSystem;                                                           // 0x00B1 (size: 0x1)

}; // Size: 0xB8

class UPolyEditCutFacesActivity : public UInteractiveToolActivity
{
    class UPolyEditCutProperties* CutProperties;                                      // 0x0040 (size: 0x8)
    class UPolyEditPreviewMesh* EditPreview;                                          // 0x0048 (size: 0x8)
    class UCollectSurfacePathMechanic* SurfacePathMechanic;                           // 0x0050 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0058 (size: 0x8)

}; // Size: 0xC0

class UPolyEditCutProperties : public UInteractiveToolPropertySet
{
    EPolyEditCutPlaneOrientation Orientation;                                         // 0x00A8 (size: 0x4)
    bool bSnapToVertices;                                                             // 0x00AC (size: 0x1)

}; // Size: 0xB0

class UPolyEditExtrudeActivity : public UInteractiveToolActivity
{
    class UPolyEditExtrudeProperties* ExtrudeProperties;                              // 0x0050 (size: 0x8)
    class UPolyEditOffsetProperties* OffsetProperties;                                // 0x0058 (size: 0x8)
    class UPolyEditPushPullProperties* PushPullProperties;                            // 0x0060 (size: 0x8)
    class UPlaneDistanceFromHitMechanic* ExtrudeHeightMechanic;                       // 0x0068 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0070 (size: 0x8)

}; // Size: 0x1D0

class UPolyEditExtrudeEdgeActivity : public UInteractiveToolActivity
{
    class UPolyEditExtrudeEdgeActivityProperties* Settings;                           // 0x0038 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0040 (size: 0x8)
    class UTransformProxy* ExtrudeFrameProxy;                                         // 0x0048 (size: 0x8)
    class UCombinedTransformGizmo* ExtrudeFrameGizmo;                                 // 0x0050 (size: 0x8)
    class UTransformProxy* SingleDirectionProxy;                                      // 0x0058 (size: 0x8)
    class UCombinedTransformGizmo* SingleDirectionGizmo;                              // 0x0060 (size: 0x8)
    class UPreviewGeometry* PreviewGeometry;                                          // 0x0068 (size: 0x8)

}; // Size: 0x2F0

class UPolyEditExtrudeEdgeActivityProperties : public UInteractiveToolPropertySet
{
    EPolyEditExtrudeEdgeDirectionMode DirectionMode;                                  // 0x00A8 (size: 0x4)
    EPolyEditExtrudeEdgeDistanceMode DistanceMode;                                    // 0x00AC (size: 0x4)
    double Distance;                                                                  // 0x00B0 (size: 0x8)
    bool bUseUnselectedForFrames;                                                     // 0x00B8 (size: 0x1)
    bool bAdjustToExtrudeEvenly;                                                      // 0x00B9 (size: 0x1)

}; // Size: 0xC0

class UPolyEditExtrudeProperties : public UInteractiveToolPropertySet
{
    EPolyEditExtrudeDistanceMode DistanceMode;                                        // 0x00A8 (size: 0x4)
    double Distance;                                                                  // 0x00B0 (size: 0x8)
    EPolyEditExtrudeModeOptions DirectionMode;                                        // 0x00B8 (size: 0x4)
    EPolyEditExtrudeDirection Direction;                                              // 0x00BC (size: 0x4)
    double MaxDistanceScaleFactor;                                                    // 0x00C0 (size: 0x8)
    bool bShellsToSolids;                                                             // 0x00C8 (size: 0x1)
    EPolyEditExtrudeDirection MeasureDirection;                                       // 0x00CC (size: 0x4)
    bool bUseColinearityForSettingBorderGroups;                                       // 0x00D0 (size: 0x1)

}; // Size: 0xD8

class UPolyEditInsertEdgeActivity : public UInteractiveToolActivity
{
    class UGroupEdgeInsertionProperties* Settings;                                    // 0x0048 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0050 (size: 0x8)

}; // Size: 0x470

class UPolyEditInsertEdgeLoopActivity : public UInteractiveToolActivity
{
    class UEdgeLoopInsertionProperties* Settings;                                     // 0x0048 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0050 (size: 0x8)

}; // Size: 0x400

class UPolyEditInsetOutsetActivity : public UInteractiveToolActivity
{
    class UPolyEditInsetOutsetProperties* Settings;                                   // 0x0040 (size: 0x8)
    class UPolyEditPreviewMesh* EditPreview;                                          // 0x0050 (size: 0x8)
    class USpatialCurveDistanceMechanic* CurveDistMechanic;                           // 0x0058 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0060 (size: 0x8)

}; // Size: 0x70

class UPolyEditInsetOutsetProperties : public UInteractiveToolPropertySet
{
    float Softness;                                                                   // 0x00A8 (size: 0x4)
    bool bBoundaryOnly;                                                               // 0x00AC (size: 0x1)
    float AreaScale;                                                                  // 0x00B0 (size: 0x4)
    bool bReproject;                                                                  // 0x00B4 (size: 0x1)
    bool bOutset;                                                                     // 0x00B5 (size: 0x1)

}; // Size: 0xB8

class UPolyEditOffsetProperties : public UInteractiveToolPropertySet
{
    EPolyEditExtrudeDistanceMode DistanceMode;                                        // 0x00A8 (size: 0x4)
    double Distance;                                                                  // 0x00B0 (size: 0x8)
    EPolyEditOffsetModeOptions DirectionMode;                                         // 0x00B8 (size: 0x4)
    double MaxDistanceScaleFactor;                                                    // 0x00C0 (size: 0x8)
    bool bShellsToSolids;                                                             // 0x00C8 (size: 0x1)
    EPolyEditExtrudeDirection MeasureDirection;                                       // 0x00CC (size: 0x4)
    bool bUseColinearityForSettingBorderGroups;                                       // 0x00D0 (size: 0x1)

}; // Size: 0xD8

class UPolyEditPlanarProjectionUVActivity : public UInteractiveToolActivity
{
    class UPolyEditSetUVProperties* SetUVProperties;                                  // 0x0040 (size: 0x8)
    class UPolyEditPreviewMesh* EditPreview;                                          // 0x0048 (size: 0x8)
    class UCollectSurfacePathMechanic* SurfacePathMechanic;                           // 0x0050 (size: 0x8)
    class UPolyEditActivityContext* ActivityContext;                                  // 0x0058 (size: 0x8)

}; // Size: 0xF0

class UPolyEditPushPullProperties : public UInteractiveToolPropertySet
{
    EPolyEditExtrudeDistanceMode DistanceMode;                                        // 0x00A8 (size: 0x4)
    double Distance;                                                                  // 0x00B0 (size: 0x8)
    EPolyEditPushPullModeOptions DirectionMode;                                       // 0x00B8 (size: 0x4)
    EPolyEditExtrudeDirection SingleDirection;                                        // 0x00BC (size: 0x4)
    double MaxDistanceScaleFactor;                                                    // 0x00C0 (size: 0x8)
    bool bShellsToSolids;                                                             // 0x00C8 (size: 0x1)
    EPolyEditExtrudeDirection MeasureDirection;                                       // 0x00CC (size: 0x4)
    bool bUseColinearityForSettingBorderGroups;                                       // 0x00D0 (size: 0x1)

}; // Size: 0xD8

class UPolyEditSetUVProperties : public UInteractiveToolPropertySet
{
    bool bShowMaterial;                                                               // 0x00A8 (size: 0x1)

}; // Size: 0xB0

class UPolyEditTopologyProperties : public UEditMeshPolygonsToolActionPropertySet
{
    bool bAddExtraCorners;                                                            // 0x00B0 (size: 0x1)
    double ExtraCornerAngleThresholdDegrees;                                          // 0x00B8 (size: 0x8)

    void RegenerateExtraCorners();
}; // Size: 0xC0

class UProceduralArrowToolProperties : public UProceduralShapeToolProperties
{
    float ShaftRadius;                                                                // 0x00B8 (size: 0x4)
    float ShaftHeight;                                                                // 0x00BC (size: 0x4)
    float HeadRadius;                                                                 // 0x00C0 (size: 0x4)
    float HeadHeight;                                                                 // 0x00C4 (size: 0x4)
    int32 RadialSlices;                                                               // 0x00C8 (size: 0x4)
    int32 HeightSubdivisions;                                                         // 0x00CC (size: 0x4)

}; // Size: 0xD0

class UProceduralBoxToolProperties : public UProceduralShapeToolProperties
{
    float Width;                                                                      // 0x00B8 (size: 0x4)
    float Depth;                                                                      // 0x00BC (size: 0x4)
    float Height;                                                                     // 0x00C0 (size: 0x4)
    int32 WidthSubdivisions;                                                          // 0x00C4 (size: 0x4)
    int32 DepthSubdivisions;                                                          // 0x00C8 (size: 0x4)
    int32 HeightSubdivisions;                                                         // 0x00CC (size: 0x4)

}; // Size: 0xD0

class UProceduralCapsuleToolProperties : public UProceduralShapeToolProperties
{
    float Radius;                                                                     // 0x00B8 (size: 0x4)
    float CylinderLength;                                                             // 0x00BC (size: 0x4)
    int32 HemisphereSlices;                                                           // 0x00C0 (size: 0x4)
    int32 CylinderSlices;                                                             // 0x00C4 (size: 0x4)
    int32 CylinderSubdivisions;                                                       // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UProceduralConeToolProperties : public UProceduralShapeToolProperties
{
    float Radius;                                                                     // 0x00B8 (size: 0x4)
    float Height;                                                                     // 0x00BC (size: 0x4)
    int32 RadialSlices;                                                               // 0x00C0 (size: 0x4)
    int32 HeightSubdivisions;                                                         // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class UProceduralCylinderToolProperties : public UProceduralShapeToolProperties
{
    float Radius;                                                                     // 0x00B8 (size: 0x4)
    float Height;                                                                     // 0x00BC (size: 0x4)
    int32 RadialSlices;                                                               // 0x00C0 (size: 0x4)
    int32 HeightSubdivisions;                                                         // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class UProceduralDiscToolProperties : public UProceduralShapeToolProperties
{
    EProceduralDiscType DiscType;                                                     // 0x00B8 (size: 0x4)
    float Radius;                                                                     // 0x00BC (size: 0x4)
    int32 RadialSlices;                                                               // 0x00C0 (size: 0x4)
    int32 RadialSubdivisions;                                                         // 0x00C4 (size: 0x4)
    float HoleRadius;                                                                 // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UProceduralRectangleToolProperties : public UProceduralShapeToolProperties
{
    EProceduralRectType RectangleType;                                                // 0x00B8 (size: 0x4)
    float Width;                                                                      // 0x00BC (size: 0x4)
    float Depth;                                                                      // 0x00C0 (size: 0x4)
    int32 WidthSubdivisions;                                                          // 0x00C4 (size: 0x4)
    int32 DepthSubdivisions;                                                          // 0x00C8 (size: 0x4)
    bool bMaintainDimension;                                                          // 0x00CC (size: 0x1)
    float CornerRadius;                                                               // 0x00D0 (size: 0x4)
    int32 CornerSlices;                                                               // 0x00D4 (size: 0x4)

}; // Size: 0xD8

class UProceduralShapeToolProperties : public UInteractiveToolPropertySet
{
    EMakeMeshPolygroupMode PolygroupMode;                                             // 0x00A8 (size: 0x1)
    EMakeMeshPlacementType TargetSurface;                                             // 0x00A9 (size: 0x1)
    EMakeMeshPivotLocation PivotLocation;                                             // 0x00AA (size: 0x1)
    float Rotation;                                                                   // 0x00AC (size: 0x4)
    bool bAlignToNormal;                                                              // 0x00B0 (size: 0x1)
    bool bShowGizmo;                                                                  // 0x00B1 (size: 0x1)
    bool bShowGizmoOptions;                                                           // 0x00B2 (size: 0x1)

}; // Size: 0xB8

class UProceduralSphereToolProperties : public UProceduralShapeToolProperties
{
    float Radius;                                                                     // 0x00B8 (size: 0x4)
    EProceduralSphereType SubdivisionType;                                            // 0x00BC (size: 0x4)
    int32 Subdivisions;                                                               // 0x00C0 (size: 0x4)
    int32 HorizontalSlices;                                                           // 0x00C4 (size: 0x4)
    int32 VerticalSlices;                                                             // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UProceduralStairsToolProperties : public UProceduralShapeToolProperties
{
    EProceduralStairsType StairsType;                                                 // 0x00B8 (size: 0x4)
    int32 NumSteps;                                                                   // 0x00BC (size: 0x4)
    float StepWidth;                                                                  // 0x00C0 (size: 0x4)
    float StepHeight;                                                                 // 0x00C4 (size: 0x4)
    float StepDepth;                                                                  // 0x00C8 (size: 0x4)
    float CurveAngle;                                                                 // 0x00CC (size: 0x4)
    float SpiralAngle;                                                                // 0x00D0 (size: 0x4)
    float InnerRadius;                                                                // 0x00D4 (size: 0x4)

}; // Size: 0xD8

class UProceduralTorusToolProperties : public UProceduralShapeToolProperties
{
    float MajorRadius;                                                                // 0x00B8 (size: 0x4)
    float MinorRadius;                                                                // 0x00BC (size: 0x4)
    int32 MajorSlices;                                                                // 0x00C0 (size: 0x4)
    int32 MinorSlices;                                                                // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class UProjectToTargetTool : public URemeshMeshTool
{
}; // Size: 0x118

class UProjectToTargetToolBuilder : public UMultiSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class UProjectToTargetToolProperties : public URemeshMeshToolProperties
{
    bool bWorldSpace;                                                                 // 0x00E0 (size: 0x1)
    bool bParallel;                                                                   // 0x00E1 (size: 0x1)
    int32 FaceProjectionPassesPerRemeshIteration;                                     // 0x00E4 (size: 0x4)
    float SurfaceProjectionSpeed;                                                     // 0x00E8 (size: 0x4)
    float NormalAlignmentSpeed;                                                       // 0x00EC (size: 0x4)
    bool bSmoothInFillAreas;                                                          // 0x00F0 (size: 0x1)
    float FillAreaDistanceMultiplier;                                                 // 0x00F4 (size: 0x4)
    float FillAreaSmoothMultiplier;                                                   // 0x00F8 (size: 0x4)

}; // Size: 0x100

class UPullKelvinletBrushOpProps : public UBaseKelvinletBrushOpProps
{
    float Falloff;                                                                    // 0x00C0 (size: 0x4)
    float Depth;                                                                      // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class URecomputeUVsTool : public USingleSelectionMeshEditingTool
{
    class UMeshUVChannelProperties* UVChannelProperties;                              // 0x00B8 (size: 0x8)
    class URecomputeUVsToolProperties* Settings;                                      // 0x00C0 (size: 0x8)
    class UPolygroupLayersProperties* PolygroupLayerProperties;                       // 0x00C8 (size: 0x8)
    class UExistingMeshMaterialProperties* MaterialSettings;                          // 0x00D0 (size: 0x8)
    bool bCreateUVLayoutViewOnSetup;                                                  // 0x00D8 (size: 0x1)
    class UUVLayoutPreview* UVLayoutView;                                             // 0x00E0 (size: 0x8)
    class URecomputeUVsOpFactory* RecomputeUVsOpFactory;                              // 0x00E8 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x00F0 (size: 0x8)

}; // Size: 0x118

class URecomputeUVsToolBuilder : public USingleSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class URemeshMeshTool : public UMultiSelectionMeshEditingTool
{
    class URemeshMeshToolProperties* BasicProperties;                                 // 0x00C0 (size: 0x8)
    class UMeshStatisticsProperties* MeshStatisticsProperties;                        // 0x00C8 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x00D0 (size: 0x8)
    class UMeshElementsVisualizer* MeshElementsDisplay;                               // 0x00D8 (size: 0x8)

}; // Size: 0x108

class URemeshMeshToolBuilder : public UMultiSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class URemeshMeshToolProperties : public URemeshProperties
{
    int32 TargetTriangleCount;                                                        // 0x00B8 (size: 0x4)
    ERemeshSmoothingType SmoothingType;                                               // 0x00BC (size: 0x1)
    bool bDiscardAttributes;                                                          // 0x00BD (size: 0x1)
    bool bShowGroupColors;                                                            // 0x00BE (size: 0x1)
    ERemeshType RemeshType;                                                           // 0x00BF (size: 0x1)
    int32 RemeshIterations;                                                           // 0x00C0 (size: 0x4)
    int32 MaxRemeshIterations;                                                        // 0x00C4 (size: 0x4)
    int32 ExtraProjectionIterations;                                                  // 0x00C8 (size: 0x4)
    bool bUseTargetEdgeLength;                                                        // 0x00CC (size: 0x1)
    float TargetEdgeLength;                                                           // 0x00D0 (size: 0x4)
    bool bReproject;                                                                  // 0x00D4 (size: 0x1)
    bool bReprojectConstraints;                                                       // 0x00D5 (size: 0x1)
    float BoundaryCornerAngleThreshold;                                               // 0x00D8 (size: 0x4)

}; // Size: 0xE0

class URemeshProperties : public UMeshConstraintProperties
{
    float SmoothingStrength;                                                          // 0x00B0 (size: 0x4)
    bool bFlips;                                                                      // 0x00B4 (size: 0x1)
    bool bSplits;                                                                     // 0x00B5 (size: 0x1)
    bool bCollapses;                                                                  // 0x00B6 (size: 0x1)

}; // Size: 0xB8

class URemoveOccludedTrianglesAdvancedProperties : public UInteractiveToolPropertySet
{
}; // Size: 0xB0

class URemoveOccludedTrianglesOperatorFactory : public UObject
{
    class URemoveOccludedTrianglesTool* Tool;                                         // 0x0030 (size: 0x8)

}; // Size: 0x40

class URemoveOccludedTrianglesTool : public UMultiSelectionMeshEditingTool
{
    class URemoveOccludedTrianglesToolProperties* BasicProperties;                    // 0x00B8 (size: 0x8)
    class UPolygroupLayersProperties* PolygroupLayersProperties;                      // 0x00C0 (size: 0x8)
    class URemoveOccludedTrianglesAdvancedProperties* AdvancedProperties;             // 0x00C8 (size: 0x8)
    TArray<UMeshOpPreviewWithBackgroundCompute*> Previews;                            // 0x00D0 (size: 0x10)
    TArray<UPreviewMesh*> PreviewCopies;                                              // 0x00E0 (size: 0x10)

}; // Size: 0x1D0

class URemoveOccludedTrianglesToolBuilder : public UMultiSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class URemoveOccludedTrianglesToolProperties : public UInteractiveToolPropertySet
{
    EOcclusionCalculationUIMode OcclusionTestMethod;                                  // 0x00A8 (size: 0x1)
    EOcclusionTriangleSamplingUIMode TriangleSampling;                                // 0x00A9 (size: 0x1)
    double WindingIsoValue;                                                           // 0x00B0 (size: 0x8)
    int32 AddRandomRays;                                                              // 0x00B8 (size: 0x4)
    int32 AddTriangleSamples;                                                         // 0x00BC (size: 0x4)
    bool bOnlySelfOcclude;                                                            // 0x00C0 (size: 0x1)
    int32 ShrinkRemoval;                                                              // 0x00C4 (size: 0x4)
    double MinAreaIsland;                                                             // 0x00C8 (size: 0x8)
    int32 MinTriCountIsland;                                                          // 0x00D0 (size: 0x4)
    EOccludedAction Action;                                                           // 0x00D4 (size: 0x1)

}; // Size: 0xD8

class URetriangulateGeometrySelectionCommand : public UGeometrySelectionEditCommand
{
}; // Size: 0x28

class URevolveOperatorFactory : public UObject
{
    class UDrawAndRevolveTool* RevolveTool;                                           // 0x0030 (size: 0x8)

}; // Size: 0x38

class URevolveProperties : public UInteractiveToolPropertySet
{
    double RevolveDegreesClamped;                                                     // 0x00A8 (size: 0x8)
    double RevolveDegrees;                                                            // 0x00B0 (size: 0x8)
    double RevolveDegreesOffset;                                                      // 0x00B8 (size: 0x8)
    double StepsMaxDegrees;                                                           // 0x00C0 (size: 0x8)
    bool bExplicitSteps;                                                              // 0x00C8 (size: 0x1)
    int32 NumExplicitSteps;                                                           // 0x00CC (size: 0x4)
    double HeightOffsetPerDegree;                                                     // 0x00D0 (size: 0x8)
    bool bReverseRevolutionDirection;                                                 // 0x00D8 (size: 0x1)
    bool bFlipMesh;                                                                   // 0x00D9 (size: 0x1)
    bool bSharpNormals;                                                               // 0x00DA (size: 0x1)
    double SharpNormalsDegreeThreshold;                                               // 0x00E0 (size: 0x8)
    bool bPathAtMidpointOfStep;                                                       // 0x00E8 (size: 0x1)
    ERevolvePropertiesPolygroupMode PolygroupMode;                                    // 0x00E9 (size: 0x1)
    ERevolvePropertiesQuadSplit QuadSplitMode;                                        // 0x00EA (size: 0x1)

}; // Size: 0x118

class URevolveToolProperties : public URevolveProperties
{
    ERevolvePropertiesCapFillMode CapFillMode;                                        // 0x0118 (size: 0x1)
    bool bClosePathToAxis;                                                            // 0x0119 (size: 0x1)
    FVector DrawPlaneOrigin;                                                          // 0x0120 (size: 0x18)
    FRotator DrawPlaneOrientation;                                                    // 0x0138 (size: 0x18)
    bool bEnableSnapping;                                                             // 0x0150 (size: 0x1)
    bool bAllowedToEditDrawPlane;                                                     // 0x0151 (size: 0x1)

}; // Size: 0x158

class UScaleKelvinletBrushOpProps : public UBaseKelvinletBrushOpProps
{
    float Strength;                                                                   // 0x00C0 (size: 0x4)
    float Falloff;                                                                    // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class USculptBrushProperties : public UInteractiveToolPropertySet
{
    FBrushToolRadius BrushSize;                                                       // 0x00A8 (size: 0x18)
    float BrushFalloffAmount;                                                         // 0x00C0 (size: 0x4)
    bool bShowFalloff;                                                                // 0x00C4 (size: 0x1)
    float Depth;                                                                      // 0x00C8 (size: 0x4)
    bool bHitBackFaces;                                                               // 0x00CC (size: 0x1)
    float FlowRate;                                                                   // 0x00D0 (size: 0x4)
    float Spacing;                                                                    // 0x00D4 (size: 0x4)
    float Lazyness;                                                                   // 0x00D8 (size: 0x4)
    bool bShowPerBrushProps;                                                          // 0x00DC (size: 0x1)
    bool bShowLazyness;                                                               // 0x00DD (size: 0x1)
    bool bShowFlowRate;                                                               // 0x00DE (size: 0x1)
    bool bShowSpacing;                                                                // 0x00DF (size: 0x1)

}; // Size: 0xE0

class USculptMaxBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float MaxHeight;                                                                  // 0x00B8 (size: 0x4)
    bool bUseFixedHeight;                                                             // 0x00BC (size: 0x1)
    float FixedHeight;                                                                // 0x00C0 (size: 0x4)

}; // Size: 0xC8

class USculptMaxBrushProperties : public UInteractiveToolPropertySet
{
    float MaxHeight;                                                                  // 0x00A8 (size: 0x4)
    bool bFreezeCurrentHeight;                                                        // 0x00AC (size: 0x1)

}; // Size: 0xB0

class USecondarySmoothBrushOpProps : public UBaseSmoothBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    bool bPreserveUVFlow;                                                             // 0x00B8 (size: 0x1)

}; // Size: 0xC0

class USelectiveTessellationProperties : public UInteractiveToolPropertySet
{
    EDisplaceMeshToolTriangleSelectionType SelectionType;                             // 0x00A8 (size: 0x1)
    FName ActiveMaterial;                                                             // 0x00AC (size: 0x8)
    TArray<FString> MaterialIDList;                                                   // 0x00B8 (size: 0x10)

    TArray<FString> GetMaterialIDsFunc();
}; // Size: 0xC8

class USharpPullKelvinletBrushOpProps : public UBaseKelvinletBrushOpProps
{
    float Falloff;                                                                    // 0x00C0 (size: 0x4)
    float Depth;                                                                      // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class USmoothBrushOpProps : public UBaseSmoothBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    bool bPreserveUVFlow;                                                             // 0x00B8 (size: 0x1)

}; // Size: 0xC0

class USmoothFillBrushOpProps : public UBaseSmoothBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    bool bPreserveUVFlow;                                                             // 0x00B8 (size: 0x1)

}; // Size: 0xC0

class USmoothHoleFillProperties : public UInteractiveToolPropertySet
{
    bool bConstrainToHoleInterior;                                                    // 0x00A8 (size: 0x1)
    int32 RemeshingExteriorRegionWidth;                                               // 0x00AC (size: 0x4)
    int32 SmoothingExteriorRegionWidth;                                               // 0x00B0 (size: 0x4)
    int32 SmoothingInteriorRegionWidth;                                               // 0x00B4 (size: 0x4)
    float InteriorSmoothness;                                                         // 0x00B8 (size: 0x4)
    double FillDensityScalar;                                                         // 0x00C0 (size: 0x8)
    bool bProjectDuringRemesh;                                                        // 0x00C8 (size: 0x1)

}; // Size: 0xD0

class USmoothMeshTool : public UBaseMeshProcessingTool
{
    class USmoothMeshToolProperties* SmoothProperties;                                // 0x0468 (size: 0x8)
    class UIterativeSmoothProperties* IterativeProperties;                            // 0x0470 (size: 0x8)
    class UDiffusionSmoothProperties* DiffusionProperties;                            // 0x0478 (size: 0x8)
    class UImplicitSmoothProperties* ImplicitProperties;                              // 0x0480 (size: 0x8)
    class USmoothWeightMapSetProperties* WeightMapProperties;                         // 0x0488 (size: 0x8)

}; // Size: 0x490

class USmoothMeshToolBuilder : public UBaseMeshProcessingToolBuilder
{
}; // Size: 0x28

class USmoothMeshToolProperties : public UInteractiveToolPropertySet
{
    ESmoothMeshToolSmoothType SmoothingType;                                          // 0x00A8 (size: 0x1)

}; // Size: 0xB0

class USmoothWeightMapSetProperties : public UWeightMapSetProperties
{
    float MinSmoothMultiplier;                                                        // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class USpaceDeformerOperatorFactory : public UObject
{
    class UMeshSpaceDeformerTool* SpaceDeformerTool;                                  // 0x0030 (size: 0x8)

}; // Size: 0x38

class UStandardSculptBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UTrimMeshesToolProperties : public UInteractiveToolPropertySet
{
    ETrimOperation WhichMesh;                                                         // 0x00A8 (size: 0x1)
    ETrimSide TrimSide;                                                               // 0x00A9 (size: 0x1)
    float WindingThreshold;                                                           // 0x00AC (size: 0x4)
    bool bShowTrimmingMesh;                                                           // 0x00B0 (size: 0x1)
    float OpacityOfTrimmingMesh;                                                      // 0x00B4 (size: 0x4)
    FLinearColor ColorOfTrimmingMesh;                                                 // 0x00B8 (size: 0x10)

}; // Size: 0xC8

class UTwistKelvinletBrushOpProps : public UBaseKelvinletBrushOpProps
{
    float Strength;                                                                   // 0x00C0 (size: 0x4)
    float Falloff;                                                                    // 0x00C4 (size: 0x4)

}; // Size: 0xC8

class UUVLayoutTool : public UMultiSelectionMeshEditingTool
{
    class UMeshUVChannelProperties* UVChannelProperties;                              // 0x00C0 (size: 0x8)
    class UUVLayoutProperties* BasicProperties;                                       // 0x00C8 (size: 0x8)
    class UExistingMeshMaterialProperties* MaterialSettings;                          // 0x00D0 (size: 0x8)
    TArray<UMeshOpPreviewWithBackgroundCompute*> Previews;                            // 0x00D8 (size: 0x10)
    TArray<UUVLayoutOperatorFactory*> Factories;                                      // 0x00E8 (size: 0x10)
    class UUVLayoutPreview* UVLayoutView;                                             // 0x0160 (size: 0x8)

}; // Size: 0x170

class UUVLayoutToolBuilder : public UMultiSelectionMeshEditingToolBuilder
{
}; // Size: 0x28

class UUVProjectionOperatorFactory : public UObject
{
    class UUVProjectionTool* Tool;                                                    // 0x0030 (size: 0x8)

}; // Size: 0x38

class UUVProjectionTool : public USingleTargetWithSelectionTool
{
    class UMeshUVChannelProperties* UVChannelProperties;                              // 0x0120 (size: 0x8)
    class UUVProjectionToolProperties* BasicProperties;                               // 0x0128 (size: 0x8)
    class UUVProjectionToolEditActions* EditActions;                                  // 0x0130 (size: 0x8)
    class UExistingMeshMaterialProperties* MaterialSettings;                          // 0x0138 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* Preview;                               // 0x0140 (size: 0x8)
    class UMaterialInstanceDynamic* CheckerMaterial;                                  // 0x0148 (size: 0x8)
    class UCombinedTransformGizmo* TransformGizmo;                                    // 0x0150 (size: 0x8)
    class UTransformProxy* TransformProxy;                                            // 0x0158 (size: 0x8)
    class UUVProjectionOperatorFactory* OperatorFactory;                              // 0x0160 (size: 0x8)
    class UPreviewGeometry* EdgeRenderer;                                             // 0x0168 (size: 0x8)
    class USingleClickInputBehavior* ClickToSetPlaneBehavior;                         // 0x04B8 (size: 0x8)

}; // Size: 0x4D0

class UUVProjectionToolBuilder : public USingleTargetWithSelectionToolBuilder
{
}; // Size: 0x28

class UUVProjectionToolEditActions : public UInteractiveToolPropertySet
{

    void Reset();
    void AutoFitAlign();
    void AutoFit();
}; // Size: 0xB0

class UUVProjectionToolProperties : public UInteractiveToolPropertySet
{
    EUVProjectionMethod ProjectionType;                                               // 0x00A8 (size: 0x1)
    FVector Dimensions;                                                               // 0x00B0 (size: 0x18)
    bool bProportionalDimensions;                                                     // 0x00C8 (size: 0x1)
    EUVProjectionToolInitializationMode Initialization;                               // 0x00CC (size: 0x4)
    float CylinderSplitAngle;                                                         // 0x00D0 (size: 0x4)
    float ExpMapNormalBlending;                                                       // 0x00D4 (size: 0x4)
    int32 ExpMapSmoothingSteps;                                                       // 0x00D8 (size: 0x4)
    float ExpMapSmoothingAlpha;                                                       // 0x00DC (size: 0x4)
    float Rotation;                                                                   // 0x00E0 (size: 0x4)
    FVector2D Scale;                                                                  // 0x00E8 (size: 0x10)
    FVector2D Translation;                                                            // 0x00F8 (size: 0x10)
    FVector SavedDimensions;                                                          // 0x0108 (size: 0x18)
    bool bSavedProportionalDimensions;                                                // 0x0120 (size: 0x1)
    FTransform SavedTransform;                                                        // 0x0130 (size: 0x60)

}; // Size: 0x190

class UVertexBrushAlphaProperties : public UInteractiveToolPropertySet
{
    class UTexture2D* Alpha;                                                          // 0x00A8 (size: 0x8)
    float RotationAngle;                                                              // 0x00B0 (size: 0x4)
    bool bRandomize;                                                                  // 0x00B4 (size: 0x1)
    float RandomRange;                                                                // 0x00B8 (size: 0x4)
    TWeakObjectPtr<class UMeshVertexSculptTool> Tool;                                 // 0x00BC (size: 0x8)

}; // Size: 0xC8

class UVertexBrushSculptProperties : public UInteractiveToolPropertySet
{
    EMeshVertexSculptBrushType PrimaryBrushType;                                      // 0x00A8 (size: 0x1)
    int32 PrimaryBrushID;                                                             // 0x00AC (size: 0x4)
    EMeshSculptFalloffType PrimaryFalloffType;                                        // 0x00B0 (size: 0x1)
    EMeshVertexSculptBrushFilterType BrushFilter;                                     // 0x00B1 (size: 0x1)
    bool bFreezeTarget;                                                               // 0x00B2 (size: 0x1)
    bool bCanFreezeTarget;                                                            // 0x00B3 (size: 0x1)
    TWeakObjectPtr<class UMeshVertexSculptTool> Tool;                                 // 0x00B4 (size: 0x8)

}; // Size: 0xC0

class UVertexColorBaseBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    EVertexColorPaintBrushOpBlendMode BlendMode;                                      // 0x00B8 (size: 0x4)
    bool bApplyFalloff;                                                               // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UVertexColorPaintBrushOpProps : public UVertexColorBaseBrushOpProps
{
    FLinearColor Color;                                                               // 0x00C0 (size: 0x10)

}; // Size: 0xD0

class UVertexColorSmoothBrushOpProps : public UVertexColorBaseBrushOpProps
{
}; // Size: 0xC0

class UVertexColorSoftenBrushOpProps : public UVertexColorBaseBrushOpProps
{
}; // Size: 0xC0

class UVertexPaintBasicProperties : public UInteractiveToolPropertySet
{
    EMeshVertexPaintBrushType PrimaryBrushType;                                       // 0x00A8 (size: 0x1)
    EMeshVertexPaintInteractionType SubToolType;                                      // 0x00A9 (size: 0x1)
    FLinearColor PaintColor;                                                          // 0x00AC (size: 0x10)
    bool bIsPaintPressureEnabled;                                                     // 0x00BC (size: 0x1)
    EMeshVertexPaintColorBlendMode BlendMode;                                         // 0x00BD (size: 0x1)
    EMeshVertexPaintSecondaryActionType SecondaryActionType;                          // 0x00BE (size: 0x1)
    FLinearColor EraseColor;                                                          // 0x00C0 (size: 0x10)
    bool bIsErasePressureEnabled;                                                     // 0x00D0 (size: 0x1)
    float SmoothStrength;                                                             // 0x00D4 (size: 0x4)
    FModelingToolsColorChannelFilter ChannelFilter;                                   // 0x00D8 (size: 0x4)
    bool bHardEdges;                                                                  // 0x00DC (size: 0x1)

}; // Size: 0xE0

class UVertexPaintBrushFilterProperties : public UInteractiveToolPropertySet
{
    EMeshVertexPaintBrushAreaType BrushAreaMode;                                      // 0x00A8 (size: 0x1)
    float AngleThreshold;                                                             // 0x00AC (size: 0x4)
    bool bUVSeams;                                                                    // 0x00B0 (size: 0x1)
    bool bNormalSeams;                                                                // 0x00B1 (size: 0x1)
    EMeshVertexPaintVisibilityType VisibilityFilter;                                  // 0x00B2 (size: 0x1)
    bool bIsolateGeometrySelection;                                                   // 0x00B3 (size: 0x1)
    bool bToolHasSelection;                                                           // 0x00B4 (size: 0x1)
    int32 MinTriVertCount;                                                            // 0x00B8 (size: 0x4)
    EMeshVertexPaintMaterialMode MaterialMode;                                        // 0x00BC (size: 0x1)
    bool bShowHitColor;                                                               // 0x00BD (size: 0x1)
    EMeshVertexPaintInteractionType CurrentSubToolType;                               // 0x00BE (size: 0x1)

}; // Size: 0xC0

class UViewAlignedPlaneBrushOpProps : public UBasePlaneBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)
    float Depth;                                                                      // 0x00B8 (size: 0x4)
    EPlaneBrushSideMode WhichSide;                                                    // 0x00BC (size: 0x1)

}; // Size: 0xC0

class UViewAlignedSculptBrushOpProps : public UMeshSculptBrushOpProps
{
    float Strength;                                                                   // 0x00B0 (size: 0x4)
    float Falloff;                                                                    // 0x00B4 (size: 0x4)

}; // Size: 0xB8

class UWeldMeshEdgesOperatorFactory : public UObject
{
    class UWeldMeshEdgesTool* WeldMeshEdgesTool;                                      // 0x0030 (size: 0x8)

}; // Size: 0x38

class UWeldMeshEdgesTool : public USingleTargetWithSelectionTool
{
    class UWeldMeshEdgesToolProperties* Settings;                                     // 0x0120 (size: 0x8)
    class UMeshOpPreviewWithBackgroundCompute* PreviewCompute;                        // 0x0128 (size: 0x8)
    class UMeshElementsVisualizer* MeshElementsDisplay;                               // 0x0130 (size: 0x8)
    class UWeldMeshEdgesOperatorFactory* OperatorFactory;                             // 0x0138 (size: 0x8)

}; // Size: 0x1A0

class UWeldMeshEdgesToolBuilder : public USingleTargetWithSelectionToolBuilder
{
}; // Size: 0x28

class UWeldMeshEdgesToolProperties : public UInteractiveToolPropertySet
{
    float Tolerance;                                                                  // 0x00A8 (size: 0x4)
    bool bOnlyUnique;                                                                 // 0x00AC (size: 0x1)
    bool bResolveTJunctions;                                                          // 0x00AD (size: 0x1)
    bool bSplitBowties;                                                               // 0x00AE (size: 0x1)
    int32 InitialEdges;                                                               // 0x00B0 (size: 0x4)
    int32 RemainingEdges;                                                             // 0x00B4 (size: 0x4)
    EWeldMeshEdgesAttributeUIMode AttrWeldingMode;                                    // 0x00B8 (size: 0x1)
    float SplitNormalThreshold;                                                       // 0x00BC (size: 0x4)
    float SplitTangentsThreshold;                                                     // 0x00C0 (size: 0x4)
    float SplitUVThreshold;                                                           // 0x00C4 (size: 0x4)
    float SplitColorThreshold;                                                        // 0x00C8 (size: 0x4)

}; // Size: 0xD0

class UWorkPlaneProperties : public UInteractiveToolPropertySet
{
    bool bPropertySetEnabled;                                                         // 0x00A8 (size: 0x1)
    bool bShowGizmo;                                                                  // 0x00A9 (size: 0x1)
    FVector Position;                                                                 // 0x00B0 (size: 0x18)
    FQuat Rotation;                                                                   // 0x00D0 (size: 0x20)

}; // Size: 0xF0

#endif
