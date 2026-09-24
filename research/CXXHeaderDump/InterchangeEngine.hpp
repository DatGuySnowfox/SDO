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
    class ULevel* ImportLevel;                                                        // 0x0020 (size: 0x8)
    FString DestinationName;                                                          // 0x0028 (size: 0x10)
    bool bReplaceExisting;                                                            // 0x0038 (size: 0x1)
    bool bForceShowDialog;                                                            // 0x0039 (size: 0x1)
    FImportAssetParametersOnAssetDone OnAssetDone;                                    // 0x003C (size: 0x10)
    void OnObjectImportDoneDynamic(class UObject* Object);
    FImportAssetParametersOnAssetsImportDone OnAssetsImportDone;                      // 0x0060 (size: 0x10)
    void OnImportDoneDynamic(const TArray<UObject*>& Objects);
    FImportAssetParametersOnSceneObjectDone OnSceneObjectDone;                        // 0x0080 (size: 0x10)
    void OnObjectImportDoneDynamic(class UObject* Object);
    FImportAssetParametersOnSceneImportDone OnSceneImportDone;                        // 0x00A0 (size: 0x10)
    void OnImportDoneDynamic(const TArray<UObject*>& Objects);

}; // Size: 0xC8

struct FInterchangeContentImportSettings : public FInterchangeImportSettings
{
    TMap<EInterchangeTranslatorAssetType, FName> DefaultPipelineStackOverride;        // 0x0088 (size: 0x50)
    TMap<EInterchangeTranslatorAssetType, FInterchangeDialogOverride> ShowImportDialogOverride; // 0x00D8 (size: 0x50)

}; // Size: 0x128

struct FInterchangeDialogOverride
{
    bool bShowImportDialog;                                                           // 0x0000 (size: 0x1)
    bool bShowReimportDialog;                                                         // 0x0001 (size: 0x1)
    TArray<FInterchangePerTranslatorDialogOverride> PerTranslatorImportDialogOverride; // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FInterchangeFilePickerParameters
{
    bool bAllowMultipleFiles;                                                         // 0x0000 (size: 0x1)
    FText Title;                                                                      // 0x0008 (size: 0x10)
    FString DefaultPath;                                                              // 0x0018 (size: 0x10)
    bool bShowAllFactoriesExtension;                                                  // 0x0028 (size: 0x1)
    TArray<FString> ExtraFormats;                                                     // 0x0030 (size: 0x10)

}; // Size: 0x40

struct FInterchangeGroup
{
    FName DisplayName;                                                                // 0x0000 (size: 0x8)
    FGuid UniqueID;                                                                   // 0x0008 (size: 0x10)
    FName DefaultPipelineStack;                                                       // 0x0018 (size: 0x8)
    TMap<EInterchangeTranslatorAssetType, FName> DefaultPipelineStackOverride;        // 0x0020 (size: 0x50)
    bool bShowImportDialog;                                                           // 0x0070 (size: 0x1)
    bool bShowReimportDialog;                                                         // 0x0071 (size: 0x1)
    TMap<EInterchangeTranslatorAssetType, FInterchangeDialogOverride> ShowImportDialogOverride; // 0x0078 (size: 0x50)

}; // Size: 0xC8

struct FInterchangeImportSettings
{
    TMap<FName, FInterchangePipelineStack> PipelineStacks;                            // 0x0000 (size: 0x50)
    FName DefaultPipelineStack;                                                       // 0x0050 (size: 0x8)
    TSoftClassPtr<UInterchangePipelineConfigurationBase> ImportDialogClass;           // 0x0058 (size: 0x28)
    bool bShowImportDialog;                                                           // 0x0080 (size: 0x1)
    bool bShowReimportDialog;                                                         // 0x0081 (size: 0x1)

}; // Size: 0x88

struct FInterchangePerTranslatorDialogOverride
{
    TSoftClassPtr<UInterchangeTranslatorBase> Translator;                             // 0x0000 (size: 0x28)
    bool bShowImportDialog;                                                           // 0x0028 (size: 0x1)
    bool bShowReimportDialog;                                                         // 0x0029 (size: 0x1)

}; // Size: 0x30

struct FInterchangePipelineStack
{
    TArray<FSoftObjectPath> Pipelines;                                                // 0x0000 (size: 0x10)
    TArray<FInterchangeTranslatorPipelines> PerTranslatorPipelines;                   // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FInterchangeSceneImportSettings : public FInterchangeImportSettings
{
    TArray<FInterchangePerTranslatorDialogOverride> PerTranslatorDialogOverride;      // 0x0088 (size: 0x10)

}; // Size: 0x98

struct FInterchangeStackInfo
{
    FName StackName;                                                                  // 0x0000 (size: 0x8)
    TArray<UInterchangePipelineBase*> Pipelines;                                      // 0x0008 (size: 0x10)

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
    TArray<UObject*> Pipelines;                                                       // 0x0060 (size: 0x10)
    class UInterchangeBaseNodeContainer* TransientNodeContainer;                      // 0x0070 (size: 0x8)
    TArray<UObject*> TransientPipelines;                                              // 0x0078 (size: 0x10)
    class UInterchangeTranslatorSettings* TransientTranslatorSettings;                // 0x0088 (size: 0x8)

