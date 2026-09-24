#ifndef UE4SS_SDK_InterchangeCore_HPP
#define UE4SS_SDK_InterchangeCore_HPP

#include "InterchangeCore_enums.hpp"

struct FInterchangePipelineContextParams
{
    EInterchangePipelineContext ContextType;                                          // 0x0000 (size: 0x1)
    UClass* ImportObjectType;                                                         // 0x0008 (size: 0x8)
    class UObject* ReimportAsset;                                                     // 0x0010 (size: 0x8)
    class UInterchangeBaseNodeContainer* BaseNodeContainer;                           // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FInterchangePipelinePropertyStatePerContext
{
    bool bVisible;                                                                    // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FInterchangePipelinePropertyStates
{
    bool bLocked;                                                                     // 0x0000 (size: 0x1)
    bool bPreDialogReset;                                                             // 0x0001 (size: 0x1)
    FInterchangePipelinePropertyStatePerContext BasicLayoutStates;                    // 0x0002 (size: 0x1)
    FInterchangePipelinePropertyStatePerContext ImportStates;                         // 0x0003 (size: 0x1)
    FInterchangePipelinePropertyStatePerContext ReimportStates;                       // 0x0004 (size: 0x1)

}; // Size: 0x5

struct FInterchangeUserDefinedAttributeInfo
{
    FString Name;                                                                     // 0x0000 (size: 0x10)

}; // Size: 0x30

class UInterchangeBaseNode : public UObject
{

    bool SetEnabled(const bool bIsEnabled);
    bool SetDisplayLabel(FString DisplayName);
    bool SetAssetName(FString AssetName);
    bool RemoveTargetNodeUid(FString AssetUid);
    bool RemoveAttribute(FString NodeAttributeKey);
    bool IsEnabled();
    void InitializeNode(FString UniqueID, FString DisplayLabel, const EInterchangeNodeContainerType NodeContainerType);
    bool GetVector2Attribute(FString NodeAttributeKey, FVector2f& OutValue);
    FString GetUniqueID();
    FString GetTypeName();
    void GetTargetNodeUids(TArray<FString>& OutTargetAssets);
    int32 GetTargetNodeCount();
    bool GetStringAttribute(FString NodeAttributeKey, FString& OutValue);
    FString GetParentUid();
    EInterchangeNodeContainerType GetNodeContainerType();
    bool GetNameSpace(FString& Namespace);
    bool GetLinearColorAttribute(FString NodeAttributeKey, FLinearColor& OutValue);
    bool GetInt32Attribute(FString NodeAttributeKey, int32& OutValue);
    FName GetIconName();
    bool GetGuidAttribute(FString NodeAttributeKey, FGuid& OutValue);
    bool GetFloatAttribute(FString NodeAttributeKey, float& OutValue);
    bool GetDoubleAttribute(FString NodeAttributeKey, double& OutValue);
    FString GetDisplayLabel();
    int32 GetDesiredChildIndex();
    bool GetBooleanAttribute(FString NodeAttributeKey, bool& OutValue);
    FString GetAssetName();
    bool AddVector2Attribute(FString NodeAttributeKey, const FVector2f& Value);
    bool AddTargetNodeUid(FString AssetUid);
    bool AddStringAttribute(FString NodeAttributeKey, FString Value);
    bool AddLinearColorAttribute(FString NodeAttributeKey, const FLinearColor& Value);
    bool AddInt32Attribute(FString NodeAttributeKey, const int32& Value);
    bool AddGuidAttribute(FString NodeAttributeKey, const FGuid& Value);
    bool AddFloatAttribute(FString NodeAttributeKey, const float& Value);
    bool AddDoubleAttribute(FString NodeAttributeKey, const double& Value);
    bool AddBooleanAttribute(FString NodeAttributeKey, const bool& Value);
}; // Size: 0x60

class UInterchangeBaseNodeContainer : public UObject
{
    TMap<FString, UInterchangeBaseNode*> Nodes;                                       // 0x0028 (size: 0x50)

