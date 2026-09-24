#ifndef UE4SS_SDK_IKRig_HPP
#define UE4SS_SDK_IKRig_HPP

#include "IKRig_enums.hpp"

struct FAnimNode_IKRig : public FAnimNode_CustomProperty
{
    FPoseLink Source;                                                                 // 0x0058 (size: 0x10)
    class UIKRigDefinition* RigDefinitionAsset;                                       // 0x0068 (size: 0x8)
    TArray<FIKRigGoal> Goals;                                                         // 0x0070 (size: 0x10)
    bool bStartFromRefPose;                                                           // 0x0080 (size: 0x1)
    EAnimAlphaInputType AlphaInputType;                                               // 0x0081 (size: 0x1)
    bool bAlphaBoolEnabled;                                                           // 0x0082 (size: 0x1)
    float Alpha;                                                                      // 0x0084 (size: 0x4)
    FInputScaleBias AlphaScaleBias;                                                   // 0x0088 (size: 0x8)
    FInputAlphaBoolBlend AlphaBoolBlend;                                              // 0x0090 (size: 0x48)
    FName AlphaCurveName;                                                             // 0x00D8 (size: 0x8)
    FInputScaleBiasClamp AlphaScaleBiasClamp;                                         // 0x00E0 (size: 0x30)
    TArray<UActorComponent*> GoalCreators;                                            // 0x0238 (size: 0x10)

}; // Size: 0x308

struct FAnimNode_RetargetPoseFromMesh : public FAnimNode_Base
{
    FPoseLink Source;                                                                 // 0x0010 (size: 0x10)
    ERetargetSourceMode RetargetFrom;                                                 // 0x0020 (size: 0x1)
    TWeakObjectPtr<class USkeletalMeshComponent> SourceMeshComponent;                 // 0x0024 (size: 0x8)
    class UIKRetargeter* IKRetargeterAsset;                                           // 0x0030 (size: 0x8)
    FRetargetProfile CustomRetargetProfile;                                           // 0x0038 (size: 0x128)
    int32 LODThreshold;                                                               // 0x0160 (size: 0x4)
    int32 LODThresholdForIK;                                                          // 0x0164 (size: 0x4)
    bool bSuppressWarnings;                                                           // 0x0168 (size: 0x1)
    bool bUseAttachedParent;                                                          // 0x0169 (size: 0x1)

}; // Size: 0x3D8

struct FBoneChain
{
    FName ChainName;                                                                  // 0x0000 (size: 0x8)
    FBoneReference StartBone;                                                         // 0x0008 (size: 0x10)
    FBoneReference EndBone;                                                           // 0x0018 (size: 0x10)
    FName IKGoalName;                                                                 // 0x0028 (size: 0x8)

}; // Size: 0x30

