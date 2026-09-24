#ifndef UE4SS_SDK_GameplayCameras_HPP
#define UE4SS_SDK_GameplayCameras_HPP

#include "GameplayCameras_enums.hpp"

struct FBaseCameraObjectReference
{
    FInstancedPropertyBag Parameters;                                                 // 0x0008 (size: 0x10)
    TArray<FCameraObjectInterfaceParameterMetaData> ParameterMetaData;                // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FBlueprintCameraDirectorActivateParams
{
    class UObject* EvaluationContextOwner;                                            // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FBlueprintCameraDirectorDeactivateParams
{
    class UObject* EvaluationContextOwner;                                            // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FBlueprintCameraDirectorEvaluationParams
{
    float DeltaTime;                                                                  // 0x0000 (size: 0x4)
    class UObject* EvaluationContextOwner;                                            // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FBlueprintCameraEvaluationDataRef
{
}; // Size: 0x18

struct FBlueprintCameraPose
{
    FVector Location;                                                                 // 0x0000 (size: 0x18)
    FRotator Rotation;                                                                // 0x0018 (size: 0x18)
    double TargetDistance;                                                            // 0x0030 (size: 0x8)
    float FieldOfView;                                                                // 0x0038 (size: 0x4)
    float FocalLength;                                                                // 0x003C (size: 0x4)
    float OrthographicWidth;                                                          // 0x0040 (size: 0x4)
    float Aperture;                                                                   // 0x0044 (size: 0x4)
    float ShutterSpeed;                                                               // 0x0048 (size: 0x4)
    float FocusDistance;                                                              // 0x004C (size: 0x4)
    float SensorWidth;                                                                // 0x0050 (size: 0x4)
    float SensorHeight;                                                               // 0x0054 (size: 0x4)
    float SensorHorizontalOffset;                                                     // 0x0058 (size: 0x4)
    float SensorVerticalOffset;                                                       // 0x005C (size: 0x4)
    float ISO;                                                                        // 0x0060 (size: 0x4)
    float SqueezeFactor;                                                              // 0x0064 (size: 0x4)
    float Overscan;                                                                   // 0x0068 (size: 0x4)
    int32 DiaphragmBladeCount;                                                        // 0x006C (size: 0x4)
    float NearClippingPlane;                                                          // 0x0070 (size: 0x4)
    float FarClippingPlane;                                                           // 0x0074 (size: 0x4)
    float PhysicalCameraBlendWeight;                                                  // 0x0078 (size: 0x4)
    bool EnablePhysicalCamera;                                                        // 0x007C (size: 0x1)
    bool ConstrainAspectRatio;                                                        // 0x007D (size: 0x1)
    bool OverrideAspectRatioAxisConstraint;                                           // 0x007E (size: 0x1)
    TEnumAsByte<EAspectRatioAxisConstraint> AspectRatioAxisConstraint;                // 0x007F (size: 0x1)
    TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;                          // 0x0080 (size: 0x1)

}; // Size: 0x88

struct FBooleanCameraParameter
{
    bool Value;                                                                       // 0x0000 (size: 0x1)
    FCameraVariableID VariableID;                                                     // 0x0004 (size: 0x4)
    class UBooleanCameraVariable* Variable;                                           // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FBooleanCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FBooleanCameraParameter Value;                                                    // 0x0038 (size: 0x10)

}; // Size: 0x48

struct FBooleanCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UBooleanCameraVariable* Variable;                                           // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FCameraActorAttachmentInfo
{
    class AActor* Actor;                                                              // 0x0000 (size: 0x8)
    FName SocketName;                                                                 // 0x0008 (size: 0x8)
    FName BoneName;                                                                   // 0x0010 (size: 0x8)
    float Weight;                                                                     // 0x0018 (size: 0x4)

}; // Size: 0x20

struct FCameraActorTargetInfo
{
    class AActor* Actor;                                                              // 0x0000 (size: 0x8)
    FName SocketName;                                                                 // 0x0008 (size: 0x8)
    FName BoneName;                                                                   // 0x0010 (size: 0x8)
    ECameraTargetShape TargetShape;                                                   // 0x0018 (size: 0x1)
    float TargetSize;                                                                 // 0x001C (size: 0x4)
    float Weight;                                                                     // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FCameraAssetAllocationInfo
{
    FCameraVariableTableAllocationInfo VariableTableInfo;                             // 0x0000 (size: 0x10)
    FCameraContextDataTableAllocationInfo ContextDataTableInfo;                       // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FCameraAssetReference
{
    class UCameraAsset* CameraAsset;                                                  // 0x0000 (size: 0x8)
    FInstancedPropertyBag Parameters;                                                 // 0x0008 (size: 0x10)
    TArray<FGuid> ParameterOverrideGuids;                                             // 0x0018 (size: 0x10)
    TArray<FGuid> ParameterAnimatedGuids;                                             // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FCameraContextDataDefinition
{
    FCameraContextDataID DataID;                                                      // 0x0000 (size: 0x4)
    ECameraContextDataType DataType;                                                  // 0x0004 (size: 0x4)
    ECameraContextDataContainerType DataContainerType;                                // 0x0008 (size: 0x4)
    class UObject* DataTypeObject;                                                    // 0x0010 (size: 0x8)
    bool bAutoReset;                                                                  // 0x0018 (size: 0x1)

}; // Size: 0x20

struct FCameraContextDataID
{
    uint32 Value;                                                                     // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FCameraContextDataTableAllocationInfo
{
    TArray<FCameraContextDataDefinition> DataDefinitions;                             // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FCameraDirectorStateTreeEvaluationData
{
    TArray<UCameraRigAsset*> ActiveCameraRigs;                                        // 0x0000 (size: 0x10)
    TArray<UCameraRigProxyAsset*> ActiveCameraRigProxies;                             // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FCameraFramingZone
{
    double Left;                                                                      // 0x0000 (size: 0x8)
    double Top;                                                                       // 0x0008 (size: 0x8)
    double Right;                                                                     // 0x0010 (size: 0x8)
    double Bottom;                                                                    // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FCameraFramingZoneParameter
{
    FCameraFramingZone Value;                                                         // 0x0000 (size: 0x20)
    FCameraVariableID VariableID;                                                     // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FCameraNodeEvaluatorAllocationInfo
{
    int16 TotalSizeof;                                                                // 0x0000 (size: 0x2)
    int16 MaxAlignof;                                                                 // 0x0002 (size: 0x2)

}; // Size: 0x4

struct FCameraObjectAllocationInfo
{
    FCameraNodeEvaluatorAllocationInfo EvaluatorInfo;                                 // 0x0000 (size: 0x4)
    FCameraVariableTableAllocationInfo VariableTableInfo;                             // 0x0008 (size: 0x10)
    FCameraContextDataTableAllocationInfo ContextDataTableInfo;                       // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FCameraObjectInterface
{
    TArray<UCameraObjectInterfaceBlendableParameter*> BlendableParameters;            // 0x0000 (size: 0x10)
    TArray<UCameraObjectInterfaceDataParameter*> DataParameters;                      // 0x0010 (size: 0x10)
    FString DisplayName;                                                              // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FCameraObjectInterfaceParameterDefinition
{
    FName ParameterName;                                                              // 0x0000 (size: 0x8)
    FGuid ParameterGuid;                                                              // 0x0008 (size: 0x10)
    ECameraObjectInterfaceParameterType ParameterType;                                // 0x0018 (size: 0x1)
    FCameraVariableID VariableID;                                                     // 0x001C (size: 0x4)
    ECameraVariableType VariableType;                                                 // 0x0020 (size: 0x4)
    class UScriptStruct* BlendableStructType;                                         // 0x0028 (size: 0x8)
    FCameraContextDataID DataID;                                                      // 0x0030 (size: 0x4)
    ECameraContextDataType DataType;                                                  // 0x0034 (size: 0x4)
    ECameraContextDataContainerType DataContainerType;                                // 0x0038 (size: 0x4)
    class UObject* DataTypeObject;                                                    // 0x0040 (size: 0x8)

}; // Size: 0x48

struct FCameraObjectInterfaceParameterMetaData
{
    FGuid ParameterGuid;                                                              // 0x0000 (size: 0x10)
    FCameraVariableID OverrideVariableID;                                             // 0x0010 (size: 0x4)
    FCameraContextDataID OverrideDataID;                                              // 0x0014 (size: 0x4)
    bool bIsOverridden;                                                               // 0x0018 (size: 0x1)
    bool bIsAnimated;                                                                 // 0x0019 (size: 0x1)

}; // Size: 0x1C

struct FCameraParameterClamping
{
    double MinValue;                                                                  // 0x0000 (size: 0x8)
    double MaxValue;                                                                  // 0x0008 (size: 0x8)
    bool bClampMin;                                                                   // 0x0010 (size: 0x1)
    bool bClampMax;                                                                   // 0x0011 (size: 0x1)

}; // Size: 0x18

struct FCameraParameterNormalization
{
    double MaxValue;                                                                  // 0x0000 (size: 0x8)
    bool bNormalize;                                                                  // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FCameraPose
{
    FVector3d Location;                                                               // 0x0000 (size: 0x18)
    FRotator3d Rotation;                                                              // 0x0018 (size: 0x18)
    double TargetDistance;                                                            // 0x0030 (size: 0x8)
    float FieldOfView;                                                                // 0x0038 (size: 0x4)
    float FocalLength;                                                                // 0x003C (size: 0x4)
    float OrthographicWidth;                                                          // 0x0040 (size: 0x4)
    float Aperture;                                                                   // 0x0044 (size: 0x4)
    float ShutterSpeed;                                                               // 0x0048 (size: 0x4)
    float FocusDistance;                                                              // 0x004C (size: 0x4)
    float SensorWidth;                                                                // 0x0050 (size: 0x4)
    float SensorHeight;                                                               // 0x0054 (size: 0x4)
    float SensorHorizontalOffset;                                                     // 0x0058 (size: 0x4)
    float SensorVerticalOffset;                                                       // 0x005C (size: 0x4)
    float ISO;                                                                        // 0x0060 (size: 0x4)
    float SqueezeFactor;                                                              // 0x0064 (size: 0x4)
    float Overscan;                                                                   // 0x0068 (size: 0x4)
    int32 DiaphragmBladeCount;                                                        // 0x006C (size: 0x4)
    float NearClippingPlane;                                                          // 0x0070 (size: 0x4)
    float FarClippingPlane;                                                           // 0x0074 (size: 0x4)
    float PhysicalCameraBlendWeight;                                                  // 0x0078 (size: 0x4)
    bool EnablePhysicalCamera;                                                        // 0x007C (size: 0x1)
    bool ConstrainAspectRatio;                                                        // 0x007D (size: 0x1)
    bool OverrideAspectRatioAxisConstraint;                                           // 0x007E (size: 0x1)
    TEnumAsByte<EAspectRatioAxisConstraint> AspectRatioAxisConstraint;                // 0x007F (size: 0x1)
    TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;                          // 0x0080 (size: 0x1)

}; // Size: 0x88

struct FCameraRigAssetReference : public FBaseCameraObjectReference
{
    class UCameraRigAsset* CameraRig;                                                 // 0x0028 (size: 0x8)
    TArray<FGuid> ParameterOverrideGuids;                                             // 0x0030 (size: 0x10)
    FCameraRigParameterOverrides ParameterOverrides;                                  // 0x0040 (size: 0xE0)

}; // Size: 0x120

struct FCameraRigInputSlotParameters
{
    bool bIsAccumulated;                                                              // 0x0000 (size: 0x1)
    bool bIsPreBlended;                                                               // 0x0001 (size: 0x1)

}; // Size: 0x2

struct FCameraRigInstanceID
{
    uint32 Value;                                                                     // 0x0000 (size: 0x4)
    ECameraRigLayer Layer;                                                            // 0x0004 (size: 0x1)

}; // Size: 0x8

struct FCameraRigParameterOverrideBase
{
    FGuid InterfaceParameterGuid;                                                     // 0x0000 (size: 0x10)
    FGuid PrivateVariableGuid;                                                        // 0x0010 (size: 0x10)
    FString InterfaceParameterName;                                                   // 0x0020 (size: 0x10)
    bool bInvalid;                                                                    // 0x0030 (size: 0x1)

}; // Size: 0x38

struct FCameraRigParameterOverrides
{
    TArray<FBooleanCameraRigParameterOverride> BooleanOverrides;                      // 0x0000 (size: 0x10)
    TArray<FInteger32CameraRigParameterOverride> Integer32Overrides;                  // 0x0010 (size: 0x10)
    TArray<FFloatCameraRigParameterOverride> FloatOverrides;                          // 0x0020 (size: 0x10)
    TArray<FDoubleCameraRigParameterOverride> DoubleOverrides;                        // 0x0030 (size: 0x10)
    TArray<FVector2fCameraRigParameterOverride> Vector2fOverrides;                    // 0x0040 (size: 0x10)
    TArray<FVector2dCameraRigParameterOverride> Vector2dOverrides;                    // 0x0050 (size: 0x10)
    TArray<FVector3fCameraRigParameterOverride> Vector3fOverrides;                    // 0x0060 (size: 0x10)
    TArray<FVector3dCameraRigParameterOverride> Vector3dOverrides;                    // 0x0070 (size: 0x10)
    TArray<FVector4fCameraRigParameterOverride> Vector4fOverrides;                    // 0x0080 (size: 0x10)
    TArray<FVector4dCameraRigParameterOverride> Vector4dOverrides;                    // 0x0090 (size: 0x10)
    TArray<FRotator3fCameraRigParameterOverride> Rotator3fOverrides;                  // 0x00A0 (size: 0x10)
    TArray<FRotator3dCameraRigParameterOverride> Rotator3dOverrides;                  // 0x00B0 (size: 0x10)
    TArray<FTransform3fCameraRigParameterOverride> Transform3fOverrides;              // 0x00C0 (size: 0x10)
    TArray<FTransform3dCameraRigParameterOverride> Transform3dOverrides;              // 0x00D0 (size: 0x10)

}; // Size: 0xE0

struct FCameraRigProxyRedirectTable
{
    TArray<FCameraRigProxyRedirectTableEntry> Entries;                                // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FCameraRigProxyRedirectTableEntry
{
    class UCameraRigProxyAsset* CameraRigProxy;                                       // 0x0000 (size: 0x8)
    class UCameraRigAsset* CameraRig;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FCameraRotatorCurve
{
    FRichCurve Curves;                                                                // 0x0000 (size: 0x180)

}; // Size: 0x180

struct FCameraShakeAssetReference : public FBaseCameraObjectReference
{
    class UCameraShakeAsset* CameraShake;                                             // 0x0028 (size: 0x8)

}; // Size: 0x30

struct FCameraSingleCurve
{
    FRichCurve Curve;                                                                 // 0x0000 (size: 0x80)

}; // Size: 0x80

struct FCameraVariableDefinition
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    ECameraVariableType VariableType;                                                 // 0x0004 (size: 0x4)
    class UScriptStruct* BlendableStructType;                                         // 0x0008 (size: 0x8)
    bool bIsPrivate;                                                                  // 0x0010 (size: 0x1)
    bool bIsInput;                                                                    // 0x0011 (size: 0x1)
    bool bAutoReset;                                                                  // 0x0012 (size: 0x1)

}; // Size: 0x18

struct FCameraVariableID
{
    uint32 Value;                                                                     // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FCameraVariableSetterHandle
{
    uint32 Value;                                                                     // 0x0000 (size: 0x4)
    uint32 SerialNumber;                                                              // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FCameraVariableTableAllocationInfo
{
    TArray<FCameraVariableDefinition> VariableDefinitions;                            // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FCameraVectorCurve
{
    FRichCurve Curves;                                                                // 0x0000 (size: 0x180)

}; // Size: 0x180

struct FCustomCameraNodeBlendableParameter
{
    FName ParameterName;                                                              // 0x0000 (size: 0x8)
    ECameraVariableType ParameterType;                                                // 0x0008 (size: 0x4)
    class UScriptStruct* BlendableStructType;                                         // 0x0010 (size: 0x8)
    FCameraVariableID OverrideVariableID;                                             // 0x0018 (size: 0x4)
    class UCameraVariableAsset* OverrideVariable;                                     // 0x0020 (size: 0x8)

}; // Size: 0x28

struct FCustomCameraNodeDataParameter
{
    FName ParameterName;                                                              // 0x0000 (size: 0x8)
    ECameraContextDataType ParameterType;                                             // 0x0008 (size: 0x4)
    ECameraContextDataContainerType ParameterContainerType;                           // 0x000C (size: 0x4)
    class UObject* ParameterTypeObject;                                               // 0x0010 (size: 0x8)
    FCameraContextDataID OverrideDataID;                                              // 0x0018 (size: 0x4)

}; // Size: 0x20

struct FCustomCameraNodeParameters
{
    TArray<FCustomCameraNodeBlendableParameter> BlendableParameters;                  // 0x0000 (size: 0x10)
    TArray<FCustomCameraNodeDataParameter> DataParameters;                            // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FDoubleCameraParameter
{
    double Value;                                                                     // 0x0000 (size: 0x8)
    FCameraVariableID VariableID;                                                     // 0x0008 (size: 0x4)
    class UDoubleCameraVariable* Variable;                                            // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FDoubleCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FDoubleCameraParameter Value;                                                     // 0x0038 (size: 0x18)

}; // Size: 0x50

struct FDoubleCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UDoubleCameraVariable* Variable;                                            // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FFloatCameraParameter
{
    float Value;                                                                      // 0x0000 (size: 0x4)
    FCameraVariableID VariableID;                                                     // 0x0004 (size: 0x4)
    class UFloatCameraVariable* Variable;                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FFloatCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FFloatCameraParameter Value;                                                      // 0x0038 (size: 0x10)

}; // Size: 0x48

struct FFloatCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UFloatCameraVariable* Variable;                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGameplayCamerasActivateCameraRigTask : public FGameplayCamerasStateTreeTask
{
    bool bRunOnce;                                                                    // 0x002E (size: 0x1)

}; // Size: 0x30

struct FGameplayCamerasActivateCameraRigTaskInstanceData
{
    class UCameraRigAsset* CameraRig;                                                 // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FGameplayCamerasActivateCameraRigViaProxyTask : public FGameplayCamerasStateTreeTask
{
    bool bRunOnce;                                                                    // 0x002E (size: 0x1)

}; // Size: 0x30

struct FGameplayCamerasActivateCameraRigViaProxyTaskInstanceData
{
    class UCameraRigProxyAsset* CameraRigProxy;                                       // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FGameplayCamerasStateTreeCondition : public FStateTreeConditionBase
{
}; // Size: 0x28

struct FGameplayCamerasStateTreeTask : public FStateTreeTaskBase
{
}; // Size: 0x28

struct FInteger32CameraParameter
{
    int32 Value;                                                                      // 0x0000 (size: 0x4)
    FCameraVariableID VariableID;                                                     // 0x0004 (size: 0x4)
    class UInteger32CameraVariable* Variable;                                         // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FInteger32CameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FInteger32CameraParameter Value;                                                  // 0x0038 (size: 0x10)

}; // Size: 0x48

struct FInteger32CameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UInteger32CameraVariable* Variable;                                         // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FPerlinNoiseData
{
    float Amplitude;                                                                  // 0x0000 (size: 0x4)
    float Frequency;                                                                  // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FRotator3dCameraParameter
{
    FRotator Value;                                                                   // 0x0000 (size: 0x18)
    FCameraVariableID VariableID;                                                     // 0x0018 (size: 0x4)
    class URotator3dCameraVariable* Variable;                                         // 0x0020 (size: 0x8)

}; // Size: 0x28

struct FRotator3dCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FRotator3dCameraParameter Value;                                                  // 0x0038 (size: 0x28)

}; // Size: 0x60

struct FRotator3dCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class URotator3dCameraVariable* Variable;                                         // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FRotator3fCameraParameter
{
    FRotator3f Value;                                                                 // 0x0000 (size: 0xC)
    FCameraVariableID VariableID;                                                     // 0x000C (size: 0x4)
    class URotator3fCameraVariable* Variable;                                         // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FRotator3fCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FRotator3fCameraParameter Value;                                                  // 0x0038 (size: 0x18)

}; // Size: 0x50

struct FRotator3fCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class URotator3fCameraVariable* Variable;                                         // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FSplineOrbitControlPoint
{
    FVector3d LocationOffset;                                                         // 0x0000 (size: 0x18)
    FVector3d TargetOffset;                                                           // 0x0018 (size: 0x18)
    FRotator3d RotationOffset;                                                        // 0x0030 (size: 0x18)
    float PitchAngle;                                                                 // 0x0048 (size: 0x4)

}; // Size: 0x50

struct FTransform3dCameraParameter
{
    FTransform Value;                                                                 // 0x0000 (size: 0x60)
    FCameraVariableID VariableID;                                                     // 0x0060 (size: 0x4)
    class UTransform3dCameraVariable* Variable;                                       // 0x0068 (size: 0x8)

}; // Size: 0x70

struct FTransform3dCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FTransform3dCameraParameter Value;                                                // 0x0040 (size: 0x70)

}; // Size: 0xB0

struct FTransform3dCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UTransform3dCameraVariable* Variable;                                       // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FTransform3fCameraParameter
{
    FTransform3f Value;                                                               // 0x0000 (size: 0x30)
    FCameraVariableID VariableID;                                                     // 0x0030 (size: 0x4)
    class UTransform3fCameraVariable* Variable;                                       // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FTransform3fCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FTransform3fCameraParameter Value;                                                // 0x0040 (size: 0x40)

}; // Size: 0x80

struct FTransform3fCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UTransform3fCameraVariable* Variable;                                       // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector2dCameraParameter
{
    FVector2D Value;                                                                  // 0x0000 (size: 0x10)
    FCameraVariableID VariableID;                                                     // 0x0010 (size: 0x4)
    class UVector2dCameraVariable* Variable;                                          // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FVector2dCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector2dCameraParameter Value;                                                   // 0x0038 (size: 0x20)

}; // Size: 0x58

struct FVector2dCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector2dCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector2fCameraParameter
{
    FVector2f Value;                                                                  // 0x0000 (size: 0x8)
    FCameraVariableID VariableID;                                                     // 0x0008 (size: 0x4)
    class UVector2fCameraVariable* Variable;                                          // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FVector2fCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector2fCameraParameter Value;                                                   // 0x0038 (size: 0x18)

}; // Size: 0x50

struct FVector2fCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector2fCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector3dCameraParameter
{
    FVector Value;                                                                    // 0x0000 (size: 0x18)
    FCameraVariableID VariableID;                                                     // 0x0018 (size: 0x4)
    class UVector3dCameraVariable* Variable;                                          // 0x0020 (size: 0x8)

}; // Size: 0x28

struct FVector3dCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector3dCameraParameter Value;                                                   // 0x0038 (size: 0x28)

}; // Size: 0x60

struct FVector3dCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector3dCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector3fCameraParameter
{
    FVector3f Value;                                                                  // 0x0000 (size: 0xC)
    FCameraVariableID VariableID;                                                     // 0x000C (size: 0x4)
    class UVector3fCameraVariable* Variable;                                          // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FVector3fCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector3fCameraParameter Value;                                                   // 0x0038 (size: 0x18)

}; // Size: 0x50

struct FVector3fCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector3fCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector4dCameraParameter
{
    FVector4 Value;                                                                   // 0x0000 (size: 0x20)
    FCameraVariableID VariableID;                                                     // 0x0020 (size: 0x4)
    class UVector4dCameraVariable* Variable;                                          // 0x0028 (size: 0x8)

}; // Size: 0x30

struct FVector4dCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector4dCameraParameter Value;                                                   // 0x0040 (size: 0x30)

}; // Size: 0x70

struct FVector4dCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector4dCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FVector4fCameraParameter
{
    FVector4f Value;                                                                  // 0x0000 (size: 0x10)
    FCameraVariableID VariableID;                                                     // 0x0010 (size: 0x4)
    class UVector4fCameraVariable* Variable;                                          // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FVector4fCameraRigParameterOverride : public FCameraRigParameterOverrideBase
{
    FVector4fCameraParameter Value;                                                   // 0x0040 (size: 0x20)

}; // Size: 0x60

struct FVector4fCameraVariableReference
{
    FCameraVariableID VariableID;                                                     // 0x0000 (size: 0x4)
    class UVector4fCameraVariable* Variable;                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

class AGameplayCameraActor : public AGameplayCameraActorBase
{
    class UGameplayCameraComponent* CameraComponent;                                  // 0x02A8 (size: 0x8)

    class UGameplayCameraComponent* GetCameraComponent();
}; // Size: 0x2B0

class AGameplayCameraActorBase : public AActor
{
}; // Size: 0x2A8

class AGameplayCameraRigActor : public AGameplayCameraActorBase
{
    class UGameplayCameraRigComponent* CameraRigComponent;                            // 0x02A8 (size: 0x8)

    class UGameplayCameraRigComponent* GetCameraRigComponent();
}; // Size: 0x2B0

class AGameplayCameraSystemActor : public AActor
{
    class UGameplayCameraSystemComponent* CameraSystemComponent;                      // 0x02A8 (size: 0x8)

    class UGameplayCameraSystemComponent* GetCameraSystemComponent();
}; // Size: 0x2B0

class AGameplayCamerasPlayerCameraManager : public APlayerCameraManager
{
    EGameplayCamerasViewRotationMode ViewRotationMode;                                // 0x27F0 (size: 0x4)
    bool bOverrideViewRotationMode;                                                   // 0x27F4 (size: 0x1)
    class APlayerCameraManager* OriginalCameraManager;                                // 0x27F8 (size: 0x8)

    void StopCameraModifierRig(FCameraRigInstanceID InstanceId, bool bImmediately);
    void StealPlayerController(class APlayerController* PlayerController);
    FCameraRigInstanceID StartVisualCameraModifierRig(const class UCameraRigAsset* CameraRig, int32 OrderKey);
    FCameraRigInstanceID StartGlobalCameraModifierRig(const class UCameraRigAsset* CameraRig, int32 OrderKey);
    void ReleasePlayerController();
}; // Size: 0x30E0

class IAssetReferenceCameraNode : public IInterface
{
}; // Size: 0x28

class ICustomCameraNodeParameterProvider : public IInterface
{
}; // Size: 0x28

class IGameplayCameraSystemHost : public IInterface
{
}; // Size: 0x28

class IHasCameraBuildStatus : public IInterface
{
}; // Size: 0x28

class IObjectTreeGraphObject : public IInterface
{
}; // Size: 0x28

class IObjectTreeGraphRootObject : public IInterface
{
}; // Size: 0x28

class UAccelerationDecelerationValueInterpolator : public UCameraValueInterpolator
{
    float Acceleration;                                                               // 0x0028 (size: 0x4)
    float MaxSpeed;                                                                   // 0x002C (size: 0x4)
    float Deceleration;                                                               // 0x0030 (size: 0x4)

}; // Size: 0x38

class UActivateCameraRigFunctions : public UBlueprintFunctionLibrary
{

    void ActivatePersistentVisualCameraRig(class UObject* WorldContextObject, class APlayerController* PlayerController, class UCameraRigAsset* CameraRig);
    void ActivatePersistentGlobalCameraRig(class UObject* WorldContextObject, class APlayerController* PlayerController, class UCameraRigAsset* CameraRig);
    void ActivatePersistentBaseCameraRig(class UObject* WorldContextObject, class APlayerController* PlayerController, class UCameraRigAsset* CameraRig);
}; // Size: 0x28

class UArrayCameraNode : public UCameraNode
{
    TArray<UCameraNode*> Children;                                                    // 0x0038 (size: 0x10)

}; // Size: 0x48

class UAttachToActorCameraNode : public UCameraNode
{
    FCameraActorAttachmentInfo Attachment;                                            // 0x0038 (size: 0x20)
    FCameraContextDataID AttachmentDataID;                                            // 0x0058 (size: 0x4)
    FBooleanCameraParameter AttachToLocation;                                         // 0x0060 (size: 0x10)
    FBooleanCameraParameter AttachToRotation;                                         // 0x0070 (size: 0x10)

}; // Size: 0x80

class UAttachToActorGroupCameraNode : public UCameraNode
{
    TArray<FCameraActorAttachmentInfo> Attachments;                                   // 0x0038 (size: 0x10)
    FCameraContextDataID AttachmentsDataID;                                           // 0x0048 (size: 0x4)

}; // Size: 0x50

class UAttachToPlayerPawnCameraNode : public UCameraNode
{
    FBooleanCameraParameter AttachToLocation;                                         // 0x0038 (size: 0x10)
    FBooleanCameraParameter AttachToRotation;                                         // 0x0048 (size: 0x10)

}; // Size: 0x58

class UAutoFocusCameraNode : public UCameraNode
{
    FBooleanCameraVariableReference EnableAutoFocus;                                  // 0x0038 (size: 0x10)
    FFloatCameraParameter AutoFocusDampingFactor;                                     // 0x0048 (size: 0x10)

}; // Size: 0x58

class UAutoRotateInput2DCameraNode : public UInput2DCameraNode
{
    ECameraAutoRotateDirection Direction;                                             // 0x0038 (size: 0x4)
    FVector3dCameraVariableReference DirectionVector;                                 // 0x0040 (size: 0x10)
    FFloatCameraParameter WaitTime;                                                   // 0x0050 (size: 0x10)
    FFloatCameraParameter DeactivationThreshold;                                      // 0x0060 (size: 0x10)
    class UCameraValueInterpolator* Interpolator;                                     // 0x0070 (size: 0x8)
    FBooleanCameraParameter FreezeControlRotation;                                    // 0x0078 (size: 0x10)
    FBooleanCameraParameter EnableAutoRotate;                                         // 0x0088 (size: 0x10)
    FBooleanCameraParameter AutoRotateYaw;                                            // 0x0098 (size: 0x10)
    FBooleanCameraParameter AutoRotatePitch;                                          // 0x00A8 (size: 0x10)
    class UInput2DCameraNode* InputNode;                                              // 0x00B8 (size: 0x8)

}; // Size: 0xC0

class UBaseCameraObject : public UObject
{
    FCameraObjectInterface Interface;                                                 // 0x0028 (size: 0x30)
    FCameraObjectAllocationInfo AllocationInfo;                                       // 0x0060 (size: 0x28)
    FInstancedPropertyBag DefaultParameters;                                          // 0x0088 (size: 0x10)
    TArray<FCameraObjectInterfaceParameterDefinition> ParameterDefinitions;           // 0x0098 (size: 0x10)

}; // Size: 0xA8

class UBaseFramingCameraNode : public UCameraNode
{
    FVector3dCameraVariableReference TargetLocation;                                  // 0x0040 (size: 0x10)
    TArray<FCameraActorTargetInfo> TargetInfos;                                       // 0x0050 (size: 0x10)
    FCameraContextDataID TargetInfosDataID;                                           // 0x0060 (size: 0x4)
    FBooleanCameraParameter SetTargetDistance;                                        // 0x0068 (size: 0x10)
    FBooleanCameraParameter InitializeWithIdealFraming;                               // 0x0078 (size: 0x10)
    FVector2dCameraParameter IdealFramingLocation;                                    // 0x0088 (size: 0x20)
    FFloatCameraParameter ReframeDampingFactor;                                       // 0x00A8 (size: 0x10)
    FFloatCameraParameter LowReframeDampingFactor;                                    // 0x00B8 (size: 0x10)
    FFloatCameraParameter ReengageTime;                                               // 0x00C8 (size: 0x10)
    FFloatCameraParameter DisengageTime;                                              // 0x00D8 (size: 0x10)
    FFloatCameraParameter TargetMovementAnticipationTime;                             // 0x00E8 (size: 0x10)
    FCameraFramingZoneParameter DeadZone;                                             // 0x00F8 (size: 0x28)
    FCameraFramingZoneParameter SoftZone;                                             // 0x0120 (size: 0x28)
    FCameraActorTargetInfo TargetInfo;                                                // 0x0148 (size: 0x28)

}; // Size: 0x170

class UBlendCameraNode : public UCameraNode
{
}; // Size: 0x38

class UBlendStackCameraNode : public UCameraNode
{
    ECameraBlendStackType BlendStackType;                                             // 0x0038 (size: 0x4)
    ECameraRigLayer Layer;                                                            // 0x003C (size: 0x1)

}; // Size: 0x40

class UBlendStackRootCameraNode : public UCameraNode
{
    class UBlendCameraNode* Blend;                                                    // 0x0038 (size: 0x8)
    class UCameraNode* RootNode;                                                      // 0x0040 (size: 0x8)

}; // Size: 0x48

class UBlueprintCameraContextDataTableFunctionLibrary : public UBlueprintFunctionLibrary
{

    bool SetStructData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, const FInstancedStruct& Data);
    bool SetStringData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, FString Data);
    bool SetObjectData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, class UObject* Data);
    bool SetNameData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, const FName& Data);
    bool SetEnumData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, const class UEnum* EnumType, uint8 Data);
    bool SetClassData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, UClass* Data);
    FInstancedStruct GetStructData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, const class UScriptStruct* DataStructType);
    FString GetStringData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID);
    class UObject* GetObjectData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID);
    FName GetNameData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID);
    uint8 GetEnumData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID, const class UEnum* EnumType);
    UClass* GetClassData(const FBlueprintCameraEvaluationDataRef& CameraData, FCameraContextDataID DataID);
}; // Size: 0x28

class UBlueprintCameraDirector : public UCameraDirector
{
    TSubclassOf<class UBlueprintCameraDirectorEvaluator> CameraDirectorEvaluatorClass; // 0x0040 (size: 0x8)

}; // Size: 0x48

class UBlueprintCameraDirectorEvaluator : public UObject
{

    bool RunChildCameraDirector(float DeltaTime, FName ChildSlotName);
    void RunCameraDirector(float DeltaTime, class UObject* EvaluationContextOwner, const FBlueprintCameraDirectorEvaluationParams& Params);
    bool RemoveChildEvaluationContext(class UObject* ChildEvaluationContextOwner, FName ChildSlotName);
    FBlueprintCameraEvaluationDataRef GetInitialContextResult();
    FBlueprintCameraEvaluationDataRef GetConditionalContextResult(ECameraEvaluationDataCondition Condition);
    class AActor* FindEvaluationContextOwnerActor(TSubclassOf<class AActor> ActorClass);
    void DeactivatePersistentVisualCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void DeactivatePersistentGlobalCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void DeactivatePersistentBaseCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void DeactivateCameraDirector(class UObject* EvaluationContextOwner, const FBlueprintCameraDirectorDeactivateParams& Params);
    FName AddChildEvaluationContext(class UObject* ChildEvaluationContextOwner);
    void ActivatePersistentVisualCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void ActivatePersistentGlobalCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void ActivatePersistentBaseCameraRig(class UCameraRigAsset* CameraRigPrefab);
    void ActivateCameraRigViaProxy(class UCameraRigProxyAsset* CameraRigProxy, bool bForceNewInstance);
    void ActivateCameraRig(class UCameraRigAsset* CameraRig, bool bForceNewInstance);
    void ActivateCameraDirector(class UObject* EvaluationContextOwner, const FBlueprintCameraDirectorActivateParams& Params);
}; // Size: 0x140

class UBlueprintCameraEvaluationDataFunctionLibrary : public UBlueprintFunctionLibrary
{

    void SetDefaultCameraRigParameters(const FBlueprintCameraEvaluationDataRef& CameraData, const class UCameraRigAsset* CameraRig);
    void SetCameraPose(const FBlueprintCameraEvaluationDataRef& CameraData, const FBlueprintCameraPose& CameraPose);
    FBlueprintCameraEvaluationDataRef MakeCameraEvaluationData();
    FBlueprintCameraPose GetCameraPose(const FBlueprintCameraEvaluationDataRef& CameraData);
    void BlendCameraEvaluationData(const FBlueprintCameraEvaluationDataRef& FromCameraData, const FBlueprintCameraEvaluationDataRef& ToCameraData, float Factor);
}; // Size: 0x28

class UBlueprintCameraNode : public UCameraNode
{
    class UBlueprintCameraNodeEvaluator* CameraNodeEvaluatorTemplate;                 // 0x0040 (size: 0x8)
    FCustomCameraNodeParameters CameraNodeEvaluatorOverrides;                         // 0x0048 (size: 0x20)
    TSubclassOf<class UBlueprintCameraNodeEvaluator> CameraNodeEvaluatorClass;        // 0x0068 (size: 0x8)

}; // Size: 0x70

class UBlueprintCameraNodeEvaluator : public UObject
{
    bool bIsFirstFrame;                                                               // 0x0028 (size: 0x1)
    class UObject* EvaluationContextOwner;                                            // 0x0030 (size: 0x8)
    FBlueprintCameraEvaluationDataRef CameraData;                                     // 0x0038 (size: 0x18)
    FBlueprintCameraPose CameraPose;                                                  // 0x0050 (size: 0x88)
    FBlueprintCameraEvaluationDataRef VariableTable;                                  // 0x00D8 (size: 0x18)

    void TickCameraNode(float DeltaTime);
    void SetDefaultOwningCameraRigParameters(FBlueprintCameraEvaluationDataRef TargetCameraData);
    void SetCurrentCameraPose(const FBlueprintCameraPose& CameraPose);
    void SetCameraPose(const FBlueprintCameraPose& InCameraPose);
    void InitializeCameraNode();
    class APlayerController* GetPlayerController();
    FBlueprintCameraPose GetCurrentCameraPose();
    FBlueprintCameraPose GetCameraPose();
    class AActor* FindEvaluationContextOwnerActor(TSubclassOf<class AActor> ActorClass);
}; // Size: 0x110

class UBlueprintCameraPoseFunctionLibrary : public UBlueprintFunctionLibrary
{

    FBlueprintCameraPose SetTransform(const FBlueprintCameraPose& CameraPose, const FTransform& Transform);
    FBlueprintCameraPose SetTargetDistance(const FBlueprintCameraPose& CameraPose, double TargetDistance);
    FBlueprintCameraPose SetRotation(const FBlueprintCameraPose& CameraPose, const FRotator& Rotation);
    FBlueprintCameraPose SetLocation(const FBlueprintCameraPose& CameraPose, const FVector& Location);
    FBlueprintCameraPose SetFocalLength(const FBlueprintCameraPose& CameraPose, float FocalLength);
    FBlueprintCameraPose SetFieldOfView(const FBlueprintCameraPose& CameraPose, float FieldOfView);
    FBlueprintCameraPose MakeCameraPoseFromCineCameraComponent(const class UCineCameraComponent* CameraComponent);
    FBlueprintCameraPose MakeCameraPoseFromCameraComponent(const class UCameraComponent* CameraComponent);
    FTransform GetTransform(const FBlueprintCameraPose& CameraPose);
    double GetTargetDistance(const FBlueprintCameraPose& CameraPose);
    FVector GetTargetAtDistance(const FBlueprintCameraPose& CameraPose, double TargetDistance);
    FVector GetTarget(const FBlueprintCameraPose& CameraPose);
    double GetSensorAspectRatio(const FBlueprintCameraPose& CameraPose);
    FRotator GetRotation(const FBlueprintCameraPose& CameraPose);
    FVector GetLocation(const FBlueprintCameraPose& CameraPose);
    double GetFocalLength(const FBlueprintCameraPose& CameraPose);
    double GetFieldOfView(const FBlueprintCameraPose& CameraPose);
    double GetEffectiveFieldOfView(const FBlueprintCameraPose& CameraPose);
    FRay GetAimRay(const FBlueprintCameraPose& CameraPose);
    FVector GetAimDir(const FBlueprintCameraPose& CameraPose);
}; // Size: 0x28

class UBlueprintCameraVariableTableFunctionLibrary : public UBlueprintFunctionLibrary
{

    void SetVector4CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector4dCameraVariable* Variable, const FVector4& Value);
    void SetVector3CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector3dCameraVariable* Variable, const FVector& Value);
    void SetVector2CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector2dCameraVariable* Variable, const FVector2D& Value);
    void SetTransformCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UTransform3dCameraVariable* Variable, const FTransform& Value);
    void SetRotatorCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class URotator3dCameraVariable* Variable, const FRotator& Value);
    void SetInteger32CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UInteger32CameraVariable* Variable, int32 Value);
    void SetFloatCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UFloatCameraVariable* Variable, float Value);
    void SetDoubleCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UDoubleCameraVariable* Variable, double Value);
    void SetBooleanCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UBooleanCameraVariable* Variable, bool Value);
    FVector4 GetVector4CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector4dCameraVariable* Variable);
    FVector GetVector3CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector3dCameraVariable* Variable);
    FVector2D GetVector2CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UVector2dCameraVariable* Variable);
    FTransform GetTransformCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UTransform3dCameraVariable* Variable);
    FRotator GetRotatorCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class URotator3dCameraVariable* Variable);
    int32 GetInteger32CameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UInteger32CameraVariable* Variable);
    float GetFloatCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UFloatCameraVariable* Variable);
    double GetDoubleCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UDoubleCameraVariable* Variable);
    bool GetBooleanCameraVariable(const FBlueprintCameraEvaluationDataRef& CameraData, class UBooleanCameraVariable* Variable);
}; // Size: 0x28

