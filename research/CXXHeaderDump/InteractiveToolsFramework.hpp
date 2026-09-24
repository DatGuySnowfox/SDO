#ifndef UE4SS_SDK_InteractiveToolsFramework_HPP
#define UE4SS_SDK_InteractiveToolsFramework_HPP

#include "InteractiveToolsFramework_enums.hpp"

struct FActiveGizmo
{
    class UInteractiveGizmo* Gizmo;                                                   // 0x0000 (size: 0x8)

}; // Size: 0x30

struct FBehaviorInfo
{
    class UInputBehavior* Behavior;                                                   // 0x0000 (size: 0x8)

}; // Size: 0x20

struct FBrushStampData
{
}; // Size: 0x140

struct FDeviceButtonState
{
    FKey Button;                                                                      // 0x0000 (size: 0x18)
    bool bPressed;                                                                    // 0x0018 (size: 0x1)
    bool bDown;                                                                       // 0x0019 (size: 0x1)
    bool bReleased;                                                                   // 0x001A (size: 0x1)
    bool bDoubleClicked;                                                              // 0x001B (size: 0x1)

}; // Size: 0x20

struct FGizmoElementColorAttribute
{
    FLinearColor Value;                                                               // 0x0000 (size: 0x10)
    bool bHasValue;                                                                   // 0x0010 (size: 0x1)
    bool bOverridesChildState;                                                        // 0x0011 (size: 0x1)

}; // Size: 0x14

struct FGizmoElementLineRenderStateAttributes
{
    FGizmoElementColorAttribute LineColor;                                            // 0x0000 (size: 0x14)
    FGizmoElementColorAttribute HoverLineColor;                                       // 0x0014 (size: 0x14)
    FGizmoElementColorAttribute InteractLineColor;                                    // 0x0028 (size: 0x14)

}; // Size: 0x3C

struct FGizmoElementMaterialAttribute
{
    TWeakObjectPtr<class UMaterialInterface> Value;                                   // 0x0000 (size: 0x8)
    bool bOverridesChildState;                                                        // 0x0008 (size: 0x1)

}; // Size: 0xC

struct FGizmoElementMeshRenderStateAttributes
{
    FGizmoElementMaterialAttribute Material;                                          // 0x0000 (size: 0xC)
    FGizmoElementMaterialAttribute HoverMaterial;                                     // 0x000C (size: 0xC)
    FGizmoElementMaterialAttribute InteractMaterial;                                  // 0x0018 (size: 0xC)
    FGizmoElementColorAttribute VertexColor;                                          // 0x0024 (size: 0x14)
    FGizmoElementColorAttribute HoverVertexColor;                                     // 0x0038 (size: 0x14)
    FGizmoElementColorAttribute InteractVertexColor;                                  // 0x004C (size: 0x14)

}; // Size: 0x60

