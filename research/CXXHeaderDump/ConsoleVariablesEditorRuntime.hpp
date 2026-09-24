#ifndef UE4SS_SDK_ConsoleVariablesEditorRuntime_HPP
#define UE4SS_SDK_ConsoleVariablesEditorRuntime_HPP

struct FConsoleVariablesEditorAssetSaveData
{
    FString CommandName;                                                              // 0x0000 (size: 0x10)
    FString CommandValueAsString;                                                     // 0x0010 (size: 0x10)
    ECheckBoxState CheckedState;                                                      // 0x0020 (size: 0x1)

}; // Size: 0x28

class UConsoleVariablesAsset : public UObject
{
    FString VariableCollectionDescription;                                            // 0x0030 (size: 0x10)
    TArray<FConsoleVariablesEditorAssetSaveData> SavedCommands;                       // 0x0040 (size: 0x10)

    void SetVariableCollectionDescription(FString InVariableCollectionDescription);
    void ReplaceSavedCommands(const TArray<FConsoleVariablesEditorAssetSaveData>& Replacement);
    bool RemoveConsoleVariable(FString InCommandString);
    FString GetVariableCollectionDescription();
    int32 GetSavedCommandsCount();
    TArray<FString> GetSavedCommandsAsStringArray(bool bOnlyIncludeChecked);
    FString GetSavedCommandsAsCommaSeparatedString(bool bOnlyIncludeChecked);
    TArray<FConsoleVariablesEditorAssetSaveData> GetSavedCommands();
    bool FindSavedDataByCommandString(FString InCommandString, FConsoleVariablesEditorAssetSaveData& OutValue, const TEnumAsByte<ESearchCase::Type> SearchCase);
    void ExecuteSavedCommands(class UObject* WorldContextObject, bool bOnlyIncludeChecked);
    void CopyFrom(const class UConsoleVariablesAsset* InAssetToCopy);
    void AddOrSetConsoleObjectSavedData(const FConsoleVariablesEditorAssetSaveData& InData);
    void AddFrom(const class UConsoleVariablesAsset* InAssetToCopy);
}; // Size: 0x50

#endif
