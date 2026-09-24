#ifndef UE4SS_SDK_AnimBP_Trader_HPP
#define UE4SS_SDK_AnimBP_Trader_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_17;                                                          // 0x0004 (size: 0x8)
    bool __BoolProperty_18;                                                           // 0x000C (size: 0x1)
    float __FloatProperty_19;                                                         // 0x0010 (size: 0x4)
    FInputScaleBiasClampConstants __StructProperty_20;                                // 0x0014 (size: 0x2C)
    float __FloatProperty_21;                                                         // 0x0040 (size: 0x4)
    EAnimSyncMethod __EnumProperty_22;                                                // 0x0044 (size: 0x1)
    bool __BoolProperty_23;                                                           // 0x0045 (size: 0x1)
    TEnumAsByte<EAnimGroupRole::Type> __ByteProperty_24;                              // 0x0046 (size: 0x1)
    FName __NameProperty_25;                                                          // 0x0048 (size: 0x8)
    FAnimNodeFunctionRef __StructProperty_26;                                         // 0x0050 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0070 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00F0 (size: 0x40)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Root;                   // 0x0130 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_SequencePlayer;         // 0x0160 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_ModifyBone;             // 0x0190 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_LocalToComponentSpace;  // 0x01C0 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_ComponentToLocalSpace;  // 0x01F0 (size: 0x30)

}; // Size: 0x220

class UAnimBP_Trader_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x03E0 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x03E8 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x03F0 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x03F8 (size: 0x20)
    FAnimNode_SequencePlayer AnimGraphNode_SequencePlayer;                            // 0x0418 (size: 0x48)
    FAnimNode_ModifyBone AnimGraphNode_ModifyBone;                                    // 0x0460 (size: 0x128)
    FAnimNode_ConvertLocalToComponentSpace AnimGraphNode_LocalToComponentSpace;       // 0x0588 (size: 0x20)
    FAnimNode_ConvertComponentToLocalSpace AnimGraphNode_ComponentToLocalSpace;       // 0x05A8 (size: 0x20)
    bool K2Node_PropertyAccess;                                                       // 0x05C8 (size: 0x1)
    FRotator HeadRotator;                                                             // 0x05D0 (size: 0x18)
    class ABP_TraderMaster_C* Trader;                                                 // 0x05E8 (size: 0x8)
    bool Interacting?;                                                                // 0x05F0 (size: 0x1)

    void AnimGraph(FPoseLink& AnimGraph);
    void BlueprintThreadSafeUpdateAnimation(float DeltaTime);
    void HeadRotation(double MaxRotationLeft/Right, double MaxRotationUp/Down, double InterpSpeed);
    void BlueprintUpdateAnimation(float DeltaTimeX);
    void BlueprintBeginPlay();
    void ExecuteUbergraph_AnimBP_Trader(int32 EntryPoint);
}; // Size: 0x5F1

#endif