struct FGizmoFloatParameterChange
{
    float InitialValue;                                                               // 0x0000 (size: 0x4)
    float CurrentValue;                                                               // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FGizmoVec2ParameterChange
{
    FVector2D InitialValue;                                                           // 0x0000 (size: 0x10)
    FVector2D CurrentValue;                                                           // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FInputDeviceRay
{
    FRay WorldRay;                                                                    // 0x0000 (size: 0x30)
    bool bHas2D;                                                                      // 0x0030 (size: 0x1)
    FVector2D ScreenPosition;                                                         // 0x0038 (size: 0x10)

}; // Size: 0x48

struct FInputDeviceState
{
    EInputDevices InputDevice;                                                        // 0x0000 (size: 0x4)
    bool bShiftKeyDown;                                                               // 0x0004 (size: 0x1)
    bool bAltKeyDown;                                                                 // 0x0005 (size: 0x1)
    bool bCtrlKeyDown;                                                                // 0x0006 (size: 0x1)
    bool bCmdKeyDown;                                                                 // 0x0007 (size: 0x1)
    FKeyboardInputDeviceState Keyboard;                                               // 0x0008 (size: 0x20)
    FMouseInputDeviceState Mouse;                                                     // 0x0028 (size: 0xB8)

}; // Size: 0xE0

struct FInputRayHit
{
    bool bHit;                                                                        // 0x0000 (size: 0x1)
    double HitDepth;                                                                  // 0x0008 (size: 0x8)
    FVector HitNormal;                                                                // 0x0010 (size: 0x18)
    bool bHasHitNormal;                                                               // 0x0028 (size: 0x1)
    int32 HitIdentifier;                                                              // 0x002C (size: 0x4)
    TWeakObjectPtr<class UObject> HitObject;                                          // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FKeyboardInputDeviceState
{
    FDeviceButtonState ActiveKey;                                                     // 0x0000 (size: 0x20)

}; // Size: 0x20

struct FMouseInputDeviceState
{
    FDeviceButtonState Left;                                                          // 0x0000 (size: 0x20)
    FDeviceButtonState Middle;                                                        // 0x0020 (size: 0x20)
    FDeviceButtonState Right;                                                         // 0x0040 (size: 0x20)
    float WheelDelta;                                                                 // 0x0060 (size: 0x4)
    FVector2D Position2D;                                                             // 0x0068 (size: 0x10)
    FVector2D Delta2D;                                                                // 0x0078 (size: 0x10)
    FRay WorldRay;                                                                    // 0x0088 (size: 0x30)

}; // Size: 0xB8

class ACombinedTransformGizmoActor : public AGizmoActor
{
    class UPrimitiveComponent* TranslateX;                                            // 0x02B0 (size: 0x8)
    class UPrimitiveComponent* TranslateY;                                            // 0x02B8 (size: 0x8)
    class UPrimitiveComponent* TranslateZ;                                            // 0x02C0 (size: 0x8)
    class UPrimitiveComponent* TranslateYZ;                                           // 0x02C8 (size: 0x8)
    class UPrimitiveComponent* TranslateXZ;                                           // 0x02D0 (size: 0x8)
    class UPrimitiveComponent* TranslateXY;                                           // 0x02D8 (size: 0x8)
    class UPrimitiveComponent* RotateX;                                               // 0x02E0 (size: 0x8)
    class UPrimitiveComponent* RotateY;                                               // 0x02E8 (size: 0x8)
    class UPrimitiveComponent* RotateZ;                                               // 0x02F0 (size: 0x8)
    class UPrimitiveComponent* RotationSphere;                                        // 0x02F8 (size: 0x8)
    class UPrimitiveComponent* FreeRotateHandle;                                      // 0x0300 (size: 0x8)
    class UPrimitiveComponent* FreeTranslateHandle;                                   // 0x0308 (size: 0x8)
    class UPrimitiveComponent* UniformScale;                                          // 0x0310 (size: 0x8)
    class UPrimitiveComponent* AxisScaleX;                                            // 0x0318 (size: 0x8)
    class UPrimitiveComponent* AxisScaleY;                                            // 0x0320 (size: 0x8)
    class UPrimitiveComponent* AxisScaleZ;                                            // 0x0328 (size: 0x8)
    class UPrimitiveComponent* PlaneScaleYZ;                                          // 0x0330 (size: 0x8)
    class UPrimitiveComponent* PlaneScaleXZ;                                          // 0x0338 (size: 0x8)
    class UPrimitiveComponent* PlaneScaleXY;                                          // 0x0340 (size: 0x8)
    class UPrimitiveComponent* FullAxisScaleX;                                        // 0x0358 (size: 0x8)
    class UPrimitiveComponent* FullAxisScaleY;                                        // 0x0360 (size: 0x8)
    class UPrimitiveComponent* FullAxisScaleZ;                                        // 0x0368 (size: 0x8)

}; // Size: 0x370

class AGizmoActor : public AInternalToolFrameworkActor
{
}; // Size: 0x2B0

class AInternalToolFrameworkActor : public AActor
{
    bool bIsSelectableInEditor;                                                       // 0x02A8 (size: 0x1)

}; // Size: 0x2B0

class AIntervalGizmoActor : public AGizmoActor
{
    class UGizmoLineHandleComponent* UpIntervalComponent;                             // 0x02B0 (size: 0x8)
    class UGizmoLineHandleComponent* DownIntervalComponent;                           // 0x02B8 (size: 0x8)
    class UGizmoLineHandleComponent* ForwardIntervalComponent;                        // 0x02C0 (size: 0x8)
    class UGizmoLineHandleComponent* BackwardIntervalComponent;                       // 0x02C8 (size: 0x8)
    class UGizmoLineHandleComponent* RightIntervalComponent;                          // 0x02D0 (size: 0x8)
    class UGizmoLineHandleComponent* LeftIntervalComponent;                           // 0x02D8 (size: 0x8)

}; // Size: 0x2E0

class IAssetBackedTarget : public IInterface
{
}; // Size: 0x28

class IGizmoAxisSource : public IInterface
{

    bool HasTangentVectors();
    void GetTangentVectors(FVector& TangentXOut, FVector& TangentYOut);
    FVector GetOrigin();
    FVector GetDirection();
}; // Size: 0x28

class IGizmoBaseComponentInterface : public IInterface
{
}; // Size: 0x28

class IGizmoClickMultiTarget : public IInterface
{

    void UpdateInteractingState(bool bInteracting, uint32 InPartIdentifier);
    void UpdateHoverState(bool bHovering, uint32 InPartIdentifier);
    void UpdateHittableState(bool bHittable, uint32 InPartIdentifier);
}; // Size: 0x28

class IGizmoClickTarget : public IInterface
{

    void UpdateInteractingState(bool bInteracting);
    void UpdateHoverState(bool bHovering);
}; // Size: 0x28

class IGizmoFloatParameterSource : public IInterface
{

    void SetParameter(float NewValue);
    float GetParameter();
    void EndModify();
    void BeginModify();
}; // Size: 0x28

class IGizmoRenderMultiTarget : public IInterface
{

    void UpdateVisibilityState(bool bVisible, uint32 InPartIdentifier);
}; // Size: 0x28

class IGizmoRenderTarget : public IInterface
{
}; // Size: 0x28

class IGizmoStateTarget : public IInterface
{

    void EndUpdate();
    void BeginUpdate();
}; // Size: 0x28

class IGizmoTransformSource : public IInterface
{

    void SetTransform(const FTransform& NewTransform);
    FTransform GetTransform();
}; // Size: 0x28

class IGizmoVec2ParameterSource : public IInterface
{

    void SetParameter(const FVector2D& NewValue);
    FVector2D GetParameter();
    void EndModify();
    void BeginModify();
}; // Size: 0x28

class IInputBehaviorSource : public IInterface
{
}; // Size: 0x28

class IInteractiveToolCameraFocusAPI : public IInterface
{
}; // Size: 0x28

class IInteractiveToolEditorGizmoAPI : public IInterface
{
}; // Size: 0x28

class IInteractiveToolExclusiveToolAPI : public IInterface
{
}; // Size: 0x28

class IInteractiveToolManageGeometrySelectionAPI : public IInterface
{
}; // Size: 0x28

class IInteractiveToolNestedAcceptCancelAPI : public IInterface
{
}; // Size: 0x28

class IInteractiveToolShutdownQueryAPI : public IInterface
{
}; // Size: 0x28

class IMaterialProvider : public IInterface
{
}; // Size: 0x28

class IMeshDescriptionCommitter : public IInterface
{
}; // Size: 0x28

class IMeshDescriptionProvider : public IInterface
{
}; // Size: 0x28

class IPhysicsDataSource : public IInterface
{
}; // Size: 0x28

class IPrimitiveComponentBackedTarget : public ISceneComponentBackedTarget
{
}; // Size: 0x28

class ISceneComponentBackedTarget : public IInterface
{
}; // Size: 0x28

class ISkeletalMeshBackedTarget : public IAssetBackedTarget
{
}; // Size: 0x28

class IStaticMeshBackedTarget : public IAssetBackedTarget
{
}; // Size: 0x28

class IToolContextTransactionProvider : public IInterface
{
}; // Size: 0x28

class IToolFrameworkComponent : public IInterface
{
}; // Size: 0x28

class IWidgetBaseBehavior : public IInterface
{
}; // Size: 0x28

class UAnyButtonInputBehavior : public UInputBehavior
{
}; // Size: 0x70

class UAxisAngleGizmo : public UInteractiveGizmo
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x0048 (size: 0x10)
    TScriptInterface<class IGizmoFloatParameterSource> AngleSource;                   // 0x0058 (size: 0x10)
    TScriptInterface<class IGizmoClickTarget> HitTarget;                              // 0x0068 (size: 0x10)
    TScriptInterface<class IGizmoStateTarget> StateTarget;                            // 0x0078 (size: 0x10)
    class UClickDragInputBehavior* MouseBehavior;                                     // 0x0088 (size: 0x8)
    bool bInInteraction;                                                              // 0x00F0 (size: 0x1)
    FVector RotationOrigin;                                                           // 0x00F8 (size: 0x18)
    FVector RotationAxis;                                                             // 0x0110 (size: 0x18)
    FVector RotationPlaneX;                                                           // 0x0128 (size: 0x18)
    FVector RotationPlaneY;                                                           // 0x0140 (size: 0x18)
    FVector InteractionStartPoint;                                                    // 0x0158 (size: 0x18)
    FVector InteractionCurPoint;                                                      // 0x0170 (size: 0x18)
    float InteractionStartAngle;                                                      // 0x0188 (size: 0x4)
    float InteractionCurAngle;                                                        // 0x018C (size: 0x4)

}; // Size: 0x1B0

class UAxisAngleGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x28

class UAxisPositionGizmo : public UInteractiveGizmo
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x0048 (size: 0x10)
    TScriptInterface<class IGizmoFloatParameterSource> ParameterSource;               // 0x0058 (size: 0x10)
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0068 (size: 0x8)
    TScriptInterface<class IGizmoClickTarget> HitTarget;                              // 0x0070 (size: 0x10)
    TScriptInterface<class IGizmoStateTarget> StateTarget;                            // 0x0080 (size: 0x10)
    class UClickDragInputBehavior* MouseBehavior;                                     // 0x0090 (size: 0x8)
    bool bEnableSignedAxis;                                                           // 0x0098 (size: 0x1)
    bool bInInteraction;                                                              // 0x0101 (size: 0x1)
    FVector InteractionOrigin;                                                        // 0x0108 (size: 0x18)
    FVector InteractionAxis;                                                          // 0x0120 (size: 0x18)
    FVector InteractionStartPoint;                                                    // 0x0138 (size: 0x18)
    FVector InteractionCurPoint;                                                      // 0x0150 (size: 0x18)
    float InteractionStartParameter;                                                  // 0x0168 (size: 0x4)
    float InteractionCurParameter;                                                    // 0x016C (size: 0x4)
    float ParameterSign;                                                              // 0x0170 (size: 0x4)

}; // Size: 0x1A0

class UAxisPositionGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x28

class UBaseBrushTool : public UMeshSurfacePointTool
{
    class UBrushBaseProperties* BrushProperties;                                      // 0x0100 (size: 0x8)
    bool bInBrushStroke;                                                              // 0x0108 (size: 0x1)
    float WorldToLocalScale;                                                          // 0x010C (size: 0x4)
    FBrushStampData LastBrushStamp;                                                   // 0x0110 (size: 0x140)
    TSoftClassPtr<UBrushBaseProperties> PropertyClass;                                // 0x0260 (size: 0x28)
    class UBrushStampIndicator* BrushStampIndicator;                                  // 0x0288 (size: 0x8)

}; // Size: 0x2A8

class UBrushAdjusterInputBehavior : public UAnyButtonInputBehavior
{
}; // Size: 0xC0

class UBrushBaseProperties : public UInteractiveToolPropertySet
{
    float BrushSize;                                                                  // 0x00A8 (size: 0x4)
    bool bSpecifyRadius;                                                              // 0x00AC (size: 0x1)
    float BrushRadius;                                                                // 0x00B0 (size: 0x4)
    float BrushStrength;                                                              // 0x00B4 (size: 0x4)
    float BrushFalloffAmount;                                                         // 0x00B8 (size: 0x4)
    bool bShowStrength;                                                               // 0x00BC (size: 0x1)
    bool bShowFalloff;                                                                // 0x00BD (size: 0x1)

}; // Size: 0xC0

class UBrushStampIndicator : public UInteractiveGizmo
{
    bool bVisible;                                                                    // 0x0038 (size: 0x1)
    float BrushRadius;                                                                // 0x003C (size: 0x4)
    float BrushFalloff;                                                               // 0x0040 (size: 0x4)
    float BrushStrength;                                                              // 0x0044 (size: 0x4)
    FVector BrushPosition;                                                            // 0x0048 (size: 0x18)
    FVector BrushNormal;                                                              // 0x0060 (size: 0x18)
    bool bDrawIndicatorLines;                                                         // 0x0078 (size: 0x1)
    bool bDrawRadiusCircle;                                                           // 0x0079 (size: 0x1)
    bool bScaleNormalByStrength;                                                      // 0x007A (size: 0x1)
    int32 SampleStepCount;                                                            // 0x007C (size: 0x4)
    FLinearColor LineColor;                                                           // 0x0080 (size: 0x10)
    float LineThickness;                                                              // 0x0090 (size: 0x4)
    bool bDepthTested;                                                                // 0x0094 (size: 0x1)
    bool bDrawSecondaryLines;                                                         // 0x0095 (size: 0x1)
    float SecondaryLineThickness;                                                     // 0x0098 (size: 0x4)
    FLinearColor SecondaryLineColor;                                                  // 0x009C (size: 0x10)
    class UPrimitiveComponent* AttachedComponent;                                     // 0x00B0 (size: 0x8)

}; // Size: 0xD8

class UBrushStampIndicatorBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x28

class UClickDragInputBehavior : public UAnyButtonInputBehavior
{
    bool bUpdateModifiersDuringDrag;                                                  // 0x0100 (size: 0x1)

}; // Size: 0x120

class UClickDragTool : public UInteractiveTool
{
}; // Size: 0xA0

class UClickDragToolBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x28

class UCombinedTransformGizmo : public UInteractiveGizmo
{
    class UTransformProxy* ActiveTarget;                                              // 0x0038 (size: 0x8)
    bool bSnapToWorldGrid;                                                            // 0x0040 (size: 0x1)
    bool bGridSizeIsExplicit;                                                         // 0x0044 (size: 0x1)
    FVector ExplicitGridSize;                                                         // 0x0048 (size: 0x18)
    bool bRotationGridSizeIsExplicit;                                                 // 0x0060 (size: 0x1)
    FRotator ExplicitRotationGridSize;                                                // 0x0068 (size: 0x18)
    bool bSnapToWorldRotGrid;                                                         // 0x0080 (size: 0x1)
    bool bSnapToScaleGrid;                                                            // 0x0081 (size: 0x1)
    bool bUseContextCoordinateSystem;                                                 // 0x0082 (size: 0x1)
    EToolContextCoordinateSystem CurrentCoordinateSystem;                             // 0x0083 (size: 0x1)
    bool bUseContextGizmoMode;                                                        // 0x0084 (size: 0x1)
    EToolContextTransformGizmoMode ActiveGizmoMode;                                   // 0x0085 (size: 0x1)
    TArray<UPrimitiveComponent*> ActiveComponents;                                    // 0x0190 (size: 0x10)
    TArray<UInteractiveGizmo*> ActiveGizmos;                                          // 0x01A0 (size: 0x10)
    class UGizmoConstantFrameAxisSource* CameraAxisSource;                            // 0x03D0 (size: 0x8)
    class UGizmoComponentAxisSource* AxisXSource;                                     // 0x03D8 (size: 0x8)
    class UGizmoComponentAxisSource* AxisYSource;                                     // 0x03E0 (size: 0x8)
    class UGizmoComponentAxisSource* AxisZSource;                                     // 0x03E8 (size: 0x8)
    class UGizmoComponentAxisSource* UnitAxisXSource;                                 // 0x03F0 (size: 0x8)
    class UGizmoComponentAxisSource* UnitAxisYSource;                                 // 0x03F8 (size: 0x8)
    class UGizmoComponentAxisSource* UnitAxisZSource;                                 // 0x0400 (size: 0x8)
    class UGizmoTransformChangeStateTarget* StateTarget;                              // 0x0408 (size: 0x8)

}; // Size: 0x540

class UCombinedTransformGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0xD0

class UCombinedTransformGizmoContextObject : public UObject
{
}; // Size: 0x58

class UComponentBoundTransformProxy : public UTransformProxy
{
}; // Size: 0x1C0

class UContextObjectStore : public UObject
{
    TArray<UObject*> ContextObjects;                                                  // 0x0028 (size: 0x10)

}; // Size: 0x38

class UDoubleClickInputBehavior : public USingleClickInputBehavior
{
}; // Size: 0x110

class UFreePositionSubGizmo : public UPlanePositionGizmo
{
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x0280 (size: 0x10)

}; // Size: 0x290

class UFreeRotationSubGizmo : public UInteractiveGizmo
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x0048 (size: 0x10)
    TScriptInterface<class IGizmoClickTarget> HitTarget;                              // 0x0058 (size: 0x10)
    TScriptInterface<class IGizmoStateTarget> StateTarget;                            // 0x0068 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x0078 (size: 0x10)
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0088 (size: 0x8)
    class UClickDragInputBehavior* MouseBehavior;                                     // 0x0090 (size: 0x8)

}; // Size: 0x100

