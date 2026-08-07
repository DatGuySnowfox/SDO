#ifndef UE4SS_SDK_MusicLayeringComponent_HPP
#define UE4SS_SDK_MusicLayeringComponent_HPP

class UMusicLayeringComponent_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    TArray<class USoundBase*> LayerList;                                              // 0x00A8 (size: 0x10)
    float Volume;                                                                     // 0x00B8 (size: 0x4)
    TArray<class UAudioComponent*> LayerReferences;                                   // 0x00C0 (size: 0x10)
    int32 NextTrackToPlay;                                                            // 0x00D0 (size: 0x4)
    class UAudioComponent* CurrentTrack;                                              // 0x00D8 (size: 0x8)

    void Initialise();
    void NextTrack();
    void StopTrack();
    void ExecuteUbergraph_MusicLayeringComponent(int32 EntryPoint);
}; // Size: 0xE0

#endif
