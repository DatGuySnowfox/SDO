#ifndef UE4SS_SDK_ChaosVehicles_HPP
#define UE4SS_SDK_ChaosVehicles_HPP

#include "ChaosVehicles_enums.hpp"

struct FAnimNode_StageCoachWheelController : public FAnimNode_SkeletalControlBase
{
    int32 WheelSpokeCount;                                                            // 0x00C8 (size: 0x4)
    float MaxAngularVelocity;                                                         // 0x00CC (size: 0x4)
    float ShutterSpeed;                                                               // 0x00D0 (size: 0x4)
    float StageCoachBlend;                                                            // 0x00D4 (size: 0x4)

}; // Size: 0xF0

struct FAnimNode_WheelController : public FAnimNode_SkeletalControlBase
{
}; // Size: 0xE0

struct FBaseSnapshotData
{
    FTransform Transform;                                                             // 0x0000 (size: 0x60)
    FVector LinearVelocity;                                                           // 0x0060 (size: 0x18)
    FVector AngularVelocity;                                                          // 0x0078 (size: 0x18)

}; // Size: 0x90

struct FChaosWheelSetup
{
    TSubclassOf<class UChaosVehicleWheel> WheelClass;                                 // 0x0000 (size: 0x8)
    FName BoneName;                                                                   // 0x0008 (size: 0x8)
    FVector AdditionalOffset;                                                         // 0x0010 (size: 0x18)

}; // Size: 0x28

struct FControlInputs : public FVehicleInputs
{
    bool ParkingEnabled;                                                              // 0x001C (size: 0x1)
    uint8 TransmissionType;                                                           // 0x001D (size: 0x1)
    bool GearUpInput;                                                                 // 0x001E (size: 0x1)
    bool GearDownInput;                                                               // 0x001F (size: 0x1)

}; // Size: 0x20

struct FNetworkVehicleInputs : public FNetworkPhysicsDatas
{
    FControlInputs VehicleInputs;                                                     // 0x0018 (size: 0x20)
    float TransmissionChangeTime;                                                     // 0x0038 (size: 0x4)
    int32 TransmissionCurrentGear;                                                    // 0x003C (size: 0x4)
    int32 TransmissionTargetGear;                                                     // 0x0040 (size: 0x4)

}; // Size: 0x48

struct FNetworkVehicleStates : public FNetworkPhysicsDatas
{
    FVector StateLastVelocity;                                                        // 0x0018 (size: 0x18)
    TArray<float> WheelsOmega;                                                        // 0x0030 (size: 0x10)
    TArray<float> WheelsAngularPosition;                                              // 0x0040 (size: 0x10)
    TArray<float> SuspensionLastDisplacement;                                         // 0x0050 (size: 0x10)
    TArray<float> SuspensionLastSpringLength;                                         // 0x0060 (size: 0x10)
    TArray<float> SuspensionAveragedLength;                                           // 0x0070 (size: 0x10)
    TArray<int32> SuspensionAveragedCount;                                            // 0x0080 (size: 0x10)
    TArray<int32> SuspensionAveragedNum;                                              // 0x0090 (size: 0x10)
    float EngineOmega;                                                                // 0x00A0 (size: 0x4)

}; // Size: 0xA8

struct FVehicleAerofoilConfig
{
    EVehicleAerofoilType AerofoilType;                                                // 0x0000 (size: 0x1)
    FName BoneName;                                                                   // 0x0004 (size: 0x8)
    FVector Offset;                                                                   // 0x0010 (size: 0x18)
    FVector UpAxis;                                                                   // 0x0028 (size: 0x18)
    float Area;                                                                       // 0x0040 (size: 0x4)
    float Camber;                                                                     // 0x0044 (size: 0x4)
    float MaxControlAngle;                                                            // 0x0048 (size: 0x4)
    float StallAngle;                                                                 // 0x004C (size: 0x4)
    float LiftMultiplier;                                                             // 0x0050 (size: 0x4)
    float DragMultiplier;                                                             // 0x0054 (size: 0x4)

}; // Size: 0xB0