class UGizmoArrowComponent : public UGizmoBaseComponent
{
    FVector Direction;                                                                // 0x0558 (size: 0x18)
    float Gap;                                                                        // 0x0570 (size: 0x4)
    float Length;                                                                     // 0x0574 (size: 0x4)
    float Thickness;                                                                  // 0x0578 (size: 0x4)

}; // Size: 0x580

class UGizmoAxisIntervalParameterSource : public UGizmoBaseFloatParameterSource
{
    TScriptInterface<class IGizmoFloatParameterSource> FloatParameterSource;          // 0x0048 (size: 0x10)
    float MinParameter;                                                               // 0x0058 (size: 0x4)
    float MaxParameter;                                                               // 0x005C (size: 0x4)

}; // Size: 0x60

class UGizmoAxisRotationParameterSource : public UGizmoBaseFloatParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x00B0 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x00C0 (size: 0x10)
    float Angle;                                                                      // 0x00D0 (size: 0x4)
    FGizmoFloatParameterChange LastChange;                                            // 0x00D4 (size: 0x8)
    FVector CurRotationAxis;                                                          // 0x00E0 (size: 0x18)
    FVector CurRotationOrigin;                                                        // 0x00F8 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x0110 (size: 0x60)

}; // Size: 0x170

class UGizmoAxisScaleParameterSource : public UGizmoBaseFloatParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x00B0 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x00C0 (size: 0x10)
    float ScaleMultiplier;                                                            // 0x00D0 (size: 0x4)
    bool bClampToZero;                                                                // 0x00D4 (size: 0x1)
    float Parameter;                                                                  // 0x00D8 (size: 0x4)
    FGizmoFloatParameterChange LastChange;                                            // 0x00DC (size: 0x8)
    FVector CurScaleAxis;                                                             // 0x00E8 (size: 0x18)
    FVector CurScaleOrigin;                                                           // 0x0100 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x0120 (size: 0x60)

}; // Size: 0x180