struct FCurveRemapPair
{
    FName SourceCurve;                                                                // 0x0000 (size: 0x8)
    FName TargetCurve;                                                                // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGoalBone
{
}; // Size: 0x10

struct FIKRetargetAdditivePoseOp : public FIKRetargetOpBase
{
    FIKRetargetAdditivePoseOpSettings Settings;                                       // 0x0028 (size: 0x20)

}; // Size: 0x48

struct FIKRetargetAdditivePoseOpSettings : public FIKRetargetOpSettingsBase
{
    FName PoseToApply;                                                                // 0x0010 (size: 0x8)
    float Alpha;                                                                      // 0x0018 (size: 0x4)

}; // Size: 0x20

struct FIKRetargetAlignPoleVectorOp : public FIKRetargetOpBase
{
    FIKRetargetAlignPoleVectorOpSettings Settings;                                    // 0x0020 (size: 0x28)
    FRetargetChainMapping ChainMapping;                                               // 0x0048 (size: 0x20)

}; // Size: 0x78

struct FIKRetargetAlignPoleVectorOpSettings : public FIKRetargetOpSettingsBase
{
    class UIKRigDefinition* IKRigAsset;                                               // 0x0010 (size: 0x8)
    TArray<FRetargetPoleVectorSettings> ChainsToAlign;                                // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FIKRetargetCopyBasePoseOp : public FIKRetargetOpBase
{
    FIKRetargetCopyBasePoseOpSettings Settings;                                       // 0x0020 (size: 0x20)

}; // Size: 0x40

struct FIKRetargetCopyBasePoseOpSettings : public FIKRetargetOpSettingsBase
{
    bool bCopyBasePose;                                                               // 0x0010 (size: 0x1)
    FName CopyBasePoseRoot;                                                           // 0x0014 (size: 0x8)

}; // Size: 0x20

struct FIKRetargetCurveRemapOp : public FIKRetargetOpBase
{
    FIKRetargetCurveRemapOpSettings Settings;                                         // 0x0020 (size: 0x28)

}; // Size: 0x68

struct FIKRetargetCurveRemapOpSettings : public FIKRetargetOpSettingsBase
{
    bool bCopyAllSourceCurves;                                                        // 0x0010 (size: 0x1)
    bool bRemapCurves;                                                                // 0x0011 (size: 0x1)
    TArray<FCurveRemapPair> CurvesToRemap;                                            // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FIKRetargetFKChainsOp : public FIKRetargetOpBase
{
    FIKRetargetFKChainsOpSettings Settings;                                           // 0x0020 (size: 0x40)
    FRetargetChainMapping ChainMapping;                                               // 0x0060 (size: 0x20)

}; // Size: 0xB0

struct FIKRetargetFKChainsOpSettings : public FIKRetargetOpSettingsBase
{
    class UIKRigDefinition* IKRigAsset;                                               // 0x0010 (size: 0x8)
    TArray<FRetargetFKChainSettings> ChainsToRetarget;                                // 0x0018 (size: 0x10)
    bool bDrawChainLines;                                                             // 0x0028 (size: 0x1)
    bool bDrawSingleBoneChains;                                                       // 0x0029 (size: 0x1)
    double ChainDrawThickness;                                                        // 0x0030 (size: 0x8)
    double ChainDrawSize;                                                             // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FIKRetargetIKChainsOp : public FIKRetargetOpBase
{
    FIKRetargetIKChainsOpSettings Settings;                                           // 0x0020 (size: 0x38)

}; // Size: 0x68

struct FIKRetargetIKChainsOpSettings : public FIKRetargetOpSettingsBase
{
    TArray<FRetargetIKChainSettings> ChainsToRetarget;                                // 0x0010 (size: 0x10)
    bool bDrawFinalGoals;                                                             // 0x0020 (size: 0x1)
    bool bDrawSourceLocations;                                                        // 0x0021 (size: 0x1)
    double GoalDrawSize;                                                              // 0x0028 (size: 0x8)
    double GoalDrawThickness;                                                         // 0x0030 (size: 0x8)

}; // Size: 0x38

struct FIKRetargetOpBase
{
    bool bIsEnabled;                                                                  // 0x0008 (size: 0x1)
    FName Name;                                                                       // 0x000C (size: 0x8)
    FName ParentOpName;                                                               // 0x0014 (size: 0x8)

}; // Size: 0x20

struct FIKRetargetOpSettingsBase
{
}; // Size: 0x10

struct FIKRetargetPelvisMotionOp : public FIKRetargetOpBase
{
    FIKRetargetPelvisMotionOpSettings Settings;                                       // 0x0020 (size: 0xC8)

}; // Size: 0x240

struct FIKRetargetPelvisMotionOpSettings : public FIKRetargetOpSettingsBase
{
    FBoneReference SourcePelvisBone;                                                  // 0x0010 (size: 0x10)
    FBoneReference TargetPelvisBone;                                                  // 0x0020 (size: 0x10)
    double RotationAlpha;                                                             // 0x0030 (size: 0x8)
    FRotator RotationOffset;                                                          // 0x0038 (size: 0x18)
    double TranslationAlpha;                                                          // 0x0050 (size: 0x8)
    FVector TranslationOffset;                                                        // 0x0058 (size: 0x18)
    double BlendToSourceTranslation;                                                  // 0x0070 (size: 0x8)
    FVector BlendToSourceTranslationWeights;                                          // 0x0078 (size: 0x18)
    double ScaleHorizontal;                                                           // 0x0090 (size: 0x8)
    double ScaleVertical;                                                             // 0x0098 (size: 0x8)
    double AffectIKHorizontal;                                                        // 0x00A0 (size: 0x8)
    double AffectIKVertical;                                                          // 0x00A8 (size: 0x8)
    bool bEnableDebugDraw;                                                            // 0x00B0 (size: 0x1)
    double DebugDrawSize;                                                             // 0x00B8 (size: 0x8)
    double DebugDrawThickness;                                                        // 0x00C0 (size: 0x8)

}; // Size: 0xC8

struct FIKRetargetPinBoneOp : public FIKRetargetOpBase
{
    FIKRetargetPinBoneOpSettings Settings;                                            // 0x0020 (size: 0xF0)

}; // Size: 0x120

struct FIKRetargetPinBoneOpSettings : public FIKRetargetOpSettingsBase
{
    TArray<FPinBoneData> BonesToPin;                                                  // 0x0010 (size: 0x10)
    ERetargetSourceOrTarget SkeletonToCopyFrom;                                       // 0x0020 (size: 0x1)
    bool bCopyTranslation;                                                            // 0x0021 (size: 0x1)
    EPinBoneTranslationMode TranslationMode;                                          // 0x0022 (size: 0x1)
    bool bCopyRotation;                                                               // 0x0023 (size: 0x1)
    EPinBoneRotationMode RotationMode;                                                // 0x0024 (size: 0x1)
    bool bCopyScale;                                                                  // 0x0025 (size: 0x1)
    bool bPropagateToChildren;                                                        // 0x0026 (size: 0x1)
    FTransform GlobalOffset;                                                          // 0x0030 (size: 0x60)
    FTransform LocalOffset;                                                           // 0x0090 (size: 0x60)

}; // Size: 0xF0

struct FIKRetargetPose
{
    FVector RootTranslationOffset;                                                    // 0x0000 (size: 0x18)
    TMap<FName, FQuat> BoneRotationOffsets;                                           // 0x0018 (size: 0x50)

}; // Size: 0x70

struct FIKRetargetRootMotionOp : public FIKRetargetOpBase
{
    FIKRetargetRootMotionOpSettings Settings;                                         // 0x0020 (size: 0xC0)

}; // Size: 0x280

struct FIKRetargetRootMotionOpSettings : public FIKRetargetOpSettingsBase
{
    FBoneReference SourceRoot;                                                        // 0x0010 (size: 0x10)
    FBoneReference TargetRoot;                                                        // 0x0020 (size: 0x10)
    ERootMotionSource RootMotionSource;                                               // 0x0030 (size: 0x1)
    FBoneReference TargetPelvis;                                                      // 0x0034 (size: 0x10)
    bool bRotateWithPelvis;                                                           // 0x0044 (size: 0x1)
    ERootMotionHeightSource RootHeightSource;                                         // 0x0045 (size: 0x1)
    FTransform GlobalOffset;                                                          // 0x0050 (size: 0x60)
    bool bMaintainOffsetFromPelvis;                                                   // 0x00B0 (size: 0x1)
    bool bPropagateToNonRetargetedChildren;                                           // 0x00B1 (size: 0x1)

}; // Size: 0xC0

struct FIKRetargetRunIKRigOp : public FIKRetargetOpBase
{
    FIKRetargetRunIKRigOpSettings Settings;                                           // 0x0020 (size: 0x40)
    FRetargetChainMapping ChainMapping;                                               // 0x0060 (size: 0x20)

}; // Size: 0x1A8

struct FIKRetargetRunIKRigOpSettings : public FIKRetargetOpSettingsBase
{
    class UIKRigDefinition* IKRigAsset;                                               // 0x0010 (size: 0x8)
    TArray<FName> ExcludedGoals;                                                      // 0x0018 (size: 0x10)
    bool bDrawGoals;                                                                  // 0x0028 (size: 0x1)
    bool bDrawGoalBoneLocations;                                                      // 0x0029 (size: 0x1)
    double GoalDrawSize;                                                              // 0x0030 (size: 0x8)
    double GoalDrawThickness;                                                         // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FIKRetargetScaleSourceOp : public FIKRetargetOpBase
{
    FIKRetargetScaleSourceOpSettings Settings;                                        // 0x0020 (size: 0x18)

}; // Size: 0x38

struct FIKRetargetScaleSourceOpSettings : public FIKRetargetOpSettingsBase
{
    double SourceScaleFactor;                                                         // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FIKRetargetSpeedPlantingOp : public FIKRetargetOpBase
{
    FIKRetargetSpeedPlantingOpSettings Settings;                                      // 0x0020 (size: 0x38)

}; // Size: 0x70

struct FIKRetargetSpeedPlantingOpSettings : public FIKRetargetOpSettingsBase
{
    TArray<FRetargetSpeedPlantingSettings> ChainsToSpeedPlant;                        // 0x0010 (size: 0x10)
    double SpeedThreshold;                                                            // 0x0020 (size: 0x8)
    double Stiffness;                                                                 // 0x0028 (size: 0x8)
    double CriticalDamping;                                                           // 0x0030 (size: 0x8)

}; // Size: 0x38

struct FIKRetargetStrideWarpingOp : public FIKRetargetOpBase
{
    FIKRetargetStrideWarpingOpSettings Settings;                                      // 0x0020 (size: 0x60)

}; // Size: 0x98

struct FIKRetargetStrideWarpingOpSettings : public FIKRetargetOpSettingsBase
{
    TArray<FRetargetStrideWarpChainSettings> ChainSettings;                           // 0x0010 (size: 0x10)
    EWarpingDirectionSource DirectionSource;                                          // 0x0020 (size: 0x4)
    EBasicAxis ForwardDirection;                                                      // 0x0024 (size: 0x4)
    FName DirectionChain;                                                             // 0x0028 (size: 0x8)
    double WarpForwards;                                                              // 0x0030 (size: 0x8)
    double SidewaysOffset;                                                            // 0x0038 (size: 0x8)
    double WarpSplay;                                                                 // 0x0040 (size: 0x8)
    bool bEnableDebugDraw;                                                            // 0x0048 (size: 0x1)
    double DebugDrawSize;                                                             // 0x0050 (size: 0x8)
    double DebugDrawThickness;                                                        // 0x0058 (size: 0x8)

}; // Size: 0x60

struct FIKRigBodyMoverGoalSettings : public FIKRigGoalSettingsBase
{
    FName BoneName;                                                                   // 0x0010 (size: 0x8)
    float InfluenceMultiplier;                                                        // 0x0018 (size: 0x4)

}; // Size: 0x20

struct FIKRigBodyMoverSettings : public FIKRigSolverSettingsBase
{
    FName StartBone;                                                                  // 0x0008 (size: 0x8)
    float PositionAlpha;                                                              // 0x0010 (size: 0x4)
    float PositionPositiveX;                                                          // 0x0014 (size: 0x4)
    float PositionNegativeX;                                                          // 0x0018 (size: 0x4)
    float PositionPositiveY;                                                          // 0x001C (size: 0x4)
    float PositionNegativeY;                                                          // 0x0020 (size: 0x4)
    float PositionPositiveZ;                                                          // 0x0024 (size: 0x4)
    float PositionNegativeZ;                                                          // 0x0028 (size: 0x4)
    float RotationAlpha;                                                              // 0x002C (size: 0x4)
    float RotateXAlpha;                                                               // 0x0030 (size: 0x4)
    float RotateYAlpha;                                                               // 0x0034 (size: 0x4)
    float RotateZAlpha;                                                               // 0x0038 (size: 0x4)

}; // Size: 0x40

struct FIKRigBodyMoverSolver : public FIKRigSolverBase
{
    FIKRigBodyMoverSettings Settings;                                                 // 0x0010 (size: 0x40)
    TArray<FIKRigBodyMoverGoalSettings> AllGoalSettings;                              // 0x0050 (size: 0x10)

}; // Size: 0x68

struct FIKRigBoneSettingsBase : public FIKRigSettingsBase
{
    FName bone;                                                                       // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FIKRigFBIKBoneSettings : public FIKRigBoneSettingsBase
{
    float RotationStiffness;                                                          // 0x0010 (size: 0x4)
    float PositionStiffness;                                                          // 0x0014 (size: 0x4)
    EPBIKLimitType X;                                                                 // 0x0018 (size: 0x1)
    float MinX;                                                                       // 0x001C (size: 0x4)
    float MaxX;                                                                       // 0x0020 (size: 0x4)
    EPBIKLimitType Y;                                                                 // 0x0024 (size: 0x1)
    float MinY;                                                                       // 0x0028 (size: 0x4)
    float MaxY;                                                                       // 0x002C (size: 0x4)
    EPBIKLimitType Z;                                                                 // 0x0030 (size: 0x1)
    float MinZ;                                                                       // 0x0034 (size: 0x4)
    float MaxZ;                                                                       // 0x0038 (size: 0x4)
    bool bUsePreferredAngles;                                                         // 0x003C (size: 0x1)
    FVector PreferredAngles;                                                          // 0x0040 (size: 0x18)

}; // Size: 0x58

struct FIKRigFBIKGoalSettings : public FIKRigGoalSettingsBase
{
    FName BoneName;                                                                   // 0x0010 (size: 0x8)
    int32 ChainDepth;                                                                 // 0x0018 (size: 0x4)
    float StrengthAlpha;                                                              // 0x001C (size: 0x4)
    float PullChainAlpha;                                                             // 0x0020 (size: 0x4)
    float PinRotation;                                                                // 0x0024 (size: 0x4)
    int32 IndexInSolver;                                                              // 0x0028 (size: 0x4)

}; // Size: 0x30

struct FIKRigFBIKSettings : public FIKRigSolverSettingsBase
{
    FName RootBone;                                                                   // 0x0008 (size: 0x8)
    int32 Iterations;                                                                 // 0x0010 (size: 0x4)
    int32 SubIterations;                                                              // 0x0014 (size: 0x4)
    float MassMultiplier;                                                             // 0x0018 (size: 0x4)
    bool bAllowStretch;                                                               // 0x001C (size: 0x1)
    EPBIKRootBehavior RootBehavior;                                                   // 0x001D (size: 0x1)
    FRootPrePullSettings PrePullRootSettings;                                         // 0x0020 (size: 0x20)
    float GlobalPullChainAlpha;                                                       // 0x0040 (size: 0x4)
    float MaxAngle;                                                                   // 0x0044 (size: 0x4)
    float OverRelaxation;                                                             // 0x0048 (size: 0x4)

}; // Size: 0x50

struct FIKRigFullBodyIKSolver : public FIKRigSolverBase
{
    FIKRigFBIKSettings Settings;                                                      // 0x0010 (size: 0x50)
    TArray<FIKRigFBIKGoalSettings> AllGoalSettings;                                   // 0x0060 (size: 0x10)
    TArray<FIKRigFBIKBoneSettings> AllBoneSettings;                                   // 0x0070 (size: 0x10)

}; // Size: 0xD8

struct FIKRigGoal
{
    FName Name;                                                                       // 0x0000 (size: 0x8)
    FName BoneName;                                                                   // 0x0008 (size: 0x8)
    EIKRigGoalTransformSource TransformSource;                                        // 0x0010 (size: 0x1)
    FBoneReference SourceBone;                                                        // 0x0014 (size: 0x10)
    FVector Position;                                                                 // 0x0028 (size: 0x18)
    FRotator Rotation;                                                                // 0x0040 (size: 0x18)
    float PositionAlpha;                                                              // 0x0058 (size: 0x4)
    float RotationAlpha;                                                              // 0x005C (size: 0x4)
    EIKRigGoalSpace PositionSpace;                                                    // 0x0060 (size: 0x1)
    EIKRigGoalSpace RotationSpace;                                                    // 0x0061 (size: 0x1)
    FVector FinalBlendedPosition;                                                     // 0x0068 (size: 0x18)
    FQuat FinalBlendedRotation;                                                       // 0x0080 (size: 0x20)
    bool bEnabled;                                                                    // 0x00A0 (size: 0x1)

}; // Size: 0xB0

struct FIKRigGoalContainer
{
}; // Size: 0x18

struct FIKRigGoalSettingsBase : public FIKRigSettingsBase
{
    FName Goal;                                                                       // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FIKRigInputSkeleton
{
}; // Size: 0x38

struct FIKRigLimbSolver : public FIKRigSolverBase
{
    FIKRigLimbSolverSettings Settings;                                                // 0x0010 (size: 0x48)

}; // Size: 0x80

struct FIKRigLimbSolverSettings : public FLimbSolverSettings
{
    FName StartBone;                                                                  // 0x0030 (size: 0x8)
    FName GoalName;                                                                   // 0x0038 (size: 0x8)
    FName EndBone;                                                                    // 0x0040 (size: 0x8)

}; // Size: 0x48

struct FIKRigPoleSolver : public FIKRigSolverBase
{
    FIKRigPoleSolverSettings Settings;                                                // 0x0010 (size: 0x28)

}; // Size: 0x58

struct FIKRigPoleSolverSettings : public FIKRigSolverSettingsBase
{
    FName StartBone;                                                                  // 0x0008 (size: 0x8)
    FName EndBone;                                                                    // 0x0010 (size: 0x8)
    FName AimAtGoal;                                                                  // 0x0018 (size: 0x8)
    float Alpha;                                                                      // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FIKRigSetTransform : public FIKRigSolverBase
{
    FIKRigSetTransformSettings Settings;                                              // 0x0010 (size: 0x28)

}; // Size: 0x40

struct FIKRigSetTransformSettings : public FIKRigSolverSettingsBase
{
    FName Goal;                                                                       // 0x0008 (size: 0x8)
    FName BoneToAffect;                                                               // 0x0010 (size: 0x8)
    float PositionAlpha;                                                              // 0x0018 (size: 0x4)
    float RotationAlpha;                                                              // 0x001C (size: 0x4)
    float Alpha;                                                                      // 0x0020 (size: 0x4)
    bool bPropagateToChildren;                                                        // 0x0024 (size: 0x1)

}; // Size: 0x28

struct FIKRigSettingsBase
{
}; // Size: 0x8

struct FIKRigSkeleton
{
    TArray<FName> BoneNames;                                                          // 0x0000 (size: 0x10)
    TArray<int32> ParentIndices;                                                      // 0x0010 (size: 0x10)
    TArray<FName> ExcludedBones;                                                      // 0x0020 (size: 0x10)
    TArray<FTransform> CurrentPoseGlobal;                                             // 0x0030 (size: 0x10)
    TArray<FTransform> CurrentPoseLocal;                                              // 0x0040 (size: 0x10)
    TArray<FTransform> RefPoseGlobal;                                                 // 0x0050 (size: 0x10)

}; // Size: 0x70

struct FIKRigSolverBase
{
    bool bIsEnabled;                                                                  // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FIKRigSolverSettingsBase : public FIKRigSettingsBase
{
}; // Size: 0x8

struct FLimbLink
{
}; // Size: 0x68

struct FLimbSolver
{
}; // Size: 0x18

struct FLimbSolverSettings : public FIKRigSolverSettingsBase
{
    float ReachPrecision;                                                             // 0x0008 (size: 0x4)
    TEnumAsByte<EAxis::Type> HingeRotationAxis;                                       // 0x000C (size: 0x1)
    int32 MaxIterations;                                                              // 0x0010 (size: 0x4)
    bool bEnableLimit;                                                                // 0x0014 (size: 0x1)
    float MinRotationAngle;                                                           // 0x0018 (size: 0x4)
    bool bAveragePull;                                                                // 0x001C (size: 0x1)
    float PullDistribution;                                                           // 0x0020 (size: 0x4)
    float ReachStepAlpha;                                                             // 0x0024 (size: 0x4)
    bool bEnableTwistCorrection;                                                      // 0x0028 (size: 0x1)
    TEnumAsByte<EAxis::Type> EndBoneForwardAxis;                                      // 0x0029 (size: 0x1)

}; // Size: 0x30

struct FPinBoneData
{
    FBoneReference BoneToCopyFrom;                                                    // 0x0000 (size: 0x10)
    FBoneReference BoneToCopyTo;                                                      // 0x0010 (size: 0x10)
    FName BoneToPin;                                                                  // 0x0150 (size: 0x8)
    FName BoneToPinTo;                                                                // 0x0158 (size: 0x8)

}; // Size: 0x160

struct FRetargetChainMapping
{
    TArray<FRetargetChainPair> ChainMap;                                              // 0x0000 (size: 0x10)
    class UIKRigDefinition* SourceIKRig;                                              // 0x0010 (size: 0x8)
    class UIKRigDefinition* TargetIKRig;                                              // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FRetargetChainPair
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    FName SourceChainName;                                                            // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FRetargetDefinition
{
    FName RootBone;                                                                   // 0x0000 (size: 0x8)
    TArray<FBoneChain> BoneChains;                                                    // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FRetargetFKChainSettings
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    bool EnableFK;                                                                    // 0x0008 (size: 0x1)
    EFKChainRotationMode RotationMode;                                                // 0x0009 (size: 0x1)
    double RotationAlpha;                                                             // 0x0010 (size: 0x8)
    EFKChainTranslationMode TranslationMode;                                          // 0x0018 (size: 0x1)
    double TranslationAlpha;                                                          // 0x0020 (size: 0x8)

}; // Size: 0x28

struct FRetargetGlobalSettings
{
    bool bEnableRoot;                                                                 // 0x0000 (size: 0x1)
    bool bEnableFK;                                                                   // 0x0001 (size: 0x1)
    bool bEnableIK;                                                                   // 0x0002 (size: 0x1)
    bool bEnablePost;                                                                 // 0x0003 (size: 0x1)
    bool bCopyBasePose;                                                               // 0x0004 (size: 0x1)
    FName CopyBasePoseRoot;                                                           // 0x0008 (size: 0x8)
    float SourceScaleFactor;                                                          // 0x0010 (size: 0x4)
    bool bWarping;                                                                    // 0x0014 (size: 0x1)
    EWarpingDirectionSource DirectionSource;                                          // 0x0018 (size: 0x4)
    EBasicAxis ForwardDirection;                                                      // 0x001C (size: 0x4)
    FName DirectionChain;                                                             // 0x0020 (size: 0x8)
    float WarpForwards;                                                               // 0x0028 (size: 0x4)
    float SidewaysOffset;                                                             // 0x002C (size: 0x4)
    float WarpSplay;                                                                  // 0x0030 (size: 0x4)

}; // Size: 0x34

struct FRetargetIKChainSettings
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    bool EnableIK;                                                                    // 0x0008 (size: 0x1)
    double BlendToSource;                                                             // 0x0010 (size: 0x8)
    double BlendToSourceTranslation;                                                  // 0x0018 (size: 0x8)
    double BlendToSourceRotation;                                                     // 0x0020 (size: 0x8)
    FVector BlendToSourceWeights;                                                     // 0x0028 (size: 0x18)
    FVector StaticOffset;                                                             // 0x0040 (size: 0x18)
    FVector StaticLocalOffset;                                                        // 0x0058 (size: 0x18)
    FRotator StaticRotationOffset;                                                    // 0x0070 (size: 0x18)
    double ScaleVertical;                                                             // 0x0088 (size: 0x8)
    double Extension;                                                                 // 0x0090 (size: 0x8)

}; // Size: 0x98

struct FRetargetOpProfile
{
    FName OpToApplySettingsTo;                                                        // 0x0000 (size: 0x8)
    FInstancedStruct SettingsToApply;                                                 // 0x0008 (size: 0x10)

}; // Size: 0x20

struct FRetargetPoleVectorSettings
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    bool bEnabled;                                                                    // 0x0008 (size: 0x1)
    double AlignAlpha;                                                                // 0x0010 (size: 0x8)
    double StaticAngularOffset;                                                       // 0x0018 (size: 0x8)
    bool MaintainOffset;                                                              // 0x0020 (size: 0x1)

}; // Size: 0x28

struct FRetargetProfile
{
    TArray<FRetargetOpProfile> RetargetOpProfiles;                                    // 0x0000 (size: 0x10)
    bool bApplyTargetRetargetPose;                                                    // 0x0010 (size: 0x1)
    FName TargetRetargetPoseName;                                                     // 0x0014 (size: 0x8)
    bool bApplySourceRetargetPose;                                                    // 0x001C (size: 0x1)
    FName SourceRetargetPoseName;                                                     // 0x0020 (size: 0x8)
    bool bForceAllIKOff;                                                              // 0x0028 (size: 0x1)
    bool bApplyChainSettings;                                                         // 0x0029 (size: 0x1)
    TMap<FName, FTargetChainSettings> ChainSettings;                                  // 0x0030 (size: 0x50)
    bool bApplyRootSettings;                                                          // 0x0080 (size: 0x1)
    FTargetRootSettings RootSettings;                                                 // 0x0088 (size: 0x68)
    bool bApplyGlobalSettings;                                                        // 0x00F0 (size: 0x1)
    FRetargetGlobalSettings GlobalSettings;                                           // 0x00F4 (size: 0x34)

}; // Size: 0x128

struct FRetargetSpeedPlantingSettings
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    FName SpeedCurveName;                                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FRetargetStrideWarpChainSettings
{
    FName TargetChainName;                                                            // 0x0000 (size: 0x8)
    bool EnableStrideWarping;                                                         // 0x0008 (size: 0x1)

}; // Size: 0xC

struct FTargetChainFKSettings
{
    bool EnableFK;                                                                    // 0x0000 (size: 0x1)
    ERetargetRotationMode RotationMode;                                               // 0x0001 (size: 0x1)
    float RotationAlpha;                                                              // 0x0004 (size: 0x4)
    ERetargetTranslationMode TranslationMode;                                         // 0x0008 (size: 0x1)
    float TranslationAlpha;                                                           // 0x000C (size: 0x4)
    float PoleVectorMatching;                                                         // 0x0010 (size: 0x4)
    bool PoleVectorMaintainOffset;                                                    // 0x0014 (size: 0x1)
    float PoleVectorOffset;                                                           // 0x0018 (size: 0x4)

}; // Size: 0x1C

struct FTargetChainIKSettings
{
    bool EnableIK;                                                                    // 0x0000 (size: 0x1)
    float BlendToSource;                                                              // 0x0004 (size: 0x4)
    float BlendToSourceTranslation;                                                   // 0x0008 (size: 0x4)
    float BlendToSourceRotation;                                                      // 0x000C (size: 0x4)
    FVector BlendToSourceWeights;                                                     // 0x0010 (size: 0x18)
    FVector StaticOffset;                                                             // 0x0028 (size: 0x18)
    FVector StaticLocalOffset;                                                        // 0x0040 (size: 0x18)
    FRotator StaticRotationOffset;                                                    // 0x0058 (size: 0x18)
    float ScaleVertical;                                                              // 0x0070 (size: 0x4)
    float Extension;                                                                  // 0x0074 (size: 0x4)
    bool bAffectedByIKWarping;                                                        // 0x0078 (size: 0x1)

}; // Size: 0x80

struct FTargetChainSettings
{
    FTargetChainFKSettings FK;                                                        // 0x0000 (size: 0x1C)
    FTargetChainIKSettings IK;                                                        // 0x0020 (size: 0x80)
    FTargetChainSpeedPlantSettings SpeedPlanting;                                     // 0x00A0 (size: 0x18)

}; // Size: 0xB8

struct FTargetChainSpeedPlantSettings
{
    bool EnableSpeedPlanting;                                                         // 0x0000 (size: 0x1)
    FName SpeedCurveName;                                                             // 0x0004 (size: 0x8)
    float SpeedThreshold;                                                             // 0x000C (size: 0x4)
    float UnplantStiffness;                                                           // 0x0010 (size: 0x4)
    float UnplantCriticalDamping;                                                     // 0x0014 (size: 0x4)

}; // Size: 0x18

struct FTargetRootSettings
{
    float RotationAlpha;                                                              // 0x0000 (size: 0x4)
    float TranslationAlpha;                                                           // 0x0004 (size: 0x4)
    float BlendToSource;                                                              // 0x0008 (size: 0x4)
    FVector BlendToSourceWeights;                                                     // 0x0010 (size: 0x18)
    float ScaleHorizontal;                                                            // 0x0028 (size: 0x4)
    float ScaleVertical;                                                              // 0x002C (size: 0x4)
    FVector TranslationOffset;                                                        // 0x0030 (size: 0x18)
    FRotator RotationOffset;                                                          // 0x0048 (size: 0x18)
    float AffectIKHorizontal;                                                         // 0x0060 (size: 0x4)
    float AffectIKVertical;                                                           // 0x0064 (size: 0x4)

}; // Size: 0x68

class IIKGoalCreatorInterface : public IInterface
{