class UBodyParametersCameraNode : public UCameraNode
{
    FFloatCameraParameter ShutterSpeed;                                               // 0x0038 (size: 0x10)
    FFloatCameraParameter ISO;                                                        // 0x0048 (size: 0x10)

}; // Size: 0x58

class UBooleanCameraVariable : public UCameraVariableAsset
{
    bool bDefaultValue;                                                               // 0x0040 (size: 0x1)

}; // Size: 0x48

class UBoomArmCameraNode : public UCameraNode
{
    FVector3dCameraParameter BoomOffset;                                              // 0x0038 (size: 0x28)
    class UCameraValueInterpolator* BoomLengthInterpolator;                           // 0x0060 (size: 0x8)
    FDoubleCameraParameter MaxForwardInterpolationFactor;                             // 0x0068 (size: 0x18)
    FDoubleCameraParameter MaxBackwardInterpolationFactor;                            // 0x0080 (size: 0x18)
    class UInput2DCameraNode* InputSlot;                                              // 0x0098 (size: 0x8)

}; // Size: 0xA0

class UCalcCameraActorCameraNode : public UCameraNode
{
}; // Size: 0x38

class UCameraAsset : public UObject
{
    class UCameraDirector* CameraDirector;                                            // 0x0048 (size: 0x8)
    TArray<UCameraRigTransition*> EnterTransitions;                                   // 0x0050 (size: 0x10)
    TArray<UCameraRigTransition*> ExitTransitions;                                    // 0x0060 (size: 0x10)
    ECameraBuildStatus BuildStatus;                                                   // 0x0070 (size: 0x1)
    FInstancedPropertyBag DefaultParameters;                                          // 0x0078 (size: 0x10)
    TArray<FCameraObjectInterfaceParameterDefinition> ParameterDefinitions;           // 0x0088 (size: 0x10)
    TArray<UCameraRigAsset*> ParameterOwners;                                         // 0x0098 (size: 0x10)
    FCameraAssetAllocationInfo AllocationInfo;                                        // 0x00A8 (size: 0x20)
    TArray<UCameraRigAsset*> CameraRigs;                                              // 0x00C8 (size: 0x10)

}; // Size: 0xD8

