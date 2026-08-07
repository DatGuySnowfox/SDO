#ifndef UE4SS_SDK_ExampleCharacter_AnimBP_HPP
#define UE4SS_SDK_ExampleCharacter_AnimBP_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_263;                                                         // 0x0004 (size: 0x8)
    FName __NameProperty_264;                                                         // 0x000C (size: 0x8)
    int32 __IntProperty_265;                                                          // 0x0014 (size: 0x4)
    float __FloatProperty_266;                                                        // 0x0018 (size: 0x4)
    FName __NameProperty_267;                                                         // 0x001C (size: 0x8)
    int32 __IntProperty_268;                                                          // 0x0024 (size: 0x4)
    FName __NameProperty_269;                                                         // 0x0028 (size: 0x8)
    int32 __IntProperty_270;                                                          // 0x0030 (size: 0x4)
    FName __NameProperty_271;                                                         // 0x0034 (size: 0x8)
    int32 __IntProperty_272;                                                          // 0x003C (size: 0x4)
    FName __NameProperty_273;                                                         // 0x0040 (size: 0x8)
    int32 __IntProperty_274;                                                          // 0x0048 (size: 0x4)
    FInputScaleBiasClampConstants __StructProperty_275;                               // 0x004C (size: 0x2C)
    FName __NameProperty_276;                                                         // 0x0078 (size: 0x8)
    int32 __IntProperty_277;                                                          // 0x0080 (size: 0x4)
    class UBlendProfile* __BlendProfile_278;                                          // 0x0088 (size: 0x8)
    class UCurveFloat* __CurveFloat_279;                                              // 0x0090 (size: 0x8)
    EAlphaBlendOption __EnumProperty_280;                                             // 0x0098 (size: 0x1)
    EBlendListTransitionType __EnumProperty_281;                                      // 0x0099 (size: 0x1)
    TArray<float> __ArrayProperty_282;                                                // 0x00A0 (size: 0x10)
    float __FloatProperty_283;                                                        // 0x00B0 (size: 0x4)
    bool __BoolProperty_284;                                                          // 0x00B4 (size: 0x1)
    float __FloatProperty_285;                                                        // 0x00B8 (size: 0x4)
    bool __BoolProperty_286;                                                          // 0x00BC (size: 0x1)
    EAnimSyncMethod __EnumProperty_287;                                               // 0x00BD (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_288;                             // 0x00BE (size: 0x1)
    FName __NameProperty_289;                                                         // 0x00C0 (size: 0x8)
    FName __NameProperty_290;                                                         // 0x00C8 (size: 0x8)
    int32 __IntProperty_291;                                                          // 0x00D0 (size: 0x4)
    FAnimNodeFunctionRef __StructProperty_292;                                        // 0x00D8 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x00F8 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x0178 (size: 0x18)

}; // Size: 0x190

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
    float __FloatProperty;                                                            // 0x0004 (size: 0x4)
    float __FloatProperty_0;                                                          // 0x0008 (size: 0x4)
    bool __BoolProperty_1;                                                            // 0x000C (size: 0x1)
    float __FloatProperty_2;                                                          // 0x0010 (size: 0x4)
    float __FloatProperty_3;                                                          // 0x0014 (size: 0x4)
    bool __BoolProperty_4;                                                            // 0x0018 (size: 0x1)
    bool __BoolProperty_5;                                                            // 0x0019 (size: 0x1)
    float __FloatProperty_6;                                                          // 0x001C (size: 0x4)
    float __FloatProperty_7;                                                          // 0x0020 (size: 0x4)
    bool __BoolProperty_8;                                                            // 0x0024 (size: 0x1)
    float __FloatProperty_9;                                                          // 0x0028 (size: 0x4)
    float __FloatProperty_10;                                                         // 0x002C (size: 0x4)

}; // Size: 0x30