    void AddIKGoals(TMap<FName, FIKRigGoal>& OutGoals);
}; // Size: 0x28

class UCurveRemapOp : public URetargetOpBase
{
    TArray<FCurveRemapPair> CurvesToRemap;                                            // 0x0030 (size: 0x10)
    bool bCopyAllSourceCurves;                                                        // 0x0040 (size: 0x1)

}; // Size: 0x48

class UIKRetargetAdditivePoseController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetAdditivePoseOpSettings InSettings);
    FIKRetargetAdditivePoseOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetAlignPoleVectorController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetAlignPoleVectorOpSettings InSettings);
    FIKRetargetAlignPoleVectorOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetCopyBasePoseController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetCopyBasePoseOpSettings InSettings);
    FIKRetargetCopyBasePoseOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetCurveRemapController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetCurveRemapOpSettings InSettings);
    FIKRetargetCurveRemapOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetFKChainsController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetFKChainsOpSettings InSettings);
    FIKRetargetFKChainsOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetGlobalSettings : public UObject
{
    FRetargetGlobalSettings Settings;                                                 // 0x0028 (size: 0x34)

}; // Size: 0x60

class UIKRetargetIKChainsController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetIKChainsOpSettings InSettings);
    FIKRetargetIKChainsOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetOpControllerBase : public UObject
{
}; // Size: 0x30

class UIKRetargetPelvisMotionController : public UIKRetargetOpControllerBase
{