class UGizmoAxisTranslationParameterSource : public UGizmoBaseFloatParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x00B0 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x00C0 (size: 0x10)
    float Parameter;                                                                  // 0x00D0 (size: 0x4)
    FGizmoFloatParameterChange LastChange;                                            // 0x00D4 (size: 0x8)
    FVector CurTranslationAxis;                                                       // 0x00E0 (size: 0x18)
    FVector CurTranslationOrigin;                                                     // 0x00F8 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x0110 (size: 0x60)

}; // Size: 0x170

class UGizmoBaseComponent : public UPrimitiveComponent
{
    FLinearColor Color;                                                               // 0x0528 (size: 0x10)
    float HoverSizeMultiplier;                                                        // 0x0538 (size: 0x4)
    float PixelHitDistanceThreshold;                                                  // 0x053C (size: 0x4)
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0548 (size: 0x8)

    void UpdateWorldLocalState(bool bWorldIn);
    void UpdateHoverState(bool bHoveringIn);
}; // Size: 0x560

class UGizmoBaseFloatParameterSource : public UObject
{
}; // Size: 0x48

class UGizmoBaseTransformSource : public UObject
{
}; // Size: 0x48

class UGizmoBaseVec2ParameterSource : public UObject
{
}; // Size: 0x48

class UGizmoBoxComponent : public UGizmoBaseComponent
{
    FVector Origin;                                                                   // 0x0558 (size: 0x18)
    FQuat Rotation;                                                                   // 0x0570 (size: 0x20)
    FVector Dimensions;                                                               // 0x0590 (size: 0x18)
    float LineThickness;                                                              // 0x05A8 (size: 0x4)
    bool bRemoveHiddenLines;                                                          // 0x05AC (size: 0x1)
    bool bEnableAxisFlip;                                                             // 0x05AD (size: 0x1)

}; // Size: 0x5B0

class UGizmoCircleComponent : public UGizmoBaseComponent
{
    FVector Normal;                                                                   // 0x0558 (size: 0x18)
    float Radius;                                                                     // 0x0570 (size: 0x4)
    float Thickness;                                                                  // 0x0574 (size: 0x4)
    int32 NumSides;                                                                   // 0x0578 (size: 0x4)
    bool bViewAligned;                                                                // 0x057C (size: 0x1)
    bool bDrawFullCircle;                                                             // 0x057D (size: 0x1)
    bool bOnlyAllowFrontFacingHits;                                                   // 0x057E (size: 0x1)

}; // Size: 0x580

class UGizmoComponentAxisSource : public UObject
{
    class USceneComponent* Component;                                                 // 0x0030 (size: 0x8)
    int32 AxisIndex;                                                                  // 0x0038 (size: 0x4)
    bool bLocalAxes;                                                                  // 0x003C (size: 0x1)

}; // Size: 0x40

class UGizmoComponentHitTarget : public UObject
{
    class UPrimitiveComponent* Component;                                             // 0x0030 (size: 0x8)

}; // Size: 0xD0

class UGizmoComponentWorldTransformSource : public UGizmoBaseTransformSource
{
    class USceneComponent* Component;                                                 // 0x0048 (size: 0x8)
    bool bModifyComponentOnTransform;                                                 // 0x0050 (size: 0x1)

}; // Size: 0x58

class UGizmoConstantAxisSource : public UObject
{
    FVector Origin;                                                                   // 0x0030 (size: 0x18)
    FVector Direction;                                                                // 0x0048 (size: 0x18)

}; // Size: 0x60

class UGizmoConstantFrameAxisSource : public UObject
{
    FVector Origin;                                                                   // 0x0030 (size: 0x18)
    FVector Direction;                                                                // 0x0048 (size: 0x18)
    FVector TangentX;                                                                 // 0x0060 (size: 0x18)
    FVector TangentY;                                                                 // 0x0078 (size: 0x18)

}; // Size: 0x90

class UGizmoElementArc : public UGizmoElementCircleBase
{
    double InnerRadius;                                                               // 0x01C8 (size: 0x8)

}; // Size: 0x1D0

class UGizmoElementArrow : public UGizmoElementBase
{
    class UGizmoElementCylinder* CylinderElement;                                     // 0x0110 (size: 0x8)
    class UGizmoElementCone* ConeElement;                                             // 0x0118 (size: 0x8)
    class UGizmoElementBox* BoxElement;                                               // 0x0120 (size: 0x8)
    FVector base;                                                                     // 0x0128 (size: 0x18)
    FVector Direction;                                                                // 0x0140 (size: 0x18)
    FVector SideDirection;                                                            // 0x0158 (size: 0x18)
    float BodyLength;                                                                 // 0x0170 (size: 0x4)
    float BodyRadius;                                                                 // 0x0174 (size: 0x4)
    float HeadLength;                                                                 // 0x0178 (size: 0x4)
    float HeadRadius;                                                                 // 0x017C (size: 0x4)
    int32 NumSides;                                                                   // 0x0180 (size: 0x4)
    bool bEndCaps;                                                                    // 0x0184 (size: 0x1)
    EGizmoElementArrowHeadType HeadType;                                              // 0x0188 (size: 0x4)
    TWeakObjectPtr<class UGizmoElementBase> HitMask;                                  // 0x018C (size: 0x8)

}; // Size: 0x198

