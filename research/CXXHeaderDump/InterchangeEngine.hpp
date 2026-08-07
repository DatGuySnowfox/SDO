#ifndef UE4SS_SDK_InterchangeEngine_HPP
#define UE4SS_SDK_InterchangeEngine_HPP

#include "InterchangeEngine_enums.hpp"

struct FImportAssetParameters
{
    class UObject* ReimportAsset;                                                     // 0x0000 (size: 0x8)
    int32 ReimportSourceIndex;                                                        // 0x0008 (size: 0x4)
    bool bIsAutomated;                                                                // 0x000C (size: 0x1)
    bool bFollowRedirectors;                                                          // 0x000D (size: 0x1)
    TArray<FSoftObjectPath> OverridePipelines;                                        // 0x0010 (size: 0x10)
    FImportAssetParametersOnAssetDone OnAssetDone;                                    // 0x0020 (size: 0x10)
    void OnObjectImportDoneDynamic(class UObject* Object);
    FImportAssetParametersOnAssetsImportDone OnAssetsImportDone;                      // 0x0040 (size: 0x10)
    void OnImportDoneDynamic(const TArray<class UObject*>& Objects);
    FImportAssetParametersOnSceneObjectDone OnSceneObjectDone;                        // 0x0060 (size: 0x10)
    void OnObjectImportDoneDynamic(class UObject* Object);
    FImportAssetParametersOnSceneImportDone OnSceneImportDone;                        // 0x0080 (size: 0x10)
    void OnImportDoneDynamic(const TArray<class UObject*>& Objects);

}; // Size: 0xA0

struct FInterchangeContentImportSettings : public FInterchangeImportSettings
{
    TMap<class EInterchangeTranslatorAssetType, class FName> DefaultPipelineStackOverride; // 0x0088 (size: 0x50)
    TMap<class EInterchangeTranslatorAssetType, class bool> ShowPipelineStacksConfigurationDialogOverride; // 0x00D8 (size: 0x50)

}; // Size: 0x128

