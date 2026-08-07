#ifndef UE4SS_SDK_PhotoModeFunctionLibrary_HPP
#define UE4SS_SDK_PhotoModeFunctionLibrary_HPP

class UPhotoModeFunctionLibrary_C : public UBlueprintFunctionLibrary
{

    void Hide HUD When Using Photo Mode(ESlateVisibility If Not Using Photo Mode Set This Widget As, class UPhotoModeComponent_C* PhotoModeComponent, class UObject* __WorldContext, ESlateVisibility& Visibility, bool& Using Photo Mode or Gallery?);
    void SaveAllPhotos(TArray<FPhotosData>& SaveNewPhoto, class UObject* __WorldContext);
    void MakeThumbnail(class UObject* World Context Object (Self), class APlayerController* Player Controller, FString Screenshot Folder Path, FIntPoint Resolution (X and Y), FString PhotoName, class UObject* __WorldContext, FString& Full Thumbnail Path, FString& BaseThumbnailName);
    void LoadPhotos(class UObject* __WorldContext, bool& SaveGameIsValid?, TArray<FPhotosData>& PhotosData);
    void SavePhoto(FPhotosData SaveNewPhoto, class UObject* __WorldContext);
    void NormalizeScreenshotPath(FString Folder Path, class UObject* __WorldContext, FString& Normalized Path);
    void Get Screenshot File(class UObject* WorldContextObject, FString FullFilePath, class UObject* __WorldContext, bool& IsValid?, class UTexture2D*& 2D Image, FString& Base Filename, FString& Full File Path);
    void Directory Path Correction(bool Use Custom Path?, FString Custom Path, class UObject* __WorldContext, FString& Corrected Path);
    void Get Default Screen Resolution(class UObject* __WorldContext, FIntPoint& Resolution (X and Y));
    void HighResScreenshot (without UI)(class UObject* World Context Object (Self), class APlayerController* Player Controller, FString Screenshot Folder Path, FIntPoint Resolution (X and Y), class UObject* __WorldContext, FString& Full Photo Path, FString& Photo Name);
    void Screenshot (with UI)(class UObject* World Context Object (Self), class APlayerController* Player Controller, FString Screenshot Folder Path, class UObject* __WorldContext, FString& Full Screenshot File Path, FString& PhotoName);
}; // Size: 0x28

#endif
