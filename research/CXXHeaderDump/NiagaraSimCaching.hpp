#ifndef UE4SS_SDK_NiagaraSimCaching_HPP
#define UE4SS_SDK_NiagaraSimCaching_HPP

#include "NiagaraSimCaching_enums.hpp"

struct FMovieSceneNiagaraCacheParams : public FMovieSceneBaseCacheParams
{
    FNiagaraSimCacheCreateParameters CacheParameters;                                 // 0x0020 (size: 0x58)
    class UNiagaraSimCache* SimCache;                                                 // 0x0078 (size: 0x8)
    bool bLockCacheToReadOnly;                                                        // 0x0080 (size: 0x1)
    bool bOverrideQualityLevel;                                                       // 0x0081 (size: 0x1)
    EPerQualityLevels RecordQualityLevel;                                             // 0x0082 (size: 0x1)
    ENiagaraSimCacheSectionPlayMode CacheReplayPlayMode;                              // 0x0083 (size: 0x1)
    ENiagaraSimCacheSectionStretchMode SectionStretchMode;                            // 0x0084 (size: 0x1)

}; // Size: 0x88

struct FMovieSceneNiagaraCacheSectionTemplate : public FMovieSceneTrackImplementation
{
    TArray<FMovieSceneNiagaraSectionTemplateParameter> CacheSections;                 // 0x0010 (size: 0x10)

}; // Size: 0x20

struct FMovieSceneNiagaraSectionTemplateParameter
{
    FMovieSceneFrameRange SectionRange;                                               // 0x0000 (size: 0x10)
    FMovieSceneNiagaraCacheParams Params;                                             // 0x0010 (size: 0x88)

}; // Size: 0x98

class UMovieSceneNiagaraCacheSection : public UMovieSceneBaseCacheSection
{
    FMovieSceneNiagaraCacheParams Params;                                             // 0x0110 (size: 0x88)
    bool bCacheOutOfDate;                                                             // 0x0198 (size: 0x1)

}; // Size: 0x1A0

class UMovieSceneNiagaraCacheTrack : public UMovieSceneNameableTrack
{
    bool bIsRecording;                                                                // 0x0120 (size: 0x1)
    TArray<UMovieSceneSection*> AnimationSections;                                    // 0x0128 (size: 0x10)
    bool bCacheRecordingEnabled;                                                      // 0x0138 (size: 0x1)

}; // Size: 0x140

#endif
