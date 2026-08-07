#ifndef UE4SS_SDK_PBIK_HPP
#define UE4SS_SDK_PBIK_HPP

#include "PBIK_enums.hpp"

struct FPBIKBoneSetting
{
    FName bone;                                                                       // 0x0000 (size: 0x8)
    float RotationStiffness;                                                          // 0x0008 (size: 0x4)
    float PositionStiffness;                                                          // 0x000C (size: 0x4)
    EPBIKLimitType X;                                                                 // 0x0010 (size: 0x1)
    float MinX;                                                                       // 0x0014 (size: 0x4)
    float MaxX;                                                                       // 0x0018 (size: 0x4)
    EPBIKLimitType Y;                                                                 // 0x001C (size: 0x1)
    float MinY;                                                                       // 0x0020 (size: 0x4)
    float MaxY;                                                                       // 0x0024 (size: 0x4)
    EPBIKLimitType Z;                                                                 // 0x0028 (size: 0x1)
    float MinZ;                                                                       // 0x002C (size: 0x4)
    float MaxZ;                                                                       // 0x0030 (size: 0x4)
    bool bUsePreferredAngles;                                                         // 0x0034 (size: 0x1)
    FVector PreferredAngles;                                                          // 0x0038 (size: 0x18)

}; // Size: 0x50

struct FPBIKDebug
{
    float DrawScale;                                                                  // 0x0000 (size: 0x4)
    bool bDrawDebug;                                                                  // 0x0004 (size: 0x1)

}; // Size: 0x8

struct FPBIKEffector
{
    FName bone;                                                                       // 0x0000 (size: 0x8)
    FTransform Transform;                                                             // 0x0010 (size: 0x60)
    float PositionAlpha;                                                              // 0x0070 (size: 0x4)
    float RotationAlpha;                                                              // 0x0074 (size: 0x4)
    float StrengthAlpha;                                                              // 0x0078 (size: 0x4)
    float PullChainAlpha;                                                             // 0x007C (size: 0x4)
    float PinRotation;                                                                // 0x0080 (size: 0x4)

}; // Size: 0x90

struct FPBIKSolver
{
}; // Size: 0x68

struct FPBIKSolverSettings
{
    int32 Iterations;                                                                 // 0x0000 (size: 0x4)
    float MassMultiplier;                                                             // 0x0004 (size: 0x4)
    bool bAllowStretch;                                                               // 0x0008 (size: 0x1)
    EPBIKRootBehavior RootBehavior;                                                   // 0x0009 (size: 0x1)
    FRootPrePullSettings PrePullRootSettings;                                         // 0x000C (size: 0x20)
    float GlobalPullChainAlpha;                                                       // 0x002C (size: 0x4)
    float MaxAngle;                                                                   // 0x0030 (size: 0x4)
    float OverRelaxation;                                                             // 0x0034 (size: 0x4)
    bool bStartSolveFromInputPose;                                                    // 0x0038 (size: 0x1)

}; // Size: 0x3C

struct FPBIKWorkData
{
    bool bNeedsInit;                                                                  // 0x0000 (size: 0x1)
    uint32 HashInitializedWith;                                                       // 0x0004 (size: 0x4)
    TArray<int32> BoneSettingToSolverBoneIndex;                                       // 0x0008 (size: 0x10)
    TArray<int32> SolverBoneToElementIndex;                                           // 0x0018 (size: 0x10)
    FPBIKSolver Solver;                                                               // 0x0028 (size: 0x68)

}; // Size: 0x90

struct FRigUnit_PBIK : public FRigUnit_HighlevelBaseMutable
{
    FName Root;                                                                       // 0x0160 (size: 0x8)
    TArray<FPBIKEffector> Effectors;                                                  // 0x0168 (size: 0x10)
    TArray<int32> EffectorSolverIndices;                                              // 0x0178 (size: 0x10)
    TArray<FPBIKBoneSetting> BoneSettings;                                            // 0x0188 (size: 0x10)
    TArray<FName> ExcludedBones;                                                      // 0x0198 (size: 0x10)
    FPBIKSolverSettings Settings;                                                     // 0x01A8 (size: 0x3C)
    FPBIKDebug Debug;                                                                 // 0x01E4 (size: 0x8)
    FPBIKWorkData WorkData;                                                           // 0x01F0 (size: 0x90)

}; // Size: 0x280

struct FRootPrePullSettings
{
    float RotationAlpha;                                                              // 0x0000 (size: 0x4)
    float RotationAlphaX;                                                             // 0x0004 (size: 0x4)
    float RotationAlphaY;                                                             // 0x0008 (size: 0x4)
    float RotationAlphaZ;                                                             // 0x000C (size: 0x4)
    float PositionAlpha;                                                              // 0x0010 (size: 0x4)
    float PositionAlphaX;                                                             // 0x0014 (size: 0x4)
    float PositionAlphaY;                                                             // 0x0018 (size: 0x4)
    float PositionAlphaZ;                                                             // 0x001C (size: 0x4)

}; // Size: 0x20

#endif
