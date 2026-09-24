#ifndef UE4SS_SDK_MetaHumanSDKRuntime_HPP
#define UE4SS_SDK_MetaHumanSDKRuntime_HPP

#include "MetaHumanSDKRuntime_enums.hpp"

struct FMetaHumanCustomizableBodyPart
{
    TSubclassOf<class UControlRig> ControlRigClass;                                   // 0x0000 (size: 0x8)
    int32 ControlRigLODThreshold;                                                     // 0x0008 (size: 0x4)
    class UPhysicsAsset* PhysicsAsset;                                                // 0x0010 (size: 0x8)
    int32 RigidBodyLODThreshold;                                                      // 0x0018 (size: 0x4)
    FString ComponentName;                                                            // 0x0020 (size: 0x10)

}; // Size: 0x30

class UMetaHumanComponentBase : public UActorComponent
{
    FString BodyComponentName;                                                        // 0x00B8 (size: 0x10)
    EMetaHumanBodyType BodyType;                                                      // 0x00C8 (size: 0x1)
    bool bEnableBodyCorrectives;                                                      // 0x00C9 (size: 0x1)
    FString FaceComponentName;                                                        // 0x00D0 (size: 0x10)
    int32 RigLogicLODThreshold;                                                       // 0x00E0 (size: 0x4)
    bool bEnableNeckCorrectives;                                                      // 0x00E4 (size: 0x1)
    int32 NeckCorrectivesLODThreshold;                                                // 0x00E8 (size: 0x4)
    bool bEnableNeckProcControlRig;                                                   // 0x00EC (size: 0x1)
    int32 NeckProcControlRigLODThreshold;                                             // 0x00F0 (size: 0x4)
    FMetaHumanCustomizableBodyPart Torso;                                             // 0x00F8 (size: 0x30)
    FMetaHumanCustomizableBodyPart Legs;                                              // 0x0128 (size: 0x30)
    FMetaHumanCustomizableBodyPart Feet;                                              // 0x0158 (size: 0x30)

}; // Size: 0x188

class UMetaHumanComponentUE : public UMetaHumanComponentBase
{
    TSoftClassPtr<UAnimInstance> PostProcessAnimBP;                                   // 0x0188 (size: 0x28)

}; // Size: 0x1B0

#endif
