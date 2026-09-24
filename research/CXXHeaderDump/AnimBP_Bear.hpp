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
    EAnimSyncMethod __EnumProperty_60;                                                // 0x0024 (size: 0x1)
    bool __BoolProperty_61;                                                           // 0x0025 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_62;                              // 0x0026 (size: 0x1)
    FName __NameProperty_63;                                                          // 0x0028 (size: 0x8)
    FName __NameProperty_64;                                                          // 0x0030 (size: 0x8)
    FName __NameProperty_65;                                                          // 0x0038 (size: 0x8)
    int32 __IntProperty_66;                                                           // 0x0040 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_67;                                         // 0x0048 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0068 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00E8 (size: 0x40)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Root;                   // 0x0128 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_TransitionResult_1;     // 0x0158 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_TransitionResult;       // 0x0188 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_RandomPlayer;           // 0x01B8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_StateResult_1;          // 0x01E8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_BlendSpacePlayer;       // 0x0218 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_StateResult;            // 0x0248 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_StateMachine;           // 0x0278 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_SaveCachedPose;         // 0x02A8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_UseCachedPose_1;        // 0x02D8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_LayeredBoneBlend;       // 0x0308 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Slot_1;                 // 0x0338 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_UseCachedPose;          // 0x0368 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Slot;                   // 0x0398 (size: 0x30)

}; // Size: 0x3C8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_Bear_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03E0 (size: 0x8)
    FAnimBlueprintGeneratedMutableData __AnimBlueprintMutables;                       // 0x03E8 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x03F0 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x03F8 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0400 (size: 0x20)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_1;                      // 0x0420 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult;                        // 0x0448 (size: 0x28)
    FAnimNode_RandomPlayer AnimGraphNode_RandomPlayer;                                // 0x0470 (size: 0x78)
    FAnimNode_StateResult AnimGraphNode_StateResult_1;                                // 0x04E8 (size: 0x20)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer;                        // 0x0508 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult;                                  // 0x0578 (size: 0x20)
    FAnimNode_StateMachine AnimGraphNode_StateMachine;                                // 0x0598 (size: 0xC8)
    FAnimNode_SaveCachedPose AnimGraphNode_SaveCachedPose;                            // 0x0660 (size: 0x80)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose_1;                            // 0x06E0 (size: 0x28)
    FAnimNode_LayeredBoneBlend AnimGraphNode_LayeredBoneBlend;                        // 0x0708 (size: 0xE8)
    FAnimNode_Slot AnimGraphNode_Slot_1;                                              // 0x07F0 (size: 0x48)
    FAnimNode_UseCachedPose AnimGraphNode_UseCachedPose;                              // 0x0838 (size: 0x28)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x0860 (size: 0x48)
    FVector K2Node_PropertyAccess;                                                    // 0x08A8 (size: 0x18)
    double Speed;                                                                     // 0x08C0 (size: 0x8)
    class ABP_AnimalBear_C* BearRef;                                                  // 0x08C8 (size: 0x8)

    void AnimGraph(FPoseLink& AnimGraph);
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void GetSpeed();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Bear_AnimGraphNode_TransitionResult_6FF8E61C44B10C43491B7391AF555902();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Bear_AnimGraphNode_TransitionResult_77695E7E4A0F3E3F26DDF784F66F5884();
    void AnimNotify_Footstep();
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_Bear(int32 EntryPoint);
}; // Size: 0x8D0

#endif
