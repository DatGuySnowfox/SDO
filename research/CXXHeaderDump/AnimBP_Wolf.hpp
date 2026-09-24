#ifndef UE4SS_SDK_AnimBP_Wolf_HPP
#define UE4SS_SDK_AnimBP_Wolf_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_39;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_40;                                                          // 0x000C (size: 0x8)
    int32 __IntProperty_41;                                                           // 0x0014 (size: 0x4)
    bool __BoolProperty_42;                                                           // 0x0018 (size: 0x1)
    float __FloatProperty_43;                                                         // 0x001C (size: 0x4)
    float __FloatProperty_44;                                                         // 0x0020 (size: 0x4)
    EAnimSyncMethod __EnumProperty_45;                                                // 0x0024 (size: 0x1)
    bool __BoolProperty_46;                                                           // 0x0025 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_47;                              // 0x0026 (size: 0x1)
    FName __NameProperty_48;                                                          // 0x0028 (size: 0x8)
    FName __NameProperty_49;                                                          // 0x0030 (size: 0x8)
    FName __NameProperty_50;                                                          // 0x0038 (size: 0x8)
    int32 __IntProperty_51;                                                           // 0x0040 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_52;                                         // 0x0048 (size: 0x20)
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
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Slot;                   // 0x02A8 (size: 0x30)

}; // Size: 0x2D8

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)

}; // Size: 0x8

class UAnimBP_Wolf_C : public UAnimInstance
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
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x0660 (size: 0x48)
    FVector K2Node_PropertyAccess;                                                    // 0x06A8 (size: 0x18)
    double Speed;                                                                     // 0x06C0 (size: 0x8)
    class ABP_AnimalWolf_C* WolfRef;                                                  // 0x06C8 (size: 0x8)

    void AnimGraph(FPoseLink& AnimGraph);
    void GetSpeed();
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Wolf_AnimGraphNode_TransitionResult_371585D64CCD99DDD6B315901C8736A6();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_AnimBP_Wolf_AnimGraphNode_TransitionResult_84FA0BBA449C725C173465BF58BE4E03();
    void AnimNotify_Footstep();
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_AnimBP_Wolf(int32 EntryPoint);
}; // Size: 0x6D0

#endif
