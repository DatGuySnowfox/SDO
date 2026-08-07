#ifndef UE4SS_SDK_BFL_SaveGames_HPP
#define UE4SS_SDK_BFL_SaveGames_HPP

class UBFL_SaveGames_C : public UBlueprintFunctionLibrary
{

    void SaveObjects(class UObject* __WorldContext);
    void ZeroFill(int32 Int, class UObject* __WorldContext, FString& Out);
    FString DateToString_AutoSave(FDateTime InDateTime, class UObject* __WorldContext);
    void DateMonth(int32 Month, class UObject* __WorldContext, FName& Date);
    void Reset Player Stats(class ABP_PlayerCharacter_C* Player, class UObject* __WorldContext);
    void SaveThumbnailImage(class UObject* __WorldContext);
    FString DateStringPrefix(int32 A, class UObject* __WorldContext);
    FString DateToString(FDateTime InDateTime, class UObject* __WorldContext);
}; // Size: 0x28

#endif
