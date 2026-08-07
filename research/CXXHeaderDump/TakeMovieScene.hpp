#ifndef UE4SS_SDK_TakeMovieScene_HPP
#define UE4SS_SDK_TakeMovieScene_HPP

class UMovieSceneTakeSection : public UMovieSceneSection
{
    FMovieSceneIntegerChannel HoursCurve;                                             // 0x00F0 (size: 0x100)
    FMovieSceneIntegerChannel MinutesCurve;                                           // 0x01F0 (size: 0x100)
    FMovieSceneIntegerChannel SecondsCurve;                                           // 0x02F0 (size: 0x100)
    FMovieSceneIntegerChannel FramesCurve;                                            // 0x03F0 (size: 0x100)
    FMovieSceneFloatChannel SubFramesCurve;                                           // 0x04F0 (size: 0x110)
    FMovieSceneStringChannel Slate;                                                   // 0x0600 (size: 0x110)

}; // Size: 0x710

class UMovieSceneTakeSettings : public UObject
{
    FString HoursName;                                                                // 0x0028 (size: 0x10)
    FString MinutesName;                                                              // 0x0038 (size: 0x10)
    FString SecondsName;                                                              // 0x0048 (size: 0x10)
    FString FramesName;                                                               // 0x0058 (size: 0x10)
    FString SubFramesName;                                                            // 0x0068 (size: 0x10)
    FString SlateName;                                                                // 0x0078 (size: 0x10)

}; // Size: 0x88

class UMovieSceneTakeTrack : public UMovieSceneNameableTrack
{
    TArray<class UMovieSceneSection*> Sections;                                       // 0x0098 (size: 0x10)

}; // Size: 0xA8

#endif
