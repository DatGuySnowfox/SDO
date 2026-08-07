#ifndef UE4SS_SDK_Weather_Override_Volume_HPP
#define UE4SS_SDK_Weather_Override_Volume_HPP

class AWeather_Override_Volume_C : public AActor
{
    FPointerToUberGraphFrame UberGraphFrame;                                          // 0x0298 (size: 0x8)
    class UUDW_Material_State_Manager_C* Material State Manager;                      // 0x02A0 (size: 0x8)
    class UUDW_Temperature_Manager_C* Temperature Manager;                            // 0x02A8 (size: 0x8)
    class URandom_Weather_Variation_C* Random Weather Manager;                        // 0x02B0 (size: 0x8)
    class UBillboardComponent* Billboard;                                             // 0x02B8 (size: 0x8)
    class USplineComponent* Spline;                                                   // 0x02C0 (size: 0x8)
    class UUDS_Weather_Settings_C* Weather;                                           // 0x02C8 (size: 0x8)
    double Transition Width;                                                          // 0x02D0 (size: 0x8)
    int32 Priority;                                                                   // 0x02D8 (size: 0x4)
    bool Override Material Effects;                                                   // 0x02DC (size: 0x1)
    double Volume Alpha;                                                              // 0x02E0 (size: 0x8)
    double Scaled Transition Width;                                                   // 0x02E8 (size: 0x8)
    class AUltra_Dynamic_Weather_C* UDW;                                              // 0x02F0 (size: 0x8)
    TEnumAsByte<UDS_RandomWeatherTiming::Type> Random Weather Variation;              // 0x02F8 (size: 0x1)
    class UUDS_Climate_Preset_C* Apply Climate Preset;                                // 0x0300 (size: 0x8)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Spring);       // 0x0308 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Summer);       // 0x0358 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Autumn);       // 0x03A8 (size: 0x50)
    TMap<UUDS_Weather_Settings_C*, double> Weather Type Probabilities (Winter);       // 0x03F8 (size: 0x50)
    double Total Sphere Bounds;                                                       // 0x0448 (size: 0x8)
    FVector Spline Bounds Center;                                                     // 0x0450 (size: 0x18)
    bool Show Weather Label in Editor;                                                // 0x0468 (size: 0x1)
    class UUDS_Weather_Settings_C* Weather State;                                     // 0x0470 (size: 0x8)
    TArray<FVector> World Space Triangles;                                            // 0x0478 (size: 0x10)
    FVector Canvas Scale Factor;                                                      // 0x0488 (size: 0x18)
    FVector Canvas Offset;                                                            // 0x04A0 (size: 0x18)
    FLinearColor Volume Color;                                                        // 0x04B8 (size: 0x10)
    TArray<FCanvasUVTri> Canvas Space Triangles;                                      // 0x04C8 (size: 0x10)
    bool Started;                                                                     // 0x04D8 (size: 0x1)
    FVector Material State Buffer;                                                    // 0x04E0 (size: 0x18)
    bool Transitioning Weather;                                                       // 0x04F8 (size: 0x1)
    class UUDS_Weather_Settings_C* Transition Weather A;                              // 0x0500 (size: 0x8)
    class UUDS_Weather_Settings_C* Transition Weather B;                              // 0x0508 (size: 0x8)
    double Transition Time Remaining;                                                 // 0x0510 (size: 0x8)
    bool Changing to Random Weather;                                                  // 0x0518 (size: 0x1)
    bool Changing from Random Weather;                                                // 0x0519 (size: 0x1)
    double Transition Timer Length;                                                   // 0x0520 (size: 0x8)
    FString Random Weather Label Text;                                                // 0x0528 (size: 0x10)
    TArray<FVector> Runtime Spline Points;                                            // 0x0538 (size: 0x10)
    bool Apply Wind Direction;                                                        // 0x0548 (size: 0x1)
    double Wind Direction;                                                            // 0x0550 (size: 0x8)
    bool Erase Other WOVs;                                                            // 0x0558 (size: 0x1)
    bool Apply Temperature Ranges;                                                    // 0x0559 (size: 0x1)
    FVector2D Summer Temperature Min and Max;                                         // 0x0560 (size: 0x10)
    FVector2D Autumn Temperature Min and Max;                                         // 0x0570 (size: 0x10)
    FVector2D Winter Temperature Min and Max;                                         // 0x0580 (size: 0x10)
    FVector2D Spring Temperature Min and Max;                                         // 0x0590 (size: 0x10)
    TEnumAsByte<UDS_TemperatureType::Type> Temperature Scale;                         // 0x05A0 (size: 0x1)
    bool Material State Needs Check;                                                  // 0x05A1 (size: 0x1)

    void 📘 Weather Override Volumes();
    void Update Material State Buffer();
    void Apply Saved WOV State(FUDW_WOV_State State);
    void Get State for Saving(FUDW_WOV_State& State);
    void Increment Material State();
    void Construct Weather State Objects();
    void Construct Editor Only Weather Labels();
    void UDW End Play(class AActor* Actor, TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Shut Down WOV();
    void Start Up WOV();
    void Sample Point for Current Alpha(FVector Location, double& Alpha);
    void Apply Climate Preset Object(class UUDS_Climate_Preset_C* Climate Preset);
    void Check to Update Temperature Scale();
    void Custom Volume Behavior(double Alpha, class AUltra_Dynamic_Sky_C* UDS, class AUltra_Dynamic_Weather_C* UDW);
    void Initialize Spline Data();
    void Check for Changing Material State to Request Target Redraw();
    void Update Volume Color(class AUltra_Dynamic_Weather_C* UDW);
    void Triangulate Polygon(TArray<FVector2D>& Vertices (Clockwise));
    void Is Point In Triangle(FVector2D Point, FVector2D v1, FVector2D v2, FVector2D V3, bool& Yes);
    void Scale And Place Vertex in Canvas Space(FVector In, FVector2D& Pos, FLinearColor& Color);
    void Add Triangle(FVector Vert 1, FVector Vert 2, FVector Vert 3);
    void Create Canvas Space Triangles(FVector2D Corner Position, double Width, int32 Resolution);
    void Add Quad(FVector Vert 1, FVector Vert 2, FVector Vert 3, FVector Vert 4);
    void Create World Space Drawing Geometry();
    void Calculate Spline Bounds();
    void UserConstructionScript();
    void Change Weather(class UUDS_Weather_Settings_C* New Weather Type, double Time To Transition To New Weather (Seconds));
    void Change to Random Weather Variation(double Time to Transition to Random Weather (Seconds), TEnumAsByte<UDS_RandomWeatherTiming::Type> Random Weather Mode);
    void Start Up Random Weather Component();
    void Apply Current Single Weather();
    void ReceiveBeginPlay();
    void ReceiveTick(float DeltaSeconds);
    void Force Startup();
    void Force Update Current Weather();
    void ReceiveEndPlay(TEnumAsByte<EEndPlayReason::Type> EndPlayReason);
    void Disable Volume();
    void Enable Volume();
    void Force Tick();
    void ExecuteUbergraph_Weather_Override_Volume(int32 EntryPoint);
}; // Size: 0x5A2

#endif