    void SetTargetPelvisBone(const FName InTargetPelvisBone);
    void SetSourcePelvisBone(const FName InSourcePelvisBone);
    void SetSettings(FIKRetargetPelvisMotionOpSettings InSettings);
    FName GetTargetPelvisBone();
    FName GetSourcePelvisBone();
    FIKRetargetPelvisMotionOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetPinBoneController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetPinBoneOpSettings InSettings);
    void SetBonePair(const FName InBoneToCopyFrom, const FName InBoneToCopyTo);
    FIKRetargetPinBoneOpSettings GetSettings();
    TMap<FName, FName> GetAllBonePairs();
    void ClearAllBonePairs();
}; // Size: 0x30

class UIKRetargetProcessor : public UObject
{
}; // Size: 0x2D8

class UIKRetargetRootMotionController : public UIKRetargetOpControllerBase
{

    void SetTargetRootBone(const FName InTargetRootBone);
    void SetTargetPelvisBone(const FName InTargetPelvisBone);
    void SetSourceRootBone(const FName InSourceRootBone);
    void SetSettings(FIKRetargetRootMotionOpSettings InSettings);
    FName GetTargetRootBone();
    FName GetTargetPelvisBone();
    FName GetSourceRootBone();
    FIKRetargetRootMotionOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetRunIKRigController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetRunIKRigOpSettings InSettings);
    FIKRetargetRunIKRigOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetScaleSourceController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetScaleSourceOpSettings InSettings);
    FIKRetargetScaleSourceOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetSpeedPlantingController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetSpeedPlantingOpSettings InSettings);
    FIKRetargetSpeedPlantingOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargetStrideWarpingController : public UIKRetargetOpControllerBase
{

    void SetSettings(FIKRetargetStrideWarpingOpSettings InSettings);
    FIKRetargetStrideWarpingOpSettings GetSettings();
}; // Size: 0x30

