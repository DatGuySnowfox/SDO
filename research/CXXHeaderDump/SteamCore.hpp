#ifndef UE4SS_SDK_SteamCore_HPP
#define UE4SS_SDK_SteamCore_HPP

#include "SteamCore_enums.hpp"

struct FAddAppDependencyResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    int32 AppID;                                                                      // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FAddUGCDependencyResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    FPublishedFileID ChildPublishedFileId;                                            // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FAssociateWithClanResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FAttachLeaderboardUGCData
{
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FAvatarImageLoaded
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    class UTexture2D* Image;                                                          // 0x0008 (size: 0x8)

}; // Size: 0x20

struct FChangeNumOpenSlotsData
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FCheckFileSignature
{
    ESteamCheckFileSignature CheckFileSignature;                                      // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FClanOfficerListResponse
{
    FSteamID SteamIDClan;                                                             // 0x0000 (size: 0x8)
    int32 Officers;                                                                   // 0x0008 (size: 0x4)
    bool bSuccess;                                                                    // 0x000C (size: 0x1)

}; // Size: 0x10

struct FClientGameServerDeny
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)
    FString GameServerIP;                                                             // 0x0008 (size: 0x10)
    int32 GameServerPort;                                                             // 0x0018 (size: 0x4)
    bool BSecure;                                                                     // 0x001C (size: 0x1)
    ESteamDenyReason Reason;                                                          // 0x001D (size: 0x1)

}; // Size: 0x20

struct FComputeNewPlayerCompatibilityResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    int32 PlayersThatDontLikeCandidate;                                               // 0x0004 (size: 0x4)
    int32 PlayersThatCandidateDoesntLike;                                             // 0x0008 (size: 0x4)
    int32 ClanPlayersThatDontLikeCandidate;                                           // 0x000C (size: 0x4)
    FSteamID SteamIDCandidate;                                                        // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FCreateBeaconData
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPartyBeaconID BeaconID;                                                          // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FCreateItemResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    bool bUserNeedsToAcceptWorkshopLegalAgreement;                                    // 0x0010 (size: 0x1)

}; // Size: 0x18

struct FCreateLobbyData
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamIDLobby;                                                            // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FDLCInstalled
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FDownloadClanActivityCountsResult
{
    bool bSuccess;                                                                    // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FDownloadItemResult
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    ESteamResult Result;                                                              // 0x0010 (size: 0x1)

}; // Size: 0x18

struct FEncryptedAppTicketResponse
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FEndGameResultCallback
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString UniqueGameID;                                                             // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FFavoritesListAccountsUpdated
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FFavoritesListChanged
{
    FString IP;                                                                       // 0x0000 (size: 0x10)
    int32 QueryPort;                                                                  // 0x0010 (size: 0x4)
    int32 ConnectionPort;                                                             // 0x0014 (size: 0x4)
    int32 AppID;                                                                      // 0x0018 (size: 0x4)
    TArray<ESteamFavoriteFlags> Flags;                                                // 0x0020 (size: 0x10)
    bool bAdd;                                                                        // 0x0030 (size: 0x1)
    FSteamID SteamID;                                                                 // 0x0038 (size: 0x8)

}; // Size: 0x40

struct FFileDetailsResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    int32 FileSize;                                                                   // 0x0004 (size: 0x4)
    FString SHA;                                                                      // 0x0008 (size: 0x10)
    TArray<int32> Flags;                                                              // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FFindOrCreateLeaderboardData
{
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0000 (size: 0x8)
    bool bLeaderboardFound;                                                           // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FFriendRichPresenceUpdate
{
    FSteamID SteamIDFriend;                                                           // 0x0000 (size: 0x8)
    int32 AppID;                                                                      // 0x0008 (size: 0x4)

}; // Size: 0x10

struct FFriendsEnumerateFollowingList
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    TArray<FSteamID> SteamIDs;                                                        // 0x0008 (size: 0x10)
    int32 Results;                                                                    // 0x0018 (size: 0x4)
    int32 TotalResult;                                                                // 0x001C (size: 0x4)

}; // Size: 0x20

struct FFriendsGetFollowerCount
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamID;                                                                 // 0x0008 (size: 0x8)
    int32 Count;                                                                      // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FFriendsIsFollowing
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamID;                                                                 // 0x0008 (size: 0x8)
    bool bIsFollowing;                                                                // 0x0010 (size: 0x1)

}; // Size: 0x18

struct FGSClientApprove
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    FSteamID OwnerSteamID;                                                            // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGSClientDeny
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    ESteamDenyReason DenyReason;                                                      // 0x0008 (size: 0x1)
    FString OptionalText;                                                             // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FGSClientGroupStatus
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)
    FSteamID SteamIDGroup;                                                            // 0x0008 (size: 0x8)
    bool bMember;                                                                     // 0x0010 (size: 0x1)
    bool bOfficer;                                                                    // 0x0011 (size: 0x1)

}; // Size: 0x18

struct FGSPolicyResponse
{
    bool BSecure;                                                                     // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FGSStatsReceived
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamIDUser;                                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGSStatsStored
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamIDUser;                                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGSStatsUnloaded
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FGameConnectedChatJoin
{
    FSteamID SteamIDClanChat;                                                         // 0x0000 (size: 0x8)
    FSteamID SteamIDUser;                                                             // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGameConnectedChatLeave
{
    FSteamID SteamIDClanChat;                                                         // 0x0000 (size: 0x8)
    FSteamID SteamIDUser;                                                             // 0x0008 (size: 0x8)
    bool bKicked;                                                                     // 0x0010 (size: 0x1)
    bool bDropped;                                                                    // 0x0011 (size: 0x1)

}; // Size: 0x18

struct FGameConnectedClanChatMsg
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)
    FSteamID SteamIDClanChat;                                                         // 0x0008 (size: 0x8)
    int32 MessageID;                                                                  // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FGameConnectedFriendChatMsg
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)
    int32 MessageID;                                                                  // 0x0008 (size: 0x4)

}; // Size: 0x10