struct FInterchangeFilePickerParameters
{
    bool bAllowMultipleFiles;                                                         // 0x0000 (size: 0x1)
    FText Title;                                                                      // 0x0008 (size: 0x18)
    FString DefaultPath;                                                              // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FInterchangeImportSettings
{
    TMap<class FName, class FInterchangePipelineStack> PipelineStacks;                // 0x0000 (size: 0x50)
    FName DefaultPipelineStack;                                                       // 0x0050 (size: 0x8)
    TSoftClassPtr<UInterchangePipelineConfigurationBase> PipelineConfigurationDialogClass; // 0x0058 (size: 0x28)
    bool bShowPipelineStacksConfigurationDialog;                                      // 0x0080 (size: 0x1)

}; // Size: 0x88

struct FInterchangePipelineStack
{
    TArray<FSoftObjectPath> Pipelines;                                                // 0x0000 (size: 0x10)
    TArray<FInterchangeTranslatorPipelines> PerTranslatorPipelines;                   // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FInterchangeStackInfo
{
    FName StackName;                                                                  // 0x0000 (size: 0x8)
    TArray<class UInterchangePipelineBase*> Pipelines;                                // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FInterchangeTranslatorPipelines
{
    TSoftClassPtr<UInterchangeTranslatorBase> Translator;                             // 0x0000 (size: 0x28)
    TArray<FSoftObjectPath> Pipelines;                                                // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FPropertyData
{
}; // Size: 0x18

class UInterchangeAssetImportData : public UAssetImportData
{
    FSoftObjectPath SceneImportAsset;                                                 // 0x0028 (size: 0x20)
    FString NodeUniqueID;                                                             // 0x0048 (size: 0x10)
    class UInterchangeBaseNodeContainer* NodeContainer;                               // 0x0058 (size: 0x8)
    TArray<class UObject*> Pipelines;                                                 // 0x0060 (size: 0x10)
    class UInterchangeBaseNodeContainer* TransientNodeContainer;                      // 0x0070 (size: 0x8)
    TArray<class UObject*> TransientPipelines;                                        // 0x0078 (size: 0x10)

    void SetPipelines(const TArray<class UObject*>& InPipelines);
    void SetNodeContainer(class UInterchangeBaseNodeContainer* InNodeContainer);
    FString ScriptGetFirstFilename();
    TArray<FString> ScriptExtractFilenames();
    TArray<FString> ScriptExtractDisplayLabels();
    class UInterchangeBaseNode* GetStoredNode(FString InNodeUniqueId);
    class UInterchangeFactoryBaseNode* GetStoredFactoryNode(FString InNodeUniqueId);
    TArray<class UObject*> GetPipelines();
    int32 GetNumberOfPipelines();
    class UInterchangeBaseNodeContainer* GetNodeContainer();
}; // Size: 0xB0

class UInterchangeBlueprintPipelineBase : public UBlueprint
{
}; // Size: 0xA8

class UInterchangeFilePickerBase : public UObject
{

    bool ScriptedFilePickerForTranslatorType(const EInterchangeTranslatorType TranslatorType, FInterchangeFilePickerParameters& Parameters, TArray<FString>& OutFilenames);
    bool ScriptedFilePickerForTranslatorAssetType(const EInterchangeTranslatorAssetType TranslatorAssetType, FInterchangeFilePickerParameters& Parameters, TArray<FString>& OutFilenames);
}; // Size: 0x28

class UInterchangeManager : public UObject
{
    TSet<UClass*> RegisteredTranslatorsClass;                                         // 0x00D8 (size: 0x50)
    TMap<UClass*, UClass*> RegisteredFactoryClasses;                                  // 0x0128 (size: 0x50)
    TMap<UClass*, class UInterchangeWriterBase*> RegisteredWriters;                   // 0x0178 (size: 0x50)

    bool ImportScene(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters);
    bool ImportAsset(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters);
    UClass* GetRegisteredFactoryClass(const UClass* ClassToMake);
    class UInterchangeManager* GetInterchangeManagerScripted();
    bool ExportScene(const class UObject* World, bool bIsAutomated);
    bool ExportAsset(const class UObject* Asset, bool bIsAutomated);
    class UInterchangeSourceData* CreateSourceData(FString InFilename);
}; // Size: 0x1E0

class UInterchangeMeshUtilities : public UObject
{
}; // Size: 0x28

class UInterchangePipelineConfigurationBase : public UObject
{

    EInterchangePipelineConfigurationDialogResult ScriptedShowScenePipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<class UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData);
    EInterchangePipelineConfigurationDialogResult ScriptedShowReimportPipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<class UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData);
    EInterchangePipelineConfigurationDialogResult ScriptedShowPipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<class UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData);
}; // Size: 0x28

class UInterchangePipelineStackOverride : public UObject
{
    TArray<FSoftObjectPath> OverridePipelines;                                        // 0x0028 (size: 0x10)

    void AddPythonPipeline(class UInterchangePythonPipelineBase* PipelineBase);
    void AddPipeline(class UInterchangePipelineBase* PipelineBase);
    void AddBlueprintPipeline(class UInterchangeBlueprintPipelineBase* PipelineBase);
}; // Size: 0x38

class UInterchangeProjectSettings : public UDeveloperSettings
{
    FInterchangeContentImportSettings ContentImportSettings;                          // 0x0038 (size: 0x128)
    FInterchangeImportSettings SceneImportSettings;                                   // 0x0160 (size: 0x88)
    TSoftClassPtr<UInterchangeFilePickerBase> FilePickerClass;                        // 0x01E8 (size: 0x28)
    bool bStaticMeshUseSmoothEdgesIfSmoothingInformationIsMissing;                    // 0x0210 (size: 0x1)
    TSoftClassPtr<UInterchangePipelineBase> GenericPipelineClass;                     // 0x0218 (size: 0x28)

}; // Size: 0x240

class UInterchangePythonPipelineAsset : public UObject
{
    TSoftClassPtr<UInterchangePythonPipelineBase> PythonClass;                        // 0x0028 (size: 0x28)
    class UInterchangePythonPipelineBase* GeneratedPipeline;                          // 0x0050 (size: 0x8)
    FString JsonDefaultProperties;                                                    // 0x0058 (size: 0x10)

}; // Size: 0x68

class UInterchangePythonPipelineBase : public UInterchangePipelineBase
{
}; // Size: 0xE8

class UInterchangeSceneImportAsset : public UObject
{
}; // Size: 0x30

#endif
