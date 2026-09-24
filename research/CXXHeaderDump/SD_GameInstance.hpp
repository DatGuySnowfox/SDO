#ifndef UE4SS_SDK_SD_GameInstance_HPP
#define UE4SS_SDK_SD_GameInstance_HPP

class USD_GameInstance_C : public UGameInstance
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x01C0 (size: 0x8)
    FSD_GameInstance_CSmoothLoadLevel SmoothLoadLevel;                                // 0x01C8 (size: 0x10)
    void SmoothLoadLevel(FName LevelName);
    bool NewGame?;                                                                    // 0x01D8 (size: 0x1)
    FSD_GameInstance_CPlayerRespawned PlayerRespawned;                                // 0x01E0 (size: 0x10)
    void PlayerRespawned();
    bool InSafeZone?;                                                                 // 0x01F0 (size: 0x1)
    TEnumAsByte<Enum_Difficulty::Type> Difficulty;                                    // 0x01F1 (size: 0x1)
    bool GameStarted?;                                                                // 0x01F2 (size: 0x1)
    FName CurrentLevel;                                                               // 0x01F4 (size: 0x8)
    FSD_GameInstance_CPlayerSpawnedInLevel PlayerSpawnedInLevel;                      // 0x0200 (size: 0x10)
    void PlayerSpawnedInLevel(bool Save?);
    bool AllowSaving?;                                                                // 0x0210 (size: 0x1)

    void Tutorial_SpawnPlayer();
    void Debug_SpawnCamera();
    void Debug_SpawnPlayer(int32 Index, bool SpawnStartingItems?);
    void GameMode_SpawnPlayer();
    void Survival_SuicideRespawn();
    void Survival_Respawn(bool Random?);
    void Survival_SpawnPlayer();
    void OnCompleted_094C167A44DBAC5826AB0A94C7C0DD1E();
    void ReceiveInit();
    void FixWeather();
    void ChangedUpscalingMethod(int32 NewValue);
    void ChangedUpscalingQuality(int32 NewValue);
    void ChangedAMDFrameGen(bool NewValue);
    void CustomEvent(float NewValue);
    void ChangedColorBlindMode(int32 NewValue);
    void SharpnessCallback(float NewValue);
    void GammaCallback(float NewValue);
    void GamepadIconsChanged(FString NewValue);
    void VehicleVolumeChanged(float NewValue);
    void RadioMusicVolumeChanged(float NewValue);
    void UIVolumeChanged(float NewValue);
    void EffectsVolumeChanged(float NewValue);
    void MusicVolumeChanged(float NewValue);
    void AmbientVolumeChanged(float NewValue);
    void MasterVolumeChanged(float NewValue);
    void Event_ResetAIOSpawner();
    void Event_LoadLevel(FName Level);
    void Event_Fog(bool NewValue);
    void Event_VolFog();
    void ExecuteUbergraph_SD_GameInstance(int32 EntryPoint);
    void PlayerSpawnedInLevel__DelegateSignature(bool Save?);
    void PlayerRespawned__DelegateSignature();
    void SmoothLoadLevel__DelegateSignature(FName LevelName);
}; // Size: 0x211

#endif