class UCameraComponentCameraNode : public UCameraNode
{
}; // Size: 0x38

class UCameraDirector : public UObject
{
    FCameraRigProxyRedirectTable CameraRigProxyRedirectTable;                         // 0x0028 (size: 0x10)
    class UCameraRigProxyTable* CameraRigProxyTable;                                  // 0x0038 (size: 0x8)

}; // Size: 0x40

class UCameraDirectorStateTreeSchema : public UStateTreeSchema
{
    TArray<FStateTreeExternalDataDesc> ContextDataDescs;                              // 0x0028 (size: 0x10)

}; // Size: 0x38

class UCameraNode : public UObject
{
    bool bIsEnabled;                                                                  // 0x0030 (size: 0x1)

}; // Size: 0x38

class UCameraObjectInterfaceBlendableParameter : public UCameraObjectInterfaceParameterBase
{
    ECameraVariableType ParameterType;                                                // 0x0060 (size: 0x4)
    class UScriptStruct* BlendableStructType;                                         // 0x0068 (size: 0x8)
    bool bIsPreBlended;                                                               // 0x0070 (size: 0x1)
    FCameraVariableID PrivateVariableID;                                              // 0x0074 (size: 0x4)
    class UCameraVariableAsset* PrivateVariable;                                      // 0x0078 (size: 0x8)

}; // Size: 0x80