class UIKRetargeter : public UObject
{
    int32 Version;                                                                    // 0x0028 (size: 0x4)
    class UIKRigDefinition* SourceIKRigAsset;                                         // 0x0030 (size: 0x8)
    class UIKRigDefinition* TargetIKRigAsset;                                         // 0x0038 (size: 0x8)
    TArray<FInstancedStruct> RetargetOps;                                             // 0x0040 (size: 0x10)
    TMap<FName, FRetargetProfile> Profiles;                                           // 0x0050 (size: 0x50)
    FName CurrentProfile;                                                             // 0x00A0 (size: 0x8)
    TMap<FName, FIKRetargetPose> SourceRetargetPoses;                                 // 0x00A8 (size: 0x50)
    TMap<FName, FIKRetargetPose> TargetRetargetPoses;                                 // 0x00F8 (size: 0x50)
    FName CurrentSourceRetargetPose;                                                  // 0x0148 (size: 0x8)
    FName CurrentTargetRetargetPose;                                                  // 0x0150 (size: 0x8)
    bool bRetargetRoot;                                                               // 0x0158 (size: 0x1)
    bool bRetargetFK;                                                                 // 0x0159 (size: 0x1)
    bool bRetargetIK;                                                                 // 0x015A (size: 0x1)
    float TargetActorOffset;                                                          // 0x015C (size: 0x4)
    float TargetActorScale;                                                           // 0x0160 (size: 0x4)
    TMap<FName, FIKRetargetPose> RetargetPoses;                                       // 0x0168 (size: 0x50)
    FName CurrentRetargetPose;                                                        // 0x01B8 (size: 0x8)
    FRetargetChainMapping ChainMap;                                                   // 0x01C0 (size: 0x20)
    TArray<URetargetChainSettings*> ChainSettings;                                    // 0x01E0 (size: 0x10)
    class URetargetRootSettings* RootSettings;                                        // 0x01F0 (size: 0x8)
    class UIKRetargetGlobalSettings* GlobalSettings;                                  // 0x01F8 (size: 0x8)
    class URetargetOpStack* OpStack;                                                  // 0x0200 (size: 0x8)

