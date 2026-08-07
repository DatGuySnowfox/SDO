#ifndef UE4SS_SDK_ControlRig_Player_HPP
#define UE4SS_SDK_ControlRig_Player_HPP

class UControlRig_Player_C : public UControlRig
{
    FQuat HeadRotation;                                                               // 0x0610 (size: 0x20)
    bool InVehicle;                                                                   // 0x0630 (size: 0x1)

}; // Size: 0x631

class URigVMMemory_Literal : public URigVMMemoryStorage
{
    FRigElementKey RigVMModel___OffsetTransformForItem_1_1_Item__Const;               // 0x0028 (size: 0xC)
    FTransform RigVMModel___OffsetTransformForItem_1_1_OffsetTransform__Const;        // 0x0040 (size: 0x60)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_MakeStruct_Elements__Const;          // 0x00A0 (size: 0x18)
    double RigVMModel___Multiply_B__Const;                                            // 0x00B8 (size: 0x8)
    float RigVMModel___OffsetTransformForItem_1_1_Weight__Const;                      // 0x00C0 (size: 0x4)
    bool RigVMModel___OffsetTransformForItem_1_1_bPropagateToChildren__Const;         // 0x00C4 (size: 0x1)
    FRigElementKey RigVMModel___OffsetTransformForItem_1_1_1_1_Item__Const;           // 0x00C8 (size: 0xC)
    float RigVMModel___OffsetTransformForItem_1_1_1_1_Weight__Const;                  // 0x00D4 (size: 0x4)

}; // Size: 0xD8

class URigVMMemory_Work : public URigVMMemoryStorage
{
    FQuat RigVMModel___Cast_Result;                                                   // 0x0030 (size: 0x20)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_MakeStruct_Struct;                   // 0x0050 (size: 0x18)
    double RigVMModel___Multiply_Result;                                              // 0x0068 (size: 0x8)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_BreakStruct_Elements;                // 0x0070 (size: 0x18)
    FRotator RigVMModel___Cast_1_Result;                                              // 0x0088 (size: 0x18)
    double RigVMModel___Multiply_A;                                                   // 0x00A0 (size: 0x8)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_MakeStruct_Elements__IO;             // 0x00A8 (size: 0x18)
    FTransform RigVMModel___OffsetTransformForItem_1_1_OffsetTransform__IO;           // 0x00C0 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___OffsetTransformForItem_1_1_CachedIndex;    // 0x0120 (size: 0x10)
    FQuat RigVMModel___Cast_2_1_Result;                                               // 0x0130 (size: 0x20)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_MakeStruct_1_1_Struct;               // 0x0150 (size: 0x18)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_BreakStruct_1_1_Elements;            // 0x0168 (size: 0x18)
    FRotator RigVMModel___Cast_1_1_1_Result;                                          // 0x0180 (size: 0x18)
    FRotator RigVMModel___DISPATCH_RigVMDispatch_MakeStruct_1_1_Elements__IO;         // 0x0198 (size: 0x18)
    FTransform RigVMModel___OffsetTransformForItem_1_1_1_1_OffsetTransform__IO;       // 0x01B0 (size: 0x60)
    TArray<FCachedRigElement> RigVMModel___OffsetTransformForItem_1_1_1_1_CachedIndex; // 0x0210 (size: 0x10)

}; // Size: 0x220

#endif