struct FVehicleAnimationInstanceProxy : public FAnimInstanceProxy
{
}; // Size: 0x720

struct FVehicleDifferentialConfig
{
    EVehicleDifferential DifferentialType;                                            // 0x0000 (size: 0x1)
    float FrontRearSplit;                                                             // 0x0004 (size: 0x4)

}; // Size: 0x10

struct FVehicleEngineConfig
{
    FRuntimeFloatCurve TorqueCurve;                                                   // 0x0000 (size: 0x88)
    float MaxTorque;                                                                  // 0x0088 (size: 0x4)
    float MaxRPM;                                                                     // 0x008C (size: 0x4)
    float EngineIdleRPM;                                                              // 0x0090 (size: 0x4)
    float EngineBrakeEffect;                                                          // 0x0094 (size: 0x4)
    float EngineRevUpMOI;                                                             // 0x0098 (size: 0x4)
    float EngineRevDownRate;                                                          // 0x009C (size: 0x4)

}; // Size: 0xC8

struct FVehicleInputRateConfig
{
    float RiseRate;                                                                   // 0x0000 (size: 0x4)
    float FallRate;                                                                   // 0x0004 (size: 0x4)
    EInputFunctionType InputCurveFunction;                                            // 0x0008 (size: 0x1)
    FRuntimeFloatCurve UserCurve;                                                     // 0x0010 (size: 0x88)

}; // Size: 0x98

struct FVehicleInputs
{
    float SteeringInput;                                                              // 0x0000 (size: 0x4)
    float ThrottleInput;                                                              // 0x0004 (size: 0x4)
    float BrakeInput;                                                                 // 0x0008 (size: 0x4)
    float PitchInput;                                                                 // 0x000C (size: 0x4)
    float RollInput;                                                                  // 0x0010 (size: 0x4)
    float YawInput;                                                                   // 0x0014 (size: 0x4)
    float HandbrakeInput;                                                             // 0x0018 (size: 0x4)

}; // Size: 0x1C

struct FVehicleReplicatedState : public FVehicleInputs
{
    int32 TargetGear;                                                                 // 0x001C (size: 0x4)
    float ThrottleUp;                                                                 // 0x0020 (size: 0x4)
    float ThrottleDown;                                                               // 0x0024 (size: 0x4)

}; // Size: 0x28

struct FVehicleStabilizeControlConfig
{
    bool Enabled;                                                                     // 0x0000 (size: 0x1)
    float AltitudeHoldZ;                                                              // 0x0004 (size: 0x4)
    float PositionHoldXY;                                                             // 0x0008 (size: 0x4)

}; // Size: 0x18

struct FVehicleSteeringConfig
{
    ESteeringType SteeringType;                                                       // 0x0000 (size: 0x1)
    float AngleRatio;                                                                 // 0x0004 (size: 0x4)
    FRuntimeFloatCurve SteeringCurve;                                                 // 0x0008 (size: 0x88)

}; // Size: 0xD8

struct FVehicleTargetRotationControlConfig
{
    bool Enabled;                                                                     // 0x0000 (size: 0x1)
    bool bRollVsSpeedEnabled;                                                         // 0x0001 (size: 0x1)
    float RollControlScaling;                                                         // 0x0004 (size: 0x4)
    float RollMaxAngle;                                                               // 0x0008 (size: 0x4)
    float PitchControlScaling;                                                        // 0x000C (size: 0x4)
    float PitchMaxAngle;                                                              // 0x0010 (size: 0x4)
    float RotationStiffness;                                                          // 0x0014 (size: 0x4)
    float RotationDamping;                                                            // 0x0018 (size: 0x4)
    float MaxAccel;                                                                   // 0x001C (size: 0x4)
    float AutoCentreRollStrength;                                                     // 0x0020 (size: 0x4)
    float AutoCentrePitchStrength;                                                    // 0x0024 (size: 0x4)
    float AutoCentreYawStrength;                                                      // 0x0028 (size: 0x4)

}; // Size: 0x58