    bool SetNodeParentUid(FString NodeUniqueID, FString NewParentNodeUid);
    bool SetNodeDesiredChildIndex(FString NodeUniqueID, const int32& NewNodeDesiredChildIndex);
    void SetNamespace(FString Namespace, UClass* TargetClass);
    void SaveToFile(FString Filename);
    void ResetChildrenCache();
    void Reset();
    void ReplaceNode(FString NodeUniqueID, class UInterchangeFactoryBaseNode* NewNode);
    void RemoveNode(FString NodeUniqueID);
    void LoadFromFile(FString Filename);
    bool IsNodeUidValid(FString NodeUniqueID);
    void GetRoots(TArray<FString>& RootNodes);
    void GetNodes(const UClass* ClassNode, TArray<FString>& OutNodes);
    TArray<FString> GetNodeChildrenUids(FString NodeUniqueID);
    int32 GetNodeChildrenCount(FString NodeUniqueID);
    class UInterchangeBaseNode* GetNodeChildren(FString NodeUniqueID, int32 ChildIndex);
    class UInterchangeBaseNode* GetNode(FString NodeUniqueID);
    bool GetIsAncestor(FString NodeUniqueID, FString AncestorUID);
    class UInterchangeFactoryBaseNode* GetFactoryNode(FString NodeUniqueID);
    void ComputeChildrenCache();
    bool ClearNodeParentUid(FString NodeUniqueID);
    FString AddNode(class UInterchangeBaseNode* Node);
}; // Size: 0xC8

class UInterchangeFactoryBase : public UObject
{
    class UInterchangeResultsContainer* Results;                                      // 0x0028 (size: 0x8)

    bool SetSourceFilename(const class UObject* Object, FString SourceFilename, int32 SourceIndex);
    bool GetSourceFilenames(const class UObject* Object, TArray<FString>& OutSourceFilenames);
    UClass* GetFactoryClass();
    EInterchangeFactoryAssetType GetFactoryAssetType();
}; // Size: 0x30

class UInterchangeFactoryBaseNode : public UInterchangeBaseNode
{
    TSet<FString> AttributesAppliedThroughDelegatesKeySet;                            // 0x0140 (size: 0x50)

    bool UnsetSkipNodeImport();
    bool UnsetForceNodeReimport();
    bool ShouldSkipNodeImport();
    bool ShouldForceNodeReimport();
    bool SetSkipNodeImport();
    bool SetReimportStrategyFlags(const EReimportStrategyFlags& ReimportStrategyFlags);
    bool SetForceNodeReimport();
    bool SetCustomSubPath(FString AttributeValue);
    bool SetCustomReferenceObject(const FSoftObjectPath& AttributeValue);
    bool SetCustomLevelUid(FString AttributeValue);
    bool RemoveFactoryDependencyUid(FString DependencyUid);
    bool IsRuntimeImportAllowed();
    EReimportStrategyFlags GetReimportStrategyFlags();
    UClass* GetObjectClass();
    void GetFactoryDependency(const int32 Index, FString& OutDependency);
    int32 GetFactoryDependenciesCount();
    void GetFactoryDependencies(TArray<FString>& OutDependencies);
    bool GetCustomSubPath(FString& AttributeValue);
    bool GetCustomReferenceObject(FSoftObjectPath& AttributeValue);
    bool GetCustomLevelUid(FString& AttributeValue);
    bool AddFactoryDependencyUid(FString DependencyUid);
}; // Size: 0x1C0

class UInterchangePipelineBase : public UObject
{
    FString DestinationName;                                                          // 0x0028 (size: 0x10)
    FString ContentImportPath;                                                        // 0x0038 (size: 0x10)
    bool bAllowPropertyStatesEdition;                                                 // 0x0048 (size: 0x1)
    bool bIsReimportContext;                                                          // 0x0049 (size: 0x1)
    bool bIsShowEssentials;                                                           // 0x004A (size: 0x1)
    bool bFromReimportOrOverride;                                                     // 0x004B (size: 0x1)
    class UInterchangeResultsContainer* Results;                                      // 0x0050 (size: 0x8)
    TMap<FName, FInterchangePipelinePropertyStates> PropertiesStates;                 // 0x0058 (size: 0x50)
    TMap<FName, FInterchangePipelinePropertyStates> CachePropertiesStates;            // 0x00A8 (size: 0x50)
    FInterchangePipelineContextParams CacheContextParam;                              // 0x00F8 (size: 0x20)

