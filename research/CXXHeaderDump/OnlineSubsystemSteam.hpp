#ifndef UE4SS_SDK_OnlineSubsystemSteam_HPP
#define UE4SS_SDK_OnlineSubsystemSteam_HPP

class USteamAuthComponentModuleInterface : public UHandlerComponentFactory
{
}; // Size: 0x28

class USteamNetConnection : public UIpConnection
{
    bool bIsPassthrough;                                                              // 0x1E88 (size: 0x1)

}; // Size: 0x1E90

class USteamNetDriver : public UIpNetDriver
{
}; // Size: 0x8A8

#endif