class UExampleCharacter_AnimBP_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    FAnimBlueprintGeneratedMutableData __AnimBlueprintMutables;                       // 0x0358 (size: 0x30)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0388 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0390 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0398 (size: 0x20)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_13;                     // 0x03B8 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_12;                     // 0x03E0 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_11;                     // 0x0408 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_10;                     // 0x0430 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_9;                      // 0x0458 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_8;                      // 0x0480 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_7;                      // 0x04A8 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_6;                      // 0x04D0 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_5;                      // 0x04F8 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_4;                      // 0x0520 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_3;                      // 0x0548 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_2;                      // 0x0570 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult_1;                      // 0x0598 (size: 0x28)
    FAnimNode_TransitionResult AnimGraphNode_TransitionResult;                        // 0x05C0 (size: 0x28)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_6;                          // 0x05E8 (size: 0x48)
    FAnimNode_StateResult AnimGraphNode_StateResult_6;                                // 0x0630 (size: 0x20)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_5;                          // 0x0650 (size: 0x48)
    FAnimNode_StateResult AnimGraphNode_StateResult_5;                                // 0x0698 (size: 0x20)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_4;                          // 0x06B8 (size: 0x48)
    FAnimNode_StateResult AnimGraphNode_StateResult_4;                                // 0x0700 (size: 0x20)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer_3;                      // 0x0720 (size: 0x70)
    FAnimNode_BlendListByBool AnimGraphNode_BlendListByBool_3;                        // 0x0790 (size: 0x48)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer_2;                      // 0x07D8 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult_3;                                // 0x0848 (size: 0x20)
    FAnimNode_BlendListByBool AnimGraphNode_BlendListByBool_2;                        // 0x0868 (size: 0x48)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_3;                          // 0x08B0 (size: 0x48)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_2;                          // 0x08F8 (size: 0x48)
    FAnimNode_StateResult AnimGraphNode_StateResult_2;                                // 0x0940 (size: 0x20)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer_1;                          // 0x0960 (size: 0x48)
    FAnimNode_BlendListByBool AnimGraphNode_BlendListByBool_1;                        // 0x09A8 (size: 0x48)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer;                            // 0x09F0 (size: 0x48)
    FAnimNode_StateResult AnimGraphNode_StateResult_1;                                // 0x0A38 (size: 0x20)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer_1;                      // 0x0A58 (size: 0x70)
    FAnimNode_BlendListByBool AnimGraphNode_BlendListByBool;                          // 0x0AC8 (size: 0x48)
    FAnimNode_BlendSpacePlayer AnimGraphNode_BlendSpacePlayer;                        // 0x0B10 (size: 0x70)
    FAnimNode_StateResult AnimGraphNode_StateResult;                                  // 0x0B80 (size: 0x20)
    FAnimNode_StateMachine AnimGraphNode_StateMachine;                                // 0x0BA0 (size: 0xC8)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x0C68 (size: 0x48)
    class ABP_ExampleCharacter_C* Example Character;                                  // 0x0CB0 (size: 0x8)
    double Speed;                                                                     // 0x0CB8 (size: 0x8)
    double Direction;                                                                 // 0x0CC0 (size: 0x8)
    bool Enable Jump;                                                                 // 0x0CC8 (size: 0x1)
    bool Crouching;                                                                   // 0x0CC9 (size: 0x1)
    bool Is Falling;                                                                  // 0x0CCA (size: 0x1)
    bool Aimed;                                                                       // 0x0CCB (size: 0x1)

    void AnimGraph(FPoseLink& AnimGraph);
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_B5FC24DC46AC278756122B949D5CDB8C();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_D25429B644E6C66D625827BA5AE5F04E();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_315E9BF04EA579C328EB6A866B91C07A();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_F13AB6764EBE8335385B039A555B9993();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_4AAD4A594EABD4F92E1D44BBD3F4830B();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_F867B5374C7EFB9ED9010FA7431019DF();
    void EvaluateGraphExposedInputs_ExecuteUbergraph_ExampleCharacter_AnimBP_AnimGraphNode_TransitionResult_BA7306BD40D3FE50A5E9239D31192E92();
    void BlueprintUpdateAnimation(float DeltaTimeX);
    void BlueprintInitializeAnimation();
    void ExecuteUbergraph_ExampleCharacter_AnimBP(int32 EntryPoint);
}; // Size: 0xCCC

#endif