    void SetRootSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FTargetRootSettings& RootSettings);
    void SetGlobalSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FRetargetGlobalSettings& GlobalSettings);
    void SetChainSpeedPlantSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FTargetChainSpeedPlantSettings& SpeedPlantSettings, const FName TargetChainName);
    void SetChainSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FTargetChainSettings& ChainSettings, const FName TargetChainName);
    void SetChainIKSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FTargetChainIKSettings& IKSettings, const FName TargetChainName);
    void SetChainFKSettingsInRetargetProfile(FRetargetProfile& RetargetProfile, const FTargetChainFKSettings& FKSettings, const FName TargetChainName);
    bool HasTargetIKRig();
    bool HasSourceIKRig();
    FTargetRootSettings GetRootSettingsFromRetargetProfile(FRetargetProfile& RetargetProfile);
    void GetRootSettingsFromRetargetAsset(const class UIKRetargeter* RetargetAsset, const FName OptionalProfileName, FTargetRootSettings& OutSettings);
    FRetargetGlobalSettings GetGlobalSettingsFromRetargetProfile(FRetargetProfile& RetargetProfile);
    void GetGlobalSettingsFromRetargetAsset(const class UIKRetargeter* RetargetAsset, const FName OptionalProfileName, FRetargetGlobalSettings& OutSettings);
    FTargetChainSettings GetChainUsingGoalFromRetargetAsset(const class UIKRetargeter* RetargetAsset, const FName IKGoalName);
    FTargetChainSettings GetChainSettingsFromRetargetProfile(FRetargetProfile& RetargetProfile, const FName TargetChainName);
    FTargetChainSettings GetChainSettingsFromRetargetAsset(const class UIKRetargeter* RetargetAsset, const FName TargetChainName, const FName OptionalProfileName);
}; // Size: 0x208

class UIKRigBodyMoverController : public UIKRigSolverControllerBase
{

    void SetSolverSettings(FIKRigBodyMoverSettings InSettings);
    void SetGoalSettings(const FName InGoalName, FIKRigBodyMoverGoalSettings InSettings);
    FIKRigBodyMoverSettings GetSolverSettings();
    FIKRigBodyMoverGoalSettings GetGoalSettings(const FName InGoalName);
}; // Size: 0x30

class UIKRigComponent : public UActorComponent
{

    void SetIKRigGoalTransform(const FName GoalName, const FTransform Transform, const float PositionAlpha, const float RotationAlpha);
    void SetIKRigGoalPositionAndRotation(const FName GoalName, const FVector Position, const FQuat Rotation, const float PositionAlpha, const float RotationAlpha);
    void SetIKRigGoal(const FIKRigGoal& Goal);
    void ClearAllGoals();
}; // Size: 0xD8

class UIKRigDefinition : public UObject
{
    TSoftObjectPtr<class USkeletalMesh> PreviewSkeletalMesh;                          // 0x0030 (size: 0x28)
    FIKRigSkeleton Skeleton;                                                          // 0x0058 (size: 0x70)
    TArray<UIKRigEffectorGoal*> Goals;                                                // 0x00C8 (size: 0x10)
    TArray<FInstancedStruct> SolverStack;                                             // 0x00D8 (size: 0x10)
    FRetargetDefinition RetargetDefinition;                                           // 0x00E8 (size: 0x18)
    TArray<UIKRigSolver*> Solvers;                                                    // 0x0100 (size: 0x10)

}; // Size: 0x110

