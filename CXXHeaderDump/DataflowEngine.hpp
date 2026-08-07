#ifndef UE4SS_SDK_DataflowEngine_HPP
#define UE4SS_SDK_DataflowEngine_HPP

struct FStringValuePair
{
    FString Key;                                                                      // 0x0000 (size: 0x10)
    FString Value;                                                                    // 0x0010 (size: 0x10)

}; // Size: 0x20

class UDataflow : public UEdGraph
{
    bool bActive;                                                                     // 0x0088 (size: 0x1)
    TArray<class UObject*> Targets;                                                   // 0x0090 (size: 0x10)
    class UMaterial* Material;                                                        // 0x00A0 (size: 0x8)

}; // Size: 0xA8

class UDataflowBlueprintLibrary : public UBlueprintFunctionLibrary
{

    void EvaluateTerminalNodeByName(class UDataflow* Dataflow, FName TerminalNodeName, class UObject* ResultAsset);
}; // Size: 0x28

class UDataflowEdNode : public UEdGraphNode
{
    bool bRenderInAssetEditor;                                                        // 0x00B8 (size: 0x1)

}; // Size: 0xC0

#endif