struct FVehicleThrustConfig
{
    EVehicleThrustType ThrustType;                                                    // 0x0000 (size: 0x1)
    FName BoneName;                                                                   // 0x0004 (size: 0x8)
    FVector Offset;                                                                   // 0x0010 (size: 0x18)
    FVector ThrustAxis;                                                               // 0x0028 (size: 0x18)
    float MaxThrustForce;                                                             // 0x0040 (size: 0x4)
    float MaxControlAngle;                                                            // 0x0044 (size: 0x4)

}; // Size: 0xA0

struct FVehicleTorqueControlConfig
{
    bool Enabled;                                                                     // 0x0000 (size: 0x1)
    float YawTorqueScaling;                                                           // 0x0004 (size: 0x4)
    float YawFromSteering;                                                            // 0x0008 (size: 0x4)
    float YawFromRollTorqueScaling;                                                   // 0x000C (size: 0x4)
    float PitchTorqueScaling;                                                         // 0x0010 (size: 0x4)
    float RollTorqueScaling;                                                          // 0x0014 (size: 0x4)
    float RollFromSteering;                                                           // 0x0018 (size: 0x4)
    float RotationDamping;                                                            // 0x001C (size: 0x4)

}; // Size: 0x40

struct FVehicleTransmissionConfig
{
    bool bUseAutomaticGears;                                                          // 0x0000 (size: 0x1)
    bool bUseAutoReverse;                                                             // 0x0001 (size: 0x1)
    float FinalRatio;                                                                 // 0x0004 (size: 0x4)
    TArray<float> ForwardGearRatios;                                                  // 0x0008 (size: 0x10)
    TArray<float> ReverseGearRatios;                                                  // 0x0018 (size: 0x10)
    float ChangeUpRPM;                                                                // 0x0028 (size: 0x4)
    float ChangeDownRPM;                                                              // 0x002C (size: 0x4)
    float GearChangeTime;                                                             // 0x0030 (size: 0x4)
    float TransmissionEfficiency;                                                     // 0x0034 (size: 0x4)

}; // Size: 0x70

struct FWheelSnapshot
{
    float SuspensionOffset;                                                           // 0x0000 (size: 0x4)
    float WheelRotationAngle;                                                         // 0x0004 (size: 0x4)
    float SteeringAngle;                                                              // 0x0008 (size: 0x4)
    float WheelRadius;                                                                // 0x000C (size: 0x4)
    float WheelAngularVelocity;                                                       // 0x0010 (size: 0x4)

}; // Size: 0x14

struct FWheelStatus
{
    bool bInContact;                                                                  // 0x0000 (size: 0x1)
    FVector ContactPoint;                                                             // 0x0008 (size: 0x18)
    FVector HitLocation;                                                              // 0x0020 (size: 0x18)
    TWeakObjectPtr<class UPhysicalMaterial> PhysMaterial;                             // 0x0038 (size: 0x8)
    float NormalizedSuspensionLength;                                                 // 0x0040 (size: 0x4)
    float SpringForce;                                                                // 0x0044 (size: 0x4)
    float SlipAngle;                                                                  // 0x0048 (size: 0x4)
    bool bIsSlipping;                                                                 // 0x004C (size: 0x1)
    float SlipMagnitude;                                                              // 0x0050 (size: 0x4)
    bool bIsSkidding;                                                                 // 0x0054 (size: 0x1)
    float SkidMagnitude;                                                              // 0x0058 (size: 0x4)
    FVector SkidNormal;                                                               // 0x0060 (size: 0x18)
    float DriveTorque;                                                                // 0x0078 (size: 0x4)
    float BrakeTorque;                                                                // 0x007C (size: 0x4)
    bool bABSActivated;                                                               // 0x0080 (size: 0x1)

}; // Size: 0x88

struct FWheeledSnaphotData : public FBaseSnapshotData
{
    int32 SelectedGear;                                                               // 0x0090 (size: 0x4)
    float EngineRPM;                                                                  // 0x0094 (size: 0x4)
    TArray<FWheelSnapshot> WheelSnapshots;                                            // 0x0098 (size: 0x10)

}; // Size: 0xB0

