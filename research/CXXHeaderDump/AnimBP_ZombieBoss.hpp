#ifndef UE4SS_SDK_AnimBP_ZombieBoss_HPP
#define UE4SS_SDK_AnimBP_ZombieBoss_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_36;                                                          // 0x0004 (size: 0x8)
    bool __BoolProperty_37;                                                           // 0x000C (size: 0x1)
    float __FloatProperty_38;                                                         // 0x0010 (size: 0x4)
    float __FloatProperty_39;                                                         // 0x0014 (size: 0x4)
    EAnimSyncMethod __EnumProperty_40;                                                // 0x0018 (size: 0x1)
    bool __BoolProperty_41;                                                           // 0x0019 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_42;                              // 0x001A (size: 0x1)
    FName __NameProperty_43;                                                          // 0x001C (size: 0x8)
    FName __NameProperty_44;                                                          // 0x0024 (size: 0x8)
    FName __NameProperty_45;                                                          // 0x002C (size: 0x8)
    int32 __IntProperty_46;                                                           // 0x0034 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_47;                                         // 0x0038 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0058 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00D8 (size: 0x40)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Root;                   // 0x0118 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_UseCachedPose_1;        // 0x0148 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_LayeredBoneBlend;       // 0x0178 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Slot_1;                 // 0x01A8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_UseCachedPose;          // 0x01D8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Slot;                   // 0x0208 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_SaveCachedPose;         // 0x0238 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_BlendSpacePlayer;       // 0x0268 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_StateResult;            // 0x0298 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_StateMachine;           // 0x02C8 (size: 0x30)

}; // Size: 0x2F8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_ZombieBoss_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03E0 (size: 0x8)
    FAnimBlueprintGeneratedMutableData __AnimBlueprintMutables;                       // 0x03E8 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x03F0 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x03F8 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0400 (size: 0x20)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose_1;                            // 0x0420 (size: 0x28)
    FAnimNode_LayeredBoneBlend AnimGraphNode_LayeredBoneBlend;                        // 0x0448 (size: 0xE8)
    FAnimNode_Slot AnimGraphNode_Slot_1;                                              // 0x0530 (size: 0x48)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose;                              // 0x0578 (size: 0x28)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x05A0 (size: 0x48)
    FAnimNode_SaveCachedPose AnimGraphNode_SaveCachedPose;                            // 0x05E8 (size: 0x80)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer;                        // 0x0668 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult;                                  // 0x06D8 (size: 0x20)
    FAnimNode_StateMachine AnimGraphNode_StateMachine;                                // 0x06F8 (size: 0xC8)
    FVector K2Node_PropertyAccess_1;                                                  // 0x07C0 (size: 0x18)
    bool K2Node_PropertyAccess;                                                       // 0x07D8 (size: 0x1)
    bool Falling?;                                                                    // 0x07D9 (size: 0x1)
    double Speed;                                                                     // 0x07E0 (size: 0x8)
    bool Dead;                                                                        // 0x07E8 (size: 0x1)

    void AnimGraph(FPoseLink& AnimGraph);
    void GetSpeed();
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void IsFalling?();
    void CombatState(int32 BlendSpace);
    void AnimNotify_Footstep();
    void DeathState(bool Dead);
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_ZombieBoss(int32 EntryPoint);
}; // Size: 0x7E9

#endif
