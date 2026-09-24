#ifndef UE4SS_SDK_ImgMedia_HPP
#define UE4SS_SDK_ImgMedia_HPP

#include "ImgMedia_enums.hpp"

struct FImgMediaSourceCustomizationSequenceProxy
{
}; // Size: 0x1

struct FMediaSourceColorSettings
{
    EMediaSourceEncoding EncodingOverride;                                            // 0x0000 (size: 0x1)
    ETextureColorSpace ColorSpaceOverride;                                            // 0x0001 (size: 0x1)
    FVector2D RedChromaticityCoordinate;                                              // 0x0008 (size: 0x10)
    FVector2D GreenChromaticityCoordinate;                                            // 0x0018 (size: 0x10)
    FVector2D BlueChromaticityCoordinate;                                             // 0x0028 (size: 0x10)
    FVector2D WhiteChromaticityCoordinate;                                            // 0x0038 (size: 0x10)
    ETextureChromaticAdaptationMethod ChromaticAdaptationMethod;                      // 0x0048 (size: 0x1)

}; // Size: 0x50

class UImgMediaSource : public UBaseMediaSource
{
    bool IsPathRelativeToProjectRoot;                                                 // 0x0088 (size: 0x1)
    FFrameRate FrameRateOverride;                                                     // 0x008C (size: 0x8)
    FString ProxyOverride;                                                            // 0x0098 (size: 0x10)
    bool bFillGapsInSequence;                                                         // 0x00A8 (size: 0x1)
    FTimecode StartTimecode;                                                          // 0x00AC (size: 0x18)
    FMediaSourceColorSettings SourceColorSettings;                                    // 0x00C8 (size: 0x50)
    FDirectoryPath SequencePath;                                                      // 0x0118 (size: 0x10)

    void SetTokenizedSequencePath(FString Path);
    void SetSequencePath(FString Path);
    void RemoveTargetObject(class AActor* InActor);
    FString GetSequencePath();
    void GetProxies(TArray<FString>& OutProxies);
    void AddTargetObject(class AActor* InActor);
}; // Size: 0x148

#endif
