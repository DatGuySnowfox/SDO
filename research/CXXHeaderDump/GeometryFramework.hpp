#ifndef UE4SS_SDK_GeometryFramework_HPP
#define UE4SS_SDK_GeometryFramework_HPP

#include "GeometryFramework_enums.hpp"

struct FDynamicMeshChangeInfo
{
    EDynamicMeshChangeType Type;                                                      // 0x0000 (size: 0x1)
    EDynamicMeshAttributeChangeFlags Flags;                                           // 0x0001 (size: 0x1)
    bool bIsRevertChange;                                                             // 0x0002 (size: 0x1)

}; // Size: 0x20

class ADynamicMeshActor : public AActor
{
    class UDynamicMeshComponent* DynamicMeshComponent;                                // 0x02A8 (size: 0x8)
    bool bEnableComputeMeshPool;                                                      // 0x02B0 (size: 0x1)
    class UDynamicMeshPool* DynamicMeshPool;                                          // 0x02B8 (size: 0x8)

    bool ReleaseComputeMesh(class UDynamicMesh* Mesh);
    void ReleaseAllComputeMeshes();
    class UDynamicMeshComponent* GetDynamicMeshComponent();
    class UDynamicMeshPool* GetComputeMeshPool();
    void FreeAllComputeMeshes();
    class UDynamicMesh* AllocateComputeMesh();
}; // Size: 0x2C0

class IMeshCommandChangeTarget : public IInterface
{
}; // Size: 0x28

class IMeshReplacementCommandChangeTarget : public IInterface
{
}; // Size: 0x28

class IMeshVertexCommandChangeTarget : public IInterface
{
}; // Size: 0x28

class UBaseDynamicMeshComponent : public UMeshComponent
{
    bool bExplicitShowWireframe;                                                      // 0x0588 (size: 0x1)
    FLinearColor WireframeColor;                                                      // 0x058C (size: 0x10)
    EDynamicMeshComponentColorOverrideMode ColorMode;                                 // 0x059C (size: 0x1)
    FColor ConstantColor;                                                             // 0x05A0 (size: 0x4)
    EDynamicMeshVertexColorTransformMode ColorSpaceMode;                              // 0x05A4 (size: 0x1)
    bool bEnableFlatShading;                                                          // 0x05A5 (size: 0x1)
    bool bEnableViewModeOverrides;                                                    // 0x05A6 (size: 0x1)
    class UMaterialInterface* OverrideRenderMaterial;                                 // 0x05A8 (size: 0x8)
    class UMaterialInterface* SecondaryRenderMaterial;                                // 0x05B0 (size: 0x8)
    class UMaterialInterface* WireframeMaterialOverride;                              // 0x05C0 (size: 0x8)
    class UMaterialInterface* SecondaryWireframeMaterialOverride;                     // 0x05C8 (size: 0x8)
    bool bEnableRayTracing;                                                           // 0x05D0 (size: 0x1)
    EDynamicMeshDrawPath DrawPath;                                                    // 0x05D1 (size: 0x1)
    EDynamicMeshComponentDistanceFieldMode DistanceFieldMode;                         // 0x05D2 (size: 0x1)
    TArray<UMaterialInterface*> BaseMaterials;                                        // 0x05D8 (size: 0x10)

    void SetViewModeOverridesEnabled(bool bEnabled);
    void SetVertexColorSpaceTransformMode(EDynamicMeshVertexColorTransformMode NewMode);
    void SetShadowsEnabled(bool bEnabled);
    void SetSecondaryRenderMaterial(class UMaterialInterface* Material);
    void SetSecondaryBuffersVisibility(bool bSetVisible);
    void SetOverrideWireframeRenderMaterial(class UMaterialInterface* Material);
    void SetOverrideSecondaryWireframeRenderMaterial(class UMaterialInterface* Material);
    void SetOverrideRenderMaterial(class UMaterialInterface* Material);
    void SetMeshDrawPath(EDynamicMeshDrawPath NewDrawPath);
    void SetEnableWireframeRenderPass(bool bEnable);
    void SetEnableRaytracing(bool bSetEnabled);
    void SetEnableFlatShading(bool bEnable);
    void SetDistanceFieldMode(EDynamicMeshComponentDistanceFieldMode NewDistFieldMode);
    void SetConstantOverrideColor(FColor NewColor);
    void SetColorOverrideMode(EDynamicMeshComponentColorOverrideMode NewMode);
    bool HasOverrideRenderMaterial(int32 K);
    bool GetViewModeOverridesEnabled();
    EDynamicMeshVertexColorTransformMode GetVertexColorSpaceTransformMode();
    bool GetShadowsEnabled();
    class UMaterialInterface* GetSecondaryRenderMaterial();
    bool GetSecondaryBuffersVisibility();
    class UMaterialInterface* GetOverrideWireframeRenderMaterial();
    class UMaterialInterface* GetOverrideSecondaryWireframeRenderMaterial();
    class UMaterialInterface* GetOverrideRenderMaterial(int32 MaterialIndex);
    EDynamicMeshDrawPath GetMeshDrawPath();
    bool GetFlatShadingEnabled();
    bool GetEnableWireframeRenderPass();
    bool GetEnableRaytracing();
    class UDynamicMesh* GetDynamicMesh();
    EDynamicMeshComponentDistanceFieldMode GetDistanceFieldMode();
    FColor GetConstantOverrideColor();
    EDynamicMeshComponentColorOverrideMode GetColorOverrideMode();
    void ClearSecondaryRenderMaterial();
    void ClearOverrideWireframeRenderMaterial();
    void ClearOverrideSecondaryWireframeRenderMaterial();
    void ClearOverrideRenderMaterial();
}; // Size: 0x5F0