class UCameraObjectInterfaceDataParameter : public UCameraObjectInterfaceParameterBase
{
    ECameraContextDataType DataType;                                                  // 0x0060 (size: 0x4)
    ECameraContextDataContainerType DataContainerType;                                // 0x0064 (size: 0x4)
    class UObject* DataTypeObject;                                                    // 0x0068 (size: 0x8)
    FCameraContextDataID PrivateDataID;                                               // 0x0070 (size: 0x4)

}; // Size: 0x78

class UCameraObjectInterfaceParameterBase : public UObject
{
    FString InterfaceParameterName;                                                   // 0x0030 (size: 0x10)
    class UCameraNode* Target;                                                        // 0x0040 (size: 0x8)
    FName TargetPropertyName;                                                         // 0x0048 (size: 0x8)
    FGuid Guid;                                                                       // 0x0050 (size: 0x10)

}; // Size: 0x60

class UCameraRigAsset : public UBaseCameraObject
{
    class UCameraNode* RootNode;                                                      // 0x00C8 (size: 0x8)
    FGameplayTagContainer GameplayTags;                                               // 0x00D0 (size: 0x20)
    TArray<UCameraRigTransition*> EnterTransitions;                                   // 0x00F0 (size: 0x10)
    TArray<UCameraRigTransition*> ExitTransitions;                                    // 0x0100 (size: 0x10)
    ECameraRigInitialOrientation InitialOrientation;                                  // 0x0110 (size: 0x4)
    ECameraBuildStatus BuildStatus;                                                   // 0x0114 (size: 0x1)
    FGuid Guid;                                                                       // 0x0118 (size: 0x10)

}; // Size: 0x128

