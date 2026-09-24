#ifndef UE4SS_SDK_NamingTokens_HPP
#define UE4SS_SDK_NamingTokens_HPP

struct FNamingTokenData
{
    FString TokenKey;                                                                 // 0x0000 (size: 0x10)
    FText DisplayName;                                                                // 0x0010 (size: 0x10)
    FText Description;                                                                // 0x0020 (size: 0x10)
    FName FunctionName;                                                               // 0x0030 (size: 0x8)

}; // Size: 0x48

struct FNamingTokenFilterArgs
{
    TArray<FString> AdditionalNamespacesToInclude;                                    // 0x0000 (size: 0x10)
    bool bIncludeGlobal;                                                              // 0x0010 (size: 0x1)
    bool bForceCaseSensitive;                                                         // 0x0011 (size: 0x1)
    bool bNativeOnly;                                                                 // 0x0012 (size: 0x1)

}; // Size: 0x18

struct FNamingTokenResultData
{
    FText OriginalText;                                                               // 0x0000 (size: 0x10)
    FText EvaluatedText;                                                              // 0x0010 (size: 0x10)
    TArray<FNamingTokenValueData> TokenValues;                                        // 0x0020 (size: 0x10)

}; // Size: 0x30

struct FNamingTokenValueData
{
    FString TokenKey;                                                                 // 0x0000 (size: 0x10)
    FString ProvidedNamespace;                                                        // 0x0010 (size: 0x10)
    FText TokenValue;                                                                 // 0x0020 (size: 0x10)
    bool bWasEvaluated;                                                               // 0x0030 (size: 0x1)

}; // Size: 0x38

struct FNamingTokensEvaluationData
{
    FDateTime CurrentDateTime;                                                        // 0x0000 (size: 0x8)
    TArray<UObject*> Contexts;                                                        // 0x0008 (size: 0x10)

}; // Size: 0x20

class UGlobalNamingTokens : public UNamingTokens
{
}; // Size: 0xF8

class UNamingTokens : public UObject
{
    TArray<FNamingTokenData> CustomTokens;                                            // 0x00B8 (size: 0x10)
    FNamingTokensEvaluationData CurrentEvaluationData;                                // 0x00C8 (size: 0x20)
    FString Namespace;                                                                // 0x00E8 (size: 0x10)

    FText ProcessTokenTemplateFunction();
    void OnPreEvaluate(const FNamingTokensEvaluationData& InEvaluationData);
    void OnPostEvaluate();
    FDateTime GetCurrentDateTime();
}; // Size: 0xF8

class UNamingTokensEngineSubsystem : public UEngineSubsystem
{
    TMap<FString, UNamingTokens*> CachedNamingTokens;                                 // 0x0030 (size: 0x50)

    void UnregisterGlobalNamespace(FString InNameSpace);
    void RegisterGlobalNamespace(FString InNameSpace);
    bool IsGlobalNamespaceRegistered(FString InNameSpace);
    class UNamingTokens* GetNamingTokensNative(FString InNameSpace);
    class UNamingTokens* GetNamingTokens(FString InNameSpace);
    TArray<UNamingTokens*> GetMultipleNamingTokens(const TArray<FString>& InNamespaces);
    TArray<FString> GetGlobalNamespaces();
    TArray<FString> GetAllNamespaces();
    FNamingTokenResultData EvaluateTokenText(const FText& InTokenText, const FNamingTokenFilterArgs& InFilter, const TArray<UObject*>& InContexts);
    FNamingTokenResultData EvaluateTokenString(FString InTokenString, const FNamingTokenFilterArgs& InFilter, const TArray<UObject*>& InContexts);
    void ClearCachedNamingTokens();
}; // Size: 0x150

#endif
