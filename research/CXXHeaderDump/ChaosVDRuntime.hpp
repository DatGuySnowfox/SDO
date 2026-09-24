#ifndef UE4SS_SDK_ChaosVDRuntime_HPP
#define UE4SS_SDK_ChaosVDRuntime_HPP

#include "ChaosVDRuntime_enums.hpp"

struct FChaosVDAABBTreeDataWrapper : public FChaosVDAccelerationStructureBase
{
    int32 RootNodeIndex;                                                              // 0x0018 (size: 0x4)
    int32 TreeDepth;                                                                  // 0x001C (size: 0x4)
    int32 NodesNum;                                                                   // 0x0020 (size: 0x4)
    int32 LeavesNum;                                                                  // 0x0024 (size: 0x4)
    bool bDynamicTree;                                                                // 0x0028 (size: 0x1)
    int32 MaxChildrenInLeaf;                                                          // 0x002C (size: 0x4)
    int32 MaxTreeDepth;                                                               // 0x0030 (size: 0x4)
    double MaxPayloadBounds;                                                          // 0x0038 (size: 0x8)

}; // Size: 0x70

struct FChaosVDAABBTreeLeafDataWrapper : public FChaosVDWrapperDataBase
{
    TArray<FChaosVDAABBTreePayloadBoundsElement> Elements;                            // 0x0010 (size: 0x10)
    FBox Bounds;                                                                      // 0x0020 (size: 0x38)

}; // Size: 0x58

struct FChaosVDAABBTreeNodeDataWrapper : public FChaosVDWrapperDataBase
{
    FBox ChildrenBounds;                                                              // 0x0010 (size: 0x70)
    int32 ChildrenNodes;                                                              // 0x0080 (size: 0x8)
    int32 ParentNode;                                                                 // 0x0088 (size: 0x4)
    uint8 bLeaf;                                                                      // 0x008C (size: 0x1)
    uint8 bDirtyNode;                                                                 // 0x008C (size: 0x1)

}; // Size: 0x90

struct FChaosVDAABBTreePayloadBoundsElement : public FChaosVDWrapperDataBase
{
    int32 ParticleIndex;                                                              // 0x0010 (size: 0x4)
    FBox Bounds;                                                                      // 0x0018 (size: 0x38)

}; // Size: 0x88

struct FChaosVDAccelerationStructureBase : public FChaosVDWrapperDataBase
{
    int32 SolverId;                                                                   // 0x0010 (size: 0x4)
    EChaosVDAccelerationStructureType Type;                                           // 0x0014 (size: 0x4)

}; // Size: 0x18

struct FChaosVDAccelerationStructureContainer
{
}; // Size: 0x50

struct FChaosVDBVCellElementDataWrapper : public FChaosVDWrapperDataBase
{
    FBox Bounds;                                                                      // 0x0010 (size: 0x38)
    int32 ParticleIndex;                                                              // 0x0048 (size: 0x4)

}; // Size: 0x68

