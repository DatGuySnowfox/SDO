#ifndef UE4SS_SDK_Transient_HPP
#define UE4SS_SDK_Transient_HPP

struct FChaosDestructionEvent_SWC
{
    FVector3f Position;                                                               // 0x0000 (size: 0xC)
    FVector3f Normal;                                                                 // 0x000C (size: 0xC)
    FVector3f Velocity;                                                               // 0x0018 (size: 0xC)
    FVector3f AngularVelocity;                                                        // 0x0024 (size: 0xC)
    float ExtentMin;                                                                  // 0x0030 (size: 0x4)
    float ExtentMax;                                                                  // 0x0034 (size: 0x4)
    int32 ParticleID;                                                                 // 0x0038 (size: 0x4)
    float Time;                                                                       // 0x003C (size: 0x4)
    int32 Type;                                                                       // 0x0040 (size: 0x4)

}; // Size: 0x44

struct FPropertyBag_2955db3f1a2a6ee6
{
    FRigElementKey RigVMModel___GetTransform_Item__Const;                             // 0x0000 (size: 0xC)
    ERigVMTransformSpace RigVMModel___GetTransform_Space__Const;                      // 0x000C (size: 0x1)
    bool RigVMModel___GetTransform_bInitial__Const;                                   // 0x000D (size: 0x1)
    double RigVMModel___Greater_B__Const;                                             // 0x0010 (size: 0x8)
    FRigVMDebugDrawSettings RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_DebugDrawSettings__Const; // 0x0018 (size: 0x8)
    ERigUnitDebugTransformMode RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_Mode__Const; // 0x0020 (size: 0x1)
    FLinearColor RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_Color__Const; // 0x0024 (size: 0x10)
    float RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_Thickness__Const;   // 0x0034 (size: 0x4)
    float RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_Scale__Const;       // 0x0038 (size: 0x4)
    FTransform RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_WorldOffset__Const; // 0x0040 (size: 0x60)
    bool RigVMModel___RigVMFunction_DebugTransformMutableNoSpace_bEnabled__Const;     // 0x00A0 (size: 0x1)
    float RigVMModel___Set_Transform_Weight__Const;                                   // 0x00A4 (size: 0x4)

}; // Size: 0xA8

struct FPropertyBag_3001f7e8222352d
{
    bool RigVMModel___Greater_Result;                                                 // 0x0000 (size: 0x1)
    float RigVMModel___RigVMFunction_MathVectorLength_Result;                         // 0x0004 (size: 0x4)
    FVector RigVMModel___Subtract_Result;                                             // 0x0008 (size: 0x18)
    FVector RigVMModel___Subtract_A;                                                  // 0x0020 (size: 0x18)
    FTransform RigVMModel___GetTransform_Transform;                                   // 0x0040 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___GetTransform_CachedIndex;                  // 0x00A0 (size: 0x10)
    FVector RigVMModel___Subtract_B;                                                  // 0x00B0 (size: 0x18)
    double RigVMModel___Greater_A;                                                    // 0x00C8 (size: 0x8)
    FTransform RigVMModel___GetTransform_1_Transform;                                 // 0x00D0 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___GetTransform_1_CachedIndex;                // 0x0130 (size: 0x10)
    FTransform RigVMModel___GetTransform_2_Transform;                                 // 0x0140 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___GetTransform_2_CachedIndex;                // 0x01A0 (size: 0x10)
    FVector RigVMModel___RigVMFunction_DebugLineNoSpace_A;                            // 0x01B0 (size: 0x18)
    FTransform RigVMModel___GetTransform_3_Transform;                                 // 0x01D0 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___GetTransform_3_CachedIndex;                // 0x0230 (size: 0x10)
    FVector RigVMModel___RigVMFunction_DebugLineNoSpace_B;                            // 0x0240 (size: 0x18)
    FTransform RigVMModel___Set_Transform_Value__IO;                                  // 0x0260 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___Set_Transform_CachedIndex;                 // 0x02C0 (size: 0x10)
    FName RigVMModel___RigVMFunction_ControlFlowBranch_BlockToRun;                    // 0x02D0 (size: 0x8)
    FName RigVMModel___RigVMFunction_ControlFlowBranch_1_BlockToRun;                  // 0x02D8 (size: 0x8)

}; // Size: 0x2E0

#endif
