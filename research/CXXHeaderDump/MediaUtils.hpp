#ifndef UE4SS_SDK_MediaUtils_HPP
#define UE4SS_SDK_MediaUtils_HPP

#include "MediaUtils_enums.hpp"

struct FMediaPlayerInitialTrackLanguageSelection
{
    FString Video;                                                                    // 0x0000 (size: 0x10)
    FString Audio;                                                                    // 0x0010 (size: 0x10)
    FString Subtitle;                                                                 // 0x0020 (size: 0x10)
    FString Caption;                                                                  // 0x0030 (size: 0x10)

}; // Size: 0x40

struct FMediaPlayerOptions
{
    FMediaPlayerTrackOptions Tracks;                                                  // 0x0000 (size: 0x1C)
    FMediaPlayerInitialTrackLanguageSelection TracksByLanguage;                       // 0x0020 (size: 0x40)
    EMediaPlayerOptionTrackSelectMode TrackSelection;                                 // 0x0060 (size: 0x1)
    FTimespan SeekTime;                                                               // 0x0068 (size: 0x8)
    EMediaPlayerOptionSeekTimeType SeekTimeType;                                      // 0x0070 (size: 0x1)
    EMediaPlayerOptionBooleanOverride PlayOnOpen;                                     // 0x0071 (size: 0x1)
    EMediaPlayerOptionBooleanOverride Loop;                                           // 0x0072 (size: 0x1)

}; // Size: 0xC8

struct FMediaPlayerTrackOptions
{
    int32 Audio;                                                                      // 0x0000 (size: 0x4)
    int32 Caption;                                                                    // 0x0004 (size: 0x4)
    int32 MetaData;                                                                   // 0x0008 (size: 0x4)
    int32 Script;                                                                     // 0x000C (size: 0x4)
    int32 Subtitle;                                                                   // 0x0010 (size: 0x4)
    int32 Text;                                                                       // 0x0014 (size: 0x4)
    int32 Video;                                                                      // 0x0018 (size: 0x4)

}; // Size: 0x1C

#endif
