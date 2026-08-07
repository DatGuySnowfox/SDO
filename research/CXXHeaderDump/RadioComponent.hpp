#ifndef UE4SS_SDK_RadioComponent_HPP
#define UE4SS_SDK_RadioComponent_HPP

class URadioComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class UMediaPlayer* MediaPlayer;                                                  // 0x00A8 (size: 0x8)
    class UMediaSoundComponent* MediaSound;                                           // 0x00B0 (size: 0x8)
    TArray<FString> Found Files;                                                      // 0x00B8 (size: 0x10)
    FString CurrentSong;                                                              // 0x00C8 (size: 0x10)
    bool TurnedOn?;                                                                   // 0x00D8 (size: 0x1)
    FString PreviousSong;                                                             // 0x00E0 (size: 0x10)
    double PlayRate;                                                                  // 0x00F0 (size: 0x8)
    bool Paused?;                                                                     // 0x00F8 (size: 0x1)
    FRadioComponent_CTurnedOn TurnedOn;                                               // 0x0100 (size: 0x10)
    void TurnedOn();
    FRadioComponent_CTurnedOff TurnedOff;                                             // 0x0110 (size: 0x10)
    void TurnedOff();
    class URadioUI_C* RadioUI;                                                        // 0x0120 (size: 0x8)
    FString Object Name;                                                              // 0x0128 (size: 0x10)

    void ToggleRadioUI(bool ForceRemove?);
    void ReceiveBeginPlay();
    void StartRadio();
    void StopRadio();
    void EndReached_Start();
    void NextSongRadio();
    void PreviousSongRadio();
    void EndReached_Previous();
    void ExecuteUbergraph_RadioComponent(int32 EntryPoint);
    void TurnedOff__DelegateSignature();
    void TurnedOn__DelegateSignature();
}; // Size: 0x138

#endif