class UDynamicMesh : public UObject
{
    FDynamicMeshMeshModifiedBPEvent MeshModifiedBPEvent;                              // 0x0070 (size: 0x10)
    void OnDynamicMeshModifiedBP(class UDynamicMesh* Mesh);
    class UDynamicMeshGenerator* MeshGenerator;                                       // 0x00A0 (size: 0x8)
    bool bEnableMeshGenerator;                                                        // 0x00A8 (size: 0x1)

    class UDynamicMesh* ResetToCube();
    class UDynamicMesh* Reset();
    bool IsEmpty();
    int32 GetTriangleCount();
}; // Size: 0xB0

class UDynamicMeshComponent : public UBaseDynamicMeshComponent
{
    class UDynamicMesh* MeshObject;                                                   // 0x05F0 (size: 0x8)
    bool bAllowsGeometrySelection;                                                    // 0x0651 (size: 0x1)
    EDynamicMeshComponentTangentsMode TangentsType;                                   // 0x0720 (size: 0x1)
    TEnumAsByte<ECollisionTraceFlag> CollisionType;                                   // 0x0810 (size: 0x1)
    bool bUseAsyncCooking;                                                            // 0x0811 (size: 0x1)
    bool bEnableComplexCollision;                                                     // 0x0812 (size: 0x1)
    bool bDeferCollisionUpdates;                                                      // 0x0813 (size: 0x1)
    bool bDisableMeshUVHitResults;                                                    // 0x0814 (size: 0x1)
    class UBodySetup* MeshBodySetup;                                                  // 0x0818 (size: 0x8)
    FKAggregateGeom AggGeom;                                                          // 0x0858 (size: 0xA0)
    TArray<UBodySetup*> AsyncBodySetupQueue;                                          // 0x08F8 (size: 0x10)

    bool ValidateMaterialSlots(bool bCreateIfMissing, bool bDeleteExtraSlots);
    void UpdateCollision(bool bOnlyIfPending);
    void SetTangentsType(EDynamicMeshComponentTangentsMode NewTangentsType);
    void SetDynamicMesh(class UDynamicMesh* NewMesh);
    void SetDeferredCollisionUpdatesEnabled(bool bEnabled, bool bImmediateUpdate);
    void SetComplexAsSimpleCollisionEnabled(bool bEnabled, bool bImmediateUpdate);
    void SetAllowsGeometrySelection(bool bInAllowsGeometrySelection);
    void NotifyMeshVertexAttributesModified(bool bPositions, bool bNormals, bool bUVs, bool bColors);
    void NotifyMeshModified();
    EDynamicMeshComponentTangentsMode GetTangentsTypePure();
    EDynamicMeshComponentTangentsMode GetTangentsType();
    void EnableComplexAsSimpleCollision();
    void ConfigureMaterialSet(const TArray<UMaterialInterface*>& NewMaterialSet, bool bDeleteExtraSlots);
    bool AllowsGeometrySelection();
}; // Size: 0x910

class UDynamicMeshGenerator : public UObject
{
}; // Size: 0x28

class UDynamicMeshPool : public UObject
{
    TArray<UDynamicMesh*> CachedMeshes;                                               // 0x0028 (size: 0x10)
    TArray<UDynamicMesh*> AllCreatedMeshes;                                           // 0x0038 (size: 0x10)

    void ReturnMesh(class UDynamicMesh* Mesh);
    void ReturnAllMeshes();
    class UDynamicMesh* RequestMesh();
    void FreeAllMeshes();
}; // Size: 0x48

class UDynamicMeshProcessorBlueprint : public UObject
{

    void ProcessDynamicMesh(class UDynamicMesh* TargetMesh, bool& bFailed);
}; // Size: 0x28

#endif
