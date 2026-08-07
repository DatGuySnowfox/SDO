#ifndef UE4SS_SDK_BlueprintFileUtils_HPP
#define UE4SS_SDK_BlueprintFileUtils_HPP

class UBlueprintFileUtilsBPLibrary : public UBlueprintFunctionLibrary
{

    bool MoveFile(FString DestFilename, FString SrcFilename, bool bReplace, bool bEvenIfReadOnly);
    bool MakeDirectory(FString Path, bool bCreateTree);
    FString GetUserDirectory();
    bool FindRecursive(FString StartDirectory, TArray<FString>& FoundPaths, FString Wildcard, bool bFindFiles, bool bFindDirectories);
    bool FindFiles(FString Directory, TArray<FString>& FoundFiles, FString FileExtension);
    bool FileExists(FString Filename);
    bool DirectoryExists(FString Directory);
    bool DeleteFile(FString Filename, bool bMustExist, bool bEvenIfReadOnly);
    bool DeleteDirectory(FString Directory, bool bMustExist, bool bDeleteRecursively);
    bool CopyFile(FString DestFilename, FString SrcFilename, bool bReplace, bool bEvenIfReadOnly);
}; // Size: 0x28

#endif