class AWheeledVehiclePawn : public APawn
{
    class USkeletalMeshComponent* Mesh;                                               // 0x0320 (size: 0x8)
    class UChaosVehicleMovementComponent* VehicleMovementComponent;                   // 0x0328 (size: 0x8)

}; // Size: 0x330

class UChaosVehicleMovementComponent : public UPawnMovementComponent
{
    uint8 bReverseAsBrake;                                                            // 0x0158 (size: 0x1)
    uint8 bThrottleAsBrake;                                                           // 0x0158 (size: 0x1)
    float Mass;                                                                       // 0x015C (size: 0x4)
    bool bEnableCenterOfMassOverride;                                                 // 0x0160 (size: 0x1)
    FVector CenterOfMassOverride;                                                     // 0x0168 (size: 0x18)
    float ChassisWidth;                                                               // 0x0180 (size: 0x4)
    float ChassisHeight;                                                              // 0x0184 (size: 0x4)
    float DragCoefficient;                                                            // 0x0188 (size: 0x4)
    float DownforceCoefficient;                                                       // 0x018C (size: 0x4)
    float DragArea;                                                                   // 0x0190 (size: 0x4)
    float DebugDragMagnitude;                                                         // 0x0194 (size: 0x4)
    FVector InertiaTensorScale;                                                       // 0x0198 (size: 0x18)
    float SleepThreshold;                                                             // 0x01B0 (size: 0x4)
    float SleepSlopeLimit;                                                            // 0x01B4 (size: 0x4)
    TArray<FVehicleAerofoilConfig> Aerofoils;                                         // 0x01B8 (size: 0x10)
    TArray<FVehicleThrustConfig> Thrusters;                                           // 0x01C8 (size: 0x10)
    FVehicleTorqueControlConfig TorqueControl;                                        // 0x01D8 (size: 0x40)
    FVehicleTargetRotationControlConfig TargetRotationControl;                        // 0x0218 (size: 0x58)
    FVehicleStabilizeControlConfig StabilizeControl;                                  // 0x0270 (size: 0x18)
    uint8 bRawHandbrakeInput;                                                         // 0x028C (size: 0x1)
    uint8 bRawGearUpInput;                                                            // 0x028C (size: 0x1)
    uint8 bRawGearDownInput;                                                          // 0x028C (size: 0x1)
    uint8 bWasAvoidanceUpdated;                                                       // 0x0290 (size: 0x1)
    uint8 bParkEnabled;                                                               // 0x0294 (size: 0x1)
    class UNetworkPhysicsComponent* NetworkPhysicsComponent;                          // 0x0298 (size: 0x8)
    FVehicleReplicatedState ReplicatedState;                                          // 0x02D8 (size: 0x28)
    float RawSteeringInput;                                                           // 0x0304 (size: 0x4)
    float RawThrottleInput;                                                           // 0x0308 (size: 0x4)
    float RawBrakeInput;                                                              // 0x030C (size: 0x4)
    float RawPitchInput;                                                              // 0x0310 (size: 0x4)
    float RawRollInput;                                                               // 0x0314 (size: 0x4)
    float RawYawInput;                                                                // 0x0318 (size: 0x4)
    float SteeringInput;                                                              // 0x031C (size: 0x4)
    float ThrottleInput;                                                              // 0x0320 (size: 0x4)
    float BrakeInput;                                                                 // 0x0324 (size: 0x4)
    float PitchInput;                                                                 // 0x0328 (size: 0x4)
    float RollInput;                                                                  // 0x032C (size: 0x4)
    float YawInput;                                                                   // 0x0330 (size: 0x4)
    float HandbrakeInput;                                                             // 0x0334 (size: 0x4)
    bool bRequiresControllerForInputs;                                                // 0x0338 (size: 0x1)
    float IdleBrakeInput;                                                             // 0x033C (size: 0x4)
    float StopThreshold;                                                              // 0x0340 (size: 0x4)
    float WrongDirectionThreshold;                                                    // 0x0344 (size: 0x4)
    FVehicleInputRateConfig ThrottleInputRate;                                        // 0x0348 (size: 0x98)
    FVehicleInputRateConfig BrakeInputRate;                                           // 0x03E0 (size: 0x98)
    FVehicleInputRateConfig SteeringInputRate;                                        // 0x0478 (size: 0x98)
    FVehicleInputRateConfig HandbrakeInputRate;                                       // 0x0510 (size: 0x98)
    FVehicleInputRateConfig PitchInputRate;                                           // 0x05A8 (size: 0x98)
    FVehicleInputRateConfig RollInputRate;                                            // 0x0640 (size: 0x98)
    FVehicleInputRateConfig YawInputRate;                                             // 0x06D8 (size: 0x98)
    class AController* OverrideController;                                            // 0x0908 (size: 0x8)