class UCameraRigCameraNode : public UCameraNode
{
    FCameraRigAssetReference CameraRigReference;                                      // 0x0048 (size: 0x120)

}; // Size: 0x168

class UCameraRigInput1DSlot : public UInput1DCameraNode
{
    FCameraRigInputSlotParameters InputSlotParameters;                                // 0x0038 (size: 0x2)
    FCameraParameterClamping clamp;                                                   // 0x0040 (size: 0x18)
    FCameraParameterNormalization Normalize;                                          // 0x0058 (size: 0x10)
    EBuiltInDoubleCameraVariable BuiltInVariable;                                     // 0x0068 (size: 0x4)
    FDoubleCameraVariableReference CustomVariable;                                    // 0x0070 (size: 0x10)
    FCameraVariableID TransientVariableID;                                            // 0x0080 (size: 0x4)
    FCameraVariableID VariableID;                                                     // 0x0084 (size: 0x4)

}; // Size: 0x88

class UCameraRigInput2DSlot : public UInput2DCameraNode
{
    FCameraRigInputSlotParameters InputSlotParameters;                                // 0x0038 (size: 0x2)
    FCameraParameterClamping ClampX;                                                  // 0x0040 (size: 0x18)
    FCameraParameterClamping ClampY;                                                  // 0x0058 (size: 0x18)
    FCameraParameterNormalization NormalizeX;                                         // 0x0070 (size: 0x10)
    FCameraParameterNormalization NormalizeY;                                         // 0x0080 (size: 0x10)
    EBuiltInVector2dCameraVariable BuiltInVariable;                                   // 0x0090 (size: 0x4)
    FVector2dCameraVariableReference CustomVariable;                                  // 0x0098 (size: 0x10)
    FCameraVariableID TransientVariableID;                                            // 0x00A8 (size: 0x4)
    FCameraVariableID VariableID;                                                     // 0x00AC (size: 0x4)

}; // Size: 0xB0