struct FGameLobbyJoinRequested
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDFriend;                                                           // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FGameOverlayActivated
{
    bool bActive;                                                                     // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FGameRichPresenceJoinRequested
{
    FSteamID SteamIDFriend;                                                           // 0x0000 (size: 0x8)
    FString Connect;                                                                  // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FGameServerChangeRequested
{
    FString Server;                                                                   // 0x0000 (size: 0x10)
    FString Password;                                                                 // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FGameServerItem
{
    FString ServerName;                                                               // 0x0000 (size: 0x10)
    FString MapName;                                                                  // 0x0010 (size: 0x10)
    FString GameDescription;                                                          // 0x0020 (size: 0x10)
    FString GameTags;                                                                 // 0x0030 (size: 0x10)
    FSteamServerAddr SteamServerAddr;                                                 // 0x0040 (size: 0x38)
    int32 Ping;                                                                       // 0x0078 (size: 0x4)
    int32 Players;                                                                    // 0x007C (size: 0x4)
    int32 MaxPlayers;                                                                 // 0x0080 (size: 0x4)
    int32 BotPlayers;                                                                 // 0x0084 (size: 0x4)
    int32 ServerVersion;                                                              // 0x0088 (size: 0x4)
    bool bPassword;                                                                   // 0x008C (size: 0x1)
    bool BSecure;                                                                     // 0x008D (size: 0x1)

}; // Size: 0xB0

struct FGameServerRule
{
    FString Name;                                                                     // 0x0000 (size: 0x10)
    FString Value;                                                                    // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FGameWebCallback
{
    FString URL;                                                                      // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FGamepadTextInputDismissed
{
    bool bSubmitted;                                                                  // 0x0000 (size: 0x1)
    int32 SubmittedText;                                                              // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FGetAppDependenciesResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    TArray<int32> AppIDs;                                                             // 0x0010 (size: 0x10)
    int32 NumAppDependencies;                                                         // 0x0020 (size: 0x4)
    int32 TotalNumAppDependencies;                                                    // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FGetAuthSessionTicketResponse
{
    FSteamTicketHandle AuthTicket;                                                    // 0x0000 (size: 0x4)
    ESteamResult Result;                                                              // 0x0004 (size: 0x1)

}; // Size: 0x8

struct FGetOPFSettingsResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    int32 AppID;                                                                      // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FGetTicketForWebApiResponse
{
    FSteamTicketHandle AuthTicket;                                                    // 0x0000 (size: 0x4)
    ESteamResult Result;                                                              // 0x0004 (size: 0x1)
    TArray<uint8> Ticket;                                                             // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FGetUserItemVoteResult
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    bool bVotedUp;                                                                    // 0x0009 (size: 0x1)
    bool bVotedDown;                                                                  // 0x000A (size: 0x1)
    bool bVoteSkipped;                                                                // 0x000B (size: 0x1)

}; // Size: 0x10

struct FGetVideoURLResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    int32 AppID;                                                                      // 0x0004 (size: 0x4)
    FString URL;                                                                      // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FGlobalAchievementPercentagesReady
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FGlobalStatsReceived
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FHostPingData
{
    FString HostString;                                                               // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FIPCFailure
{
    ESteamFailureType FailureType;                                                    // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FIPCountry
{
}; // Size: 0x1

struct FInputActionSetHandle
{
}; // Size: 0x8

struct FInputAnalogActionData
{
    ESteamCoreInputSourceMode Mode;                                                   // 0x0000 (size: 0x1)
    float X;                                                                          // 0x0004 (size: 0x4)
    float Y;                                                                          // 0x0008 (size: 0x4)
    bool bActive;                                                                     // 0x000C (size: 0x1)

}; // Size: 0x10

struct FInputAnalogActionHandle
{
}; // Size: 0x8

struct FInputDigitalActionData
{
    bool bState;                                                                      // 0x0000 (size: 0x1)
    bool bActive;                                                                     // 0x0001 (size: 0x1)

}; // Size: 0x2

struct FInputDigitalActionHandle
{
}; // Size: 0x8

struct FInputHandle
{
}; // Size: 0x8

struct FInputMotionData
{
    float RotQuatX;                                                                   // 0x0000 (size: 0x4)
    float RotQuatY;                                                                   // 0x0004 (size: 0x4)
    float RotQuatZ;                                                                   // 0x0008 (size: 0x4)
    float RotQuatW;                                                                   // 0x000C (size: 0x4)
    float PosAccelX;                                                                  // 0x0010 (size: 0x4)
    float PosAccelY;                                                                  // 0x0014 (size: 0x4)
    float PosAccelZ;                                                                  // 0x0018 (size: 0x4)
    float RotVelX;                                                                    // 0x001C (size: 0x4)
    float RotVelY;                                                                    // 0x0020 (size: 0x4)
    float RotVelZ;                                                                    // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FItemInstalled
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FJoinClanChatRoomCompletionResult
{
    FSteamID SteamIDClanChat;                                                         // 0x0000 (size: 0x8)
    ESteamChatRoomEnterResponse ChatRoomEnterResponse;                                // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FJoinLobbyData
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    bool bLocked;                                                                     // 0x0008 (size: 0x1)
    ESteamChatRoomEnterResponse ChatRoomEnterResponse;                                // 0x0009 (size: 0x1)

}; // Size: 0x10

struct FJoinPartyData
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPartyBeaconID BeaconID;                                                          // 0x0008 (size: 0x8)
    FSteamID SteamIDBeaconOwner;                                                      // 0x0010 (size: 0x8)
    FString ConnectString;                                                            // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FLeaderboardFindResult
{
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0000 (size: 0x8)
    bool bLeaderboardFound;                                                           // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FLeaderboardScoreUploaded
{
    bool bSuccess;                                                                    // 0x0000 (size: 0x1)
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0008 (size: 0x8)
    int32 Score;                                                                      // 0x0010 (size: 0x4)
    bool bScoreChanged;                                                               // 0x0014 (size: 0x1)
    int32 GlobalRankNew;                                                              // 0x0018 (size: 0x4)
    int32 GlobalRankPrevious;                                                         // 0x001C (size: 0x4)

}; // Size: 0x20

struct FLeaderboardScoresDownloaded
{
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0000 (size: 0x8)
    FSteamLeaderboardEntries SteamLeaderboardEntries;                                 // 0x0008 (size: 0x8)
    int32 EntryCount;                                                                 // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FLeaderboardScoresDownloadedForUsers
{
    FSteamLeaderboard SteamLeaderboard;                                               // 0x0000 (size: 0x8)
    FSteamLeaderboardEntries SteamLeaderboardEntries;                                 // 0x0008 (size: 0x8)
    int32 EntryCount;                                                                 // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FLicensesUpdated
{
}; // Size: 0x1

struct FLobbyChatMsg
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDUser;                                                             // 0x0008 (size: 0x8)
    ESteamChatEntryType ChatEntryType;                                                // 0x0010 (size: 0x1)
    int32 ChatID;                                                                     // 0x0014 (size: 0x4)

}; // Size: 0x18

struct FLobbyChatUpdate
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDUserChanged;                                                      // 0x0008 (size: 0x8)
    FSteamID SteamIDMakingChange;                                                     // 0x0010 (size: 0x8)
    TArray<ESteamChatMemberStateChange> ChatMemberStateChange;                        // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FLobbyDataUpdate
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDMember;                                                           // 0x0008 (size: 0x8)
    bool bSuccess;                                                                    // 0x0010 (size: 0x1)

}; // Size: 0x18

struct FLobbyEnterData
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    bool bLocked;                                                                     // 0x0008 (size: 0x1)
    ESteamChatRoomEnterResponse ChatRoomEnterResponse;                                // 0x0009 (size: 0x1)

}; // Size: 0x10

struct FLobbyGameCreated
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDGameServer;                                                       // 0x0008 (size: 0x8)
    FString IP;                                                                       // 0x0010 (size: 0x10)
    int32 Port;                                                                       // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FLobbyInviteData
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)
    FSteamID SteamIDLobby;                                                            // 0x0008 (size: 0x8)
    FSteamGameID GameID;                                                              // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FLobbyKickedData
{
    FSteamID SteamIDLobby;                                                            // 0x0000 (size: 0x8)
    FSteamID SteamIDAdmin;                                                            // 0x0008 (size: 0x8)
    bool bKickedDueToDisconnect;                                                      // 0x0010 (size: 0x1)

}; // Size: 0x18

struct FLobbyMatchList
{
    int32 LobbiesMatching;                                                            // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FLowBatteryPower
{
    int32 MinutesBatteryLeft;                                                         // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FMicroTxnAuthorizationResponse
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)
    FString OrderID;                                                                  // 0x0008 (size: 0x10)
    bool bAuthorized;                                                                 // 0x0018 (size: 0x1)

}; // Size: 0x20

struct FNumberOfCurrentPlayers
{
    bool bSuccess;                                                                    // 0x0000 (size: 0x1)
    int32 Players;                                                                    // 0x0004 (size: 0x4)

}; // Size: 0x8

struct FP2PSessionConnectFail
{
    FSteamID SteamIDRemote;                                                           // 0x0000 (size: 0x8)
    ESteamP2PSessionError P2PSessionError;                                            // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FP2PSessionRequest
{
    FSteamID SteamIDRemote;                                                           // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FPartyBeaconID
{
}; // Size: 0x8

struct FPersonaStateChange
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    TArray<ESteamPersonaChange> Flags;                                                // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FPlaybackStatusHasChanged
{
}; // Size: 0x1

struct FPublishedFileID
{
}; // Size: 0x8

struct FRemoteStorageDownloadUGCResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamUGCHandle FileHandle;                                                       // 0x0008 (size: 0x8)
    int32 AppID;                                                                      // 0x0010 (size: 0x4)
    int32 SizeInBytes;                                                                // 0x0014 (size: 0x4)
    FString Filename;                                                                 // 0x0018 (size: 0x10)
    FSteamID SteamIDOwner;                                                            // 0x0028 (size: 0x8)

}; // Size: 0x30

struct FRemoteStorageFileReadAsyncComplete
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    int32 Offset;                                                                     // 0x0004 (size: 0x4)
    int32 Read;                                                                       // 0x0008 (size: 0x4)

}; // Size: 0x18

struct FRemoteStorageFileShareResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamUGCHandle File;                                                             // 0x0008 (size: 0x8)
    FString Filename;                                                                 // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FRemoteStorageFileWriteAsyncComplete
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FRemoteStoragePublishedFileSubscribed
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    int32 AppID;                                                                      // 0x0008 (size: 0x4)

}; // Size: 0x10

struct FRemoteStoragePublishedFileUnsubscribed
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    int32 AppID;                                                                      // 0x0008 (size: 0x4)

}; // Size: 0x10

struct FRemoteStorageSubscribePublishedFileResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FRemoteStorageUnsubscribePublishedFileResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FRemoveAppDependencyResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    int32 AppID;                                                                      // 0x0010 (size: 0x4)

}; // Size: 0x18

struct FRemoveUGCDependencyResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)
    FPublishedFileID ChildPublishedFileId;                                            // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FRequestPlayersForGameFinalResultCallback
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString SearchID;                                                                 // 0x0008 (size: 0x10)
    FString UniqueGameID;                                                             // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FRequestPlayersForGameProgressCallback
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString SearchID;                                                                 // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FRequestPlayersForGameResultCallback
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString SearchID;                                                                 // 0x0008 (size: 0x10)
    FSteamID SteamIDPlayerFound;                                                      // 0x0018 (size: 0x8)
    FSteamID SteamIDLobby;                                                            // 0x0020 (size: 0x8)
    ESteamPlayerAcceptState PlayerAcceptState;                                        // 0x0028 (size: 0x1)
    int32 PlayerIndex;                                                                // 0x002C (size: 0x4)
    int32 TotalPlayersFound;                                                          // 0x0030 (size: 0x4)
    int32 TotalPlayersAcceptedGame;                                                   // 0x0034 (size: 0x4)
    int32 SuggestedTeamIndex;                                                         // 0x0038 (size: 0x4)
    FString UniqueGameID;                                                             // 0x0040 (size: 0x10)

}; // Size: 0x50

struct FRequestUserStatsData
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    FSteamID SteamIDUser;                                                             // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FReservationNotificationData
{
    FPartyBeaconID BeaconID;                                                          // 0x0000 (size: 0x8)
    FSteamID SteamIDJoiner;                                                           // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FScreenshotHandle
{
}; // Size: 0x4

struct FScreenshotReady
{
    FScreenshotHandle Handle;                                                         // 0x0000 (size: 0x4)
    ESteamResult Result;                                                              // 0x0004 (size: 0x1)

}; // Size: 0x8

struct FScreenshotRequested
{
}; // Size: 0x1

struct FSearchForGameProgressCallback
{
    FString SearchID;                                                                 // 0x0000 (size: 0x10)
    ESteamResult Result;                                                              // 0x0010 (size: 0x1)
    FSteamID LobbyID;                                                                 // 0x0018 (size: 0x8)
    FSteamID SteamIDEndedSearch;                                                      // 0x0020 (size: 0x8)
    int32 SecondsRemainingEstimate;                                                   // 0x0028 (size: 0x4)
    int32 PlayersSearching;                                                           // 0x002C (size: 0x4)

}; // Size: 0x30

struct FSearchForGameResultCallback
{
    FString SearchID;                                                                 // 0x0000 (size: 0x10)
    ESteamResult Result;                                                              // 0x0010 (size: 0x1)
    int32 CountPlayersInGame;                                                         // 0x0014 (size: 0x4)
    int32 CountAcceptedGame;                                                          // 0x0018 (size: 0x4)
    FSteamID SteamIDHost;                                                             // 0x0020 (size: 0x8)
    bool bFinalCallback;                                                              // 0x0028 (size: 0x1)

}; // Size: 0x30

struct FSetPersonaNameResponse
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    bool bSuccess;                                                                    // 0x0001 (size: 0x1)
    bool bLocalSuccess;                                                               // 0x0002 (size: 0x1)

}; // Size: 0x3

struct FSetUserItemVoteResult
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    bool bVoteUp;                                                                     // 0x0009 (size: 0x1)

}; // Size: 0x10

struct FStartPlaytimeTrackingResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FSteamAppInstalled
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamAppUninstalled
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamFriendsGroupID
{
}; // Size: 0x2

struct FSteamGameID
{
}; // Size: 0x8

struct FSteamID
{
}; // Size: 0x8

struct FSteamInventoryEligiblePromoItemDefIDs
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FSteamID SteamID;                                                                 // 0x0008 (size: 0x8)
    int32 NumEligiblePromoItemDefs;                                                   // 0x0010 (size: 0x4)
    bool bCachedData;                                                                 // 0x0014 (size: 0x1)

}; // Size: 0x18

struct FSteamInventoryFullUpdate
{
    FSteamInventoryResult Handle;                                                     // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamInventoryRequestPricesResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString Currency;                                                                 // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FSteamInventoryResult
{
    int32 Value;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamInventoryResultReady
{
    FSteamInventoryResult Handle;                                                     // 0x0000 (size: 0x4)
    ESteamResult Result;                                                              // 0x0004 (size: 0x1)

}; // Size: 0x8

struct FSteamInventoryStartPurchaseResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString OrderID;                                                                  // 0x0008 (size: 0x10)
    FString TransactionId;                                                            // 0x0018 (size: 0x10)

}; // Size: 0x28

struct FSteamInventoryUpdateHandle
{
}; // Size: 0x8

struct FSteamItemDef
{
    int32 Value;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamItemDetails
{
    FSteamItemInstanceID InstanceId;                                                  // 0x0000 (size: 0x8)
    FSteamItemDef Definition;                                                         // 0x0008 (size: 0x4)
    int32 Quantity;                                                                   // 0x000C (size: 0x4)
    TArray<TEnumAsByte<ESteamCoreItemFlags>> Flags;                                   // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FSteamItemInstanceID
{
}; // Size: 0x8

struct FSteamLeaderboard
{
}; // Size: 0x8

struct FSteamLeaderboardEntries
{
}; // Size: 0x8

struct FSteamLeaderboardEntry
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    int32 GlobalRank;                                                                 // 0x0008 (size: 0x4)
    int32 Score;                                                                      // 0x000C (size: 0x4)
    int32 Details;                                                                    // 0x0010 (size: 0x4)
    FSteamUGCHandle UGCHandle;                                                        // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FSteamNetworkPingLocation
{
    FString Location;                                                                 // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FSteamP2PSessionState
{
    bool bConnectionActive;                                                           // 0x0000 (size: 0x1)
    bool bConnecting;                                                                 // 0x0001 (size: 0x1)
    ESteamP2PSessionError P2PSessionError;                                            // 0x0002 (size: 0x1)
    bool bUsingRelay;                                                                 // 0x0003 (size: 0x1)
    int32 BytesQueuedForSend;                                                         // 0x0004 (size: 0x4)
    int32 PacketsQueuedForSend;                                                       // 0x0008 (size: 0x4)
    FString RemoteIP;                                                                 // 0x0010 (size: 0x10)
    int32 RemotePort;                                                                 // 0x0020 (size: 0x4)

}; // Size: 0x28

struct FSteamParentalSettingsChanged
{
}; // Size: 0x1

struct FSteamPartyBeaconLocation
{
    ESteamPartiesBeaconLocationType Type;                                             // 0x0000 (size: 0x1)
    FString LocationId;                                                               // 0x0008 (size: 0x10)

}; // Size: 0x18

struct FSteamRemotePlaySessionConnected
{
    int32 SessionID;                                                                  // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamRemotePlaySessionDisconnected
{
    int32 SessionID;                                                                  // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FSteamServerAddr
{
    FString IP;                                                                       // 0x0000 (size: 0x10)
    int32 Port;                                                                       // 0x0010 (size: 0x4)
    int32 QueryPort;                                                                  // 0x0014 (size: 0x4)
    FString ConnectionAddressString;                                                  // 0x0018 (size: 0x10)
    FString SteamP2PAddr;                                                             // 0x0028 (size: 0x10)

}; // Size: 0x38

struct FSteamServerConnectFailure
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    bool bStillRetrying;                                                              // 0x0001 (size: 0x1)

}; // Size: 0x2

struct FSteamServersConnected
{
}; // Size: 0x1

struct FSteamServersDisconnected
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FSteamSessionResult
{
    FBlueprintSessionResult Result;                                                   // 0x0000 (size: 0x120)
    TArray<FSteamSessionSetting> SessionSettings;                                     // 0x0120 (size: 0x10)

}; // Size: 0x130

struct FSteamSessionSearchSetting
{
}; // Size: 0x20

struct FSteamSessionSetting
{
}; // Size: 0x28

struct FSteamShutdown
{
}; // Size: 0x1

struct FSteamTicketHandle
{
}; // Size: 0x4

struct FSteamUGCDetails
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    ESteamWorkshopFileType FileType;                                                  // 0x0009 (size: 0x1)
    int32 CreatorAppID;                                                               // 0x000C (size: 0x4)
    int32 ConsumerAppID;                                                              // 0x0010 (size: 0x4)
    FString Title;                                                                    // 0x0018 (size: 0x10)
    FString Description;                                                              // 0x0028 (size: 0x10)
    FSteamID SteamIDOwner;                                                            // 0x0038 (size: 0x8)
    int32 TimeCreated;                                                                // 0x0040 (size: 0x4)
    int32 TimeUpdated;                                                                // 0x0044 (size: 0x4)
    int32 TimeAddedToUserList;                                                        // 0x0048 (size: 0x4)
    ESteamRemoteStoragePublishedFileVisibility Visibility;                            // 0x004C (size: 0x1)
    bool bBanned;                                                                     // 0x004D (size: 0x1)
    bool bAcceptedForUse;                                                             // 0x004E (size: 0x1)
    bool bTagsTruncated;                                                              // 0x004F (size: 0x1)
    TArray<FString> Tags;                                                             // 0x0050 (size: 0x10)
    FSteamUGCHandle File;                                                             // 0x0060 (size: 0x8)
    FSteamUGCHandle PreviewFile;                                                      // 0x0068 (size: 0x8)
    FString Filename;                                                                 // 0x0070 (size: 0x10)
    int32 FileSize;                                                                   // 0x0080 (size: 0x4)
    int32 PreviewFileSize;                                                            // 0x0084 (size: 0x4)
    FString URL;                                                                      // 0x0088 (size: 0x10)
    int32 VotesUp;                                                                    // 0x0098 (size: 0x4)
    int32 VotesDown;                                                                  // 0x009C (size: 0x4)
    float Score;                                                                      // 0x00A0 (size: 0x4)
    int32 NumChildren;                                                                // 0x00A4 (size: 0x4)

}; // Size: 0xA8

struct FSteamUGCHandle
{
}; // Size: 0x8

struct FSteamUGCQueryCompleted
{
    FUGCQueryHandle Handle;                                                           // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    int32 NumResultsReturned;                                                         // 0x000C (size: 0x4)
    int32 TotalMatchingResults;                                                       // 0x0010 (size: 0x4)
    bool bCachedData;                                                                 // 0x0014 (size: 0x1)

}; // Size: 0x18

struct FStopPlaytimeTrackingResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

struct FStoreAuthURLResponse
{
    FString URL;                                                                      // 0x0000 (size: 0x10)

}; // Size: 0x10

struct FSubmitItemUpdateResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    bool bUserNeedsToAcceptWorkshopLegalAgreement;                                    // 0x0001 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FSubmitPlayerResultResultCallback
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FString UniqueGameID;                                                             // 0x0008 (size: 0x10)
    FSteamID SteamIDPlayer;                                                           // 0x0018 (size: 0x8)

}; // Size: 0x20

struct FTimedTrialStatus
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)
    bool bIsOffline;                                                                  // 0x0004 (size: 0x1)
    int32 SecondsAllowed;                                                             // 0x0008 (size: 0x4)
    int32 SecondsPlayed;                                                              // 0x000C (size: 0x4)

}; // Size: 0x10

struct FUGCDeleteItemResult
{
    ESteamResult Result;                                                              // 0x0000 (size: 0x1)
    FPublishedFileID PublishedFileID;                                                 // 0x0008 (size: 0x8)

}; // Size: 0x10

struct FUGCFileWriteStreamHandle
{
}; // Size: 0x8

struct FUGCQueryHandle
{
}; // Size: 0x8

struct FUGCUpdateHandle
{
}; // Size: 0x8

struct FUserAchievementIconFetched
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    FString AchievementName;                                                          // 0x0008 (size: 0x10)
    bool bAchieved;                                                                   // 0x0018 (size: 0x1)
    class UTexture2D* Icon;                                                           // 0x0020 (size: 0x8)

}; // Size: 0x30

struct FUserAchievementStored
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    bool bGroupAchievement;                                                           // 0x0008 (size: 0x1)
    FString AchievementName;                                                          // 0x0010 (size: 0x10)
    int32 CurrentProgress;                                                            // 0x0020 (size: 0x4)
    int32 MaxProgress;                                                                // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FUserFavoriteItemsListChanged
{
    FPublishedFileID PublishedFileID;                                                 // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    bool bWasAddRequest;                                                              // 0x0009 (size: 0x1)

}; // Size: 0x10

struct FUserStatsReceived
{
    FSteamID GameID;                                                                  // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)
    FSteamID SteamID;                                                                 // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FUserStatsStored
{
    FSteamGameID GameID;                                                              // 0x0000 (size: 0x8)
    ESteamResult Result;                                                              // 0x0008 (size: 0x1)

}; // Size: 0x10

struct FUserStatsUnloaded
{
    FSteamID SteamIDUser;                                                             // 0x0000 (size: 0x8)

}; // Size: 0x8

struct FUserSubscribedItemsListChanged
{
    int32 AppID;                                                                      // 0x0000 (size: 0x4)

}; // Size: 0x4

struct FValidateAuthTicketResponse
{
    FSteamID SteamID;                                                                 // 0x0000 (size: 0x8)
    ESteamAuthSessionResponse AuthSessionResponse;                                    // 0x0008 (size: 0x1)
    FSteamID OwnerSteamID;                                                            // 0x0010 (size: 0x8)

}; // Size: 0x18

struct FVolumeHasChanged
{
    float Volume;                                                                     // 0x0000 (size: 0x4)

}; // Size: 0x4

class ASteamCoreGameMode : public AGameMode
{
}; // Size: 0x380

class UAppList : public USteamCoreSubsystem
{
    FAppListSteamAppInstalled SteamAppInstalled;                                      // 0x0068 (size: 0x10)
    void OnSteamAppInstalled(const FSteamAppInstalled& Data);
    FAppListSteamAppUninstalled SteamAppUninstalled;                                  // 0x0078 (size: 0x10)
    void OnSteamAppUninstalled(const FSteamAppUninstalled& Data);

    int32 GetNumInstalledApps();
    int32 GetInstalledApps(TArray<int32>& AppIDs, int32 MaxAppIDs);
    int32 GetAppName(int32 AppID, FString& Name);
    int32 GetAppInstallDir(int32 AppID, FString& Directory);
    int32 GetAppBuildId(int32 AppID);
}; // Size: 0xC8

class UApps : public USteamCoreSubsystem
{
    FAppsDLCInstalled DLCInstalled;                                                   // 0x0068 (size: 0x10)
    void OnDLCInstalled(const FDLCInstalled& Data);
    FAppsFileDetailsResultDelegate FileDetailsResultDelegate;                         // 0x0078 (size: 0x10)
    void OnFileDetailsResultDelegate(const FFileDetailsResult& Data);
    FAppsNewUrlLaunchParametersDelegate NewUrlLaunchParametersDelegate;               // 0x0088 (size: 0x10)
    void OnNewUrlLaunchParametersDelegate();
    FAppsTimedTrialStatusDelegate TimedTrialStatusDelegate;                           // 0x0098 (size: 0x10)
    void OnTimedTrialStatusDelegate(const FTimedTrialStatus& Data);

    void UninstallDLC(int32 AppID);
    bool MarkContentCorrupt(bool bMissingFilesOnly);
    void InstallDLC(int32 AppID);
    FString GetLaunchQueryParam(FString Key);
    int32 GetLaunchCommandLine(FString& CommandLine);
    int32 GetInstalledDepots(int32 AppID, int32 MaxDepots, TArray<int32>& Depots);
    void GetFileDetails(const FGetFileDetailsCallback& Callback, FString Filename);
    int32 GetEarliestPurchaseUnixTime(int32 AppID);
    bool GetDlcDownloadProgress(int32 AppID, int32& BytesDownloaded, int32& BytesTotal);
    int32 GetDLCCount();
    FString GetCurrentGameLanguage();
    bool GetCurrentBetaName(FString& Name);
    FString GetAvailableGameLanguages();
    FSteamID GetAppOwner();
    int32 GetAppInstallDir(int32 AppID, FString& Folder);
    int32 GetAppBuildId();
    bool BIsVACBanned();
    bool BIsTimedTrial(int32& SecondsAllowed, int32& SecondsPlayed);
    bool BIsSubscribedFromFreeWeekend();
    bool BIsSubscribedFromFamilySharing();
    bool BIsSubscribedApp(int32 AppID);
    bool BIsSubscribed();
    bool BIsLowViolence();
    bool BIsDlcInstalled(int32 AppID);
    bool BIsCybercafe();
    bool BIsAppInstalled(int32 AppID);
    bool BGetDLCDataByIndex(int32 DLC, int32& AppID, bool& bAvailable, FString& Name);
}; // Size: 0x128

class UFriends : public USteamCoreSubsystem
{
    FFriendsAvatarImageLoaded AvatarImageLoaded;                                      // 0x0068 (size: 0x10)
    void OnAvatarImageLoaded(const FAvatarImageLoaded& Data);
    FFriendsFriendRichPresenceUpdate FriendRichPresenceUpdate;                        // 0x0078 (size: 0x10)
    void OnFriendRichPresenceUpdate(const FFriendRichPresenceUpdate& Data);
    FFriendsGameConnectedChatJoin GameConnectedChatJoin;                              // 0x0088 (size: 0x10)
    void OnGameConnectedChatJoin(const FGameConnectedChatJoin& Data);
    FFriendsGameConnectedChatLeave GameConnectedChatLeave;                            // 0x0098 (size: 0x10)
    void OnGameConnectedChatLeave(const FGameConnectedChatLeave& Data);
    FFriendsGameConnectedClanChatMsg GameConnectedClanChatMsg;                        // 0x00A8 (size: 0x10)
    void OnGameConnectedClanChatMsg(const FGameConnectedClanChatMsg& Data);
    FFriendsGameConnectedFriendChatMsg GameConnectedFriendChatMsg;                    // 0x00B8 (size: 0x10)
    void OnGameConnectedFriendChatMsg(const FGameConnectedFriendChatMsg& Data);
    FFriendsGameLobbyJoinRequested GameLobbyJoinRequested;                            // 0x00C8 (size: 0x10)
    void OnGameLobbyJoinRequested(const FGameLobbyJoinRequested& Data);
    FFriendsGameOverlayActivated GameOverlayActivated;                                // 0x00D8 (size: 0x10)
    void OnGameOverlayActivated(const FGameOverlayActivated& Data);
    FFriendsGameRichPresenceJoinRequested GameRichPresenceJoinRequested;              // 0x00E8 (size: 0x10)
    void OnGameRichPresenceJoinRequested(const FGameRichPresenceJoinRequested& Data);
    FFriendsGameServerChangeRequested GameServerChangeRequested;                      // 0x00F8 (size: 0x10)
    void OnGameServerChangeRequested(const FGameServerChangeRequested& Data);
    FFriendsPersonaStateChange PersonaStateChange;                                    // 0x0108 (size: 0x10)
    void OnPersonaStateChange(const FPersonaStateChange& Data);
    FFriendsSetPersonaNameResponse SetPersonaNameResponse;                            // 0x0118 (size: 0x10)
    void OnSetPersonaNameResponse(const FSetPersonaNameResponse& Data);
    FFriendsJoinClanChatRoomCompletionResult JoinClanChatRoomCompletionResult;        // 0x0128 (size: 0x10)
    void OnJoinClanChatRoomCompletionResult(const FJoinClanChatRoomCompletionResult& Data);
    FFriendsClanOfficerListResponse ClanOfficerListResponse;                          // 0x0138 (size: 0x10)
    void OnClanOfficerListResponse(const FClanOfficerListResponse& Data);
    FFriendsDownloadClanActivityCountsResult DownloadClanActivityCountsResult;        // 0x0148 (size: 0x10)
    void OnDownloadClanActivityCountsResult(const FDownloadClanActivityCountsResult& Data);

    bool SetRichPresence(FString Key, FString Value);
    void SetPlayedWith(FSteamID SteamIDUserPlayedWith);
    void SetPersonaName(const FSetPersonaNameCallback& Callback, FString Name);
    bool SetListenForFriendsMessages(bool bInterceptEnabled);
    void SetInGameVoiceSpeaking(FSteamID SteamIDUser, bool bSpeaking);
    bool SendClanChatMessage(FSteamID SteamIDClanChat, FString Text);
    bool RequestUserInformation(FSteamID SteamIDUser, bool bRequireNameOnly);
    void RequestFriendRichPresence(FSteamID SteamIDFriend);
    void RequestClanOfficerList(const FRequestClanOfficerListCallback& Callback, FSteamID SteamIDClan);
    bool ReplyToFriendMessage(FSteamID SteamIDFriend, FString MsgToSend);
    bool RegisterProtocolInOverlayBrowser(FString Protocol);
    bool OpenClanChatWindowInSteam(FSteamID SteamIDClanChat);
    bool LeaveClanChatRoom(FSteamID SteamIDClan);
    void JoinClanChatRoom(const FJoinClanChatRoomCallback& Callback, FSteamID SteamIDClan);
    bool IsUserInSource(FSteamID SteamIDUser, FSteamID SteamIDSource);
    void IsFollowing(const FIsFollowingCallback& Callback, FSteamID SteamID);
    bool IsClanPublic(FSteamID SteamIDClan);
    bool IsClanOfficialGameGroup(FSteamID SteamIDClan);
    bool IsClanChatWindowOpenInSteam(FSteamID SteamIDClanChat);
    bool IsClanChatAdmin(FSteamID SteamIDClanChat, FSteamID SteamIDUser);
    bool InviteUserToGame(FSteamID SteamIDFriend, FString ConnectString);
    bool HasFriend(FSteamID SteamIDFriend, TArray<ESteamFriendFlags> Flags);
    TArray<ESteamUserRestriction> GetUserRestrictions();
    class UTexture2D* GetSmallFriendAvatar(FSteamID SteamIDFriend);
    FString GetPlayerNickname_Pure(FSteamID SteamIDPlayer);
    FString GetPlayerNickname(FSteamID SteamIDPlayer);
    ESteamPersonaState GetPersonaState_Pure();
    ESteamPersonaState GetPersonaState();
    FString GetPersonaName_Pure();
    FString GetPersonaName();
    class UTexture2D* GetMediumFriendAvatar(FSteamID SteamIDFriend);
    class UTexture2D* GetLargeFriendAvatar(FSteamID SteamIDFriend);
    int32 GetFriendSteamLevel(FSteamID SteamIDFriend);
    FString GetFriendsGroupName(FSteamFriendsGroupID FriendsGroupID);
    void GetFriendsGroupMembersList(FSteamFriendsGroupID FriendsGroupID, TArray<FSteamID>& SteamIDMembers, int32 MembersCount);
    int32 GetFriendsGroupMembersCount(FSteamFriendsGroupID FriendsGroupID);
    FSteamFriendsGroupID GetFriendsGroupIDByIndex(int32 FriendGroup);
    int32 GetFriendsGroupCount();
    int32 GetFriendRichPresenceKeyCount(FSteamID SteamIDFriend);
    FString GetFriendRichPresenceKeyByIndex(FSteamID SteamIDFriend, int32 Key);
    FString GetFriendRichPresence(FSteamID SteamIDFriend, FString Key);
    ESteamFriendRelationship GetFriendRelationship(FSteamID SteamIDFriend);
    ESteamPersonaState GetFriendPersonaState(FSteamID SteamIDFriend);
    FString GetFriendPersonaNameHistory(FSteamID SteamIDFriend, int32 PersonaName);
    FString GetFriendPersonaName(FSteamID SteamIDFriend);
    int32 GetFriendMessage(FSteamID SteamIDFriend, int32 MessageID, FString& Text, ESteamChatEntryType& ChatEntryType);
    bool GetFriendGamePlayed(FSteamID SteamIDFriend, FSteamGameID& GameID, FString& GameIP, int32& ConnectionPort, int32& QueryPort, FSteamID& SteamIDLobby);
    FSteamID GetFriendFromSourceByIndex(FSteamID SteamIDSource, int32 Ifriend);
    int32 GetFriendCountFromSource(FSteamID SteamIDSource);
    int32 GetFriendCount(TArray<ESteamFriendFlags> Flags);
    int32 GetFriendCoplayTime(FSteamID SteamIDFriend);
    int32 GetFriendCoplayGame(FSteamID SteamIDFriend);
    FSteamID GetFriendByIndex(int32 Ifriend, TArray<ESteamFriendFlags> Flags);
    void GetFollowerCount(const FGetFollowerCountCallback& Callback, FSteamID SteamID);
    int32 GetCoplayFriendCount();
    FSteamID GetCoplayFriend(int32 CoplayFriend);
    FString GetClanTag(FSteamID SteamIDClan);
    FSteamID GetClanOwner(FSteamID SteamIDClan);
    int32 GetClanOfficerCount(FSteamID SteamIDClan);
    FSteamID GetClanOfficerByIndex(FSteamID SteamIDClan, int32 Officer);
    FString GetClanName(FSteamID SteamIDClan);
    int32 GetClanCount();
    int32 GetClanChatMessage(FSteamID SteamIDClanChat, int32 MessageID, FString& Text, ESteamChatEntryType& ChatEntryType, FSteamID& SteamIDChatter);
    int32 GetClanChatMemberCount(FSteamID SteamIDClan);
    FSteamID GetClanByIndex(int32 Clan);
    bool GetClanActivityCounts(FSteamID SteamIDClan, int32& Online, int32& InGame, int32& Chatting);
    FSteamID GetChatMemberByIndex(FSteamID SteamIDClan, int32 User);
    void EnumerateFollowingList(const FEnumerateFollowingListCallback& Callback, int32 StartIndex);
    void DownloadClanActivityCounts(const FDownloadClanActivityCountsCallback& Callback, TArray<FSteamID> SteamIDClans);
    bool CloseClanChatWindowInSteam(FSteamID SteamIDClanChat);
    void ClearRichPresence();
    void ActivateGameOverlayToWebPage(FString URL, ESteamActivateGameOverlayToWebPageMode Mode);
    void ActivateGameOverlayToUser(FString Dialog, FSteamID SteamID);
    void ActivateGameOverlayToStore(int32 AppID, ESteamOverlayToStoreFlag Flag);
    void ActivateGameOverlayInviteDialogConnectString(FString ConnectString);
    void ActivateGameOverlayInvitedialog(FSteamID SteamIDLobby);
    void ActivateGameOverlay(FString Dialog);
}; // Size: 0x338

class UGameServerStats : public USteamCoreSubsystem
{
    FGameServerStatsGSStatsUnloaded GSStatsUnloaded;                                  // 0x0068 (size: 0x10)
    void OnGSStatsUnloaded(const FGSStatsUnloaded& Data);

    bool UpdateUserAvgRateStat(FSteamID SteamIDUser, FString Name, float CountThisSession, float SessionLength);
    bool SetUserStatInt(FSteamID SteamIDUser, FString Name, int32 Data);
    bool SetUserStatFloat(FSteamID SteamIDUser, FString Name, float Data);
    bool SetUserAchievement(FSteamID SteamIDUser, FString Name);
    void ServerStoreUserStats(const FServerStoreUserStatsCallback& Callback, FSteamID SteamIDUser);
    void ServerRequestUserStats(const FServerRequestUserStatsCallback& Callback, FSteamID SteamIDUser);
    bool GetUserStatInt(FSteamID SteamIDUser, FString Name, int32& Data);
    bool GetUserStatFloat(FSteamID SteamIDUser, FString Name, float& Data);
    bool GetUserAchievement(FSteamID SteamIDUser, FString Name, bool& bAchieved);
    bool ClearUserAchievement(FSteamID SteamIDUser, FString Name);
}; // Size: 0x98

class UInput : public USteamCoreSubsystem
{
    TMap<int32, FString> InputActionOrigin;                                           // 0x0068 (size: 0x50)

    void TriggerVibration(FInputHandle Handle, float LeftSpeed, float RightSpeed);
    void TriggerRepeatedHapticPulse(FInputHandle Handle, ESteamCoreControllerPad TargetPad, float DurationMicroSec, float OffMicroSec, int32 Repeat, uint8 Flags);
    void TriggerHapticPulse(FInputHandle Handle, ESteamCoreControllerPad TargetPad, float DurationMicroSec);
    int32 TranslateActionOrigin(ESteamCoreInputType DestinationInputType, int32 SourceOrigin);
    void StopAnalogActionMomentum(FInputHandle Handle, FInputAnalogActionHandle EAction);
    bool Shutdown();
    bool ShowBindingPanel(FInputHandle Handle);
    void SetLEDColor(FInputHandle Handle, uint8 ColorR, uint8 ColorG, uint8 ColorB, ESteamCoreInputLEDFlag Flags);
    bool Init();
    FString GetStringForXboxOrigin(ESteamCoreXboxOrigin Origin);
    FString GetStringForActionOrigin(int32 Origin);
    int32 GetRemotePlaySessionID(FInputHandle Handle);
    FInputMotionData GetMotionData(FInputHandle Handle);
    ESteamCoreInputType GetInputTypeForHandle(FInputHandle Handle);
    TArray<FString> GetInputActionOrigins();
    FString GetInputActionOriginAsString(int32 ID);
    int32 GetInputActionOriginAsInteger(FString Value);
    FString GetGlyphForXboxOrigin(ESteamCoreXboxOrigin Origin);
    FString GetGlyphForActionOrigin(int32 Origin);
    int32 GetGamepadIndexForController(FInputHandle Handle);
    int32 GetDigitalActionOrigins(FInputHandle Handle, FInputActionSetHandle ActionSetHandle, FInputDigitalActionHandle DigitalActionHandle, TArray<int32>& OriginsOut);
    FInputDigitalActionHandle GetDigitalActionHandle(FString PszActionName);
    FInputDigitalActionData GetDigitalActionData(FInputHandle Handle, FInputDigitalActionHandle DigitalActionHandle);
    bool GetDeviceBindingRevision(FInputHandle Handle, int32& Major, int32& Minor);
    FInputActionSetHandle GetCurrentActionSet(FInputHandle Handle);
    FInputHandle GetControllerForGamepadIndex(int32 Index);
    int32 GetConnectedControllers(TArray<FInputHandle>& OutHandles);
    int32 GetAnalogActionOrigins(FInputHandle Handle, FInputActionSetHandle ActionSetHandle, FInputAnalogActionHandle AnalogActionHandle, TArray<int32>& OriginsOut);
    FInputAnalogActionHandle GetAnalogActionHandle(FString PszActionName);
    FInputAnalogActionData GetAnalogActionData(FInputHandle Handle, FInputAnalogActionHandle AnalogActionHandle);
    int32 GetActiveActionSetLayers(FInputHandle Handle, TArray<FInputActionSetHandle>& Data);
    FInputActionSetHandle GetActionSetHandle(FString ActionSetName);
    int32 GetActionOriginFromXboxOrigin(FInputHandle Handle, ESteamCoreXboxOrigin Origin);
    void DeactivateAllActionSetLayers(FInputHandle Handle);
    void DeactivateActionSetLayer(FInputHandle Handle, FInputActionSetHandle ActionSetLayerHandle);
    void ActivateActionSetLayer(FInputHandle Handle, FInputActionSetHandle ActionSetLayerHandle);
    void ActivateActionSet(FInputHandle Handle, FInputActionSetHandle ActionSetHandle);
}; // Size: 0xB8

class UInventory : public USteamCoreSubsystem
{
    FInventorySteamInventoryResultReady SteamInventoryResultReady;                    // 0x0068 (size: 0x10)
    void OnSteamInventoryResultReady(const FSteamInventoryResultReady& Data);
    FInventorySteamInventoryDefinitionUpdate SteamInventoryDefinitionUpdate;          // 0x0078 (size: 0x10)
    void OnSteamInventoryDefinitionUpdate();
    FInventorySteamInventoryFullUpdate SteamInventoryFullUpdate;                      // 0x0088 (size: 0x10)
    void OnSteamInventoryFullUpdate(const FSteamInventoryFullUpdate& Data);
    FInventorySteamInventoryStartPurchaseResult SteamInventoryStartPurchaseResult;    // 0x0098 (size: 0x10)
    void OnSteamInventoryStartPurchaseResultDelegate(const FSteamInventoryStartPurchaseResult& Data);
    FInventorySteamInventoryRequestPricesResultDelegate SteamInventoryRequestPricesResultDelegate; // 0x00A8 (size: 0x10)
    void OnSteamInventoryRequestPricesResultDelegate(const FSteamInventoryRequestPricesResult& Data);
    FInventorySteamInventoryEligiblePromoItemDefIDs SteamInventoryEligiblePromoItemDefIDs; // 0x00B8 (size: 0x10)
    void OnSteamInventoryEligiblePromoItemDefIDs(const FSteamInventoryEligiblePromoItemDefIDs& Data);

    bool TriggerItemDrop(FSteamInventoryResult& Result, FSteamItemDef ListDefinition);
    bool TransferItemQuantity(FSteamInventoryResult& Result, FSteamItemInstanceID ItemIDSource, int32 Quantity, FSteamItemInstanceID ItemIDDest);
    bool SubmitUpdateProperties(FSteamInventoryUpdateHandle Handle, FSteamInventoryResult& ResultHandle);
    FSteamInventoryUpdateHandle StartUpdateProperties();
    void StartPurchase(const FStartPurchaseCallback& Callback, const TArray<FSteamItemDef> ItemDefs, const TArray<int32> Quantity);
    bool SetPropertyString(FSteamInventoryUpdateHandle Handle, FSteamItemInstanceID ItemId, FString PropertyName, FString Value);
    bool SetPropertyInt(FSteamInventoryUpdateHandle Handle, FSteamItemInstanceID ItemId, FString PropertyName, int32 Value);
    bool SetPropertyFloat(FSteamInventoryUpdateHandle Handle, FSteamItemInstanceID ItemId, FString PropertyName, float Value);
    bool SetPropertyBool(FSteamInventoryUpdateHandle Handle, FSteamItemInstanceID ItemId, FString PropertyName, bool bValue);
    bool SerializeResult(FSteamInventoryResult Handle, TArray<uint8>& Buffer);
    void RequestPrices(const FRequestPricesCallback& Callback);
    void RequestEligiblePromoItemDefinitionsIDs(const FRequestEligiblePromoItemDefinitionsIDsCallback& Callback, FSteamID SteamID);
    bool RemoveProperty(FSteamInventoryUpdateHandle Handle, FSteamItemInstanceID ItemId, FString PropertyName);
    bool LoadItemDefinitions();
    bool GrantPromoItems(FSteamInventoryResult& Result);
    int32 GetResultTimestamp(FSteamInventoryResult Handle);
    ESteamResult GetResultStatus(FSteamInventoryResult Handle);
    bool GetResultItems(FSteamInventoryResult Handle, TArray<FSteamItemDetails>& Items);
    bool GetResultItemProperty(FSteamInventoryResult Handle, int32 ItemIndex, FString PropertyName, FString& Value);
    int32 GetNumItemsWithPrices();
    bool GetItemsWithPrices(TArray<FSteamItemDef>& ItemDefs, TArray<int32>& Prices, TArray<int32>& BasePrices);
    bool GetItemsByID(FSteamInventoryResult& InventoryResult, TArray<FSteamItemInstanceID> InstanceIDs);
    bool GetItemPrice(FSteamItemDef ItemDef, int32& Price, int32& BasePrice);
    bool GetItemDefinitionProperty(FSteamItemDef ItemDef, FString PropertyName, FString& Value);
    bool GetItemDefinitionIDs(TArray<FSteamItemDef>& ItemDefs);
    bool GetEligiblePromoItemDefinitionIDs(FSteamID SteamID, TArray<FSteamItemDef>& ItemDefs);
    bool GetAllItems(FSteamInventoryResult& Handle);
    bool GenerateItems(FSteamInventoryResult& Result, TArray<FSteamItemDef> ItemDefs, TArray<int32> Quantity);
    bool ExchangeItems(FSteamInventoryResult& Result, TArray<FSteamItemDef> ArrayGenerate, TArray<int32> ArrayGenerateQuantity, TArray<FSteamItemInstanceID> ArrayDestroy, TArray<int32> ArrayDestroyQuantity);
    void DestroyResult(FSteamInventoryResult Handle);
    bool DeserializeResult(FSteamInventoryResult& Result, TArray<uint8> Buffer, bool bReservedMustBeFalse);
    bool ConsumeItem(FSteamInventoryResult& Result, FSteamItemInstanceID ItemConsume, int32 Quantity);
    bool CheckResultSteamID(FSteamInventoryResult Handle, FSteamID SteamIDExpected);
    bool AddPromoItems(FSteamInventoryResult& OutResult, TArray<FSteamItemDef> ItemDefs);
    bool AddPromoItem(FSteamInventoryResult& Result, FSteamItemDef ItemDef);
}; // Size: 0x188

class UMatchmaking : public USteamCoreSubsystem
{
    FMatchmakingFavoritesListAccountsUpdated FavoritesListAccountsUpdated;            // 0x0068 (size: 0x10)
    void OnFavoritesListAccountsUpdated(const FFavoritesListAccountsUpdated& Data);
    FMatchmakingFavoritesListChanged FavoritesListChanged;                            // 0x0078 (size: 0x10)
    void OnFavoritesListChanged(const FFavoritesListChanged& Data);
    FMatchmakingLobbyChatMsg LobbyChatMsg;                                            // 0x0088 (size: 0x10)
    void OnLobbyChatMsg(const FLobbyChatMsg& Data);
    FMatchmakingLobbyChatUpdate LobbyChatUpdate;                                      // 0x0098 (size: 0x10)
    void OnLobbyChatUpdate(const FLobbyChatUpdate& Data);
    FMatchmakingLobbyDataUpdate LobbyDataUpdate;                                      // 0x00A8 (size: 0x10)
    void OnLobbyDataUpdate(FLobbyDataUpdate Data);
    FMatchmakingLobbyEnter LobbyEnter;                                                // 0x00B8 (size: 0x10)
    void OnLobbyEnter(const FLobbyEnterData& Data);
    FMatchmakingLobbyGameCreated LobbyGameCreated;                                    // 0x00C8 (size: 0x10)
    void OnLobbyGameCreated(const FLobbyGameCreated& Data);
    FMatchmakingLobbyInvite LobbyInvite;                                              // 0x00D8 (size: 0x10)
    void OnLobbyInvite(const FLobbyInviteData& Data);
    FMatchmakingLobbyKicked LobbyKicked;                                              // 0x00E8 (size: 0x10)
    void OnLobbyKicked(const FLobbyKickedData& Data);

    bool SetLobbyType(FSteamID SteamIDLobby, ESteamLobbyType LobbyType);
    bool SetLobbyOwner(FSteamID SteamIDLobby, FSteamID SteamIDNewOwner);
    bool SetLobbyMemberLimit(FSteamID SteamIDLobby, int32 MaxMembers);
    void SetLobbyMemberData(FSteamID SteamIDLobby, FString Key, FString Value);
    bool SetLobbyJoinable(FSteamID SteamIDLobby, bool bLobbyJoinable);
    void SetLobbyGameServer(FSteamID SteamIDLobby, FString GameServerIP, int32 GameServerPort, FSteamID SteamIDGameServer);
    bool SetLobbyData(FSteamID SteamIDLobby, FString Key, FString Value);
    bool SetLinkedLobby(FSteamID SteamIDLobby, FSteamID SteamIDLobbyDependent);
    bool SendLobbyChatMsg(FSteamID SteamIDLobby, FString Message);
    void RequestLobbyList(const FRequestLobbyListCallback& Callback);
    bool RequestLobbyData(FSteamID SteamIDLobby);
    bool RemoveFavoriteGame(int32 AppID, FString IP, int32 ConnectionPort, int32 QueryPort, TArray<ESteamFavoriteFlags> Flags);
    void LeaveLobby(FSteamID SteamIDLobby);
    void JoinLobby(const FJoinLobbyCallback& Callback, FSteamID SteamIDLobby);
    bool InviteUserToLobby(FSteamID SteamIDLobby, FSteamID SteamIDInvitee);
    int32 GetNumLobbyMembers(FSteamID SteamIDLobby);
    FSteamID GetLobbyOwner(FSteamID SteamIDLobby);
    int32 GetLobbyMemberLimit(FSteamID SteamIDLobby);
    FString GetLobbyMemberData(FSteamID SteamIDLobby, FSteamID SteamIDUser, FString Key);
    FSteamID GetLobbyMemberByIndex(FSteamID SteamIDLobby, int32 Member);
    bool GetLobbyGameServer(FSteamID SteamIDLobby, FString& GameServerIP, int32& GameServerPort, FSteamID& SteamIDGameServer);
    int32 GetLobbyDataCount(FSteamID SteamIDLobby);
    bool GetLobbyDataByIndex(FSteamID SteamIDLobby, int32 LobbyData, FString& Key, FString& Value);
    FString GetLobbyData(FSteamID SteamIDLobby, FString Key);
    int32 GetLobbyChatEntry(FSteamID SteamIDLobby, int32 MessageID, FSteamID& SteamIDUser, FString& Message, ESteamChatEntryType& ChatEntryType);
    FSteamID GetLobbyByIndex(int32 Lobby);
    int32 GetFavoriteGameCount();
    bool GetFavoriteGame(int32 Game, int32& AppID, FString& IP, int32& ConnectionPort, int32& QueryPort, TArray<ESteamFavoriteFlags>& Flags, int32& TimeLastPlayedOnServer);
    bool DeleteLobbyData(FSteamID SteamIDLobby, FString Key);
    void CreateLobby(const FCreateLobbyCallback& Callback, ESteamLobbyType LobbyType, int32 MaxMembers);
    void AddRequestLobbyListStringFilter(FString KeyToMatch, FString ValueToMatch, ESteamLobbyComparison ComparisonType);
    void AddRequestLobbyListResultCountFilter(int32 MaxResults);
    void AddRequestLobbyListNumericalFilter(FString KeyToMatch, int32 ValueToMatch, ESteamLobbyComparison ComparisonType);
    void AddRequestLobbyListNearValueFilter(FString KeyToMatch, int32 ValueToBeCloseTo);
    void AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable);
    void AddRequestLobbyListDistanceFilter(ESteamLobbyDistanceFilter LobbyDistanceFilter);
    void AddRequestLobbyListCompatibleMembersFilter(FSteamID SteamIDLobby);
    int32 AddFavoriteGame(int32 AppID, FString IP, int32 ConnectionPort, int32 QueryPort, TArray<ESteamFavoriteFlags> Flags, int32 TimeLastPlayedOnServer);
}; // Size: 0x218

class UMatchmakingServers : public USteamCoreSubsystem
{

    void ServerRules(const FServerRulesCallback& Callback, FString IP, int32 QueryPort);
    void RequestSpectatorServerList(const FRequestSpectatorServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void RequestLANServerList(const FRequestLANServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void RequestInternetServerList(const FRequestInternetServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void RequestHistoryServerList(const FRequestHistoryServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void RequestFriendsServerList(const FRequestFriendsServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void RequestFavoritesServerList(const FRequestFavoritesServerListServerCallback& ServerCallback, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void PingServer(const FPingServerCallback& Callback, FString IP, int32 QueryPort);
}; // Size: 0x68

class UMusic : public USteamCoreSubsystem
{
    FMusicPlaybackStatusHasChanged PlaybackStatusHasChanged;                          // 0x0068 (size: 0x10)
    void OnPlaybackStatusHasChanged(const FPlaybackStatusHasChanged& Data);
    FMusicVolumeHasChanged VolumeHasChanged;                                          // 0x0078 (size: 0x10)
    void OnVolumeHasChanged(const FVolumeHasChanged& Data);

    void SetVolume(float flVolume);
    void PlayPrevious();
    void PlayNext();
    void Play();
    void Pause();
    float GetVolume();
    ESteamAudioPlaybackStatus GetPlaybackStatus();
    bool BIsPlaying();
    bool bIsEnabled();
}; // Size: 0xC8

class UNetworking : public USteamCoreSubsystem
{
    FNetworkingOnP2PSessionRequestDelegate OnP2PSessionRequestDelegate;               // 0x0068 (size: 0x10)
    void OnP2PSessionRequest(const FP2PSessionRequest& Data);
    FNetworkingOnP2PSessionConnectFailDelegate OnP2PSessionConnectFailDelegate;       // 0x0078 (size: 0x10)
    void OnP2PSessionConnectFail(const FP2PSessionConnectFail& Data);

    bool SendP2PPacket(FSteamID SteamIDRemote, TArray<uint8> Data, ESteamP2PSend P2PSendType, int32 Channel);
    bool ReadP2PPacket(TArray<uint8>& Data, FSteamID& OutSteamIdRemote, int32 MessageSize, int32 Channel);
    bool IsP2PPacketAvailable(int32& MessageSize, int32 Channel);
    bool GetP2PSessionState(FSteamID SteamIDRemote, FSteamP2PSessionState& ConnectionState);
    bool CloseP2PSessionWithUser(FSteamID SteamIDRemote);
    bool CloseP2PChannelWithUser(FSteamID SteamIDRemote, int32 Channel);
    bool AllowP2PPacketRelay(bool bAllow);
    bool AcceptP2PSessionWithUser(FSteamID SteamIDRemote);
}; // Size: 0xC8

class UNetworkingUtils : public USteamCoreSubsystem
{

    bool ParsePingLocationString(FString String, FSteamNetworkPingLocation& Result);
    void InitRelayNetworkAccess();
    float GetLocalPingLocation(FSteamNetworkPingLocation& Result);
    int32 EstimatePingTimeFromLocalHost(const FSteamNetworkPingLocation& RemoteLocation);
    int32 EstimatePingTimeBetweenTwoLocations(const FSteamNetworkPingLocation& Location1, const FSteamNetworkPingLocation& Location2);
    void ConvertPingLocationToString(const FSteamNetworkPingLocation& Location, FString& String);
    bool CheckPingDataUpToDate(float MaxAgeSeconds);
}; // Size: 0x68

class UParentalSettings : public USteamCoreSubsystem
{

    bool BIsParentalLockLocked();
    bool BIsParentalLockEnabled();
    bool BIsFeatureInBlockList(ESteamParentalFeature Feature);
    bool BIsFeatureBlocked(ESteamParentalFeature Feature);
    bool BIsAppInBlockList(int32 AppID);
    bool BIsAppBlocked(int32 AppID);
}; // Size: 0x68

class URemotePlay : public USteamCoreSubsystem
{
    FRemotePlaySteamRemotePlaySessionConnected SteamRemotePlaySessionConnected;       // 0x0068 (size: 0x10)
    void OnSteamRemotePlaySessionConnected(const FSteamRemotePlaySessionConnected& Data);
    FRemotePlaySteamRemotePlaySessionDisconnected SteamRemotePlaySessionDisconnected; // 0x0078 (size: 0x10)
    void OnSteamRemotePlaySessionDisconnected(const FSteamRemotePlaySessionDisconnected& Data);

    FSteamID GetSessionSteamID(int32 SessionID);
    int32 GetSessionID(int32 SessionIndex);
    int32 GetSessionCount();
    FString GetSessionClientName(int32 SessionID);
    ESteamCoreDeviceFormFactor GetSessionClientFormFactor(int32 SessionID);
    bool BSendRemotePlayTogetherInvite(FSteamID SteamIDFriend);
    bool BGetSessionClientResolution(int32 SessionID, int32& ResolutionX, int32& ResolutionY);
}; // Size: 0xC8

class URemoteStorage : public USteamCoreSubsystem
{
    FRemoteStorageRemoteStorageUnsubscribePublishedFileResult RemoteStorageUnsubscribePublishedFileResult; // 0x0068 (size: 0x10)
    void OnRemoteStorageUnsubscribePublishedFileResult(const FRemoteStorageUnsubscribePublishedFileResult& Data);
    FRemoteStorageRemoteStorageSubscribePublishedFileResult RemoteStorageSubscribePublishedFileResult; // 0x0078 (size: 0x10)
    void OnRemoteStorageSubscribePublishedFileResult(const FRemoteStorageSubscribePublishedFileResult& Data);
    FRemoteStorageRemoteStoragePublishedFileUnsubscribed RemoteStoragePublishedFileUnsubscribed; // 0x0088 (size: 0x10)
    void OnRemoteStoragePublishedFileUnsubscribed(const FRemoteStoragePublishedFileUnsubscribed& Data);
    FRemoteStorageRemoteStoragePublishedFileSubscribed RemoteStoragePublishedFileSubscribed; // 0x0098 (size: 0x10)
    void OnRemoteStoragePublishedFileSubscribed(const FRemoteStoragePublishedFileSubscribed& Data);

    int32 UGCRead(FSteamUGCHandle Content, TArray<uint8>& OutData, int32 DataToRead, int32 Offset, ESteamUGCReadAction Action);
    void UGCDownloadToLocation(const FUGCDownloadToLocationCallback& Callback, FSteamUGCHandle Content, FString Location, int32 Priority);
    void UGCDownload(const FUGCDownloadCallback& Callback, FSteamUGCHandle Content, int32 Priority);
    bool SetSyncPlatforms(FString File, ESteamRemoteStoragePlatform RemoteStoragePlatform);
    void SetCloudEnabledForApp(bool bEnabled);
    bool IsCloudEnabledForApp();
    bool IsCloudEnabledForAccount();
    bool GetUGCDownloadProgress(FSteamUGCHandle Handle, int32& BytesDownloaded, int32& BytesExpected);
    bool GetUGCDetails(FSteamUGCHandle Handle, int32& AppID, FString& Name, int32& FileSizeInBytes, FSteamID& SteamIDOwner);
    ESteamRemoteStoragePlatform GetSyncPlatforms(FString File);
    bool GetQuota(int32& TotalBytes, int32& AvailableBytes);
    int32 GetFileTimestamp(FString File);
    int32 GetFileSize(FString File);
    FString GetFileNameAndSize(int32 File, int32& FileSizeInBytes);
    int32 GetFileCount();
    FSteamUGCHandle GetCachedUGCHandle(int32 ICachedContent);
    int32 GetCachedUGCCount();
    bool FileWriteStreamWriteChunk(FUGCFileWriteStreamHandle Handle, TArray<uint8> Data);
    FUGCFileWriteStreamHandle FileWriteStreamOpen(FString File);
    bool FileWriteStreamClose(FUGCFileWriteStreamHandle Handle);
    bool FileWriteStreamCancel(FUGCFileWriteStreamHandle Handle);
    void FileWriteAsync(const FFileWriteAsyncCallback& Callback, FString File, TArray<uint8> Data);
    bool FileWrite(FString File, TArray<uint8> Data);
    void FileShare(const FFileShareCallback& Callback, FString File);
    bool FileReadAsyncComplete(FRemoteStorageFileReadAsyncComplete ReadCall, TArray<uint8>& Buffer, int32 BytesToRead);
    void FileReadAsync(const FFileReadAsyncCallback& Callback, FString File, int32 Offset, int32 BytesToRead);
    int32 FileRead(FString File, TArray<uint8>& Buffer, int32 DataToRead);
    bool FilePersisted(FString File);
    bool FileForget(FString File);
    bool FileExists(FString File);
    bool FileDelete(FString File);
}; // Size: 0x128

class UScreenshots : public USteamCoreSubsystem
{
    FScreenshotsScreenshotReady ScreenshotReady;                                      // 0x0068 (size: 0x10)
    void OnScreenshotReady(const FScreenshotReady& Data);
    FScreenshotsScreenshotRequested ScreenshotRequested;                              // 0x0078 (size: 0x10)
    void OnScreenshotRequested(const FScreenshotRequested& Data);

    FScreenshotHandle WriteScreenshot(TArray<uint8> PubRGB, int32 Width, int32 Height);
    void TriggerScreenshot();
    bool TagUser(FScreenshotHandle Handle, FSteamID SteamID);
    bool TagPublishedFile(FScreenshotHandle Handle, FPublishedFileID PublishedFileID);
    bool SetLocation(FScreenshotHandle Handle, FString Location);
    bool IsScreenshotsHooked();
    void HookScreenshots(bool bHook);
    FScreenshotHandle AddVRScreenshotToLibrary(ESteamVRScreenshotType EType, FString Filename, FString VRFileName);
    FScreenshotHandle AddScreenshotToLibrary(FString Filename, FString ThumbnailFilename, int32 Width, int32 Height);
}; // Size: 0xC8

class UServerFilter : public UObject
{

    TMap<class FString, class FString> GetFilters();
    void AddFilterWhitelisted();
    void AddFilterVersion(FString Value);
    void AddFilterSecure();
    void AddFilterProxy();
    void AddFilterPassword();
    void AddFilterOr(FString Value);
    void AddFilterNotFull();
    void AddFilterNotAppId(FString Value);
    void AddFilterNor(FString Value);
    void AddFilterNoPlayers();
    void AddFilterNand(FString Value);
    void AddFilterName(FString Value);
    void AddFilterMap(FString Value);
    void AddFilterLinux();
    void AddFilterHasPlayers();
    void AddFilterGameTagsNor(FString Value);
    void AddFilterGameTagsAnd(FString Value);
    void AddFilterGameDataOr(FString Value);
    void AddFilterGameDataNor(FString Value);
    void AddFilterGameDataAnd(FString Value);
    void AddFilterGameAddr(FString Value);
    void AddFilterDedicated();
    void AddFilterAnd(FString Value);
}; // Size: 0x38

class USteamCore : public UObject
{
}; // Size: 0x28

class USteamCoreAppsAsyncActionGetFileDetails : public USteamCoreAsyncAction
{
    FSteamCoreAppsAsyncActionGetFileDetailsOnCallback OnCallback;                     // 0x0038 (size: 0x10)
    void OnFileDetailsResultAsyncDelegate(const FFileDetailsResult& Data, bool bWasSuccessful);

    void HandleCallback(const FFileDetailsResult& Data, bool bWasSuccessful);
    class USteamCoreAppsAsyncActionGetFileDetails* GetFileDetailsAsync(class UObject* WorldContextObject, FString Filename, float Timeout);
}; // Size: 0x48

class USteamCoreAsyncAction : public UBlueprintAsyncActionBase
{
    TWeakObjectPtr<class UObject> m_WorldContextObject;                               // 0x0030 (size: 0x8)

}; // Size: 0x38

class USteamCoreAsyncActionListenForControllerChange : public USteamCoreAsyncAction
{
    FSteamCoreAsyncActionListenForControllerChangeOnControllerChanged OnControllerChanged; // 0x0038 (size: 0x10)
    void OnControllerChangedCallback(bool bIsConnected, int32 UserId);

    class USteamCoreAsyncActionListenForControllerChange* ListenForControllerChange(class UObject* WorldContextObject);
    void HandleCallback(bool bIsConnected, int32 PlatformUserId, int32 UserId);
}; // Size: 0x48

class USteamCoreCreateSession : public USteamCoreAsyncAction
{
    FSteamCoreCreateSessionOnSuccess OnSuccess;                                       // 0x0038 (size: 0x10)
    void EmptyOnlineDelegate();
    FSteamCoreCreateSessionOnFailure OnFailure;                                       // 0x0048 (size: 0x10)
    void EmptyOnlineDelegate();

    class USteamCoreCreateSession* CreateSteamCoreSession(class UObject* WorldContextObject, TMap<class FString, class FSteamSessionSetting> SessionSettings, FString SessionName, int32 MaxPlayers, bool bUseLAN, bool bAllowInvites, bool bUsesPresence, bool bAllowJoinViaPresence, bool bAllowJoinViaPresenceFriendsOnly, bool bAntiCheatProtected, bool bUsesStats, bool bShouldAdvertise, bool bUseLobbiesVoiceChatIfAvailable, float Timeout);
}; // Size: 0x158

class USteamCoreDestroySession : public USteamCoreAsyncAction
{
    FSteamCoreDestroySessionOnSuccess OnSuccess;                                      // 0x0038 (size: 0x10)
    void EmptyOnlineDelegate();
    FSteamCoreDestroySessionOnFailure OnFailure;                                      // 0x0048 (size: 0x10)
    void EmptyOnlineDelegate();

    void OnCompleted(FName SessionName, bool bWasSuccessful);
    class USteamCoreDestroySession* DestroySteamCoreSession(class UObject* WorldContextObject, float Timeout);
}; // Size: 0x78

class USteamCoreFindSession : public USteamCoreAsyncAction
{
    FSteamCoreFindSessionOnCallback OnCallback;                                       // 0x0038 (size: 0x10)
    void FindSteamSessionsResultDelegate(const TArray<FSteamSessionResult>& Results);

    void OnCompleted(bool bSuccessful);
    class USteamCoreFindSession* FindSteamCoreSessions(class UObject* WorldContextObject, TMap<class FString, class FSteamSessionSearchSetting> SearchSettings, int32 MaxResults, bool bUseLAN, ESteamSessionFindType ServerType, bool bEmptyServersOnly, bool bSecureServersOnly, float Timeout);
}; // Size: 0xD8

class USteamCoreFriendsAsyncActionDownloadClanActivityCounts : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionDownloadClanActivityCountsOnCallback OnCallback;      // 0x0038 (size: 0x10)
    void OnDownloadClanActivityCountsAsyncDelegate(const FDownloadClanActivityCountsResult& Data, bool bWasSuccessful);

    void HandleCallback(const FDownloadClanActivityCountsResult& Data, bool bWasSuccessful);
    class USteamCoreFriendsAsyncActionDownloadClanActivityCounts* DownloadClanActivityCountsAsync(class UObject* WorldContextObject, TArray<FSteamID> SteamIDClans, float Timeout);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionEnumerateFollowingList : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionEnumerateFollowingListOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void OnEnumerateFollowingListAsyncDelegate(const FFriendsEnumerateFollowingList& Data, bool bWasSuccessful);

    void HandleCallback(const FFriendsEnumerateFollowingList& Data, bool bWasSuccessful);
    class USteamCoreFriendsAsyncActionEnumerateFollowingList* EnumerateFollowingListAsync(class UObject* WorldContextObject, int32 StartIndex, float Timeout);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionGetFollowerCount : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionGetFollowerCountOnCallback OnCallback;                // 0x0038 (size: 0x10)
    void OnGetFollowerCountAsyncDelegate(const FFriendsGetFollowerCount& Data, bool bWasSuccessful);

    void HandleCallback(const FFriendsGetFollowerCount& Data, bool bWasSuccessful);
    class USteamCoreFriendsAsyncActionGetFollowerCount* GetFollowerCountAsync(class UObject* WorldContextObject, FSteamID SteamID, float Timeout);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionIsFollowing : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionIsFollowingOnCallback OnCallback;                     // 0x0038 (size: 0x10)
    void OnIsFollowingAsyncDelegate(const FFriendsIsFollowing& Data, bool bWasSuccessful);

    class USteamCoreFriendsAsyncActionIsFollowing* IsFollowingAsync(class UObject* WorldContextObject, FSteamID SteamID, float Timeout);
    void HandleCallback(const FFriendsIsFollowing& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionJoinClanChatRoom : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionJoinClanChatRoomOnCallback OnCallback;                // 0x0038 (size: 0x10)
    void OnJoinClanChatRoomAsyncDelegate(const FJoinClanChatRoomCompletionResult& Data, bool bWasSuccessful);

    class USteamCoreFriendsAsyncActionJoinClanChatRoom* JoinClanChatRoomAsync(class UObject* WorldContextObject, FSteamID SteamIDClan, float Timeout);
    void HandleCallback(const FJoinClanChatRoomCompletionResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionRequestClanOfficerList : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionRequestClanOfficerListOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void OnRequestClanOfficerListAsyncDelegate(const FClanOfficerListResponse& Data, bool bWasSuccessful);

    class USteamCoreFriendsAsyncActionRequestClanOfficerList* RequestClanOfficerListAsync(class UObject* WorldContextObject, FSteamID SteamIDClan, float Timeout);
    void HandleCallback(const FClanOfficerListResponse& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreFriendsAsyncActionRequestUserInformation : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionRequestUserInformationOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void OnRequestUserInformationAsyncDelegate();

    class USteamCoreFriendsAsyncActionRequestUserInformation* RequestUserInformationAsync(class UObject* WorldContextObject, FSteamID SteamIDUser, bool bRequireNameOnly, float Timeout);
    void HandleCallback();
}; // Size: 0x78

class USteamCoreFriendsAsyncActionSetPersonaName : public USteamCoreAsyncAction
{
    FSteamCoreFriendsAsyncActionSetPersonaNameOnCallback OnCallback;                  // 0x0038 (size: 0x10)
    void OnSetPersonaNameAsyncDelegate(const FSetPersonaNameResponse& Data, bool bWasSuccessful);

    class USteamCoreFriendsAsyncActionSetPersonaName* SetPersonaNameAsync(class UObject* WorldContextObject, FString Name, float Timeout);
    void HandleCallback(const FSetPersonaNameResponse& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreInventoryAsyncActionRequestEligiblePromoItemDefinitionsIDs : public USteamCoreAsyncAction
{
    FSteamCoreInventoryAsyncActionRequestEligiblePromoItemDefinitionsIDsOnCallback OnCallback; // 0x0038 (size: 0x10)
    void OnRequestEligiblePromoItemDefinitionsIDsAsyncDelegate(const FSteamInventoryEligiblePromoItemDefIDs& Data, bool bWasSuccessful);

    class USteamCoreInventoryAsyncActionRequestEligiblePromoItemDefinitionsIDs* RequestEligiblePromoItemDefinitionsIDsAsync(class UObject* WorldContextObject, FSteamID SteamID, float Timeout);
    void HandleCallback(const FSteamInventoryEligiblePromoItemDefIDs& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreInventoryAsyncActionRequestPricesResult : public USteamCoreAsyncAction
{
    FSteamCoreInventoryAsyncActionRequestPricesResultOnCallback OnCallback;           // 0x0038 (size: 0x10)
    void OnSteamInventoryRequestPricesResultAsyncDelegate(const FSteamInventoryRequestPricesResult& Data, bool bWasSuccessful);

    class USteamCoreInventoryAsyncActionRequestPricesResult* RequestPricesAsync(class UObject* WorldContextObject, float Timeout);
    void HandleCallback(const FSteamInventoryRequestPricesResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreInventoryAsyncActionStartPurchaseResult : public USteamCoreAsyncAction
{
    FSteamCoreInventoryAsyncActionStartPurchaseResultOnCallback OnCallback;           // 0x0038 (size: 0x10)
    void OnSteamInventoryStartPurchaseResultAsyncDelegate(const FSteamInventoryStartPurchaseResult& Data, bool bWasSuccessful);

    class USteamCoreInventoryAsyncActionStartPurchaseResult* StartPurchaseAsync(class UObject* WorldContextObject, const TArray<FSteamItemDef> ItemDefs, const TArray<int32> Quantity, float Timeout);
    void HandleCallback(const FSteamInventoryStartPurchaseResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreMatchmakingAsyncActionCreateLobby : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingAsyncActionCreateLobbyOnCallback OnCallback;                 // 0x0038 (size: 0x10)
    void OnCreateLobbyAsyncDelegate(const FCreateLobbyData& Data, bool bWasSuccessful);

    void HandleCallback(const FCreateLobbyData& Data, bool bWasSuccessful);
    class USteamCoreMatchmakingAsyncActionCreateLobby* CreateLobbyAsync(class UObject* WorldContextObject, ESteamLobbyType LobbyType, int32 MaxMembers, float Timeout);
}; // Size: 0x48

class USteamCoreMatchmakingAsyncActionJoinLobby : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingAsyncActionJoinLobbyOnCallback OnCallback;                   // 0x0038 (size: 0x10)
    void OnJoinLobbyAsyncDelegate(const FJoinLobbyData& Data, bool bWasSuccessful);

    class USteamCoreMatchmakingAsyncActionJoinLobby* JoinLobbyAsync(class UObject* WorldContextObject, FSteamID SteamIDLobby, float Timeout);
    void HandleCallback(const FJoinLobbyData& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreMatchmakingAsyncActionRequestLobbyList : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingAsyncActionRequestLobbyListOnCallback OnCallback;            // 0x0038 (size: 0x10)
    void OnRequestLobbyListAsyncDelegate(const FLobbyMatchList& Data, bool bWasSuccessful);

    class USteamCoreMatchmakingAsyncActionRequestLobbyList* RequestLobbyListAsync(class UObject* WorldContextObject, float Timeout);
    void HandleCallback(const FLobbyMatchList& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreMatchmakingServersAsyncActionPingServer : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingServersAsyncActionPingServerOnCallback OnCallback;           // 0x0038 (size: 0x10)
    void OnPingServerAsyncDelegate(const FGameServerItem& Data, bool bWasSuccessful);

    class USteamCoreMatchmakingServersAsyncActionPingServer* PingServerAsync(class UObject* WorldContextObject, FString IP, int32 Port, float Timeout);
    void HandleCallback(const FGameServerItem& Data, bool bWasSuccessful);
    void CancelPingQueries(class UObject* WorldContextObject);
}; // Size: 0x48

class USteamCoreMatchmakingServersAsyncActionRequestServerList : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingServersAsyncActionRequestServerListOnCallback OnCallback;    // 0x0038 (size: 0x10)
    void OnServerUpdatedAsyncDelegate(const FGameServerItem& Data);
    FSteamCoreMatchmakingServersAsyncActionRequestServerListOnRefreshCompleted OnRefreshCompleted; // 0x0048 (size: 0x10)
    void OnServerRefreshCompleteDelegate();

    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestSpectatorServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestLANServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestInternetServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestHistoryServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestFriendsServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    class USteamCoreMatchmakingServersAsyncActionRequestServerList* RequestFavoritesServerListAsync(class UObject* WorldContextObject, int32 AppID, float Timeout, int32 MaxResults, bool bIgnoreNonResponsive, class UServerFilter* ServerFilter);
    void HandleServerListFinished();
    void HandleCallback(const FGameServerItem& Data);
    void CancelServerListQueries(class UObject* WorldContextObject);
}; // Size: 0x58

class USteamCoreMatchmakingServersAsyncActionServerRules : public USteamCoreAsyncAction
{
    FSteamCoreMatchmakingServersAsyncActionServerRulesOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void OnServerRuleAsyncDelegate(const TArray<FGameServerRule>& Data, bool bWasSuccessful);

    class USteamCoreMatchmakingServersAsyncActionServerRules* ServerRulesAsync(class UObject* WorldContextObject, FString IP, int32 QueryPort, float Timeout);
    void HandleCallback(const TArray<FGameServerRule>& Data, bool bWasSuccessful);
    void CancelServerRulesQueries(class UObject* WorldContextObject);
}; // Size: 0x48

class USteamCoreSettings : public UDeveloperSettings
{
    bool bEnabled;                                                                    // 0x0038 (size: 0x1)
    bool bRelaunchInSteam;                                                            // 0x0039 (size: 0x1)
    bool bVACEnabled;                                                                 // 0x003A (size: 0x1)
    bool bAllowP2PPacketRelay;                                                        // 0x003B (size: 0x1)
    int32 P2PConnectionTimeout;                                                       // 0x003C (size: 0x4)
    int32 SteamAppId;                                                                 // 0x0040 (size: 0x4)
    int32 SteamDevAppId;                                                              // 0x0044 (size: 0x4)
    int32 Port;                                                                       // 0x0048 (size: 0x4)
    int32 GameServerQueryPort;                                                        // 0x004C (size: 0x4)
    FString GameVersion;                                                              // 0x0050 (size: 0x10)

}; // Size: 0x60

class USteamCoreSteamPartiesAsyncActionChangeNumOpenSlots : public USteamCoreAsyncAction
{
    FSteamCoreSteamPartiesAsyncActionChangeNumOpenSlotsOnCallback OnCallback;         // 0x0038 (size: 0x10)
    void OnChangeNumOpenSlotsCallback(const FChangeNumOpenSlotsData& Data, bool bWasSuccessful);

    void HandleCallback(const FChangeNumOpenSlotsData& Data, bool bWasSuccessful);
    class USteamCoreSteamPartiesAsyncActionChangeNumOpenSlots* ChangeNumOpenSlotsAsync(class UObject* WorldContextObject, FPartyBeaconID BeaconID, int32 OpenSlots, float Timeout);
}; // Size: 0x48

class USteamCoreSteamPartiesAsyncActionCreateBeacon : public USteamCoreAsyncAction
{
    FSteamCoreSteamPartiesAsyncActionCreateBeaconOnCallback OnCallback;               // 0x0038 (size: 0x10)
    void OnCreateBeaconCallback(const FCreateBeaconData& Data, bool bWasSuccessful);

    void HandleCallback(const FCreateBeaconData& Data, bool bWasSuccessful);
    class USteamCoreSteamPartiesAsyncActionCreateBeacon* CreateBeaconAsync(class UObject* WorldContextObject, int32 OpenSlots, FSteamPartyBeaconLocation BeaconLocation, FString ConnectString, FString MetaData, float Timeout);
}; // Size: 0x48

class USteamCoreSteamPartiesAsyncActionJoinParty : public USteamCoreAsyncAction
{
    FSteamCoreSteamPartiesAsyncActionJoinPartyOnCallback OnCallback;                  // 0x0038 (size: 0x10)
    void OnJoinPartyCallback(const FJoinPartyData& Data, bool bWasSuccessful);

    class USteamCoreSteamPartiesAsyncActionJoinParty* JoinPartyAsync(class UObject* WorldContextObject, FPartyBeaconID BeaconID, float Timeout);
    void HandleCallback(const FJoinPartyData& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreSubsystem : public UGameInstanceSubsystem
{
}; // Size: 0x68

class USteamCoreUGCAsyncActionAddAppDependency : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionAddAppDependencyOnCallback OnCallback;                    // 0x0038 (size: 0x10)
    void OnAddAppDependencyResultAsyncDelegate(const FAddAppDependencyResult& Data, bool bWasSuccessful);

    void HandleCallback(const FAddAppDependencyResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionAddAppDependency* AddAppDependencyAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, int32 AppID, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionAddItemToFavorites : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionAddItemToFavoritesOnCallback OnCallback;                  // 0x0038 (size: 0x10)
    void OnAddItemToFavoritesAsyncDelegate(const FUserFavoriteItemsListChanged& Data, bool bWasSuccessful);

    void HandleCallback(const FUserFavoriteItemsListChanged& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionAddItemToFavorites* AddItemToFavoritesAsync(class UObject* WorldContextObject, int32 AppID, FPublishedFileID PublishedFileID, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionAddUGCDependency : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionAddUGCDependencyOnCallback OnCallback;                    // 0x0038 (size: 0x10)
    void OnAddUGCDependencyResultAsyncDelegate(const FAddUGCDependencyResult& Data, bool bWasSuccessful);

    void HandleCallback(const FAddUGCDependencyResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionAddUGCDependency* AddDependencyAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, FPublishedFileID ChildPublishedFileId, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionCreateItem : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionCreateItemOnCallback OnCallback;                          // 0x0038 (size: 0x10)
    void OnCreateItemAsyncDelegate(const FCreateItemResult& Data, bool bWasSuccessful);

    void HandleCallback(const FCreateItemResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionCreateItem* CreateItemAsync(class UObject* WorldContextObject, int32 ConsumerAppID, ESteamWorkshopFileType FileType, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionDeleteItem : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionDeleteItemOnCallback OnCallback;                          // 0x0038 (size: 0x10)
    void OnDeleteItemResultAsyncDelegate(const FUGCDeleteItemResult& Data, bool bWasSuccessful);

    void HandleCallback(const FUGCDeleteItemResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionDeleteItem* DeleteItemAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionDownloadItem : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionDownloadItemOnCallback OnCallback;                        // 0x0038 (size: 0x10)
    void OnDownloadItemResultDelegate(const FDownloadItemResult& Data, bool bWasSuccessful);

    void HandleCallback(const FDownloadItemResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionDownloadItem* DownloadItemAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, bool bHighPriority, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionGetAppDependencies : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionGetAppDependenciesOnCallback OnCallback;                  // 0x0038 (size: 0x10)
    void OnGetAppDependenciesResultAsyncDelegate(const FGetAppDependenciesResult& Data, bool bWasSuccessful);

    void HandleCallback(const FGetAppDependenciesResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionGetAppDependencies* GetAppDependenciesAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionGetUserItemVote : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionGetUserItemVoteOnCallback OnCallback;                     // 0x0038 (size: 0x10)
    void OnGetUserItemVoteAsyncDelegate(const FGetUserItemVoteResult& Data, bool bWasSuccessful);

    void HandleCallback(const FGetUserItemVoteResult& Data, bool bWasSuccessful);
    class USteamCoreUGCAsyncActionGetUserItemVote* GetUserItemVoteAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, float Timeout);
}; // Size: 0x48

class USteamCoreUGCAsyncActionRemoveAppDependency : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionRemoveAppDependencyOnCallback OnCallback;                 // 0x0038 (size: 0x10)
    void OnRemoveAppDependencyResultAsyncDelegate(const FRemoveAppDependencyResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionRemoveAppDependency* RemoveAppDependencyAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, int32 AppID, float Timeout);
    void HandleCallback(const FRemoveAppDependencyResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionRemoveItemFromFavorites : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionRemoveItemFromFavoritesOnCallback OnCallback;             // 0x0038 (size: 0x10)
    void OnRemoveItemFromFavoritesAsyncDelegate(const FUserFavoriteItemsListChanged& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionRemoveItemFromFavorites* RemoveItemFromFavoritesAsync(class UObject* WorldContextObject, int32 AppID, FPublishedFileID PublishedFileID, float Timeout);
    void HandleCallback(const FUserFavoriteItemsListChanged& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionRemoveUGCDependency : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionRemoveUGCDependencyOnCallback OnCallback;                 // 0x0038 (size: 0x10)
    void OnRemoveUGCDependencyResultAsyncDelegate(const FRemoveUGCDependencyResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionRemoveUGCDependency* RemoveDependencyAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, FPublishedFileID ChildPublishedFileId, float Timeout);
    void HandleCallback(const FRemoveUGCDependencyResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionSendQueryUGCRequest : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionSendQueryUGCRequestOnCallback OnCallback;                 // 0x0038 (size: 0x10)
    void OnSendQueryUGCRequestAsyncDelegate(const FSteamUGCQueryCompleted& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionSendQueryUGCRequest* SendQueryUGCRequestAsync(class UObject* WorldContextObject, FUGCQueryHandle Handle, float Timeout);
    void HandleCallback(const FSteamUGCQueryCompleted& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionSetUserItemVote : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionSetUserItemVoteOnCallback OnCallback;                     // 0x0038 (size: 0x10)
    void OnSetUserItemVoteAsyncDelegate(const FSetUserItemVoteResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionSetUserItemVote* SetUserItemVoteAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileID, bool bVoteUp, float Timeout);
    void HandleCallback(const FSetUserItemVoteResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionStartPlaytimeTracking : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionStartPlaytimeTrackingOnCallback OnCallback;               // 0x0038 (size: 0x10)
    void OnStartPlaytimeTrackingAsyncDelegate(const FStartPlaytimeTrackingResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionStartPlaytimeTracking* StartPlaytimeTrackingAsync(class UObject* WorldContextObject, TArray<FPublishedFileID> PublishedFileIDs, float Timeout);
    void HandleCallback(const FStartPlaytimeTrackingResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionStopPlaytimeTracking : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionStopPlaytimeTrackingOnCallback OnCallback;                // 0x0038 (size: 0x10)
    void OnStopPlaytimeTrackingAsyncDelegate(const FStopPlaytimeTrackingResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionStopPlaytimeTracking* StopPlaytimeTrackingAsync(class UObject* WorldContextObject, TArray<FPublishedFileID> PublishedFileIDs, float Timeout);
    void HandleCallback(const FStopPlaytimeTrackingResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionStopPlaytimeTrackingForAllItems : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionStopPlaytimeTrackingForAllItemsOnCallback OnCallback;     // 0x0038 (size: 0x10)
    void OnStopPlaytimeTrackingForAllItemsAsyncDelegate(const FStopPlaytimeTrackingResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionStopPlaytimeTrackingForAllItems* StopPlaytimeTrackingForAllItemsAsync(class UObject* WorldContextObject, float Timeout);
    void HandleCallback(const FStopPlaytimeTrackingResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionSubmitItemUpdate : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionSubmitItemUpdateOnCallback OnCallback;                    // 0x0038 (size: 0x10)
    void OnSubmitItemUpdateAsyncDelegate(const FSubmitItemUpdateResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionSubmitItemUpdate* SubmitItemUpdateAsync(class UObject* WorldContextObject, FUGCUpdateHandle Handle, FString ChangeNote, float Timeout);
    void HandleCallback(const FSubmitItemUpdateResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionSubscribeItem : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionSubscribeItemOnCallback OnCallback;                       // 0x0038 (size: 0x10)
    void OnSubscribeItemAsyncDelegate(const FRemoteStorageSubscribePublishedFileResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionSubscribeItem* SubscribeItemAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileIDs, float Timeout);
    void HandleCallback(const FRemoteStorageSubscribePublishedFileResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUGCAsyncActionUnsubscribeItem : public USteamCoreAsyncAction
{
    FSteamCoreUGCAsyncActionUnsubscribeItemOnCallback OnCallback;                     // 0x0038 (size: 0x10)
    void OnUnsubscribeItemAsyncDelegate(const FRemoteStorageSubscribePublishedFileResult& Data, bool bWasSuccessful);

    class USteamCoreUGCAsyncActionUnsubscribeItem* UnsubscribeItemAsync(class UObject* WorldContextObject, FPublishedFileID PublishedFileIDs, float Timeout);
    void HandleCallback(const FRemoteStorageSubscribePublishedFileResult& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUpdateSession : public USteamCoreAsyncAction
{
    FSteamCoreUpdateSessionOnCallback OnCallback;                                     // 0x0038 (size: 0x10)
    void EmptyOnlineDelegate();

    class USteamCoreUpdateSession* UpdateSteamCoreSession(class UObject* WorldContextObject, TMap<class FString, class FSteamSessionSetting> Settings, FString SessionName, int32 MaxPlayers);
    void OnCompleted(FName SessionName, bool bWasSuccessful);
}; // Size: 0x68

class USteamCoreUserAsyncActionRequestEncryptedAppTicket : public USteamCoreAsyncAction
{
    FSteamCoreUserAsyncActionRequestEncryptedAppTicketOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void RequestEncryptedAppTicketDelegate(const FEncryptedAppTicketResponse& Data, bool bWasSuccessful);

    class USteamCoreUserAsyncActionRequestEncryptedAppTicket* RequestEncryptedAppTicketAsync(class UObject* WorldContextObject, TArray<uint8> DataToInclude, float Timeout);
    void HandleCallback(const FEncryptedAppTicketResponse& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUserAsyncActionRequestStoreAuthURL : public USteamCoreAsyncAction
{
    FSteamCoreUserAsyncActionRequestStoreAuthURLOnCallback OnCallback;                // 0x0038 (size: 0x10)
    void RequestStoreAuthURLDelegate(const FStoreAuthURLResponse& Data, bool bWasSuccessful);

    class USteamCoreUserAsyncActionRequestStoreAuthURL* RequestStoreAuthURLAsync(class UObject* WorldContextObject, FString RedirectURL, float Timeout);
    void HandleCallback(const FStoreAuthURLResponse& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionAttachLeaderboardUGC : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionAttachLeaderboardUGCOnCallback OnCallback;          // 0x0038 (size: 0x10)
    void OnAttachLeaderboardUGCAsyncDelegate(const FAttachLeaderboardUGCData& Data, bool bWasSuccessful);

    void HandleCallback(const FAttachLeaderboardUGCData& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionAttachLeaderboardUGC* AttachLeaderboardUGCAsync(class UObject* WorldContextObject, FSteamLeaderboard SteamLeaderboard, FSteamUGCHandle Handle, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionDownloadLeaderboardEntries : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionDownloadLeaderboardEntriesOnCallback OnCallback;    // 0x0038 (size: 0x10)
    void OnDownloadLeaderboardEntriesAsyncDelegate(const FLeaderboardScoresDownloaded& Data, bool bWasSuccessful);

    void HandleCallback(const FLeaderboardScoresDownloaded& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionDownloadLeaderboardEntries* DownloadLeaderboardEntriesAsync(class UObject* WorldContextObject, FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardDataRequest Request, int32 RangeStart, int32 RangeEnd, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionDownloadLeaderboardEntriesForUsers : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionDownloadLeaderboardEntriesForUsersOnCallback OnCallback; // 0x0038 (size: 0x10)
    void OnDownloadLeaderboardEntriesForUsersAsyncDelegate(const FLeaderboardScoresDownloadedForUsers& Data, bool bWasSuccessful);

    void HandleCallback(const FLeaderboardScoresDownloadedForUsers& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionDownloadLeaderboardEntriesForUsers* DownloadLeaderboardEntriesForUsersAsync(class UObject* WorldContextObject, FSteamLeaderboard SteamLeaderboard, TArray<FSteamID> Users, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionDownloadRequestGlobalStats : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionDownloadRequestGlobalStatsOnCallback OnCallback;    // 0x0038 (size: 0x10)
    void OnRequestGlobalStatsAsyncDelegate(const FGlobalStatsReceived& Data, bool bWasSuccessful);

    class USteamCoreUserStatsAsyncActionDownloadRequestGlobalStats* RequestGlobalStatsAsync(class UObject* WorldContextObject, int32 HistoryDays, float Timeout);
    void HandleCallback(const FGlobalStatsReceived& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionFindLeaderboard : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionFindLeaderboardOnCallback OnCallback;               // 0x0038 (size: 0x10)
    void OnFindLeaderboardAsyncDelegate(const FLeaderboardFindResult& Data, bool bWasSuccessful);

    void HandleCallback(const FLeaderboardFindResult& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionFindLeaderboard* FindLeaderboardAsync(class UObject* WorldContextObject, FString LeaderboardName, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionFindOrCreateLeaderboard : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionFindOrCreateLeaderboardOnCallback OnCallback;       // 0x0038 (size: 0x10)
    void OnFindOrCreateLeaderboardAsyncDelegate(const FFindOrCreateLeaderboardData& Data, bool bWasSuccessful);

    void HandleCallback(const FFindOrCreateLeaderboardData& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionFindOrCreateLeaderboard* FindOrCreateLeaderboardAsync(class UObject* WorldContextObject, FString LeaderboardName, ESteamLeaderboardSortMethod SortMethod, ESteamLeaderboardDisplayType DisplayType, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionGetNumberOfCurrentPlayers : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionGetNumberOfCurrentPlayersOnCallback OnCallback;     // 0x0038 (size: 0x10)
    void OnGetNumberOfCurrentPlayersAsyncDelegate(const FNumberOfCurrentPlayers& Data, bool bWasSuccessful);

    void HandleCallback(const FNumberOfCurrentPlayers& Data, bool bWasSuccessful);
    class USteamCoreUserStatsAsyncActionGetNumberOfCurrentPlayers* GetNumberOfCurrentPlayersAsync(class UObject* WorldContextObject, float Timeout);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionRequestGlobalAchievementPercentages : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionRequestGlobalAchievementPercentagesOnCallback OnCallback; // 0x0038 (size: 0x10)
    void OnRequestGlobalAchievementPercentagesAsyncDelegate(const FGlobalAchievementPercentagesReady& Data, bool bWasSuccessful);

    class USteamCoreUserStatsAsyncActionRequestGlobalAchievementPercentages* RequestGlobalAchievementPercentagesAsync(class UObject* WorldContextObject, float Timeout);
    void HandleCallback(const FGlobalAchievementPercentagesReady& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionRequestUserStats : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionRequestUserStatsOnCallback OnCallback;              // 0x0038 (size: 0x10)
    void OnRequestUserStatsAsyncDelegate(const FRequestUserStatsData& Data, bool bWasSuccessful);

    class USteamCoreUserStatsAsyncActionRequestUserStats* RequestUserStatsAsync(class UObject* WorldContextObject, FSteamID SteamID, float Timeout);
    void HandleCallback(const FRequestUserStatsData& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreUserStatsAsyncActionUploadLeaderboardScore : public USteamCoreAsyncAction
{
    FSteamCoreUserStatsAsyncActionUploadLeaderboardScoreOnCallback OnCallback;        // 0x0038 (size: 0x10)
    void OnUploadLeaderboardScoreAsyncDelegate(const FLeaderboardScoreUploaded& Data, bool bWasSuccessful);

    class USteamCoreUserStatsAsyncActionUploadLeaderboardScore* UploadLeaderboardScoreAsync(class UObject* WorldContextObject, FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardUploadScoreMethod UploadScoreMethod, int32 Score, TArray<int32> ScoreDetails, float Timeout);
    void HandleCallback(const FLeaderboardScoreUploaded& Data, bool bWasSuccessful);
}; // Size: 0x48

class USteamCoreVoice : public USoundWaveProcedural
{

    void DestroySteamCoreVoice(class USteamCoreVoice* Obj);
    class USteamCoreVoice* ConstructSteamCoreVoice(int32 AudioSampleRate);
    void AddAudioBuffer(const TArray<uint8>& Buffer);
}; // Size: 0x480

class USteamGameSearch : public USteamCoreSubsystem
{
    FSteamGameSearchSearchForGameProgressDelegate SearchForGameProgressDelegate;      // 0x0068 (size: 0x10)
    void SearchForGameProgressDelegate(const FSearchForGameProgressCallback& Data);
    FSteamGameSearchSearchForGameResultDelegate SearchForGameResultDelegate;          // 0x0078 (size: 0x10)
    void SearchForGameResultDelegate(const FSearchForGameResultCallback& Data);
    FSteamGameSearchRequestPlayersForGameProgressDelegate RequestPlayersForGameProgressDelegate; // 0x0088 (size: 0x10)
    void RequestPlayersForGameProgressDelegate(const FRequestPlayersForGameProgressCallback& Data);
    FSteamGameSearchRequestPlayersForGameResultDelegate RequestPlayersForGameResultDelegate; // 0x0098 (size: 0x10)
    void RequestPlayersForGameResultDelegate(const FRequestPlayersForGameResultCallback& Data);
    FSteamGameSearchRequestPlayersForGameFinalResultDelegate RequestPlayersForGameFinalResultDelegate; // 0x00A8 (size: 0x10)
    void RequestPlayersForGameFinalResultDelegate(const FRequestPlayersForGameFinalResultCallback& Data);
    FSteamGameSearchSubmitPlayerResultResultDelegate SubmitPlayerResultResultDelegate; // 0x00B8 (size: 0x10)
    void SubmitPlayerResultResultDelegate(const FSubmitPlayerResultResultCallback& Data);
    FSteamGameSearchEndGameResultDelegate EndGameResultDelegate;                      // 0x00C8 (size: 0x10)
    void EndGameResultDelegate(const FEndGameResultCallback& Data);

    ESteamGameSearchErrorCode SubmitPlayerResult(FString UniqueGameID, FSteamID SteamIDPlayer, ESteamPlayerResult PlayerResult);
    ESteamGameSearchErrorCode SetGameHostParams(FString Key, TArray<FString> Values);
    ESteamGameSearchErrorCode SetConnectionDetails(FString ConnectionDetails);
    ESteamGameSearchErrorCode SearchForGameWithLobby(FSteamID SteamIDLobby, int32 PlayerMin, int32 PlayerMax);
    ESteamGameSearchErrorCode SearchForGameSolo(int32 PlayerMin, int32 PlayerMax);
    ESteamGameSearchErrorCode RetrieveConnectionDetails(FSteamID SteamIDHost, FString& ConnectionDetails, int32 NumConnectionDetails);
    ESteamGameSearchErrorCode RequestPlayersForGame(int32 PlayerMin, int32 PlayerMax, int32 MaxTeamSize);
    ESteamGameSearchErrorCode HostConfirmGameStart(FString UniqueGameID);
    ESteamGameSearchErrorCode EndGameSearch();
    ESteamGameSearchErrorCode EndGame(FString UniqueGameID);
    ESteamGameSearchErrorCode DeclineGame();
    ESteamGameSearchErrorCode CancelRequestPlayersForGame();
    ESteamGameSearchErrorCode AddGameSearchParams(FString KeyToFind, TArray<FString> ValuesToFind);
    ESteamGameSearchErrorCode AcceptGame();
}; // Size: 0x1B8

class USteamGameServer : public USteamCoreSubsystem
{
    FSteamGameServerGSPolicyResponse GSPolicyResponse;                                // 0x0068 (size: 0x10)
    void OnGSPolicyResponse(const FGSPolicyResponse& Data);
    FSteamGameServerGSClientGroupStatus GSClientGroupStatus;                          // 0x0078 (size: 0x10)
    void OnGSClientGroupStatus(const FGSClientGroupStatus& Data);
    FSteamGameServerGSValidateAuthTicketResponse GSValidateAuthTicketResponse;        // 0x0088 (size: 0x10)
    void OnGSValidateAuthTicketResponse(const FValidateAuthTicketResponse& Data);
    FSteamGameServerGSClientApprove GSClientApprove;                                  // 0x0098 (size: 0x10)
    void OnGSClientApprove(const FGSClientApprove& Data);
    FSteamGameServerGSClientDeny GSClientDeny;                                        // 0x00A8 (size: 0x10)
    void OnGSClientDeny(const FGSClientDeny& Data);

    bool WasRestartRequested();
    ESteamUserHasLicenseForAppResult UserHasLicenseForApp(FSteamID SteamID, int32 AppID);
    void SetSpectatorServerName(FString SpectatorServerName);
    void SetSpectatorPort(int32 SpectatorPort);
    void SetServerName(FString ServerName);
    void SetRegion(FString Region);
    void SetProduct(FString Product);
    void SetPasswordProtected(bool bPasswordProtected);
    void SetModDir(FString ModDir);
    void SetMaxPlayerCount(int32 PlayersMax);
    void SetMapName(FString MapName);
    void SetKeyValue(FString Key, FString Value);
    void SetHeartbeatInterval(int32 HeartbeatInterval);
    void SetGameTags(FString GameTags);
    void SetGameDescription(FString GameDescription);
    void SetGameData(FString GameData);
    void SetDedicatedServer(bool bDedicated);
    void SetBotPlayerCount(int32 BotPlayers);
    void SetAdvertiseServerActive(bool bActive);
    bool RequestUserGroupStatus(FSteamID SteamIDUser, FSteamID SteamIDGroup);
    void LogOnAnonymous();
    void LogOn(FString Token);
    void LogOff();
    FSteamID GetServerSteamID_PureCompact();
    FSteamID GetServerSteamID_Pure();
    FSteamID GetServerSteamID();
    FString GetServerPublicIP_PureCompact();
    FString GetServerPublicIP_Pure();
    FString GetServerPublicIP();
    FSteamTicketHandle GetAuthSessionTicket(TArray<uint8>& Ticket);
    void ForceHeartbeat();
    void EndAuthSession(FSteamID SteamID);
    void EnableHeartbeats(bool bActive);
    FSteamID CreateUnauthenticatedUserConnection();
    void ComputeNewPlayerCompatibility(const FComputeNewPlayerCompatibilityCallback& Callback, FSteamID SteamIDNewPlayer);
    void ClearAllKeyValues();
    void CancelAuthTicket(FSteamTicketHandle TicketHandle);
    bool BUpdateUserData(FSteamID SteamIDUser, FString PlayerName, int32 Score);
    bool BSecure();
    bool BLoggedOn();
    ESteamBeginAuthSessionResult BeginAuthSession(TArray<uint8> Ticket, FSteamID SteamID);
    void AssociateWithClan(const FAssociateWithClanCallback& Callback, FSteamID SteamIDClan);
}; // Size: 0x158

class USteamParties : public USteamCoreSubsystem
{
    FSteamPartiesJoinPartyDelegate JoinPartyDelegate;                                 // 0x0068 (size: 0x10)
    void OnJoinPartyDelegate(const FJoinPartyData& Data);
    FSteamPartiesCreateBeaconDelegate CreateBeaconDelegate;                           // 0x0078 (size: 0x10)
    void OnCreateBeaconDelegate(const FCreateBeaconData& Data);
    FSteamPartiesReservationNotificationDelegate ReservationNotificationDelegate;     // 0x0088 (size: 0x10)
    void OnReservationNotificationDelegate(const FReservationNotificationData& Data);
    FSteamPartiesChangeNumOpenSlotsDelegate ChangeNumOpenSlotsDelegate;               // 0x0098 (size: 0x10)
    void OnChangeNumOpenSlotsDelegate(const FChangeNumOpenSlotsData& Data);
    FSteamPartiesAvailableBeaconLocationsDelegate AvailableBeaconLocationsDelegate;   // 0x00A8 (size: 0x10)
    void OnAvailableBeaconLocationsDelegate();
    FSteamPartiesActiveBeaconsDelegate ActiveBeaconsDelegate;                         // 0x00B8 (size: 0x10)
    void OnActiveBeaconsDelegate();

    void OnReservationCompleted(FPartyBeaconID BeaconID, FSteamID SteamIDUser);
    void JoinParty(const FJoinPartyCallback& Callback, FPartyBeaconID BeaconID);
    bool GetNumAvailableBeaconLocations(int32& NumLocations);
    int32 GetNumActiveBeacons();
    bool GetBeaconLocationData(FSteamPartyBeaconLocation BeaconLocation, ESteamPartiesBeaconLocationData EData, FString& PCHDataStringOut);
    bool GetBeaconDetails(FPartyBeaconID BeaconID, FSteamID& SteamIDBeaconOwner, FSteamPartyBeaconLocation& Location, FString& OutMetadata);
    FPartyBeaconID GetBeaconByIndex(int32 Index);
    bool GetAvailableBeaconLocations(TArray<FSteamPartyBeaconLocation>& LocationList, int32 MaxNumLocations);
    bool DestroyBeacon(FPartyBeaconID BeaconID);
    void CreateBeacon(const FCreateBeaconCallback& Callback, int32 OpenSlots, FSteamPartyBeaconLocation BeaconLocation, FString ConnectString, FString MetaData);
    void ChangeNumOpenSlots(const FChangeNumOpenSlotsCallback& Callback, FPartyBeaconID BeaconID, int32 OpenSlots);
    void CancelReservation(FPartyBeaconID BeaconID, FSteamID SteamIDUser);
}; // Size: 0x188

class USteamUtilities : public UBlueprintFunctionLibrary
{

    bool WriteBytesToFile(bool bOverwriteIfExists, FString AbsoluteFilePath, const TArray<uint8>& DataBuffer);
    void SteamItemInstanceID_Equals_Exec(FSteamItemInstanceID A, FSteamItemInstanceID B, ESteamCoreIdentical& Result);
    bool SteamItemInstanceID_Equals(FSteamItemInstanceID A, FSteamItemInstanceID B);
    TArray<uint8> ReadFileToBytes(FString AbsoluteFilePath);
    bool PublishedFileID_NotEquals(FPublishedFileID A, FPublishedFileID B);
    void PublishedFileID_Equals_Exec(FPublishedFileID A, FPublishedFileID B, ESteamCoreIdentical& Result);
    bool PublishedFileID_Equals(FPublishedFileID A, FPublishedFileID B);
    void PatchSessionResult(const FBlueprintSessionResult& SearchResult, FBlueprintSessionResult& SessionResult);
    bool NotEqual(FSteamID A, FSteamID B);
    FSteamUGCHandle MakeUGCHandle(FString Value);
    FSteamTicketHandle MakeTicketHandle(FString Value);
    FSteamSessionSetting MakeString(const FString Value);
    FSteamID MakeSteamID(FString Value);
    FSteamGameID MakeSteamGameID(FString Value);
    FSteamSessionSearchSetting MakeSearchString(const FString Value);
    FSteamSessionSearchSetting MakeSearchInteger(const ESteamComparisonOp ComparisonOperator, const int32 Value);
    FPublishedFileID MakePublishedFileID(FString Value);
    FSteamInventoryUpdateHandle MakeInventoryUpdateHandle(FString Value);
    FSteamSessionSetting MakeInteger(const int32 Value);
    void ListenForSteamMessages(const FListenForSteamMessagesCallback& Callback);
    bool K2_IsPlayerInSession(int32 LocalUserNum);
    FString K2_HexToString(TArray<uint8> Array);
    TArray<uint8> K2_HexToBytes(FString String);
    bool IsValid(FSteamID SteamID);
    bool IsUsingP2PRelays();
    void IsUGCHandleValid_Exec(const FSteamUGCHandle Handle, ESteamCoreValid& Result);
    bool IsUGCHandleValid(const FSteamUGCHandle Handle);
    void IsSteamTicketHandleValid_Exec(const FSteamTicketHandle Handle, ESteamCoreValid& Result);
    bool IsSteamTicketHandleValid(const FSteamTicketHandle Handle);
    bool IsSteamServerInitialized();
    void IsSteamInventoryUpdateHandleValid_Exec(const FSteamInventoryUpdateHandle Handle, ESteamCoreValid& Result);
    bool IsSteamInventoryUpdateHandleValid(const FSteamInventoryUpdateHandle Handle);
    void IsSteamIDValid_Exec(const FSteamID SteamID, ESteamCoreValid& Result);
    bool IsSteamAvailable();
    bool IsRecalculatingPing();
    void IsPublishedFileIDValid_Exec(const FPublishedFileID Handle, ESteamCoreValid& Result);
    bool IsPublishedFileIDValid(FPublishedFileID PublishedFileID);
    bool IsLobby(FSteamID SteamID);
    void IsGameIDValid_Exec(const FSteamGameID GameID, ESteamCoreValid& Result);
    bool IsGameIDValid(const FSteamGameID GameID);
    ESteamAttributeType GetType(FSteamSessionSetting Settings);
    bool GetString(FSteamSessionSetting Settings, FString& Key, FString& OutValue);
    FSteamID GetSteamIdFromPlayerState(class APlayerState* PlayerState);
    void GetPublicIp(const FGetPublicIpCallback& Callback);
    int32 GetPingFromHostData(const FHostPingData& Data);
    bool GetInteger(FSteamSessionSetting Settings, FString& Key, int32& OutValue);
    FHostPingData GetHostPingData();
    bool GetGameEngineInitialized();
    ESteamAccountType GetAccountType(FSteamID SteamID);
    FDateTime FromUnixTimestamp(const FString Timestamp);
    void Equal_Exec(FSteamID A, const FSteamID B, ESteamCoreIdentical& Result);
    bool Equal(FSteamID A, FSteamID B);
    FString EncryptString(FString String);
    class UServerFilter* ConstructServerFilter(class UObject* WorldContextObject);
    FString BreakUGCHandle(FSteamUGCHandle Handle);
    FString BreakTicketHandle(FSteamTicketHandle Handle);
    FString BreakSteamID(FSteamID SteamID);
    FString BreakSteamGameID(FSteamGameID SteamID);
    FString BreakPublishedFileID(FPublishedFileID FileID);
    FString BreakInventoryUpdateHandle(FSteamInventoryUpdateHandle Handle);
    TArray<uint8> BP_StringToBytes(FString String);
    FString BP_BytesToString(TArray<uint8> Array);
}; // Size: 0x28

class UUGC : public USteamCoreSubsystem
{
    FUGCItemInstalled ItemInstalled;                                                  // 0x0068 (size: 0x10)
    void OnItemInstalled(const FItemInstalled& Data);
    FUGCDownloadItemResult DownloadItemResult;                                        // 0x0078 (size: 0x10)
    void OnDownloadItemResult(const FDownloadItemResult& Data);
    FUGCUserSubscribedItemsListChanged UserSubscribedItemsListChanged;                // 0x0088 (size: 0x10)
    void OnUserSubscribedItemsListChanged(const FUserSubscribedItemsListChanged& Data);

    bool UpdateItemPreviewVideo(FUGCUpdateHandle Handle, int32 Index, FString PreviewVideo);
    bool UpdateItemPreviewFile(FUGCUpdateHandle Handle, int32 Index, FString PreviewFile);
    void UnsubscribeItem(const FUnsubscribeItemCallback& Callback, FPublishedFileID PublishedFileID);
    void SuspendDownloads(bool bSuspend);
    void SubscribeItem(const FSubscribeItemCallback& Callback, FPublishedFileID PublishedFileID);
    void SubmitItemUpdate(const FSubmitItemUpdateCallback& Callback, FUGCUpdateHandle Handle, FString ChangeNote);
    void StopPlaytimeTrackingForAllItems(const FStopPlaytimeTrackingForAllItemsCallback& Callback);
    void StopPlaytimeTracking(const FStopPlaytimeTrackingCallback& Callback, TArray<FPublishedFileID> PublishedFileIDs);
    void StartPlaytimeTracking(const FStartPlaytimeTrackingCallback& Callback, TArray<FPublishedFileID> PublishedFileID);
    FUGCUpdateHandle StartItemUpdate(int32 ConsumerAppID, FPublishedFileID PublishedFileID);
    void SetUserItemVote(const FSetUserItemVoteCallback& Callback, FPublishedFileID PublishedFileID, bool bVoteUp);
    bool SetSearchText(FUGCQueryHandle Handle, FString SearchText);
    bool SetReturnTotalOnly(FUGCQueryHandle Handle, bool bReturnTotalOnly);
    bool SetReturnPlaytimeStats(FUGCQueryHandle Handle, int32 Days);
    bool SetReturnOnlyIDs(FUGCQueryHandle Handle, bool bReturnOnlyIDs);
    bool SetReturnMetadata(FUGCQueryHandle Handle, bool bReturnMetadata);
    bool SetReturnLongDescription(FUGCQueryHandle Handle, bool bReturnLongDescription);
    bool SetReturnKeyValueTags(FUGCQueryHandle Handle, bool bReturnKeyValueTags);
    bool SetReturnChildren(FUGCQueryHandle Handle, bool bReturnChildren);
    bool SetReturnAdditionalPreviews(FUGCQueryHandle Handle, bool bReturnAdditionalPreviews);
    bool SetRankedByTrendDays(FUGCQueryHandle Handle, int32 Days);
    bool SetMatchAnyTag(FUGCQueryHandle Handle, bool bMatchAnyTag);
    bool SetLanguage(FUGCQueryHandle Handle, FString Language);
    bool SetItemVisibility(FUGCUpdateHandle Handle, ESteamRemoteStoragePublishedFileVisibility Visibility);
    bool SetItemUpdateLanguage(FUGCUpdateHandle Handle, FString Language);
    bool SetItemTitle(FUGCUpdateHandle Handle, FString Title);
    bool SetItemTags(FUGCUpdateHandle Handle, TArray<FString> Tags);
    bool SetItemPreview(FUGCUpdateHandle Handle, FString PreviewFile);
    bool SetItemMetadata(FUGCUpdateHandle Handle, FString MetaData);
    bool SetItemDescription(FUGCUpdateHandle Handle, FString Description);
    bool SetItemContent(FUGCUpdateHandle Handle, FString ContentFolder);
    bool SetCloudFileNameFilter(FUGCQueryHandle Handle, FString MatchCloudFileName);
    bool SetAllowLegacyUpload(FUGCUpdateHandle Handle, bool bAllowLegacyUpload);
    bool SetAllowCachedResponse(FUGCQueryHandle Handle, int32 MaxAgeSeconds);
    void SendQueryUGCRequest(const FSendQueryUGCRequestCallback& Callback, FUGCQueryHandle Handle);
    bool RemoveItemPreview(FUGCUpdateHandle Handle, int32 Index);
    bool RemoveItemKeyValueTags(FUGCUpdateHandle Handle, FString Key);
    void RemoveItemFromFavorites(const FRemoveItemFromFavoritesCallback& Callback, int32 AppID, FPublishedFileID PublishedFileID);
    void RemoveDependency(const FRemoveDependencyCallback& Callback, FPublishedFileID ParentPublishedFileID, FPublishedFileID ChildPublishedFileId);
    void RemoveAppDependency(const FRemoveAppDependencyCallback& Callback, FPublishedFileID PublishedFileID, int32 AppID);
    bool ReleaseQueryUGCRequest(FUGCQueryHandle Handle);
    void GetUserItemVote(const FGetUserItemVoteCallback& Callback, FPublishedFileID PublishedFileID);
    int32 GetSubscribedItems(TArray<FPublishedFileID>& PublishedFileIDs, int32 MaxEntries);
    bool GetQueryUGCTagDisplayName(FUGCQueryHandle Handle, int32 Index, int32 IndexTag, FString& Value);
    bool GetQueryUGCTag(FUGCQueryHandle Handle, int32 Index, int32 IndexTag, FString& Value);
    bool GetQueryUGCStatistic(FUGCQueryHandle Handle, int32 Index, ESteamItemStatistic StatType, FString& StatValue);
    bool GetQueryUGCResult(FUGCQueryHandle Handle, int32 Index, FSteamUGCDetails& Details);
    bool GetQueryUGCPreviewURL(FUGCQueryHandle Handle, int32 Index, FString& URL);
    int32 GetQueryUGCNumTags(FUGCQueryHandle Handle, int32 Index);
    int32 GetQueryUGCNumKeyValueTags(FUGCQueryHandle Handle, int32 Index);
    int32 GetQueryUGCNumAdditionalPreviews(FUGCQueryHandle Handle, int32 Index);
    bool GetQueryUGCMetadata(FUGCQueryHandle Handle, int32 Index, FString& MetaData, int32 MetadataSize);
    bool GetQueryUGCKeyValueTag(FUGCQueryHandle Handle, int32 Index, int32 KeyValueTagIndex, FString& Key, FString& Value);
    bool GetQueryUGCChildren(FUGCQueryHandle Handle, int32 Index, TArray<FPublishedFileID>& PublishedFileIDs, int32 MaxEntries);
    bool GetQueryUGCAdditionalPreview(FUGCQueryHandle Handle, int32 Index, int32 PreviewIndex, FString& URLOrVideoID, FString& OriginalFileName, ESteamItemPreviewType& PreviewType);
    int32 GetNumSubscribedItems();
    ESteamItemUpdateStatus GetItemUpdateProgress(FUGCUpdateHandle Handle, int32& BytesProcessed, int32& BytesTotal);
    int32 GetItemState(FPublishedFileID PublishedFileID, TArray<ESteamItemState>& States);
    bool GetItemInstallInfo(FPublishedFileID PublishedFileID, int32& SizeOnDisk, FString& Folder, int32& Timestamp);
    bool GetItemDownloadInfo(FPublishedFileID PublishedFileID, int32& BytesDownloaded, int32& BytesTotal);
    void GetAppDependencies(const FGetAppDependenciesCallback& Callback, FPublishedFileID PublishedFileID);
    bool DownloadItem(FPublishedFileID PublishedFileID, bool bHighPriority);
    void DeleteItem(const FDeleteItemCallback& Callback, FPublishedFileID PublishedFileID);
    FUGCQueryHandle CreateQueryUserUGCRequest(FSteamID SteamID, ESteamUserUGCList ListType, ESteamUGCMatchingUGCType MatchingUGCType, ESteamUserUGCListSortOrder SortOrder, int32 CreatorAppID, int32 ConsumerAppID, int32 Page);
    FUGCQueryHandle CreateQueryUGCDetailsRequest(TArray<FPublishedFileID> PublishedFileIDs);
    FUGCQueryHandle CreateQueryAllUGCRequest(ESteamUGCQuery QueryType, ESteamUGCMatchingUGCType FileType, int32 CreatorAppID, int32 ConsumerAppID, int32 Page);
    void CreateItem(const FCreateItemCallback& Callback, int32 ConsumerAppID, ESteamWorkshopFileType FileType);
    bool BInitWorkshopForGameServer(int32 WorkshopDepotID, FString Folder);
    bool AddRequiredTagGroup(FUGCQueryHandle Handle, TArray<FString> TagGroups);
    bool AddRequiredTag(FUGCQueryHandle Handle, FString TagName);
    bool AddRequiredKeyValueTag(FUGCQueryHandle Handle, FString Key, FString Value);
    void AddItemToFavorites(const FAddItemToFavoritesCallback& Callback, int32 AppID, FPublishedFileID PublishedFileID);
    bool AddItemPreviewVideo(FUGCUpdateHandle Handle, FString VideoID);
    bool AddItemPreviewFile(FUGCUpdateHandle Handle, FString PreviewFile, ESteamItemPreviewType Type);
    bool AddItemKeyValueTag(FUGCUpdateHandle Handle, FString Key, FString Value);
    bool AddExcludedTag(FUGCQueryHandle Handle, FString TagName);
    void AddDependency(const FAddDependencyCallback& Callback, FPublishedFileID PublishedFileID, FPublishedFileID ChildPublishedFileId);
    void AddAppDependency(const FAddAppDependencyCallback& Callback, FPublishedFileID PublishedFileID, int32 AppID);
}; // Size: 0xF8

class UUser : public USteamCoreSubsystem
{
    FUserClientGameServerDeny ClientGameServerDeny;                                   // 0x0068 (size: 0x10)
    void OnClientGameServerDeny(const FClientGameServerDeny& Data);
    FUserGameWebCallback GameWebCallback;                                             // 0x0078 (size: 0x10)
    void OnGameWebCallback(const FGameWebCallback& Data);
    FUserGetAuthSessionTicketResponse GetAuthSessionTicketResponse;                   // 0x0088 (size: 0x10)
    void OnGetAuthSessionTicketResponse(const FGetAuthSessionTicketResponse& Data);
    FUserIPCFailure IPCFailure;                                                       // 0x0098 (size: 0x10)
    void OnIPCFailure(const FIPCFailure& Data);
    FUserLicensesUpdated LicensesUpdated;                                             // 0x00A8 (size: 0x10)
    void OnLicensesUpdated(const FLicensesUpdated& Data);
    FUserMicroTxnAuthorizationResponse MicroTxnAuthorizationResponse;                 // 0x00B8 (size: 0x10)
    void OnMicroTxnAuthorizationResponse(const FMicroTxnAuthorizationResponse& Data);
    FUserSteamServerConnectFailure SteamServerConnectFailure;                         // 0x00C8 (size: 0x10)
    void OnSteamServerConnectFailure(const FSteamServerConnectFailure& Data);
    FUserSteamServersConnected SteamServersConnected;                                 // 0x00D8 (size: 0x10)
    void OnSteamServersConnected(const FSteamServersConnected& Data);
    FUserSteamServersDisconnected SteamServersDisconnected;                           // 0x00E8 (size: 0x10)
    void OnSteamServersDisconnected(const FSteamServersDisconnected& Data);
    FUserValidateAuthTicketResponse ValidateAuthTicketResponse;                       // 0x00F8 (size: 0x10)
    void OnValidateAuthTicketResponse(const FValidateAuthTicketResponse& Data);
    FUserEncryptedAppTicketResponse EncryptedAppTicketResponse;                       // 0x0108 (size: 0x10)
    void OnRequestEncryptedAppTicketResponse(const FEncryptedAppTicketResponse& Data);
    FUserGetTicketForWebApiResponse GetTicketForWebApiResponse;                       // 0x0118 (size: 0x10)
    void OnGetTicketForWebApiResponse(const FGetTicketForWebApiResponse& Data);

    ESteamUserHasLicenseForAppResult UserHasLicenseForApp(FSteamID SteamID, int32 AppID);
    void StopVoiceRecording();
    void StartVoiceRecording();
    void RequestStoreAuthURL(const FRequestStoreAuthURLCallback& Callback, FString RedirectURL);
    void RequestEncryptedAppTicket(const FRequestEncryptedAppTicketCallback& Callback, TArray<uint8> DataToInclude);
    int32 GetVoiceOptimalSampleRate();
    ESteamVoiceResult GetVoice(TArray<uint8>& DestBuffer, int32& BytesWritten);
    FSteamID GetSteamID_Pure();
    FSteamID GetSteamID();
    int32 GetPlayerSteamLevel();
    int32 GetGameBadgeLevel(int32 Series, bool bFoil);
    bool GetEncryptedAppTicket(TArray<uint8>& Ticket);
    ESteamVoiceResult GetAvailableVoice(int32& CompressedBytes, int32& UncompressedBytes, int32 UncompressedVoiceDesiredSampleRate);
    FSteamTicketHandle GetAuthTicketForWebApi(FString Identity);
    FSteamTicketHandle GetAuthSessionTicket(TArray<uint8>& Ticket);
    void EndAuthSession(FSteamID SteamID);
    ESteamVoiceResult DecompressVoice(const TArray<uint8>& CompressedBuffer, int32 DesiredSampleRate, TArray<uint8>& DestBuffer);
    void CancelAuthTicket(FSteamTicketHandle TicketHandle);
    bool BLoggedOn();
    bool BIsTwoFactorEnabled();
    bool BIsPhoneVerified();
    bool BIsPhoneRequiringVerification();
    bool BIsPhoneIdentifying();
    bool BIsBehindNAT();
    ESteamBeginAuthSessionResult BeginAuthSession(TArray<uint8> Ticket, FSteamID SteamID);
    void AdvertiseGame(FSteamID SteamIDGameServer, FString ServerIP, int32 ServerPort);
}; // Size: 0x288

class UUserStats : public USteamCoreSubsystem
{
    FUserStatsUserAchievementIconFetched UserAchievementIconFetched;                  // 0x0068 (size: 0x10)
    void OnUserAchievementIconFetched(const FUserAchievementIconFetched& Data);
    FUserStatsUserAchievementStored UserAchievementStored;                            // 0x0078 (size: 0x10)
    void OnUserAchievementStored(const FUserAchievementStored& Data);
    FUserStatsUserStatsReceived UserStatsReceived;                                    // 0x0088 (size: 0x10)
    void OnUserStatsReceived(const FUserStatsReceived& Data);
    FUserStatsUserStatsStored UserStatsStored;                                        // 0x0098 (size: 0x10)
    void OnUserStatsStored(const FUserStatsStored& Data);
    FUserStatsUserStatsUnloaded UserStatsUnloaded;                                    // 0x00A8 (size: 0x10)
    void OnUserStatsUnloaded(const FUserStatsUnloaded& Data);

    void UploadLeaderboardScore(const FUploadLeaderboardScoreCallback& Callback, FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardUploadScoreMethod UploadScoreMethod, int32 Score, TArray<int32> ScoreDetails);
    bool UpdateAvgRateStat(FString Name, float CountThisSession, float SessionLength);
    bool StoreStats();
    bool SetStatInt(FString Name, int32 Data);
    bool SetStatFloat(FString Name, float Data);
    bool SetAchievement(FString Name);
    bool ResetAllStats(bool bAchievementsToo);
    void RequestUserStats(const FRequestUserStatsCallback& Callback, FSteamID SteamID);
    void RequestGlobalStats(const FRequestGlobalStatsCallback& Callback, int32 HistoryDays);
    void RequestGlobalAchievementPercentages(const FRequestGlobalAchievementPercentagesCallback& Callback);
    bool RequestCurrentStats();
    bool IndicateAchievementProgress(FString Name, int32 CurrentProgress, int32 MaxProgress);
    bool GetUserStatInteger(FSteamID SteamIDUser, FString Name, int32& Data);
    bool GetUserStatFloat(FSteamID SteamIDUser, FString Name, float& Data);
    bool GetUserAchievementAndUnlockTime(FSteamID SteamIDUser, FString Name, bool& bAchieved, int32& UnlockTime);
    bool GetUserAchievement(FSteamID SteamIDUser, FString Name, bool& bAchieved);
    bool GetStatInt(FString Name, int32& Data);
    bool GetStatFloat(FString Name, float& Data);
    void GetNumberOfCurrentPlayers(const FGetNumberOfCurrentPlayersCallback& Callback);
    int32 GetNumAchievements();
    int32 GetNextMostAchievedAchievementInfo(int32 IteratorPrevious, FString& Name, float& Percent, bool& bAchieved);
    int32 GetMostAchievedAchievementInfo(FString& Name, float& Percent, bool& bAchieved);
    ESteamLeaderboardSortMethod GetLeaderboardSortMethod(FSteamLeaderboard SteamLeaderboard);
    FString GetLeaderboardName(FSteamLeaderboard SteamLeaderboard);
    int32 GetLeaderboardEntryCount(FSteamLeaderboard SteamLeaderboard);
    ESteamLeaderboardDisplayType GetLeaderboardDisplayType(FSteamLeaderboard SteamLeaderboard);
    bool GetGlobalStatInt(FString StatName, int32& Data);
    int32 GetGlobalStatHistoryInt(FString StatName, int32 HistoryDays, TArray<int32>& Data);
    int32 GetGlobalStatHistoryFloat(FString StatName, int32 HistoryDays, TArray<float>& Data);
    bool GetGlobalStatFloat(FString StatName, float& Data);
    bool GetDownloadedLeaderboardEntry(FSteamLeaderboardEntries LeaderboardEntries, int32 Index, FSteamLeaderboardEntry& LeaderboardEntry, TArray<int32> Details, TArray<int32>& outDetails);
    bool GetAchievementProgressLimitsFloat(FString Name, float& MinProgress, float& MaxProgress);
    bool GetAchievementProgressLimits(FString Name, int32& MinProgress, int32& MaxProgress);
    FString GetAchievementName(int32 Achievement);
    class UTexture2D* GetAchievementIcon(FString Name);
    FString GetAchievementDisplayAttribute(FString Name, FString Key);
    bool GetAchievementAndUnlockTime(FString Name, bool& bAchieved, int32& UnlockTime);
    bool GetAchievementAchievedPercent(FString Name, float& Percent);
    bool GetAchievement(FString Name, bool& bAchieved);
    void FindOrCreateLeaderboard(const FFindOrCreateLeaderboardCallback& Callback, FString LeaderboardName, ESteamLeaderboardSortMethod SortMethod, ESteamLeaderboardDisplayType DisplayType);
    void FindLeaderboard(const FFindLeaderboardCallback& Callback, FString LeaderboardName);
    void DownloadLeaderboardEntriesForUsers(const FDownloadLeaderboardEntriesForUsersCallback& Callback, FSteamLeaderboard SteamLeaderboard, TArray<FSteamID> Users);
    void DownloadLeaderboardEntries(const FDownloadLeaderboardEntriesCallback& Callback, FSteamLeaderboard SteamLeaderboard, ESteamLeaderboardDataRequest DataRequest, int32 RangeStart, int32 RangeEnd);
    bool ClearAchievement(FString Name);
    void AttachLeaderboardUGC(const FAttachLeaderboardUGCCallback& Callback, FSteamLeaderboard SteamLeaderboard, FSteamUGCHandle Handle);
}; // Size: 0x158

class UUtils : public USteamCoreSubsystem
{
    FUtilsCheckFileSignature CheckFileSignature;                                      // 0x0068 (size: 0x10)
    void OnCheckFileSignature(const FCheckFileSignature& Data);
    FUtilsGamepadTextInputDismissed GamepadTextInputDismissed;                        // 0x0078 (size: 0x10)
    void OnGamepadTextInputDismissed(const FGamepadTextInputDismissed& Data);
    FUtilsIPCountry IPCountry;                                                        // 0x0088 (size: 0x10)
    void OnIPCountry(const FIPCountry& Data);
    FUtilsLowBatteryPower LowBatteryPower;                                            // 0x0098 (size: 0x10)
    void OnLowBatteryPower(const FLowBatteryPower& Data);
    FUtilsSteamShutdown SteamShutdown;                                                // 0x00A8 (size: 0x10)
    void OnSteamShutdown(const FSteamShutdown& Data);

    void StartVRDashboard();
    bool ShowGamepadTextInput(ESteamGamepadTextInputMode InputMode, ESteamGamepadTextInputLineMode LineInputMode, FString Description, int32 CharMax, FString ExistingText);
    void SetVRHeadsetStreamingEnabled(bool bEnabled);
    void SetOverlayNotificationPosition(ESteamNotificationPosition NotificationPosition);
    void SetOverlayNotificationInset(int32 HorizontalInset, int32 VerticalInset);
    bool IsVRHeadsetStreamingEnabled();
    bool IsSteamRunningOnSteamDeck();
    bool IsSteamRunningInVR();
    bool IsSteamInBigPictureMode();
    bool IsSteamChinaLauncher();
    bool IsOverlayEnabled();
    bool InitFilterText();
    FString GetSteamUILanguage();
    int32 GetServerRealTime();
    int32 GetSecondsSinceComputerActive();
    int32 GetSecondsSinceAppActive();
    FString GetIPCountry();
    int32 GetIPCCallCount();
    bool GetImageSize(int32 iImage, int32& Width, int32& Height);
    bool GetImageRGBA(int32 iImage, TArray<uint8>& OutBuffer);
    int32 GetEnteredGamepadTextLength();
    bool GetEnteredGamepadTextInput(FString& Text);
    int32 GetCurrentBatteryPower();
    ESteamUniverse GetConnectedUniverse();
    int32 GetAppID_Pure();
    int32 GetAppID();
    bool BOverlayNeedsPresent();
}; // Size: 0x158

class UVideo : public USteamCoreSubsystem
{
    FVideoGetOPFSettingsResult GetOPFSettingsResult;                                  // 0x0068 (size: 0x10)
    void OnGetOPFSettingsResult(const FGetOPFSettingsResult& Data);
    FVideoGetVideoURLResult GetVideoURLResult;                                        // 0x0078 (size: 0x10)
    void OnGetVideoURLResult(const FGetVideoURLResult& Data);

    bool IsBroadcasting(int32& NumViewers);
    void GetVideoURL(int32 VideoAppID);
    bool GetOPFStringForApp(int32 VideoAppID, FString& OutBuffer);
    void GetOPFSettings(int32 VideoAppID);
}; // Size: 0xC8

#endif