    void SetYawInput(float Yaw);
    void SetUseAutomaticGears(bool bUseAuto);
    void SetThrottleInput(float Throttle);
    void SetTargetGear(int32 GearNum, bool bImmediate);
    void SetSteeringInput(float Steering);
    void SetSleeping(bool bEnableSleep);
    void SetRollInput(float Roll);
    void SetRequiresControllerForInputs(bool bRequiresController);
    void SetPitchInput(float Pitch);
    void SetParked(bool bParked);
    void SetHandbrakeInput(bool bNewHandbrake);
    void SetChangeUpInput(bool bNewGearUp);
    void SetChangeDownInput(bool bNewGearDown);
    void SetBrakeInput(float Brake);
    void ServerUpdateState(float InSteeringInput, float InThrottleInput, float InBrakeInput, float InHandbrakeInput, int32 InCurrentGear, float InRollInput, float InPitchInput, float InYawInput);
    void ResetVehicle();
    bool IsParked();
    void IncreaseThrottleInput(float ThrottleDelta);
    bool GetUseAutoGears();
    float GetThrottleInput();
    int32 GetTargetGear();
    float GetSteeringInput();
    bool GetHandbrakeInput();
    float GetForwardSpeedMPH();
    float GetForwardSpeed();
    int32 GetCurrentGear();
    float GetBrakeInput();
    void EnableSelfRighting(bool InState);
    void DecreaseThrottleInput(float ThrottleDelta);
}; // Size: 0x930

