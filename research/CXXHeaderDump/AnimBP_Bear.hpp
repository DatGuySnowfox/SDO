#ifndef UE4SS_SDK_AnimBP_Bear_HPP
#define UE4SS_SDK_AnimBP_Bear_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_54;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_55;                                                          // 0x000C (size: 0x8)
    int32 __IntProperty_56;                                                           // 0x0014 (size: 0x4)
    bool __BoolProperty_57;                                                           // 0x0018 (size: 0x1)
    float __FloatProperty_58;                                                         // 0x001C (size: 0x4)
    float __FloatProperty_59;                                                         // 0x0020 (size: 0x4)
    bool __BoolProperty_60;                                                           // 0x0024 (size: 0x1)
    EAnimSyncMethod __EnumProperty_61;                                                // 0x0025 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_62;                              // 0x0026 (size: 0x1)
    FName __NameProperty_63;                                                          // 0x0028 (size: 0x8)
    FName __NameProperty_64;                                                          // 0x0030 (size: 0x8)
    int32 __IntProperty_65;                                                           // 0x0038 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_66;                                         // 0x0040 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0060 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00E0 (size: 0x18)

}; // Size: 0xF8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_Bear_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    FAnimBlueprintGeneratedMutableData __AnimBlueprintMutables;                       // 0x0358 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0360 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0368 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0370 (size: 0x20)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_1;                      // 0x0390 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult;                        // 0x03B8 (size: 0x28)
    FAnimNode_RandomPlayer AnimGraphNode_RandomPlayer;                                // 0x03E0 (size: 0x78)
    FAnimNode_StateResult AnimGraphNode_StateResult_1;                                // 0x0458 (size: 0x20)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer;                        // 0x0478 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult;                                  // 0x04E8 (size: 0x20)
    FAnimNode_StateMachine AnimGraphNode_StateMachine;                                // 0x0508 (size: 0xC8)
    FAnimNode_SaveCachedPose AnimGraphNode_SaveCachedPose;                            // 0x05D0 (size: 0x80)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose_1;                            // 0x0650 (size: 0x28)
    FAnimNode_LayeredBoneBlend AnimGraphNode_LayeredBoneBlend;                        // 0x0678 (size: 0xF0)
    FAnimNode_Slot AnimGraphNode_Slot_1;                                              // 0x0768 (size: 0x48)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose;                              // 0x07B0 (size: 0x28)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x07D8 (size: 0x48)
    FVector K2Node_PropertyAccess;                                                    // 0x0820 (size: 0x18)
    double Speed;                                                                     // 0x0838 (size: 0x8)
    class ABP_AnimalBear_C* BearRef;                                                  // 0x0840 (size: 0x8)

    void AnimGraph(FPoseLink& AnimGraph);
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void GetSpeed();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Bear_AnimGraphNode_TransitionResult_6FF8E61C44B10C43491B7391AF555902();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Bear_AnimGraphNode_TransitionResult_77695E7E4A0F3E3F26DDF784F66F5884();
    void AnimNotify_Footstep();
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_Bear(int32 EntryPoint);
}; // Size: 0x848

#endif