class UGizmoElementBase : public UObject
{
    bool bEnabled;                                                                    // 0x0028 (size: 0x1)
    bool bEnabledForPerspectiveProjection;                                            // 0x0029 (size: 0x1)
    bool bEnabledForOrthographicProjection;                                           // 0x002A (size: 0x1)
    bool bEnabledForDefaultState;                                                     // 0x002B (size: 0x1)
    bool bEnabledForHoveringState;                                                    // 0x002C (size: 0x1)
    bool bEnabledForInteractingState;                                                 // 0x002D (size: 0x1)
    uint32 PartIdentifier;                                                            // 0x0030 (size: 0x4)
    FGizmoElementMeshRenderStateAttributes MeshRenderAttributes;                      // 0x0034 (size: 0x60)
    EGizmoElementState ElementState;                                                  // 0x0094 (size: 0x1)
    EGizmoElementInteractionState ElementInteractionState;                            // 0x0098 (size: 0x4)
    EGizmoElementViewDependentType ViewDependentType;                                 // 0x009C (size: 0x4)
    FVector ViewDependentAxis;                                                        // 0x00A0 (size: 0x18)
    float ViewDependentAngleTol;                                                      // 0x00B8 (size: 0x4)
    float ViewDependentAxialMaxCosAngleTol;                                           // 0x00BC (size: 0x4)
    float ViewDependentPlanarMinCosAngleTol;                                          // 0x00C0 (size: 0x4)
    EGizmoElementViewAlignType ViewAlignType;                                         // 0x00C4 (size: 0x4)
    FVector ViewAlignAxis;                                                            // 0x00C8 (size: 0x18)
    FVector ViewAlignNormal;                                                          // 0x00E0 (size: 0x18)
    float ViewAlignAxialAngleTol;                                                     // 0x00F8 (size: 0x4)
    float ViewAlignAxialMaxCosAngleTol;                                               // 0x00FC (size: 0x4)
    float PixelHitDistanceThreshold;                                                  // 0x0100 (size: 0x4)

}; // Size: 0x108

class UGizmoElementBox : public UGizmoElementBase
{
    FVector Center;                                                                   // 0x0108 (size: 0x18)
    FVector Dimensions;                                                               // 0x0120 (size: 0x18)
    FVector upDirection;                                                              // 0x0138 (size: 0x18)
    FVector SideDirection;                                                            // 0x0150 (size: 0x18)

}; // Size: 0x168

class UGizmoElementCircle : public UGizmoElementCircleBase
{
    bool bDrawMesh;                                                                   // 0x01C8 (size: 0x1)
    bool bDrawLine;                                                                   // 0x01C9 (size: 0x1)
    bool bHitMesh;                                                                    // 0x01CA (size: 0x1)
    bool bHitLine;                                                                    // 0x01CB (size: 0x1)

}; // Size: 0x1D0

class UGizmoElementCircleBase : public UGizmoElementLineBase
{
    FVector Center;                                                                   // 0x0158 (size: 0x18)
    FVector Axis0;                                                                    // 0x0170 (size: 0x18)
    FVector Axis1;                                                                    // 0x0188 (size: 0x18)
    double Radius;                                                                    // 0x01A0 (size: 0x8)
    int32 NumSegments;                                                                // 0x01A8 (size: 0x4)
    EGizmoElementPartialType PartialType;                                             // 0x01AC (size: 0x4)
    double PartialStartAngle;                                                         // 0x01B0 (size: 0x8)
    double PartialEndAngle;                                                           // 0x01B8 (size: 0x8)
    double PartialViewDependentMaxCosTol;                                             // 0x01C0 (size: 0x8)

}; // Size: 0x1C8

class UGizmoElementCone : public UGizmoElementBase
{
    FVector Origin;                                                                   // 0x0108 (size: 0x18)
    FVector Direction;                                                                // 0x0120 (size: 0x18)
    float Height;                                                                     // 0x0138 (size: 0x4)
    float Radius;                                                                     // 0x013C (size: 0x4)
    int32 NumSides;                                                                   // 0x0140 (size: 0x4)
    bool bEndCaps;                                                                    // 0x0144 (size: 0x1)

}; // Size: 0x148

class UGizmoElementCylinder : public UGizmoElementBase
{
    FVector base;                                                                     // 0x0108 (size: 0x18)
    FVector Direction;                                                                // 0x0120 (size: 0x18)
    float Height;                                                                     // 0x0138 (size: 0x4)
    float Radius;                                                                     // 0x013C (size: 0x4)
    int32 NumSides;                                                                   // 0x0140 (size: 0x4)

}; // Size: 0x148

class UGizmoElementGroup : public UGizmoElementLineBase
{
    bool bConstantScale;                                                              // 0x0158 (size: 0x1)
    bool bHitOwner;                                                                   // 0x0159 (size: 0x1)
    TArray<UGizmoElementBase*> Elements;                                              // 0x0160 (size: 0x10)

}; // Size: 0x170

class UGizmoElementHitMultiTarget : public UObject
{
    class UGizmoElementBase* GizmoElement;                                            // 0x0030 (size: 0x8)
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0038 (size: 0x8)
    class UTransformProxy* GizmoTransformProxy;                                       // 0x0040 (size: 0x8)

}; // Size: 0x80

class UGizmoElementHitTarget : public UObject
{
    class UGizmoElementBase* GizmoElement;                                            // 0x0030 (size: 0x8)
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0038 (size: 0x8)
    class UTransformProxy* GizmoTransformProxy;                                       // 0x0040 (size: 0x8)

}; // Size: 0x80

class UGizmoElementLineBase : public UGizmoElementBase
{
    FGizmoElementLineRenderStateAttributes LineRenderAttributes;                      // 0x0108 (size: 0x3C)
    float LineThickness;                                                              // 0x0144 (size: 0x4)
    bool bScreenSpaceLine;                                                            // 0x0148 (size: 0x1)
    float HoverLineThicknessMultiplier;                                               // 0x014C (size: 0x4)
    float InteractLineThicknessMultiplier;                                            // 0x0150 (size: 0x4)

}; // Size: 0x158

class UGizmoElementLineStrip : public UGizmoElementLineBase
{
    TArray<FVector> Vertices;                                                         // 0x0158 (size: 0x10)
    FVector base;                                                                     // 0x0168 (size: 0x18)
    FVector upDirection;                                                              // 0x0180 (size: 0x18)
    FVector SideDirection;                                                            // 0x0198 (size: 0x18)
    bool bDrawLineStrip;                                                              // 0x01B0 (size: 0x1)

}; // Size: 0x1C8