class UIKRigEffectorGoal : public UObject
{
    FName GoalName;                                                                   // 0x0028 (size: 0x8)
    FName BoneName;                                                                   // 0x0030 (size: 0x8)
    float PositionAlpha;                                                              // 0x0038 (size: 0x4)
    float RotationAlpha;                                                              // 0x003C (size: 0x4)
    FTransform CurrentTransform;                                                      // 0x0040 (size: 0x60)
    FTransform InitialTransform;                                                      // 0x00A0 (size: 0x60)

}; // Size: 0x100

class UIKRigFBIKController : public UIKRigSolverControllerBase
{

    void SetSolverSettings(FIKRigFBIKSettings InSettings);
    void SetGoalSettings(const FName InGoalName, FIKRigFBIKGoalSettings InSettings);
    void SetBoneSettings(const FName InBoneName, FIKRigFBIKBoneSettings InSettings);
    FIKRigFBIKSettings GetSolverSettings();
    FIKRigFBIKGoalSettings GetGoalSettings(const FName InGoalName);
    FIKRigFBIKBoneSettings GetBoneSettings(const FName InBoneName);
}; // Size: 0x30

class UIKRigFBIKSolver : public UIKRigSolver
{
    FName RootBone;                                                                   // 0x0030 (size: 0x8)
    int32 Iterations;                                                                 // 0x0038 (size: 0x4)
    int32 SubIterations;                                                              // 0x003C (size: 0x4)
    float MassMultiplier;                                                             // 0x0040 (size: 0x4)
    bool bAllowStretch;                                                               // 0x0044 (size: 0x1)
    EPBIKRootBehavior RootBehavior;                                                   // 0x0045 (size: 0x1)
    FRootPrePullSettings PrePullRootSettings;                                         // 0x0048 (size: 0x20)
    float PullChainAlpha;                                                             // 0x0068 (size: 0x4)
    float MaxAngle;                                                                   // 0x006C (size: 0x4)
    float OverRelaxation;                                                             // 0x0070 (size: 0x4)
    TArray<UIKRig_FBIKEffector*> Effectors;                                           // 0x0078 (size: 0x10)
    TArray<UIKRig_FBIKBoneSettings*> BoneSettings;                                    // 0x0088 (size: 0x10)

    TArray<UIKRig_FBIKEffector*> GetEffectors();
    TArray<UIKRig_FBIKBoneSettings*> GetBoneSettings();
}; // Size: 0x98

class UIKRigLimbSolverController : public UIKRigSolverControllerBase
{

    void SetSolverSettings(FIKRigLimbSolverSettings InSettings);
    FIKRigLimbSolverSettings GetSolverSettings();
}; // Size: 0x30

class UIKRigPoleSolverController : public UIKRigSolverControllerBase
{

    void SetSolverSettings(FIKRigPoleSolverSettings InSettings);
    FIKRigPoleSolverSettings GetSolverSettings();
}; // Size: 0x30

class UIKRigProcessor : public UObject
{
}; // Size: 0x188

class UIKRigSetTransformController : public UIKRigSolverControllerBase
{

    void SetSolverSettings(FIKRigSetTransformSettings InSettings);
    FIKRigSetTransformSettings GetSolverSettings();
}; // Size: 0x30

class UIKRigSolver : public UObject
{
    bool bIsEnabled;                                                                  // 0x0028 (size: 0x1)

}; // Size: 0x30

class UIKRigSolverControllerBase : public UObject
{

    void SetEnabled(bool bIsEnabled);
    bool GetEnabled();
}; // Size: 0x30

class UIKRig_BodyMover : public UIKRigSolver
{
    FName RootBone;                                                                   // 0x0030 (size: 0x8)
    float PositionAlpha;                                                              // 0x0038 (size: 0x4)
    float PositionPositiveX;                                                          // 0x003C (size: 0x4)
    float PositionNegativeX;                                                          // 0x0040 (size: 0x4)
    float PositionPositiveY;                                                          // 0x0044 (size: 0x4)
    float PositionNegativeY;                                                          // 0x0048 (size: 0x4)
    float PositionPositiveZ;                                                          // 0x004C (size: 0x4)
    float PositionNegativeZ;                                                          // 0x0050 (size: 0x4)
    float RotationAlpha;                                                              // 0x0054 (size: 0x4)
    float RotateXAlpha;                                                               // 0x0058 (size: 0x4)
    float RotateYAlpha;                                                               // 0x005C (size: 0x4)
    float RotateZAlpha;                                                               // 0x0060 (size: 0x4)
    TArray<UIKRig_BodyMoverEffector*> Effectors;                                      // 0x0068 (size: 0x10)

}; // Size: 0x78

class UIKRig_BodyMoverEffector : public UObject
{
    FName GoalName;                                                                   // 0x0028 (size: 0x8)
    FName BoneName;                                                                   // 0x0030 (size: 0x8)
    float InfluenceMultiplier;                                                        // 0x0038 (size: 0x4)

}; // Size: 0x40

class UIKRig_FBIKBoneSettings : public UObject
{
    FName bone;                                                                       // 0x0028 (size: 0x8)
    float RotationStiffness;                                                          // 0x0030 (size: 0x4)
    float PositionStiffness;                                                          // 0x0034 (size: 0x4)
    EPBIKLimitType X;                                                                 // 0x0038 (size: 0x1)
    float MinX;                                                                       // 0x003C (size: 0x4)
    float MaxX;                                                                       // 0x0040 (size: 0x4)
    EPBIKLimitType Y;                                                                 // 0x0044 (size: 0x1)
    float MinY;                                                                       // 0x0048 (size: 0x4)
    float MaxY;                                                                       // 0x004C (size: 0x4)
    EPBIKLimitType Z;                                                                 // 0x0050 (size: 0x1)
    float MinZ;                                                                       // 0x0054 (size: 0x4)
    float MaxZ;                                                                       // 0x0058 (size: 0x4)
    bool bUsePreferredAngles;                                                         // 0x005C (size: 0x1)
    FVector PreferredAngles;                                                          // 0x0060 (size: 0x18)

}; // Size: 0x78

class UIKRig_FBIKEffector : public UObject
{
    FName GoalName;                                                                   // 0x0028 (size: 0x8)
    FName BoneName;                                                                   // 0x0030 (size: 0x8)
    int32 ChainDepth;                                                                 // 0x0038 (size: 0x4)
    float StrengthAlpha;                                                              // 0x003C (size: 0x4)
    float PullChainAlpha;                                                             // 0x0040 (size: 0x4)
    float PinRotation;                                                                // 0x0044 (size: 0x4)

}; // Size: 0x48

class UIKRig_LimbEffector : public UObject
{
    FName GoalName;                                                                   // 0x0028 (size: 0x8)
    FName BoneName;                                                                   // 0x0030 (size: 0x8)

}; // Size: 0x38