class UCameraRigInstanceFunctions : public UBlueprintFunctionLibrary
{

    bool IsValid(FCameraRigInstanceID InstanceId);
}; // Size: 0x28

class UCameraRigParameterInterop : public UBlueprintFunctionLibrary
{

    void SetCameraParameter(const FBlueprintCameraEvaluationDataRef& CameraData, class UCameraRigAsset* CameraRig, FName ParameterName, const int32& NewValue);
    void GetCameraParameter(const FBlueprintCameraEvaluationDataRef& CameraData, class UCameraRigAsset* CameraRig, FName ParameterName, int32& ReturnValue);
}; // Size: 0x28

class UCameraRigParameterInteropLibrary : public UBlueprintFunctionLibrary
{

    FVector3f MakeLiteralVector3f(FVector3f Value);
    FVector2D MakeLiteralVector2D(FVector2D Value);
    FVector MakeLiteralVector(FVector Value);
    FRotator MakeLiteralRotator(FRotator Value);
    FLinearColor MakeLiteralLinearColor(FLinearColor Value);
}; // Size: 0x28

class UCameraRigProxyAsset : public UObject
{
    FGuid Guid;                                                                       // 0x0028 (size: 0x10)

}; // Size: 0x38

class UCameraRigProxyTable : public UObject
{
    TArray<FCameraRigProxyRedirectTableEntry> Entries;                                // 0x0028 (size: 0x10)

}; // Size: 0x38

class UCameraRigTransition : public UObject
{
    TArray<UCameraRigTransitionCondition*> Conditions;                                // 0x0030 (size: 0x10)
    class UBlendCameraNode* Blend;                                                    // 0x0040 (size: 0x8)
    ECameraRigInitialOrientation InitialOrientation;                                  // 0x0048 (size: 0x4)
    bool bOverrideInitialOrientation;                                                 // 0x004C (size: 0x1)
    bool bAllowCameraRigMerging;                                                      // 0x004D (size: 0x1)

}; // Size: 0x50

class UCameraRigTransitionCondition : public UObject
{
}; // Size: 0x30

class UCameraShakeAsset : public UBaseCameraObject
{
    class UShakeCameraNode* RootNode;                                                 // 0x00C0 (size: 0x8)
    class USimpleFixedTimeBlendCameraNode* BlendIn;                                   // 0x00C8 (size: 0x8)
    class USimpleFixedTimeBlendCameraNode* BlendOut;                                  // 0x00D0 (size: 0x8)
    bool bIsSingleInstance;                                                           // 0x00D8 (size: 0x1)
    ECameraBuildStatus BuildStatus;                                                   // 0x00D9 (size: 0x1)
    FGuid Guid;                                                                       // 0x00DC (size: 0x10)

}; // Size: 0xF0

class UCameraShakeCameraNode : public UCameraNode
{
    FCameraShakeAssetReference CameraShakeReference;                                  // 0x0040 (size: 0x30)
    ECameraShakeEvaluationMode EvaluationMode;                                        // 0x0070 (size: 0x1)

}; // Size: 0x78

class UCameraShakeServiceCameraNode : public UCameraNode
{
}; // Size: 0x38

class UCameraValueInterpolator : public UObject
{
}; // Size: 0x28

class UCameraVariableAsset : public UObject
{
    bool bAutoReset;                                                                  // 0x0028 (size: 0x1)
    bool bIsPrivate;                                                                  // 0x0029 (size: 0x1)
    bool bIsInput;                                                                    // 0x002A (size: 0x1)
    FGuid Guid;                                                                       // 0x002C (size: 0x10)

}; // Size: 0x40

class UCameraVariableCollection : public UObject
{
    TArray<UCameraVariableAsset*> Variables;                                          // 0x0028 (size: 0x10)

}; // Size: 0x38

class UClippingPlanesCameraNode : public UCameraNode
{
    FDoubleCameraParameter NearPlane;                                                 // 0x0038 (size: 0x18)
    FDoubleCameraParameter FarPlane;                                                  // 0x0050 (size: 0x18)

}; // Size: 0x68

class UCollisionPushCameraNode : public UCameraNode
{
    ECollisionSafePosition SafePosition;                                              // 0x0038 (size: 0x1)
    FVector3dCameraVariableReference CustomSafePosition;                              // 0x0040 (size: 0x10)
    FVector3dCameraParameter SafePositionOffset;                                      // 0x0050 (size: 0x28)
    ECollisionSafePositionOffsetSpace SafePositionOffsetSpace;                        // 0x0078 (size: 0x1)
    FBooleanCameraVariableReference EnableCollision;                                  // 0x0080 (size: 0x10)
    FFloatCameraParameter CollisionSphereRadius;                                      // 0x0090 (size: 0x10)
    TEnumAsByte<ECollisionChannel> CollisionChannel;                                  // 0x00A0 (size: 0x1)
    class UCameraValueInterpolator* PushInterpolator;                                 // 0x00A8 (size: 0x8)
    class UCameraValueInterpolator* PullInterpolator;                                 // 0x00B0 (size: 0x8)
    bool bRunAsyncCollision;                                                          // 0x00B8 (size: 0x1)

}; // Size: 0xC0

class UCombinedCameraRigsCameraNode : public UCameraNode
{
    TArray<FCameraRigAssetReference> CameraRigReferences;                             // 0x0038 (size: 0x10)

}; // Size: 0x48

class UCompositeShakeCameraNode : public UShakeCameraNode
{
    TArray<UShakeCameraNode*> Shakes;                                                 // 0x0038 (size: 0x10)

}; // Size: 0x48

class UControllerGameplayCameraEvaluationComponent : public UActorComponent
{
    TScriptInterface<class IGameplayCameraSystemHost> CameraSystemHost;               // 0x00D8 (size: 0x10)

}; // Size: 0xE8

class UCriticalDamperValueInterpolator : public UCameraValueInterpolator
{
    float DampingFactor;                                                              // 0x0028 (size: 0x4)

}; // Size: 0x30

class UDampenPositionCameraNode : public UCameraNode
{
    FFloatCameraParameter ForwardDampingFactor;                                       // 0x0038 (size: 0x10)
    FFloatCameraParameter LateralDampingFactor;                                       // 0x0048 (size: 0x10)
    FFloatCameraParameter VerticalDampingFactor;                                      // 0x0058 (size: 0x10)
    ECameraNodeSpace DampenSpace;                                                     // 0x0068 (size: 0x1)

}; // Size: 0x70

class UDampenRotationCameraNode : public UCameraNode
{
    FFloatCameraParameter YawDampingFactor;                                           // 0x0038 (size: 0x10)
    FFloatCameraParameter PitchDampingFactor;                                         // 0x0048 (size: 0x10)
    FFloatCameraParameter RollDampingFactor;                                          // 0x0058 (size: 0x10)

}; // Size: 0x68

class UDefaultRootCameraNode : public URootCameraNode
{
    class UBlendStackCameraNode* BaseLayer;                                           // 0x0038 (size: 0x8)
    class UBlendStackCameraNode* MainLayer;                                           // 0x0040 (size: 0x8)
    class UBlendStackCameraNode* GlobalLayer;                                         // 0x0048 (size: 0x8)
    class UBlendStackCameraNode* VisualLayer;                                         // 0x0050 (size: 0x8)

}; // Size: 0x58

class UDollyFramingCameraNode : public UBaseFramingCameraNode
{
    FBooleanCameraParameter CanMoveLaterally;                                         // 0x0170 (size: 0x10)
    FBooleanCameraParameter CanMoveVertically;                                        // 0x0180 (size: 0x10)

}; // Size: 0x190

class UDoubleCameraVariable : public UCameraVariableAsset
{
    double DefaultValue;                                                              // 0x0040 (size: 0x8)

}; // Size: 0x48

class UDoubleIIRValueInterpolator : public UCameraValueInterpolator
{
    float PrimarySpeed;                                                               // 0x0028 (size: 0x4)
    float IntermediateSpeed;                                                          // 0x002C (size: 0x4)
    bool bUseFixedStep;                                                               // 0x0030 (size: 0x1)

}; // Size: 0x38

class UEnvelopeShakeCameraNode : public UShakeCameraNode
{
    FFloatCameraParameter EaseInTime;                                                 // 0x0038 (size: 0x10)
    FFloatCameraParameter EaseOutTime;                                                // 0x0048 (size: 0x10)
    FFloatCameraParameter TotalTime;                                                  // 0x0058 (size: 0x10)
    class UShakeCameraNode* Shake;                                                    // 0x0068 (size: 0x8)

}; // Size: 0x70

class UFieldOfViewCameraNode : public UCameraNode
{
    FFloatCameraParameter FieldOfView;                                                // 0x0038 (size: 0x10)

}; // Size: 0x48