    void SetTranslatorSettings(class UInterchangeTranslatorSettings* TranslatorSettings);
    void SetPipelines(const TArray<UObject*>& InPipelines);
    void SetNodeContainer(class UInterchangeBaseNodeContainer* InNodeContainer);
    FString ScriptGetFirstFilename();
    TArray<FString> ScriptExtractFilenames();
    TArray<FString> ScriptExtractDisplayLabels();
    class UInterchangeTranslatorSettings* GetTranslatorSettings();
    class UInterchangeBaseNode* GetStoredNode(FString InNodeUniqueId);
    class UInterchangeFactoryBaseNode* GetStoredFactoryNode(FString InNodeUniqueId);
    TArray<UObject*> GetPipelines();
    int32 GetNumberOfPipelines();
    class UInterchangeBaseNodeContainer* GetNodeContainer();
}; // Size: 0xD8

class UInterchangeAssetImportDataConverterBase : public UObject
{
}; // Size: 0x28

class UInterchangeBlueprintPipelineBase : public UBlueprint
{
}; // Size: 0xA8

class UInterchangeEditorSettings : public UDeveloperSettings
{
    bool bShowImportDialogAtReimport;                                                 // 0x0038 (size: 0x1)
    FName UsedGroupName;                                                              // 0x003C (size: 0x8)
    FGuid UsedGroupUID;                                                               // 0x0044 (size: 0x10)

    void SetUsedGroupName(const FName& InUsedGroupName);
    FName GetUsedGroupName();
    TArray<FName> GetSelectableItems();
}; // Size: 0x58

class UInterchangeEditorUtilitiesBase : public UObject
{
}; // Size: 0x28

class UInterchangeFilePickerBase : public UObject
{

    bool ScriptedFilePickerForTranslatorType(const EInterchangeTranslatorType TranslatorType, FInterchangeFilePickerParameters& Parameters, TArray<FString>& OutFilenames);
    bool ScriptedFilePickerForTranslatorAssetType(const EInterchangeTranslatorAssetType TranslatorAssetType, FInterchangeFilePickerParameters& Parameters, TArray<FString>& OutFilenames);
}; // Size: 0x28

class UInterchangeManager : public UObject
{
    TSet<UClass*> RegisteredTranslatorsClass;                                         // 0x0238 (size: 0x50)
    TMap<UClass*, UClass*> RegisteredFactoryClasses;                                  // 0x0288 (size: 0x50)
    TMap<UClass*, UInterchangeWriterBase*> RegisteredWriters;                         // 0x02D8 (size: 0x50)
    TMap<UClass*, UInterchangeAssetImportDataConverterBase*> RegisteredConverters;    // 0x0328 (size: 0x50)

