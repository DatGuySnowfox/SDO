#ifndef UE4SS_SDK_SteamCoreWeb_HPP
#define UE4SS_SDK_SteamCoreWeb_HPP

#include "SteamCoreWeb_enums.hpp"

struct FModifyItemsItem
{
    FString ItemId;                                                                   // 0x0000 (size: 0x10)
    FString PropertyName;                                                             // 0x0010 (size: 0x10)
    EModifyItemPropertyType ModifyItemPropertyType;                                   // 0x0020 (size: 0x1)
    FString PropertyValueString;                                                      // 0x0028 (size: 0x10)
    bool PropertyValueBool;                                                           // 0x0038 (size: 0x1)
    int32 PropertyValueInt;                                                           // 0x003C (size: 0x4)
    int32 PropertyValueFloat;                                                         // 0x0040 (size: 0x4)
    bool PropertyValueRemove;                                                         // 0x0044 (size: 0x1)

}; // Size: 0x48

struct FSteamCoreJson
{
    FString Key;                                                                      // 0x0000 (size: 0x10)
    FString String;                                                                   // 0x0010 (size: 0x10)
    int32 Number;                                                                     // 0x0020 (size: 0x4)
    bool bool;                                                                        // 0x0024 (size: 0x1)
    ESteamValueType Type;                                                             // 0x0025 (size: 0x1)

}; // Size: 0x28

struct FWebAppsGetAppList
{
    FString AppID;                                                                    // 0x0000 (size: 0x10)
    FString Name;                                                                     // 0x0010 (size: 0x10)

}; // Size: 0x20

class USteamCoreAppsAsyncActionGetAppBetas : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetAppBetas* GetAppBetasAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetAppBuilds : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetAppBuilds* GetAppBuildsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 Count);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetAppDepotVersions : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetAppDepotVersions* GetAppDepotVersionsAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetAppList : public UBlueprintAsyncActionBase
{
    FSteamCoreAppsAsyncActionGetAppListOnCallback OnCallback;                         // 0x0030 (size: 0x10)
    void OnSteamCoreWebAppListAsyncCallback(const TArray<FWebAppsGetAppList>& Data, bool bWasSuccessful);

    void HandleCallback(const TArray<FWebAppsGetAppList>& Data, bool bWasSuccessful);
    class USteamCoreAppsAsyncActionGetAppList* GetAppListAsync(class UObject* WorldContextObject);
}; // Size: 0x50

class USteamCoreAppsAsyncActionGetCheatingReports : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetCheatingReports* GetCheatingReportsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 TimeBegin, int32 TimeEnd, bool bIncludeReports, bool bIncludeBans, int32 ReportidMin);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetPlayersBanned : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetPlayersBanned* GetPlayersBannedAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetServerList : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetServerList* GetServerListAsync(class UObject* WorldContextObject, FString Key, FString Filter, int32 Limit);
}; // Size: 0x58

class USteamCoreAppsAsyncActionGetServersAtAddress : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionGetServersAtAddress* GetServersAtAddressAsync(class UObject* WorldContextObject, FString Addr);
}; // Size: 0x58

class USteamCoreAppsAsyncActionSetAppBuildLive : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionSetAppBuildLive* SetAppBuildLiveAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 BuildID, FString BetaKey, FString Description);
}; // Size: 0x58

class USteamCoreAppsAsyncActionUpToDateCheck : public USteamCoreWebAsyncAction
{

    class USteamCoreAppsAsyncActionUpToDateCheck* UpToDateCheckAsync(class UObject* WorldContextObject, int32 AppID, int32 Version);
}; // Size: 0x58

class USteamCoreWeb : public UObject
{
}; // Size: 0x28

class USteamCoreWebAsyncAction : public UBlueprintAsyncActionBase
{
    FSteamCoreWebAsyncActionOnCallback OnCallback;                                    // 0x0030 (size: 0x10)
    void OnSteamCoreWebAsyncCallback(FString Data, bool bWasSuccessful);

    void HandleCallback(FString Data, bool bWasSuccessful);
}; // Size: 0x58

class USteamCoreWebAsyncActionAddItem : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionAddItem* AddItemAsync(class UObject* WorldContextObject, FString Key, int32 AppID, TArray<int32> ItemdefId, FString ItemPropsJson, TArray<int32> ItemQuantity, FString SteamID, bool bNotify, FString RequestID, bool bTradeRestriction);
}; // Size: 0x58

class USteamCoreWebAsyncActionAddPromoItem : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionAddPromoItem* AddPromoItemAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 ItemdefId, FString ItemPropsJson, FString SteamID, bool bNotify, FString RequestID);
}; // Size: 0x58

class USteamCoreWebAsyncActionAdjustAgreement : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionAdjustAgreement* AdjustAgreementAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString AgreementId, int32 AppID, FString NextProcessDate);
}; // Size: 0x58

class USteamCoreWebAsyncActionAuthenticateUser : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionAuthenticateUser* AuthenticateUserAsync(class UObject* WorldContextObject, FString SteamID, TArray<uint8> SessionKey, TArray<uint8> EncryptedLoginKey);
}; // Size: 0x58

class USteamCoreWebAsyncActionAuthenticateUserTicket : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionAuthenticateUserTicket* AuthenticateUserTicketAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Ticket);
}; // Size: 0x58

class USteamCoreWebAsyncActionCanTrade : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCanTrade* CanTradeAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString TargetId);
}; // Size: 0x58

class USteamCoreWebAsyncActionCancelAgreement : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCancelAgreement* CancelAgreementAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString AgreementId, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionCancelAppListingsForUser : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCancelAppListingsForUser* CancelAppListingsForUserAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, bool bSynchronous);
}; // Size: 0x58

class USteamCoreWebAsyncActionCancelTradeOffer : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCancelTradeOffer* CancelTradeOfferAsync(class UObject* WorldContextObject, FString Key, FString TradeOfferId);
}; // Size: 0x58

class USteamCoreWebAsyncActionCheckAppOwnership : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCheckAppOwnership* CheckAppOwnershipAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionConsolidate : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionConsolidate* ConsolidateAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, TArray<int32> ItemdefIDs, bool bForce);
}; // Size: 0x58

class USteamCoreWebAsyncActionConsumeItem : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionConsumeItem* ConsumeItemAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString ItemId, FString Quantity, FString SteamID, FString RequestID);
}; // Size: 0x58

