#ifndef UE4SS_SDK_DLWE_Interaction_HPP
#define UE4SS_SDK_DLWE_Interaction_HPP

class UDLWE_Interaction_C : public USceneComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x02A0 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x02A8 (size: 0x8)
    double Size;                                                                      // 0x02B0 (size: 0x8)
    bool UDW Valid;                                                                   // 0x02B8 (size: 0x1)
    class UUDS_DLWE_Interaction_Settings_C* Interaction Settings;                     // 0x02C0 (size: 0x8)
    FTimerHandle Tick Timer;                                                          // 0x02C8 (size: 0x8)
    double Current Tick Interval;                                                     // 0x02D0 (size: 0x8)
    bool Ticking;                                                                     // 0x02D8 (size: 0x1)
    double Current Average Speed;                                                     // 0x02E0 (size: 0x8)
    FVector Last Trace Location;                                                      // 0x02E8 (size: 0x18)
    double Collision Check Radius;                                                    // 0x0300 (size: 0x8)
    FVector Location of Last Material Check;                                          // 0x0308 (size: 0x18)
    double Puddle Depth;                                                              // 0x0320 (size: 0x8)
    double Snow Depth;                                                                // 0x0328 (size: 0x8)
    double Dust Depth;                                                                // 0x0330 (size: 0x8)
    FVector Last Draw Location;                                                       // 0x0338 (size: 0x18)
    FVector Last Tick Location;                                                       // 0x0350 (size: 0x18)
    double Speed Total;                                                               // 0x0368 (size: 0x8)
    TArray<double> Speed History;                                                     // 0x0370 (size: 0x10)
    int32 Speed Step;                                                                 // 0x0380 (size: 0x4)
    FVector Ground Location;                                                          // 0x0388 (size: 0x18)
    FVector Ground Normal;                                                            // 0x03A0 (size: 0x18)
    class UNiagaraComponent* Snow Trail Particles;                                    // 0x03B8 (size: 0x8)
    class UNiagaraComponent* Puddle Ripple System;                                    // 0x03C0 (size: 0x8)
    class UAudioComponent* Water Movement Sound;                                      // 0x03C8 (size: 0x8)
    class UAudioComponent* Snow Movement Sound;                                       // 0x03D0 (size: 0x8)
    FTimerHandle Sound Update Timer;                                                  // 0x03D8 (size: 0x8)
    double Current Snow Sound Volume;                                                 // 0x03E0 (size: 0x8)
    double Speed On This Tick;                                                        // 0x03E8 (size: 0x8)
    double Snow Sound Target Volume;                                                  // 0x03F0 (size: 0x8)
    double Current Water Sound Volume;                                                // 0x03F8 (size: 0x8)
    FHitResult Landscape Hit;                                                         // 0x0400 (size: 0xE8)
    bool Run General Surface Impacts;                                                 // 0x04E8 (size: 0x1)
    double Puddle Fluid Depth;                                                        // 0x04F0 (size: 0x8)
    double Water Sound Target Volume;                                                 // 0x04F8 (size: 0x8)
    bool Hit Puddle Fluid Volume;                                                     // 0x0500 (size: 0x1)
    double Snow/Dust Threshold Distance;                                              // 0x0508 (size: 0x8)
    double Puddle Threshold Distance;                                                 // 0x0510 (size: 0x8)
    bool Last Update Hit Landscape;                                                   // 0x0518 (size: 0x1)
    bool Use Non Mobile Features;                                                     // 0x0519 (size: 0x1)
    FDLWE_Interaction_CImpacted Ground Impacted Ground;                               // 0x0520 (size: 0x10)
    void Impacted Ground(FHitResult Ground Collision Hit);

    void Ripple Size(double& Size);
    void Spawn Ripple(FVector Location, FVector Normal, double Size);
    void Play Puddle Splash Sound();
    void Update Water Sound Target Volume();
    void Update Snow Sound Target Volume();
    void Query Puddle Volume(TArray<FHitResult>& Hits);
    void Clear Position History();
    void General Surface Impact();
    void Disable Sound and Particles();
    void Sound Update();
    void Create Persistent Sound and Niagara Components();
    void Puddle Impact();
    void Snow/Dust Impact();
    void Draw Surface Interactions();
    void Impact Surface();
    void Set Current Speed();
    void Trace for DLWE Surface();
    double Collision Trace Length();
    void Tick Function();
    void Distance Check();
    void Stop Running();
    void Startup Variables();
    void Deactivated Component(class UActorComponent* Component);
    void Activated Component(class UActorComponent* Component, bool bReset);
    void Get UDW();
    void ReceiveBeginPlay();
    void ExecuteUbergraph_DLWE_Interaction(int32 EntryPoint);
    void Impacted Ground__DelegateSignature(FHitResult Ground Collision Hit);
}; // Size: 0x530

#endif
