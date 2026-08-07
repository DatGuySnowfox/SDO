#ifndef UE4SS_SDK_AnimBP_Trader_HPP
#define UE4SS_SDK_AnimBP_Trader_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_16;                                                          // 0x0004 (size: 0x8)
    bool __BoolProperty_17;                                                           // 0x000C (size: 0x1)
    float __FloatProperty_18;                                                         // 0x0010 (size: 0x4)
    FInputScaleBiasClampConstants __StructProperty_19;                                // 0x0014 (size: 0x2C)
    float __FloatProperty_20;                                                         // 0x0040 (size: 0x4)
    bool __BoolProperty_21;                                                           // 0x0044 (size: 0x1)
    EAnimSyncMethod __EnumProperty_22;                                                // 0x0045 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_23;                              // 0x0046 (size: 0x1)
    FName __NameProperty_24;                                                          // 0x0048 (size: 0x8)
    FAnimNodeFunctionRef __StructProperty_25;                                         // 0x0050 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0070 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00F0 (size: 0x18)

}; // Size: 0x108

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
}; // Size: 0x1

class UAnimBP_Trader_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0358 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0360 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0368 (size: 0x20)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer;                            // 0x0388 (size: 0x48)
    FAnimNode_ModifyBone AnimGraphNode_ModifyBone;                                    // 0x03D0 (size: 0x128)
    FAnimNode_ConvertLocalToComponentSpace AnimGraphNode_LocalToComponentSpace;       // 0x04F8 (size: 0x20)
    FAnimNode_ConvertComponentToLocalSpace AnimGraphNode_ComponentToLocalSpace;       // 0x0518 (size: 0x20)
    bool K2Node_PropertyAccess;                                                       // 0x0538 (size: 0x1)
    FRotator HeadRotator;                                                             // 0x0540 (size: 0x18)
    class ABP_TraderMaster_C* Trader;                                                 // 0x0558 (size: 0x8)
    bool Interacting?;                                                                // 0x0560 (size: 0x1)

    void AnimGraph(FPoseLink& AnimGraph);
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void HeadRotation(double MaxRotationLeft/Right, double MaxRotationUp/Down, double InterpSpeed);
    void BlueprintUpdateAnimation(float DeltaTimeX);
    void BlueprintBeginPlay();
    void ExecuteUbergraph_AnimBP_Trader(int32 EntryPoint);
}; // Size: 0x561

#endif
