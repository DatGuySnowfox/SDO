#ifndef UE4SS_SDK_DiscordRpc_HPP
#define UE4SS_SDK_DiscordRpc_HPP

#include "DiscordRpc_enums.hpp"

struct FDiscordRichPresence
{
    FString State;                                                                    // 0x0000 (size: 0x10)
    FString Details;                                                                  // 0x0010 (size: 0x10)
    int64 startTimestamp;                                                             // 0x0020 (size: 0x8)
    int64 endTimestamp;                                                               // 0x0028 (size: 0x8)
    FString largeImageKey;                                                            // 0x0030 (size: 0x10)
    FString largeImageText;                                                           // 0x0040 (size: 0x10)
    FString smallImageKey;                                                            // 0x0050 (size: 0x10)
    FString smallImageText;                                                           // 0x0060 (size: 0x10)
    FString partyId;                                                                  // 0x0070 (size: 0x10)
    int32 partySize;                                                                  // 0x0080 (size: 0x4)
    int32 partyMax;                                                                   // 0x0084 (size: 0x4)
    FString matchSecret;                                                              // 0x0088 (size: 0x10)
    FString joinSecret;                                                               // 0x0098 (size: 0x10)
    FString spectateSecret;                                                           // 0x00A8 (size: 0x10)
    bool Instance;                                                                    // 0x00B8 (size: 0x1)

}; // Size: 0xC0

struct FDiscordUserData
{
    FString UserId;                                                                   // 0x0000 (size: 0x10)
    FString username;                                                                 // 0x0010 (size: 0x10)
    FString discriminator;                                                            // 0x0020 (size: 0x10)
    FString avatar;                                                                   // 0x0030 (size: 0x10)

}; // Size: 0x40

class UDiscordRpc : public UObject
{
    bool IsConnected;                                                                 // 0x0028 (size: 0x1)
    FDiscordRpcOnConnected OnConnected;                                               // 0x0030 (size: 0x10)
    void DiscordConnected(const FDiscordUserData& joinRequest);
    FDiscordRpcOnDisconnected OnDisconnected;                                         // 0x0040 (size: 0x10)
    void DiscordDisconnected(int32 ErrorCode, FString errorMessage);
    FDiscordRpcOnErrored OnErrored;                                                   // 0x0050 (size: 0x10)
    void DiscordErrored(int32 ErrorCode, FString errorMessage);
    FDiscordRpcOnJoin OnJoin;                                                         // 0x0060 (size: 0x10)
    void DiscordJoin(FString joinSecret);
    FDiscordRpcOnSpectate OnSpectate;                                                 // 0x0070 (size: 0x10)
    void DiscordSpectate(FString spectateSecret);
    FDiscordRpcOnJoinRequest OnJoinRequest;                                           // 0x0080 (size: 0x10)
    void DiscordJoinRequest(const FDiscordUserData& joinRequest);
    FDiscordRichPresence RichPresence;                                                // 0x0090 (size: 0xC0)

    void UpdatePresence();
    void Shutdown();
    void RunCallbacks();
    void Respond(FString UserId, int32 Reply);
    void Initialize(FString applicationId, bool autoRegister, FString optionalSteamId);
    int64 GetStartTimeStamp();
    void ClearPresence();
}; // Size: 0x150

#endif
