#ifndef UE4SS_SDK_DataflowEngine_HPP
#define UE4SS_SDK_DataflowEngine_HPP

#include "DataflowEngine_enums.hpp"

struct FDataflowCallSubGraphNode : public FDataflowNode
{
    FGuid SubGraphGuid;                                                               // 0x0290 (size: 0x10)
    FDataflowDynamicConnections DynamicInputs;                                        // 0x02A0 (size: 0x78)
    FInstancedPropertyBag InputsPropertyBag;                                          // 0x0318 (size: 0x10)
    FDataflowDynamicConnections DynamicOutputs;                                       // 0x0328 (size: 0x78)
    FInstancedPropertyBag OutputsPropertyBag;                                         // 0x03A0 (size: 0x10)

}; // Size: 0x3C0

struct FDataflowDynamicConnections
{
    TArray<FDataflowAllTypes> DynamicProperties;                                      // 0x0000 (size: 0x10)

}; // Size: 0x78

struct FDataflowImageToTextureNode : public FDataflowNode
{
    FDataflowImage Image;                                                             // 0x0280 (size: 0x28)
    FName TextureName;                                                                // 0x02A8 (size: 0x8)
    class UTexture2D* TransientTexture;                                               // 0x02B0 (size: 0x8)

}; // Size: 0x2B8

struct FDataflowInstance
{
    class UDataflow* DataflowAsset;                                                   // 0x0000 (size: 0x8)
    FName DataflowTerminal;                                                           // 0x0008 (size: 0x8)
    FDataflowVariableOverrides VariableOverrides;                                     // 0x0010 (size: 0x28)
    class UObject* Owner;                                                             // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FDataflowNodeDebugDrawSettings
{
    EDataflowDebugDrawRenderType RenderType;                                          // 0x0000 (size: 0x1)
    bool bTranslucent;                                                                // 0x0001 (size: 0x1)
    FLinearColor Color;                                                               // 0x0004 (size: 0x10)
    float LineWidthMultiplier;                                                        // 0x0014 (size: 0x4)

}; // Size: 0x18

struct FDataflowNodeSphereCoveringDebugDrawSettings
{
    bool bDisplaySphereCovering;                                                      // 0x0000 (size: 0x1)
    EDataflowDebugDrawRenderType RenderType;                                          // 0x0001 (size: 0x1)
    bool bTranslucent;                                                                // 0x0002 (size: 0x1)
    float LineWidthMultiplier;                                                        // 0x0004 (size: 0x4)
    EDataflowSphereCoveringColorMethod ColorMethod;                                   // 0x0008 (size: 0x1)
    FLinearColor Color;                                                               // 0x000C (size: 0x10)
    int32 ColorRandomSeed;                                                            // 0x001C (size: 0x4)
    FLinearColor ColorA;                                                              // 0x0020 (size: 0x10)
    FLinearColor ColorB;                                                              // 0x0030 (size: 0x10)

}; // Size: 0x40

struct FDataflowPreviewCacheParams
{
    int32 FrameRate;                                                                  // 0x0000 (size: 0x4)
    int32 SubframeRate;                                                               // 0x0004 (size: 0x4)
    bool bCanEditSubframeRate;                                                        // 0x0008 (size: 0x1)
    FVector2f TimeRange;                                                              // 0x000C (size: 0x8)
    bool bRestartSimulation;                                                          // 0x0014 (size: 0x1)
    FVector2f RestartTimeRange;                                                       // 0x0018 (size: 0x8)
    bool bAsyncCaching;                                                               // 0x0020 (size: 0x1)

}; // Size: 0x24

struct FDataflowProxyElement : public FDataflowBaseElement
{
}; // Size: 0x68

struct FDataflowSubGraphGetCurrentIndexNode : public FDataflowNode
{
    int32 Index;                                                                      // 0x0280 (size: 0x4)

}; // Size: 0x288

struct FDataflowSubGraphInputNode : public FDataflowNode
{
    FDataflowDynamicConnections DynamicConnections;                                   // 0x0288 (size: 0x78)
    FInstancedPropertyBag PropertyBag;                                                // 0x0300 (size: 0x10)

}; // Size: 0x310

struct FDataflowSubGraphOutputNode : public FDataflowNode
{
    FDataflowDynamicConnections DynamicConnections;                                   // 0x0288 (size: 0x78)
    FInstancedPropertyBag PropertyBag;                                                // 0x0300 (size: 0x10)

}; // Size: 0x310

struct FDataflowTextureTerminalNode : public FDataflowTerminalNode
{
    FDataflowImage Image;                                                             // 0x0280 (size: 0x28)
    class UTexture2D* TextureAsset;                                                   // 0x02A8 (size: 0x8)

}; // Size: 0x2B0

struct FDataflowTextureToImageNode : public FDataflowNode
{
    class UTexture2D* TextureAsset;                                                   // 0x0280 (size: 0x8)
    FDataflowImage Image;                                                             // 0x0288 (size: 0x28)

}; // Size: 0x2B0

struct FDataflowVariableOverrides
{
    FInstancedPropertyBag Variables;                                                  // 0x0000 (size: 0x10)
    TArray<FGuid> OverriddenVariableGuids;                                            // 0x0010 (size: 0x10)

}; // Size: 0x28

struct FGetDataflowVariableNode : public FDataflowNode
{
    FDataflowAnyType Value;                                                           // 0x0280 (size: 0x1)
    FInstancedPropertyBag VariablePropertyBag;                                        // 0x0288 (size: 0x10)
    FName VariableName;                                                               // 0x0298 (size: 0x8)

}; // Size: 0x2B8

struct FStringValuePair
{
    FString Key;                                                                      // 0x0000 (size: 0x10)
    FString Value;                                                                    // 0x0010 (size: 0x10)

}; // Size: 0x20

class IDataflowContentOwner : public IInterface
{
}; // Size: 0x28

class IDataflowInstanceInterface : public IInterface
{
}; // Size: 0x28

class UDataflow : public UEdGraph
{
    bool bActive;                                                                     // 0x00A8 (size: 0x1)
    TArray<UObject*> Targets;                                                         // 0x00B0 (size: 0x10)
    class UMaterial* Material;                                                        // 0x00C0 (size: 0x8)
    EDataflowType Type;                                                               // 0x00C8 (size: 0x1)
    FInstancedPropertyBag Variables;                                                  // 0x00D0 (size: 0x10)
    TArray<UDataflowSubGraph*> DataflowSubGraphs;                                     // 0x00E0 (size: 0x10)

}; // Size: 0x108

class UDataflowBaseContent : public UDataflowContextObject
{
    FString DataflowTerminal;                                                         // 0x0078 (size: 0x10)
    class UObject* TerminalAsset;                                                     // 0x0088 (size: 0x8)
    bool bIsConstructionDirty;                                                        // 0x0098 (size: 0x1)
    bool bIsSimulationDirty;                                                          // 0x0099 (size: 0x1)

}; // Size: 0xA8

class UDataflowBlueprintLibrary : public UBlueprintFunctionLibrary
{

