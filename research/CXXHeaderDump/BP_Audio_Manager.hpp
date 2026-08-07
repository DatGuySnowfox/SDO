#ifndef UE4SS_SDK_BP_Audio_Manager_HPP
#define UE4SS_SDK_BP_Audio_Manager_HPP

class ABP_Audio_Manager_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UAudioComponent* Ambient_2;                                                 // 0x02A0 (size: 0x8)
    class UAudioComponent* Ambient_1;                                                 // 0x02A8 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B0 (size: 0x8)
    TEnumAsByte<ETimelineDirection::Type> TimeBeforeNewSong__Direction_365E0CD44604E667EF7ED4AEBA9ED0E6; // 0x02B8 (size: 0x1)
    class UTimelineComponent* TimeBeforeNewSong;                                      // 0x02C0 (size: 0x8)
    float ExplorationTimeOfDayBlend_Blend_F8AE2C0B4ECCED7F40FEC48A11649C81;           // 0x02C8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> ExplorationTimeOfDayBlend__Direction_F8AE2C0B4ECCED7F40FEC48A11649C81; // 0x02CC (size: 0x1)
    class UTimelineComponent* ExplorationTimeOfDayBlend;                              // 0x02D0 (size: 0x8)
    float CombatMusicBlend_Blend_839A7D494AD054EEDFF65DAB7182882A;                    // 0x02D8 (size: 0x4)
    TEnumAsByte<ETimelineDirection::Type> CombatMusicBlend__Direction_839A7D494AD054EEDFF65DAB7182882A; // 0x02DC (size: 0x1)
    class UTimelineComponent* CombatMusicBlend;                                       // 0x02E0 (size: 0x8)
    int32 MaxConcurrentAmbientSounds;                                                 // 0x02E8 (size: 0x4)
    bool bRandomSoundsUseTimeOfDay;                                                   // 0x02EC (size: 0x1)
    bool PauseRandom;                                                                 // 0x02ED (size: 0x1)
    double RandomSoundMinDelay;                                                       // 0x02F0 (size: 0x8)
    double RandomSoundMaxDelay;                                                       // 0x02F8 (size: 0x8)
    TArray<class UAudioComponent*> RandomSoundsPlaying;                               // 0x0300 (size: 0x10)
    FName AreaName;                                                                   // 0x0310 (size: 0x8)
    class UObject* Player Character;                                                  // 0x0318 (size: 0x8)
    class ABP_Audio_Area_C* AudioAreaReference;                                       // 0x0320 (size: 0x8)
    TArray<class ABP_Audio_Area_C*> OverlappingAudioAreas;                            // 0x0328 (size: 0x10)
    class ABP_Audio_Area_C* MostRelevantAudioArea;                                    // 0x0338 (size: 0x8)
    FST_Audio SelectedCombat;                                                         // 0x0340 (size: 0x10)
    FST_Audio SelectedAmbient;                                                        // 0x0350 (size: 0x10)
    FST_Audio SelectedSoundtrack;                                                     // 0x0360 (size: 0x10)
    TArray<FST_Audio> AmbientSounds;                                                  // 0x0370 (size: 0x10)
    TArray<FST_Audio> RandomAmbientCue;                                               // 0x0380 (size: 0x10)
    TArray<FST_Audio> Combat_Music;                                                   // 0x0390 (size: 0x10)
    TArray<FST_Audio> SuspenseMusic;                                                  // 0x03A0 (size: 0x10)
    TArray<FST_Audio> ExplorationMusic;                                               // 0x03B0 (size: 0x10)
    bool CombatMusic;                                                                 // 0x03C0 (size: 0x1)
    bool Inside;                                                                      // 0x03C1 (size: 0x1)
    double DesiredAmbientVolume;                                                      // 0x03C8 (size: 0x8)
    double IndoorAmbientMultiplier;                                                   // 0x03D0 (size: 0x8)
    bool MusicPaused;                                                                 // 0x03D8 (size: 0x1)
    bool AmbientPaused;                                                               // 0x03D9 (size: 0x1)
    bool StartCompleted;                                                              // 0x03DA (size: 0x1)
    bool ChangingMusic;                                                               // 0x03DB (size: 0x1)
    bool ChangingAmbient;                                                             // 0x03DC (size: 0x1)
    bool CanCheckAgain;                                                               // 0x03DD (size: 0x1)
    bool Active Music component 1;                                                    // 0x03DE (size: 0x1)
    class UAudioComponent* Music_Sound_1;                                             // 0x03E0 (size: 0x8)
    class UAudioComponent* Music_Sound_2;                                             // 0x03E8 (size: 0x8)
    double AmbientCrossFadeDuration;                                                  // 0x03F0 (size: 0x8)
    TEnumAsByte<E_TimeOfDay::Type> TimeOfDay;                                         // 0x03F8 (size: 0x1)
    TEnumAsByte<E_Switch_Method::Type> Combat_AudioSwitchMethod;                      // 0x03F9 (size: 0x1)
    double Combat_SwitchDelay;                                                        // 0x0400 (size: 0x8)
    double Combat_SwitchFadeDelay;                                                    // 0x0408 (size: 0x8)
    class USoundCue* CombatSwitchStinger;                                             // 0x0410 (size: 0x8)
    TEnumAsByte<E_Switch_Method::Type> Exploration_AudioSwitchMethod;                 // 0x0418 (size: 0x1)
    double Exploration_SwitchDelay;                                                   // 0x0420 (size: 0x8)
    double Exploration_SwitchFadeDelay;                                               // 0x0428 (size: 0x8)
    class USoundCue* MusicSwitchStinger;                                              // 0x0430 (size: 0x8)
    TEnumAsByte<E_Combat::Type> CombatStatus;                                         // 0x0438 (size: 0x1)
    double CurrentTimeOfDayBlend;                                                     // 0x0440 (size: 0x8)
    double DesiredTimeOfDayBlend;                                                     // 0x0448 (size: 0x8)
    double CurrentCombatBlend;                                                        // 0x0450 (size: 0x8)
    double DesiredCombatBlend;                                                        // 0x0458 (size: 0x8)
    bool bShowAudioVisualizers;                                                       // 0x0460 (size: 0x1)
    class USoundAttenuation* RandomAmbientAttenuation;                                // 0x0468 (size: 0x8)
    double RandomSoundMultiplier;                                                     // 0x0470 (size: 0x8)
    bool Active Ambient component 1;                                                  // 0x0478 (size: 0x1)
    bool DelayBeforeStart;                                                            // 0x0479 (size: 0x1)
    bool UseDelayedMusic;                                                             // 0x047A (size: 0x1)
    double DelayedFadeDuration;                                                       // 0x0480 (size: 0x8)
    double DelayBetweenMusicMin;                                                      // 0x0488 (size: 0x8)
    double DelayBetweenMusicMax;                                                      // 0x0490 (size: 0x8)
    int32 DesiredLoopsMax;                                                            // 0x0498 (size: 0x4)
    class UAudioComponent* Music;                                                     // 0x04A0 (size: 0x8)
    bool PlayMusicOnGameStart;                                                        // 0x04A8 (size: 0x1)
    double AmbientSavedSoundVolume;                                                   // 0x04B0 (size: 0x8)
    class ABP_PlayerCharacter_C* Player;                                              // 0x04B8 (size: 0x8)

    void RandomAmbient(FST_Audio& SelectedAudio, bool& ValidResult);
    class UAudioComponent* GetMusicComponent();
    class UAudioComponent* GetAmbientComponent();
    void RandomAmbientTrace(FVector& Location, bool& Condition);
    void CheckRandomSounds(bool& FadeOutSounds);
    void ShowAllAreaVisualizers();
    void HideAllAreaVisualizers();
    void SetFloatParametersCombat(double Combat);
    void SetAmbientVolume(double NewVolumeMultiplier);
    void SetFloatParametersTimeOfDay(double TimeOfDay);
    int32 GetRandomAudioInteger(const TArray<FST_Audio>& TargetArray);
    void SetAudioVariables(class ABP_Audio_Area_C* AudioAreaReference);
    void Random Combat or exploration(FST_Audio& SelectedAudio, bool& Valid);
    void UserConstructionScript();
    void TimeBeforeNewSong__FinishedFunc();
    void TimeBeforeNewSong__UpdateFunc();
    void TimeBeforeNewSong__Continue__EventFunc();
    void ExplorationTimeOfDayBlend__FinishedFunc();
    void ExplorationTimeOfDayBlend__UpdateFunc();
    void CombatMusicBlend__FinishedFunc();
    void CombatMusicBlend__UpdateFunc();
    void CloseDelayedMusicGate();
    void PlayDelayedMusicInstant();
    void PlayDelayedMusic(bool ContinueTimeline_Delay);
    void ReceiveBeginPlay();
    void PlayRandomSound();
    void FadeOutRandomAmbient();
    void StartCombatMusic();
    void CombatMusicCheck();
    void ChangeAudio();
    void RemoveFromArray(class ABP_Audio_Area_C* AreaToRemove);
    void AddToAudioArray(const class ABP_Audio_Area_C*& NewArea);
    void UpdateTimeOfDay(double Time, TEnumAsByte<E_TimeOfDay::Type> Night?);
    void Set Time Of Day New Sound();
    void SetInside(bool bInside, double AmbientMultiplier, FReverbSettings ReverbSettings, FName ReverbAreaName, double ReverbPriority);
    void Pause Music();
    void Continue Music();
    void UpdateCombatStatus(TEnumAsByte<E_Combat::Type> CombatStatus);
    void CheckForCombatMusic();
    void Set Combat or tension sound();
    void Pause Ambient();
    void Unpause Ambient();
    void ReceiveDestroyed();
    void ExecuteUbergraph_BP_Audio_Manager(int32 EntryPoint);
}; // Size: 0x4C0

#endif