class USteamCoreWebAsyncActionCreateSession : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionCreateSession* CreateSessionAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Context, FString Title, FString Users, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionDeclineTradeOffer : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionDeclineTradeOffer* DeclineTradeOfferAsync(class UObject* WorldContextObject, FString Key, FString TradeOfferId);
}; // Size: 0x58

class USteamCoreWebAsyncActionDeleteLeaderboard : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionDeleteLeaderboard* DeleteLeaderboardAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Name);
}; // Size: 0x58

class USteamCoreWebAsyncActionDeleteSession : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionDeleteSession* DeleteSessionAsync(class UObject* WorldContextObject, FString Key, FString SessionID, int32 AppID, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionDeleteSessionBatch : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionDeleteSessionBatch* DeleteSessionBatchAsync(class UObject* WorldContextObject, FString Key, FString SessionID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionEndSecureMultiplayerSession : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionEndSecureMultiplayerSession* EndSecureMultiplayerSessionAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString SessionID);
}; // Size: 0x58

class USteamCoreWebAsyncActionEnumerateSessionsForApp : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionEnumerateSessionsForApp* EnumerateSessionsForAppAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, bool bIncludeAllUserMessages, bool bIncludeAuthUserMessage, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionEnumerateUserPublishedFiles : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionEnumerateUserPublishedFiles* EnumerateUserPublishedFilesAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionEnumerateUserSubscribedFiles : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionEnumerateUserSubscribedFiles* EnumerateUserSubscribedFilesAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, int32 ListType);
}; // Size: 0x58

class USteamCoreWebAsyncActionExchangeItem : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionExchangeItem* ExchangeItemAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, TArray<int64> MaterialsItemID, TArray<int32> MaterialsQuantity, int64 OutputItemdefId);
}; // Size: 0x58

class USteamCoreWebAsyncActionFinalizeAssetTransaction : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFinalizeAssetTransaction* FinalizeAssetTransactionAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString TxnId, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionFinalizeTxn : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFinalizeTxn* FinalizeTxnAsync(class UObject* WorldContextObject, FString Key, FString OrderID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionFindOrCreateLeaderboard : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFindOrCreateLeaderboard* FindOrCreateLeaderboardAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Name, FString SortMethod, FString DisplayType, bool bCreateIfNotFound, bool bOnlyTrustedWrites, bool bOnlyFriendsReads);
}; // Size: 0x58

class USteamCoreWebAsyncActionFlushAssetAppearanceCache : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFlushAssetAppearanceCache* FlushAssetAppearanceCacheAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionFlushContextCache : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFlushContextCache* FlushContextCacheAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionFlushInventoryCache : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionFlushInventoryCache* FlushInventoryCacheAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString ContextID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetAppPriceInfo : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetAppPriceInfo* GetAppPriceInfoAsync(class UObject* WorldContextObject, FString Key, FString SteamID, TArray<int32> AppIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetAssetClassInfo : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetAssetClassInfo* GetAssetClassInfoAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Language, int32 ClassCount, FString ClassID, FString InstanceId);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetAssetID : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetAssetID* GetAssetIDAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString ListingId);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetAssetPrices : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetAssetPrices* GetAssetPricesAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Currency, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetBadges : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetBadges* GetBadgesAsync(class UObject* WorldContextObject, FString Key, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetCheatingReports : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetCheatingReports* GetCheatingReportsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 TimeEnd, int32 TimeBegin, FString ReportidMin, bool bIncludeReports, bool bIncludeBans, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetCollectionDetails : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetCollectionDetails* GetCollectionDetailsAsync(class UObject* WorldContextObject, TArray<FString> PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetCommunityBadgeProgress : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetCommunityBadgeProgress* GetCommunityBadgeProgressAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 BadgeId);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetExportedAssetsForUser : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetExportedAssetsForUser* GetExportedAssetsForUserAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString ContextID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetFriendList : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetFriendList* GetFriendListAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString Relationship);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetGlobalAchievementPercentagesForApp : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetGlobalAchievementPercentagesForApp* GetGlobalAchievementPercentagesForAppAsync(class UObject* WorldContextObject, FString GameID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetGlobalStatsForGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetGlobalStatsForGame* GetGlobalStatsForGameAsync(class UObject* WorldContextObject, int32 AppID, TArray<FString> Names, int32 StartDate, int32 EndDate);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetHistoryCommandDetails : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetHistoryCommandDetails* GetHistoryCommandDetailsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString Command, FString ContextID, FString Arguments);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetInventory : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetInventory* GetInventoryAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetItemDefs : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetItemDefs* GetItemDefsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString ModifiedSince, TArray<int32> ItemdefIDs, TArray<int32> WorkshopIDs, int32 CacheMaxAgeSeconds);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetLeaderboardEntries : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetLeaderboardEntries* GetLeaderboardEntriesAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 RangeStart, int32 RangeEnd, int32 LeaderboardId, int32 DataRequest, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetLeaderboardsForGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetLeaderboardsForGame* GetLeaderboardsForGameAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetMarketEligibility : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetMarketEligibility* GetMarketEligibilityAsync(class UObject* WorldContextObject, FString Key, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetMarketPrices : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetMarketPrices* GetMarketPricesAsync(class UObject* WorldContextObject, FString Key, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetNewsForApp : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetNewsForApp* GetNewsForAppAsync(class UObject* WorldContextObject, int32 AppID, int32 MaxLength, TArray<FString> Feeds, int32 EndDate, int32 Count);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetNewsForAppAuthed : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetNewsForAppAuthed* GetNewsForAppAuthedAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 MaxLength, TArray<FString> Feeds, int32 EndDate, int32 Count);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetNumberOfCurrentPlayers : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetNumberOfCurrentPlayers* GetNumberOfCurrentPlayersAsync(class UObject* WorldContextObject, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetOwnedGames : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetOwnedGames* GetOwnedGamesAsync(class UObject* WorldContextObject, FString Key, FString SteamID, bool bIncludeAppInfo, bool bIncludePlayedFreeGames, TArray<int32> Filter);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPlayerAchievements : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPlayerAchievements* GetPlayerAchievementsAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPlayerBans : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPlayerBans* GetPlayerBansAsync(class UObject* WorldContextObject, FString Key, TArray<FString> SteamIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPlayerSummaries : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPlayerSummaries* GetPlayerSummariesAsync(class UObject* WorldContextObject, FString Key, TArray<FString> SteamIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPopular : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPopular* GetPopularAsync(class UObject* WorldContextObject, FString Key, FString Language, int32 Rows, int32 Start, int32 FilterAppId, int32 ECurrency);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPriceSheet : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPriceSheet* GetPriceSheetAsync(class UObject* WorldContextObject, FString Key, int32 Currency);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPublishedFileDetails : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPublishedFileDetails* GetPublishedFileDetailsAsync(class UObject* WorldContextObject, FString PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPublisherAppOwnership : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPublisherAppOwnership* GetPublisherAppOwnershipAsync(class UObject* WorldContextObject, FString Key, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetPublisherAppOwnershipChanges : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetPublisherAppOwnershipChanges* GetPublisherAppOwnershipChangesAsync(class UObject* WorldContextObject, FString Key, FString PackageRowVersion, FString CDKeyRowVersion);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetQuantity : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetQuantity* GetQuantityAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, TArray<int32> ItemdefIDs, bool bForce);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetRecentlyPlayedGames : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetRecentlyPlayedGames* GetRecentlyPlayedGamesAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 Count);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetReport : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetReport* GetReportAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString Time, FString Type, int32 MaxResults);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetSchemaForGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetSchemaForGame* GetSchemaForGameAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetSessionDetailsForApp : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetSessionDetailsForApp* GetSessionDetailsForAppAsync(class UObject* WorldContextObject, FString Key, FString Sessions, int32 AppID, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetSteamLevel : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetSteamLevel* GetSteamLevelAsync(class UObject* WorldContextObject, FString Key, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetTradeHistory : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetTradeHistory* GetTradeHistoryAsync(class UObject* WorldContextObject, FString Key, int32 MaxTrades, int32 StartAfterTime, FString StartAfterTradeId, bool bNavigatingBack, bool bGetDescriptions, FString Language, bool bIncludeFailed, bool bIncludeTotal);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetTradeOffer : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetTradeOffer* GetTradeOfferAsync(class UObject* WorldContextObject, FString Key, FString TradeOfferId, FString Language);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetTradeOffers : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetTradeOffers* GetTradeOffersAsync(class UObject* WorldContextObject, FString Key, bool bGetSentOffers, bool bGetReceivedOffers, bool bGetDescriptions, FString Language, bool bActiveOnly, bool bHistoricalOnly, int32 TimeHistoricalCutoff);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetTradeOffersSummary : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetTradeOffersSummary* GetTradeOffersSummaryAsync(class UObject* WorldContextObject, FString Key, int32 TimeLastVisit);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUGCFileDetails : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUGCFileDetails* GetUGCFileDetailsAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString UGCID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUserAgreementInfo : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUserAgreementInfo* GetUserAgreementInfoAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUserGroupList : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUserGroupList* GetUserGroupListAsync(class UObject* WorldContextObject, FString Key, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUserHistory : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUserHistory* GetUserHistoryAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString ContextID, int32 StartTime, int32 EndTime);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUserInfo : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUserInfo* GetUserInfoAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString Ipaddress);
}; // Size: 0x58

class USteamCoreWebAsyncActionGetUserStatsForGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGetUserStatsForGame* GetUserStatsForGameAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionGrantPackage : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionGrantPackage* GrantPackageAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 PackageId, FString Ipaddress, FString ThirdPartyKey, int32 ThirdPartyAppId);
}; // Size: 0x58

class USteamCoreWebAsyncActionHistoryExecuteCommand : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionHistoryExecuteCommand* HistoryExecuteCommandAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString ContextID, int32 ActorId);
}; // Size: 0x58