    bool SupportReimport();
    void ScriptedSetReimportSourceIndex(UClass* ReimportObjectClass, const int32 SourceFileIndex);
    FString ScriptedGetPipelineDisplayName();
    void ScriptedExecutePostImportPipeline(const class UInterchangeBaseNodeContainer* BaseNodeContainer, FString FactoryNodeKey, class UObject* CreatedAsset, bool bIsAReimport);
    void ScriptedExecutePostFactoryPipeline(const class UInterchangeBaseNodeContainer* BaseNodeContainer, FString FactoryNodeKey, class UObject* CreatedAsset, bool bIsAReimport);
    void ScriptedExecutePostBroadcastPipeline(const class UInterchangeBaseNodeContainer* BaseNodeContainer, FString FactoryNodeKey, class UObject* CreatedAsset, bool bIsAReimport);
    void ScriptedExecutePipeline(class UInterchangeBaseNodeContainer* BaseNodeContainer, const TArray<UInterchangeSourceData*>& SourceDatas, FString ContentBasePath);
    void ScriptedExecuteExportPipeline(class UInterchangeBaseNodeContainer* BaseNodeContainer);
    bool IsReimportContext();
    FInterchangePipelinePropertyStates FindOrAddPropertyStates(const FName PropertyPath);
    bool DoesPropertyStatesExist(const FName PropertyPath);
}; // Size: 0x128

class UInterchangeResult : public UObject
{
    FString SourceAssetName;                                                          // 0x0028 (size: 0x10)
    FString DestinationAssetName;                                                     // 0x0038 (size: 0x10)
    FString AssetFriendlyName;                                                        // 0x0048 (size: 0x10)
    UClass* AssetType;                                                                // 0x0058 (size: 0x8)
    FString InterchangeKey;                                                           // 0x0060 (size: 0x10)

}; // Size: 0x70

class UInterchangeResultDisplay_Generic : public UInterchangeResultSuccess
{
    FText Text;                                                                       // 0x0070 (size: 0x10)

}; // Size: 0x80

class UInterchangeResultError : public UInterchangeResult
{
}; // Size: 0x70

class UInterchangeResultError_Generic : public UInterchangeResultError
{
    FText Text;                                                                       // 0x0070 (size: 0x10)

}; // Size: 0x80

class UInterchangeResultError_ReimportFail : public UInterchangeResultError
{
}; // Size: 0x70

class UInterchangeResultSuccess : public UInterchangeResult
{
}; // Size: 0x70

class UInterchangeResultWarning : public UInterchangeResult
{
}; // Size: 0x70

class UInterchangeResultWarning_Generic : public UInterchangeResultWarning
{
    FText Text;                                                                       // 0x0070 (size: 0x10)

}; // Size: 0x80

class UInterchangeResultsContainer : public UObject
{
    TArray<UInterchangeResult*> Results;                                              // 0x0050 (size: 0x10)

}; // Size: 0x60

class UInterchangeSourceData : public UObject
{
    FString Filename;                                                                 // 0x0028 (size: 0x10)
    TMap<FString, UObject*> ContextObjectsByTag;                                      // 0x0050 (size: 0x50)

    bool SetFilename(FString InFilename);
    void SetContextObjectByTag(FString Tag, class UObject* Object);
    void RemoveAllContextObjects();
    FString GetFilename();
    class UObject* GetContextObjectByTag(FString Tag);
    TArray<FString> GetAllContextObjectTags();
}; // Size: 0xA0

class UInterchangeSourceNode : public UInterchangeBaseNode
{