class UFilmbackCameraNode : public UCameraNode
{
    FFloatCameraParameter SensorWidth;                                                // 0x0038 (size: 0x10)
    FFloatCameraParameter SensorHeight;                                               // 0x0048 (size: 0x10)
    FFloatCameraParameter SensorHorizontalOffset;                                     // 0x0058 (size: 0x10)
    FFloatCameraParameter SensorVerticalOffset;                                       // 0x0068 (size: 0x10)
    FFloatCameraParameter Overscan;                                                   // 0x0078 (size: 0x10)
    FBooleanCameraParameter ConstrainAspectRatio;                                     // 0x0088 (size: 0x10)
    FBooleanCameraParameter OverrideAspectRatioAxisConstraint;                        // 0x0098 (size: 0x10)
    TEnumAsByte<EAspectRatioAxisConstraint> AspectRatioAxisConstraint;                // 0x00A8 (size: 0x1)

}; // Size: 0xB0

class UFloatCameraVariable : public UCameraVariableAsset
{
    float DefaultValue;                                                               // 0x0040 (size: 0x4)

}; // Size: 0x48

class UGameplayCameraComponent : public UGameplayCameraComponentBase
{
    FCameraAssetReference CameraReference;                                            // 0x0280 (size: 0x38)
    class UCameraAsset* Camera;                                                       // 0x02B8 (size: 0x8)

}; // Size: 0x2C0

class UGameplayCameraComponentBase : public USceneComponent
{
    TEnumAsByte<EAutoReceiveInput::Type> AutoActivateForPlayer;                       // 0x0258 (size: 0x1)
    bool bSetControlRotationWhenViewTarget;                                           // 0x0259 (size: 0x1)
    class UCineCameraComponent* OutputCameraComponent;                                // 0x0260 (size: 0x8)

    class UCineCameraComponent* GetOutputCameraComponent();
    FBlueprintCameraEvaluationDataRef GetInitialResult();
    FBlueprintCameraEvaluationDataRef GetConditionalResult(ECameraEvaluationDataCondition Condition);
    void DeactivateCamera(bool bImmediately);
    void ActivateCameraForPlayerIndex(int32 PlayerIndex, bool bSetAsViewTarget, EGameplayCameraComponentActivationMode ActivationMode);
    void ActivateCameraForPlayerController(class APlayerController* PlayerController, bool bSetAsViewTarget, EGameplayCameraComponentActivationMode ActivationMode);
}; // Size: 0x280

class UGameplayCameraParameterSetterComponent : public UActorComponent
{
    FCameraRigAssetReference CameraRigReference;                                      // 0x00B8 (size: 0x120)
    float BlendInTime;                                                                // 0x01D8 (size: 0x4)
    float BlendOutTime;                                                               // 0x01DC (size: 0x4)
    ECameraVariableSetterBlendType BlendType;                                         // 0x01E0 (size: 0x1)

    void StopParameterSetters(bool bImmediately);
    void StartParameterSetters();
    void OnActorEndOverlap(class AActor* OverlappedActor, class AActor* OtherActor);
    void OnActorBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);
}; // Size: 0x1F8

class UGameplayCameraRigComponent : public UGameplayCameraComponentBase
{
    FCameraRigAssetReference CameraRigReference;                                      // 0x0280 (size: 0x120)
    class UCameraAsset* GeneratedCameraAsset;                                         // 0x03A0 (size: 0x8)

}; // Size: 0x3B0

class UGameplayCameraSystemComponent : public USceneComponent
{
    TEnumAsByte<EAutoReceiveInput::Type> AutoActivateForPlayer;                       // 0x0258 (size: 0x1)
    bool bSetPlayerControllerRotation;                                                // 0x0259 (size: 0x1)
    TWeakObjectPtr<class APlayerController> WeakPlayerController;                     // 0x025C (size: 0x8)

    void StopCameraModifierRig(FCameraRigInstanceID InstanceId, bool bImmediately);
    FCameraRigInstanceID StartVisualCameraModifierRig(const class UCameraRigAsset* CameraRig, int32 OrderKey);
    FCameraRigInstanceID StartGlobalCameraModifierRig(const class UCameraRigAsset* CameraRig, int32 OrderKey);
    bool IsCameraSystemActiveForPlayController(class APlayerController* PlayerController);
    void DeactivateCameraSystem(class AActor* NextViewTarget);
    void ActivateCameraSystemForPlayerIndex(int32 PlayerIndex);
    void ActivateCameraSystemForPlayerController(class APlayerController* PlayerController);
}; // Size: 0x270

class UGameplayCamerasSettings : public UDeveloperSettings
{
    bool bAutoBuildInPIE;                                                             // 0x0038 (size: 0x1)
    EGameplayCamerasViewRotationMode DefaultViewRotationMode;                         // 0x003C (size: 0x4)
    int32 CombinedCameraRigNumThreshold;                                              // 0x0040 (size: 0x4)
    double DefaultIKAimingAngleTolerance;                                             // 0x0048 (size: 0x8)
    double DefaultIKAimingDistanceTolerance;                                          // 0x0050 (size: 0x8)
    uint8 DefaultIKAimingMaxIterations;                                               // 0x0058 (size: 0x1)
    double DefaultIKAimingMinDistance;                                                // 0x0060 (size: 0x8)

}; // Size: 0x68

class UGameplayControlRotationComponent : public UActorComponent
{
    TArray<UInputAction*> AxisActions;                                                // 0x00B8 (size: 0x10)
    float AxisActionAngularSpeedThreshold;                                            // 0x00C8 (size: 0x4)
    float AxisActionMagnitudeThreshold;                                               // 0x00CC (size: 0x4)
    TEnumAsByte<EAutoReceiveInput::Type> AutoActivateForPlayer;                       // 0x00D0 (size: 0x1)
    class APlayerController* PlayerController;                                        // 0x00D8 (size: 0x8)
    TScriptInterface<class IGameplayCameraSystemHost> CameraSystemHost;               // 0x00E0 (size: 0x10)

    void DeactivateControlRotationManagement();
    void ActivateControlRotationManagementForPlayerIndex(int32 PlayerIndex);
    void ActivateControlRotationManagementForPlayerController(class APlayerController* PlayerController);
}; // Size: 0x100

class UGameplayTagTransitionCondition : public UCameraRigTransitionCondition
{
    FGameplayTagQuery PreviousGameplayTagQuery;                                       // 0x0030 (size: 0x48)
    FGameplayTagQuery NextGameplayTagQuery;                                           // 0x0078 (size: 0x48)

}; // Size: 0xC0

class UIIRValueInterpolator : public UCameraValueInterpolator
{
    float Speed;                                                                      // 0x0028 (size: 0x4)
    bool bUseFixedStep;                                                               // 0x002C (size: 0x1)

}; // Size: 0x30

class UInput1DCameraNode : public UCameraNode
{
}; // Size: 0x38

class UInput2DCameraNode : public UCameraNode
{
}; // Size: 0x38

class UInputAxisBinding2DCameraNode : public UCameraRigInput2DSlot
{
    TArray<UInputAction*> AxisActions;                                                // 0x00B0 (size: 0x10)
    FBooleanCameraParameter RevertAxisX;                                              // 0x00C0 (size: 0x10)
    FBooleanCameraParameter RevertAxisY;                                              // 0x00D0 (size: 0x10)
    FVector2dCameraParameter Multiplier;                                              // 0x00E0 (size: 0x20)

}; // Size: 0x100

class UInteger32CameraVariable : public UCameraVariableAsset
{
    int32 DefaultValue;                                                               // 0x0040 (size: 0x4)

}; // Size: 0x48

class UIsCameraRigTransitionCondition : public UCameraRigTransitionCondition
{
    class UCameraRigAsset* PreviousCameraRig;                                         // 0x0030 (size: 0x8)
    class UCameraRigAsset* NextCameraRig;                                             // 0x0038 (size: 0x8)

}; // Size: 0x40

class ULensCalibrationCameraNode : public UCameraNode
{
    class ULensFile* LensFile;                                                        // 0x0038 (size: 0x8)

}; // Size: 0x40

class ULensParametersCameraNode : public UCameraNode
{
    FFloatCameraParameter FocalLength;                                                // 0x0038 (size: 0x10)
    FFloatCameraParameter FocusDistance;                                              // 0x0048 (size: 0x10)
    FFloatCameraParameter Aperture;                                                   // 0x0058 (size: 0x10)
    FBooleanCameraParameter EnablePhysicalCamera;                                     // 0x0068 (size: 0x10)

}; // Size: 0x78

class ULinearBlendCameraNode : public USimpleFixedTimeBlendCameraNode
{
}; // Size: 0x40

class ULocationRotationBlendCameraNode : public UBlendCameraNode
{
    class USimpleBlendCameraNode* LocationBlend;                                      // 0x0038 (size: 0x8)
    class USimpleBlendCameraNode* RotationBlend;                                      // 0x0040 (size: 0x8)
    class USimpleBlendCameraNode* OtherBlend;                                         // 0x0048 (size: 0x8)

}; // Size: 0x50

class UMovieSceneCameraFramingZonePropertySystem : public UMovieScenePropertySystem
{
}; // Size: 0x58

class UMovieSceneCameraFramingZoneSection : public UMovieSceneSection
{
    FMovieSceneDoubleChannel LeftMarginCurve;                                         // 0x0110 (size: 0x118)
    FMovieSceneDoubleChannel TopMarginCurve;                                          // 0x0228 (size: 0x118)
    FMovieSceneDoubleChannel RightMarginCurve;                                        // 0x0340 (size: 0x118)
    FMovieSceneDoubleChannel BottomMarginCurve;                                       // 0x0458 (size: 0x118)

}; // Size: 0x570

class UMovieSceneCameraFramingZoneTrack : public UMovieScenePropertyTrack
{
}; // Size: 0x140

