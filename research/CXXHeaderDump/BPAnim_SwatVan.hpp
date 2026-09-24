#ifndef UE4SS_SDK_BPAnim_SwatVan_HPP
#define UE4SS_SDK_BPAnim_SwatVan_HPP

struct FAnimBlueprintGeneratedConstantData : public FAnimBlueprintConstantData
{
    FName __NameProperty_10;                                                          // 0x0004 (size: 0x8)
    FName __NameProperty_11;                                                          // 0x000C (size: 0x8)
    FAnimNodeFunctionRef __StructProperty_12;                                         // 0x0018 (size: 0x20)
    FAnimSubsystem_PropertyAccess AnimBlueprintExtension_PropertyAccess;              // 0x0038 (size: 0x80)
    FAnimSubsystem_Base AnimBlueprintExtension_Base;                                  // 0x00B8 (size: 0x40)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_Root;                   // 0x00F8 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_MeshRefPose;            // 0x0128 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_ComponentToLocalSpace;  // 0x0158 (size: 0x30)
    FAnimNodeExposedValueHandler_PropertyAccess AnimGraphNode_WheelController;        // 0x0188 (size: 0x30)

}; // Size: 0x1B8

class UBPAnim_SwatVan_C : public UVehicleAnimationInstance
{
    FAnimSubsystemInstance AnimBlueprintExtension_PropertyAccess;                     // 0x0BC0 (size: 0x8)
    FAnimSubsystemInstance AnimBlueprintExtension_Base;                               // 0x0BC8 (size: 0x8)
    FAnimNode_Root AnimGraphNode_Root;                                                // 0x0BD0 (size: 0x20)
    FAnimNode_MeshSpaceRefPose AnimGraphNode_MeshRefPose;                             // 0x0BF0 (size: 0x10)
    FAnimNode_ConvertComponentToLocalSpace AnimGraphNode_ComponentToLocalSpace;       // 0x0C00 (size: 0x20)
    FAnimNode_WheelController AnimGraphNode_WheelController;                          // 0x0C20 (size: 0xE0)

    void AnimGraph(FPoseLink& AnimGraph);
}; // Size: 0xD00

#endif