    bool SetExtraInformation(FString Name, FString Value);
    bool SetCustomUseLegacySkeletalMeshBakeTransform(const bool& AttributeValue);
    bool SetCustomUseAssetTypeSubPathSuffix(const bool& Suffix);
    bool SetCustomSubPathPrefix(FString Prefix);
    bool SetCustomSourceTimelineStart(const double& AttributeValue);
    bool SetCustomSourceTimelineEnd(const double& AttributeValue);
    bool SetCustomSourceFrameRateNumerator(const int32& AttributeValue);
    bool SetCustomSourceFrameRateDenominator(const int32& AttributeValue);
    bool SetCustomReimportStrategyFlags(uint8 StrategyFlag);
    bool SetCustomImportUnusedMaterial(const bool& AttributeValue);
    bool SetCustomAxisConversionInverseTransform(const FTransform& AxisConversionInverseTransform);
    bool SetCustomAnimatedTimeStart(const double& AttributeValue);
    bool SetCustomAnimatedTimeEnd(const double& AttributeValue);
    bool RemoveExtraInformation(FString Name);
    void InitializeSourceNode(FString UniqueID, FString DisplayLabel, class UInterchangeBaseNodeContainer* NodeContainer);
    class UInterchangeSourceNode* GetUniqueInstance(const class UInterchangeBaseNodeContainer* NodeContainer);
    void GetExtraInformation(TMap<FString, FString>& OutExtraInformation);
    bool GetCustomUseLegacySkeletalMeshBakeTransform(bool& AttributeValue);
    bool GetCustomUseAssetTypeSubPathSuffix(bool& Suffix);
    bool GetCustomSubPathPrefix(FString& Prefix);
    bool GetCustomSourceTimelineStart(double& AttributeValue);
    bool GetCustomSourceTimelineEnd(double& AttributeValue);
    bool GetCustomSourceFrameRateNumerator(int32& AttributeValue);
    bool GetCustomSourceFrameRateDenominator(int32& AttributeValue);
    bool GetCustomReimportStrategyFlags(uint8& StrategyFlag);
    bool GetCustomImportUnusedMaterial(bool& AttributeValue);
    bool GetCustomAxisConversionInverseTransform(FTransform& AxisConversionInverseTransform);
    bool GetCustomAnimatedTimeStart(double& AttributeValue);
    bool GetCustomAnimatedTimeEnd(double& AttributeValue);
}; // Size: 0x198

class UInterchangeTranslatorBase : public UObject
{
    class UInterchangeResultsContainer* Results;                                      // 0x0028 (size: 0x8)
    class UInterchangeSourceData* SourceData;                                         // 0x0030 (size: 0x8)

    void SetSettings(const class UInterchangeTranslatorSettings* InterchangeTranslatorSettings);
    EInterchangeTranslatorType GetTranslatorType();
    TArray<FString> GetSupportedFormats();
    EInterchangeTranslatorAssetType GetSupportedAssetTypes();
    class UInterchangeSourceData* GetSourceData();
    class UInterchangeTranslatorSettings* GetSettings();
    bool CanImportSourceData(const class UInterchangeSourceData* InSourceData);
}; // Size: 0x48

class UInterchangeTranslatorSettings : public UObject
{

    void SaveSettings();
    void LoadSettings();
}; // Size: 0x28

class UInterchangeUserDefinedAttributesAPI : public UObject
{

    bool RemoveUserDefinedAttribute(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName);
    void GetUserDefinedAttributeInfos(const class UInterchangeBaseNode* InterchangeNode, TArray<FInterchangeUserDefinedAttributeInfo>& UserDefinedAttributeInfos);
    bool GetUserDefinedAttribute_Int32(const class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, int32& OutValue, FString& OutPayloadKey);
    bool GetUserDefinedAttribute_FString(const class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, FString& OutValue, FString& OutPayloadKey);
    bool GetUserDefinedAttribute_Float(const class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, float& OutValue, FString& OutPayloadKey);
    bool GetUserDefinedAttribute_Double(const class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, double& OutValue, FString& OutPayloadKey);
    bool GetUserDefinedAttribute_Boolean(const class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, bool& OutValue, FString& OutPayloadKey);
    void DuplicateAllUserDefinedAttribute(const class UInterchangeBaseNode* InterchangeSourceNode, class UInterchangeBaseNode* InterchangeDestinationNode, bool bAddSourceNodeName);
    bool CreateUserDefinedAttribute_Int32(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, const int32& Value, FString PayloadKey, bool RequiresDelegate);
    bool CreateUserDefinedAttribute_FString(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, FString Value, FString PayloadKey, bool RequiresDelegate);
    bool CreateUserDefinedAttribute_Float(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, const float& Value, FString PayloadKey, bool RequiresDelegate);
    bool CreateUserDefinedAttribute_Double(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, const double& Value, FString PayloadKey, bool RequiresDelegate);
    bool CreateUserDefinedAttribute_Boolean(class UInterchangeBaseNode* InterchangeNode, FString UserDefinedAttributeName, const bool& Value, FString PayloadKey, bool RequiresDelegate);
}; // Size: 0x28

class UInterchangeWriterBase : public UObject
{
}; // Size: 0x28

#endif