class USteamCoreWebAsyncActionInitTxn : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionInitTxn* InitTxnAsync(class UObject* WorldContextObject, FString Key, FString OrderID, FString SteamID, int32 AppID, FString Language, FString Currency, FString UserSession, FString Ipaddress, TArray<int32> ItemId, TArray<int32> Quantity, TArray<FString> Amount, TArray<FString> Description, TArray<FString> Category, TArray<int32> AssociatedBundle, TArray<FString> BillingType, TArray<FString> StartDate, TArray<FString> EndDate, TArray<FString> Period, TArray<int32> Frequency, TArray<FString> RecurringAmt, TArray<int32> BundleCount, TArray<int32> BundleId, TArray<int32> BundleQty, TArray<FString> BundleDesc, TArray<FString> BundleCategory);
}; // Size: 0x58

class USteamCoreWebAsyncActionIsPlayingSharedGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionIsPlayingSharedGame* IsPlayingSharedGameAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppIdPlaying);
}; // Size: 0x58

class USteamCoreWebAsyncActionItemVoteSummary : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionItemVoteSummary* ItemVoteSummaryAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, TArray<FString> PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionModifyItems : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionModifyItems* ModifyItemsAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FModifyItemsItem Updates);
}; // Size: 0x58

class USteamCoreWebAsyncActionPostGameDataFrame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionPostGameDataFrame* PostGameDataFrameAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString BroadcastId, FString FrameData);
}; // Size: 0x58

class USteamCoreWebAsyncActionProcessAgreement : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionProcessAgreement* ProcessAgreementAsync(class UObject* WorldContextObject, FString Key, FString OrderID, FString SteamID, FString AgreementId, int32 AppID, int32 Amount, FString Currency);
}; // Size: 0x58

class USteamCoreWebAsyncActionQueryTxn : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionQueryTxn* QueryTxnAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString OrderID, FString TransId);
}; // Size: 0x58

class USteamCoreWebAsyncActionRankedByPublicationOrder : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRankedByPublicationOrder* RankedByPublicationOrderAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
}; // Size: 0x58

class USteamCoreWebAsyncActionRankedByTrend : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRankedByTrend* RankedByTrendAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, int32 Days, TArray<FString> Tag, TArray<FString> UserTag);
}; // Size: 0x58

class USteamCoreWebAsyncActionRankedByVote : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRankedByVote* RankedByVoteAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
}; // Size: 0x58