class UGizmoElementRectangle : public UGizmoElementLineBase
{
    FVector Center;                                                                   // 0x0158 (size: 0x18)
    float Width;                                                                      // 0x0170 (size: 0x4)
    float Height;                                                                     // 0x0174 (size: 0x4)
    FVector upDirection;                                                              // 0x0178 (size: 0x18)
    FVector SideDirection;                                                            // 0x0190 (size: 0x18)
    bool bDrawMesh;                                                                   // 0x01A8 (size: 0x1)
    bool bDrawLine;                                                                   // 0x01A9 (size: 0x1)
    bool bHitMesh;                                                                    // 0x01AA (size: 0x1)
    bool bHitLine;                                                                    // 0x01AB (size: 0x1)

}; // Size: 0x1B0

class UGizmoElementSphere : public UGizmoElementBase
{
    FVector Center;                                                                   // 0x0108 (size: 0x18)
    float Radius;                                                                     // 0x0120 (size: 0x4)
    int32 NumSides;                                                                   // 0x0124 (size: 0x4)

}; // Size: 0x128

class UGizmoElementTorus : public UGizmoElementCircleBase
{
    double InnerRadius;                                                               // 0x01C8 (size: 0x8)
    int32 NumInnerSlices;                                                             // 0x01D0 (size: 0x4)
    bool bEndCaps;                                                                    // 0x01D4 (size: 0x1)

}; // Size: 0x1D8

class UGizmoElementTriangleList : public UGizmoElementBase
{
    TArray<FVector> Vertices;                                                         // 0x0108 (size: 0x10)
    FVector base;                                                                     // 0x0118 (size: 0x18)
    FVector upDirection;                                                              // 0x0130 (size: 0x18)
    FVector SideDirection;                                                            // 0x0148 (size: 0x18)

}; // Size: 0x170

class UGizmoLambdaHitTarget : public UObject
{
}; // Size: 0xC0

class UGizmoLambdaStateTarget : public UObject
{
}; // Size: 0x90

class UGizmoLineHandleComponent : public UGizmoBaseComponent
{
    FVector Normal;                                                                   // 0x0558 (size: 0x18)
    float HandleSize;                                                                 // 0x0570 (size: 0x4)
    float Thickness;                                                                  // 0x0574 (size: 0x4)
    FVector Direction;                                                                // 0x0578 (size: 0x18)
    float Length;                                                                     // 0x0590 (size: 0x4)
    bool bImageScale;                                                                 // 0x0594 (size: 0x1)

}; // Size: 0x5A0

class UGizmoLocalFloatParameterSource : public UGizmoBaseFloatParameterSource
{
    float Value;                                                                      // 0x0048 (size: 0x4)
    FGizmoFloatParameterChange LastChange;                                            // 0x004C (size: 0x8)

}; // Size: 0x58

class UGizmoLocalVec2ParameterSource : public UGizmoBaseVec2ParameterSource
{
    FVector2D Value;                                                                  // 0x0048 (size: 0x10)
    FGizmoVec2ParameterChange LastChange;                                             // 0x0058 (size: 0x20)

}; // Size: 0x78

class UGizmoNilStateTarget : public UObject
{
}; // Size: 0x30

class UGizmoObjectModifyStateTarget : public UObject
{
    TScriptInterface<class IToolContextTransactionProvider> TransactionManager;       // 0x0048 (size: 0x10)

}; // Size: 0x58

class UGizmoPlaneScaleParameterSource : public UGizmoBaseVec2ParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x0110 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x0120 (size: 0x10)
    float ScaleMultiplier;                                                            // 0x0130 (size: 0x4)
    bool bUseEqualScaling;                                                            // 0x0134 (size: 0x1)
    bool bClampToZero;                                                                // 0x0135 (size: 0x1)
    FVector2D Parameter;                                                              // 0x0138 (size: 0x10)
    FGizmoVec2ParameterChange LastChange;                                             // 0x0148 (size: 0x20)
    FVector CurScaleOrigin;                                                           // 0x0168 (size: 0x18)
    FVector CurScaleNormal;                                                           // 0x0180 (size: 0x18)
    FVector CurScaleAxisX;                                                            // 0x0198 (size: 0x18)
    FVector CurScaleAxisY;                                                            // 0x01B0 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x01D0 (size: 0x60)

}; // Size: 0x230

class UGizmoPlaneTranslationParameterSource : public UGizmoBaseVec2ParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x00E0 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x00F0 (size: 0x10)
    FVector2D Parameter;                                                              // 0x0100 (size: 0x10)
    FGizmoVec2ParameterChange LastChange;                                             // 0x0110 (size: 0x20)
    FVector CurTranslationOrigin;                                                     // 0x0130 (size: 0x18)
    FVector CurTranslationNormal;                                                     // 0x0148 (size: 0x18)
    FVector CurTranslationAxisX;                                                      // 0x0160 (size: 0x18)
    FVector CurTranslationAxisY;                                                      // 0x0178 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x0190 (size: 0x60)

}; // Size: 0x1F0

class UGizmoRectangleComponent : public UGizmoBaseComponent
{
    FVector DirectionX;                                                               // 0x0558 (size: 0x18)
    FVector DirectionY;                                                               // 0x0570 (size: 0x18)
    bool bOrientYAccordingToCamera;                                                   // 0x0588 (size: 0x1)
    float OffsetX;                                                                    // 0x058C (size: 0x4)
    float OffsetY;                                                                    // 0x0590 (size: 0x4)
    float LengthX;                                                                    // 0x0594 (size: 0x4)
    float LengthY;                                                                    // 0x0598 (size: 0x4)
    float Thickness;                                                                  // 0x059C (size: 0x4)
    uint8 SegmentFlags;                                                               // 0x05A0 (size: 0x1)

}; // Size: 0x5B0

class UGizmoScaledAndUnscaledTransformSources : public UGizmoBaseTransformSource
{
    TScriptInterface<class IGizmoTransformSource> ScaledTransformSource;              // 0x0048 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> UnscaledTransformSource;            // 0x0058 (size: 0x10)

}; // Size: 0x68

class UGizmoScaledTransformSource : public UGizmoBaseTransformSource
{
    TScriptInterface<class IGizmoTransformSource> ChildTransformSource;               // 0x0048 (size: 0x10)

}; // Size: 0xC0

class UGizmoTransformChangeStateTarget : public UObject
{
    TScriptInterface<class IToolContextTransactionProvider> TransactionManager;       // 0x0048 (size: 0x10)

}; // Size: 0x140

class UGizmoTransformProxyTransformSource : public UGizmoBaseTransformSource
{
    class UTransformProxy* Proxy;                                                     // 0x0048 (size: 0x8)

}; // Size: 0x70

class UGizmoUniformScaleParameterSource : public UGizmoBaseVec2ParameterSource
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x00B0 (size: 0x10)
    TScriptInterface<class IGizmoTransformSource> TransformSource;                    // 0x00C0 (size: 0x10)
    float ScaleMultiplier;                                                            // 0x00D0 (size: 0x4)
    FVector2D Parameter;                                                              // 0x00D8 (size: 0x10)
    FGizmoVec2ParameterChange LastChange;                                             // 0x00E8 (size: 0x20)
    FVector CurScaleOrigin;                                                           // 0x0108 (size: 0x18)
    FVector CurScaleNormal;                                                           // 0x0120 (size: 0x18)
    FVector CurScaleAxisX;                                                            // 0x0138 (size: 0x18)
    FVector CurScaleAxisY;                                                            // 0x0150 (size: 0x18)
    FTransform InitialTransform;                                                      // 0x0170 (size: 0x60)

}; // Size: 0x1D0

