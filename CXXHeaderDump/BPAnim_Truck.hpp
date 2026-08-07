#ifndef UE4SS_SDK_BPAnim_Truck_HPP
#define UE4SS_SDK_BPAnim_Truck_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_10;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_11;                                                          // 0x000C (size: 0x8)
    FAnimNodeFunctionRef __StructProperty_12;                                         // 0x0018 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0038 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00B8 (size: 0x18)

}; // Size: 0xD0

class UBPAnim_Truck_C : public UVehicleAnimationInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0A90 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0A98 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0AA0 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0AA8 (size: 0x20)
    FAnimNode_MeshSpaceRefPose AnimGraphNode_MeshRefPose;                             // 0x0AC8 (size: 0x10)
    FAnimNode_WheelController AnimGraphNode_WheelController;                          // 0x0AD8 (size: 0xE0)
    FAnimNode_ConvertComponentToLocalSpace AnimGraphNode_ComponentToLocalSpace;       // 0x0BB8 (size: 0x20)

    void AnimGraph(FPoseLink& AnimGraph);
    void ExecuteUbergraph_BPAnim_Truck(int32 EntryPoint);
}; // Size: 0xBD8

#endif