class USteamCoreWebAsyncActionRefundTxn : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRefundTxn* RefundTxnAsync(class UObject* WorldContextObject, FString Key, FString OrderID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionRemovePlayerGameBan : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRemovePlayerGameBan* RemovePlayerGameBanAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionReportAbuse : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionReportAbuse* ReportAbuseAsync(class UObject* WorldContextObject, FString Key, FString SteamIdActor, FString SteamIdTarget, int32 AppID, int32 AbuseType, int32 ContentType, FString Description, FString Gid);
}; // Size: 0x58

class USteamCoreWebAsyncActionReportCheatData : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionReportCheatData* ReportCheatDataAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString PathAndFileName, FString WebCheatURL, FString TimeNow, FString TimeStarted, FString TimeStopped, FString CheatName, int32 GameProcessId, int32 CheatProcessId, FString CheatParam1, FString CheatParam2);
}; // Size: 0x58

class USteamCoreWebAsyncActionReportPlayerCheating : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionReportPlayerCheating* ReportPlayerCheatingAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString SteamIdReporter, FString AppData, bool bHeuristic, bool bDetection, bool bPlayerReport, bool bNoReportID, int32 GameMode, int32 SuspicionStartTime, int32 Severity);
}; // Size: 0x58

class USteamCoreWebAsyncActionRequestNotifications : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRequestNotifications* RequestNotificationsAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionRequestPlayerGameBan : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRequestPlayerGameBan* RequestPlayerGameBanAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString ReportID, FString CheatDescription, int32 Duration, bool bDelayBan);
}; // Size: 0x58

class USteamCoreWebAsyncActionRequestVacStatusForUser : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionRequestVacStatusForUser* RequestVacStatusForUserAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString SessionID);
}; // Size: 0x58

class USteamCoreWebAsyncActionResetLeaderboard : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionResetLeaderboard* ResetLeaderboardAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 LeaderboardId);
}; // Size: 0x58

class USteamCoreWebAsyncActionResolveVanityURL : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionResolveVanityURL* ResolveVanityURLAsync(class UObject* WorldContextObject, FString Key, FString VanityURL, EVanityUrlType URLType);
}; // Size: 0x58

class USteamCoreWebAsyncActionResultSetSummary : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionResultSetSummary* ResultSetSummaryAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, int32 StartId, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
}; // Size: 0x58

class USteamCoreWebAsyncActionSetLeaderboardScore : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionSetLeaderboardScore* SetLeaderboardScoreAsync(class UObject* WorldContextObject, FString Key, int32 AppID, int32 LeaderboardId, FString SteamID, int32 Score, TArray<uint8> Details, FString ScoreMethod);
}; // Size: 0x58

class USteamCoreWebAsyncActionSetUGCUsedByGC : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionSetUGCUsedByGC* SetUGCUsedByGCAsync(class UObject* WorldContextObject, FString Key, FString SteamID, FString UGCID, int32 AppID, bool bUsed);
}; // Size: 0x58

class USteamCoreWebAsyncActionSetUserStatsForGame : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionSetUserStatsForGame* SetUserStatsForGameAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, TArray<FString> Names, TArray<int32> Values);
}; // Size: 0x58

class USteamCoreWebAsyncActionStartAssetTransaction : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionStartAssetTransaction* StartAssetTransactionAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamID, FString AssetId, int32 AssetQuantity, FString Currency, FString Language, FString Ipaddress, FString Referer, bool bClientAuth);
}; // Size: 0x58

class USteamCoreWebAsyncActionStartSecureMultiplayerSession : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionStartSecureMultiplayerSession* StartSecureMultiplayerSessionAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x58

class USteamCoreWebAsyncActionStartTrade : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionStartTrade* StartTradeAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString SteamId1, FString SteamId2);
}; // Size: 0x58

class USteamCoreWebAsyncActionSubscribePublishedFile : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionSubscribePublishedFile* SubscribePublishedFileAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionSupportGetAssetHistory : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionSupportGetAssetHistory* SupportGetAssetHistoryAsync(class UObject* WorldContextObject, FString Key, int32 AppID, FString AssetId, FString ContextID);
}; // Size: 0x58

class USteamCoreWebAsyncActionUnsubscribePublishedFile : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionUnsubscribePublishedFile* UnsubscribePublishedFileAsync(class UObject* WorldContextObject, FString Key, FString SteamID, int32 AppID, FString PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebAsyncActionUpdateSession : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionUpdateSession* UpdateSessionAsync(class UObject* WorldContextObject, FString Key, FString SessionID, int32 AppID, FString Title, FString Users, FString SteamID);
}; // Size: 0x58

class USteamCoreWebAsyncActionUserVoteSummary : public USteamCoreWebAsyncAction
{

    class USteamCoreWebAsyncActionUserVoteSummary* UserVoteSummaryAsync(class UObject* WorldContextObject, FString Key, FString SteamID, TArray<FString> PublishedFileIDs);
}; // Size: 0x58

class USteamCoreWebSettings : public UDeveloperSettings
{
    float AsyncTaskTimeout;                                                           // 0x0038 (size: 0x4)
    int32 DisabledSubsystems;                                                         // 0x003C (size: 0x4)
    bool bDebugging;                                                                  // 0x0040 (size: 0x1)
    bool bDevMode;                                                                    // 0x0041 (size: 0x1)
    FString Key;                                                                      // 0x0048 (size: 0x10)
    int32 AppID;                                                                      // 0x0058 (size: 0x4)
    FString DevSteamID;                                                               // 0x0060 (size: 0x10)
    bool bSandboxMode;                                                                // 0x0070 (size: 0x1)

}; // Size: 0x78

class USteamCoreWebSubsystem : public UGameInstanceSubsystem
{
}; // Size: 0x48

class USteamWebUtilities : public UBlueprintFunctionLibrary
{

    bool ParseJson(FString JsonString, TArray<FSteamCoreJson>& Data);
    FString GetProjectKey();
    int32 GetProjectAppID();
    FString GetDevSteamID();
    void FindJsonStrings(FString JsonString, FString Key, TArray<FString>& Values, ESteamJsonResult& Result);
    void FindJsonString(FString JsonString, FString Key, FString& Value, ESteamJsonResult& Result);
    void FindJsonNumbers(FString JsonString, FString Key, TArray<int32>& Values, ESteamJsonResult& Result);
    void FindJsonNumber(FString JsonString, FString Key, int32& Value, ESteamJsonResult& Result);
    void FindJsonBools(FString JsonString, FString Key, TArray<bool>& bValues, ESteamJsonResult& Result);
    void FindJsonBool(FString JsonString, FString Key, bool& bValue, ESteamJsonResult& Result);
}; // Size: 0x28