class UChaosVehicleWheel : public UObject
{
    class UStaticMesh* CollisionMesh;                                                 // 0x0028 (size: 0x8)
    EAxleType AxleType;                                                               // 0x0030 (size: 0x1)
    FVector Offset;                                                                   // 0x0038 (size: 0x18)
    float WheelRadius;                                                                // 0x0050 (size: 0x4)
    float WheelWidth;                                                                 // 0x0054 (size: 0x4)
    float WheelMass;                                                                  // 0x0058 (size: 0x4)
    float CorneringStiffness;                                                         // 0x005C (size: 0x4)
    float FrictionForceMultiplier;                                                    // 0x0060 (size: 0x4)
    float SideSlipModifier;                                                           // 0x0064 (size: 0x4)
    float SlipThreshold;                                                              // 0x0068 (size: 0x4)
    float SkidThreshold;                                                              // 0x006C (size: 0x4)
    float MaxSteerAngle;                                                              // 0x0070 (size: 0x4)
    bool bAffectedBySteering;                                                         // 0x0074 (size: 0x1)
    bool bAffectedByBrake;                                                            // 0x0075 (size: 0x1)
    bool bAffectedByHandbrake;                                                        // 0x0076 (size: 0x1)
    bool bAffectedByEngine;                                                           // 0x0077 (size: 0x1)
    bool bABSEnabled;                                                                 // 0x0078 (size: 0x1)
    bool bTractionControlEnabled;                                                     // 0x0079 (size: 0x1)
    float MaxWheelspinRotation;                                                       // 0x007C (size: 0x4)
    ETorqueCombineMethod ExternalTorqueCombineMethod;                                 // 0x0080 (size: 0x1)
    FRuntimeFloatCurve LateralSlipGraph;                                              // 0x0088 (size: 0x88)
    FVector SuspensionAxis;                                                           // 0x0110 (size: 0x18)
    FVector SuspensionForceOffset;                                                    // 0x0128 (size: 0x18)
    float SuspensionMaxRaise;                                                         // 0x0140 (size: 0x4)
    float SuspensionMaxDrop;                                                          // 0x0144 (size: 0x4)
    float SuspensionDampingRatio;                                                     // 0x0148 (size: 0x4)
    float WheelLoadRatio;                                                             // 0x014C (size: 0x4)
    float SpringRate;                                                                 // 0x0150 (size: 0x4)
    float SpringPreload;                                                              // 0x0154 (size: 0x4)
    int32 SuspensionSmoothing;                                                        // 0x0158 (size: 0x4)
    float RollbarScaling;                                                             // 0x015C (size: 0x4)
    ESweepShape SweepShape;                                                           // 0x0160 (size: 0x1)
    ESweepType SweepType;                                                             // 0x0161 (size: 0x1)
    float MaxBrakeTorque;                                                             // 0x0164 (size: 0x4)
    float MaxHandBrakeTorque;                                                         // 0x0168 (size: 0x4)
    class UChaosWheeledVehicleMovementComponent* VehicleComponent;                    // 0x0170 (size: 0x8)
    int32 WheelIndex;                                                                 // 0x0178 (size: 0x4)
    float DebugLongSlip;                                                              // 0x017C (size: 0x4)
    float DebugLatSlip;                                                               // 0x0180 (size: 0x4)
    float DebugNormalizedTireLoad;                                                    // 0x0184 (size: 0x4)
    float DebugWheelTorque;                                                           // 0x018C (size: 0x4)
    float DebugLongForce;                                                             // 0x0190 (size: 0x4)
    float DebugLatForce;                                                              // 0x0194 (size: 0x4)
    FVector Location;                                                                 // 0x0198 (size: 0x18)
    FVector OldLocation;                                                              // 0x01B0 (size: 0x18)
    FVector Velocity;                                                                 // 0x01C8 (size: 0x18)

    bool IsInAir();
    float GetWheelRadius();
    float GetWheelAngularVelocity();
    float GetSuspensionOffset();
    FVector GetSuspensionAxis();
    float GetSteerAngle();
    float GetRotationAngularVelocity();
    float GetRotationAngle();
    EAxleType GetAxleType();
}; // Size: 0x2E0

class UChaosWheeledVehicleMovementComponent : public UChaosVehicleMovementComponent
{
    bool bSuspensionEnabled;                                                          // 0x0930 (size: 0x1)
    bool bWheelFrictionEnabled;                                                       // 0x0931 (size: 0x1)
    bool bLegacyWheelFrictionPosition;                                                // 0x0932 (size: 0x1)
    TArray<FChaosWheelSetup> WheelSetups;                                             // 0x0938 (size: 0x10)
    FCollisionResponseContainer WheelTraceCollisionResponses;                         // 0x0948 (size: 0x20)
    bool bMechanicalSimEnabled;                                                       // 0x0968 (size: 0x1)
    FVehicleEngineConfig EngineSetup;                                                 // 0x0970 (size: 0xC8)
    FVehicleDifferentialConfig DifferentialSetup;                                     // 0x0A38 (size: 0x10)
    FVehicleTransmissionConfig TransmissionSetup;                                     // 0x0A48 (size: 0x70)
    FVehicleSteeringConfig SteeringSetup;                                             // 0x0AB8 (size: 0xD8)
    TArray<class UChaosVehicleWheel*> Wheels;                                         // 0x0B90 (size: 0x10)