    bool WarnIfInterchangeIsActive();
    void WaitUntilAllTasksDone(bool bCancel);
    bool ScriptedReimportAssetAsync(class UObject* ObjectToReimport, const FImportAssetParameters& ImportAssetParameters);
    bool ScriptedImportSceneAsync(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters);
    bool ScriptedImportAssetAsync(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters);
    bool ReimportAsset(class UObject* ObjectToReimport, const FImportAssetParameters& ImportAssetParameters, TArray<UObject*>& OutImportedObjects);
    bool IsObjectBeingImported(class UObject* Object);
    bool IsInterchangeActive();
    bool ImportScene(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters);
    bool ImportAsset(FString ContentPath, const class UInterchangeSourceData* SourceData, const FImportAssetParameters& ImportAssetParameters, TArray<UObject*>& OutImportedObjects);
    class UInterchangeTranslatorBase* GetTranslatorForSourceData(const class UInterchangeSourceData* SourceData);
    TArray<FString> GetSupportedFormatsForObject(const class UObject* Object, int32 SourceFileIndex);
    TArray<FString> GetSupportedFormats(const EInterchangeTranslatorType ForTranslatorType);
    TArray<FString> GetSupportedAssetTypeFormats(const EInterchangeTranslatorAssetType ForTranslatorAssetType, const EInterchangeTranslatorType ForTranslatorType, bool bStrictMatchTranslatorType);
    UClass* GetRegisteredFactoryClass(const UClass* ClassToMake);
    class UInterchangeManager* GetInterchangeManagerScripted();
    class UInterchangeAssetImportData* GetAssetImportData(class UObject* Asset);
    bool ExportScene(const class UObject* World, bool bIsAutomated);
    bool ExportAsset(const class UObject* Asset, bool bIsAutomated);
    class UInterchangeSourceData* CreateSourceData(FString InFilename);
    bool CanTranslateSourceData(const class UInterchangeSourceData* SourceData, bool bSceneImportOnly);
    bool CanReimport(const class UObject* Object, TArray<FString>& OutFilenames);
    void CancelAllTasks();
}; // Size: 0x400

class UInterchangeMeshUtilities : public UObject
{

    bool ScriptedImportMorphTarget(class USkeletalMesh* SkeletalMesh, const int32 LODIndex, const class UInterchangeSourceData* SourceData, FString MorphTargetName);
}; // Size: 0x28

class UInterchangePipelineConfigurationBase : public UObject
{

    EInterchangePipelineConfigurationDialogResult ScriptedShowTestPlanConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData, class UInterchangeTranslatorBase* Translator, class UInterchangeBaseNodeContainer* BaseNodeContainer, class UObject* ReimportAsset, bool bSceneImport, bool bReimport);
    EInterchangePipelineConfigurationDialogResult ScriptedShowScenePipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData, class UInterchangeTranslatorBase* Translator, class UInterchangeBaseNodeContainer* BaseNodeContainer);
    EInterchangePipelineConfigurationDialogResult ScriptedShowReimportPipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData, class UInterchangeTranslatorBase* Translator, class UInterchangeBaseNodeContainer* BaseNodeContainer, class UObject* ReimportAsset, bool bSceneImport);
    EInterchangePipelineConfigurationDialogResult ScriptedShowPipelineConfigurationDialog(TArray<FInterchangeStackInfo>& PipelineStacks, TArray<UInterchangePipelineBase*>& OutPipelines, class UInterchangeSourceData* SourceData, class UInterchangeTranslatorBase* Translator, class UInterchangeBaseNodeContainer* BaseNodeContainer);
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
    FInterchangeSceneImportSettings SceneImportSettings;                              // 0x0160 (size: 0x98)
    TSoftClassPtr<UInterchangeFilePickerBase> FilePickerClass;                        // 0x01F8 (size: 0x28)
    bool bStaticMeshUseSmoothEdgesIfSmoothingInformationIsMissing;                    // 0x0220 (size: 0x1)
    TSoftClassPtr<UInterchangePipelineBase> GenericPipelineClass;                     // 0x0228 (size: 0x28)
    FSoftObjectPath ConverterDefaultPipeline;                                         // 0x0250 (size: 0x20)
    TArray<FInterchangeGroup> InterchangeGroups;                                      // 0x0270 (size: 0x10)

}; // Size: 0x280

class UInterchangePythonPipelineAsset : public UObject
{
    TSoftClassPtr<UInterchangePythonPipelineBase> PythonClass;                        // 0x0028 (size: 0x28)
    class UInterchangePythonPipelineBase* GeneratedPipeline;                          // 0x0050 (size: 0x8)
    FString JsonDefaultProperties;                                                    // 0x0058 (size: 0x10)

}; // Size: 0x68

class UInterchangePythonPipelineBase : public UInterchangePipelineBase
{
}; // Size: 0x128

class UInterchangeSceneImportAsset : public UObject
{
}; // Size: 0x30

#endif