class UWebApps : public USteamCoreWebSubsystem
{

    void UpToDateCheck(const FUpToDateCheckCallback& Callback, int32 AppID, int32 Version);
    void SetAppBuildLive(const FSetAppBuildLiveCallback& Callback, FString Key, int32 AppID, int32 BuildID, FString BetaKey, FString Description);
    void GetServersAtAddress(const FGetServersAtAddressCallback& Callback, FString Addr);
    void GetServerList(const FGetServerListCallback& Callback, FString Key, FString Filter, int32 Limit);
    void GetPlayersBanned(const FGetPlayersBannedCallback& Callback, FString Key, int32 AppID);
    void GetCheatingReports(const FGetCheatingReportsCallback& Callback, FString Key, int32 AppID, int32 TimeBegin, int32 TimeEnd, bool bIncludeReports, bool bIncludeBans, int32 ReportidMin);
    void GetAppList(const FGetAppListCallback& Callback, FString Key);
    void GetAppDepotVersions(const FGetAppDepotVersionsCallback& Callback, FString Key, int32 AppID);
    void GetAppBuilds(const FGetAppBuildsCallback& Callback, FString Key, int32 AppID, int32 Count);
    void GetAppBetas(const FGetAppBetasCallback& Callback, FString Key, int32 AppID);
}; // Size: 0x48

class UWebBroadcastService : public USteamCoreWebSubsystem
{

    void PostGameDataFrame(const FPostGameDataFrameCallback& Callback, FString Key, int32 AppID, FString SteamID, FString BroadcastId, FString FrameData);
}; // Size: 0x48

class UWebCheatReporting : public USteamCoreWebSubsystem
{

    void StartSecureMultiplayerSession(const FStartSecureMultiplayerSessionCallback& Callback, FString Key, FString SteamID, int32 AppID);
    void RequestVacStatusForUser(const FRequestVacStatusForUserCallback& Callback, FString Key, FString SteamID, int32 AppID, FString SessionID);
    void RequestPlayerGameBan(const FRequestPlayerGameBanCallback& Callback, FString Key, FString SteamID, int32 AppID, FString ReportID, FString CheatDescription, int32 Duration, bool bDelayBan);
    void ReportPlayerCheating(const FReportPlayerCheatingCallback& Callback, FString Key, int32 AppID, FString SteamID, FString SteamIdReporter, FString AppData, bool bHeuristic, bool bDetection, bool bPlayerReport, bool bNoReportID, int32 GameMode, int32 SuspicionStartTime, int32 Severity);
    void ReportCheatData(const FReportCheatDataCallback& Callback, FString Key, FString SteamID, int32 AppID, FString PathAndFileName, FString WebCheatURL, FString TimeNow, FString TimeStarted, FString TimeStopped, FString CheatName, int32 GameProcessId, int32 CheatProcessId, FString CheatParam1, FString CheatParam2);
    void RemovePlayerGameBan(const FRemovePlayerGameBanCallback& Callback, FString Key, FString SteamID, int32 AppID);
    void GetCheatingReports(const FGetCheatingReportsCallback& Callback, FString Key, int32 AppID, int32 TimeEnd, int32 TimeBegin, FString ReportidMin, bool bIncludeReports, bool bIncludeBans, FString SteamID);
    void EndSecureMultiplayerSession(const FEndSecureMultiplayerSessionCallback& Callback, FString Key, FString SteamID, int32 AppID, FString SessionID);
}; // Size: 0x48

class UWebEconMarketService : public USteamCoreWebSubsystem
{

    void GetPopular(const FGetPopularCallback& Callback, FString Key, FString Language, int32 Rows, int32 Start, int32 FilterAppId, int32 ECurrency);
    void GetMarketEligibility(const FGetMarketEligibilityCallback& Callback, FString Key, FString SteamID);
    void GetAssetID(const FGetAssetIDCallback& Callback, FString Key, int32 AppID, FString ListingId);
    void CancelAppListingsForUser(const FCancelAppListingsForUserCallback& Callback, FString Key, int32 AppID, FString SteamID, bool bSynchronous);
}; // Size: 0x48

class UWebEconService : public USteamCoreWebSubsystem
{

    void GetTradeOffersSummary(const FGetTradeOffersSummaryCallback& Callback, FString Key, int32 TimeLastVisit);
    void GetTradeOffers(const FGetTradeOffersCallback& Callback, FString Key, bool bGetSentOffers, bool bGetReceivedOffers, bool bGetDescriptions, FString Language, bool bActiveOnly, bool bHistoricalOnly, int32 TimeHistoricalCutoff);
    void GetTradeOffer(const FGetTradeOfferCallback& Callback, FString Key, FString TradeOfferId, FString Language);
    void GetTradeHistory(const FGetTradeHistoryCallback& Callback, FString Key, int32 MaxTrades, int32 StartAfterTime, FString StartAfterTradeId, bool bNavigatingBack, bool bGetDescriptions, FString Language, bool bIncludeFailed, bool bIncludeTotal);
    void FlushInventoryCache(const FFlushInventoryCacheCallback& Callback, FString Key, FString SteamID, int32 AppID, FString ContextID);
    void FlushContextCache(const FFlushContextCacheCallback& Callback, FString Key, int32 AppID);
    void FlushAssetAppearanceCache(const FFlushAssetAppearanceCacheCallback& Callback, FString Key, int32 AppID);
    void DeclineTradeOffer(const FDeclineTradeOfferCallback& Callback, FString Key, FString TradeOfferId);
    void CancelTradeOffer(const FCancelTradeOfferCallback& Callback, FString Key, FString TradeOfferId);
}; // Size: 0x48

class UWebGameInventory : public USteamCoreWebSubsystem
{

    void UpdateItemDefs();
    void SupportGetAssetHistory(const FSupportGetAssetHistoryCallback& Callback, FString Key, int32 AppID, FString AssetId, FString ContextID);
    void HistoryExecuteCommands(const FHistoryExecuteCommandsCallback& Callback, FString Key, int32 AppID, FString SteamID, FString ContextID, int32 ActorId);
    void GetUserHistory(const FGetUserHistoryCallback& Callback, FString Key, int32 AppID, FString SteamID, FString ContextID, int32 StartTime, int32 EndTime);
    void GetHistoryCommandDetails(const FGetHistoryCommandDetailsCallback& Callback, FString Key, int32 AppID, FString SteamID, FString Command, FString ContextID, FString Arguments);
}; // Size: 0x48