struct FChaosVDBoundingVolumeDataWrapper : public FChaosVDAccelerationStructureBase
{
    double MaxPayloadBounds;                                                          // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FChaosVDCharacterGroundConstraint : public FChaosVDConstraintDataWrapperBase
{
    int32 ConstraintIndex;                                                            // 0x0014 (size: 0x4)
    FChaosVDCharacterGroundConstraintStateDataWrapper State;                          // 0x0020 (size: 0x58)
    FChaosVDCharacterGroundConstraintSettingsDataWrapper Settings;                    // 0x0078 (size: 0x68)
    FChaosVDCharacterGroundConstraintDataDataWrapper Data;                            // 0x00E0 (size: 0x58)

}; // Size: 0x138

struct FChaosVDCharacterGroundConstraintDataDataWrapper : public FChaosVDWrapperDataBase
{
    FVector GroundNormal;                                                             // 0x0010 (size: 0x18)
    FVector TargetDeltaPosition;                                                      // 0x0028 (size: 0x18)
    double TargetDeltaFacing;                                                         // 0x0040 (size: 0x8)
    double GroundDistance;                                                            // 0x0048 (size: 0x8)
    double CosMaxWalkableSlopeAngle;                                                  // 0x0050 (size: 0x8)

}; // Size: 0x58

struct FChaosVDCharacterGroundConstraintSettingsDataWrapper : public FChaosVDWrapperDataBase
{
    FVector VerticalAxis;                                                             // 0x0010 (size: 0x18)
    double TargetHeight;                                                              // 0x0028 (size: 0x8)
    double RadialForceLimit;                                                          // 0x0030 (size: 0x8)
    double FrictionForceLimit;                                                        // 0x0038 (size: 0x8)
    double TwistTorqueLimit;                                                          // 0x0040 (size: 0x8)
    double SwingTorqueLimit;                                                          // 0x0048 (size: 0x8)
    double CosMaxWalkableSlopeAngle;                                                  // 0x0050 (size: 0x8)
    double DampingFactor;                                                             // 0x0058 (size: 0x8)
    double AssumedOnGroundHeight;                                                     // 0x0060 (size: 0x8)

}; // Size: 0x68

struct FChaosVDCharacterGroundConstraintStateDataWrapper : public FChaosVDWrapperDataBase
{
    bool bDisabled;                                                                   // 0x0020 (size: 0x1)
    FVector SolverAppliedForce;                                                       // 0x0028 (size: 0x18)
    FVector SolverAppliedTorque;                                                      // 0x0040 (size: 0x18)

}; // Size: 0x58

struct FChaosVDCollisionChannelInfo
{
    FString DisplayName;                                                              // 0x0000 (size: 0x10)
    int32 CollisionChannel;                                                           // 0x0010 (size: 0x4)
    bool bIsTraceType;                                                                // 0x0014 (size: 0x1)

}; // Size: 0x18

struct FChaosVDCollisionChannelsInfoContainer
{
    FChaosVDCollisionChannelInfo CustomChannelsNames;                                 // 0x0000 (size: 0x300)

}; // Size: 0x300

struct FChaosVDCollisionFilterData
{
    uint32 Word0;                                                                     // 0x0000 (size: 0x4)
    uint32 Word1;                                                                     // 0x0004 (size: 0x4)
    uint32 Word2;                                                                     // 0x0008 (size: 0x4)
    uint32 Word3;                                                                     // 0x000C (size: 0x4)

}; // Size: 0x10

struct FChaosVDCollisionMaterial
{
    int32 FaceIndex;                                                                  // 0x0000 (size: 0x4)
    float MaterialDynamicFriction;                                                    // 0x0004 (size: 0x4)
    float MaterialStaticFriction;                                                     // 0x0008 (size: 0x4)
    float MaterialRestitution;                                                        // 0x000C (size: 0x4)
    float DynamicFriction;                                                            // 0x0010 (size: 0x4)
    float StaticFriction;                                                             // 0x0014 (size: 0x4)
    float Restitution;                                                                // 0x0018 (size: 0x4)
    float RestitutionThreshold;                                                       // 0x001C (size: 0x4)
    float InvMassScale0;                                                              // 0x0020 (size: 0x4)
    float InvMassScale1;                                                              // 0x0024 (size: 0x4)
    float InvInertiaScale0;                                                           // 0x0028 (size: 0x4)
    float InvInertiaScale1;                                                           // 0x002C (size: 0x4)

}; // Size: 0x30

struct FChaosVDCollisionObjectQueryParams : public FChaosVDWrapperDataBase
{
    uint8 ObjectTypesToQuery;                                                         // 0x0010 (size: 0x1)
    uint8 IgnoreMask;                                                                 // 0x0011 (size: 0x1)

}; // Size: 0x18

struct FChaosVDCollisionQueryParams : public FChaosVDWrapperDataBase
{
    FName TraceTag;                                                                   // 0x0010 (size: 0x8)
    FName OwnerTag;                                                                   // 0x0018 (size: 0x8)
    uint8 bTraceComplex;                                                              // 0x0020 (size: 0x1)
    uint8 bFindInitialOverlaps;                                                       // 0x0020 (size: 0x1)
    uint8 bReturnFaceIndex;                                                           // 0x0020 (size: 0x1)
    uint8 bReturnPhysicalMaterial;                                                    // 0x0020 (size: 0x1)
    uint8 bIgnoreBlocks;                                                              // 0x0020 (size: 0x1)
    uint8 bIgnoreTouches;                                                             // 0x0020 (size: 0x1)
    uint8 bSkipNarrowPhase;                                                           // 0x0020 (size: 0x1)
    uint8 bTraceIntoSubComponents;                                                    // 0x0020 (size: 0x1)
    uint8 bReplaceHitWithSubComponents;                                               // 0x0021 (size: 0x1)
    uint8 IgnoreMask;                                                                 // 0x0022 (size: 0x1)
    TArray<FName> IgnoredActorsNames;                                                 // 0x0048 (size: 0x10)
    TArray<FName> IgnoredComponentsNames;                                             // 0x0058 (size: 0x10)

}; // Size: 0x68

struct FChaosVDCollisionResponseParams : public FChaosVDWrapperDataBase
{
}; // Size: 0x40

struct FChaosVDConnectivityEdge
{
    int32 SiblingParticleID;                                                          // 0x0000 (size: 0x4)
    float Strain;                                                                     // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FChaosVDConstraint
{
    uint8 bIsCurrent;                                                                 // 0x0000 (size: 0x1)
    uint8 bDisabled;                                                                  // 0x0000 (size: 0x1)
    uint8 bUseManifold;                                                               // 0x0000 (size: 0x1)
    uint8 bUseIncrementalManifold;                                                    // 0x0000 (size: 0x1)
    uint8 bCanRestoreManifold;                                                        // 0x0000 (size: 0x1)
    uint8 bWasManifoldRestored;                                                       // 0x0000 (size: 0x1)
    uint8 bIsQuadratic0;                                                              // 0x0000 (size: 0x1)
    uint8 bIsQuadratic1;                                                              // 0x0000 (size: 0x1)
    uint8 bIsProbe;                                                                   // 0x0001 (size: 0x1)
    uint8 bCCDEnabled;                                                                // 0x0001 (size: 0x1)
    uint8 bCCDSweepEnabled;                                                           // 0x0001 (size: 0x1)
    uint8 bModifierApplied;                                                           // 0x0001 (size: 0x1)
    uint8 bMaterialSet;                                                               // 0x0001 (size: 0x1)
    FChaosVDCollisionMaterial Material;                                               // 0x0004 (size: 0x30)
    FVector AccumulatedImpulse;                                                       // 0x0038 (size: 0x18)
    EChaosVDContactShapesType ShapesType;                                             // 0x0050 (size: 0x4)
    FTransform ShapeWorldTransforms;                                                  // 0x0060 (size: 0xC0)
    FTransform ImplicitTransforms;                                                    // 0x0120 (size: 0xC0)
    float CullDistance;                                                               // 0x01E0 (size: 0x4)
    TArray<float> CollisionMargins;                                                   // 0x01E8 (size: 0x10)
    float CollisionTolerance;                                                         // 0x01F8 (size: 0x4)
    int32 ClosestManifoldPointIndex;                                                  // 0x01FC (size: 0x4)
    int32 ExpectedNumManifoldPoints;                                                  // 0x0200 (size: 0x4)
    FVector LastShapeWorldPositionDelta;                                              // 0x0208 (size: 0x18)
    FQuat LastShapeWorldRotationDelta;                                                // 0x0220 (size: 0x20)
    float Stiffness;                                                                  // 0x0240 (size: 0x4)
    float MinInitialPhi;                                                              // 0x0244 (size: 0x4)
    float InitialOverlapDepenetrationVelocity;                                        // 0x0248 (size: 0x4)
    float CCDTimeOfImpact;                                                            // 0x024C (size: 0x4)
    float CCDEnablePenetration;                                                       // 0x0250 (size: 0x4)
    float CCDTargetPenetration;                                                       // 0x0254 (size: 0x4)
    TArray<FChaosVDManifoldPoint> ManifoldPoints;                                     // 0x0258 (size: 0x10)
    int32 Particle0Index;                                                             // 0x0268 (size: 0x4)
    int32 Particle1Index;                                                             // 0x026C (size: 0x4)
    int32 SolverId;                                                                   // 0x0270 (size: 0x4)

}; // Size: 0x280

struct FChaosVDConstraintDataWrapperBase : public FChaosVDWrapperDataBase
{
}; // Size: 0x10

struct FChaosVDContactPoint
{
    FVector ShapeContactPoints;                                                       // 0x0000 (size: 0x30)
    FVector ShapeContactNormal;                                                       // 0x0030 (size: 0x18)
    float Phi;                                                                        // 0x0048 (size: 0x4)
    int32 FaceIndex;                                                                  // 0x004C (size: 0x4)
    EChaosVDContactPointType ContactType;                                             // 0x0050 (size: 0x1)

}; // Size: 0x58

struct FChaosVDDebugDrawBoxDataWrapper : public FChaosVDDebugDrawShapeBase
{
    FBox Box;                                                                         // 0x0028 (size: 0x38)

}; // Size: 0x60

struct FChaosVDDebugDrawImplicitObjectDataWrapper : public FChaosVDDebugDrawShapeBase
{
}; // Size: 0x90

struct FChaosVDDebugDrawLineDataWrapper : public FChaosVDDebugDrawShapeBase
{
    FVector StartLocation;                                                            // 0x0028 (size: 0x18)
    FVector EndLocation;                                                              // 0x0040 (size: 0x18)
    bool bIsArrow;                                                                    // 0x0058 (size: 0x1)

}; // Size: 0x60

struct FChaosVDDebugDrawShapeBase : public FChaosVDWrapperDataBase
{
    int32 SolverId;                                                                   // 0x0010 (size: 0x4)
    FName Tag;                                                                        // 0x0014 (size: 0x8)
    FColor Color;                                                                     // 0x001C (size: 0x4)
    EChaosVDParticleContext ThreadContext;                                            // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FChaosVDDebugDrawSphereDataWrapper : public FChaosVDDebugDrawShapeBase
{
    FVector Origin;                                                                   // 0x0028 (size: 0x18)
    float Radius;                                                                     // 0x0040 (size: 0x4)

}; // Size: 0x48

struct FChaosVDDebugShapeDataContainer
{
}; // Size: 0x40

struct FChaosVDFRigidParticleControlFlags : public FChaosVDWrapperDataBase
{
    bool bGravityEnabled;                                                             // 0x0010 (size: 0x1)
    bool bCCDEnabled;                                                                 // 0x0011 (size: 0x1)
    bool bOneWayInteractionEnabled;                                                   // 0x0012 (size: 0x1)
    bool bInertiaConditioningEnabled;                                                 // 0x0013 (size: 0x1)
    int32 GravityGroupIndex;                                                          // 0x0014 (size: 0x4)
    bool bMACDEnabled;                                                                // 0x0018 (size: 0x1)
    bool bGyroscopicTorqueEnabled;                                                    // 0x0019 (size: 0x1)
    uint8 PositionSolverIterationCount;                                               // 0x001A (size: 0x1)
    uint8 VelocitySolverIterationCount;                                               // 0x001B (size: 0x1)
    uint8 ProjectionSolverIterationCount;                                             // 0x001C (size: 0x1)

}; // Size: 0x20

struct FChaosVDGTJointStateDataWrapper : public FChaosVDWrapperDataBase
{
    uint8 bIsBreaking;                                                                // 0x0010 (size: 0x1)
    uint8 bIsBroken;                                                                  // 0x0010 (size: 0x1)
    uint8 bDriveTargetChanged;                                                        // 0x0010 (size: 0x1)
    uint8 bIsViolating;                                                               // 0x0010 (size: 0x1)
    FVector Force;                                                                    // 0x0018 (size: 0x18)
    FVector Torque;                                                                   // 0x0030 (size: 0x18)
    float LinearViolation;                                                            // 0x0048 (size: 0x4)
    float AngularViolation;                                                           // 0x004C (size: 0x4)

}; // Size: 0x50

struct FChaosVDJointConstraint : public FChaosVDConstraintDataWrapperBase
{
    int32 ConstraintIndex;                                                            // 0x0014 (size: 0x4)
    FChaosVDJointStateDataWrapper PhysicsThreadJointState;                            // 0x0020 (size: 0x60)
    FChaosVDGTJointStateDataWrapper GameThreadJointState;                             // 0x0080 (size: 0x50)
    FChaosVDJointSettingsDataWrapper JointSettings;                                   // 0x00D0 (size: 0x2F0)

}; // Size: 0x3C0

struct FChaosVDJointSettingsDataWrapper : public FChaosVDWrapperDataBase
{
    FTransform ConnectorTransforms;                                                   // 0x0010 (size: 0xC0)
    double Stiffness;                                                                 // 0x00D0 (size: 0x8)
    double LinearProjection;                                                          // 0x00D8 (size: 0x8)
    double AngularProjection;                                                         // 0x00E0 (size: 0x8)
    double ShockPropagation;                                                          // 0x00E8 (size: 0x8)
    double TeleportDistance;                                                          // 0x00F0 (size: 0x8)
    double TeleportAngle;                                                             // 0x00F8 (size: 0x8)
    double ParentInvMassScale;                                                        // 0x0100 (size: 0x8)
    uint8 bCollisionEnabled;                                                          // 0x0108 (size: 0x1)
    uint8 bMassConditioningEnabled;                                                   // 0x0108 (size: 0x1)
    uint8 bUseLinearSolver;                                                           // 0x0108 (size: 0x1)
    uint8 bSoftLinearLimitsEnabled;                                                   // 0x0108 (size: 0x1)
    uint8 bSoftTwistLimitsEnabled;                                                    // 0x0108 (size: 0x1)
    uint8 bSoftSwingLimitsEnabled;                                                    // 0x0108 (size: 0x1)
    uint8 bAngularSLerpPositionDriveEnabled;                                          // 0x0108 (size: 0x1)
    uint8 bAngularSLerpVelocityDriveEnabled;                                          // 0x0108 (size: 0x1)
    uint8 bAngularTwistPositionDriveEnabled;                                          // 0x0109 (size: 0x1)
    uint8 bAngularTwistVelocityDriveEnabled;                                          // 0x0109 (size: 0x1)
    uint8 bAngularSwingPositionDriveEnabled;                                          // 0x0109 (size: 0x1)
    uint8 bAngularSwingVelocityDriveEnabled;                                          // 0x0109 (size: 0x1)
    EChaosVDJointMotionType LinearMotionTypes;                                        // 0x010C (size: 0xC)
    double LinearLimit;                                                               // 0x0118 (size: 0x8)
    EChaosVDJointMotionType AngularMotionTypes;                                       // 0x0120 (size: 0xC)
    FVector AngularLimits;                                                            // 0x0130 (size: 0x18)
    EChaosVDJointForceMode LinearSoftForceMode;                                       // 0x0148 (size: 0x4)
    EChaosVDJointForceMode AngularSoftForceMode;                                      // 0x014C (size: 0x4)
    double SoftLinearStiffness;                                                       // 0x0150 (size: 0x8)
    double SoftLinearDamping;                                                         // 0x0158 (size: 0x8)
    double SoftTwistStiffness;                                                        // 0x0160 (size: 0x8)
    double SoftTwistDamping;                                                          // 0x0168 (size: 0x8)
    double SoftSwingStiffness;                                                        // 0x0170 (size: 0x8)
    double SoftSwingDamping;                                                          // 0x0178 (size: 0x8)
    double LinearRestitution;                                                         // 0x0180 (size: 0x8)
    double TwistRestitution;                                                          // 0x0188 (size: 0x8)
    double SwingRestitution;                                                          // 0x0190 (size: 0x8)
    double LinearContactDistance;                                                     // 0x0198 (size: 0x8)
    double TwistContactDistance;                                                      // 0x01A0 (size: 0x8)
    double SwingContactDistance;                                                      // 0x01A8 (size: 0x8)
    FVector LinearDrivePositionTarget;                                                // 0x01B0 (size: 0x18)
    FVector LinearDriveVelocityTarget;                                                // 0x01C8 (size: 0x18)
    uint8 bLinearPositionDriveEnabled0;                                               // 0x01E0 (size: 0x1)
    uint8 bLinearPositionDriveEnabled1;                                               // 0x01E0 (size: 0x1)
    uint8 bLinearPositionDriveEnabled2;                                               // 0x01E0 (size: 0x1)
    uint8 bLinearVelocityDriveEnabled0;                                               // 0x01E0 (size: 0x1)
    uint8 bLinearVelocityDriveEnabled1;                                               // 0x01E0 (size: 0x1)
    uint8 bLinearVelocityDriveEnabled2;                                               // 0x01E0 (size: 0x1)
    EChaosVDJointForceMode LinearDriveForceMode;                                      // 0x01E4 (size: 0x4)
    FVector LinearDriveStiffness;                                                     // 0x01E8 (size: 0x18)
    FVector LinearDriveDamping;                                                       // 0x0200 (size: 0x18)
    FVector LinearDriveMaxForce;                                                      // 0x0218 (size: 0x18)
    FQuat AngularDrivePositionTarget;                                                 // 0x0230 (size: 0x20)
    FVector AngularDriveVelocityTarget;                                               // 0x0250 (size: 0x18)
    EChaosVDJointForceMode AngularDriveForceMode;                                     // 0x0268 (size: 0x4)
    FVector AngularDriveStiffness;                                                    // 0x0270 (size: 0x18)
    FVector AngularDriveDamping;                                                      // 0x0288 (size: 0x18)
    FVector AngularDriveMaxTorque;                                                    // 0x02A0 (size: 0x18)
    double LinearBreakForce;                                                          // 0x02B8 (size: 0x8)
    double LinearPlasticityLimit;                                                     // 0x02C0 (size: 0x8)
    EChaosVDPlasticityType LinearPlasticityType;                                      // 0x02C8 (size: 0x4)
    double LinearPlasticityInitialDistanceSquared;                                    // 0x02D0 (size: 0x8)
    double AngularBreakTorque;                                                        // 0x02D8 (size: 0x8)
    double AngularPlasticityLimit;                                                    // 0x02E0 (size: 0x8)
    double ContactTransferScale;                                                      // 0x02E8 (size: 0x8)

}; // Size: 0x2F0

struct FChaosVDJointSolverSettingsDataWrapper : public FChaosVDWrapperDataBase
{
    double SwingTwistAngleTolerance;                                                  // 0x0010 (size: 0x8)
    double PositionTolerance;                                                         // 0x0018 (size: 0x8)
    double AngleTolerance;                                                            // 0x0020 (size: 0x8)
    double MinParentMassRatio;                                                        // 0x0028 (size: 0x8)
    double MaxInertiaRatio;                                                           // 0x0030 (size: 0x8)
    double MinSolverStiffness;                                                        // 0x0038 (size: 0x8)
    double MaxSolverStiffness;                                                        // 0x0040 (size: 0x8)
    int32 NumIterationsAtMaxSolverStiffness;                                          // 0x0048 (size: 0x4)
    int32 NumShockPropagationIterations;                                              // 0x004C (size: 0x4)
    uint8 bUseLinearSolver;                                                           // 0x0050 (size: 0x1)
    uint8 bSortEnabled;                                                               // 0x0050 (size: 0x1)
    uint8 bSolvePositionLast;                                                         // 0x0050 (size: 0x1)
    uint8 bUsePositionBasedDrives;                                                    // 0x0050 (size: 0x1)
    uint8 bEnableTwistLimits;                                                         // 0x0050 (size: 0x1)
    uint8 bEnableSwingLimits;                                                         // 0x0050 (size: 0x1)
    uint8 bEnableDrives;                                                              // 0x0050 (size: 0x1)
    double LinearStiffnessOverride;                                                   // 0x0058 (size: 0x8)
    double TwistStiffnessOverride;                                                    // 0x0060 (size: 0x8)
    double SwingStiffnessOverride;                                                    // 0x0068 (size: 0x8)
    double LinearProjectionOverride;                                                  // 0x0070 (size: 0x8)
    double AngularProjectionOverride;                                                 // 0x0078 (size: 0x8)
    double ShockPropagationOverride;                                                  // 0x0080 (size: 0x8)
    double LinearDriveStiffnessOverride;                                              // 0x0088 (size: 0x8)
    double LinearDriveDampingOverride;                                                // 0x0090 (size: 0x8)
    double AngularDriveStiffnessOverride;                                             // 0x0098 (size: 0x8)
    double AngularDriveDampingOverride;                                               // 0x00A0 (size: 0x8)
    double SoftLinearStiffnessOverride;                                               // 0x00A8 (size: 0x8)
    double SoftLinearDampingOverride;                                                 // 0x00B0 (size: 0x8)
    double SoftTwistStiffnessOverride;                                                // 0x00B8 (size: 0x8)
    double SoftTwistDampingOverride;                                                  // 0x00C0 (size: 0x8)
    double SoftSwingStiffnessOverride;                                                // 0x00C8 (size: 0x8)
    double SoftSwingDampingOverride;                                                  // 0x00D0 (size: 0x8)

}; // Size: 0xD8

struct FChaosVDJointStateDataWrapper : public FChaosVDWrapperDataBase
{
    uint8 bDisabled;                                                                  // 0x0020 (size: 0x1)
    uint8 bBroken;                                                                    // 0x0020 (size: 0x1)
    uint8 bBreaking;                                                                  // 0x0020 (size: 0x1)
    uint8 bDriveTargetChanged;                                                        // 0x0020 (size: 0x1)
    uint8 bEnabledDuringResim;                                                        // 0x0020 (size: 0x1)
    FVector LinearImpulse;                                                            // 0x0028 (size: 0x18)
    FVector AngularImpulse;                                                           // 0x0040 (size: 0x18)
    EChaosVDJointReSimType ResimType;                                                 // 0x0058 (size: 0x4)
    EChaosVDJointSyncType SyncState;                                                  // 0x005C (size: 0x4)

}; // Size: 0x60

struct FChaosVDKinematicTarget : public FChaosVDWrapperDataBase
{
    FQuat Rotation;                                                                   // 0x0010 (size: 0x20)
    FVector Position;                                                                 // 0x0030 (size: 0x18)
    EChaosVDKinematicTargetMode Mode;                                                 // 0x0048 (size: 0x4)

}; // Size: 0x50

struct FChaosVDManifoldPoint
{
    uint8 bDisabled;                                                                  // 0x0000 (size: 0x1)
    uint8 bWasRestored;                                                               // 0x0000 (size: 0x1)
    uint8 bWasReplaced;                                                               // 0x0000 (size: 0x1)
    uint8 bHasStaticFrictionAnchor;                                                   // 0x0000 (size: 0x1)
    uint8 bIsValid;                                                                   // 0x0000 (size: 0x1)
    uint8 bInsideStaticFrictionCone;                                                  // 0x0000 (size: 0x1)
    FVector NetPushOut;                                                               // 0x0008 (size: 0x18)
    FVector NetImpulse;                                                               // 0x0020 (size: 0x18)
    float TargetPhi;                                                                  // 0x0038 (size: 0x4)
    float InitialPhi;                                                                 // 0x003C (size: 0x4)
    FVector ShapeAnchorPoints;                                                        // 0x0040 (size: 0x30)
    FVector InitialShapeContactPoints;                                                // 0x0070 (size: 0x30)
    FChaosVDContactPoint ContactPoint;                                                // 0x00A0 (size: 0x58)
    FVector ShapeContactPoints;                                                       // 0x00F8 (size: 0x30)

}; // Size: 0x130

struct FChaosVDMultiSolverDebugShapeDataContainer
{
}; // Size: 0x50

struct FChaosVDParticleBounds : public FChaosVDWrapperDataBase
{
    FVector MMin;                                                                     // 0x0010 (size: 0x18)
    FVector MMax;                                                                     // 0x0028 (size: 0x18)

}; // Size: 0x40

struct FChaosVDParticleCluster : public FChaosVDWrapperDataBase
{
    int32 ParentParticleID;                                                           // 0x0010 (size: 0x4)
    int32 NumChildren;                                                                // 0x0014 (size: 0x4)
    FTransform ChildToParent;                                                         // 0x0020 (size: 0x60)
    int32 ClusterGroupIndex;                                                          // 0x0080 (size: 0x4)
    bool bInternalCluster;                                                            // 0x0084 (size: 0x1)
    float CollisionImpulse;                                                           // 0x0088 (size: 0x4)
    float ExternalStrains;                                                            // 0x008C (size: 0x4)
    float InternalStrains;                                                            // 0x0090 (size: 0x4)
    float Strain;                                                                     // 0x0094 (size: 0x4)
    TArray<FChaosVDConnectivityEdge> ConnectivityEdges;                               // 0x0098 (size: 0x10)
    bool bIsAnchored;                                                                 // 0x00A8 (size: 0x1)
    bool bUnbreakable;                                                                // 0x00A9 (size: 0x1)
    bool bIsChildToParentLocked;                                                      // 0x00AA (size: 0x1)

}; // Size: 0xB0

struct FChaosVDParticleDataWrapper : public FChaosVDWrapperDataBase
{
    int32 DirtyFlagsBits;                                                             // 0x0010 (size: 0x4)
    EChaosVDParticleContext ParticleContext;                                          // 0x0014 (size: 0x4)
    uint32 GeometryHash;                                                              // 0x0018 (size: 0x4)
    FString DebugName;                                                                // 0x0020 (size: 0x10)
    int32 ParticleIndex;                                                              // 0x0038 (size: 0x4)
    int32 SolverId;                                                                   // 0x003C (size: 0x4)
    EChaosVDParticleType Type;                                                        // 0x0040 (size: 0x1)
    FChaosVDParticlePositionRotation ParticlePositionRotation;                        // 0x0050 (size: 0x50)
    FChaosVDParticleVelocities ParticleVelocities;                                    // 0x00A0 (size: 0x40)
    FChaosVDParticleBounds ParticleInflatedBounds;                                    // 0x00E0 (size: 0x40)
    FChaosVDKinematicTarget ParticleKinematicTarget;                                  // 0x0120 (size: 0x50)
    FChaosVDVSmooth ParticleVWSmooth;                                                 // 0x0170 (size: 0x40)
    FChaosVDParticleDynamics ParticleDynamics;                                        // 0x01B0 (size: 0x70)
    FChaosVDParticleDynamicMisc ParticleDynamicsMisc;                                 // 0x0220 (size: 0x70)
    FChaosVDParticleMassProps ParticleMassProps;                                      // 0x0290 (size: 0x90)
    FChaosVDParticleCluster ParticleCluster;                                          // 0x0320 (size: 0xB0)
    TArray<FChaosVDShapeCollisionData> CollisionDataPerShape;                         // 0x03D0 (size: 0x10)

}; // Size: 0x3F0

struct FChaosVDParticleDynamicMisc : public FChaosVDWrapperDataBase
{
    double MLinearEtherDrag;                                                          // 0x0010 (size: 0x8)
    double MAngularEtherDrag;                                                         // 0x0018 (size: 0x8)
    double MMaxLinearSpeedSq;                                                         // 0x0020 (size: 0x8)
    double MMaxAngularSpeedSq;                                                        // 0x0028 (size: 0x8)
    float MInitialOverlapDepenetrationVelocity;                                       // 0x0030 (size: 0x4)
    float MSleepThresholdMultiplier;                                                  // 0x0034 (size: 0x4)
    int32 MCollisionGroup;                                                            // 0x0038 (size: 0x4)
    EChaosVDObjectStateType MObjectState;                                             // 0x003C (size: 0x1)
    EChaosVDSleepType MSleepType;                                                     // 0x003D (size: 0x1)
    uint32 MCollisionConstraintFlag;                                                  // 0x0040 (size: 0x4)
    FChaosVDFRigidParticleControlFlags MControlFlags;                                 // 0x0048 (size: 0x20)
    bool bDisabled;                                                                   // 0x0068 (size: 0x1)

}; // Size: 0x70

struct FChaosVDParticleDynamics : public FChaosVDWrapperDataBase
{
    FVector MAcceleration;                                                            // 0x0010 (size: 0x18)
    FVector MAngularAcceleration;                                                     // 0x0028 (size: 0x18)
    FVector MLinearImpulseVelocity;                                                   // 0x0040 (size: 0x18)
    FVector MAngularImpulseVelocity;                                                  // 0x0058 (size: 0x18)

}; // Size: 0x70

struct FChaosVDParticleMassProps : public FChaosVDWrapperDataBase
{
    FVector MCenterOfMass;                                                            // 0x0010 (size: 0x18)
    FQuat MRotationOfMass;                                                            // 0x0030 (size: 0x20)
    FVector MI;                                                                       // 0x0050 (size: 0x18)
    FVector MInvI;                                                                    // 0x0068 (size: 0x18)
    double MM;                                                                        // 0x0080 (size: 0x8)
    double MInvM;                                                                     // 0x0088 (size: 0x8)

}; // Size: 0x90

struct FChaosVDParticlePairMidPhase
{
    int32 SolverId;                                                                   // 0x0000 (size: 0x4)
    EChaosVDMidPhaseType MidPhaseType;                                                // 0x0004 (size: 0x1)
    uint8 bIsActive;                                                                  // 0x0005 (size: 0x1)
    uint8 bIsCCD;                                                                     // 0x0005 (size: 0x1)
    uint8 bIsCCDActive;                                                               // 0x0005 (size: 0x1)
    uint8 bIsSleeping;                                                                // 0x0005 (size: 0x1)
    uint8 bIsModified;                                                                // 0x0005 (size: 0x1)
    int32 LastUsedEpoch;                                                              // 0x0008 (size: 0x4)
    int32 Particle0Idx;                                                               // 0x000C (size: 0x4)
    int32 Particle1Idx;                                                               // 0x0010 (size: 0x4)
    TArray<FChaosVDConstraint> Constraints;                                           // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FChaosVDParticlePositionRotation : public FChaosVDWrapperDataBase
{
    FVector MX;                                                                       // 0x0010 (size: 0x18)
    FQuat MR;                                                                         // 0x0030 (size: 0x20)

}; // Size: 0x50

struct FChaosVDParticleVelocities : public FChaosVDWrapperDataBase
{
    FVector MV;                                                                       // 0x0010 (size: 0x18)
    FVector MW;                                                                       // 0x0028 (size: 0x18)

}; // Size: 0x40

struct FChaosVDQueryDataWrapper
{
    int32 ID;                                                                         // 0x0000 (size: 0x4)
    int32 ParentQueryID;                                                              // 0x0004 (size: 0x4)
    int32 WorldSolverID;                                                              // 0x0008 (size: 0x4)
    bool bIsRetryQuery;                                                               // 0x000C (size: 0x1)
    FQuat GeometryOrientation;                                                        // 0x0020 (size: 0x20)
    EChaosVDSceneQueryType Type;                                                      // 0x0040 (size: 0x4)
    EChaosVDSceneQueryMode Mode;                                                      // 0x0044 (size: 0x4)
    FVector StartLocation;                                                            // 0x0048 (size: 0x18)
    FVector EndLocation;                                                              // 0x0060 (size: 0x18)
    int32 CollisionChannel;                                                           // 0x0078 (size: 0x4)
    FChaosVDCollisionQueryParams CollisionQueryParams;                                // 0x0080 (size: 0x68)
    FChaosVDCollisionResponseParams CollisionResponseParams;                          // 0x00E8 (size: 0x40)
    FChaosVDCollisionObjectQueryParams CollisionObjectQueryParams;                    // 0x0128 (size: 0x18)
    TArray<FChaosVDQueryVisitStep> Hits;                                              // 0x0150 (size: 0x10)

}; // Size: 0x180

struct FChaosVDQueryFastData : public FChaosVDWrapperDataBase
{
    FVector Dir;                                                                      // 0x0010 (size: 0x18)
    FVector InvDir;                                                                   // 0x0028 (size: 0x18)
    double CurrentLength;                                                             // 0x0040 (size: 0x8)
    double InvCurrentLength;                                                          // 0x0048 (size: 0x8)
    uint8 bParallel0;                                                                 // 0x0050 (size: 0x1)
    uint8 bParallel1;                                                                 // 0x0050 (size: 0x1)
    uint8 bParallel2;                                                                 // 0x0050 (size: 0x1)

}; // Size: 0x58

struct FChaosVDQueryHitData : public FChaosVDWrapperDataBase
{
    float Distance;                                                                   // 0x0010 (size: 0x4)
    int32 FaceIdx;                                                                    // 0x0014 (size: 0x4)
    uint16 Flags;                                                                     // 0x0018 (size: 0x2)
    FVector WorldPosition;                                                            // 0x0020 (size: 0x18)
    FVector WorldNormal;                                                              // 0x0038 (size: 0x18)
    FVector FaceNormal;                                                               // 0x0050 (size: 0x18)

}; // Size: 0x68

struct FChaosVDQueryVisitStep : public FChaosVDWrapperDataBase
{
    EChaosVDSceneQueryVisitorType Type;                                               // 0x0014 (size: 0x4)
    uint32 ShapeIndex;                                                                // 0x0018 (size: 0x4)
    int32 ParticleIndex;                                                              // 0x001C (size: 0x4)
    FTransform ParticleTransform;                                                     // 0x0020 (size: 0x60)
    FChaosVDQueryFastData QueryFastData;                                              // 0x0080 (size: 0x58)
    EChaosVDCollisionQueryHitType HitType;                                            // 0x00D8 (size: 0x4)
    FChaosVDQueryHitData HitData;                                                     // 0x00E0 (size: 0x68)
    EChaosVDSQVisitRejectReason RejectReason;                                         // 0x0148 (size: 0x4)

}; // Size: 0x150

struct FChaosVDSceneQueriesDataContainer
{
}; // Size: 0xA0

struct FChaosVDShapeCollisionData
{
    EChaosVDCollisionTraceFlag CollisionTraceType;                                    // 0x0000 (size: 0x4)
    uint8 bSimCollision;                                                              // 0x0004 (size: 0x1)
    uint8 bQueryCollision;                                                            // 0x0004 (size: 0x1)
    uint8 bIsProbe;                                                                   // 0x0004 (size: 0x1)
    FChaosVDCollisionFilterData QueryData;                                            // 0x0008 (size: 0x10)
    FChaosVDCollisionFilterData SimData;                                              // 0x0018 (size: 0x10)
    bool bIsComplex;                                                                  // 0x0028 (size: 0x1)
    bool bIsValid;                                                                    // 0x0029 (size: 0x1)

}; // Size: 0x2C

struct FChaosVDTraceDetails
{
    FGuid TraceGuid;                                                                  // 0x0000 (size: 0x10)
    FGuid SessionGuid;                                                                // 0x0010 (size: 0x10)
    FString TraceTarget;                                                              // 0x0020 (size: 0x10)
    bool bIsConnected;                                                                // 0x0030 (size: 0x1)
    EChaosVDRecordingMode Mode;                                                       // 0x0031 (size: 0x1)

}; // Size: 0x38

struct FChaosVDVSmooth : public FChaosVDWrapperDataBase
{
    FVector MV;                                                                       // 0x0010 (size: 0x18)
    FVector MW;                                                                       // 0x0028 (size: 0x18)

}; // Size: 0x40

struct FChaosVDWrapperDataBase
{
    bool bHasValidData;                                                               // 0x0008 (size: 0x1)

}; // Size: 0x10

#endif