class UGizmoViewContext : public UObject
{
}; // Size: 0x260

class UGizmoWorldAxisSource : public UObject
{
    FVector Origin;                                                                   // 0x0030 (size: 0x18)
    int32 AxisIndex;                                                                  // 0x0048 (size: 0x4)

}; // Size: 0x50

class UInputBehavior : public UObject
{
}; // Size: 0x30

class UInputBehaviorSet : public UObject
{
    TArray<FBehaviorInfo> Behaviors;                                                  // 0x0028 (size: 0x10)

}; // Size: 0x38

class UInputRouter : public UObject
{
    bool bAutoInvalidateOnHover;                                                      // 0x0028 (size: 0x1)
    bool bAutoInvalidateOnCapture;                                                    // 0x0029 (size: 0x1)
    class UInputBehaviorSet* ActiveInputBehaviors;                                    // 0x0038 (size: 0x8)

}; // Size: 0x190

class UInteractionMechanic : public UObject
{
}; // Size: 0x30

class UInteractiveCommand : public UObject
{
}; // Size: 0x28

class UInteractiveCommandArguments : public UObject
{
}; // Size: 0x30

class UInteractiveCommandResult : public UObject
{
}; // Size: 0x28

class UInteractiveGizmo : public UObject
{
    class UInputBehaviorSet* InputBehaviors;                                          // 0x0030 (size: 0x8)

}; // Size: 0x38

class UInteractiveGizmoBuilder : public UObject
{
}; // Size: 0x28

class UInteractiveGizmoManager : public UObject
{
    TArray<FActiveGizmo> ActiveGizmos;                                                // 0x0030 (size: 0x10)
    TMap<FString, UInteractiveGizmoBuilder*> GizmoBuilders;                           // 0x0058 (size: 0x50)

}; // Size: 0xC0

class UInteractiveTool : public UObject
{
    class UInputBehaviorSet* InputBehaviors;                                          // 0x0060 (size: 0x8)
    TArray<UObject*> ToolPropertyObjects;                                             // 0x0068 (size: 0x10)

}; // Size: 0x98

class UInteractiveToolBuilder : public UObject
{
}; // Size: 0x28

class UInteractiveToolManager : public UObject
{
    class UInteractiveTool* ActiveLeftTool;                                           // 0x0058 (size: 0x8)
    class UInteractiveTool* ActiveRightTool;                                          // 0x0060 (size: 0x8)
    TMap<FString, UInteractiveToolBuilder*> ToolBuilders;                             // 0x0100 (size: 0x50)

}; // Size: 0x1B0

class UInteractiveToolPropertySet : public UObject
{
    TMap<FString, UInteractiveToolPropertySet*> CachedPropertiesMap;                  // 0x0038 (size: 0x50)
    bool bIsPropertySetEnabled;                                                       // 0x0088 (size: 0x1)

}; // Size: 0xA8

class UInteractiveToolWithToolTargetsBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x28

class UInteractiveToolsContext : public UObject
{
    class UInputRouter* InputRouter;                                                  // 0x0058 (size: 0x8)
    class UToolTargetManager* TargetManager;                                          // 0x0060 (size: 0x8)
    class UInteractiveToolManager* ToolManager;                                       // 0x0068 (size: 0x8)
    class UInteractiveGizmoManager* GizmoManager;                                     // 0x0070 (size: 0x8)
    class UContextObjectStore* ContextObjectStore;                                    // 0x0078 (size: 0x8)
    TSoftClassPtr<UInteractiveToolManager> ToolManagerClass;                          // 0x0260 (size: 0x28)

}; // Size: 0x290

class UIntervalGizmo : public UInteractiveGizmo
{
    class UGizmoTransformChangeStateTarget* StateTarget;                              // 0x0038 (size: 0x8)
    class UWorld* World;                                                              // 0x0088 (size: 0x8)
    class AIntervalGizmoActor* GizmoActor;                                            // 0x0090 (size: 0x8)
    class UTransformProxy* TransformProxy;                                            // 0x0098 (size: 0x8)
    TArray<UPrimitiveComponent*> ActiveComponents;                                    // 0x00A0 (size: 0x10)
    TArray<UInteractiveGizmo*> ActiveGizmos;                                          // 0x00B0 (size: 0x10)
    class UGizmoLocalFloatParameterSource* UpIntervalSource;                          // 0x00C0 (size: 0x8)
    class UGizmoLocalFloatParameterSource* DownIntervalSource;                        // 0x00C8 (size: 0x8)
    class UGizmoLocalFloatParameterSource* ForwardIntervalSource;                     // 0x00D0 (size: 0x8)
    class UGizmoLocalFloatParameterSource* BackwardIntervalSource;                    // 0x00D8 (size: 0x8)
    class UGizmoLocalFloatParameterSource* RightIntervalSource;                       // 0x00E0 (size: 0x8)
    class UGizmoLocalFloatParameterSource* LeftIntervalSource;                        // 0x00E8 (size: 0x8)
    class UGizmoComponentAxisSource* AxisXSource;                                     // 0x00F0 (size: 0x8)
    class UGizmoComponentAxisSource* AxisYSource;                                     // 0x00F8 (size: 0x8)
    class UGizmoComponentAxisSource* AxisZSource;                                     // 0x0100 (size: 0x8)

}; // Size: 0x210

class UIntervalGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0xA0

class UKeyAsModifierInputBehavior : public UInputBehavior
{
}; // Size: 0x98

class UKeyInputBehavior : public UInputBehavior
{
}; // Size: 0x100

class ULocalClickDragInputBehavior : public UClickDragInputBehavior
{
}; // Size: 0x210

class ULocalDoubleClickInputBehavior : public UDoubleClickInputBehavior
{
}; // Size: 0x1B0

class ULocalInputBehaviorSource : public UObject
{
}; // Size: 0x60

class ULocalMouseHoverBehavior : public UMouseHoverBehavior
{
}; // Size: 0x1C0

class ULocalSingleClickInputBehavior : public USingleClickInputBehavior
{
}; // Size: 0x1B0

class ULocalTwoAxisPropertyEditInputBehavior : public UTwoAxisPropertyEditInputBehavior
{
}; // Size: 0x250

class UMeshSelectionSet : public USelectionSet
{
    TArray<int32> Vertices;                                                           // 0x0040 (size: 0x10)
    TArray<int32> Edges;                                                              // 0x0050 (size: 0x10)
    TArray<int32> Faces;                                                              // 0x0060 (size: 0x10)
    TArray<int32> Groups;                                                             // 0x0070 (size: 0x10)

}; // Size: 0x80

class UMeshSurfacePointTool : public USingleSelectionTool
{
    TWeakObjectPtr<class UWorld> TargetWorld;                                         // 0x00F8 (size: 0x8)

}; // Size: 0x100