class UWebGameNotificationsService : public USteamCoreWebSubsystem
{

    void UpdateSession(const FUpdateSessionCallback& Callback, FString Key, FString SessionID, int32 AppID, FString Title, FString Users, FString SteamID);
    void RequestNotifications(const FRequestNotificationsCallback& Callback, FString Key, FString SteamID, int32 AppID);
    void GetSessionDetailsForApp(const FGetSessionDetailsForAppCallback& Callback, FString Key, FString Sessions, int32 AppID, FString Language);
    void EnumerateSessionsForApp(const FEnumerateSessionsForAppCallback& Callback, FString Key, int32 AppID, FString SteamID, bool bIncludeAllUserMessages, bool bIncludeAuthUserMessage, FString Language);
    void DeleteSessionBatch(const FDeleteSessionBatchCallback& Callback, FString Key, FString SessionID, int32 AppID);
    void DeleteSession(const FDeleteSessionCallback& Callback, FString Key, FString SessionID, int32 AppID, FString SteamID);
    void CreateSession(const FCreateSessionCallback& Callback, FString Key, int32 AppID, FString Context, FString Title, FString Users, FString SteamID);
}; // Size: 0x48

class UWebGameServersService : public USteamCoreWebSubsystem
{

    void SetMemo(const FSetMemoCallback& Callback, FString Key, FString SteamID, FString Memo);
    void SetBanStatus(const FSetBanStatusCallback& Callback, FString Key, FString SteamID, bool bBanned, int32 BanSeconds);
    void ResetLoginToken(const FResetLoginTokenCallback& Callback, FString Key, FString SteamID);
    void QueryLoginToken(const FQueryLoginTokenCallback& Callback, FString Key, FString LoginToken);
    void GetServerSteamIdsByIP(const FGetServerSteamIdsByIPCallback& Callback, FString Key, FString ServerIP);
    void GetServerIPsBySteamId(const FGetServerIPsBySteamIdCallback& Callback, FString Key, FString ServerSteamId);
    void GetAccountPublicInfo(const FGetAccountPublicInfoCallback& Callback, FString Key, FString SteamID);
    void GetAccountList(const FGetAccountListCallback& Callback, FString Key);
    void DeleteAccount(const FDeleteAccountCallback& Callback, FString Key, FString SteamID);
    void CreateAccount(const FCreateAccountCallback& Callback, FString Key, int32 AppID, FString Memo);
}; // Size: 0x48

class UWebInventoryService : public USteamCoreWebSubsystem
{

    void ModifyItems(const FModifyItemsCallback& Callback, FString Key, int32 AppID, FString SteamID, FModifyItemsItem Updates);
    void GetQuantity(const FGetQuantityCallback& Callback, FString Key, int32 AppID, FString SteamID, TArray<int32> ItemdefIDs, bool bForce);
    void GetPriceSheet(const FGetPriceSheetCallback& Callback, FString Key, int32 Currency);
    void GetItemDefs(const FGetItemDefsCallback& Callback, FString Key, int32 AppID, FString ModifiedSince, TArray<int32> ItemdefIDs, TArray<int32> WorkshopIDs, int32 CacheMaxAgeSeconds);
    void GetInventory(const FGetInventoryCallback& Callback, FString Key, int32 AppID, FString SteamID);
    void ExchangeItem(const FExchangeItemCallback& Callback, FString Key, int32 AppID, FString SteamID, TArray<int64> MaterialsItemID, TArray<int32> MaterialsQuantity, int64 OutputItemdefId);
    void ConsumeItem(const FConsumeItemCallback& Callback, FString Key, int32 AppID, FString ItemId, FString Quantity, FString SteamID, FString RequestID);
    void Consolidate(const FConsolidateCallback& Callback, FString Key, int32 AppID, FString SteamID, TArray<int32> ItemdefIDs, bool bForce);
    void AddPromoItem(const FAddPromoItemCallback& Callback, FString Key, int32 AppID, int32 ItemdefId, FString ItemPropsJson, FString SteamID, bool bNotify, FString RequestID);
    void AddItem(const FAddItemCallback& Callback, FString Key, int32 AppID, TArray<int32> ItemdefId, FString ItemPropsJson, TArray<int32> ItemQuantity, FString SteamID, bool bNotify, FString RequestID, bool bTradeRestriction);
}; // Size: 0x48

class UWebLeaderboards : public USteamCoreWebSubsystem
{

    void SetLeaderboardScore(const FSetLeaderboardScoreCallback& Callback, FString Key, int32 AppID, int32 LeaderbordId, FString SteamID, int32 Score, TArray<uint8> Details, FString ScoreMethod);
    void ResetLeaderboard(const FResetLeaderboardCallback& Callback, FString Key, int32 AppID, int32 LeaderboardId);
    void GetLeaderboardsForGame(const FGetLeaderboardsForGameCallback& Callback, FString Key, int32 AppID);
    void GetLeaderboardEntries(const FGetLeaderboardEntriesCallback& Callback, FString Key, int32 AppID, int32 RangeStart, int32 RangeEnd, int32 LeaderboardId, int32 DataRequest, FString SteamID);
    void FindOrCreateLeaderboard(const FFindOrCreateLeaderboardCallback& Callback, FString Key, int32 AppID, FString Name, FString SortMethod, FString DisplayType, bool bCreateIfNotFound, bool bOnlyTrustedWrites, bool bOnlyFriendsReads);
    void DeleteLeaderboard(const FDeleteLeaderboardCallback& Callback, FString Key, int32 AppID, FString Name);
}; // Size: 0x48

class UWebLobbyMatchmakingService : public USteamCoreWebSubsystem
{

    void RemoveUserFromLobby(const FRemoveUserFromLobbyCallback& Callback, FString Key, int32 AppID, FString SteamIdToRemove, FString SteamIDLobby, FString InputJson);
    void CreateLobby(const FCreateLobbyCallback& Callback, FString Key, int32 AppID, int32 MaxMembers, ESteamCoreWebLobbyType LobbyType, FString LobbyName, FString InputJson, TArray<FString> SteamIdInvitedMembers, FString LobbyMetaData);
}; // Size: 0x48

