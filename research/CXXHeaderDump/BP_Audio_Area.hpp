#ifndef UE4SS_SDK_BP_Audio_Area_HPP
#define UE4SS_SDK_BP_Audio_Area_HPP

class ABP_Audio_Area_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UStaticMeshComponent* SM_VisualizeArea;                                     // 0x02A0 (size: 0x8)
    class UTextRenderComponent* T_Priority;                                           // 0x02A8 (size: 0x8)
    class UBillboardComponent* AudioBillboard;                                        // 0x02B0 (size: 0x8)
    class UBoxComponent* Trigger_Collision;                                           // 0x02B8 (size: 0x8)
    bool GameStarted;                                                                 // 0x02C0 (size: 0x1)
    class ABP_Audio_Manager_C* AudioReference;                                        // 0x02C8 (size: 0x8)
    FName AreaName;                                                                   // 0x02D0 (size: 0x8)
    int32 Priority;                                                                   // 0x02D8 (size: 0x4)
    TArray<FST_Audio> Ambient_Sounds;                                                 // 0x02E0 (size: 0x10)
    TArray<FST_Audio> ExplorationMusic;                                               // 0x02F0 (size: 0x10)
    TArray<FST_Audio> CombatMusic;                                                    // 0x0300 (size: 0x10)
    TArray<FST_Audio> RandomAmbientSoundCue;                                          // 0x0310 (size: 0x10)
    double RandomAmbientSoundMinDelay;                                                // 0x0320 (size: 0x8)
    double RandomAmbientSoundMaxDelay;                                                // 0x0328 (size: 0x8)
    TEnumAsByte<E_Switch_Method::Type> E_AudioSwitchMethod;                           // 0x0330 (size: 0x1)
    class USoundCue* Switch_Stinger_Combat;                                           // 0x0338 (size: 0x8)
    double Switch_Stinger_Combat_Delay;                                               // 0x0340 (size: 0x8)
    class USoundCue* Switch_Stinger_Exploration;                                      // 0x0348 (size: 0x8)
    double Switch_Stinger_Exploration_Delay;                                          // 0x0350 (size: 0x8)
    bool Delete_SM_VisualizeArea;                                                     // 0x0358 (size: 0x1)
    bool bOverwriteVisualizationSettings;                                             // 0x0359 (size: 0x1)
    bool bVisualizeArea;                                                              // 0x035A (size: 0x1)
    bool DelayBeforeStart;                                                            // 0x035B (size: 0x1)
    bool UseDelayedMusic;                                                             // 0x035C (size: 0x1)
    double DelayedFadeDuration;                                                       // 0x0360 (size: 0x8)
    double DelayBetweenMusicMin;                                                      // 0x0368 (size: 0x8)
    double DelayBetweenMusicMax;                                                      // 0x0370 (size: 0x8)

    void VisualizeArea(bool bNewVisibility);
    void UserConstructionScript();
    void ReceiveBeginPlay();
    void BndEvt__Box_K2Node_ComponentBoundEvent_0_ComponentBeginOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    void BndEvt__Box_K2Node_ComponentBoundEvent_1_ComponentEndOverlapSignature__DelegateSignature(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void NewAudioManager(class ABP_Audio_Manager_C* AudioReference);
    void ExecuteUbergraph_BP_Audio_Area(int32 EntryPoint);
}; // Size: 0x378

#endif