class UIKRig_LimbSolver : public UIKRigSolver
{
    FName RootName;                                                                   // 0x0030 (size: 0x8)
    float ReachPrecision;                                                             // 0x0038 (size: 0x4)
    TEnumAsByte<EAxis::Type> HingeRotationAxis;                                       // 0x003C (size: 0x1)
    int32 MaxIterations;                                                              // 0x0040 (size: 0x4)
    bool bEnableLimit;                                                                // 0x0044 (size: 0x1)
    float MinRotationAngle;                                                           // 0x0048 (size: 0x4)
    bool bAveragePull;                                                                // 0x004C (size: 0x1)
    float PullDistribution;                                                           // 0x0050 (size: 0x4)
    float ReachStepAlpha;                                                             // 0x0054 (size: 0x4)
    bool bEnableTwistCorrection;                                                      // 0x0058 (size: 0x1)
    TEnumAsByte<EAxis::Type> EndBoneForwardAxis;                                      // 0x0059 (size: 0x1)
    class UIKRig_LimbEffector* Effector;                                              // 0x0060 (size: 0x8)

}; // Size: 0x68

class UIKRig_PoleSolver : public UIKRigSolver
{
    FName RootName;                                                                   // 0x0030 (size: 0x8)
    FName EndName;                                                                    // 0x0038 (size: 0x8)
    class UIKRig_PoleSolverEffector* Effector;                                        // 0x0040 (size: 0x8)

}; // Size: 0x48

class UIKRig_PoleSolverEffector : public UObject
{
    FName GoalName;                                                                   // 0x0028 (size: 0x8)
    FName BoneName;                                                                   // 0x0030 (size: 0x8)
    float Alpha;                                                                      // 0x0038 (size: 0x4)

}; // Size: 0x40

class UIKRig_SetTransform : public UIKRigSolver
{
    FName Goal;                                                                       // 0x0030 (size: 0x8)
    FName RootBone;                                                                   // 0x0038 (size: 0x8)
    class UIKRig_SetTransformEffector* Effector;                                      // 0x0040 (size: 0x8)

}; // Size: 0x48

class UIKRig_SetTransformEffector : public UObject
{
    bool bEnablePosition;                                                             // 0x0028 (size: 0x1)
    bool bEnableRotation;                                                             // 0x0029 (size: 0x1)
    float Alpha;                                                                      // 0x002C (size: 0x4)

}; // Size: 0x30

class UPinBoneOp : public URetargetOpBase
{
    TArray<FPinBoneData> BonesToPin;                                                  // 0x0030 (size: 0x10)
    ERetargetSourceOrTarget PinTo;                                                    // 0x0040 (size: 0x1)
    bool bCopyTranslation;                                                            // 0x0041 (size: 0x1)
    bool bCopyRotation;                                                               // 0x0042 (size: 0x1)
    bool bCopyScale;                                                                  // 0x0043 (size: 0x1)
    EPinBoneTranslationMode TranslationMode;                                          // 0x0044 (size: 0x1)
    EPinBoneRotationMode RotationMode;                                                // 0x0045 (size: 0x1)
    FTransform GlobalOffset;                                                          // 0x0050 (size: 0x60)
    FTransform LocalOffset;                                                           // 0x00B0 (size: 0x60)
    bool bMaintainOffset;                                                             // 0x0110 (size: 0x1)
    EPinBoneType PinType;                                                             // 0x0111 (size: 0x1)

}; // Size: 0x120

class URetargetChainSettings : public UObject
{
    FName SourceChain;                                                                // 0x0028 (size: 0x8)
    FName TargetChain;                                                                // 0x0030 (size: 0x8)
    FTargetChainSettings Settings;                                                    // 0x0038 (size: 0xB8)
    bool CopyPoseUsingFK;                                                             // 0x00F0 (size: 0x1)
    ERetargetRotationMode RotationMode;                                               // 0x00F1 (size: 0x1)
    float RotationAlpha;                                                              // 0x00F4 (size: 0x4)
    ERetargetTranslationMode TranslationMode;                                         // 0x00F8 (size: 0x1)
    float TranslationAlpha;                                                           // 0x00FC (size: 0x4)
    bool DriveIKGoal;                                                                 // 0x0100 (size: 0x1)
    float BlendToSource;                                                              // 0x0104 (size: 0x4)
    FVector BlendToSourceWeights;                                                     // 0x0108 (size: 0x18)
    FVector StaticOffset;                                                             // 0x0120 (size: 0x18)
    FVector StaticLocalOffset;                                                        // 0x0138 (size: 0x18)
    FRotator StaticRotationOffset;                                                    // 0x0150 (size: 0x18)
    float Extension;                                                                  // 0x0168 (size: 0x4)
    bool UseSpeedCurveToPlantIK;                                                      // 0x016C (size: 0x1)
    FName SpeedCurveName;                                                             // 0x0170 (size: 0x8)
    float VelocityThreshold;                                                          // 0x0178 (size: 0x4)
    float UnplantStiffness;                                                           // 0x017C (size: 0x4)
    float UnplantCriticalDamping;                                                     // 0x0180 (size: 0x4)

}; // Size: 0x188

class URetargetOpBase : public UObject
{
    bool bIsEnabled;                                                                  // 0x0028 (size: 0x1)

}; // Size: 0x30

class URetargetOpStack : public UObject
{
    TArray<URetargetOpBase*> RetargetOps;                                             // 0x0028 (size: 0x10)

}; // Size: 0x38

class URetargetProfileLibrary : public UBlueprintFunctionLibrary
{

    class UIKRetargetOpControllerBase* GetOpControllerFromRetargetProfile(FRetargetProfile& InRetargetProfile, const FName InRetargetOpName);
    FRetargetProfile CopyRetargetProfileFromRetargetAsset(const class UIKRetargeter* InRetargetAsset);
}; // Size: 0x28

class URetargetRootSettings : public UObject
{
    FTargetRootSettings Settings;                                                     // 0x0028 (size: 0x68)
    bool RetargetRootTranslation;                                                     // 0x0090 (size: 0x1)
    float GlobalScaleHorizontal;                                                      // 0x0094 (size: 0x4)
    float GlobalScaleVertical;                                                        // 0x0098 (size: 0x4)
    FVector BlendToSource;                                                            // 0x00A0 (size: 0x18)
    FVector StaticOffset;                                                             // 0x00B8 (size: 0x18)
    FRotator StaticRotationOffset;                                                    // 0x00D0 (size: 0x18)

}; // Size: 0xE8

class URootMotionGeneratorOp : public URetargetOpBase
{
    FName SourceRootBone;                                                             // 0x0030 (size: 0x8)
    FName TargetRootBone;                                                             // 0x0038 (size: 0x8)
    FName TargetPelvisBone;                                                           // 0x0040 (size: 0x8)
    ERootMotionSource RootMotionSource;                                               // 0x0048 (size: 0x1)
    ERootMotionHeightSource RootHeightSource;                                         // 0x0049 (size: 0x1)
    bool bPropagateToNonRetargetedChildren;                                           // 0x004A (size: 0x1)
    bool bMaintainOffsetFromPelvis;                                                   // 0x004B (size: 0x1)
    bool bRotateWithPelvis;                                                           // 0x004C (size: 0x1)
    FTransform GlobalOffset;                                                          // 0x0050 (size: 0x60)

}; // Size: 0xB0

#endif