class UWebMicroTxn : public USteamCoreWebSubsystem
{

    void RefundTxn(const FRefundTxnCallback& Callback, FString Key, FString OrderID, int32 AppID);
    void QueryTxn(const FQueryTxnCallback& Callback, FString Key, int32 AppID, FString OrderID, FString TransId);
    void ProcessAgreement(const FProcessAgreementCallback& Callback, FString Key, FString OrderID, FString SteamID, FString AgreementId, int32 AppID, int32 Amount, FString Currency);
    FString MakeTransactionID();
    void InitTxn(const FInitTxnCallback& Callback, FString Key, FString OrderID, FString SteamID, int32 AppID, FString Language, FString Currency, FString UserSession, FString Ipaddress, TArray<int32> ItemId, TArray<int32> Quantity, TArray<FString> Amount, TArray<FString> Description, TArray<FString> Category, TArray<int32> AssociatedBundle, TArray<FString> BillingType, TArray<FString> StartDate, TArray<FString> EndDate, TArray<FString> Period, TArray<int32> Frequency, TArray<FString> RecurringAmt, TArray<int32> BundleCount, TArray<int32> BundleId, TArray<int32> BundleQty, TArray<FString> BundleDesc, TArray<FString> BundleCategory);
    void GetUserInfo(const FGetUserInfoCallback& Callback, FString Key, FString SteamID, FString Ipaddress);
    void GetUserAgreementInfo(const FGetUserAgreementInfoCallback& Callback, FString Key, FString SteamID, int32 AppID);
    void GetReport(const FGetReportCallback& Callback, FString Key, int32 AppID, FString Time, FString Type, int32 MaxResults);
    void FinalizeTxn(const FFinalizeTxnCallback& Callback, FString Key, FString OrderID, int32 AppID);
    void CancelAgreement(const FCancelAgreementCallback& Callback, FString Key, FString SteamID, FString AgreementId, int32 AppID);
    void AdjustAgreement(const FAdjustAgreementCallback& Callback, FString Key, FString SteamID, FString AgreementId, int32 AppID, FString NextProcessDate);
}; // Size: 0x48

class UWebPlayerService : public USteamCoreWebSubsystem
{

    void IsPlayingSharedGame(const FIsPlayingSharedGameCallback& Callback, FString Key, FString SteamID, int32 AppIdPlaying);
    void GetSteamLevel(const FGetSteamLevelCallback& Callback, FString Key, FString SteamID);
    void GetRecentlyPlayedGames(const FGetRecentlyPlayedGamesCallback& Callback, FString Key, FString SteamID, int32 Count);
    void GetOwnedGames(const FGetOwnedGamesCallback& Callback, FString Key, FString SteamID, bool bIncludeAppInfo, bool bIncludePlayedFreeGames, TArray<int32> Filter);
    void GetCommunityBadgeProgress(const FGetCommunityBadgeProgressCallback& Callback, FString Key, FString SteamID, int32 BadgeId);
    void GetBadges(const FGetBadgesCallback& Callback, FString Key, FString SteamID);
}; // Size: 0x48

class UWebPublishedFileService : public USteamCoreWebSubsystem
{

    void UpdateTags(const FUpdateTagsCallback& Callback, FString Key, FString PublishedFileID, int32 AppID, FString AddTags, FString RemoveTags);
    void UpdateIncompatibleStatus(const FUpdateIncompatibleStatusCallback& Callback, FString Key, FString PublishedFileID, int32 AppID, bool bIncompatible);
    void UpdateBanStatus(const FUpdateBanStatusCallback& Callback, FString Key, FString PublishedFileID, int32 AppID, bool bBanned, FString Reason);
    void SetDeveloperMetadata(const FSetDeveloperMetadataCallback& Callback, FString Key, FString PublishedFileID, int32 AppID, FString MetaData);
    void QueryFiles(const FQueryFilesCallback& Callback, FString Key, int32 QueryType, int32 Page, FString Cursor, int32 NumPerPage, int32 CreatorAppID, int32 AppID, FString RequiredTags, FString ExcludedTags, bool bMatchAllTags, FString RequiredFlags, FString OmittedFlags, FString SearchText, int32 FileType, FString ChildPublishedFileId, int32 Days, bool bIncludeRecentVotesOnly, int32 CacheMaxAgeSeconds, int32 Language, FString RequiredKvTags, bool bTotalOnly, bool bIDsOnly, bool bReturnVoteData, bool bReturnTags, bool bReturnKvTags, bool bReturnPreviews, bool bReturnChildren, bool bReturnShortDescription, bool bReturnForSaleData, bool bReturnMetadata, int32 ReturnPlaytimeStats);
}; // Size: 0x48

class UWebPublishedItemVoting : public USteamCoreWebSubsystem
{

    void UserVoteSummary(const FUserVoteSummaryCallback& Callback, FString Key, FString SteamID, TArray<FString> PublishedFileIDs);
    void ItemVoteSummary(const FItemVoteSummaryCallback& Callback, FString Key, FString SteamID, int32 AppID, TArray<FString> PublishedFileIDs);
}; // Size: 0x48

class UWebRemoteStorage : public USteamCoreWebSubsystem
{

