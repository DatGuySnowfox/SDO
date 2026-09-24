#ifndef UE4SS_SDK_MediaPlate_HPP
#define UE4SS_SDK_MediaPlate_HPP

#include "MediaPlate_enums.hpp"

struct FMediaPlateResource
{
    EMediaPlateResourceType Type;                                                     // 0x0000 (size: 0x1)
    FString ExternalMediaPath;                                                        // 0x0008 (size: 0x10)
    TSoftObjectPtr<class UMediaSource> MediaAsset;                                    // 0x0018 (size: 0x28)
    TSoftObjectPtr<class UMediaPlaylist> SourcePlaylist;                              // 0x0040 (size: 0x28)

}; // Size: 0x68

struct FMediaTextureResourceSettings
{
    bool bEnableGenMips;                                                              // 0x0000 (size: 0x1)

}; // Size: 0x1

class AMediaPlate : public AActor
{
    class UMediaPlateComponent* MediaPlateComponent;                                  // 0x02A8 (size: 0x8)
    class UStaticMeshComponent* StaticMeshComponent;                                  // 0x02B0 (size: 0x8)
    bool bEnableHoldoutComposite;                                                     // 0x02B8 (size: 0x1)

    void SetHoldoutCompositeEnabled(bool bInEnabled);
    bool IsHoldoutCompositeEnabled();
}; // Size: 0x2C0

class UMediaPlateAssetUserData : public UAssetUserData
{
}; // Size: 0x38

class UMediaPlateComponent : public UActorComponent
{
    bool bPlayOnOpen;                                                                 // 0x00C0 (size: 0x1)
    bool bAutoPlay;                                                                   // 0x00C1 (size: 0x1)
    bool bEnableAudio;                                                                // 0x00C2 (size: 0x1)
    float StartTime;                                                                  // 0x00C4 (size: 0x4)
    class UMediaSoundComponent* SoundComponent;                                       // 0x00C8 (size: 0x8)
    class UStaticMeshComponent* StaticMeshComponent;                                  // 0x00D0 (size: 0x8)
    TArray<UStaticMeshComponent*> Letterboxes;                                        // 0x00D8 (size: 0x10)
    FMediaPlateResource MediaPlateResource;                                           // 0x00E8 (size: 0x68)
    int32 PlaylistIndex;                                                              // 0x0150 (size: 0x4)
    FMediaSourceCacheSettings CacheSettings;                                          // 0x0154 (size: 0x8)
    bool bIsMediaPlatePlaying;                                                        // 0x015C (size: 0x1)
    bool bPlayOnlyWhenVisible;                                                        // 0x0170 (size: 0x1)
    bool bLoop;                                                                       // 0x0171 (size: 0x1)
    EMediaTextureVisibleMipsTiles VisibleMipsTilesCalculations;                       // 0x0172 (size: 0x1)
    float MipMapBias;                                                                 // 0x0174 (size: 0x4)
    bool bIsAspectRatioAuto;                                                          // 0x0178 (size: 0x1)
    bool bEnableMipMapUpscaling;                                                      // 0x0179 (size: 0x1)
    int32 MipLevelToUpscale;                                                          // 0x017C (size: 0x4)
    bool bAdaptivePoleMipUpscaling;                                                   // 0x0180 (size: 0x1)
    float LetterboxAspectRatio;                                                       // 0x0184 (size: 0x4)
    FVector2D MeshRange;                                                              // 0x0190 (size: 0x10)
    TArray<UMediaTexture*> MediaTextures;                                             // 0x01A0 (size: 0x10)
    FMediaTextureResourceSettings MediaTextureSettings;                               // 0x01B0 (size: 0x1)
    class UMediaPlayer* MediaPlayer;                                                  // 0x01B8 (size: 0x8)
    class UMediaSource* ExternalMediaSource;                                          // 0x01C0 (size: 0x8)
    class UMediaPlaylist* ActivePlaylist;                                             // 0x01C8 (size: 0x8)

    void SetPlayOnlyWhenVisible(bool bInPlayOnlyWhenVisible);
    void SetMeshRange(FVector2D InMeshRange);
    void SetLoop(bool bInLoop);
    void SetLetterboxAspectRatio(float AspectRatio);
    void SetIsAspectRatioAuto(bool bInIsAspectRatioAuto);
    void SetEnableAudio(bool bInEnableAudio);
    void SelectMediaSourceAsset(const class UMediaSource* InMediaSource);
    void SelectMediaPlaylistAsset(const class UMediaPlaylist* InMediaPlaylist);
    void SelectExternalMedia(FString InFilePath);
    bool Seek(const FTimespan& Time);
    bool Rewind();
    bool Previous();
    void Play();
    void Pause();
    void OpenLatent(const class UObject* InWorldContextObject, FLatentActionInfo InLatentInfo, float InTimeout, bool bInWaitForTexture, bool& bOutSuccess);
    void Open();
    void OnMediaSuspended();
    void OnMediaResumed();
    void OnMediaOpened(FString DeviceUrl);
    void OnMediaEnd();
    bool Next();
    bool IsMediaPlatePlaying();
    bool IsEventStateChangeAllowed(EMediaPlateEventState InRequestEventState);
    FVector2D GetMeshRange();
    class UMediaTexture* GetMediaTexture(int32 Index);
    class UMediaPlayer* GetMediaPlayer();
    bool GetLoop();
    float GetLetterboxAspectRatio();
    bool GetIsAspectRatioAuto();
    void Close();
}; // Size: 0x260

#endif
