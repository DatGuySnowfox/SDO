#ifndef UE4SS_SDK_Actor_Weather_Status_HPP
#define UE4SS_SDK_Actor_Weather_Status_HPP

class UActor_Weather_Status_C : public UActorComponent
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x00A0 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x00A8 (size: 0x8)
    double Temperature (C);                                                           // 0x00B0 (size: 0x8)
    double Temperature (F);                                                           // 0x00B8 (size: 0x8)
    TEnumAsByte<UDS_ActorWeatherBoundsMode::Type> Exposure Test Bounds;               // 0x00C0 (size: 0x1)
    double Temperature Change Speed;                                                  // 0x00C8 (size: 0x8)
    double Inverted Occlusion;                                                        // 0x00D0 (size: 0x8)
    double Status Query Period;                                                       // 0x00D8 (size: 0x8)
    double Update Values Period;                                                      // 0x00E0 (size: 0x8)
    class UUDS_OcclusionSettings_C* Occlusion Settings;                               // 0x00E8 (size: 0x8)
    double Max Wet Value;                                                             // 0x00F0 (size: 0x8)
    double Wet;                                                                       // 0x00F8 (size: 0x8)
    double Snowy;                                                                     // 0x0100 (size: 0x8)
    double Dusty;                                                                     // 0x0108 (size: 0x8)
    double Current Rain Exposure;                                                     // 0x0110 (size: 0x8)
    double Current Snow Exposure;                                                     // 0x0118 (size: 0x8)
    double Current Wind Exposure;                                                     // 0x0120 (size: 0x8)
    double Current Dust Exposure;                                                     // 0x0128 (size: 0x8)
    double Target Temp (F);                                                           // 0x0130 (size: 0x8)
    bool First Tick;                                                                  // 0x0138 (size: 0x1)
    TArray<class UMaterialInstanceDynamic*> Dynamic Material Instances;               // 0x0140 (size: 0x10)
    double Max Snowy Value;                                                           // 0x0150 (size: 0x8)
    double Max Dusty Value;                                                           // 0x0158 (size: 0x8)
    double Wet Increase Speed;                                                        // 0x0160 (size: 0x8)
    double Wet Decrease Speed;                                                        // 0x0168 (size: 0x8)
    double Snowy Increase Speed;                                                      // 0x0170 (size: 0x8)
    double Snowy Decrease Speed;                                                      // 0x0178 (size: 0x8)
    double Dusty Increase Speed;                                                      // 0x0180 (size: 0x8)
    double Dusty Decrease Speed;                                                      // 0x0188 (size: 0x8)
    double Wind;                                                                      // 0x0190 (size: 0x8)
    double Hit by Rain;                                                               // 0x0198 (size: 0x8)
    double Hit by Snow;                                                               // 0x01A0 (size: 0x8)
    double Hit by Dust;                                                               // 0x01A8 (size: 0x8)
    FActor_Weather_Status_CExposed to Rain Exposed to Rain;                           // 0x01B0 (size: 0x10)
    void Exposed to Rain();
    FActor_Weather_Status_CNot Exposed to Rain Not Exposed to Rain;                   // 0x01C0 (size: 0x10)
    void Not Exposed to Rain();
    FActor_Weather_Status_CExposed to Snow Exposed to Snow;                           // 0x01D0 (size: 0x10)
    void Exposed to Snow();
    FActor_Weather_Status_CNot Exposed to Snow Not Exposed to Snow;                   // 0x01E0 (size: 0x10)
    void Not Exposed to Snow();
    FActor_Weather_Status_CExposed to Dust Exposed to Dust;                           // 0x01F0 (size: 0x10)
    void Exposed to Dust();
    FActor_Weather_Status_CNot Exposed to Dust Not Exposed to Dust;                   // 0x0200 (size: 0x10)
    void Not Exposed to Dust();
    FActor_Weather_Status_CTemperature Cold Temperature Cold;                         // 0x0210 (size: 0x10)
    void Temperature Cold();
    FActor_Weather_Status_CTemperature Neutral Temperature Neutral;                   // 0x0220 (size: 0x10)
    void Temperature Neutral();
    FActor_Weather_Status_CTemperature Hot Temperature Hot;                           // 0x0230 (size: 0x10)
    void Temperature Hot();
    bool ED_Rain Exposed;                                                             // 0x0240 (size: 0x1)
    bool ED_Snow Exposed;                                                             // 0x0241 (size: 0x1)
    bool ED_Dust Exposed;                                                             // 0x0242 (size: 0x1)
    bool ED_Wind Exposed;                                                             // 0x0243 (size: 0x1)
    FActor_Weather_Status_CExposed to Wind Exposed to Wind;                           // 0x0248 (size: 0x10)
    void Exposed to Wind();
    FActor_Weather_Status_CNot Exposed to Wind Not Exposed to Wind;                   // 0x0258 (size: 0x10)
    void Not Exposed to Wind();
    int32 ED_Temperature State;                                                       // 0x0268 (size: 0x4)
    double Rain Event Dispatcher Threshold;                                           // 0x0270 (size: 0x8)
    double Snow Event Dispatcher Threshold;                                           // 0x0278 (size: 0x8)
    double Dust Event Dispatcher Threshold;                                           // 0x0280 (size: 0x8)
    double Wind Event Dispatcher Threshold;                                           // 0x0288 (size: 0x8)
    double Cold Temperature Threshold;                                                // 0x0290 (size: 0x8)
    double Hot Temperature Threshold;                                                 // 0x0298 (size: 0x8)
    class UUDS_Weather_Settings_C* Weather State;                                     // 0x02A0 (size: 0x8)
    class UUDS_PlayerOcclusion_C* Player Occlusion;                                   // 0x02A8 (size: 0x8)
    TArray<class UNiagaraComponent*> Drip Systems;                                    // 0x02B0 (size: 0x10)
    bool Controlling Drip Systems;                                                    // 0x02C0 (size: 0x1)
    double Local Temperature Offset (F);                                              // 0x02C8 (size: 0x8)

    void Check for Drip Particles();
    void Clear Timers();
    void UDW End Play(class AActor* Actor, TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Check for UDW();
    void Start Occlusion();
    void Trigger Event Dispatchers();
    void Increment Surface State(double Exposure, double Increase Speed, double Decrease Speed, double Max Value, double Current Value, double Delta, double& Out);
    void Update Current Status();
    void Query Target Status();
    void Activated(class UActorComponent* Component, bool bReset);
    void Deactivated(class UActorComponent* Component);
    void ReceiveTick(float DeltaSeconds);
    void ReceiveBeginPlay();
    void ExecuteUbergraph_Actor_Weather_Status(int32 EntryPoint);
    void Not Exposed to Wind__DelegateSignature();
    void Exposed to Wind__DelegateSignature();
    void Temperature Hot__DelegateSignature();
    void Temperature Neutral__DelegateSignature();
    void Temperature Cold__DelegateSignature();
    void Not Exposed to Dust__DelegateSignature();
    void Exposed to Dust__DelegateSignature();
    void Not Exposed to Snow__DelegateSignature();
    void Exposed to Snow__DelegateSignature();
    void Not Exposed to Rain__DelegateSignature();
    void Exposed to Rain__DelegateSignature();
}; // Size: 0x2D0

#endif