    bool RegenerateAssetFromDataflow(class UObject* AssetToRegenerate, bool bRegenerateDependentAssets);
    bool OverrideDataflowVariableObjectArray(class UObject* Asset, FName VariableName, const TArray<UObject*>& VariableArrayValue);
    bool OverrideDataflowVariableObject(class UObject* Asset, FName VariableName, class UObject* VariableValue);
    bool OverrideDataflowVariableIntArray(class UObject* Asset, FName VariableName, const TArray<int32>& VariableArrayValue);
    bool OverrideDataflowVariableInt(class UObject* Asset, FName VariableName, int64 VariableValue);
    bool OverrideDataflowVariableFloatArray(class UObject* Asset, FName VariableName, const TArray<float>& VariableArrayValue);
    bool OverrideDataflowVariableFloat(class UObject* Asset, FName VariableName, float VariableValue);
    bool OverrideDataflowVariableBoolArray(class UObject* Asset, FName VariableName, const TArray<bool>& VariableArrayValue);
    bool OverrideDataflowVariableBool(class UObject* Asset, FName VariableName, bool VariableValue);
    void EvaluateTerminalNodeByName(class UDataflow* Dataflow, FName TerminalNodeName, class UObject* ResultAsset);
}; // Size: 0x28

class UDataflowContextObject : public UObject
{
    class UDataflowEdNode* SelectedNode;                                              // 0x0038 (size: 0x8)
    class UDataflow* DataflowGraph;                                                   // 0x0058 (size: 0x8)

}; // Size: 0x78

class UDataflowDebugDrawComponent : public UDebugDrawComponent
{
}; // Size: 0x580

class UDataflowEdNode : public UEdGraphNode
{
    bool bRenderInAssetEditor;                                                        // 0x00B8 (size: 0x1)
    bool bRenderWireframeInAssetEditor;                                               // 0x00B9 (size: 0x1)
    bool bCanEnableRenderWireframe;                                                   // 0x00BA (size: 0x1)

}; // Size: 0xD8

class UDataflowMesh : public UObject
{
    TArray<UMaterialInterface*> Materials;                                            // 0x0030 (size: 0x10)

}; // Size: 0x40

class UDataflowSkeletalContent : public UDataflowBaseContent
{
    class USkeletalMesh* SkeletalMesh;                                                // 0x00A8 (size: 0x8)
    class UAnimationAsset* AnimationAsset;                                            // 0x00B0 (size: 0x8)

}; // Size: 0xC0

class UDataflowSubGraph : public UEdGraph
{
    FGuid SubGraphGuid;                                                               // 0x0060 (size: 0x10)
    bool bIsForEach;                                                                  // 0x0070 (size: 0x1)

}; // Size: 0x78

#endif
