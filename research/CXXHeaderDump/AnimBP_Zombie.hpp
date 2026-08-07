#ifndef UE4SS_SDK_AnimBP_Zombie_HPP
#define UE4SS_SDK_AnimBP_Zombie_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_36;                                                          // 0x0004 (size: 0x8)
    bool __BoolProperty_37;                                                           // 0x000C (size: 0x1)
    float __FloatProperty_38;                                                         // 0x0010 (size: 0x4)
    float __FloatProperty_39;                                                         // 0x0014 (size: 0x4)
    bool __BoolProperty_40;                                                           // 0x0018 (size: 0x1)
    EAnimSyncMethod __EnumProperty_41;                                                // 0x0019 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_42;                              // 0x001A (size: 0x1)
    FName __NameProperty_43;                                                          // 0x001C (size: 0x8)
    FName __NameProperty_44;                                                          // 0x0024 (size: 0x8)
    int32 __IntProperty_45;                                                           // 0x002C (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_46;                                         // 0x0030 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0050 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00D0 (size: 0x18)

}; // Size: 0xE8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_Zombie_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    FAnimBlueprintGeneratedMutableData __AnimBlueprintMutables;                       // 0x0358 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0360 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0368 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0370 (size: 0x20)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer;                        // 0x0390 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult;                                  // 0x0400 (size: 0x20)
    FAnimNode_StateMachine AnimGraphNode_StateMachine;                                // 0x0420 (size: 0xC8)
    FAnimNode_SaveCachedPose AnimGraphNode_SaveCachedPose;                            // 0x04E8 (size: 0x80)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose_1;                            // 0x0568 (size: 0x28)
    FAnimNode_LayeredBoneBlend AnimGraphNode_LayeredBoneBlend;                        // 0x0590 (size: 0xF0)
    FAnimNode_Slot AnimGraphNode_Slot_1;                                              // 0x0680 (size: 0x48)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose;                              // 0x06C8 (size: 0x28)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x06F0 (size: 0x48)
    FVector K2Node_PropertyAccess_1;                                                  // 0x0738 (size: 0x18)
    bool K2Node_PropertyAccess;                                                       // 0x0750 (size: 0x1)
    bool Dead;                                                                        // 0x0751 (size: 0x1)
    double Speed;                                                                     // 0x0758 (size: 0x8)
    bool IsZombie?;                                                                   // 0x0760 (size: 0x1)
    bool Falling?;                                                                    // 0x0761 (size: 0x1)
    class UBlendSpace1D* Movement Blendspace;                                         // 0x0768 (size: 0x8)
    class ABP_MasterZombie_C* ZombieRef;                                              // 0x0770 (size: 0x8)

    void AnimGraph(FPoseLink& AnimGraph);
    void SetAnimations();
    void GetSpeed();
    void IsFalling?();
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void CombatState(int32 BlendSpace);
    void DeathState(bool Dead);
    void AnimNotify_Footstep();
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_Zombie(int32 EntryPoint);
}; // Size: 0x778

#endif
