#ifndef UE4SS_SDK_AnimBP_Dog_HPP
#define UE4SS_SDK_AnimBP_Dog_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_16;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_17;                                                          // 0x000C (size: 0x8)
    bool __BoolProperty_18;                                                           // 0x0014 (size: 0x1)
    FAnimNodeFunctionRef __StructProperty_19;                                         // 0x0018 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0038 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00B8 (size: 0x18)

}; // Size: 0xD0

struct FAnimBlueprintGeneratedMutableData : public FAnimBlueprintMutableData
{
}; // Size: 0x1

class UAnimBP_Dog_C : public UAnimInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0350 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0358 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0360 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0368 (size: 0x20)
    FAnimNode_RandomPlayer AnimGraphNode_RandomPlayer;                                // 0x0388 (size: 0x78)
    FAnimNode_Slot AnimGraphNode_Slot;                                                // 0x0400 (size: 0x48)
    FAnimNode_ModifyBone AnimGraphNode_ModifyBone;                                    // 0x0448 (size: 0x128)
    FAnimNode_ConvertLocalToComponentSpace AnimGraphNode_LocalToComponentSpace;       // 0x0570 (size: 0x20)
    FAnimNode_ConvertComponentToLocalSpace AnimGraphNode_ComponentToLocalSpace;       // 0x0590 (size: 0x20)
    FRotator HeadRotator;                                                             // 0x05B0 (size: 0x18)

    void AnimGraph(FPoseLink& AnimGraph);
    void HeadRotation(double MaxRotationLeft/Right, double MaxRotationUp/Down, double InterpSpeed);
    void BlueprintUpdateAnimation(float DeltaTimeX);
    void ExecuteUbergraph_AnimBP_Dog(int32 EntryPoint);
}; // Size: 0x5C8

#endif