    void SetWheelSlipGraphMultiplier(int32 WheelIndex, float Multiplier);
    void SetWheelRadius(int32 WheelIndex, float Radius);
    void SetWheelMaxSteerAngle(int32 WheelIndex, float AngleDegrees);
    void SetWheelMaxBrakeTorque(int32 WheelIndex, float Torque);
    void SetWheelHandbrakeTorque(int32 WheelIndex, float Torque);
    void SetWheelFrictionMultiplier(int32 WheelIndex, float Friction);
    void SetWheelClass(int32 WheelIndex, TSubclassOf<class UChaosVehicleWheel> InWheelClass);
    void SetTractionControlEnabled(int32 WheelIndex, bool Enabled);
    void SetTorqueCombineMethod(ETorqueCombineMethod InCombineMethod, int32 WheelIndex);
    void SetSuspensionParams(float Rate, float Damping, float Preload, float MaxRaise, float MaxDrop, int32 WheelIndex);
    void SetSnapshot(const FWheeledSnaphotData& SnapshotIn);
    void SetMaxEngineTorque(float Torque);
    void SetDriveTorque(float DriveTorque, int32 WheelIndex);
    void SetDragCoefficient(float DragCoeff);
    void SetDownforceCoefficient(float DownforceCoeff);
    void SetDifferentialFrontRearSplit(float FrontRearSlpit);
    void SetBrakeTorque(float BrakeTorque, int32 WheelIndex);
    void SetAffectedBySteering(int32 WheelIndex, bool Enabled);
    void SetAffectedByHandbrake(int32 WheelIndex, bool Enabled);
    void SetAffectedByEngine(int32 WheelIndex, bool Enabled);
    void SetAffectedByBrake(int32 WheelIndex, bool Enabled);
    void SetABSEnabled(int32 WheelIndex, bool Enabled);
    FWheelStatus MakeWheelStatus(bool bInContact, FVector& ContactPoint, class UPhysicalMaterial* PhysMaterial, float NormalizedSuspensionLength, float SpringForce, float SlipAngle, bool bIsSlipping, float SlipMagnitude, bool bIsSkidding, float SkidMagnitude, FVector& SkidNormal, float DriveTorque, float BrakeTorque, bool bABSActivated);
    FWheelSnapshot MakeWheelSnapshot(float SuspensionOffset, float WheelRotationAngle, float SteeringAngle, float WheelRadius, float WheelAngularVelocity);
    FWheeledSnaphotData MakeWheeledSnapshot(FTransform Transform, FVector LinearVelocity, FVector AngularVelocity, int32 SelectedGear, float EngineRPM, const TArray<FWheelSnapshot>& WheelSnapshots);
    FWheelStatus GetWheelState(int32 WheelIndex);
    FWheeledSnaphotData GetSnapshot();
    int32 GetNumWheels();
    float GetEngineRotationSpeed();
    float GetEngineMaxRotationSpeed();
    void EnableWheelFriction(bool InState);
    void EnableSuspension(bool InState);
    void EnableMechanicalSim(bool InState);
    void BreakWheelStatus(const FWheelStatus& Status, bool& bInContact, FVector& ContactPoint, class UPhysicalMaterial*& PhysMaterial, float& NormalizedSuspensionLength, float& SpringForce, float& SlipAngle, bool& bIsSlipping, float& SlipMagnitude, bool& bIsSkidding, float& SkidMagnitude, FVector& SkidNormal, float& DriveTorque, float& BrakeTorque, bool& bABSActivated);
    void BreakWheelSnapshot(const FWheelSnapshot& Snapshot, float& SuspensionOffset, float& WheelRotationAngle, float& SteeringAngle, float& WheelRadius, float& WheelAngularVelocity);
    void BreakWheeledSnapshot(const FWheeledSnaphotData& Snapshot, FTransform& Transform, FVector& LinearVelocity, FVector& AngularVelocity, int32& SelectedGear, float& EngineRPM, TArray<FWheelSnapshot>& WheelSnapshots);
}; // Size: 0xC50

class UVehicleAnimationInstance : public UAnimInstance
{
    class UChaosWheeledVehicleMovementComponent* WheeledVehicleComponent;             // 0x0A80 (size: 0x8)

    class AWheeledVehiclePawn* GetVehicle();
}; // Size: 0xA90

#endif