class UMeshSurfacePointToolBuilder : public UInteractiveToolWithToolTargetsBuilder
{
}; // Size: 0x30

class UMouseHoverBehavior : public UInputBehavior
{
}; // Size: 0xD0

class UMouseWheelInputBehavior : public UAnyButtonInputBehavior
{
}; // Size: 0x110

class UMultiClickSequenceInputBehavior : public UAnyButtonInputBehavior
{
}; // Size: 0x140

class UMultiSelectionTool : public UInteractiveTool
{
    TArray<UToolTarget*> Targets;                                                     // 0x00A0 (size: 0x10)

}; // Size: 0xB0

class UPlanePositionGizmo : public UInteractiveGizmo
{
    TScriptInterface<class IGizmoAxisSource> AxisSource;                              // 0x0048 (size: 0x10)
    TScriptInterface<class IGizmoVec2ParameterSource> ParameterSource;                // 0x0058 (size: 0x10)
    TScriptInterface<class IGizmoClickTarget> HitTarget;                              // 0x0068 (size: 0x10)
    TScriptInterface<class IGizmoStateTarget> StateTarget;                            // 0x0078 (size: 0x10)
    class UClickDragInputBehavior* MouseBehavior;                                     // 0x0088 (size: 0x8)
    bool bEnableSignedAxis;                                                           // 0x0090 (size: 0x1)
    bool bFlipX;                                                                      // 0x0091 (size: 0x1)
    bool bFlipY;                                                                      // 0x0092 (size: 0x1)
    bool bInInteraction;                                                              // 0x0100 (size: 0x1)
    FVector InteractionOrigin;                                                        // 0x0108 (size: 0x18)
    FVector InteractionNormal;                                                        // 0x0120 (size: 0x18)
    FVector InteractionAxisX;                                                         // 0x0138 (size: 0x18)
    FVector InteractionAxisY;                                                         // 0x0150 (size: 0x18)
    FVector InteractionStartPoint;                                                    // 0x0168 (size: 0x18)
    FVector InteractionCurPoint;                                                      // 0x0180 (size: 0x18)
    FVector2D InteractionStartParameter;                                              // 0x0198 (size: 0x10)
    FVector2D InteractionCurParameter;                                                // 0x01A8 (size: 0x10)
    FVector2D ParameterSigns;                                                         // 0x01B8 (size: 0x10)

}; // Size: 0x280

class UPlanePositionGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x28

class UPrimitiveComponentToolTarget : public UToolTarget
{
}; // Size: 0x38

class UPrimitiveComponentToolTargetFactory : public UToolTargetFactory
{
}; // Size: 0x28

class URepositionableTransformGizmo : public UCombinedTransformGizmo
{
    class UGizmoTransformChangeStateTarget* RepositionStateTarget;                    // 0x05B0 (size: 0x8)

}; // Size: 0x5C0

class URepositionableTransformGizmoBuilder : public UCombinedTransformGizmoBuilder
{
}; // Size: 0xD0

class UScalableSphereGizmo : public UInteractiveGizmo
{
    float HitErrorThreshold;                                                          // 0x0070 (size: 0x4)
    FText TransactionDescription;                                                     // 0x0078 (size: 0x10)
    float Radius;                                                                     // 0x0088 (size: 0x4)
    bool bIsHovering;                                                                 // 0x008C (size: 0x1)
    bool bIsDragging;                                                                 // 0x008D (size: 0x1)
    class UTransformProxy* ActiveTarget;                                              // 0x0090 (size: 0x8)
    FVector ActiveAxis;                                                               // 0x0098 (size: 0x18)
    FVector DragStartWorldPosition;                                                   // 0x00B0 (size: 0x18)
    FVector DragCurrentPositionProjected;                                             // 0x00C8 (size: 0x18)
    float InteractionStartParameter;                                                  // 0x00E0 (size: 0x4)

}; // Size: 0xF0

class UScalableSphereGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x28

class UScalableSphereGizmoInputBehavior : public UAnyButtonInputBehavior
{
}; // Size: 0xC0

class USceneComponentToolTarget : public UToolTarget
{
}; // Size: 0x38

class USceneComponentToolTargetFactory : public UToolTargetFactory
{
}; // Size: 0x28

class USceneSnappingManager : public UObject
{
}; // Size: 0x28

class USelectionSet : public UObject
{
}; // Size: 0x40

class USimpleLambdaInteractiveGizmoBuilder : public UInteractiveGizmoBuilder
{
}; // Size: 0x60

class USimpleSingleClickGizmo : public UInteractiveGizmo
{
    TScriptInterface<class IGizmoClickTarget> HitTarget;                              // 0x0060 (size: 0x10)
    class USingleClickInputBehavior* ClickBehavior;                                   // 0x0070 (size: 0x8)

}; // Size: 0x78

class USingleClickInputBehavior : public UAnyButtonInputBehavior
{
    bool HitTestOnRelease;                                                            // 0x00A0 (size: 0x1)

}; // Size: 0x110

class USingleClickOrDragInputBehavior : public UAnyButtonInputBehavior
{
    bool bBeginDragIfClickTargetNotHit;                                               // 0x0100 (size: 0x1)
    float ClickDistanceThreshold;                                                     // 0x0104 (size: 0x4)

}; // Size: 0x160

class USingleClickTool : public UInteractiveTool
{
}; // Size: 0xA0

class USingleClickToolBuilder : public UInteractiveToolBuilder
{
}; // Size: 0x28

class USingleKeyCaptureBehavior : public UInputBehavior
{
}; // Size: 0x100

class USingleSelectionTool : public UInteractiveTool
{
    class UToolTarget* Target;                                                        // 0x00A0 (size: 0x8)

}; // Size: 0xA8

class UToolTarget : public UObject
{
}; // Size: 0x28

class UToolTargetFactory : public UObject
{
}; // Size: 0x28

class UToolTargetManager : public UObject
{
    TArray<UToolTargetFactory*> Factories;                                            // 0x0030 (size: 0x10)

}; // Size: 0x40

class UToolsContextCursorAPI : public UObject
{
}; // Size: 0x30

class UTransformProxy : public UObject
{
    bool bRotatePerObject;                                                            // 0x00D0 (size: 0x1)
    bool bSetPivotMode;                                                               // 0x00D1 (size: 0x1)
    FTransform SharedTransform;                                                       // 0x00F0 (size: 0x60)
    FTransform InitialSharedTransform;                                                // 0x0150 (size: 0x60)

}; // Size: 0x1B0

class UTwoAxisPropertyEditInputBehavior : public UInputBehavior
{
}; // Size: 0x70

class UViewAdjustedStaticMeshGizmoComponent : public UStaticMeshComponent
{
    class UGizmoViewContext* GizmoViewContext;                                        // 0x0618 (size: 0x8)
    class UPrimitiveComponent* SubstituteInteractionComponent;                        // 0x0620 (size: 0x8)
    class UMaterialInterface* HoverOverrideMaterial;                                  // 0x0628 (size: 0x8)

}; // Size: 0x680

#endif
