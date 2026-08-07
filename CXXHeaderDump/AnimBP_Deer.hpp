#ifndef UE4SS_SDK_AnimBP_Deer_HPP
#define UE4SS_SDK_AnimBP_Deer_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_39;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_40;                                                          // 0x000C (size: 0x8)
    int32 __IntProperty_41;                                                           // 0x0014 (size: 0x4)
    bool __BoolProperty_42;                                                           // 0x0018 (size: 0x1)
    float __FloatProperty_43;                                                         // 0x001C (size: 0x4)
    float __FloatProperty_44;                                                         // 0x0020 (size: 0x4)
    bool __BoolProperty_45;                                                           // 0x0024 (size: 0x1)
    EAnimSyncMethod __EnumProperty_46;                                                // 0x0025 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_47;                              // 0x0026 (size: 0x1)
    FName __NameProperty_48;                                                          // 0x0028 (size: 0x8)
    FName __NameProperty_49;                                                          // 0x0030 (size: 0x8)
    int32 __IntProperty_50;                                                           // 0x0038 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_51;                                         // 0x0040 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0060 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00E0 (size: 0x18)

}; // Size: 0xF8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_Deer_C : public UAnimInstance
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
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x05D0 (size: 0x48)
    FVector K2Node_PropertyAccess;                                                    // 0x0618 (size: 0x18)
    double Speed;                                                                     // 0x0630 (size: 0x8)
    class ABP_AnimalDeer_C* DeerRef;                                                  // 0x0638 (size: 0x8)

    void AnimGraph(FPoseLink& AnimGraph);
    void GetSpeed();
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Deer_AnimGraphNode_TransitionResult_2FE4AF3741ACFED8DC95ADB86FB05005();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Deer_AnimGraphNode_TransitionResult_BD0F68DF4B701F24BAAA25B911DD7040();
    void AnimNotify_Footstep();
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_Deer(int32 EntryPoint);
}; // Size: 0x640

#endif