class UMovieSceneCameraParameterDecoration : public UObject
{
}; // Size: 0x30

class UMovieSceneCameraParameterInstantiator : public UMovieSceneEntityInstantiatorSystem
{
}; // Size: 0x50

class UObjectTreeGraphComment : public UObject
{
}; // Size: 0x30

class UOcclusionMaterialCameraNode : public UCameraNode
{
    class UMaterialInterface* OcclusionTransparencyMaterial;                          // 0x0038 (size: 0x8)
    FFloatCameraParameter OcclusionSphereRadius;                                      // 0x0040 (size: 0x10)
    TEnumAsByte<ECollisionChannel> OcclusionChannel;                                  // 0x0050 (size: 0x1)
    ECameraNodeOriginPosition OcclusionTargetPosition;                                // 0x0051 (size: 0x1)
    ECameraNodeSpace OcclusionTargetOffsetSpace;                                      // 0x0052 (size: 0x1)
    FVector3dCameraParameter OcclusionTargetOffset;                                   // 0x0058 (size: 0x28)

}; // Size: 0x80

class UOffsetCameraNode : public UCameraNode
{
    FVector3dCameraParameter TranslationOffset;                                       // 0x0038 (size: 0x28)
    FRotator3dCameraParameter RotationOffset;                                         // 0x0060 (size: 0x28)
    ECameraNodeSpace OffsetSpace;                                                     // 0x0088 (size: 0x1)

}; // Size: 0x90

class UOrbitBlendCameraNode : public UBlendCameraNode
{
    class USimpleBlendCameraNode* DrivingBlend;                                       // 0x0038 (size: 0x8)

}; // Size: 0x40

class UOrthographicCameraNode : public UCameraNode
{
    FBooleanCameraParameter EnableOrthographicMode;                                   // 0x0038 (size: 0x10)
    FFloatCameraParameter OrthographicWidth;                                          // 0x0048 (size: 0x10)

}; // Size: 0x58

class UPanningFramingCameraNode : public UBaseFramingCameraNode
{
    FBooleanCameraParameter CanPanLaterally;                                          // 0x0170 (size: 0x10)
    FBooleanCameraParameter CanPanVertically;                                         // 0x0180 (size: 0x10)

}; // Size: 0x190

class UPerlinNoiseLocationShakeCameraNode : public UShakeCameraNode
{
    FFloatCameraParameter AmplitudeMultiplier;                                        // 0x0038 (size: 0x10)
    FFloatCameraParameter FrequencyMultiplier;                                        // 0x0048 (size: 0x10)
    FInteger32CameraParameter Octaves;                                                // 0x0058 (size: 0x10)
    FPerlinNoiseData X;                                                               // 0x0068 (size: 0x8)
    FPerlinNoiseData Y;                                                               // 0x0070 (size: 0x8)
    FPerlinNoiseData Z;                                                               // 0x0078 (size: 0x8)

}; // Size: 0x80

class UPerlinNoiseRotationShakeCameraNode : public UShakeCameraNode
{
    FFloatCameraParameter AmplitudeMultiplier;                                        // 0x0038 (size: 0x10)
    FFloatCameraParameter FrequencyMultiplier;                                        // 0x0048 (size: 0x10)
    FInteger32CameraParameter Octaves;                                                // 0x0058 (size: 0x10)
    FPerlinNoiseData Yaw;                                                             // 0x0068 (size: 0x8)
    FPerlinNoiseData Pitch;                                                           // 0x0070 (size: 0x8)
    FPerlinNoiseData Roll;                                                            // 0x0078 (size: 0x8)

}; // Size: 0x80

class UPopBlendCameraNode : public UBlendCameraNode
{
}; // Size: 0x38

class UPopValueInterpolator : public UCameraValueInterpolator
{
}; // Size: 0x28

class UPostProcessCameraNode : public UCameraNode
{
    FPostProcessSettings PostProcessSettings;                                         // 0x0040 (size: 0x760)

}; // Size: 0x7A0

class UPriorityQueueCameraDirector : public UCameraDirector
{
}; // Size: 0x40

class URootCameraNode : public UCameraNode
{
}; // Size: 0x38

class URotator3dCameraVariable : public UCameraVariableAsset
{
    FRotator3d DefaultValue;                                                          // 0x0040 (size: 0x18)

}; // Size: 0x58

class URotator3fCameraVariable : public UCameraVariableAsset
{
    FRotator3f DefaultValue;                                                          // 0x0040 (size: 0xC)

}; // Size: 0x50

class USetLocationCameraNode : public UCameraNode
{
    FVector3dCameraParameter Location;                                                // 0x0038 (size: 0x28)
    ECameraNodeSpace OffsetSpace;                                                     // 0x0060 (size: 0x1)

}; // Size: 0x68

class USetRotationCameraNode : public UCameraNode
{
    FRotator3dCameraParameter Rotation;                                               // 0x0038 (size: 0x28)
    ECameraNodeSpace OffsetSpace;                                                     // 0x0060 (size: 0x1)

}; // Size: 0x68

class UShakeCameraNode : public UCameraNode
{
}; // Size: 0x38

class USimpleBlendCameraNode : public UBlendCameraNode
{
}; // Size: 0x38

class USimpleFixedTimeBlendCameraNode : public USimpleBlendCameraNode
{
    float BlendTime;                                                                  // 0x0038 (size: 0x4)

}; // Size: 0x40

class USingleCameraDirector : public UCameraDirector
{
    class UCameraRigAsset* CameraRig;                                                 // 0x0040 (size: 0x8)

}; // Size: 0x48

class USmoothBlendCameraNode : public USimpleFixedTimeBlendCameraNode
{
    ESmoothCameraBlendType BlendType;                                                 // 0x0040 (size: 0x4)

}; // Size: 0x48

class USplineFieldOfViewCameraNode : public UCameraNode
{
    FFloatCameraParameter SplineInput;                                                // 0x0038 (size: 0x10)
    FCameraSingleCurve FieldOfViewSpline;                                             // 0x0048 (size: 0x80)

}; // Size: 0xC8

class USplineOffsetCameraNode : public UCameraNode
{
    FFloatCameraParameter SplineInput;                                                // 0x0038 (size: 0x10)
    FCameraVectorCurve TranslationOffsetSpline;                                       // 0x0048 (size: 0x180)
    FCameraRotatorCurve RotationOffsetSpline;                                         // 0x01C8 (size: 0x180)
    ECameraNodeSpace OffsetSpace;                                                     // 0x0348 (size: 0x1)

}; // Size: 0x350

class USplineOrbitCameraNode : public UCameraNode
{
    FCameraVectorCurve LocationOffsetSpline;                                          // 0x0038 (size: 0x180)
    FCameraVectorCurve TargetOffsetSpline;                                            // 0x01B8 (size: 0x180)
    FCameraRotatorCurve RotationOffsetSpline;                                         // 0x0338 (size: 0x180)
    FFloatCameraParameter LocationOffsetMultiplier;                                   // 0x04B8 (size: 0x10)
    ECameraNodeSpace TargetOffsetSpace;                                               // 0x04C8 (size: 0x1)
    class UInput2DCameraNode* InputSlot;                                              // 0x04D0 (size: 0x8)

}; // Size: 0x4D8

class UStateTreeCameraDirector : public UCameraDirector
{
    FStateTreeReference StateTreeReference;                                           // 0x0040 (size: 0x28)

}; // Size: 0x68

class UTargetRayCastCameraNode : public UCameraNode
{
    TEnumAsByte<ECollisionChannel> TraceChannel;                                      // 0x0038 (size: 0x1)
    FBooleanCameraParameter AutoFocus;                                                // 0x0040 (size: 0x10)

}; // Size: 0x50

class UTransform3dCameraVariable : public UCameraVariableAsset
{
    FTransform3d DefaultValue;                                                        // 0x0040 (size: 0x60)

}; // Size: 0xA0

class UTransform3fCameraVariable : public UCameraVariableAsset
{
    FTransform3f DefaultValue;                                                        // 0x0040 (size: 0x30)

}; // Size: 0x70

class UUpdateTrackerCameraNode : public UCameraNode
{
    FDoubleCameraParameter DoubleParameter;                                           // 0x0038 (size: 0x18)
    FVector3dCameraParameter VectorParameter;                                         // 0x0050 (size: 0x28)

}; // Size: 0x78

class UVector2dCameraVariable : public UCameraVariableAsset
{
    FVector2D DefaultValue;                                                           // 0x0040 (size: 0x10)

}; // Size: 0x50

class UVector2fCameraVariable : public UCameraVariableAsset
{
    FVector2f DefaultValue;                                                           // 0x0040 (size: 0x8)

}; // Size: 0x48

class UVector3dCameraVariable : public UCameraVariableAsset
{
    FVector3d DefaultValue;                                                           // 0x0040 (size: 0x18)

}; // Size: 0x58

class UVector3fCameraVariable : public UCameraVariableAsset
{
    FVector3f DefaultValue;                                                           // 0x0040 (size: 0xC)

}; // Size: 0x50

class UVector4dCameraVariable : public UCameraVariableAsset
{
    FVector4d DefaultValue;                                                           // 0x0040 (size: 0x20)

}; // Size: 0x60

class UVector4fCameraVariable : public UCameraVariableAsset
{
    FVector4f DefaultValue;                                                           // 0x0040 (size: 0x10)

}; // Size: 0x50

class UViewTargetTransitionParamsBlendCameraNode : public USimpleBlendCameraNode
{
    FViewTargetTransitionParams TransitionParams;                                     // 0x0038 (size: 0x10)

}; // Size: 0x48

#endif