    void UnsubscribePublishedFile(const FUnsubscribePublishedFileCallback& Callback, FString Key, FString SteamID, int32 AppID, FString PublishedFileIDs);
    void SubscribePublishedFile(const FSubscribePublishedFileCallback& Callback, FString Key, FString SteamID, int32 AppID, FString PublishedFileIDs);
    void SetUGCUsedByGC(const FSetUGCUsedByGCCallback& Callback, FString Key, FString SteamID, FString UGCID, int32 AppID, bool bUsed);
    void GetUGCFileDetails(const FGetUGCFileDetailsCallback& Callback, FString Key, FString SteamID, FString UGCID, int32 AppID);
    void GetPublishedFileDetails(const FGetPublishedFileDetailsCallback& Callback, FString PublishedFileIDs);
    void GetCollectionDetails(const FGetCollectionDetailsCallback& Callback, TArray<FString> PublishedFileIDs);
    void EnumerateUserSubscribedFiles(const FEnumerateUserSubscribedFilesCallback& Callback, FString Key, FString SteamID, int32 AppID, int32 ListType);
    void EnumerateUserPublishedFiles(const FEnumerateUserPublishedFilesCallback& Callback, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x48

class UWebSteamCommunity : public USteamCoreWebSubsystem
{

    void ReportAbuse(const FReportAbuseCallback& Callback, FString Key, FString SteamIdActor, FString SteamIdTarget, int32 AppID, int32 AbuseType, int32 ContentType, FString Description, FString Gid);
}; // Size: 0x48

class UWebSteamEconomy : public USteamCoreWebSubsystem
{

    void StartTrade(const FStartTradeCallback& Callback, FString Key, int32 AppID, FString SteamIDd, FString SteamId2);
    void StartAssetTransaction(const FStartAssetTransactionCallback& Callback, FString Key, int32 AppID, FString SteamID, FString AssetId, int32 AssetQuantity, FString Currency, FString Language, FString Ipaddress, FString Referer, bool bClientAuth);
    void GetMarketPrices(const FGetMarketPricesCallback& Callback, FString Key, int32 AppID);
    void GetExportedAssetsForUser(const FGetExportedAssetsForUserCallback& Callback, FString Key, FString SteamID, int32 AppID, FString ContextID);
    void GetAssetPrices(const FGetAssetPricesCallback& Callback, FString Key, int32 AppID, FString Currency, FString Language);
    void GetAssetClassInfo(const FGetAssetClassInfoCallback& Callback, FString Key, int32 AppID, FString Language, int32 ClassCount, FString ClassID, FString InstanceId);
    void FinalizeAssetTransaction(const FFinalizeAssetTransactionCallback& Callback, FString Key, int32 AppID, FString SteamID, FString TxnId, FString Language);
    void CanTrade(const FCanTradeCallback& Callback, FString Key, int32 AppID, FString SteamID, FString TargetId);
}; // Size: 0x48

class UWebSteamGameServerStats : public USteamCoreWebSubsystem
{

    void GetGameServerPlayerStatsForGame(const FGetGameServerPlayerStatsForGameCallback& Callback, FString Key, FString GameID, int32 AppID, FString RangeStart, FString RangeEnd, int32 MaxResults);
}; // Size: 0x48

class UWebSteamNews : public USteamCoreWebSubsystem
{

    void GetNewsForAppAuthed(const FGetNewsForAppAuthedCallback& Callback, FString Key, int32 AppID, int32 MaxLength, TArray<FString> Feeds, int32 EndDate, int32 Count);
    void GetNewsForApp(const FGetNewsForAppCallback& Callback, int32 AppID, int32 MaxLength, TArray<FString> Feeds, int32 EndDate, int32 Count);
}; // Size: 0x48

class UWebSteamPublishedItemSearch : public USteamCoreWebSubsystem
{

    void ResultSetSummary(const FResultSetSummaryCallback& Callback, FString Key, FString SteamID, int32 AppID, int32 StartId, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
    void RankedByVote(const FRankedByVoteCallback& Callback, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
    void RankedByTrend(const FRankedByTrendCallback& Callback, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, int32 Days, TArray<FString> Tag, TArray<FString> UserTag);
    void RankedByPublicationOrder(const FRankedByPublicationOrderCallback& Callback, FString Key, FString SteamID, int32 AppID, int32 StartId, int32 Count, bool bHasAppAdminAccess, int32 FileType, TArray<FString> Tag, TArray<FString> UserTag);
}; // Size: 0x48

class UWebSteamUser : public USteamCoreWebSubsystem
{

    void ResolveVanityURL(const FResolveVanityURLCallback& Callback, FString Key, FString VanityURL, EVanityUrlType URLType);
    void GrantPackage(const FGrantPackageCallback& Callback, FString Key, FString SteamID, int32 PackageId, FString Ipaddress, FString ThirdPartyKey, int32 ThirdPartyAppId);
    void GetUserGroupList(const FGetUserGroupListCallback& Callback, FString Key, FString SteamID);
    void GetPublisherAppOwnershipChanges(const FGetPublisherAppOwnershipChangesCallback& Callback, FString Key, FString PackageRowVersion, FString CDKeyRowVersion);
    void GetPublisherAppOwnership(const FGetPublisherAppOwnershipCallback& Callback, FString Key, FString SteamID);
    void GetPlayerSummaries(const FGetPlayerSummariesCallback& Callback, FString Key, TArray<FString> SteamIDs);
    void GetPlayerBans(const FGetPlayerBansCallback& Callback, FString Key, TArray<FString> SteamIDs);
    void GetFriendList(const FGetFriendListCallback& Callback, FString Key, FString SteamID, FString Relationship);
    void GetAppPriceInfo(const FGetAppPriceInfoCallback& Callback, FString Key, FString SteamID, TArray<int32> AppIDs);
    void CheckAppOwnership(const FCheckAppOwnershipCallback& Callback, FString Key, FString SteamID, int32 AppID);
}; // Size: 0x48

class UWebUserAuth : public USteamCoreWebSubsystem
{

    void AuthenticateUserTicket(const FAuthenticateUserTicketCallback& Callback, FString Key, int32 AppID, FString Ticket);
    void AuthenticateUser(const FAuthenticateUserCallback& Callback, FString SteamID, TArray<uint8> SessionKey, TArray<uint8> EncryptedLoginKey);
}; // Size: 0x48

class UWebUserStats : public USteamCoreWebSubsystem
{

    void SetUserStatsForGame(const FSetUserStatsForGameCallback& Callback, FString Key, FString SteamID, int32 AppID, TArray<FString> Names, TArray<int32> Values);
    void GetUserStatsForGame(const FGetUserStatsForGameCallback& Callback, FString Key, FString SteamID, int32 AppID);
    void GetSchemaForGame(const FGetSchemaForGameCallback& Callback, FString Key, FString SteamID, int32 AppID, FString Language);
    void GetPlayerAchievements(const FGetPlayerAchievementsCallback& Callback, FString Key, FString SteamID, int32 AppID, FString Language);
    void GetNumberOfCurrentPlayers(const FGetNumberOfCurrentPlayersCallback& Callback, int32 AppID);
    void GetGlobalStatsForGame(const FGetGlobalStatsForGameCallback& Callback, int32 AppID, TArray<FString> Names, int32 StartDate, int32 EndDate);
    void GetGlobalAchievementPercentagesForApp(const FGetGlobalAchievementPercentagesForAppCallback& Callback, FString GameID);
}; // Size: 0x48

#endif
